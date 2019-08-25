#ifndef WIZARD_H
#define WIZARD_H

#include "ui_wizard.h"

class ConfigManager;
class QSortFilterProxyModel;
class QStandardItemModel;
class Wizard : public QWizard, private Ui::Wizard
{
    Q_OBJECT
    ConfigManager *_config;
    QSortFilterProxyModel *featuresFilter;
    QSortFilterProxyModel *optionsFilter;
    QStandardItemModel *featuresModel;
    QStandardItemModel *optionsModel;

    enum class PageIndex {
        Path,
        Platform,
        XPlatform,
        Modules,
        ExcludeMakes,
        Features,
        Options,
        SelectBuildMode,
        RunBuild
    };


public:
    enum Roles {
        CheckStateRole = Qt::UserRole,
        DropDownRole,
        TypeRole
    };
    explicit Wizard(QWidget *parent = nullptr);
private slots:
    void on_pushButtonSelectSourcePath_clicked();
    void on_pushButtonSelectBuildPath_clicked();
    void on_pushButtonSelectInstallPath_clicked();

    // QWizard interface

    void on_lineEditFilterFeature_textChanged(const QString &arg1);

    void on_lineEditFilterOption_textChanged(const QString &arg1);

    void on_Wizard_accepted();
    void next_clicked();
    void run();

    void on_pushButtonFeatureInclude_clicked();

    void on_pushButtonFeatureExclude_clicked();

    void on_pushButtonFeatureUncheck_clicked();
    void setSelectedFeaturesState(const Qt::CheckState &state);
public:
    bool validateCurrentPage();

protected:
    void initializePage(int id);
    void cleanupPage(int id);

    // QObject interface
public:
    bool eventFilter(QObject *watched, QEvent *event);

    // QWizard interface
public:
    int nextId() const;
};

#endif // WIZARD_H
