#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWebView/QtWebView>

#include "qtunnel.h"
#include "nodelistmodel.h"
#include "localmodel.h"
#include "licensemodel.h"
#include "equipmentlistmodel.h"

int main(int argc, char *argv[])
{
    QtWebView::initialize();
    QGuiApplication app(argc, argv);

    qmlRegisterType<QTunnel>("qmlmodels", 1, 0,"QTunnel");
    qmlRegisterType<NodeListModel>("qmlmodels", 1, 0,"NodeListModel");
    qmlRegisterType<LocalModel>("qmlmodels", 1, 0,"LocalModel");
    qmlRegisterType<LicenseModel>("qmlmodels", 1, 0,"LicenseModel");
    qmlRegisterType<EquipmentListModel>("qmlmodels", 1, 0,"EquipmentListModel");
    qRegisterMetaType<EquipmentListModel::Equipment>("Equipment");

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("unicon_launcher", "Main");

/*
    NodeListModel nodeListModel;
    nodeListModel.init();

    engine.rootContext()->setContextProperty("nodeListModel", QVariant::fromValue(&nodeListModel));
*/

    QIcon icon = QIcon(":/qt/qml/unicon_launcher/images/icons/logo_window.png");
    app.setWindowIcon(icon);

    return app.exec();
}
