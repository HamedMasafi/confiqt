#include <QDebug>
#include <QPainter>
#include <QPlainTextEdit>

#include "optioneditwidget.h"

OptionEditWidget::OptionEditWidget(QWidget *parent) :
    QWidget(parent), reseted(false)
{
    setupUi(this);
}

QVariant OptionEditWidget::value() const
{
    if (reseted)
        return QVariant();

    switch (_type) {
    case Option::Void:
        if (checkBox->isChecked())
            return true;
        else
            return QVariant();

    case Option::Bool:
        if (checkBox->checkState() == Qt::PartiallyChecked)
            return QVariant();
        return checkBox->isChecked();

    case Option::String:
    case Option::OptionalString:
        return _lineEdit->text();

    case Option::AddString:
        return plainEditComboStrings->value();

    case Option::Enum:
        return _comboBox->currentText();

    default:
        return QVariant();
    }
}

Option::Type OptionEditWidget::type() const
{
    return _type;
}

void OptionEditWidget::setValue(const QVariant &value)
{
    if (!value.isValid()) {
        on_toolButtonReset_clicked();
        return;
    }

    switch (_type) {
    case Option::Bool:
    case Option::Void:
        checkBox->setChecked(value.toBool());
        checkBox->setTristate(false);
        break;

    case Option::String:
        _lineEdit->setText(value.toString());
        break;

    case Option::Enum:
        _comboBox->setCurrentText(value.toString());
        break;

    case Option::AddString:
        plainEditComboStrings->setValue(value);
        break;

    default:
        break;
    }
}

void OptionEditWidget::setType(const Option::Type &type)
{
    _type = type;
    reseted =  false;

    switch (type) {
    case Option::Bool:
    case Option::Void:
        stackedWidget->setCurrentWidget(pageBool);
        toolButtonReset->show();
        break;
    case Option::Enum:
        stackedWidget->setCurrentWidget(pageEnum);
        toolButtonReset->show();
        break;

    case Option::String:
    case Option::OptionalString:
        stackedWidget->setCurrentWidget(pageString);
        toolButtonReset->show();
        break;

    case Option::AddString:
        stackedWidget->setCurrentWidget(pageMultilineString);
        break;

    case Option::Unknown:
        checkBox->hide();
        _lineEdit->hide();
        _comboBox->hide();
        toolButtonReset->hide();
        break;
    }
}

void OptionEditWidget::setDropDown(const QVariant &dropDownData)
{
    if (dropDownData.type() == QVariant::List) {
        _comboBox->clear();
        auto list = dropDownData.toList();
        foreach (QVariant v, list)
            _comboBox->addItem(v.toString());
    }
}

void OptionEditWidget::valueSet()
{
    reseted = false;
}

void OptionEditWidget::on_toolButtonReset_clicked()
{
//    reseted = true;
//    switch (_type) {
//    case Option::Bool:
//        checkBox->setCheckState(Qt::PartiallyChecked);
//        break;

//    case Option::String:
//    case Option::OptionalString:
//        _lineEdit->clear();
//        break;

//    case Option::Enum:
//        _comboBox->setCurrentIndex(-1);
//        break;

//    default:
//        break;
//    }
}

void OptionEditWidget::on_checkBox_stateChanged(int n)
{
    switch (static_cast<Qt::CheckState>(n)) {
    case Qt::Checked:
        checkBox->setText("true");
        break;

    case Qt::Unchecked:
        checkBox->setText("false");
        break;

    case Qt::PartiallyChecked:
        checkBox->setText("<Not set>");
        break;
    }
}




