#ifndef OPTIONEDITDELEGATE_H
#define OPTIONEDITDELEGATE_H

#include "ui_optioneditwidget.h"

class OptionEditWidget : public QWidget, private Ui::OptionEditWidget
{
    Q_OBJECT
    bool reseted;
    QVariant::Type _type;

public:
    explicit OptionEditWidget(QWidget *parent = nullptr);

    QVariant value() const;
    QVariant::Type type() const;

    void setType(const QVariant::Type &type);
    void setDropDown(const QVariant &dropDownData);
    void setValue(const QVariant &value);

private slots:
    void valueSet();
    void on_toolButtonReset_clicked();
};

#endif // OPTIONEDITDELEGATE_H
