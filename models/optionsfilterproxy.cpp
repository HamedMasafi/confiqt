#include "optionsfilterproxy.h"
#include "optionsmodel.h"
#include "option.h"

OptionsFilterProxy::OptionsFilterProxy(QObject *parent) : QSortFilterProxyModel(parent)
{

}

QString OptionsFilterProxy::searchName() const
{
    return m_searchName;
}

QString OptionsFilterProxy::moduleName() const
{
    return m_moduleName;
}

void OptionsFilterProxy::setSearchName(QString searchName)
{
    if (m_searchName == searchName)
        return;

    m_searchName = searchName;
    invalidate();
    emit searchNameChanged(m_searchName);
}

void OptionsFilterProxy::setModuleName(QString moduleName)
{
    if (m_moduleName == moduleName)
        return;

    m_moduleName = moduleName;
    invalidate();
    emit moduleNameChanged(m_moduleName);
}

bool OptionsFilterProxy::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (m_searchName.isEmpty() && m_moduleName.isEmpty())
        return true;

    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    return hasToBeDisplayed(index);
}

bool OptionsFilterProxy::hasToBeDisplayed(const QModelIndex index) const
{
    auto o = qobject_cast<OptionsModel*>(sourceModel())->option(index);
    if (!o)
        return false;

    if (!m_moduleName.isEmpty() && o->moduleName() != m_moduleName)
        return false;
    if (!m_searchName.isEmpty() && !o->name().contains(m_searchName))
        return false;

    return true;
}

