#include "backend.h"
#include "dist_updater.h"

#include "lib/hobrasofthttp/httpserver.h"
#include "lib/hobrasofthttp/httpsettings.h"

#include <QDebug>
#include <QtCore>

// TODO: Make configurable through program argument
static const short SERVER_PORT = 8080;

Backend::Backend(QObject* parent)
  : QObject(parent)
  , mDistUpdater(this, getAppDataDir())
{}

void
Backend::init()
{
  qDebug() << "SSL Library build version: " << QSslSocket::sslLibraryBuildVersionString();
  qDebug() << "SSL Library runtime version: " << QSslSocket::sslLibraryVersionString();
  qDebug() << "AppData directory: " << getAppDataDir().absolutePath();

  connect(&mDistUpdater, &DistUpdater::stateChanged, this, &Backend::distUpdaterStateChanged);
  mDistUpdater.updateAndVerify();
}

short
Backend::getExposedDistUpdaterState() const
{
  return mExposedDistUpdaterState;
}

QString
Backend::getExposedServerAddress() const
{
  return mExposedServerAddress;
}

short
Backend::getExposedServerState() const
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
  mExposedServerAddress = QStringLiteral("http://localhost:%1").arg(SERVER_PORT);
  emit exposedServerAddressChanged();

  auto* serverSettings = new HobrasoftHttpd::HttpSettings(this);
  serverSettings->setDocroot(getAppDataDir().absolutePath());
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
