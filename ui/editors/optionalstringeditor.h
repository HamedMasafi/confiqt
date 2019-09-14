#ifndef OPTIONALSTRINGEDITOR_H
#define OPTIONALSTRINGEDITOR_H

#include "abstractoptioneditor.h"

class QLineEdit;
class QCheckBox;
class OptionalStringEditor : public AbstractOptionEditor
{
    Q_OBJECT
    QLineEdit *lineEdit;
    QCheckBox *checkBox;
    QWidget *editor;

public:
    OptionalStringEditor(QWidget *parent = nullptr);

public:
    QVariant value() const override;
    QWidget *createWidget() override;

public slots:
    void setValue(const QVariant &value) override;
};

#endif // OPTIONALSTRINGEDITOR_H
