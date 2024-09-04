#ifndef LICENSEMODEL_H
#define LICENSEMODEL_H

#include <QObject>
#include "licenseservice.h"
#include "tunnelservice.h"

class LicenseModel : public QObject
{
    Q_OBJECT
public:
    explicit LicenseModel(QObject *parent = nullptr);

    Q_PROPERTY(QString appKey MEMBER m_appKey NOTIFY appKeyChanged)
    Q_PROPERTY(QString licenseNumber MEMBER m_licenseNumber NOTIFY licenseNumberChanged)
    Q_PROPERTY(bool isValid MEMBER m_isValid NOTIFY isValidChanged)
    Q_PROPERTY(bool isExists MEMBER m_isExists NOTIFY isExistsChanged)

    Q_INVOKABLE void saveLicense(QString filePath);
    Q_INVOKABLE void generateKey();
    Q_INVOKABLE void clearKey();
    Q_INVOKABLE void parseLicense();
    Q_INVOKABLE void deleteLicense();

    Q_INVOKABLE void sendEmail();

signals:
    void errorRaised(QString error, QString informativeError);
    void appKeyChanged(QString);
    void licenseNumberChanged(QString);
    void isValidChanged(bool);
    void isExistsChanged(bool);

private:
    TunnelService* m_tunnelSrv;
    LicenseService* m_license;

    QString m_licenseNumber = "";
    QString m_appKey = "";

    bool m_isValid = true;
    bool m_isExists = false;

};

#endif // LICENSEMODEL_H
