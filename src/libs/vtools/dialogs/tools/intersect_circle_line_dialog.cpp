//---------------------------------------------------------------------------------------------------------------------
//  @file   intersect_circle_line_dialog.cpp
//  @author Douglas S Caskey
//  @date   14 Aug, 2024
//
//  @copyright
//  Copyright (C) 2017 - 2024 Seamly, LLC
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
//  along with Seamly2D. If not, see <http://www.gnu.org/licenses/>.
//---------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------
//  @file   dialogpointofcontact.cpp
//  @author Roman Telezhynskyi <dismine(at)gmail.com>
//  @date   15 Nov, 2013
//
//  @copyright
//  Copyright (C) 2013 Valentina project.
//  This source code is part of the Valentina project, a pattern making
//  program, whose allow create and modeling patterns of clothing.
//  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
//
//  Valentina is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published
//  by the Free Software Foundation, either version 3 of the License,
//  or (at your option) any later version.
//
//  Valentina is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
//---------------------------------------------------------------------------------------------------------------------

#include "intersect_circle_line_dialog.h"

#include <QColor>
#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPointer>
#include <QPushButton>
#include <QSet>
#include <QToolButton>

#include "../vpatterndb/vtranslatevars.h"
#include "../../visualization/visualization.h"
#include "../../visualization/line/intersect_circle_line_visual.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../ifc/xml/vdomdocument.h"
#include "../support/edit_formula_dialog.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "ui_intersect_circle_line_dialog.h"

//---------------------------------------------------------------------------------------------------------------------
/// @brief IntersectCircleLineDialog create dialog
/// @param data container with data
/// @param parent parent widget
//---------------------------------------------------------------------------------------------------------------------
IntersectCircleLineDialog::IntersectCircleLineDialog(const VContainer *data, const quint32 &toolId, QWidget *parent)
    : DialogTool(data, toolId, parent)
    , ui(new Ui::IntersectCircleLineDialog)
    , radius(QString())
    , formulaBaseHeight(0)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowIcon(QIcon(":/toolicon/32x32/point_intersect_circle_line.png"));

    // Set the position that the dialog opens based on user preference.
    setDialogPosition();

    ui->lineEditNamePoint->setClearButtonEnabled(true);

    initializeFormulaUi(ui);
    ui->lineEditNamePoint->setText(qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel));
    labelEditNamePoint = ui->labelEditNamePoint;
    this->formulaBaseHeight = ui->plainTextEditFormula->height();
    ui->plainTextEditFormula->installEventFilter(this);

    initializeOkCancelApply(ui);
    flagFormula = false;
    DialogTool::CheckState();

    fillComboBoxPoints(ui->comboBoxFirstPoint);
    fillComboBoxPoints(ui->comboBoxSecondPoint);
    fillComboBoxPoints(ui->comboBoxCenter);

    connect(ui->toolButtonExprRadius, &QPushButton::clicked,          this, &IntersectCircleLineDialog::editFXRadius);
    connect(ui->lineEditNamePoint,    &QLineEdit::textChanged,        this, &IntersectCircleLineDialog::NamePointChanged);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged,   this, &IntersectCircleLineDialog::formulaTextChanged);
    connect(ui->pushButtonGrowLength, &QPushButton::clicked,          this, &IntersectCircleLineDialog::deployFormulaTextEdit);
    connect(ui->comboBoxFirstPoint,   &QComboBox::currentTextChanged, this, &IntersectCircleLineDialog::PointNameChanged);
    connect(ui->comboBoxSecondPoint,  &QComboBox::currentTextChanged, this, &IntersectCircleLineDialog::PointNameChanged);
    connect(ui->comboBoxCenter,       &QComboBox::currentTextChanged, this, &IntersectCircleLineDialog::PointNameChanged);

    vis = new IntersectCircleLineVisual(data);
}

//---------------------------------------------------------------------------------------------------------------------
IntersectCircleLineDialog::~IntersectCircleLineDialog()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void IntersectCircleLineDialog::formulaTextChanged()
{
    this->FormulaChangedPlainText();
}

//---------------------------------------------------------------------------------------------------------------------
void IntersectCircleLineDialog::PointNameChanged()
{
    QSet<quint32> set;
    set.insert(getCurrentObjectId(ui->comboBoxFirstPoint));
    set.insert(getCurrentObjectId(ui->comboBoxSecondPoint));
    set.insert(getCurrentObjectId(ui->comboBoxCenter));

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
    ChangeColor(ui->labelFirstPoint, color);
    ChangeColor(ui->labelSecondPoint, color);
    ChangeColor(ui->labelArcCenter, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void IntersectCircleLineDialog::editFXRadius()
{
    EditFormulaDialog *dialog = new EditFormulaDialog(data, toolId, ToolDialog, this);
    dialog->setWindowTitle(tr("Edit radius"));
    dialog->SetFormula(getRadius());
    dialog->setPostfix(UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        setRadius(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void IntersectCircleLineDialog::ShowVisualization()
{
    AddVisualization<IntersectCircleLineVisual>();
}

//---------------------------------------------------------------------------------------------------------------------
void IntersectCircleLineDialog::deployFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormula, ui->pushButtonGrowLength, formulaBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief ChosenObject gets id and type of selected object. Save right data and ignore wrong.
/// @param id id of point or detail
/// @param type type of object
//---------------------------------------------------------------------------------------------------------------------
void IntersectCircleLineDialog::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            IntersectCircleLineVisual *line = qobject_cast<IntersectCircleLineVisual *>(vis);
            SCASSERT(line != nullptr)

            switch (number)
            {
                case 0:
                    if (SetObject(id, ui->comboBoxFirstPoint, tr("Select second point of line")))
                    {
                        number++;
                        line->VisualMode(id);
                    }
                    break;
                case 1:
                    if (getCurrentObjectId(ui->comboBoxFirstPoint) != id)
                    {
                        if (SetObject(id, ui->comboBoxSecondPoint, tr("Select point of center of arc")))
                        {
                            number++;
                            line->setLinePt2Id(id);
                            line->RefreshGeometry();
                        }
                    }
                    break;
                case 2:
                {
                    QSet<quint32> set;
                    set.insert(getCurrentObjectId(ui->comboBoxFirstPoint));
                    set.insert(getCurrentObjectId(ui->comboBoxSecondPoint));
                    set.insert(id);

                    if (set.size() == 3)
                    {
                        if (SetObject(id, ui->comboBoxCenter, ""))
                        {
                            line->setRadiusId(id);
                            line->RefreshGeometry();
                            prepare = true;
                            this->setModal(true);
                            this->show();
                        }
                    }
                }
                    break;
                default:
                    break;
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void IntersectCircleLineDialog::SaveData()
{
    pointName = ui->lineEditNamePoint->text();
    radius = ui->plainTextEditFormula->toPlainText();
    radius.replace("\n", " ");

    IntersectCircleLineVisual *visual = qobject_cast<IntersectCircleLineVisual *>(vis);
    SCASSERT(visual != nullptr)

    visual->setObject1Id(getFirstPoint());
    visual->setLinePt2Id(getSecondPoint());
    visual->setRadiusId(getCenter());
    visual->setRadius(radius);
    visual->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void IntersectCircleLineDialog::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormula->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief setSecondPoint set id second point
/// @param value id
//---------------------------------------------------------------------------------------------------------------------
void IntersectCircleLineDialog::setSecondPoint(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxSecondPoint, value);

    IntersectCircleLineVisual *visual = qobject_cast<IntersectCircleLineVisual *>(vis);
    SCASSERT(visual != nullptr)
    visual->setLinePt2Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief setFirstPoint set id first point
/// @param value id
//---------------------------------------------------------------------------------------------------------------------
void IntersectCircleLineDialog::setFirstPoint(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxFirstPoint, value);

    IntersectCircleLineVisual *visual = qobject_cast<IntersectCircleLineVisual *>(vis);
    SCASSERT(visual != nullptr)
    visual->setObject1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief SetCenter set id of center point
/// @param value id
//---------------------------------------------------------------------------------------------------------------------
void IntersectCircleLineDialog::setCenter(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxCenter, value);

    IntersectCircleLineVisual *visual = qobject_cast<IntersectCircleLineVisual *>(vis);
    SCASSERT(visual != nullptr)
    visual->setRadiusId(value);
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief setRadius set formula radius of arc
/// @param value formula
//---------------------------------------------------------------------------------------------------------------------
void IntersectCircleLineDialog::setRadius(const QString &value)
{
    radius = qApp->translateVariables()->FormulaToUser(value, qApp->Settings()->getOsSeparator());
    // increase height if needed.
    if (radius.length() > 80)
    {
        this->deployFormulaTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(radius);

    IntersectCircleLineVisual *visual = qobject_cast<IntersectCircleLineVisual *>(vis);
    SCASSERT(visual != nullptr)
    visual->setRadius(radius);

    MoveCursorToEnd(ui->plainTextEditFormula);
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief setPointName set name of point
/// @param value name
//---------------------------------------------------------------------------------------------------------------------
void IntersectCircleLineDialog::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief getRadius return formula radius of arc
/// @return formula
//---------------------------------------------------------------------------------------------------------------------
QString IntersectCircleLineDialog::getRadius() const
{
    return qApp->translateVariables()->TryFormulaFromUser(radius, qApp->Settings()->getOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief GetCenter return id of center point
/// @return id
//---------------------------------------------------------------------------------------------------------------------
quint32 IntersectCircleLineDialog::getCenter() const
{
    return getCurrentObjectId(ui->comboBoxCenter);
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief getFirstPoint return id first point
/// @return id
//---------------------------------------------------------------------------------------------------------------------
quint32 IntersectCircleLineDialog::getFirstPoint() const
{
    return getCurrentObjectId(ui->comboBoxFirstPoint);
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief getSecondPoint return id second point
/// @return id
//---------------------------------------------------------------------------------------------------------------------
quint32 IntersectCircleLineDialog::getSecondPoint() const
{
    return getCurrentObjectId(ui->comboBoxSecondPoint);
}
