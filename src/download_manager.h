#ifndef LISONS_LOCAL_DOWNLOAD_MANAGER_H
#define LISONS_LOCAL_DOWNLOAD_MANAGER_H

#include <QtCore>
#include <QtNetwork>

struct ManifestEntry
{
  QString md5;
  QString fileName;
};

// TODO: Make it a class?
struct Manifest
{
  QString suffix;
  QByteArray md5;
  QVector<ManifestEntry> entries;
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
  DownloadManager(QObject* parent, const QDir& saveDir);
  void start();

signals:
  void finished();
  void stateChanged(DownloadManagerState newState);

private:
  void enqueue(const QString& fileName);
  std::unique_ptr<Manifest> readManifest(QFile& file, const QString& suffix);
  bool verifyPackage(std::unique_ptr<Manifest> const& manifest);
  void deleteNewPackage();
  void overwritePackage();

private slots:
  void startNextDownload();
  void downloadReadyRead();
  void downloadFinished();

private:
  QDir mDownloadDir;
  QString mManifestPath;
  QNetworkAccessManager mNetworkAccessManager;
  QQueue<QUrl> mDownloadQueue;
  QNetworkReply* mCurrentDownload = nullptr;
  QFile mOutputFile;
  std::unique_ptr<Manifest> mManifest;
  std::unique_ptr<Manifest> mNewManifest;
};

#endif // LISONS_LOCAL_DOWNLOAD_MANAGER_H
