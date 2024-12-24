#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWebView/QtWebView>

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QQuickStyle>

#include "qtunnel.h"
#include "nodelistmodel.h"
#include "localmodel.h"
#include "licensemodel.h"
#include "equipmentlistmodel.h"

#include <iostream>
#include <fstream>

QFile qtLogFile("log_qt.txt");

void logMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    if (msg.contains("QMetaObject::indexOfSignal")) {
        return; // Suppress this message
    }

    QTextStream out(&qtLogFile);
    out << msg << Qt::endl;
}

int main(int argc, char *argv[])
{
    // qputenv("QT_OPENGL", "software");
    // qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--disable-gpu");

    // Optional: Log to confirm the environment variable
    // qDebug() << "QT_OPENGL set to" << qgetenv("QT_OPENGL");

    QtWebView::initialize();
    QGuiApplication app(argc, argv);

    qmlRegisterType<QTunnel>("qmlmodels", 1, 0,"QTunnel");
    qmlRegisterType<NodeListModel>("qmlmodels", 1, 0,"NodeListModel");
    qmlRegisterType<LocalModel>("qmlmodels", 1, 0,"LocalModel");
    qmlRegisterType<LicenseModel>("qmlmodels", 1, 0,"LicenseModel");
    qmlRegisterType<EquipmentListModel>("qmlmodels", 1, 0,"EquipmentListModel");
    qRegisterMetaType<EquipmentListModel::Equipment>("Equipment");

    QQuickStyle::setStyle("Basic");
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

//  QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);
    QLoggingCategory::setFilterRules("qt.qmetaobject.warning=false");

    qInstallMessageHandler(logMessageHandler);

    std::ofstream logFile("log_cpp.txt");

    // Check if the file opened successfully
    if (!logFile.is_open()) {
        std::cerr << "Error opening log file!" << std::endl;
        return 1;
    }

    // Redirect std::cout to the log file
    std::cout.rdbuf(logFile.rdbuf()); // Redirect to file buffer
    std::cerr.rdbuf(logFile.rdbuf()); // Redirect to file buffer

    if (QFileInfo(qtLogFile.fileName()).lastModified().date() != QDateTime::currentDateTime().date()) {
        qtLogFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    } else {
        qtLogFile.open(QIODevice::WriteOnly | QIODevice::Append);
    }

    if (!qtLogFile.isOpen()) {
        std::cerr << "Error opening log file!" << std::endl;
        return 1;
    }

    QTextStream out(&qtLogFile);

    out << Qt::endl << "App started at " << QDateTime::currentDateTime().toString() << Qt::endl;

    return app.exec();
}
