#include "pagefeatures.h"

#include <QKeyEvent>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>

PageFeatures::PageFeatures(ConfigManager *config, QWidget *parent)
    : WizardPageBase(config, parent)
{
    setupUi(this);

    featuresModel = new QStandardItemModel(this);
    featuresFilter = new QSortFilterProxyModel(this);
    featuresFilter->setFilterKeyColumn(0);
    treeView->setModel(featuresFilter);
    treeView->installEventFilter(this);
    connect(_config, &ConfigManager::configuresUpdated,
            this, &PageFeatures::config_configuresUpdated);
}

void PageFeatures::config_configuresUpdated()
{
    QList<Feature> opts = _config->features();
    std::sort(opts.begin(), opts.end(), [](const Feature &l, const Feature &r) -> bool{
        return l.moduleName < r.moduleName && l.section < r.section;
    });

    QMap<QString, QStandardItem*> rootItems;
    featuresModel->clear();
    featuresModel->setHorizontalHeaderLabels(QStringList() << "Name" << "Value");
    featuresModel->setColumnCount(2);

    for (int i = 0; i < opts.count(); ++i) {
        auto k = opts.at(i);

        QStandardItem *moduleItem;
        if (rootItems.contains(k.moduleName)) {
            moduleItem = rootItems.value(k.moduleName);
        } else {
            moduleItem = new QStandardItem(k.moduleName);
            moduleItem->setColumnCount(2);
            rootItems.insert(k.moduleName, moduleItem);
            featuresModel->appendRow(moduleItem);
        }

        QStandardItem *item = new QStandardItem(k.name);
        item->setData(QVariant::fromValue(k), DataRole);

        QStandardItem *checkItem = new QStandardItem;
        checkItem->setData(Qt::PartiallyChecked, CheckStateRole);


        if (k.section.isEmpty()) {
            moduleItem->appendRow(QList<QStandardItem*>()<< item << checkItem);
        } else {
            QStandardItem *sectionItem = nullptr;
            for (int i = 0; i < moduleItem->rowCount(); ++i) {
                if (moduleItem->child(i)->text() == k.section) {
                    sectionItem = moduleItem->child(i);
                    break;
                }
            }
            if (sectionItem == nullptr) {
                sectionItem = new QStandardItem(k.section);
                sectionItem->setColumnCount(2);
                moduleItem->appendRow(sectionItem);
            }
            sectionItem->appendRow(QList<QStandardItem*>()<< item << checkItem);
        }
    }
    featuresFilter->setSourceModel(featuresModel);
    treeView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
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
        auto i = featuresModel->index(mapped.row(), 1, mapped.parent());
        auto v = featuresModel->index(mapped.row(), 0, mapped.parent()).data(DataRole);
        if (v.isValid()) {
            featuresModel->setData(i, state, CheckStateRole);
            featuresModel->setData(i, text, Qt::DisplayRole);
            featuresModel->setData(i, color, Qt::DecorationRole);
        }
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
        Feature ft = v.value<Feature>();
        Qt::CheckState state = featuresModel->data(idx, CheckStateRole).value<Qt::CheckState>();
        QString name = ft.name;

        if (state != Qt::PartiallyChecked)
            _config->setFeatureState(name, state);
    }
}
bool PageFeatures::validatePage()
{
    _config->clearFeatureStates();
    for (int i = 0; i < featuresModel->rowCount(); ++i) {
        auto moduleItem = featuresModel->item(i, 0);
        checkItem(moduleItem);
    }
    return true;
}

void PageFeatures::on_treeView_activated(const QModelIndex &index)
{
    QVariant v = featuresFilter->mapToSource(index).data(DataRole);
    if (v.isValid()) {
        Feature ft = v.value<Feature>();
        labelLabel->setText(ft.label);
        labelSection->setText(ft.section);
        labelPurpose->setText(ft.purpose);
    }
}

void PageFeatures::on_lineEditFilterFeature_textChanged(const QString &s)
{
    featuresFilter->setFilterFixedString(s);
}
