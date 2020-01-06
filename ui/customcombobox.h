#ifndef CUSTOMCOMBOBOX_H
#define CUSTOMCOMBOBOX_H

#include <QPushButton>
#include <QVariant>

class CustomComboBox : public QPushButton
{
    Q_OBJECT
    QWidget *_editor;

public:
    CustomComboBox(QWidget *parent = nullptr);
    void setPopup(QWidget *popup);
public slots:
    void me_click();

protected:
    void paintEvent(QPaintEvent *event);

public:
    bool eventFilter(QObject *watched, QEvent *event);
    QVariant value() const;
    void setValue(const QVariant &value);

    // QWidget interface
protected:
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // CUSTOMCOMBOBOX_H
