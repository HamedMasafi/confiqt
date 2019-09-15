#include "featurefilterproxy.h"
#include "pagefeatures.h"
#include "global.h"
#include "condition.h"

#include <QKeyEvent>
#include <QMessageBox>
#include <QStandardItemModel>
#include <featuresmodel.h>

#ifdef WEB_REQUEST_LIB
#include <qtlitedialog.h>
#endif

PageFeatures::PageFeatures(ConfigManager *config, QWidget *parent)
    : WizardPageBase(config, parent)
{
    setupUi(this);

    featuresModel = _config->featuresModel();
    featuresFilter = new FeatureFilterProxy(this);
    featuresFilter->setFilterKeyColumn(0);
    featuresFilter->setSourceModel(featuresModel);
    treeView->installEventFilter(this);

    treeView->setModel(featuresFilter);
    connect(_config, &ConfigManager::configuresUpdated,
            this, &PageFeatures::config_configuresUpdated);

#ifndef WEB_REQUEST_LIB
    pushButtonQtLite->hide();
#endif
}

void PageFeatures::config_configuresUpdated()
{
    comboBoxModules->clear();
    comboBoxModules->addItem("");
    comboBoxModules->addItems(_config->selectedModules());

    treeView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    treeView->header()->setSectionResizeMode(1, QHeaderView::Interactive);
}

void PageFeatures::on_pushButtonFeatureInclude_clicked()
{
    setSelectedFeaturesState(Qt::Checked);
}

void PageFeatures::on_pushButtonFeatureExclude_clicked()
{
    setSelectedFeaturesState(Qt::Unchecked);
}

void PageFeatures::on_pushButtonFeatureUncheck_clicked()
{
    setSelectedFeaturesState(Qt::PartiallyChecked);
}

void PageFeatures::setSelectedFeaturesState(const Qt::CheckState &state)
{
    QString text;
    QVariant color;
    switch (state) {
    case Qt::Checked:
        text = "Yes";
        color = QColor(Qt::green);
        break;
    case Qt::Unchecked:
        text = "No";
        color = QColor(Qt::red);
        break;
    case Qt::PartiallyChecked:
        break;
    }

    auto selectionModel = treeView->selectionModel();
    foreach (QModelIndex idx, selectionModel->selectedIndexes()){
        auto mapped = featuresFilter->mapToSource(idx);
        featuresModel->setState(mapped, state);
    }
}

bool PageFeatures::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == treeView && event->type() == QEvent::KeyPress) {
        auto e = dynamic_cast<QKeyEvent*>(event);
        switch (e->key()) {
        case Qt::Key_Plus:
            setSelectedFeaturesState(Qt::Checked);
            break;
        case Qt::Key_Minus:
            setSelectedFeaturesState(Qt::Unchecked);
            break;
        case Qt::Key_Equal:
            setSelectedFeaturesState(Qt::PartiallyChecked);
            break;
        default:
            break;
        }
    }

    return WizardPageBase::eventFilter(watched, event);
}

void PageFeatures::checkItem(QStandardItem *item) {
    for (int i = 0; i < item->rowCount(); ++i) {
        auto child = item->child(i, 0);
        checkItem(child);
    }
    QVariant v = item->data(DataRole);
    if (v.isValid()) {
        QModelIndex idx = featuresModel->index(item->index().row(), 1, item->index().parent());
        Feature *ft = v.value<Feature*>();
        Qt::CheckState state = featuresModel->data(idx, CheckStateRole).value<Qt::CheckState>();
        QString name = ft->name;

        if (state != Qt::PartiallyChecked)
            _config->setFeatureState(name, state);
    }
}
bool PageFeatures::validatePage()
{
    _config->clearFeatureStates();
//    for (int i = 0; i < featuresModel->rowCount(); ++i) {
//        auto moduleItem = featuresModel->item(i, 0);
//        checkItem(moduleItem);
//    }
    return true;
}

void PageFeatures::on_treeView_activated(const QModelIndex &index)
{
    auto ft = featuresModel->feature(featuresFilter->mapToSource(index));
    if (ft) {
        auto condition = ft->condition.join(" && ");
        cond = Condition(condition, _config);
        cond.check();
        labelName->setText(ft->name);
        labelLabel->setText(ft->label);
        labelSection->setText(ft->section);
        labelPurpose->setText(ft->purpose);
        labelCondition->setText(condition);
        auto requirement = cond.requirement();
        if (requirement.size())
            labelRequires->setText(requirement.join(", "));
        else
            labelRequires->setText("None");
//        if (condition.isEmpty())
//            labelCondition->clear();
//        else {
            bool cc = cond.result();
            labelCondition->setText(QString(cc ? "True" : "False") + " (<a href=#>Explain</a>)");
//        }
//        qDebug() << cond.check() << condition;
    }
}

void PageFeatures::on_lineEditFilterFeature_textChanged(const QString &s)
{
    featuresFilter->setSearchName(s);
}

void PageFeatures::on_comboBoxModules_currentTextChanged(const QString &s)
{
    featuresFilter->setModuleName(s);
}

void PageFeatures::on_labelCondition_linkActivated(const QString &link)
{
    Q_UNUSED(link)
    QMessageBox::information(this, "Condition result",
                             cond.cond() + "\n" +
                             cond.toString());
}

void PageFeatures::on_pushButtonQtLite_clicked()
{
#ifdef WEB_REQUEST_LIB
    QtLiteDialog d(this);
    if (d.exec() == QDialog::Accepted) {
        auto fs = d.features();
        foreach (QString f, fs)
            _config->setFeatureState(f, Qt::Unchecked);
        config_configuresUpdated();
    }
#endif
}
