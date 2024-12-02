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

const int MAX_EQUIP_COUNT = 3;
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

        int i = 0;
        foreach (const QVariant &node, nodes) {
            EquipmentListModel::Equipment equip(node.toMap());
            m_equipmentListmodel->addItem(equip);
            if (++i == MAX_EQUIP_COUNT ) {
                break; // Only three eqipment are supported
            }
        }
    }

    if (m_isValid && key != m_appKey.trimmed())
    {
        QString msg = "The license is generated for another App key ";
        qWarning() << msg;
        emit errorRaised(msg, "Please request a valid license from support");
//      m_isValid = false;
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


bool LicenseModel::checkData()
{
    auto isValidEmail = [](const QString &email) -> bool {
        QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
        QRegularExpressionMatch match = emailRegex.match(email);
        return match.hasMatch();
    };

    if (!m_usrEmail.isEmpty() && !isValidEmail(m_usrEmail)) {
        emit errorRaised("Email is filled in incorrectly!", "Please correct the errors and try again");
        return false;
    }

    auto isValidPhoneNumber = [](const QString &phoneNumber) -> bool {
        QRegularExpression phoneRegex("^\\+?[1-9]\\d{0,2}[ -]?\(?\\d{1,3}\\)?[ -]?\\d{1,3}[ -]?\\d{1,9}$");
        QRegularExpressionMatch match = phoneRegex.match(phoneNumber);
        return match.hasMatch();
    };

    if (m_usrPhone.isEmpty() /*|| !isValidPhoneNumber(m_usrPhone)*/) {
        emit errorRaised("Phone number is filled in incorrectly!", "Please correct the errors and try again");
        return false;
    }

    return true;
}

void LicenseModel::sendRequest()
{
    if (!checkData()) {
        return;
    }

//  sendEmail();
    sendAPIRequest();
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

void LicenseModel::sendAPIRequest()
{
    QNetworkAccessManager* networkManager = new QNetworkAccessManager;

    // Endpoint URL
    QUrl url("https://solconigel.com/wp-json/contact-form-7/v1/contact-forms/81848/feedback");
    QNetworkRequest request(url);

    // Set headers
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data;  boundary=1BEF0A57BE110FD467A");

    // Create the multipart object
    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    // Add '_wpcf7_unit_tag' field
    QHttpPart unitTagPart;
    unitTagPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"_wpcf7_unit_tag\""));
    unitTagPart.setBody("wpcf7-f81848-o2");
    multiPart->append(unitTagPart);

    // Add 'fname' field
    QHttpPart fnamePart;
    fnamePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"fname\""));
    fnamePart.setBody(m_usrFullName.toUtf8());
    multiPart->append(fnamePart);

    // Add 'email' field
    QHttpPart emailPart;
    emailPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"email\""));
    emailPart.setBody(m_usrEmail.toUtf8());
    multiPart->append(emailPart);

    // Add 'subject' field
    QHttpPart subjectPart;
    subjectPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"subject\""));
    subjectPart.setBody("IRST License request");
    multiPart->append(subjectPart);

    // Add 'message' field
    QString msg = buildSendMessage();
    QHttpPart messagePart;
    messagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"message\""));
    messagePart.setBody(msg.toUtf8());
    multiPart->append(messagePart);

    // Send the POST request

    QNetworkReply* reply = networkManager->post(request, multiPart);

    // Ensure the multiPart is deleted after the request is sent
    multiPart->setParent(reply);

    // Handle the response
    QObject::connect(reply, &QNetworkReply::finished, [reply, this]() {
        QByteArray response = reply->readAll();
        qDebug() << "Response:" << response;

        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "Success:";
            emit errorRaised("Request succeded", "");

        } else {
            qDebug() << "Error:" << reply->errorString();
            emit errorRaised("Request failed!", "Please contact support to resolve this issue.");

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
