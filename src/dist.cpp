#include "dist.h"
#include "file_md5.h"

#include <QDebug>

namespace Lisons {

static const char* const COLUMN_SEPARATOR = " ";

Dist::Dist(QDir& dir, const QString suffix, const QByteArray md5)
  : mDir(dir)
  , mSuffix(suffix)
  , mMd5(md5)
{}

std::unique_ptr<Dist>
Dist::fromManifestFile(QFile& file, QDir& dir, const QString suffix)
{
  if (!file.isOpen() && !file.open(QIODevice::ReadOnly)) {
    return std::make_unique<Dist>(Dist(dir, suffix, QByteArrayLiteral("\x00")));
  }

  file.seek(0);
  QByteArray md5 = fileMd5(file);
  Dist dist(dir, suffix, md5);

  file.seek(0);
  QTextStream in(&file);
  while (!in.atEnd()) {
    QString line = in.readLine();
    QStringList fields = line.split(QLatin1String(COLUMN_SEPARATOR));
    if (fields.size() != 2) {
      return nullptr;
    }
    dist.mEntries.push_back({ fields[0], fields[1] });
  }

  return std::make_unique<Dist>(dist);
}

bool
Dist::isValid()
{
  if (mMd5.size() == 1 && mMd5[0] == '\x00') {
    return false;
  }
  for (const FileEntry& entry : mEntries) {
    QString entryFilePath = mDir.absoluteFilePath(entry.fileName) + mSuffix;
    QFile entryFile(entryFilePath);
    QString checksum = fileMd5(entryFile).toHex();
    if (entry.md5 != checksum) {
      return false;
    }
  }
  return true;
}

bool
Dist::changeSuffix(const QString& newSuffix)
{
  // Add the new manifest itself as an entry so that we rename it inside the loop
  mEntries.push_back({ "", QLatin1String(MANIFEST_FILE_NAME) });
  for (const QString& entryFileName : entryFileNames()) {
    QString entryFilePathNoSuffix = mDir.absoluteFilePath(entryFileName);
    QString oldFilePath = entryFilePathNoSuffix + mSuffix;
    QString newFilePath = entryFilePathNoSuffix + newSuffix;
    if (!QFile::rename(oldFilePath, newFilePath)) {
      qWarning() << "Could not rename" << oldFilePath << "to" << newFilePath;
      return false;
    }
    qDebug() << "Renamed" << oldFilePath << "to" << newFilePath;
  }

  mSuffix = newSuffix;
  return true;
}

void
Dist::remove()
{
  mDir.setNameFilters(QStringList() << QString("*.%1").arg(mSuffix.isEmpty() ? "*" : mSuffix));
  mDir.setFilter(QDir::Files);
  for (const QString& dirEntry : mDir.entryList()) {
    if (!mDir.remove(dirEntry)) {
      qDebug() << "Could not delete" << dirEntry;
    } else {
      qDebug() << "Deleted" << dirEntry; 
    }
  }
}

QVector<QString>
Dist::entryFileNames() const
{
  QVector<QString> fileNames;
  for (const FileEntry& entry : mEntries) {
    fileNames.append(entry.fileName);
  }
  return fileNames;
}

const QString&
Dist::suffix() const
{
  return mSuffix;
}

QByteArray
Dist::md5() const
{
  return mMd5;
}

bool
operator==(const Dist& lhs, const Dist& rhs)
{
  return lhs.md5() == rhs.md5();
}
}
