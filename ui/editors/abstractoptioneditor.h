#ifndef ABSTRACTOPTIONEDITOR_H
#define ABSTRACTOPTIONEDITOR_H

#include <QVariant>
#include <QWidget>

class AbstractOptionEditor : public QWidget
{
    Q_OBJECT

    QVariant _value;
    bool created;

public:
    AbstractOptionEditor(QWidget *parent = nullptr);
    virtual ~AbstractOptionEditor();
    virtual QVariant value() const;
    virtual QString text() const;
    virtual QWidget *createWidget();

public slots:
    virtual void reset();
    virtual void setValue(const QVariant &value);
    virtual void setDropDown(const QStringList &list);

    // QWidget interface
protected:
    void showEvent(QShowEvent *event);
};

#endif // ABSTRACTOPTIONEDITOR_H
