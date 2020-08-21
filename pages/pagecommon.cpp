#include "pagecommon.h"

PageCommon::PageCommon(ConfigManager *config, QWidget *parent) :
    WizardPageBase(config, parent)
{
    setupUi(this);

    connect(_config, &ConfigManager::configuresUpdated,
            this, &PageCommon::config_configuresUpdated);

    map.insert(checkBoxDeveloperBuild, "developer-build");
    map.insert(checkBoxStatic, "static");
    map.insert(checkBoxDebugAndRelease, "debug_and_release");
    map.insert(checkBoxReleaseTools, "release_tools");
    map.insert(checkBoxForceDebugInfo, "force_debug_info");

    map.insert(checkBoxBuildForDebug, "debug");
    map.insert(checkBoxSeprateDebugInfo, "separate_debug_info");

    map.insert(checkBoxOptimizeDebug, "optimize_debug");
    map.insert(checkBoxOptimizeForSize, "optimize_size");
}

void PageCommon::config_configuresUpdated()
{
    auto i = map.begin();
    while (i != map.end()) {
        i.key()->setCheckState(_config->featureState(i.value()));
        ++i;
    }
}

bool PageCommon::validatePage()
{
    auto i = map.begin();
    while (i != map.end()) {
        _config->setFeatureState(i.value(), i.key()->checkState());
        ++i;
    }

    return true;
}
