#ifndef PATHSELECTEDITOR_H
#define PATHSELECTEDITOR_H

#include "abstractoptioneditor.h"

class QLineEdit;
class PathSelectEditor : public AbstractOptionEditor
{
    Q_OBJECT
    QLineEdit *lineEdit;
    QWidget *editor;

public:
    PathSelectEditor(QWidget *parent = nullptr);

    // AbstractOptionEditor interface
public:
    QVariant value() const;
    void setValue(const QVariant &value);
private slots:
    void button_clicked();

    // AbstractOptionEditor interface
public:
    QWidget *createWidget();
};

#endif // PATHSELECTEDITOR_H
