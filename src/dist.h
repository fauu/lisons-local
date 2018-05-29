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
  static std::unique_ptr<Dist> fromManifestFile(QFile &file, const QString &suffix);

public:
  QString fileNameSuffix;
  QByteArray md5;
  QVector<FileEntry> entries;
};

bool
operator==(const Dist& lhs, const Dist& rhs);

#endif // LISONS_LOCAL_DIST_H
