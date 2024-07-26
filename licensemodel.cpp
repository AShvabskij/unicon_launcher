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
    emit appKeyChanged(m_appKey);
}

void LicenseModel::parseLicense()
{
    QVariantMap conf;
    long res = m_license->getConfig(conf);

    m_isValid = (res == 1);
    if (res) {
        m_isValid = m_license->checkConfig(conf);
    }

    if (m_isValid) {
        QVariantList nodes;
        res = m_license->getNodes(nodes);
        m_isValid = (res == 1);
    }

    QString key = conf.value("PrivateKey").toString();

    if (m_appKey.isEmpty()) {
        m_appKey = key;
        emit appKeyChanged(m_appKey);
    }

    if (key == m_appKey)
    {
        m_licenseNumber = conf.value("Number").toString();
        emit licenseNumberChanged(m_licenseNumber);
    } else {
        m_isValid = false;
    }

    emit isValidChanged(m_isValid);
}

