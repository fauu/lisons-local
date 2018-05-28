#ifndef LISONS_LOCAL_DIST_MANAGER_H
#define LISONS_LOCAL_DIST_MANAGER_H

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

enum DistManagerState
{
  DownloadingManifest,
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
  void start();

signals:
  void stateChanged(DistManagerState newState);

private:
  void enqueueDownload(const QString &fileName);
  std::unique_ptr<Manifest> readManifest(QFile& file, const QString& suffix);
  bool verifyDist(std::unique_ptr<Manifest> const &manifest);
  void deleteNewDist();
  void overwriteCurrDist();

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
  std::unique_ptr<Manifest> mCurrManifest;
  std::unique_ptr<Manifest> mNewManifest;
};

#endif // LISONS_LOCAL_DIST_MANAGER_H
