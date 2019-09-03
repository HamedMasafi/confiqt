#include "wizard.h"

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QKeyEvent>
#include <QProcess>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include "configmanager.h"
#include "optionsselectdelegate.h"

#define checkCurrentId(p) currentId() == static_cast<int>(p)
Wizard::Wizard(QWidget *parent) :
    QWizard(parent)
{
    setupUi(this);
    _config = new ConfigManager;

    featuresModel = new QStandardItemModel(this);
    optionsModel = new QStandardItemModel(this);

    featuresFilter = new QSortFilterProxyModel(this);
    featuresFilter->setFilterKeyColumn(0);
    tableViewFeatures->setModel(featuresFilter);
    tableViewFeatures->installEventFilter(this);

    optionsFilter = new QSortFilterProxyModel(this);
    optionsFilter->setFilterKeyColumn(0);
    tableViewOptions->setModel(optionsFilter);
    tableViewOptions->setItemDelegateForColumn(2, new OptionsSelectDelegate(this));

    for (int i = 0; i < listWidgetNoMake->count(); ++i) {
        auto item = listWidgetNoMake->item(i);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->setCheckState(Qt::Checked);
    }

}

void Wizard::on_pushButtonSelectSourcePath_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this);
    if (path != QString())
        lineEditSourcePath->setText(path);
}

void Wizard::on_pushButtonSelectBuildPath_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this);
    if (path != QString())
        lineEditBuildPath->setText(path);
}

void Wizard::on_pushButtonSelectInstallPath_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this);
    if (path != QString())
        lineEditInstallPath->setText(path);
}

bool Wizard::validateCurrentPage()
{
    switch (static_cast<PageIndex>(currentId())) {

    case PageIndex::Path: {
        QDir d;
        if (!d.exists(lineEditBuildPath->text())
                || !d.exists(lineEditSourcePath->text())
                || !d.exists(lineEditInstallPath->text()))
            return false;

        if (lineEditSourcePath->text() == lineEditInstallPath->text())
            return false;

        if (!checkBoxConfirmLicense->isChecked())
            return false;

        _config->setSourcePath(lineEditSourcePath->text());
        _config->setBuildPath(lineEditBuildPath->text());
        _config->setInstallPath(lineEditInstallPath->text());

        _config->setConfirmLicense(checkBoxConfirmLicense->isChecked());
        _config->setUseCommercial(comboBoxLicenseType->currentIndex() == 1);
        return true;
    }

    case PageIndex::Modules: {
        QStringList modules;
        for (int i = 0; i < listWidgetModules->count(); ++i) {
            auto item = listWidgetModules->item(i);
            if (item->checkState() == Qt::Checked)
                modules.append(item->text());
        }
        if (!modules.count())
            return false;
        _config->setSelectedModules(modules);
        _config->process();
        break;
    }

    case PageIndex::Features: {
        _config->clearFeatureStates();
        for (int i = 0; i < featuresModel->rowCount(); ++i) {
            Qt::CheckState state = featuresModel->data(featuresModel->index(i, 4), CheckStateRole).value<Qt::CheckState>();
            QString name = featuresModel->data(featuresModel->index(i, 0)).toString();

            if (state != Qt::PartiallyChecked)
                _config->setFeatureState(name, state);
        }
        break;
    }

    case PageIndex::Options: {
        _config->clearOptionsStates();
        for (int i = 0; i < optionsModel->rowCount(); ++i) {
            QString state = optionsModel->data(optionsModel->index(i, 2), Qt::DisplayRole).toString();
            QString name = optionsModel->data(optionsModel->index(i, 0)).toString();

            if (state != QString())
                _config->setOptionsState(name, state);
        }
        break;
    }

    default:
        break;
    }
    return true;
}

void Wizard::initializePage(int id)
{
    switch (static_cast<PageIndex>(id)) {

    case PageIndex::Platform: {
        listWidgetPlatform->clear();
        listWidgetPlatform->addItems(_config->platforms());
        break;
    }

    case PageIndex::XPlatform: {
        listWidgetPlatform_2->clear();
        listWidgetPlatform_2->addItems(_config->platforms());
        break;
    }

    case PageIndex::Modules: {
        QStringList modules = _config->submodules();
        listWidgetModules->clear();
        foreach (QString module, modules) {
            QListWidgetItem *item = new QListWidgetItem(listWidgetModules);
            item->setText(module);
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            item->setCheckState(Qt::Checked);
            listWidgetModules->addItem(item);
        }
        break;
    }

    case PageIndex::Features: {
        QList<Feature> opts = _config->features();
        featuresModel->clear();
        featuresModel->setHorizontalHeaderLabels(QStringList() << "Name" << "Label"
                                         << "Purpose" << "Section" << "Value");
        featuresModel->setColumnCount(5);
        featuresModel->setRowCount(opts.count());
        for (int i = 0; i < opts.count(); ++i) {
            auto k = opts.at(i);
            featuresModel->setData(featuresModel->index(i, 0), k.name, Qt::DisplayRole);
            featuresModel->setData(featuresModel->index(i, 1), k.label, Qt::DisplayRole);
            featuresModel->setData(featuresModel->index(i, 2), k.purpose, Qt::DisplayRole);
            featuresModel->setData(featuresModel->index(i, 3), k.section, Qt::DisplayRole);
            featuresModel->setData(featuresModel->index(i, 4), " ", Qt::DisplayRole);
            featuresModel->setData(featuresModel->index(i, 4), Qt::PartiallyChecked, CheckStateRole);
        }
        featuresFilter->setSourceModel(featuresModel);
        break;
    }

    case PageIndex::Options: {
        QList<Option> opts = _config->options();
        optionsModel->clear();
        optionsModel->setHorizontalHeaderLabels(QStringList() << "Name" << "Type" << "Value");
        optionsModel->setColumnCount(3);
        optionsModel->setRowCount(opts.count());
        for (int i = 0; i < opts.count(); ++i) {
            auto k = opts.at(i);
            optionsModel->setData(optionsModel->index(i, 0), k.name);
            optionsModel->setData(optionsModel->index(i, 1), k.type);
            auto item = optionsModel->item(i, 0);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            item = optionsModel->item(i, 1);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);

            optionsModel->setData(optionsModel->index(i, 2), k.createValues(), DropDownRole);
            optionsModel->setData(optionsModel->index(i, 2), static_cast<int>(k.type), TypeRole);
            optionsModel->setData(optionsModel->index(i, 2), QVariant(), Qt::DisplayRole);
        }
        optionsFilter->setSourceModel(optionsModel);
        break;
    }

    case PageIndex::RunBuild:
        run();
        break;

    default:
        break;
    }
}

void Wizard::cleanupPage(int id)
{
    qDebug() << Q_FUNC_INFO;
    switch (static_cast<PageIndex>(id)) {


    default:
        break;
    }
}

bool Wizard::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == tableViewFeatures && event->type() == QEvent::KeyPress) {
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
            return QWizard::eventFilter(watched, event);
        }
    }

    return QWizard::eventFilter(watched, event);
}

int Wizard::nextId() const
{
    return QWizard::nextId();
}

void Wizard::on_lineEditFilterFeature_textChanged(const QString &s)
{
    featuresFilter->setFilterFixedString(s);
}

void Wizard::on_lineEditFilterOption_textChanged(const QString &s)
{
    optionsFilter->setFilterFixedString(s);
}

void Wizard::on_Wizard_accepted()
{
    qDebug() << _config->createCommand();
}

void Wizard::next_clicked()
{
    if (checkCurrentId(PageIndex::RunBuild)) {
//        run();
    }
}

void Wizard::run()
{
    auto p = _config->createProcess();
    connect(p, &QProcess::readyReadStandardError, [this, p](){
        auto b = p->readAllStandardError();
        plainTextEditBuildOutput->appendPlainText(b);
    });
    connect(p, &QProcess::readyReadStandardOutput, [this, p](){
        auto b = p->readAllStandardOutput();
        plainTextEditBuildOutput->appendPlainText(b);
    });
    connect(p, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [this](int code, QProcess::ExitStatus status){
        if (status == QProcess::NormalExit)
            plainTextEditBuildOutput->appendPlainText("Configure successful");
        else
            plainTextEditBuildOutput->appendPlainText(QString("Configure failed with code: %1").arg(code));
    });
    p->start();
}

void Wizard::on_pushButtonFeatureInclude_clicked()
{
    setSelectedFeaturesState(Qt::Checked);
}

void Wizard::on_pushButtonFeatureExclude_clicked()
{
    setSelectedFeaturesState(Qt::Unchecked);
}

void Wizard::on_pushButtonFeatureUncheck_clicked()
{
    setSelectedFeaturesState(Qt::PartiallyChecked);
}

void Wizard::setSelectedFeaturesState(const Qt::CheckState &state)
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

    foreach (QModelIndex idx, tableViewFeatures->selectionModel()->selectedRows(4)){
        auto i = featuresFilter->mapToSource(idx);
        featuresModel->setData(i, state, CheckStateRole);
        featuresModel->setData(i, text, Qt::DisplayRole);
        featuresModel->setData(i, color, Qt::DecorationRole);
    }
}
