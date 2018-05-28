#include "backend.h"
#include "dist_manager.h"

#include "lib/hobrasofthttp/httpserver.h"
#include "lib/hobrasofthttp/httpsettings.h"

#include <QDebug>
#include <QtCore>

Backend::Backend(QObject* parent)
  : QObject(parent)
  , mDistManager(this, getAppDataDir())
{}

void
Backend::init()
{
  qDebug() << "appDataDir =" << getAppDataDir().absolutePath();
  connect(&mDistManager, &DistManager::stateChanged, this, &Backend::distManagerStateChanged);
  mDistManager.updateDist();
}

short
Backend::getExposedDistManagerState() const
{
  return mExposedDistManagerState;
}

short
Backend::getExposedServerState() const
{
  return mExposedServerState;
}

void
Backend::setExposedDistManagerState(short newState)
{
  if (newState != mExposedDistManagerState) {
    mExposedDistManagerState = newState;
    emit exposedDistManagerStateChanged();
  }
}

void
Backend::setExposedServerState(short newState)
{
  if (newState != mExposedServerState) {
    mExposedServerState = newState;
    emit exposedServerStateChanged();
  }
}

QDir&
Backend::getAppDataDir()
{
  static QDir appDataDir(
    QStandardPaths::writableLocation(QStandardPaths::StandardLocation::AppLocalDataLocation));
  return appDataDir;
}

void
Backend::launchServer()
{
  HobrasoftHttpd::HttpSettings serverSettings(this);
  serverSettings.setDocroot(getAppDataDir().absolutePath());
  mServer = new HobrasoftHttpd::HttpServer(&serverSettings, this);
  connect(mServer, &HobrasoftHttpd::HttpServer::started, this, &Backend::serverStarted);
  connect(mServer, &HobrasoftHttpd::HttpServer::couldNotStart, this, &Backend::serverCouldNotStart);
  mServer->start();
}

void
Backend::distManagerStateChanged(DistManagerState newState)
{
  switch (newState) {
    case DistManagerState::UpToDateAndDistValid: // Fall through
    case DistManagerState::CouldNotUpdateButDistValid:
      launchServer();
      break;
    default:; // Skip
  }
  setExposedDistManagerState(newState);
}

void
Backend::serverStarted()
{
  setExposedServerState(ServerState::Started);
}

void
Backend::serverCouldNotStart()
{
  setExposedServerState(ServerState::CouldNotStart);
}
