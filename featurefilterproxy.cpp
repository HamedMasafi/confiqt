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

    QModelIndex index0 = sourceModel()->index(source_row, 0, source_parent);
    FeatureTreeNodeType type = static_cast<FeatureTreeNodeType>(sourceModel()->data(index0, TypeRole).toInt());

    if (type == FeatureTreeNodeType::Module && !m_moduleName.isEmpty())
        return sourceModel()->data(index0).toString() == m_moduleName;

    if (type != FeatureTreeNodeType::Feature)
        return true;

    if (m_searchName.isEmpty())
        return true;

    QVariant v = sourceModel()->data(index0, DataRole);
    Feature ft = v.value<Feature>();
    return ft.name.contains(m_searchName);
}
