#ifndef CONFIGDATA_H
#define CONFIGDATA_H

#include <QSet>
#include <QSharedData>
#include <QStringList>
#include <QVariant>

class OptionsModel;
class FeaturesModel;
class Option;
struct Feature;
class ConfigData : public QSharedData
{
public:
    ConfigData();
    enum class LicenceType {
        Unset,
        OpenSource,
        Commercial
    };
    QStringList submodules;
    QStringList selectedModules;
    QStringList platforms;
    QStringList devices;
    QList<Option*> options;
    QList<Feature*> features;
    QStringList licenses;
    QSet<QString> configs;

    QMap<QString, Qt::CheckState> featuresStates;
    QMap<QString, QVariant> optionsStates;

    QMap<QString, QVariant> optionsValueStrings;

    QString sourcePath;
    QString buildPath;
    LicenceType licenseType;

    OptionsModel *optionsModel;
    FeaturesModel *featuresModel;
};

#endif // CONFIGDATA_H
