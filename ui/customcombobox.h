#ifndef CUSTOMCOMBOBOX_H
#define CUSTOMCOMBOBOX_H

#include <QPushButton>
#include <QVariant>

class CustomComboBox : public QPushButton
{
    Q_OBJECT
    QWidget *editor;

public:
    CustomComboBox(QWidget *parent = nullptr);

public slots:
    void me_click();

protected:
    void paintEvent(QPaintEvent *event);
    virtual QWidget *createEditor() = 0;
    // QObject interface
public:
    bool eventFilter(QObject *watched, QEvent *event);
    QVariant value() const;
    void setValue(const QVariant &value);
};

#endif // CUSTOMCOMBOBOX_H
