#include "backend.h"
#include "dist_updater.h"

#include "lib/hobrasofthttp/httpserver.h"
#include "lib/hobrasofthttp/httpsettings.h"

#include <QDebug>
#include <QtCore>

namespace Lisons {

// TODO: Make configurable through program argument
static const short SERVER_PORT = 8080;

Backend::Backend(QObject* parent)
  : QObject(parent)
  , mAppDataDir(QDir(
      QStandardPaths::writableLocation(QStandardPaths::StandardLocation::AppLocalDataLocation)))
  , mDistUpdater(this, mAppDataDir)
{}

void
Backend::init()
{
  qDebug() << "SSL Library build version:" << QSslSocket::sslLibraryBuildVersionString();
  qDebug() << "SSL Library runtime version:" << QSslSocket::sslLibraryVersionString();
  qDebug() << "AppData directory:" << mAppDataDir.absolutePath();

  connect(&mDistUpdater, &DistUpdater::stateChanged, this, &Backend::distUpdaterStateChanged);
  mDistUpdater.updateAndVerify();
}

short
Backend::exposedDistUpdaterState() const
{
  return mExposedDistUpdaterState;
}

QString
Backend::exposedServerAddress() const
{
  return mExposedServerAddress;
}

short
Backend::exposedServerState() const
{
  return mExposedServerState;
}

void
Backend::setExposedDistUpdaterState(short newState)
{
  if (newState != mExposedDistUpdaterState) {
    mExposedDistUpdaterState = newState;
    emit exposedDistUpdaterStateChanged();
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
  mExposedServerAddress = QStringLiteral("http://localhost:%1").arg(SERVER_PORT);
  emit exposedServerAddressChanged();

  auto* serverSettings = new HobrasoftHttpd::HttpSettings(this);
  serverSettings->setDocroot(mAppDataDir.absolutePath());
  serverSettings->setPort(SERVER_PORT);

  mServer = new HobrasoftHttpd::HttpServer(serverSettings, this);
  connect(mServer, &HobrasoftHttpd::HttpServer::started, this, &Backend::serverStarted);
  connect(mServer, &HobrasoftHttpd::HttpServer::couldNotStart, this, &Backend::serverCouldNotStart);
  mServer->start();
}

void
Backend::distUpdaterStateChanged(DistUpdaterState newState)
{
  switch (newState) {
    case DistUpdaterState::UpToDateAndDistValid: // Fall through
    case DistUpdaterState::CouldNotUpdateButDistValid:
      launchServer();
      break;
    default:; // Skip
  }
  setExposedDistUpdaterState(newState);
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
}
