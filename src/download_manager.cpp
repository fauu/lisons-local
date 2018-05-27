#include "download_manager.h"
#include "file_md5.h"

#include <QStandardPaths>

static const char* const BASE_URL = "https://raw.githubusercontent.com/fauu/lisons/pwa/web/";
static const char* const MANIFEST_FILE_NAME = "manifest.txt";
static const char* const NEW_FILE_SUFFIX = ".new";

DownloadManager::DownloadManager(QObject* parent, const QString& savePath)
  : QObject(parent)
  , mDownloadDir(savePath)
  , mManifestPath(mDownloadDir.absoluteFilePath(QLatin1String(MANIFEST_FILE_NAME)))
  , mNewManifestFileName(QLatin1String(MANIFEST_FILE_NAME) + QLatin1String(NEW_FILE_SUFFIX))
{
  QFile manifestFile(mManifestPath);
  mManifest = readManifest(manifestFile);
}

void
DownloadManager::start()
{
  if (!mDownloadDir.exists()) {
    mDownloadDir.mkpath(".");
  }
    enqueue(QLatin1String(MANIFEST_FILE_NAME));
    QTimer::singleShot(0, this, &DownloadManager::startNextDownload);
  emit stateChanged(DownloadManagerState::DownloadingManifest);
}

void
DownloadManager::enqueue(const QString& fileName)
{
  auto url = QUrl(QLatin1String(BASE_URL) + fileName);
  mDownloadQueue.enqueue(url);
}

std::unique_ptr<Manifest>
DownloadManager::readManifest(QFile& file)
{
  auto manifest = std::make_unique<Manifest>();
  if (!file.isOpen() && !file.open(QIODevice::ReadOnly)) {
    return nullptr;
  }
  manifest->md5 = fileMd5(file);
  file.seek(0);
  QTextStream in(&file);
  while (!in.atEnd()) {
    QString line = in.readLine();
    QStringList fields = line.split(" ");
    if (fields.size() != 2) {
      return nullptr;
    }
    manifest->entries.push_back({ fields[0], fields[1] });
  }
  return manifest->entries.empty() ? nullptr : std::move(manifest);
}

// TODO: Make options an enum? Check if two-element enums are "recommended"
bool
DownloadManager::verifyPackage(bool newOne)
{
  // TODO: TODON'T
  Manifest* manifest = (newOne ? mNewManifest : mManifest).get();
  if (!manifest || manifest->entries.isEmpty()) {
    return false;
  }
  for (const auto& entry : manifest->entries) {
    QString entryFilePath = mDownloadDir.absoluteFilePath(entry.fileName);
    if (newOne) {
      entryFilePath += NEW_FILE_SUFFIX;
    }
    QFile entryFile(entryFilePath);
    QString checksum = fileMd5(entryFile).toHex();
    if (entry.md5 != checksum) {
      delete manifest;
      return false;
    }
  }
  return true;
}

void
DownloadManager::deleteNewPackage()
{
  mDownloadDir.setNameFilters(QStringList() << "*" + QLatin1String(NEW_FILE_SUFFIX));
  mDownloadDir.setFilter(QDir::Files);
  for (const auto& dirFile : mDownloadDir.entryList()) {
    mDownloadDir.remove(dirFile);
  }
}

void
DownloadManager::overwritePackage()
{
  // TODO: Error handling?
  mNewManifest->entries.push_back({ "", QLatin1String(MANIFEST_FILE_NAME) });
  for (const auto& entry : mNewManifest->entries) {
    QString entryFilePath = mDownloadDir.absoluteFilePath(entry.fileName);
    if (QFile::exists(entryFilePath)) {
      QFile::remove(entryFilePath);
    }
    QFile::rename(entryFilePath + QLatin1String(NEW_FILE_SUFFIX), entryFilePath);
  }
}

void
DownloadManager::startNextDownload()
{
  if (mDownloadQueue.isEmpty()) {
    qDebug() << "Finished downloading";
    // TODO: DRY
    if (verifyPackage(true)) {
      overwritePackage();
      emit stateChanged(DownloadManagerState::UpToDateAndPackageValid);
    } else {
      if (verifyPackage()) {
        emit stateChanged(DownloadManagerState::CouldNotUpdateButPackageValid);
      } else {
        emit stateChanged(DownloadManagerState::PackageInvalid);
      }
      deleteNewPackage();
    }
    // TODO: remove finished() signal?
    emit finished();
    return;
  }

  QUrl url = mDownloadQueue.dequeue();
  QString fileName = QFileInfo(url.path()).fileName() + QLatin1String(NEW_FILE_SUFFIX);
  QString savePath = mDownloadDir.absoluteFilePath(fileName);
  mOutputFile.setFileName(savePath);
  if (!mOutputFile.open(QIODevice::ReadWrite)) {
    qWarning() << "Could not open '" << savePath << "' for writing: " << mOutputFile.errorString();
    if (verifyPackage()) {
      emit stateChanged(DownloadManagerState::CouldNotUpdateButPackageValid);
    } else {
      emit stateChanged(DownloadManagerState::PackageInvalid);
    }
    deleteNewPackage();
    // XXX: do I need to delete something here after giving up or what?
    return;
  }

  QNetworkRequest request(url);
  mCurrentDownload = mNetworkAccessManager.get(request);
  connect(mCurrentDownload, &QNetworkReply::finished, this, &DownloadManager::downloadFinished);
  connect(mCurrentDownload, &QNetworkReply::readyRead, this, &DownloadManager::downloadReadyRead);

  qDebug() << "Downloading" << url.toEncoded().constData();
}

void
DownloadManager::downloadFinished()
{
  if (mCurrentDownload->error()) {
    qDebug() << "File download failed";
    if (verifyPackage()) {
      emit stateChanged(DownloadManagerState::CouldNotUpdateButPackageValid);
    } else {
      emit stateChanged(DownloadManagerState::PackageInvalid);
    }
    mCurrentDownload->deleteLater();
    mOutputFile.remove();
  } else {
    auto fileInfo = QFileInfo(mOutputFile);

    if (fileInfo.fileName() == mNewManifestFileName) {
      qDebug() << "Downloaded new manifest file";

      mNewManifest = readManifest(mOutputFile);
      if (!mNewManifest) {
        // TODO: DRY startNextDownload()
        if (verifyPackage()) {
          emit stateChanged(DownloadManagerState::CouldNotUpdateButPackageValid);
        } else {
          emit stateChanged(DownloadManagerState::PackageInvalid);
        }
        deleteNewPackage();
      } else {
        // BUG: md5s are different somehow?
        if (mManifest && mNewManifest->md5 == mManifest->md5 && verifyPackage()) {
          emit stateChanged(DownloadManagerState::UpToDateAndPackageValid);
        } else {
          for (const auto& entry : mNewManifest->entries) {
            enqueue(entry.fileName);
          }
          emit stateChanged(DownloadManagerState::DownloadingLisons);
        }
      }
    }

    qDebug() << "File download succeeded";
    mCurrentDownload->deleteLater();
    mOutputFile.close();
    startNextDownload();
  }
}

void
DownloadManager::downloadReadyRead()
{
  mOutputFile.write(mCurrentDownload->readAll());
}
