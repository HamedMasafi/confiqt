#include "plaintextcombobox.h"

#include <QPainter>
#include <QDebug>
#include <QStyleOptionComboBox>
#include <QApplication>
#include <QPlainTextEdit>
#include <QShortcut>

PlainTextComboBox::PlainTextComboBox(QWidget *parent) : QPushButton(parent)
{
    editor = new QPlainTextEdit(this);
    editor->setWindowFlag(Qt::Popup);
//    editor->installEventFilter(this);
    connect(this, &QPushButton::clicked, this, &PlainTextComboBox::me_click);

    QShortcut *closeShortcut = new QShortcut(this);
    closeShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    closeShortcut->setKey(Qt::CTRL + Qt::Key_Return);
    connect(closeShortcut, &QShortcut::activated, [this](){
        editor->close();
    });

    QShortcut *closeShortcut2 = new QShortcut(this);
    closeShortcut2->setContext(Qt::WidgetWithChildrenShortcut);
    closeShortcut2->setKey(Qt::CTRL + Qt::Key_Enter);
    connect(closeShortcut2, &QShortcut::activated, [this](){
        editor->close();
    });

    QShortcut *closeShortcut3 = new QShortcut(this);
    closeShortcut3->setContext(Qt::WidgetWithChildrenShortcut);
    closeShortcut3->setKey(Qt::Key_Escape);
    connect(closeShortcut3, &QShortcut::activated, [this](){
        editor->clear();
        editor->close();
    });


}

void PlainTextComboBox::me_click()
{
    editor->move(mapToGlobal(QPoint(0, 0)));
    editor->resize(width(), height() * 4);
    editor->show();
    editor->setFocus();
}

void PlainTextComboBox::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    QStyleOptionComboBox style;
    style.init(this);
    style.editable = false;
    style.frame = false;
    auto parts = editor->toPlainText().split('\n');
    int itemCount = 0;
    foreach (QString s, parts)
        if (!s.isEmpty())
            itemCount++;

    switch (itemCount) {
    case 0:
        style.currentText = "No item";
        break;
    case 1:
        style.currentText = "1 item";
        break;
    default:
        style.currentText = QString("%1 items").arg(itemCount);
        break;
    }

    qApp->style()->drawComplexControl(QStyle::CC_ComboBox, &style, &p);
    qApp->style()->drawControl(QStyle::CE_ComboBoxLabel, &style, &p);
}

bool PlainTextComboBox::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == editor) {
        if (event->type() == QEvent::FocusOut)
            editor->close();
    }

    return QWidget::eventFilter(watched, event);
}

QVariant PlainTextComboBox::value() const
{
    QVariantList v;
    foreach (QString s, editor->toPlainText().split('\n'))
        if (!s.isEmpty())
            v.append(s);
    return v;
}

void PlainTextComboBox::setValue(const QVariant &value)
{
    if (value.type() == QVariant::List) {
        QString text;
        auto list = value.toList();
        foreach (QVariant v, list)
            if (v.type() == QVariant::String) {
                if (!text.isEmpty())
                    text.append('\n');
                text.append(v.toString());
            }
        editor->setPlainText(text);
    }
}
