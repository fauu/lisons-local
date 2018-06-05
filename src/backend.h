#ifndef LISONS_LOCAL_BACKEND_H
#define LISONS_LOCAL_BACKEND_H

#include "dist_updater.h"

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
  Q_PROPERTY(short distUpdaterState
             READ getExposedDistUpdaterState
             WRITE setExposedDistUpdaterState
             NOTIFY exposedDistUpdaterStateChanged)
  Q_PROPERTY(short serverPort
                 READ getServerPort
                 NOTIFY serverPortChanged)
  Q_PROPERTY(short serverState
             READ getExposedServerState
             WRITE setExposedServerState
             NOTIFY exposedServerStateChanged)
  // clang-format on

public:
  explicit Backend(QObject* parent);
  void init();
  short getExposedDistUpdaterState() const;
  short getServerPort() const;
  short getExposedServerState() const;

signals:
  void exposedDistUpdaterStateChanged();
  void serverPortChanged();
  void exposedServerStateChanged();

private:
  void setExposedDistUpdaterState(short newState);
  void setExposedServerState(short newState);
  QDir& getAppDataDir();
  void launchServer();

private slots:
  void distUpdaterStateChanged(DistUpdaterState newState);
  void serverStarted();
  void serverCouldNotStart();

private:
  DistUpdater mDistUpdater;
  HobrasoftHttpd::HttpServer* mServer;
  short mServerPort = -1;
  short mExposedDistUpdaterState = 0;
  short mExposedServerState = 0;
};

#endif // LISONS_LOCAL_BACKEND_H
