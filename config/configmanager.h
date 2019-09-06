#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include "global.h"

#include <QJsonObject>
#include <QObject>
#include <jsonconfig.h>

class QProcess;
class ConfigManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString sourcePath READ sourcePath WRITE setSourcePath NOTIFY sourcePathChanged)
    Q_PROPERTY(QString buildPath READ buildPath WRITE setBuildPath NOTIFY buildPathChanged)
    Q_PROPERTY(QString installPath READ installPath WRITE setInstallPath NOTIFY installPathChanged)
//    Q_PROPERTY(bool confirmLicense READ confirmLicense WRITE setConfirmLicense NOTIFY confirmLicenseChanged)
//    Q_PROPERTY(bool useCommercial READ useCommercial WRITE setUseCommercial NOTIFY useCommercialChanged)

public:
    enum class LicenceType {
        Unset,
        OpenSource,
        Commercial
    };
    enum SaveParametereTypes {
        Features = 1,
        Options = 2,
        Commons = 4,
        All = Features | Options | Commons
    };
    Q_DECLARE_FLAGS(SaveParametereType, SaveParametereTypes)
    Q_FLAG(SaveParametereType)

    ConfigManager();

    QString sourcePath() const;
    QString buildPath() const;
    QString installPath() const;
    QList<Option *> options() const;
    QList<Feature *> features() const;
    QStringList submodules() const;
    QStringList selectedModules() const;
    QStringList platforms() const;

    QStringList createFeatures() const;
    QStringList createLibs() const;
    QStringList createCommons() const;
    QStringList createCommand() const;
    QProcess *createProcess() const;

    void setSelectedModules(const QStringList &selectedModules);

    void clearFeatureStates();
    Qt::CheckState featureState(const QString &featureName) const;
    void setFeatureState(const QString &featureName, const Qt::CheckState &state);
    QVariant optionState(const QString &name) const;
    void setOptionsState(const QString &optionName, const QVariant &state);

    void clearOptionsStates();
    QString defaultPlatform() const;
    QString defaultPlatform(QString &platform_notes) const;

    void readConfig();
    void importSettings();
    void deleteSettings();
    Option *option(const QString &name) const;
    Option *optionByOpt(const QString &opt, QString &val);
    Feature *feature(const QString &name);

    void process();

    bool save(const QString &path = QString(), const SaveParametereType &params = All);

    bool confirmLicense() const;

    bool useCommercial() const;
    bool isSaveNeeded() const;
    bool hasConfig(const QString &name) const;
    QStringList licenses() const;

    QStringList nomake() const;

    QStringList devices() const;

    LicenceType licenseType() const;
    void setLicenseType(const LicenceType &licenseType);

public slots:
    void setSourcePath(QString sourcePath);
    void setBuildPath(QString buildPath);
    void setInstallPath(QString installPath);

    void setConfirmLicense(bool confirmLicense);

//    void setUseCommercial(bool useCommercial);

signals:
    void sourcePathChanged(QString sourcePath);
    void buildPathChanged(QString buildPath);
    void installPathChanged(QString installPath);
    void confirmLicenseChanged(bool confirmLicense);
//    void useCommercialChanged(bool useCommercial);

    void readComplate();
    void modulesUpdated();
    void platformsUpdated();
    void configuresUpdated();
    void licensesUpdated();

private:

    QStringList _submodules;
    QStringList _selectedModules;
    QStringList _platforms;
    QStringList _devices;
    QList<Option*> _options;
    QList<Feature*> _features;
    QStringList _licenses;
    QSet<QString> _configs;

    //depreacted
    QStringList _nomake;

    QMap<QString, Qt::CheckState> _featuresStates;
    QMap<QString, QVariant> _optionsStates;

    QString m_sourcePath;
    QString m_buildPath;
//    QString m_installPath;
//    bool m_confirmLicense;
//    bool m_useCommercial;
    LicenceType _licenseType;

    void clear();
    QStringList platformsInDir(const QString &path);
    QByteArray readFile(const QString &path);
    void readConfig(const QString &path, const QString &moduleName);
    void qmakeConfFile(const QString &path);
};

#endif // CONFIGMANAGER_H
