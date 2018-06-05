#ifndef LISONS_LOCAL_DIST_H
#define LISONS_LOCAL_DIST_H

#include <QtCore>

namespace Lisons {

static const char* const MANIFEST_FILE_NAME = "manifest.txt";

class Dist
{
public:
  static std::unique_ptr<Dist> fromManifestFile(QFile& file, QDir& dir, QString suffix);
  bool isValid();
  bool changeSuffix(const QString& newSuffix);
  void remove();
  QVector<QString> entryFileNames() const;
  const QString& suffix() const;
  QByteArray md5() const;

private:
  struct FileEntry
  {
    QString md5;
    QString fileName;
  };

private:
  Dist(QDir& dir, QString suffix, QByteArray md5);

private:
  QDir& mDir;
  QString mSuffix;
  const QByteArray mMd5;
  QVector<FileEntry> mEntries;
};

bool
operator==(const Dist& lhs, const Dist& rhs);
}

#endif // LISONS_LOCAL_DIST_H
