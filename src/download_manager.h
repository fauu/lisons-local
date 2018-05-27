#ifndef LISONS_LOCAL_DOWNLOAD_MANAGER_H
#define LISONS_LOCAL_DOWNLOAD_MANAGER_H

#include <QtCore>
#include <QtNetwork>

struct ManifestEntry
{
  QString md5;
  QString fileName;
};

enum DownloadManagerState
{
  DownloadingManifest,
  DownloadingLisons,
  UpToDateAndPackageValid,
  CouldNotUpdateButPackageValid,
  PackageInvalid,
};

class DownloadManager : public QObject
{
  Q_OBJECT
public:
  explicit DownloadManager(QObject* parent = nullptr);
  void start();

signals:
  void finished();
  void stateChanged(DownloadManagerState newState);

private:
  void enqueue(const QString& fileName);
  void processManifestReply(QNetworkReply* manifestReply); // TODO: remove?
  std::unique_ptr<QVector<ManifestEntry>> readManifest(QFile& file);
  bool verifyPackage(bool newOne = false);
  void deleteNewPackage();
  void overwritePackage();

private slots:
  void startNextDownload();
  void downloadReadyRead();
  void downloadFinished();

private:
  QDir mDownloadDir;
  QString mNewManifestFileName;
  QNetworkAccessManager mNetworkAccessManager;
  QQueue<QUrl> mDownloadQueue;
  QNetworkReply* mCurrentDownload = nullptr;
  QFile mOutputFile;
  std::unique_ptr<QVector<ManifestEntry>> mNewManifest;
};

#endif // LISONS_LOCAL_DOWNLOAD_MANAGER_H
