#ifndef LISONS_LOCAL_DIST_MANAGER_H
#define LISONS_LOCAL_DIST_MANAGER_H

#include "dist.h"

#include <QtCore>
#include <QtNetwork>

enum DistManagerState
{
  DownloadingDistManifest,
  DownloadingDistFiles,
  UpToDateAndDistValid,
  CouldNotUpdateButDistValid,
  DistInvalid,
};

class DistManager : public QObject
{
  Q_OBJECT
public:
  DistManager(QObject* parent, const QDir& saveDir);
  void updateDist();

signals:
  void stateChanged(DistManagerState newState);

private:
  void enqueueDownload(const QString& fileName);
  bool verifyDist(std::unique_ptr<Dist> const& manifest);
  void deleteNewDist();
  bool overwriteCurrDist();
  void fallBackToCurrDist();

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

#endif // LISONS_LOCAL_DIST_MANAGER_H
