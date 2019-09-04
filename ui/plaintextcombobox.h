#ifndef PLAINTEXTCOMBOBOX_H
#define PLAINTEXTCOMBOBOX_H

#include <QPushButton>
#include <QVariant>


class QPlainTextEdit;
class PlainTextComboBox : public QPushButton
{
    Q_OBJECT
    QPlainTextEdit *editor;

public:
    PlainTextComboBox(QWidget *parent = nullptr);

public slots:
    void me_click();

protected:
    void paintEvent(QPaintEvent *event);

    // QObject interface
public:
    bool eventFilter(QObject *watched, QEvent *event);
    QVariant value() const;
    void setValue(const QVariant &value);
};

#endif // PLAINTEXTCOMBOBOX_H
