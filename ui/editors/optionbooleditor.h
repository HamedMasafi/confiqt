#ifndef OPTIONBOOLEDITOR_H
#define OPTIONBOOLEDITOR_H

#include "abstractoptioneditor.h"
#include <QCheckBox>

class OptionBoolEditor : public QCheckBox, public AbstractOptionEditor
{
    Q_OBJECT

public:
    OptionBoolEditor();

    // AbstractOptionEditor interface
public:
    QVariant value() const;
    void setValue(const QVariant &value);
private slots:
    void me_stateChanged(int);
};

#endif // OPTIONBOOLEDITOR_H
