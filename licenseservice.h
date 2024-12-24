#ifndef LICENSESERVICE_H
#define LICENSESERVICE_H

#include <QJsonObject>

class LicenseService
{
public:
    LicenseService();
    static LicenseService* instance() {
        static LicenseService i;
        return &i;
    }

    void createLicFile();
    void deleteLicFile();

    long getConfig(QVariantMap &retDat);
    bool checkConfig(const QVariantMap &dat);
    long getServerConfig(QVariantMap& retDat, bool useReserveConfig = false);
    bool checkServerConfig(const QVariantMap &dat);
    long getNodes(QVariantList &retDat);
    long saveFile(QString licFilePath);
    long saveKey(const QString &key);
    long getKey(QString &key);
    long deleteKey();

private:
    long load(QJsonObject& retObj);
    long save(const QString fileName, const QJsonObject &obj, bool useBinaryFormat);
    long replaceFile(const QString &sourceFilePath, const QString &destinationFilePath);
    QString removeFilePrefix(const QString &filePath);
};

#endif // LICENSESERVICE_H
