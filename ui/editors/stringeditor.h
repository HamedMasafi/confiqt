#ifndef STRINGEDITOR_H
#define STRINGEDITOR_H

#include "abstractoptioneditor.h"

#include <QLineEdit>

class StringEditor : public QLineEdit, public AbstractOptionEditor
{
    Q_OBJECT

public:
    StringEditor();
};

#endif // STRINGEDITOR_H
