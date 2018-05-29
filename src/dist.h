#ifndef LISONS_LOCAL_DIST_H
#define LISONS_LOCAL_DIST_H

#include <QtCore>

class Dist
{
public:
  struct FileEntry
  {
    QString md5;
    QString fileName;
  };

public:
  static std::unique_ptr<Dist> fromManifestFile(QFile& file, QDir& dir, const QString& suffix);
  bool isValid();
  bool changeSuffixOverwriting(const QString& newSuffix);
  void remove();

public:
  QDir& dir;
  QString fileNameSuffix;
  QByteArray md5;
  QVector<FileEntry> entries;

private:
  Dist(QDir& dir, const QString& suffix, QByteArray& md5);
};

bool
operator==(const Dist& lhs, const Dist& rhs);

#endif // LISONS_LOCAL_DIST_H
