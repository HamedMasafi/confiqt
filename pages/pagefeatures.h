#ifndef PAGEFEATURES_H
#define PAGEFEATURES_H

#include "ui_pagefeatures.h"
#include "wizardpagebase.h"

#include <condition.h>

class FeatureFilterProxy;
class FeaturesModel;
class QStandardItem;
class PageFeatures : public WizardPageBase, private Ui::PageFeatures
{
    Q_OBJECT

    FeatureFilterProxy *featuresFilter;
    FeaturesModel *featuresModel;

    void checkItem(QStandardItem *item);
    Condition cond;
public:
    explicit PageFeatures(ConfigManager *config, QWidget *parent = nullptr);

private slots:
    void config_configuresUpdated();
    void on_pushButtonFeatureInclude_clicked();
    void on_pushButtonFeatureExclude_clicked();
    void on_pushButtonFeatureUncheck_clicked();
    void setSelectedFeaturesState(const Qt::CheckState &state);

    // QObject interface
    void on_treeView_activated(const QModelIndex &index);

    void on_lineEditFilterFeature_textChanged(const QString &arg1);

    void on_comboBoxModules_currentTextChanged(const QString &arg1);

    void on_labelCondition_linkActivated(const QString &link);

    void on_pushButtonQtLite_clicked();

public:
    bool eventFilter(QObject *watched, QEvent *event);

    // QWizardPage interface
public:
    bool validatePage();

};

#endif // PAGEFEATURES_H
