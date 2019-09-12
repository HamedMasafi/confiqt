#ifndef OPTIONSFILTERPROXY_H
#define OPTIONSFILTERPROXY_H

#include <QSortFilterProxyModel>

class OptionsFilterProxy : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(QString moduleName READ moduleName WRITE setModuleName NOTIFY moduleNameChanged)
    Q_PROPERTY(QString searchName READ searchName WRITE setSearchName NOTIFY searchNameChanged)

    QString m_searchName;

    QString m_moduleName;

    bool hasToBeDisplayed(const QModelIndex index) const;
public:
    OptionsFilterProxy(QObject *parent = nullptr);

    // QSortFilterProxyModel interface
    QString searchName() const;

    QString moduleName() const;

public slots:
    void setSearchName(QString searchName);

    void setModuleName(QString moduleName);

signals:
    void searchNameChanged(QString searchName);

    void moduleNameChanged(QString moduleName);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

};

#endif // OPTIONSFILTERPROXY_H
