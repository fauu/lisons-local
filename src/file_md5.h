#ifndef LISONS_LOCAL_FILE_MD5_H
#define LISONS_LOCAL_FILE_MD5_H

#include <QtCore>

namespace Lisons {

QByteArray
fileMd5(QFile& file);
}

#endif // LISONS_LOCAL_FILE_MD5_H
