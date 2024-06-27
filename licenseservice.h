#ifndef LICENSESERVICE_H
#define LICENSESERVICE_H

#include <QJsonObject>

class LicenseService
{
public:
    LicenseService();

    void createLicFile();
    long getConfig(QVariantMap &retDat);
    long getNodes(QVariantList &retDat);
private:
    long saveObj(const QString fileName, const QJsonObject &obj, bool useBinaryFormat);
};

#endif // LICENSESERVICE_H
