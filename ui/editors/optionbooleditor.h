#ifndef OPTIONBOOLEDITOR_H
#define OPTIONBOOLEDITOR_H

#include "abstractoptioneditor.h"
#include <QCheckBox>

class OptionBoolEditor : public QCheckBox, public AbstractOptionEditor
{
public:
    OptionBoolEditor();

    // AbstractOptionEditor interface
public:
    QVariant value() const;
    void setValue(const QVariant &value);
};

#endif // OPTIONBOOLEDITOR_H
