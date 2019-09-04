#ifndef WIZARDPAGEBASE_H
#define WIZARDPAGEBASE_H

#include "configmanager.h"

#include <QWizardPage>
#define WIZARD_PAGE_DECL(name) \
    ConfigManager *_config; \
    public: \
    name(ConfigManager *config, QWidget *parent = nullptr); \
    ConfigManager *config() const; \
    void setConfig(ConfigManager *config);

#define WIZARD_PAGE_IMPL(name) \
    ConfigManager *name::config() const \
{ return _config; }\
    void name::setConfig(ConfigManager *config) \
{ _config = config; } \
    name::name(ConfigManager *config, QWidget *parent) : QWizardPage(parent), _config(config)

#define getWizard() qobject_cast<QtConfigWizard*>(wizard())

class QtConfigWizard;
class WizardPageBase : public QWizardPage
{
    Q_OBJECT

protected:
    ConfigManager *_config;

public:

    WizardPageBase(ConfigManager *config, QWidget *parent = nullptr);

    ConfigManager *config() const;
    void setConfig(ConfigManager *config);

    QtConfigWizard *wizard() const;
};

#endif // WIZARDPAGEBASE_H
