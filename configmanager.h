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
    Q_PROPERTY(bool confirmLicense READ confirmLicense WRITE setConfirmLicense NOTIFY confirmLicenseChanged)
    Q_PROPERTY(bool useCommercial READ useCommercial WRITE setUseCommercial NOTIFY useCommercialChanged)

    QString m_sourcePath;
    QStringList _submodules;
    QStringList _selectedModules;
    QStringList _platforms;
    QList<Option> _options;
    QList<Feature> _features;

    QByteArray readFile(const QString &path);
    void readConfig(const QString &path, const QString &moduleName);
    QMap<QString, Qt::CheckState> _featuresStates;
    QMap<QString, QString> _optionsStates;
    QStringList _licenses;

    QString m_buildPath;

    QString m_installPath;

    bool m_confirmLicense;

    bool m_useCommercial;

public:
    ConfigManager();

    QString sourcePath() const;
    QString buildPath() const;
    QString installPath() const;
    QList<Option> options() const;
    QList<Feature> features() const;
    QStringList submodules() const;
    QStringList selectedModules() const;
    QStringList platforms() const;

    QStringList createCommand() const;
    QProcess *createProcess() const;

    void setSelectedModules(const QStringList &selectedModules);

    void clearFeatureStates();
    void setFeatureState(const QString &featureName, const Qt::CheckState &state);

    void clearOptionsStates();
    void setOptionsState(const QString &optionName, const QString &state);

    void process();


    bool confirmLicense() const;

    bool useCommercial() const;

    QStringList licenses() const;

public slots:
    void setSourcePath(QString sourcePath);
    void setBuildPath(QString buildPath);
    void setInstallPath(QString installPath);

    void setConfirmLicense(bool confirmLicense);

    void setUseCommercial(bool useCommercial);

signals:
    void sourcePathChanged(QString sourcePath);
    void buildPathChanged(QString buildPath);
    void installPathChanged(QString installPath);
    void confirmLicenseChanged(bool confirmLicense);
    void useCommercialChanged(bool useCommercial);

    void modulesUpdated();
    void platformsUpdated();
    void configuresUpdated();
    void licensesUpdated();
};

#endif // CONFIGMANAGER_H
