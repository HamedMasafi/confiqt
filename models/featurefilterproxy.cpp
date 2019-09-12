#include "global.h"
#include "featurefilterproxy.h"
#include "featuresmodel.h"

FeatureFilterProxy::FeatureFilterProxy(QObject *parent) : QSortFilterProxyModel(parent)
{

}

QString FeatureFilterProxy::searchName() const
{
    return m_searchName;
}

QString FeatureFilterProxy::moduleName() const
{
    return m_moduleName;
}

void FeatureFilterProxy::setSearchName(QString searchName)
{
    if (m_searchName == searchName)
        return;

    m_searchName = searchName;
    invalidate();
    emit searchNameChanged(m_searchName);
}

void FeatureFilterProxy::setModuleName(QString moduleName)
{
    if (m_moduleName == moduleName)
        return;

    m_moduleName = moduleName;
    invalidate();
    emit moduleNameChanged(m_moduleName);
}

bool FeatureFilterProxy::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (m_searchName.isEmpty() && m_moduleName.isEmpty())
        return true;

    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    return hasToBeDisplayed(index);
}

bool FeatureFilterProxy::hasToBeDisplayed(const QModelIndex index) const
{
    auto node = static_cast<FeaturesModel::Node*>(index.internalPointer());
    bool result = false;

    if (node->type == FeaturesModel::Node::Module || node->type == FeaturesModel::Node::Section) {
        if (node->type == FeaturesModel::Node::Module
                && !m_moduleName.isEmpty()
                && sourceModel()->data(index).toString() != m_moduleName)
            return false;
        for( int i = 0; i < sourceModel()->rowCount(index); i++)
        {
            QModelIndex childIndex = sourceModel()->index(i, 0, index);
            if (!childIndex.isValid())
                break;
            result = hasToBeDisplayed(childIndex);
            if (result)
                break;
        }
        return result;
    }

    if (node->type != FeaturesModel::Node::Feature)
        return true;

    if (m_searchName.isEmpty())
        return true;

    return node->feature->name.contains(m_searchName, Qt::CaseInsensitive);
}
