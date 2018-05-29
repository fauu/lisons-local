#include "dist.h"
#include "file_md5.h"

#include <QDebug>

static const char* const COLUMN_SEPARATOR = " ";

std::unique_ptr<Dist>
Dist::fromManifestFile(QFile &file, const QString &suffix)
{
  if (!file.isOpen() && !file.open(QIODevice::ReadOnly)) {
    return nullptr;
  }

  auto manifest = std::make_unique<Dist>();
  manifest->fileNameSuffix = suffix;
  file.seek(0);
  manifest->md5 = fileMd5(file);

  file.seek(0);
  QTextStream in(&file);
  while (!in.atEnd()) {
    QString line = in.readLine();
    QStringList fields = line.split(QLatin1String(COLUMN_SEPARATOR));
    if (fields.size() != 2) {
      return nullptr;
    }
    manifest->entries.push_back({ fields[0], fields[1] });
  }

  return manifest->entries.empty() ? nullptr : std::move(manifest);
}

bool
operator==(const Dist& lhs, const Dist& rhs)
{
  return lhs.md5.size() > 0 && rhs.md5.size() > 0 && lhs.md5 == rhs.md5;
}