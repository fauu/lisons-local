#ifndef LISONS_LOCAL_BACKEND_H
#define LISONS_LOCAL_BACKEND_H

#include "dist_manager.h"

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
  Q_PROPERTY(short distManagerState
             READ getExposedDistManagerState
             WRITE setExposedDistManagerState
             NOTIFY exposedDistManagerStateChanged)
  Q_PROPERTY(short serverState
             READ getExposedServerState
             WRITE setExposedServerState
             NOTIFY exposedServerStateChanged)
  // clang-format on

public:
  explicit Backend(QObject* parent);
  void init();
  short getExposedDistManagerState() const;
  short getExposedServerState() const;

signals:
  void exposedDistManagerStateChanged();
  void exposedServerStateChanged();

private:
  void setExposedDistManagerState(short newState);
  void setExposedServerState(short newState);
  QDir& getAppDataDir();
  void launchServer();

private slots:
  void distManagerStateChanged(DistManagerState newState);
  void serverStarted();
  void serverCouldNotStart();

private:
  DistManager mDistManager;
  HobrasoftHttpd::HttpServer* mServer;
  short mExposedDistManagerState = 0;
  short mExposedServerState = 0;
};

#endif // LISONS_LOCAL_BACKEND_H
