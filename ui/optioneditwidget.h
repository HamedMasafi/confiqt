#ifndef OPTIONEDITDELEGATE_H
#define OPTIONEDITDELEGATE_H

#include "ui_optioneditwidget.h"
#include "global.h"
#include "option.h"

#define FOREACH_PAGE(x) \
    x(Bool) \
    x(Enum) \
    x(String) \
    x(MultilineString) \
    x(StringsSelection)

class QListWidget;
class Option;
class OptionEditWidget : public QWidget, private Ui::OptionEditWidget
{
    Q_OBJECT
    bool reseted;
    Option *_option;
    Option::Type _type;
    QListWidget *checkableListWidget;

#define x(name) int _index##name;
    FOREACH_PAGE(x)
#undef x

public:
    explicit OptionEditWidget(QWidget *parent = nullptr);

    QVariant value() const;
    Option::Type type() const;

    void setType(const Option::Type &type);
    void setDropDown(const QVariant &dropDownData);
    void setValue(const QVariant &value);

    Option *option() const;
    void setOption(Option *option);

private slots:
    void valueSet();
    void on_toolButtonReset_clicked();
    void on_checkBox_stateChanged(int arg1);
    void on_toolButtonBrowse_clicked();
};

#endif // OPTIONEDITDELEGATE_H
