#include "licensemodel.h"
#include <QDesktopServices>

#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>

LicenseModel::LicenseModel(QObject *parent)
    : QObject{parent}
{
    m_tunnelSrv = TunnelService::instance();
    m_license = LicenseService::instance();

    m_equipmentListmodel = new EquipmentListModel();
}

void LicenseModel::saveLicense(QString filePath)
{
    long res = m_license->saveFile(filePath);
    if (!res) {
        emit errorRaised("Error saving license!", "");
    }
}

void LicenseModel::generateKey()
{
    m_appKey = m_tunnelSrv->genPrivateKey();
    m_license->saveKey(m_appKey);
    emit appKeyChanged(m_appKey);
}

void LicenseModel::clearKey()
{
    long res = m_license->deleteKey();

    if (res == 1) {
        m_appKey = "";
        emit appKeyChanged(m_appKey);
    } else {
        emit errorRaised("Error deleting application key!", "");
    }
}

void LicenseModel::parseLicense()
{
    if (m_appKey.isEmpty()) {
        m_license->getKey(m_appKey);
        emit appKeyChanged(m_appKey);
    }

    QVariantMap conf;
    long res = m_license->getConfig(conf);

    m_isExists = (res != -2);
    if (res == 1) {
        res = m_license->checkConfig(conf);
    }

    m_isValid = (res == 1);

    QString key = conf.value("AppKey").toString().trimmed();
    if (m_appKey.isEmpty()) {
        m_appKey = key;
        emit appKeyChanged(m_appKey);
    }

    m_licenseNumber = conf.value("Number").toString().trimmed();
    emit licenseNumberChanged(m_licenseNumber);

    m_usrFullName = conf.value("UserName").toString().trimmed();
    m_usrEmail = conf.value("UserEmail").toString().trimmed();
    m_usrPhone = conf.value("UserPhone").toString().trimmed();
    m_usrCompany = conf.value("Company").toString().trimmed();

    emit usrNameChanged(m_usrFullName);
    emit usrEmailChanged(m_usrEmail);
    emit usrPhoneChanged(m_usrPhone);
    emit usrCompanyChanged(m_usrCompany);

    if (res == 1) {
        QVariantList nodes;
        res = m_license->getNodes(nodes);

        if (!nodes.empty()) {
            m_equipmentListmodel->clear();
        }

        foreach (const QVariant &node, nodes) {
            EquipmentListModel::Equipment equip(node.toMap());
            m_equipmentListmodel->addItem(equip);
        }
    }

    if (key != m_appKey.trimmed())
    {
        qWarning() << "The license is not valid! License App key " << key << " is not equal to the generated key:" << m_appKey;
        m_isValid = false;
    }

    emit isValidChanged(m_isValid);
    emit isExistsChanged(m_isExists);
}

void LicenseModel::deleteLicense()
{
    m_license->deleteLicFile();
    m_isValid = false;
    m_isExists = false;
    m_licenseNumber = "";
    emit isValidChanged(false);
    emit isExistsChanged(false);
    emit licenseNumberChanged("");
}

void LicenseModel::sendRequest()
{
    sendEmail();
//  sendPutRequest();
}

void LicenseModel::sendEmail()
{
    QString recipient = "Andrei Drozdov <andrei.d@solconigel.com>";
    QString subject = "IRST License request";
    QString body = buildSendMessage();

    // Create the mailto URL
    QString mailtoUrl = QString("mailto:%1?subject=%2&body=%3")
                            .arg(recipient)
                            .arg(subject)
                            .arg(QUrl::toPercentEncoding(body));

    QUrl url(mailtoUrl);
    QDesktopServices::openUrl(url);

}

void LicenseModel::sendPutRequest()
{
    // Create a QNetworkAccessManager instance
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    // Create the URL for the request
    QUrl url("https://solconigel.com/api/");

    QString body = QUrl::toPercentEncoding(buildSendMessage());

    // Add query parameters to the URL
    QUrlQuery query;
    query.addQueryItem("fname", m_usrFullName);
    query.addQueryItem("email", m_usrEmail);
    query.addQueryItem("subject", "IRST License request");
    query.addQueryItem("message", body);
    url.setQuery(query);

    // Create a QNetworkRequest
    QNetworkRequest request(url);

    // Set headers if required (e.g., Content-Type or authentication)
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    // You can set the body for a PUT request (if required)
    QByteArray data = body.toUtf8();  // Empty body if the parameters are passed in the URL

    // Send the PUT request
    QNetworkReply *reply = manager->put(request, data);

    // Connect signals to handle the response
    QObject::connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            qDebug() << "Response:" << response;
        } else {
            qDebug() << "Error occurred:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

QString LicenseModel::buildSendMessage()
{
    QStringList body;
    body << "App key: " + m_appKey;
    body << "User:" + m_usrFullName;
    body << "Phone:" + m_usrPhone;
    body << "EMail:" + m_usrEmail;
    body << "Company:" + m_usrCompany;
    body << "------------------------------------------";

    for (int i = 0; i < m_equipmentListmodel->rowCount(); i++) {
        QModelIndex index = m_equipmentListmodel->index(i);

        QVariant eqName = m_equipmentListmodel->data(index, EquipmentListModel::NameRole);
        QVariant eqRegion = m_equipmentListmodel->data(index, EquipmentListModel::RegionRole);
        QVariant eqAddress = m_equipmentListmodel->data(index, EquipmentListModel::AddressRole);

        body << "Equipment: " + eqName.toString();
        body << "Region: " + eqRegion.toString();
        body << "Address: " + eqAddress.toString();
        body << "";

    }
    body << "------------------------------------------";

    QString res = body.join("\n");
    return res;
}
