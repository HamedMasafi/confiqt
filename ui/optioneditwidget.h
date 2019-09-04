#ifndef OPTIONEDITDELEGATE_H
#define OPTIONEDITDELEGATE_H

#include "ui_optioneditwidget.h"
#include "global.h"

class OptionEditWidget : public QWidget, private Ui::OptionEditWidget
{
    Q_OBJECT
    bool reseted;
    Option::Type _type;

public:
    explicit OptionEditWidget(QWidget *parent = nullptr);

    QVariant value() const;
    Option::Type type() const;

    void setType(const Option::Type &type);
    void setDropDown(const QVariant &dropDownData);
    void setValue(const QVariant &value);

private slots:
    void valueSet();
    void on_toolButtonReset_clicked();
    void on_checkBox_stateChanged(int arg1);
};

#endif // OPTIONEDITDELEGATE_H
