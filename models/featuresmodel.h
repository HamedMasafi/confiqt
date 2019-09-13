#ifndef FEATURESMODEL_H
#define FEATURESMODEL_H

#include <QAbstractItemModel>
#include <QExplicitlySharedDataPointer>
#include <configdata.h>

class ConfigManager;
struct Feature;
class FeaturesModel : public QAbstractItemModel
{
    Q_OBJECT
    ConfigManager *_config;
    QExplicitlySharedDataPointer<ConfigData> d;

    struct Node {
        enum Type {
            Root,
            Module,
            Section,
            Feature
        };

        QString title;
        QList<Node*> childs;
        Node *parent;
        struct Feature *feature;
        Type type;
        int row;

        Node(Type type) : parent(nullptr), feature(nullptr), type(type)
        {}
        Node(Node *parent, Type type) : parent(parent), feature(nullptr), type(type)
        {
            row = parent->childs.count();
        }

        ~Node() {
            qDeleteAll(childs);
        }

        Node *createChild(Type type) {
            auto ch = new Node(type);
            ch->parent = this;
            ch->row = childs.count();
            return ch;
        }
        Node *find(const QString &title) {
            auto i = childs.begin();
            while (i != childs.end()) {
                if ((*i)->title == title)
                    return *i;
                ++i;
            }
            return nullptr;
        }

        void clear() {
            qDeleteAll(childs);
            childs.clear();
        }
    };

    Node *rootNode;

public:
    FeaturesModel(ConfigManager *parent);
    virtual ~FeaturesModel() override;

private slots:
    void config_configuresUpdated();

public:
    Feature *feature(const QModelIndex &index) const;
    void setState(const QString &name, const Qt::CheckState &state);
    void setState(const QModelIndex &index, const Qt::CheckState &state);
    QModelIndex index(const Node *node, int col) const;

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    friend class FeatureFilterProxy;
};

#endif // FEATURESMODEL_H
