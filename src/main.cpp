#include "backend.h"

#include <QDebug>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int
main(int argc, char* argv[])
{
  QGuiApplication app(argc, argv);
  QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication::setApplicationVersion("0.1");

  QCommandLineParser cliParser;
  cliParser.setApplicationDescription("Local server launcher for Lisons!");
  cliParser.addHelpOption();
  cliParser.addVersionOption();
  cliParser.addOption({ { "port", "p" }, "Sets server port.", "port", "8080" });
  cliParser.process(app);

  QFontDatabase::addApplicationFont(":/fonts/Lato-Bold.ttf");

  QQmlApplicationEngine engine;
  Lisons::Backend backend{ &app, cliParser.value("port").toShort() };
  backend.init();
  engine.rootContext()->setContextProperty("backend", &backend);

  engine.load(QUrl(QStringLiteral("qrc:///ui/main.qml")));
  if (engine.rootObjects().isEmpty()) {
    return -1;
  }

  return QGuiApplication::exec();
}
