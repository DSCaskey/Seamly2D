//-----------------------------------------------------------------------------
//  @file   marker_dialog.cpp
//  @author Douglas S Caskey
//  @date   20 Apr, 2025
//
//  @copyright
//  Copyright (C) 2017 - 2025 Seamly, LLC
//  https://github.com/fashionfreedom/seamly2d
//
//  @brief
//  Seamly2D is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Seamly2D is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Seamly2D. if not, see <http://www.gnu.org/licenses/>.
//-----------------------------------------------------------------------------

#include "marker_dialog.h"
#include "ui_marker_dialog.h"
#include "../vtools/tools/vabstracttool.h"

#include <QMenu>
#include <QString>
#include <QTimer>
#include <QDoubleSpinbox>

//---------------------------------------------------------------------------------------------------------------------
MarkerDialog::MarkerDialog(const VContainer *data, Marker mode, QWidget *parent)
    : DialogTool(data, NULL_ID, parent)
    , ui(new Ui::MarkerDialog)
    , m_mode(mode)
    , m_isInitialized(false)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // Set the position that the dialog opens based on user preference.
    setDialogPosition();

    initializeOkCancel(ui);
    initialize();
    initializeFabricTab();
    initializePaperTab();

    bool flagName = true;//We have default name of marker.
    CheckState();

}

//---------------------------------------------------------------------------------------------------------------------
MarkerDialog::~MarkerDialog()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void MarkerDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent( event );
    if ( event->spontaneous() )
    {
        return;
    }

    if (m_isInitialized)
    {
        return;
    }

    setFixedHeight(size().height());

    m_isInitialized = true;//first show windows are held
}

//---------------------------------------------------------------------------------------------------------------------
void MarkerDialog::CheckState()
{
    SCASSERT(ok_Button != nullptr);
    ok_Button->setEnabled(flagName);
}

//---------------------------------------------------------------------------------------------------------------------
void MarkerDialog::closeEvent(QCloseEvent *event)
{
    DialogTool::closeEvent(event);
}


//---------------------------------------------------------------------------------------------------------------------
void MarkerDialog::nameChanged()
{
    if (ui->name_LineEdit->text().isEmpty())
    {
        flagName = false;
        ChangeColor(ui->name_Label, Qt::red);
    }
    else
    {
        flagName = true;
        ChangeColor(ui->name_Label, okColor);
    }
    CheckState();
}


//---------------------------------------------------------------------------------------------------------------------
void MarkerDialog::initialize()
{
    if (m_mode == Marker::Fabric)
    {
        ui->marker_TabWidget->removeTab(ui->marker_TabWidget->indexOf(ui->paper_Tab));
        ui->name_LineEdit->setText(QString("Fabric Marker"));
    }
    else
    {
        ui->marker_TabWidget->removeTab(ui->marker_TabWidget->indexOf(ui->fabric_Tab));
        ui->name_LineEdit->setText(QString("Paper Marker"));
    }
    ui->name_LineEdit->setClearButtonEnabled(true);
    ui->convertToPath_CheckBox->setChecked(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MarkerDialog::initializeFabricTab()
{
    connect(ui->name_LineEdit, &QLineEdit::textChanged, this, &MarkerDialog::nameChanged);

/*
    connect(ui->spreadType_ComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [this]()
    {
        ui->penType_ComboBox->setEnabled(GetType() == PiecePathType::InternalPath);
        ui->cutOnFabric_CheckBox->setEnabled(GetType() == PiecePathType::InternalPath);
    });
*/
}

//---------------------------------------------------------------------------------------------------------------------
void MarkerDialog::initializePaperTab()
{

}

//---------------------------------------------------------------------------------------------------------------------
QString MarkerDialog::getName() const
{
    return ui->name_LineEdit->text();
}

//---------------------------------------------------------------------------------------------------------------------
void MarkerDialog::setName(const QString &name)
{
    ui->name_LineEdit->setText(name);
}

//---------------------------------------------------------------------------------------------------------------------
bool MarkerDialog::convertToPath() const
{
    return ui->convertToPath_CheckBox->isChecked();
}

//---------------------------------------------------------------------------------------------------------------------
void MarkerDialog::setConvertToPath(bool value)
{
    ui->convertToPath_CheckBox->setChecked(value);
}

//---------------------------------------------------------------------------------------------------------------------
SpreadType MarkerDialog::getType() const
{
    return static_cast<SpreadType>(ui->spreadType_ComboBox->currentData().toInt());
}

//---------------------------------------------------------------------------------------------------------------------
void MarkerDialog::setType(SpreadType type)
{
    const qint32 index = ui->spreadType_ComboBox->findData(static_cast<int>(type));
    if (index != -1)
    {
        ui->spreadType_ComboBox->setCurrentIndex(index);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString MarkerDialog::getMaterial() const
{
    return ui->material_ComboBox->currentText();
}

//---------------------------------------------------------------------------------------------------------------------
void MarkerDialog::setMaterial(const QString &material)
{
    qint32 index = ui->material_ComboBox->findText(material);
    if (index != -1)
    {
        ui->material_ComboBox->setCurrentIndex(index);
    }
}

//---------------------------------------------------------------------------------------------------------------------
qreal MarkerDialog::getFabricWidth() const
{
    return ui->fabricWidth_DoubleSpinBox->value();
}

//---------------------------------------------------------------------------------------------------------------------
void MarkerDialog::setFabricWidth(const qreal &width)
{
    ui->fabricWidth_DoubleSpinBox->setValue(width);
}

//---------------------------------------------------------------------------------------------------------------------
qreal MarkerDialog::getCutLength() const
{
    return ui->cutLength_DoubleSpinBox->value();
}

//---------------------------------------------------------------------------------------------------------------------
void MarkerDialog::setCutLength(const qreal &length)
{
    ui->cutLength_DoubleSpinBox->setValue(length);
}

//---------------------------------------------------------------------------------------------------------------------
qreal MarkerDialog::getSelvageTop() const
{
    return ui->selvageTop_DoubleSpinBox->value();
}

//---------------------------------------------------------------------------------------------------------------------
void MarkerDialog::setSelvageTop(const qreal &value)
{
    ui->selvageTop_DoubleSpinBox->setValue(value);
}

//---------------------------------------------------------------------------------------------------------------------
qreal MarkerDialog::getSelvageBottom() const
{
    return ui->selvageBottom_DoubleSpinBox->value();
}

//---------------------------------------------------------------------------------------------------------------------
void MarkerDialog::setSelvageBottom(const qreal &value)
{
    ui->selvageBottom_DoubleSpinBox->setValue(value);
}

//---------------------------------------------------------------------------------------------------------------------
qreal MarkerDialog::getPaperWidth() const
{
    return ui->paperWidth_DoubleSpinBox->value();
}

//---------------------------------------------------------------------------------------------------------------------
void MarkerDialog::setPaperWidth(const qreal &value)
{
    ui->paperWidth_DoubleSpinBox->setValue(value);
}

//---------------------------------------------------------------------------------------------------------------------
qreal MarkerDialog::getPaperHeight() const
{
    return ui->paperHeight_DoubleSpinBox->value();
}

//---------------------------------------------------------------------------------------------------------------------
void MarkerDialog::setPaperHeight(const qreal &value)
{
    ui->paperHeight_DoubleSpinBox->setValue(value);
}

//---------------------------------------------------------------------------------------------------------------------
void MarkerDialog::DialogAccepted()
{
    accepted();
}
