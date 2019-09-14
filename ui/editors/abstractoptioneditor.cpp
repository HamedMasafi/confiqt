#include "abstractoptioneditor.h"

#include <QHBoxLayout>
#include <QToolButton>
#include <QDebug>

AbstractOptionEditor::AbstractOptionEditor(QWidget *parent) : QWidget(parent), created(false)
{
    setStyleSheet(R"(
                  QCheckBox{
                    background-color: rgb(255, 255, 255);
                  }
                  QCheckBox::indicator {
                    left: 10px;
                  }
                  )");

}

AbstractOptionEditor::~AbstractOptionEditor()
{

}

QVariant AbstractOptionEditor::value() const
{
    return _value;
}

QString AbstractOptionEditor::text() const
{
    return value().toString();
}

QWidget *AbstractOptionEditor::createWidget()
{
    return nullptr;
}

void AbstractOptionEditor::setValue(const QVariant &value)
{
    _value = value;
}

void AbstractOptionEditor::setDropDown(const QStringList &list)
{
    Q_UNUSED(list)
}

void AbstractOptionEditor::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    if (!created) {

        auto editor = createWidget();

        if (editor) {
            auto l = new QHBoxLayout(this);
            l->setMargin(0);
            l->setSpacing(0);

            editor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            l->addWidget(editor);

            auto resetButton = new QToolButton(this);
            resetButton->setArrowType(Qt::LeftArrow);
            resetButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
            l->addWidget(resetButton);

            setLayout(l);

            connect(resetButton, &QAbstractButton::clicked, this, &AbstractOptionEditor::reset);
        } else {
            qDebug() << "Editor is not ready";
        }
        created = true;
    }
}

void AbstractOptionEditor::reset()
{
    setValue(QVariant());
}
