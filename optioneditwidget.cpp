#include <QDebug>

#include "optioneditwidget.h"

void OptionEditWidget::setValue(const QVariant &value)
{
    if (!value.isValid()) {
        on_toolButtonReset_clicked();
        return;
    }

    switch (_type) {
    case QVariant::Bool:
        checkBox->setChecked(value.toBool());
        break;

    case QVariant::String:
        _lineEdit->setText(value.toString());
        break;

    case QVariant::List:
        _comboBox->setCurrentText(value.toString());
        break;

    default:
        break;
    }
}

QVariant OptionEditWidget::value() const
{
    if (reseted)
        return QVariant();

    switch (_type) {
    case QVariant::Bool:
        if (checkBox->checkState() == Qt::PartiallyChecked)
            return QVariant();
        return checkBox->isChecked();

    case QVariant::String:
        return _lineEdit->text();

    case QVariant::List:
        return _comboBox->currentText();

    default:
        return QVariant();
    }
}

void OptionEditWidget::valueSet()
{
    reseted = false;
}

QVariant::Type OptionEditWidget::type() const
{
    return _type;
}

void OptionEditWidget::setType(const QVariant::Type &type)
{
    _type = type;
    reseted =  false;

    switch (type) {
    case QVariant::Bool:
        checkBox->show();
        _lineEdit->hide();
        _comboBox->hide();
        toolButtonReset->show();
        break;

    case QVariant::String:
        checkBox->hide();
        _lineEdit->show();
        _comboBox->hide();
        toolButtonReset->show();
        break;

    case QVariant::List:
        checkBox->hide();
        _lineEdit->hide();
        _comboBox->show();
        toolButtonReset->show();
        break;

    case QVariant::Invalid:
        checkBox->hide();
        _lineEdit->hide();
        _comboBox->hide();
        toolButtonReset->hide();
        break;

    default:
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

OptionEditWidget::OptionEditWidget(QWidget *parent) :
    QWidget(parent), reseted(false)
{
    setupUi(this);
}

void OptionEditWidget::on_toolButtonReset_clicked()
{
    reseted = true;
    switch (_type) {
    case QVariant::Bool:
        checkBox->setCheckState(Qt::PartiallyChecked);
        break;

    case QVariant::String:
        _lineEdit->clear();
        break;

    case QVariant::List:
        _comboBox->setCurrentIndex(-1);
        break;

    default:
        break;
    }
}
