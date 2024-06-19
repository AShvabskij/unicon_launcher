#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWebView/QtWebView>

int main(int argc, char *argv[])
{
    QtWebView::initialize();
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("unicon_launcher", "Main");

    return app.exec();
}
