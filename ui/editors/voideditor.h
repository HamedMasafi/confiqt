#ifndef VOIDEDITOR_H
#define VOIDEDITOR_H

#include "abstractoptioneditor.h"

class QCheckBox;
class VoidEditor : public AbstractOptionEditor
{
    Q_OBJECT
    QCheckBox *editor;

public:
    VoidEditor(QWidget *parent = nullptr);

public:
    QVariant value() const override;
    void setValue(const QVariant &value) override;
    QWidget *createWidget() override;
    QString text() const override;

private slots:
    void me_toggled(bool);
};

#endif // VOIDEDITOR_H
