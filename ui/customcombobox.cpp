#include "customcombobox.h"


#include <QPainter>
#include <QDebug>
#include <QStyleOptionComboBox>
#include <QApplication>
#include <QPlainTextEdit>
#include <QShortcut>

CustomComboBox::CustomComboBox(QWidget *parent) : QPushButton(parent)
{
//    editor->installEventFilter(this);
    connect(this, &QPushButton::clicked, this, &CustomComboBox::me_click);

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
//        editor->clear();
        editor->close();
    });


}

void CustomComboBox::me_click()
{
    if (!editor) {
        editor = createEditor();
        editor->setWindowFlag(Qt::Popup);
    }

    editor->move(mapToGlobal(QPoint(0, 0)));
    editor->resize(width(), height() * 4);
    editor->show();
    editor->setFocus();
}

void CustomComboBox::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    QStyleOptionComboBox style;
    style.init(this);
    style.editable = false;
    style.frame = false;
//    auto parts = text()editor->toPlainText().split('\n');
//    int itemCount = 0;
//    foreach (QString s, parts)
//        if (!s.isEmpty())
//            itemCount++;

//    switch (itemCount) {
//    case 0:
//        style.currentText = "No item";
//        break;
//    case 1:
//        style.currentText = "1 item";
//        break;
//    default:
//        style.currentText = QString("%1 items").arg(itemCount);
//        break;
//    }
    style.currentText = text();

    qApp->style()->drawComplexControl(QStyle::CC_ComboBox, &style, &p);
    qApp->style()->drawControl(QStyle::CE_ComboBoxLabel, &style, &p);
}

bool CustomComboBox::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == editor) {
        if (event->type() == QEvent::FocusOut)
            editor->close();
    }

    return QWidget::eventFilter(watched, event);
}

