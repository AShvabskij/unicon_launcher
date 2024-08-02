#include "licensemodel.h"

LicenseModel::LicenseModel(QObject *parent)
    : QObject{parent}
{
    m_tunnelSrv = TunnelService::instance();
    m_license = LicenseService::instance();
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

    if (res == 1) {
        QVariantList nodes;
        res = m_license->getNodes(nodes);
    }

    m_isValid = (res == 1);

    QString key = conf.value("AppKey").toString();
    if (m_appKey.isEmpty()) {
        m_appKey = key;
        emit appKeyChanged(m_appKey);
    }

    m_licenseNumber = conf.value("Number").toString();
    emit licenseNumberChanged(m_licenseNumber);

    if (key != m_appKey)
    {
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

