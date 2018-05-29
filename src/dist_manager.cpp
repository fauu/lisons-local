#include "dist_manager.h"
#include "dist_manifest.h"
#include "file_md5.h"

#include <QStandardPaths>

static const char* const BASE_URL = "https://raw.githubusercontent.com/fauu/lisons/pwa/web/";
static const char* const MANIFEST_FILE_NAME = "manifest.txt";
static const char* const NEW_FILE_SUFFIX = ".new";

DistManager::DistManager(QObject* parent, const QDir& saveDir)
  : QObject(parent)
  , mDistDir(saveDir)
{
  QString DistManifestPath = mDistDir.absoluteFilePath(QLatin1String(MANIFEST_FILE_NAME));
  QFile DistManifestFile(DistManifestPath);
  mCurrDistManifest = DistManifest::fromFile(DistManifestFile, QLatin1String(""));
}

void
DistManager::updateDist()
{
  if (!mDistDir.exists()) {
    mDistDir.mkpath(".");
  }
  enqueueDownload(QLatin1String(MANIFEST_FILE_NAME));
  QTimer::singleShot(0, this, &DistManager::startNextDownload);
  emit stateChanged(DistManagerState::DownloadingDistManifest);
}

void
DistManager::enqueueDownload(const QString& fileName)
{
  auto url = QUrl(QLatin1String(BASE_URL) + fileName);
  mDownloadQueue.enqueue(url);
}

bool
DistManager::verifyDist(std::unique_ptr<DistManifest> const& manifest)
{
  if (!manifest) {
    return false;
  }
  for (const DistManifest::Entry& entry : manifest->entries) {
    QString entryFilePath = mDistDir.absoluteFilePath(entry.fileName) + manifest->fileNameSuffix;
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

bool
DistManager::overwriteCurrDist()
{
  if (!mNewDistManifest) {
    return false;
  }

  // Add the new manifest itself as an entry so that we overwrite the old one with it as well
  mNewDistManifest->entries.push_back({ "", QLatin1String(MANIFEST_FILE_NAME) });
  for (const DistManifest::Entry& entry : mNewDistManifest->entries) {
    QString entryFilePath = mDistDir.absoluteFilePath(entry.fileName);
    if (QFile::exists(entryFilePath) && !QFile::remove(entryFilePath)) {
      qWarning() << "Could not remove" << entryFilePath;
      return false;
    }
    if (!QFile::rename(entryFilePath + mNewDistManifest->fileNameSuffix, entryFilePath)) {
      qWarning() << "Could not rename" << entryFilePath;
      return false;
    }
  }

  return true;
}

void
DistManager::fallBackToCurrDist()
{
  if (verifyDist(mCurrDistManifest)) {
    emit stateChanged(DistManagerState::CouldNotUpdateButDistValid);
  } else {
    emit stateChanged(DistManagerState::DistInvalid);
  }
  deleteNewDist();
}

void
DistManager::startNextDownload()
{
  if (mDownloadQueue.isEmpty()) {
    return;
  }

  QUrl url = mDownloadQueue.dequeue();
  QString fileName = QFileInfo(url.path()).fileName() + QLatin1String(NEW_FILE_SUFFIX);
  QString filePath = mDistDir.absoluteFilePath(fileName);
  mOutputFile.setFileName(filePath);
  if (!mOutputFile.open(QIODevice::ReadWrite)) {
    qWarning() << "Could not open" << filePath << "for writing:" << mOutputFile.errorString();
    fallBackToCurrDist();
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
  mCurrentDownload->deleteLater();
  if (mCurrentDownload->error()) {
    qDebug() << "File download failed:" << mCurrentDownload->errorString();
    fallBackToCurrDist();
    mOutputFile.remove();
    return;
  }

  if (!mNewDistManifest) {
    // Assumption: if we don't have the new DistManifest data yet then the file is the new manifest
    mNewDistManifest = DistManifest::fromFile(mOutputFile, QLatin1String(NEW_FILE_SUFFIX));

    if (!mNewDistManifest) {
      // Can't read the downloaded manifest file
      fallBackToCurrDist();
      mOutputFile.remove();
      return;
    }

    if (*mNewDistManifest == *mCurrDistManifest && verifyDist(mCurrDistManifest)) {
      // We already have the latest version
      emit stateChanged(DistManagerState::UpToDateAndDistValid);
      mOutputFile.remove();
      return;
    }

    for (const DistManifest::Entry& entry : mNewDistManifest->entries) {
      enqueueDownload(entry.fileName);
    }
    emit stateChanged(DistManagerState::DownloadingDistFiles);
  }

  qDebug() << "Downloaded:" << QFileInfo(mOutputFile).absoluteFilePath();
  mOutputFile.close();

  if (!mDownloadQueue.isEmpty()) {
    startNextDownload();
    return;
  }

  qDebug() << "Download queue is now empty";
  if (verifyDist(mNewDistManifest) && overwriteCurrDist()) {
    // We've successfully committed the downloaded version
    emit stateChanged(DistManagerState::UpToDateAndDistValid);
    return;
  }

  fallBackToCurrDist();
}

void
DistManager::downloadReadyRead()
{
  mOutputFile.write(mCurrentDownload->readAll());
}
