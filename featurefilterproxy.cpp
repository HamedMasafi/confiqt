#include "global.h"
#include "featurefilterproxy.h"

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
    FeatureTreeNodeType type = static_cast<FeatureTreeNodeType>(sourceModel()->data(index, TypeRole).toInt());
    bool result = false;

    if (type == FeatureTreeNodeType::Module || type == FeatureTreeNodeType::Section) {
        if (type == FeatureTreeNodeType::Module
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

    if (type != FeatureTreeNodeType::Feature)
        return true;

    if (m_searchName.isEmpty())
        return true;

    QVariant v = sourceModel()->data(index, DataRole);
    Feature ft = v.value<Feature>();
    return ft.name.contains(m_searchName, Qt::CaseInsensitive);

}
