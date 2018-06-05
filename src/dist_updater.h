#ifndef LISONS_LOCAL_DIST_UPDATER_H
#define LISONS_LOCAL_DIST_UPDATER_H

#include "dist.h"

#include <QtCore>
#include <QtNetwork>

namespace Lisons {

enum DistUpdaterState
{
  DownloadingDistManifest,
  DownloadingDistFiles,
  UpToDateAndDistValid,
  CouldNotUpdateButDistValid,
  DistInvalid,
};

class DistUpdater : public QObject
{
  Q_OBJECT
public:
  DistUpdater(QObject* parent, const QDir& saveDir);
  void updateAndVerify();

signals:
  void stateChanged(DistUpdaterState newState);

private:
  void enqueueDownload(const QString& fileName);
  void fallBackToCurrDist();
  bool cleanDistDirPreserving(const Dist& distToPreserve);

private slots:
  void startNextDownload();
  void downloadReadyRead();
  void downloadFinished();

private:
  QDir mDistDir;
  QNetworkAccessManager mNetworkAccessManager;
  QQueue<QUrl> mDownloadQueue;
  QNetworkReply* mCurrentDownload = nullptr;
  QFile mOutputFile;
  std::unique_ptr<Dist> mCurrDist;
  std::unique_ptr<Dist> mNewDist;
};
}

#endif // LISONS_LOCAL_DIST_UPDATER_H
