//-----------------------------------------------------------------------------
//  @file   shoulder_length_dialog.cpp
//  @author Douglas S Caskey
//  @date   17 Sep, 2023
//
//  @brief
//  @copyright
//  This source code is part of the Seamly2D project, a pattern making
//  program, whose allow create and modeling patterns of clothing.
//  Copyright (C) 2013-2022 Seamly2D project
//  <https://github.com/fashionfreedom/seamly2d> All Rights Reserved.
//
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
//  along with Seamly2D.  If not, see <http://www.gnu.org/licenses/>.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  @file   dialogshoulderpoint.cpp
//  @author Roman Telezhynskyi <dismine(at)gmail.com>
//  @date   November 15, 2013
//
//  @brief
//  @copyright
//  This source code is part of the Valentina project, a pattern making
//  program, whose allow create and modeling patterns of clothing.
//  Copyright (C) 2013 Valentina project
//  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
//
//  Valentina is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Valentina is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
//-----------------------------------------------------------------------------

#include "shoulder_length_dialog.h"

#include <QColor>
#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QLineF>
#include <QPointF>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPointer>
#include <QPushButton>
#include <QSet>
#include <QSharedPointer>
#include <QToolButton>

#include "../../tools/vabstracttool.h"
#include "../../visualization/visualization.h"
#include "../../visualization/line/shoulder_length_visual.h"
#include "../vgeometry/vpointf.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../ifc/xml/vdomdocument.h"
#include "../support/edit_formula_dialog.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../vwidgets/vabstractmainwindow.h"

#include "ui_shoulder_length_dialog.h"

/// @brief ShoulderLengthDialog create dialog
/// @param data container with data
/// @param parent parent widget
ShoulderLengthDialog::ShoulderLengthDialog(const VContainer *data, const quint32 &toolId, QWidget *parent)
    : DialogTool(data, toolId, parent)
    , ui(new Ui::ShoulderLengthDialog)
    , m_formula(QString())
    , m_formulaBaseHeight(0)
    , m_firstRelease(false)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowIcon(QIcon(":/toolicon/32x32/shoulder.png"));

    ui->lineEditNamePoint->setClearButtonEnabled(true);

    initializeFormulaUi(ui);
    ui->lineEditNamePoint->setText(qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel));
    labelEditNamePoint = ui->labelEditNamePoint;
    m_formulaBaseHeight = ui->plainTextEditFormula->height();
    ui->plainTextEditFormula->installEventFilter(this);

    initializeOkCancelApply(ui);
    flagFormula = false;
    DialogTool::CheckState();

    FillComboBoxPoints(ui->neckPoint_ComboBox);
    FillComboBoxPoints(ui->linePoint1_ComboBox);
    FillComboBoxPoints(ui->linePoint2_ComboBox);

    int index = ui->lineColor_ComboBox->findData(qApp->getCurrentDocument()->getDefaultLineColor());
    if (index != -1)
    {
        ui->lineColor_ComboBox->setCurrentIndex(index);
    }

    index = ui->lineWeight_ComboBox->findData(qApp->getCurrentDocument()->getDefaultLineWeight());
    if (index != -1)
    {
        ui->lineWeight_ComboBox->setCurrentIndex(index);
    }

    index = ui->lineType_ComboBox->findData(qApp->getCurrentDocument()->getDefaultLineType());
    if (index != -1)
    {
        ui->lineType_ComboBox->setCurrentIndex(index);
    }

    connect(ui->toolButtonExprLength, &QPushButton::clicked,          this, &ShoulderLengthDialog::FXLength);
    connect(ui->lineEditNamePoint,    &QLineEdit::textChanged,        this, &ShoulderLengthDialog::NamePointChanged);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged,   this, &ShoulderLengthDialog::formulaTextChanged);
    connect(ui->pushButtonGrowLength, &QPushButton::clicked,          this, &ShoulderLengthDialog::DeployFormulaTextEdit);
    connect(ui->neckPoint_ComboBox,   &QComboBox::currentTextChanged, this, &ShoulderLengthDialog::PointNameChanged);
    connect(ui->linePoint1_ComboBox,  &QComboBox::currentTextChanged, this, &ShoulderLengthDialog::PointNameChanged);
    connect(ui->linePoint2_ComboBox,  &QComboBox::currentTextChanged, this, &ShoulderLengthDialog::PointNameChanged);

    vis = new ShoulderLengthVisual(data);

    // Call after visual initialized.
    // If true current pen overides the default tool pen
    if(!qApp->Settings()->useCurrentPen())
    {
        setLineType(LineTypeDashLine);
        setLineWeight("0.35");
    }
}

//---------------------------------------------------------------------------------------------------------------------
void ShoulderLengthDialog::formulaTextChanged()
{
    this->FormulaChangedPlainText();
}

//---------------------------------------------------------------------------------------------------------------------
void ShoulderLengthDialog::PointNameChanged()
{
    QSet<quint32> set;
    set.insert(getCurrentObjectId(ui->neckPoint_ComboBox));
    set.insert(getCurrentObjectId(ui->linePoint1_ComboBox));
    set.insert(getCurrentObjectId(ui->linePoint2_ComboBox));

    QColor color = okColor;
    if (set.size() != 3)
    {
        flagError = false;
        color = errorColor;
    }
    else
    {
        flagError = true;
        color = okColor;
    }
    ChangeColor(ui->neckPoint_Label, color);
    ChangeColor(ui->linePoint1_Label, color);
    ChangeColor(ui->linePoint2_Label, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void ShoulderLengthDialog::FXLength()
{
    EditFormulaDialog *dialog = new EditFormulaDialog(data, toolId, this);
    dialog->setWindowTitle(tr("Edit length"));
    dialog->SetFormula(getFormula());
    dialog->setPostfix(UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        setFormula(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void ShoulderLengthDialog::ShowVisualization()
{
    AddVisualization<ShoulderLengthVisual>();
}

//---------------------------------------------------------------------------------------------------------------------
void ShoulderLengthDialog::DeployFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormula, ui->pushButtonGrowLength, m_formulaBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
ShoulderLengthDialog::~ShoulderLengthDialog()
{
    delete ui;
}

/// @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
/// @param id id of point or detail
/// @param type type of object
void ShoulderLengthDialog::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare)
    {
        return;
    }

    if (type == SceneObject::Point)
    {
        ShoulderLengthVisual *line = qobject_cast<ShoulderLengthVisual *>(vis);
        SCASSERT(line != nullptr)

        switch (number)
        {
            case 0:
                if (SetObject(id, ui->neckPoint_ComboBox, tr("Select first point of line")))
                {
                    number++;
                    line->VisualMode(id);
                }
                break;
            case 1:
                if (getCurrentObjectId(ui->neckPoint_ComboBox) != id)
                {
                    if (SetObject(id, ui->linePoint1_ComboBox, tr("Select second point of line")))
                    {
                        number++;
                        line->setLineP1Id(id);
                        line->RefreshGeometry();
                    }
                }
                break;
            case 2:
            {
                QSet<quint32> set;
                set.insert(getCurrentObjectId(ui->neckPoint_ComboBox));
                set.insert(getCurrentObjectId(ui->linePoint1_ComboBox));
                set.insert(id);

                if (set.size() == 3)
                {
                    if (SetObject(id, ui->linePoint2_ComboBox, QString()))
                    {
                        VAbstractMainWindow *window = qobject_cast<VAbstractMainWindow *>(qApp->getMainWindow());
                        SCASSERT(window != nullptr)

                        ShoulderLengthVisual *line = qobject_cast<ShoulderLengthVisual *>(vis);
                        SCASSERT(line != nullptr)
                        connect(line, &Visualization::ToolTip, window, &VAbstractMainWindow::ShowToolTip);

                        line->setLineP2Id(id);
                        line->RefreshGeometry();
                        prepare = true;

                        FinishCreating();
                    }
                }
            }
                break;
            default:
                break;
        }
    }

}

//---------------------------------------------------------------------------------------------------------------------
/// @brief SaveData Put dialog data in local variables
//---------------------------------------------------------------------------------------------------------------------
void ShoulderLengthDialog::SaveData()
{
    pointName = ui->lineEditNamePoint->text();
    m_formula = ui->plainTextEditFormula->toPlainText();
    m_formula.replace("\n", " ");

    ShoulderLengthVisual *line = qobject_cast<ShoulderLengthVisual *>(vis);
    SCASSERT(line != nullptr)

    line->setObject1Id(getNeckPoint());
    line->setLineP1Id(getFirstLinePoint());
    line->setLineP2Id(getSecondLinePoint());
    line->setLength(m_formula);
    line->setLineStyle(lineTypeToPenStyle(getLineType()));
    line->setLineWeight(getLineWeight());
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void ShoulderLengthDialog::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormula->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void ShoulderLengthDialog::FinishCreating()
{
    vis->SetMode(Mode::Show);
    vis->RefreshGeometry();
    emit ToolTip(QString());
    setModal(true);
    show();
}

/// @brief setPointName set name of point
/// @param value name
void ShoulderLengthDialog::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

/// @brief getLineType return type of line
/// @return type
QString ShoulderLengthDialog::getLineType() const
{
    return GetComboBoxCurrentData(ui->lineType_ComboBox, LineTypeSolidLine);
}

/// @brief setLineType set type of line
/// @param value type
void ShoulderLengthDialog::setLineType(const QString &value)
{
    ChangeCurrentData(ui->lineType_ComboBox, value);
    vis->setLineStyle(lineTypeToPenStyle(value));
}

/// @brief getLineWeight return weight of the lines
/// @return type
QString ShoulderLengthDialog::getLineWeight() const
{
        return GetComboBoxCurrentData(ui->lineWeight_ComboBox, "0.35");
}

/// @brief setLineWeight set weight of the lines
/// @param value type
void ShoulderLengthDialog::setLineWeight(const QString &value)
{
    ChangeCurrentData(ui->lineWeight_ComboBox, value);
    vis->setLineWeight(value);
}

//---------------------------------------------------------------------------------------------------------------------
QString ShoulderLengthDialog::getLineColor() const
{
    return GetComboBoxCurrentData(ui->lineColor_ComboBox, ColorBlack);
}

//---------------------------------------------------------------------------------------------------------------------
void ShoulderLengthDialog::setLineColor(const QString &value)
{
    ChangeCurrentData(ui->lineColor_ComboBox, value);
}

/// @brief getFormula return string of formula
/// @return formula
QString ShoulderLengthDialog::getFormula() const
{
    return qApp->translateVariables()->TryFormulaFromUser(m_formula, qApp->Settings()->getOsSeparator());
}

/// @brief setFormula set string of formula
/// @param value formula
void ShoulderLengthDialog::setFormula(const QString &value)
{
    m_formula = qApp->translateVariables()->FormulaToUser(value, qApp->Settings()->getOsSeparator());
    // increase height if needed.
    if (m_formula.length() > 80)
    {
        this->DeployFormulaTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(m_formula);

    ShoulderLengthVisual *line = qobject_cast<ShoulderLengthVisual *>(vis);
    SCASSERT(line != nullptr)
    line->setLength(m_formula);
    MoveCursorToEnd(ui->plainTextEditFormula);
}

/// @brief getNeckPoint return id shoulder point
/// @return id
quint32 ShoulderLengthDialog::getNeckPoint() const
{
    return getCurrentObjectId(ui->neckPoint_ComboBox);
}

/// @brief setNeckPoint set id shoulder point
/// @param value id
void ShoulderLengthDialog::setNeckPoint(const quint32 &value)
{
    setCurrentPointId(ui->neckPoint_ComboBox, value);

    ShoulderLengthVisual *line = qobject_cast<ShoulderLengthVisual *>(vis);
    SCASSERT(line != nullptr)
    line->setObject1Id(value);
}

/// @brief getFirstLinePoint return id first point of line
/// @return id
quint32 ShoulderLengthDialog::getFirstLinePoint() const
{
    return getCurrentObjectId(ui->linePoint1_ComboBox);
}

/// @brief setSecondLinePoint - set id first point of line
/// @param value id
void ShoulderLengthDialog::setFirstLinePoint(const quint32 &value)
{
    setCurrentPointId(ui->linePoint1_ComboBox, value);

    ShoulderLengthVisual *line = qobject_cast<ShoulderLengthVisual *>(vis);
    SCASSERT(line != nullptr)
    line->setLineP1Id(value);
}

/// @brief getSecondLinePoint return id second point of line
/// @return id
quint32 ShoulderLengthDialog::getSecondLinePoint() const
{
    return getCurrentObjectId(ui->linePoint2_ComboBox);
}

/// @brief setSecondLinePoint( set id second point of line
/// @param value id
void ShoulderLengthDialog::setSecondLinePoint(const quint32 &value)
{
    setCurrentPointId(ui->linePoint2_ComboBox, value);

    ShoulderLengthVisual *line = qobject_cast<ShoulderLengthVisual *>(vis);
    SCASSERT(line != nullptr)
    line->setLineP2Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
void ShoulderLengthDialog::ShowDialog(bool click)
{
    if (!prepare)
    {
        return;
    }

    if (click)
    {
        // The check need to ignore first release of mouse button.
        // User can select point by clicking on a label.
        if (!m_firstRelease)
        {
            m_firstRelease = true;
            return;
        }

        VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(qApp->getCurrentScene());
        SCASSERT(scene != nullptr)

        const QSharedPointer<VPointF> neckPoint = data->GeometricObject<VPointF>(getNeckPoint());
        QLineF toolLine = QLineF(static_cast<QPointF>(*neckPoint), scene->getScenePos());
        setFormula(QString::number(FromPixel(toolLine.length(), *data->GetPatternUnit())));
    }

    FinishCreating();
}
