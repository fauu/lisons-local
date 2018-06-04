#ifndef LISONS_LOCAL_DIST_H
#define LISONS_LOCAL_DIST_H

#include <QtCore>

class Dist
{
public:
  static std::unique_ptr<Dist> fromManifestFile(QFile& file, QDir& dir, const QString& suffix);
  bool isValid();
  bool overwrite(const Dist& other);
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
  Dist(QDir& dir, const QString& suffix, QByteArray& md5);

private:
  QDir& mDir;
  QString mSuffix;
  QByteArray mMd5;
  QVector<FileEntry> mEntries;
};

bool
operator==(const Dist& lhs, const Dist& rhs);

#endif // LISONS_LOCAL_DIST_H
