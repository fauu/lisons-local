#include "dist.h"
#include "file_md5.h"

#include <QDebug>

static const char* const COLUMN_SEPARATOR = " ";
// TODO: DRY dist_manager.cpp
static const char* const MANIFEST_FILE_NAME = "manifest.txt";

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
  if (mMd5.size() == 1) {
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


// TODO: Rename to `overwrite`?
bool
Dist::overwrite(const Dist& other)
{
  for (const QString& entryFileName : other.entryFileNames()) {
    QString entryFilePath = mDir.absoluteFilePath(entryFileName) + other.suffix();
    if (QFile::exists(entryFilePath) && !QFile::remove(entryFilePath)) {
      qWarning() << "Could not remove" << entryFilePath;
      return false;
    }
  }
  QString manifestFilePath = mDir.absoluteFilePath(QLatin1String(MANIFEST_FILE_NAME)) + other.suffix();
  if (QFile::exists(manifestFilePath) && !QFile::remove(manifestFilePath)) {
    qWarning() << "Could not remove" << manifestFilePath;
    return false;
  }

  // Add the new manifest itself as an entry so that we rename it inside the loop
  mEntries.push_back({ "", QLatin1String(MANIFEST_FILE_NAME) });
  for (const QString& entryFileName : entryFileNames()) {
    QString entryFilePathNoSuffix = mDir.absoluteFilePath(entryFileName);
    if (!QFile::rename(entryFilePathNoSuffix + mSuffix, entryFilePathNoSuffix + other.suffix())) {
      qWarning() << "Could not rename" << entryFilePathNoSuffix + mSuffix;
      return false;
    }
  }

  mSuffix = other.suffix();
  return true;
}


void
Dist::remove()
{
  if (mSuffix.isEmpty()) {
    mDir.setNameFilters(QStringList() << "*.*");
  } else {
    mDir.setNameFilters(QStringList() << "*" + mSuffix);
  }
  mDir.setFilter(QDir::Files);
  for (const QString& dirEntry : mDir.entryList()) {
    if (!mDir.remove(dirEntry)) {
      qDebug() << "Could not remove" << dirEntry;
    }
  }
}

QVector<QString>
Dist::entryFileNames() const {
  QVector<QString> fileNames;
  for (const FileEntry& entry : mEntries) {
    fileNames.append(entry.fileName);
  }
  return fileNames;
}

const QString&
Dist::suffix() const {
  return mSuffix;
}

QByteArray
Dist::md5() const {
  return mMd5;
}

bool
operator==(const Dist& lhs, const Dist& rhs)
{
  return lhs.md5().size() > 0 && rhs.md5().size() > 0 && lhs.md5() == rhs.md5();
}
