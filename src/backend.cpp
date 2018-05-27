#include "backend.h"
#include "download_manager.h"

#include "lib/hobrasofthttp/httpserver.h"
#include "lib/hobrasofthttp/httpsettings.h"

#include <QDebug>
#include <QtCore>

Backend::Backend(QObject* parent)
  : QObject(parent)
{}

void
Backend::init()
{
  connect(
    &mDownloadManager, &DownloadManager::stateChanged, this, &Backend::downloadManagerStateChanged);
  mDownloadManager.start();
}

short
Backend::getExposedDownloadManagerState() const
{
  return mExposedDownloadManagerState;
}

short
Backend::getExposedServerState() const
{
  return mExposedServerState;
}

void
Backend::setExposedDownloadManagerState(short newState)
{
  if (newState != mExposedDownloadManagerState) {
    mExposedDownloadManagerState = newState;
    emit exposedDownloadManagerStateChanged();
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

void
Backend::launchServer()
{
  HobrasoftHttpd::HttpSettings serverSettings(this);
  serverSettings.setDocroot("./web/");
  mServer = new HobrasoftHttpd::HttpServer(&serverSettings, this);
  connect(mServer, &HobrasoftHttpd::HttpServer::started, this, &Backend::serverStarted);
  connect(mServer, &HobrasoftHttpd::HttpServer::couldNotStart, this, &Backend::serverCouldNotStart);
  mServer->start();
}

void
Backend::downloadManagerStateChanged(DownloadManagerState newState)
{
  switch (newState) {
    case DownloadManagerState::UpToDateAndPackageValid: // Fall through
    case DownloadManagerState::CouldNotUpdateButPackageValid:
      launchServer();
      break;
    default:
      ; // Skip
  }
  setExposedDownloadManagerState(newState);
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
