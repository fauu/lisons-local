#ifndef LISONS_LOCAL_BACKEND_H
#define LISONS_LOCAL_BACKEND_H

#include "dist_updater.h"

#include "lib/hobrasofthttp/httpserver.h"

#include <QHostAddress>
#include <QtCore>

namespace Lisons {

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
                 READ exposedDistUpdaterState
                 WRITE setExposedDistUpdaterState
                 NOTIFY
                 exposedDistUpdaterStateChanged)
  Q_PROPERTY(QString serverAddress
                 READ exposedServerAddress
                 NOTIFY
                 exposedServerAddressChanged)
  Q_PROPERTY(short serverState
                 READ exposedServerState
                 WRITE setExposedServerState
                 NOTIFY
                 exposedServerStateChanged)
  // clang-format on

public:
  explicit Backend(QObject* parent);
  void init();
  short exposedDistUpdaterState() const;
  QString exposedServerAddress() const;
  short exposedServerState() const;

signals:
  void exposedDistUpdaterStateChanged();
  void exposedServerAddressChanged();
  void exposedServerStateChanged();

private:
  void setExposedDistUpdaterState(short newState);
  void setExposedServerState(short newState);
  void launchServer();

private slots:
  void distUpdaterStateChanged(DistUpdaterState newState);
  void serverStarted();
  void serverCouldNotStart();

private:
  QDir mAppDataDir;
  DistUpdater mDistUpdater;
  HobrasoftHttpd::HttpServer* mServer;
  short mExposedDistUpdaterState = 0;
  QString mExposedServerAddress;
  short mExposedServerState = ServerState::Initial;
};
}

#endif // LISONS_LOCAL_BACKEND_H
