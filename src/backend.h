#ifndef LISONS_LOCAL_BACKEND_H
#define LISONS_LOCAL_BACKEND_H

#include "download_manager.h"

#include "lib/hobrasofthttp/httpserver.h"

#include <QHostAddress>
#include <QtCore>

enum ServerState
{
  Initial,
  Started,
  CouldNotStart,
};

class Backend : public QObject
{
  Q_OBJECT
  // clang-format off
  Q_PROPERTY(short downloadManagerState
             READ getExposedDownloadManagerState
             WRITE setExposedDownloadManagerState
             NOTIFY exposedDownloadManagerStateChanged)
  Q_PROPERTY(short serverState
             READ getExposedServerState
             WRITE setExposedServerState
             NOTIFY exposedServerStateChanged)
  // clang-format on

public:
  explicit Backend(QObject* parent = nullptr);
  void init();
  short getExposedDownloadManagerState() const;
  short getExposedServerState() const;

signals:
  void exposedDownloadManagerStateChanged();
  void exposedServerStateChanged();

private:
  void setExposedDownloadManagerState(short newState);
  void setExposedServerState(short newState);
  void launchServer();

private slots:
  void downloadManagerStateChanged(DownloadManagerState newState);
  void serverStarted();
  void serverCouldNotStart();

private:
  DownloadManager mDownloadManager;
  HobrasoftHttpd::HttpServer* mServer;
  short mExposedDownloadManagerState;
  short mExposedServerState = ServerState::Initial;
};

#endif // LISONS_LOCAL_BACKEND_H
