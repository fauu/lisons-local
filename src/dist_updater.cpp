#include "dist_updater.h"
#include "dist.h"
#include "file_md5.h"

#include <QStandardPaths>

namespace Lisons {

static const char* const BASE_URL = "https://raw.githubusercontent.com/fauu/lisons/pwa/web/";
static const char* const NEW_FILE_SUFFIX = ".new";

DistUpdater::DistUpdater(QObject* parent, const QDir& saveDir)
  : QObject(parent)
  , mDistDir(saveDir)
{
  QString distManifestPath = mDistDir.absoluteFilePath(QLatin1String(MANIFEST_FILE_NAME));
  QFile distManifestFile(distManifestPath);
  mCurrDist = Dist::fromManifestFile(distManifestFile, mDistDir, QString());
}

void
DistUpdater::updateAndVerify()
{
  if (!mDistDir.exists()) {
    mDistDir.mkpath(".");
  }
  enqueueDownload(QLatin1String(MANIFEST_FILE_NAME));
  QTimer::singleShot(0, this, &DistUpdater::startNextDownload);
  emit stateChanged(DistUpdaterState::DownloadingDistManifest);
}

void
DistUpdater::enqueueDownload(const QString& fileName)
{
  auto url = QUrl(QLatin1String(BASE_URL) + fileName);
  mDownloadQueue.enqueue(url);
}

void
DistUpdater::fallBackToCurrDist()
{
  if (mCurrDist && mCurrDist->isValid()) {
    emit stateChanged(DistUpdaterState::CouldNotUpdateButDistValid);
  } else {
    emit stateChanged(DistUpdaterState::DistInvalid);
  }
  if (mNewDist) {
    mNewDist->remove();
  }
}

bool
DistUpdater::cleanDistDirPreserving(const Dist& distToPreserve)
{
  mDistDir.setNameFilters(QStringList() << QString("*.*"));
  mDistDir.setFilter(QDir::Files);
  for (const QString& dirEntry : mDistDir.entryList()) {
    if (dirEntry.endsWith(distToPreserve.suffix())) {
      continue; 
    }
    if (!mDistDir.remove(dirEntry)) {
      qDebug() << "Could not delete" << dirEntry;
      return false;
    }
    qDebug() << "Deleted" << dirEntry; 
  }
  return true;
}

void
DistUpdater::startNextDownload()
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
  connect(mCurrentDownload, &QNetworkReply::finished, this, &DistUpdater::downloadFinished);
  connect(mCurrentDownload, &QNetworkReply::readyRead, this, &DistUpdater::downloadReadyRead);
  qDebug() << "Downloading:" << url.toEncoded().constData();
}

void
DistUpdater::downloadFinished()
{
  mCurrentDownload->deleteLater();
  if (mCurrentDownload->error()) {
    qDebug() << "File download failed:" << mCurrentDownload->errorString();
    fallBackToCurrDist();
    mOutputFile.remove();
    return;
  }

  if (!mNewDist) {
    // Assumption: if we don't have the new Dist yet then the file is the new Dist manifest
    mNewDist = Dist::fromManifestFile(mOutputFile, mDistDir, QLatin1String(NEW_FILE_SUFFIX));
    if (!mNewDist) {
      // Can't read the downloaded manifest file
      fallBackToCurrDist();
      mOutputFile.remove();
      return;
    }

    if (mCurrDist && *mCurrDist == *mNewDist && mCurrDist->isValid()) {
      // We already have the latest version
      emit stateChanged(DistUpdaterState::UpToDateAndDistValid);
      mOutputFile.remove();
      return;
    }

    for (const QString& entryFileName : mNewDist->entryFileNames()) {
      enqueueDownload(entryFileName);
    }
    emit stateChanged(DistUpdaterState::DownloadingDistFiles);
  }

  qDebug() << "Saved" << QFileInfo(mOutputFile).absoluteFilePath();
  mOutputFile.close();

  if (!mDownloadQueue.isEmpty()) {
    startNextDownload();
    return;
  }

  qDebug() << "Download queue is now empty";
  if (mNewDist->isValid()
      && cleanDistDirPreserving(*mNewDist)
      && mNewDist->changeSuffix(mCurrDist->suffix())) {
    // We've successfully committed the downloaded version
    emit stateChanged(DistUpdaterState::UpToDateAndDistValid);
    return;
  }

  fallBackToCurrDist();
}

void
DistUpdater::downloadReadyRead()
{
  mOutputFile.write(mCurrentDownload->readAll());
}
}
