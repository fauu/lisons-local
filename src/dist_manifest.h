#ifndef LISONS_LOCAL_DIST_MANIFEST_H
#define LISONS_LOCAL_DIST_MANIFEST_H

#include <QtCore>

class DistManifest
{
public:
  struct Entry
  {
    QString md5;
    QString fileName;
  };

public:
  static std::unique_ptr<DistManifest> fromFile(QFile& file, const QString& suffix);

public:
  QString fileNameSuffix;
  QByteArray md5;
  QVector<Entry> entries;
};

#endif // LISONS_LOCAL_DIST_MANIFEST_H
