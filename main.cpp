#include "mainwindow.h"
#include <QApplication>
#include <QQmlapplicationEngine>
#include <QQmlContext>
#include <QQuickWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("QmlMainwindow",&w);
    engine.load(QUrl(QStringLiteral("qrc:///qml/main.qml")));

   // w.show();

 //   QQuickWidget *view = new QQuickWidget;
//    view->setSource(QUrl(QStringLiteral("qrc:///qml/main.qml")));
  //  view->show();


    return a.exec();
}
