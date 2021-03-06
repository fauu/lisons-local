#include "backend.h"
#include "dist_updater.h"

#include "lib/hobrasofthttp/httpserver.h"
#include "lib/hobrasofthttp/httpsettings.h"

#include <QDebug>
#include <QtCore>
#include <QtGui/QDesktopServices>

namespace Lisons {

Backend::Backend(QObject* parent, short serverPort)
  : QObject(parent)
  , mServerPort(serverPort)
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
  mExposedServerAddress = QStringLiteral("http://localhost:%1").arg(mServerPort);
  emit exposedServerAddressChanged();

  auto* serverSettings = new HobrasoftHttpd::HttpSettings(this);
  serverSettings->setDocroot(mAppDataDir.absolutePath());
  serverSettings->setPort(mServerPort);

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
  QDesktopServices::openUrl(QUrl(mExposedServerAddress));
}

void
Backend::serverCouldNotStart()
{
  setExposedServerState(ServerState::CouldNotStart);
}
}
