#ifndef STRINGEDITOR_H
#define STRINGEDITOR_H

#include "abstractoptioneditor.h"

class QLineEdit;
class StringEditor : public AbstractOptionEditor
{
    Q_OBJECT
    QLineEdit *editor;

public:
    StringEditor(QWidget *parent = nullptr);

    // AbstractOptionEditor interface
public:
    QVariant value() const;
    QWidget *createWidget();

public slots:
    void reset();
    void setValue(const QVariant &value);
};

#endif // STRINGEDITOR_H
