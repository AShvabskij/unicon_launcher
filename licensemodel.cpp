#include "licensemodel.h"
#include <QDesktopServices>

#include <QCoreApplication>
#include <QClipboard>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QUrl>
#include <QUrlQuery>
#include <QGuiApplication>

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
    }

    QVariantMap conf;
    long res = m_license->getConfig(conf);

    m_isExists = (res != -2);
    if (res == 1) {
        res = m_license->checkConfig(conf);
    }

    m_isValid = (res == 1);

    QString key = conf.value("AppKey").toString().trimmed();

    if (m_appKey.isEmpty() && m_isValid) {
        m_appKey = key;
    }

    if (m_appKey.isEmpty()) {
        generateKey();
    }

    m_licenseNumber = conf.value("Number").toString().trimmed();
    m_usrFullName = conf.value("UserName").toString().trimmed();
    m_usrEmail = conf.value("UserEmail").toString().trimmed();
    m_usrPhone = conf.value("UserPhone").toString().trimmed();
    m_usrCompany = conf.value("Company").toString().trimmed();

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

    if (m_isValid && key != m_appKey.trimmed())
    {
        qWarning() << "The license is not valid! License App key " << key << " is not equal to the generated key:" << m_appKey;
        m_isValid = false;
    }

    emit appKeyChanged(m_appKey);
    emit licenseNumberChanged(m_licenseNumber);
    emit usrNameChanged(m_usrFullName);
    emit usrEmailChanged(m_usrEmail);
    emit usrPhoneChanged(m_usrPhone);
    emit usrCompanyChanged(m_usrCompany);
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
    copyToClipboard();
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

void LicenseModel::copyToClipboard()
{
    QString recipient = "Andrei Drozdov <andrei.d@solconigel.com>";
    QString subject = "IRST License request";
    QString msg = buildSendMessage();

    QStringList res;
    res << "Send to: " + recipient;
    res << "";
    res << "Subject: " + subject;
    res << "";
    res << "Message: " << msg;

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(res.join("\n"));
}

void LicenseModel::sendPutRequest()
{
    QNetworkAccessManager* networkManager = new QNetworkAccessManager;

    // Endpoint URL
    QUrl url("https://solconigel.com/wp-json/contact-form-7/v1/contact-forms/81848/feedback");
    QNetworkRequest request(url);

    // Set headers
    request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");

    // Create the multipart object
    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    // Add 'fname' field
    QHttpPart fnamePart;
    fnamePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"fname\""));
    fnamePart.setBody("YOUR_NAME");
    multiPart->append(fnamePart);

    // Add 'email' field
    QHttpPart emailPart;
    emailPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"email\""));
    emailPart.setBody("YOUR_EMAIL");
    multiPart->append(emailPart);

    // Add 'subject' field
    QHttpPart subjectPart;
    subjectPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"subject\""));
    subjectPart.setBody("YOUR_SUBJECT");
    multiPart->append(subjectPart);

    // Add 'message' field
    QHttpPart messagePart;
    messagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"message\""));
    messagePart.setBody("YOUR_MESSAGE");
    multiPart->append(messagePart);

    // Add '_wpcf7_unit_tag' field
    QHttpPart unitTagPart;
    unitTagPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"_wpcf7_unit_tag\""));
    unitTagPart.setBody("wpcf7-f81848-o2");
    multiPart->append(unitTagPart);

    // Send the POST request
    QNetworkReply* reply = networkManager->post(request, multiPart);

    // Ensure the multiPart is deleted after the request is sent
    multiPart->setParent(reply);

    // Handle the response
    QObject::connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            qDebug() << "Success:" << response;
        } else {
            qDebug() << "Error:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

QString LicenseModel::buildSendMessage()
{
    QStringList body;
    body << "App key: " + m_appKey;
    body << "User: " + m_usrFullName;
    body << "Phone: " + m_usrPhone;
    body << "EMail: " + m_usrEmail;
    body << "Company: " + m_usrCompany;
    body << "";

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
    body << "";

    QString res = body.join("\n");
    return res;
}
