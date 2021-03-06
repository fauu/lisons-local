#include "file_md5.h"

#include <QCryptographicHash>
#include <QtCore>

namespace Lisons {

QByteArray
fileMd5(QFile& file)
{
  if (file.isOpen() || file.open(QFile::ReadOnly)) {
    QCryptographicHash hash(QCryptographicHash::Algorithm::Md5);
    if (hash.addData(&file)) {
      return hash.result();
    }
  }
  return QByteArray();
}
}
