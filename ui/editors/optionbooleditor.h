#ifndef OPTIONBOOLEDITOR_H
#define OPTIONBOOLEDITOR_H

#include "abstractoptioneditor.h"
#include <QCheckBox>

class QCheckBox;
class OptionBoolEditor : public AbstractOptionEditor
{
    Q_OBJECT
    QCheckBox *editor;

public:
    OptionBoolEditor(QWidget *parent = nullptr);
    QVariant value() const override;
    void setValue(const QVariant &value) override;
    QWidget *createWidget() override;
    QString text() const override;

private slots:
    void me_stateChanged(int);
};

#endif // OPTIONBOOLEDITOR_H
