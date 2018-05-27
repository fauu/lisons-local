#include "backend.h"

#include <QDebug>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int
main(int argc, char* argv[])
{
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);

  QFontDatabase::addApplicationFont(":/assets/fonts/Lato-Bold.ttf");

  QQmlApplicationEngine engine;

  Backend backend;
  backend.init();
  engine.rootContext()->setContextProperty("backend", &backend);

  engine.load(QUrl(QStringLiteral("qrc:/res/main.qml")));
  if (engine.rootObjects().isEmpty()) {
    return -1;
  }

  return QGuiApplication::exec();
}
