#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include "configdata.h"
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

    QExplicitlySharedDataPointer<ConfigData> d;

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

    //properties
    QString sourcePath() const;
    QString buildPath() const;
    QString installPath() const;


    LicenceType licenseType() const;
    void setLicenseType(const LicenceType &licenseType);

    QStringList selectedModules() const;
    void setSelectedModules(const QStringList &selectedModules);

    bool confirmLicense() const;
    QStringList submodules() const;
    QStringList platforms() const;
    QStringList licenses() const;

    QString defaultPlatform() const;
    QString defaultPlatform(QString &platform_notes) const;
    QVariantList nomake() const;

    QStringList devices() const;

    // features
    void clearFeatureStates();
    Qt::CheckState featureState(const QString &featureName) const;
    void setFeatureState(const QString &featureName, const Qt::CheckState &state);
    Feature *feature(const QString &name);
    QList<Feature *> features() const;

    // options
    void clearOptionsStates();
    QVariant optionState(const QString &name) const;
    void setOptionsState(const Option *option, const QVariant &state);
    void setOptionsState(const QString &optionName, const QVariant &state);
    Option *option(const QString &name) const;
    Option *optionByOpt(const QString &opt, QString &val);
    QList<Option *> options() const;

    // config
    bool hasConfig(const QString &name) const;

    // save/restore
    QStringList createFeatures() const;
    QStringList createOptions() const;
    QStringList createCommons() const;
    QStringList createCommand(const SaveParametereType &params = All) const;
    QProcess *createProcess() const;
    void readConfig();
    void importSettings();
    void deleteSettings();
    bool save(const QString &path = QString(), const SaveParametereType &params = All);
    bool isSaveNeeded() const;

    void process();

    //models
    OptionsModel *optionsModel() const;
    FeaturesModel *featuresModel() const;

public slots:
    void setSourcePath(QString sourcePath);
    void setBuildPath(QString buildPath);
    void setInstallPath(QString installPath);
    void setConfirmLicense(bool confirmLicense);

signals:
    void sourcePathChanged(QString sourcePath);
    void buildPathChanged(QString buildPath);

//    void readComplate();
    void modulesUpdated();
    void platformsUpdated();
    void licensesUpdated();
    void configuresUpdated();

private:
    void clear();
    QStringList platformsInDir(const QString &path);
    QByteArray readFile(const QString &path);
    void readConfig(const QString &path, const QString &moduleName);
    void qmakeConfFile(const QString &path);

    friend class OptionsModel;
    friend class FeaturesModel;
};

#endif // CONFIGMANAGER_H
