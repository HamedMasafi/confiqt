#ifndef MULTISTRINGEDITOR_H
#define MULTISTRINGEDITOR_H

#include "abstractoptioneditor.h"
#include "customcombobox.h"

class QPlainTextEdit;
class CustomComboBox;
class MultiStringEditor : public AbstractOptionEditor
{
    Q_OBJECT
    CustomComboBox *comboBox;
    QPlainTextEdit *editor;

public:
    MultiStringEditor(QWidget *parent = nullptr);
    QVariant value() const override;
    void setValue(const QVariant &value) override;
    QWidget *createWidget() override;

private slots:
    void editor_textChanged();
};

#endif // MULTISTRINGEDITOR_H
