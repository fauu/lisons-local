#include "backend.h"
#include "main_window.h"

#include <QApplication>
#include <QDebug>
#include <QtGui/QFontDatabase>

int
main(int argc, char* argv[])
{
  QApplication::setAttribute(Qt::AA_Use96Dpi);

  QApplication app(argc, argv);

  QFontDatabase::addApplicationFont(":/fonts/Lato-Bold.ttf");
  QFont mainFont("Lato", 16, QFont::Bold);
  QApplication::setFont(mainFont);


  MainWindow window;
  window.show();

  return app.exec();

//  QGuiApplication app(argc, argv);
//
//  QQmlApplicationEngine engine;
//
//  Backend backend(&app);
//  backend.init();
//  engine.rootContext()->setContextProperty("backend", &backend);
//
//  engine.load(QUrl(QStringLiteral("qrc:///ui/main.qml")));
//  if (engine.rootObjects().isEmpty()) {
//    return -1;
//  }
//
//  return QGuiApplication::exec();
}
