#include "featuresmodel.h"
#include <configmanager.h>
#include <QDebug>
#include <QColor>

FeaturesModel::FeaturesModel(ConfigManager *parent) : QAbstractItemModel(parent)
        ,_config(parent), d(parent->d), rootNode(new Node(Node::Root))
{
    connect(_config, &ConfigManager::configuresUpdated,
            this, &FeaturesModel::config_configuresUpdated);
}

FeaturesModel::~FeaturesModel()
{
    delete rootNode;
}

int FeaturesModel::rowCount(const QModelIndex &parent) const
{
    Node *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootNode;
    else
        parentItem = static_cast<Node*>(parent.internalPointer());

    return parentItem->childs.count();
}

int FeaturesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant FeaturesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();


    if (role == Qt::DisplayRole && index.column() == 0) {
        Node *item = static_cast<Node*>(index.internalPointer());

        return item->title;
    }
    if (index.column() == 1) {
        Node *item = static_cast<Node*>(index.internalPointer());
        if (item->type == Node::Feature) {
            auto state = d->featuresStates.value(item->feature->name, Qt::PartiallyChecked);

            if (role == Qt::DisplayRole) {
                switch (state) {
                case Qt::Checked: return "Include";
                case Qt::Unchecked: return "Exclude";
                case Qt::PartiallyChecked: return QVariant();
                }
            }
            if (role == Qt::DecorationRole) {
                switch (state) {
                case Qt::Checked: return QColor(Qt::green);
                case Qt::Unchecked: return QColor(Qt::red);
                case Qt::PartiallyChecked: return QVariant();
                }
            }
        }
    }

    return QVariant();
}

void FeaturesModel::config_configuresUpdated()
{
    beginRemoveRows(QModelIndex(), 0, rootNode->childs.count() - 1);
    rootNode->clear();
    endRemoveRows();

    QList<Feature*> opts = _config->features();
    std::sort(opts.begin(), opts.end(), [](const Feature *l, const Feature *r) -> bool{
        if (l->moduleName < r->moduleName)
            return true;
        if (l->moduleName == r->moduleName) {
            if (l->section.isEmpty() || r->section.isEmpty())
                return l->section.size() > r->section.size();

            if (l->section < r->section)
                return true;

            if (l->section == r->section)
                return l->name < r->name;
        }
        return false;
    });
    QMap<QString, Node*> modules;
    auto i = opts.begin();
    while (i != opts.end()) {
        Node *moduleNode;
        if (modules.contains((*i)->moduleName)) {
            moduleNode = modules.value((*i)->moduleName);
        } else {
            moduleNode = rootNode->createChild(Node::Module);
//            moduleNode->row = rootNode->childs.count();
            moduleNode->title = (*i)->moduleName;
            modules.insert(moduleNode->title, moduleNode);
            rootNode->childs.append(moduleNode);
        }

        Node *parent = nullptr;
        if ((*i)->section.isEmpty()) {
            parent = moduleNode;
        } else {
            Node *sectionNode = moduleNode->find((*i)->section);
            if (!sectionNode) {
                sectionNode = moduleNode->createChild(Node::Section);
                sectionNode->title = (*i)->section;
                moduleNode->childs.append(sectionNode);
            }
            parent = sectionNode;
        }
        if (!parent)
            qFatal("No parent");

        auto featureNode = parent->createChild(Node::Feature);
        featureNode->feature = *i;
        featureNode->title = (*i)->name;
        parent->childs.append(featureNode);
        ++i;
    }

    beginInsertRows(QModelIndex(), 0, rootNode->childs.count() - 1);
    endInsertRows();
}

Feature *FeaturesModel::feature(const QModelIndex &index) const
{
    return static_cast<Node*>(index.internalPointer())->feature;
}

void FeaturesModel::setState(const QString &name, const Qt::CheckState &state)
{
    std::function<Node*(Node*)> find;
    find = [&find, name](Node *node) -> Node*{
        if (node->type == Node::Feature && name == node->title)
            return node;

        auto i = node->childs.begin();
        while (i != node->childs.end()) {
            auto tmp = find(*i);
            if (tmp)
                return tmp;
            ++i;
        }
        return nullptr;
    };

    auto node = find(rootNode);
    if (node) {
        auto s1 = data(index(node, 0), Qt::DisplayRole).toString();
        auto s2 = node->title;
//        if (s1 != s2)
            qDebug() << s1 << " != " << s2;

        d->featuresStates.insert(node->feature->name, state);
        emit dataChanged(index(node, 0), index(node, 1));
    }
}

void FeaturesModel::setState(const QModelIndex &index, const Qt::CheckState &state)
{
    auto node = static_cast<Node*>(index.internalPointer());
    if (node->type != Node::Feature)
        return;

    d->featuresStates.insert(node->feature->name, state);
    emit dataChanged(index, index);
}

QModelIndex FeaturesModel::index(const FeaturesModel::Node *node, int col) const
{
    if (node->type == Node::Module)
        return index(node->row, col, QModelIndex());

    return index(node->row, col, index(node->parent, col));
}

QModelIndex FeaturesModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    Node *parentItem;

    if (!parent.isValid())
        parentItem = rootNode;
    else
        parentItem = static_cast<Node*>(parent.internalPointer());

    Node *childItem = parentItem->childs.at(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex FeaturesModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    Node *childItem = static_cast<Node*>(index.internalPointer());
    Node *parentItem = childItem->parent;

    if (parentItem == rootNode)
        return QModelIndex();

    return createIndex(parentItem->row, 0, parentItem);
}

Qt::ItemFlags FeaturesModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant FeaturesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
//    if (role == Qt::SizeHintRole && orientation == Qt::Horizontal) {
//        if (section == 0) {
//            qDebug() << "****";
//            return Qt::MinimumDescent;
//        }
//        else if (section == 1)
//            return Qt::MinimumSize;
//    }
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        if (section == 0)
            return "Name";
        else
            return "Status";
    } else {
        return section + 1;
    }
}
