#ifndef QTCONFIGWIZARD_H
#define QTCONFIGWIZARD_H

#include "ui_qtconfigwizard.h"

#define FOREACH_PAGES(x) \
    x(Welcome) \
    x(SelectPaths) \
    x(License) \
    x(Common) \
    x(SelectPlatform) \
    x(CrossCompile) \
    x(Modules) \
    x(NoMake) \
    x(Features) \
    x(Libs) \
    x(SelectBuildMethod) \
    x(Run) \
    x(Finish)

#define x(name) class Page##name;
    FOREACH_PAGES(x)
#undef x

class ConfigManager;
class QtConfigWizard : public QWizard, private Ui::QtConfigWizard
{
    Q_OBJECT
    ConfigManager *_config;

    QList<int> _simpleIds;


public:
#define x(name) int _index##name; Page##name *_page##name;
    FOREACH_PAGES(x)
#undef x

    explicit QtConfigWizard(QWidget *parent = nullptr);

private slots:
    void on_QtConfigWizard_accepted();

    // QWizard interface
    void on_QtConfigWizard_rejected();

public:
    int nextId() const;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // QTCONFIGWIZARD_H
