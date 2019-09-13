#include "abstractoptioneditor.h"

AbstractOptionEditor::AbstractOptionEditor()
{

}

AbstractOptionEditor::~AbstractOptionEditor()
{

}

QVariant AbstractOptionEditor::value() const
{
    return _value;
}

void AbstractOptionEditor::setValue(const QVariant &value)
{
    _value = value;
}

void AbstractOptionEditor::setDropDown(const QStringList &list)
{
    Q_UNUSED(list)
}

void AbstractOptionEditor::reset()
{
    _value = QVariant();
}