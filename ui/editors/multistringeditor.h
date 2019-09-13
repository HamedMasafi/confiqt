#ifndef MULTISTRINGEDITOR_H
#define MULTISTRINGEDITOR_H

#include "abstractoptioneditor.h"
#include "customcombobox.h"

class QPlainTextEdit;
class MultiStringEditor : public CustomComboBox, public AbstractOptionEditor
{
    Q_OBJECT
    QPlainTextEdit *editor;

public:
    MultiStringEditor();

    // CustomComboBox interface
protected:
    QWidget *createEditor();

    // AbstractOptionEditor interface
public:
    QVariant value() const;
    void setValue(const QVariant &value);
private slots:
    void editor_textChanged();
};

#endif // MULTISTRINGEDITOR_H
