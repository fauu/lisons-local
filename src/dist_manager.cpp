#include "dist_manager.h"
#include "file_md5.h"

#include <QStandardPaths>

static const char* const BASE_URL = "https://raw.githubusercontent.com/fauu/lisons/pwa/web/";
static const char* const MANIFEST_FILE_NAME = "manifest.txt";
static const char* const MANIFEST_COLUMN_SEPARATOR = " ";
static const char* const NEW_FILE_SUFFIX = ".new";

DistManager::DistManager(QObject* parent, const QDir& saveDir)
  : QObject(parent)
  , mDistDir(saveDir)
{
  QFile manifestFile(mDistDir.absoluteFilePath(QLatin1String(MANIFEST_FILE_NAME)));
  mCurrManifest = readManifest(manifestFile, QLatin1String(""));
}

void
DistManager::start()
{
  if (!mDistDir.exists()) {
    mDistDir.mkpath(".");
  }
  enqueueDownload(QLatin1String(MANIFEST_FILE_NAME));
  QTimer::singleShot(0, this, &DistManager::startNextDownload);
  emit stateChanged(DistManagerState::DownloadingManifest);
}

void
DistManager::enqueueDownload(const QString &fileName)
{
  auto url = QUrl(QLatin1String(BASE_URL) + fileName);
  mDownloadQueue.enqueue(url);
}

std::unique_ptr<Manifest>
DistManager::readManifest(QFile& file, const QString& suffix)
{
  if (!file.isOpen() && !file.open(QIODevice::ReadOnly)) {
    return nullptr;
  }
  auto manifest = std::make_unique<Manifest>();
  manifest->suffix = suffix;
  file.seek(0);
  manifest->md5 = fileMd5(file);
  file.seek(0);
  QTextStream in(&file);
  while (!in.atEnd()) {
    QString line = in.readLine();
    QStringList fields = line.split(QLatin1String(MANIFEST_COLUMN_SEPARATOR));
    if (fields.size() != 2) {
      return nullptr;
    }
    manifest->entries.push_back({ fields[0], fields[1] });
  }
  return manifest->entries.empty() ? nullptr : std::move(manifest);
}

bool
DistManager::verifyDist(std::unique_ptr<Manifest> const &manifest)
{
  if (!manifest) {
    return false;
  }
  for (const ManifestEntry& entry : manifest->entries) {
    QString entryFilePath = mDistDir.absoluteFilePath(entry.fileName) + manifest->suffix;
    QFile entryFile(entryFilePath);
    QString checksum = fileMd5(entryFile).toHex();
    if (entry.md5 != checksum) {
      return false;
    }
  }
  return true;
}

void
DistManager::deleteNewDist()
{
  mDistDir.setNameFilters(QStringList() << "*" + QLatin1String(NEW_FILE_SUFFIX));
  mDistDir.setFilter(QDir::Files);
  for (const QString& dirEntry : mDistDir.entryList()) {
    if (!mDistDir.remove(dirEntry)) {
      qDebug() << "Could not remove" << dirEntry;
    }
  }
}

void
DistManager::overwriteCurrDist()
{
  if (!mNewManifest) {
    return;
  }
  mNewManifest->entries.push_back({ "", QLatin1String(MANIFEST_FILE_NAME) });
  for (const ManifestEntry& entry : mNewManifest->entries) {
    QString entryFilePath = mDistDir.absoluteFilePath(entry.fileName);
    if (QFile::exists(entryFilePath)) {
      if (!QFile::remove(entryFilePath)) {
        qWarning() << "Could not remove" << entryFilePath;
      }
    }
    if (!QFile::rename(entryFilePath + mNewManifest->suffix, entryFilePath)) {
      qWarning() << "Could not rename" << entryFilePath;
    }
  }
}

void
DistManager::startNextDownload()
{
  if (mDownloadQueue.isEmpty()) {
    return;
  }
  QUrl url = mDownloadQueue.dequeue();
  QString fileName = QFileInfo(url.path()).fileName() + QLatin1String(NEW_FILE_SUFFIX);
  QString savePath = mDistDir.absoluteFilePath(fileName);
  mOutputFile.setFileName(savePath);
  if (!mOutputFile.open(QIODevice::ReadWrite)) {
    qWarning() << "Could not open" << savePath << "for writing:" << mOutputFile.errorString();
    if (verifyDist(mCurrManifest)) {
      emit stateChanged(DistManagerState::CouldNotUpdateButDistValid);
    } else {
      emit stateChanged(DistManagerState::DistInvalid);
    }
    deleteNewDist();
    return;
  }

  QNetworkRequest request(url);
  mCurrentDownload = mNetworkAccessManager.get(request);
  connect(mCurrentDownload, &QNetworkReply::finished, this, &DistManager::downloadFinished);
  connect(mCurrentDownload, &QNetworkReply::readyRead, this, &DistManager::downloadReadyRead);
  qDebug() << "Downloading:" << url.toEncoded().constData();
}

void
DistManager::downloadFinished()
{
  if (mCurrentDownload->error()) {
    qDebug() << "File download failed:" << mCurrentDownload->errorString();
    if (verifyDist(mCurrManifest)) {
      emit stateChanged(DistManagerState::CouldNotUpdateButDistValid);
    } else {
      emit stateChanged(DistManagerState::DistInvalid);
    }
    mCurrentDownload->deleteLater();
    mOutputFile.remove();
    return;
  } else {
    auto fileInfo = QFileInfo(mOutputFile);

    if (!mNewManifest) {
      mNewManifest = readManifest(mOutputFile, QLatin1String(NEW_FILE_SUFFIX));
      if (!mNewManifest) {
        // TODO: DRY startNextDownload()
        if (verifyDist(mCurrManifest)) {
          emit stateChanged(DistManagerState::CouldNotUpdateButDistValid);
        } else {
          emit stateChanged(DistManagerState::DistInvalid);
        }
        deleteNewDist();
      } else {
        if (!mCurrManifest || mNewManifest->md5 != mCurrManifest->md5 || !verifyDist(mCurrManifest)) {
          for (const ManifestEntry& entry : mNewManifest->entries) {
            enqueueDownload(entry.fileName);
          }
          emit stateChanged(DistManagerState::DownloadingDistFiles);
        } else {
          emit stateChanged(DistManagerState::UpToDateAndDistValid);
          deleteNewDist();
          return;
        }
      }
    }

    qDebug() << "Downloaded:" << fileInfo.absoluteFilePath();
    mCurrentDownload->deleteLater();
    mOutputFile.close();

    if (mDownloadQueue.isEmpty()) {
      qDebug() << "Download queue is now empty";
      // TODO: DRY
      if (verifyDist(mNewManifest)) {
        overwriteCurrDist();
        emit stateChanged(DistManagerState::UpToDateAndDistValid);
      } else {
        if (verifyDist(mCurrManifest)) {
          emit stateChanged(DistManagerState::CouldNotUpdateButDistValid);
        } else {
          emit stateChanged(DistManagerState::DistInvalid);
        }
        deleteNewDist();
      }
    } else {
      startNextDownload();
    }
  }
}

void
DistManager::downloadReadyRead()
{
  mOutputFile.write(mCurrentDownload->readAll());
}
