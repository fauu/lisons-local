#include "dist.h"
#include "file_md5.h"

#include <QDebug>

static const char* const COLUMN_SEPARATOR = " ";
// TODO: DRY dist_manager.cpp
static const char* const MANIFEST_FILE_NAME = "manifest.txt";

Dist::Dist(QDir& dir, const QString& suffix, QByteArray& md5)
  : dir(dir)
  , fileNameSuffix(suffix)
  , md5(md5)
{}

std::unique_ptr<Dist>
Dist::fromManifestFile(QFile& file, QDir& dir, const QString& suffix)
{
  if (!file.isOpen() && !file.open(QIODevice::ReadOnly)) {
    return nullptr;
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
    dist.entries.push_back({ fields[0], fields[1] });
  }

  return dist.entries.empty() ? nullptr : std::make_unique<Dist>(dist);
}

bool
Dist::isValid()
{
  for (const FileEntry& entry : entries) {
    QString entryFilePath = dir.absoluteFilePath(entry.fileName) + fileNameSuffix;
    QFile entryFile(entryFilePath);
    QString checksum = fileMd5(entryFile).toHex();
    if (entry.md5 != checksum) {
      return false;
    }
  }
  return true;
}

bool
Dist::changeSuffixOverwriting(const QString& newSuffix)
{
  // Add the new manifest itself as an entry so that we overwrite the old one with it as well
  entries.push_back({ "", QLatin1String(MANIFEST_FILE_NAME) });
  for (const FileEntry& entry : entries) {
    QString entryFilePath = dir.absoluteFilePath(entry.fileName) + newSuffix;
    if (QFile::exists(entryFilePath) && !QFile::remove(entryFilePath)) {
      qWarning() << "Could not remove" << entryFilePath;
      return false;
    }
    if (!QFile::rename(entryFilePath, entryFilePath + newSuffix)) {
      qWarning() << "Could not rename" << entryFilePath;
      return false;
    }
  }

  return true;
}


void
Dist::remove()
{
  if (fileNameSuffix.isEmpty()) {
    dir.setNameFilters(QStringList() << "*.*");
  } else {
    dir.setNameFilters(QStringList() << "*" + fileNameSuffix);
  }
  dir.setFilter(QDir::Files);
  for (const QString& dirEntry : dir.entryList()) {
    if (!dir.remove(dirEntry)) {
      qDebug() << "Could not remove" << dirEntry;
    }
  }
}

bool
operator==(const Dist& lhs, const Dist& rhs)
{
  return lhs.md5.size() > 0 && rhs.md5.size() > 0 && lhs.md5 == rhs.md5;
}
