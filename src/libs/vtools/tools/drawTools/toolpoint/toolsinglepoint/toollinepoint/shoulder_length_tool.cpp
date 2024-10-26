//-----------------------------------------------------------------------------
//  @file   shoulder_length_tool..cpp
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
//  @file   vtoolshoulderpoint.cpp
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

#include "shoulder_length_tool.h"

#include <QLineF>
#include <QMessageLogger>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QtDebug>
#include <new>

#include "../../../../../dialogs/tools/shoulder_length_dialog.h"
#include "../../../../../dialogs/tools/dialogtool.h"
#include "../../../../../visualization/visualization.h"
#include "../../../../../visualization/line/shoulder_length_visual.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../../../../vabstracttool.h"
#include "../../../vdrawtool.h"
#include "vtoollinepoint.h"

template <class T> class QSharedPointer;

const QString ShoulderLengthTool::ToolType = QStringLiteral("shoulder");

//---------------------------------------------------------------------------------------------------------------------
/// @brief ShoulderLengthTool constructor.
/// @param doc dom document container.
/// @param data container with variables.
/// @param id object id in container.
/// @param lineType line type.
/// @param lineWeight line weight.
/// @param lineColor line color.
/// @param formula string with formula length.
/// @param lineP1Id id first line point.
/// @param lineP2Id id second line point.
/// @param neckPointId id neck point.
/// @param typeCreation way we create this tool.
/// @param parent parent object.
//--------------------------------------------------------------------------------------------------------------------
ShoulderLengthTool::ShoulderLengthTool(VAbstractPattern *doc, VContainer *data, const quint32 &id,
                                       const QString &lineType, const QString &lineWeight,
                                       const QString &lineColor, const QString &formula, const quint32 &neckPointId,
                                       const quint32 &lineP1Id, const quint32 &lineP2Id,  const Source &typeCreation,
                                       QGraphicsItem *parent)
    : VToolLinePoint(doc, data, id, lineType, lineWeight, lineColor, formula, neckPointId, 0, parent)
    , m_lineP1Id(lineP1Id)
    , m_lineP2Id(lineP2Id)

{
    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief setDialog set dialog when user want change tool option.
//--------------------------------------------------------------------------------------------------------------------
void ShoulderLengthTool::setDialog()
{
    SCASSERT(!m_dialog.isNull())
    QSharedPointer<ShoulderLengthDialog> dialogTool = m_dialog.objectCast<ShoulderLengthDialog>();
    SCASSERT(!dialogTool.isNull())
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(m_id);
    dialogTool->setLineType(m_lineType);
    dialogTool->setLineColor(lineColor);
    dialogTool->setFormula(formulaLength);
    dialogTool->setNeckPoint(basePointId);
    dialogTool->setFirstLinePoint(m_lineP1Id);
    dialogTool->setSecondLinePoint(m_lineP2Id);
    dialogTool->setPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief findPoint find point.
/// @param lineP1 first point of line.
/// @param lineP2 second point of line.
/// @param neckPoint neck point.
/// @param length length from neck point to line.
/// @return point.
//--------------------------------------------------------------------------------------------------------------------
QPointF ShoulderLengthTool::findPoint(const QPointF &lineP1, const QPointF &lineP2, const QPointF &neckPoint,
                                      const qreal &length)
{
    QPointF toolPoint = lineP2; // Tool Point if result was not found
    if (length <= 0)
    {
        return toolPoint;
    }

    QLineF line = QLineF(lineP1, lineP2);
    const qreal baseLength = line.length();
    const int baseAngle = qRound(line.angle());
    line.setLength(length*2);


    QPointF p1;
    QPointF p2;
    const qint32 result = VGObject::LineIntersectCircle(neckPoint, length, line, p1, p2);

    if (result == 1 || result == 2)
    {
        const QLineF line1 = QLineF(lineP1, p1);
        const QLineF line2 = QLineF(lineP1, p2);
        if (line1.length() > baseLength && baseAngle == qRound(line1.angle()))
        {
            toolPoint = p1;
        }
        else if (result == 2 && line2.length() > baseLength && baseAngle == qRound(line2.angle()))
        {
            toolPoint = p2;
        }
    }

    return toolPoint;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief Create help create tool from GUI.
/// @param dialog dialog.
/// @param scene pointer to scene.
/// @param doc dom document container.
/// @param data container with variables.
/// @return the created tool
//--------------------------------------------------------------------------------------------------------------------
ShoulderLengthTool* ShoulderLengthTool::Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene *scene,
                                               VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(!dialog.isNull())
    QSharedPointer<ShoulderLengthDialog> dialogTool = dialog.objectCast<ShoulderLengthDialog>();
    SCASSERT(!dialogTool.isNull())
    QString formula           = dialogTool->getFormula();
    const quint32 neckPointId = dialogTool->getNeckPoint();
    const quint32 lineP1Id    = dialogTool->getFirstLinePoint();
    const quint32 lineP2Id    = dialogTool->getSecondLinePoint();
    const QString lineType    = dialogTool->getLineType();
    const QString lineWeight  = dialogTool->getLineWeight();
    const QString lineColor   = dialogTool->getLineColor();
    const QString pointName   = dialogTool->getPointName();
    ShoulderLengthTool *point = Create(0, formula, neckPointId, lineP1Id, lineP2Id,  lineType, lineWeight, lineColor, pointName,
                                        5, 10, true, scene, doc, data, Document::FullParse, Source::FromGui);
    if (point != nullptr)
    {
        point->m_dialog = dialogTool;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief Create help create tool.
/// @param _id tool id, 0 if tool doesn't exist yet.
/// @param formula string with formula length.
/// @param lineP1Id id first line point.
/// @param lineP2Id id second line point.
/// @param neckPointId id neck point.
/// @param lineType line type.
/// @param lineWeight line weight.
/// @param lineColor line color.
/// @param pointName point name.
/// @param mx label bias x axis.
/// @param my label bias y axis.
/// @param showPointName show/hide point name text.
/// @param scene pointer to scene.
/// @param doc dom document container.
/// @param data container with variables.
/// @param parse parser file mode.
/// @param typeCreation way we create this tool.
/// @return the created tool
//--------------------------------------------------------------------------------------------------------------------
ShoulderLengthTool* ShoulderLengthTool::Create(const quint32 _id, QString &formula,
                                               quint32 neckPointId, quint32 lineP1Id, quint32 lineP2Id,
                                               const QString &lineType, const QString &lineWeight,
                                               const QString &lineColor, const QString &pointName, qreal mx, qreal my,
                                               bool showPointName, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                               VContainer *data, const Document &parse, const Source &typeCreation)
{
    const QSharedPointer<VPointF> neckPoint = data->GeometricObject<VPointF>(neckPointId);
    const QSharedPointer<VPointF> firstPoint = data->GeometricObject<VPointF>(lineP1Id);
    const QSharedPointer<VPointF> secondPoint = data->GeometricObject<VPointF>(lineP2Id);

    const qreal result = CheckFormula(_id, formula, data);

    QPointF fPoint = ShoulderLengthTool::findPoint(static_cast<QPointF>(*firstPoint),
                                                   static_cast<QPointF>(*secondPoint),
                                                   static_cast<QPointF>(*neckPoint), qApp->toPixel(result));
    quint32 id =  _id;
    VPointF *toolPoint = new VPointF(fPoint, pointName, mx, my);
    toolPoint->setShowPointName(showPointName);

    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(toolPoint);
        data->AddLine(neckPointId, id);
        data->AddLine(lineP1Id, id);
        data->AddLine(lineP2Id, id);

    }
    else
    {
        data->UpdateGObject(id, toolPoint);
        data->AddLine(neckPointId, id);
        data->AddLine(lineP1Id, id);
        data->AddLine(lineP2Id, id);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }

    if (parse == Document::FullParse)
    {
        VDrawTool::AddRecord(id, Tool::ShoulderPoint, doc);
        ShoulderLengthTool *point = new ShoulderLengthTool(doc, data, id, lineType, lineWeight, lineColor, formula,
                                                           neckPointId, lineP1Id, lineP2Id,
                                                           typeCreation);
        scene->addItem(point);
        InitToolConnections(scene, point);
        VAbstractPattern::AddTool(id, point);
        doc->IncrementReferens(neckPoint->getIdTool());
        doc->IncrementReferens(firstPoint->getIdTool());
        doc->IncrementReferens(secondPoint->getIdTool());
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
QString ShoulderLengthTool::linePoint1Name() const
{
    return VAbstractTool::data.GetGObject(m_lineP1Id)->name();
}

//---------------------------------------------------------------------------------------------------------------------
QString ShoulderLengthTool::linePoint2Name() const
{
    return VAbstractTool::data.GetGObject(m_lineP2Id)->name();
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief contextMenuEvent handle context menu events.
/// @param event context menu event.
//--------------------------------------------------------------------------------------------------------------------
void ShoulderLengthTool::showContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id)
{
    try
    {
        ContextMenu<ShoulderLengthDialog>(event, id);
    }
    catch(const VExceptionToolWasDeleted &error)
    {
        Q_UNUSED(error)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief RemoveReferens decrement value of reference.
//--------------------------------------------------------------------------------------------------------------------
void ShoulderLengthTool::RemoveReferens()
{
    const auto point1 = VAbstractTool::data.GetGObject(m_lineP1Id);
    const auto point2 = VAbstractTool::data.GetGObject(m_lineP2Id);

    doc->DecrementReferens(point1->getIdTool());
    doc->DecrementReferens(point2->getIdTool());
    VToolLinePoint::RemoveReferens(); // remove reference to neckpoint (basepoint)
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief SaveDialog save options into file after change in dialog.
//--------------------------------------------------------------------------------------------------------------------
void ShoulderLengthTool::SaveDialog(QDomElement &domElement)
{
    SCASSERT(!m_dialog.isNull())
    QSharedPointer<ShoulderLengthDialog> dialogTool = m_dialog.objectCast<ShoulderLengthDialog>();
    SCASSERT(!dialogTool.isNull())

    doc->SetAttribute(domElement, AttrName,       dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrLineType,   dialogTool->getLineType());
    doc->SetAttribute(domElement, AttrLineWeight, dialogTool->getLineWeight());
    doc->SetAttribute(domElement, AttrLineColor,  dialogTool->getLineColor());
    doc->SetAttribute(domElement, AttrLength,     dialogTool->getFormula());
    doc->SetAttribute(domElement, AttrPShoulder,  QString().setNum(dialogTool->getNeckPoint()));
    doc->SetAttribute(domElement, AttrP1Line,     QString().setNum(dialogTool->getFirstLinePoint()));
    doc->SetAttribute(domElement, AttrP2Line,     QString().setNum(dialogTool->getSecondLinePoint()));
}

//---------------------------------------------------------------------------------------------------------------------
void ShoulderLengthTool::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolLinePoint::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType,      ToolType);
    doc->SetAttribute(tag, AttrLength,    formulaLength);
    doc->SetAttribute(tag, AttrPShoulder, basePointId);
    doc->SetAttribute(tag, AttrP1Line,    m_lineP1Id);
    doc->SetAttribute(tag, AttrP2Line,    m_lineP2Id);
}

//---------------------------------------------------------------------------------------------------------------------
void ShoulderLengthTool::ReadToolAttributes(const QDomElement &domElement)
{
    m_lineType    = doc->GetParametrString(domElement, AttrLineType,   LineTypeSolidLine);
    m_lineWeight  = doc->GetParametrString(domElement, AttrLineWeight, "0.35");
    lineColor     = doc->GetParametrString(domElement, AttrLineColor,  ColorBlack);
    formulaLength = doc->GetParametrString(domElement, AttrLength,     "");
    basePointId   = doc->GetParametrUInt(domElement,   AttrPShoulder,  NULL_ID_STR);
    m_lineP1Id    = doc->GetParametrUInt(domElement,   AttrP1Line,     NULL_ID_STR);
    m_lineP2Id    = doc->GetParametrUInt(domElement,   AttrP2Line,     NULL_ID_STR);
}

//---------------------------------------------------------------------------------------------------------------------
void ShoulderLengthTool::SetVisualization()
{
    if (!vis.isNull())
    {
        ShoulderLengthVisual *visual = qobject_cast<ShoulderLengthVisual *>(vis);
        SCASSERT(visual != nullptr)

        visual->setObject1Id(basePointId);
        visual->setLineP1Id(m_lineP1Id);
        visual->setLineP2Id(m_lineP2Id);
        visual->setLength(qApp->translateVariables()->FormulaToUser(formulaLength, qApp->Settings()->getOsSeparator()));
        visual->setLineStyle(lineTypeToPenStyle(m_lineType));
        visual->setLineWeight(m_lineWeight);
        visual->SetMode(Mode::Show);
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString ShoulderLengthTool::makeToolTip() const
{

    const QSharedPointer<VPointF> first  = VAbstractTool::data.GeometricObject<VPointF>(basePointId);
    const QSharedPointer<VPointF> second = VAbstractTool::data.GeometricObject<VPointF>(m_lineP1Id);
    const QSharedPointer<VPointF> third  = VAbstractTool::data.GeometricObject<VPointF>(m_lineP2Id);
    const QSharedPointer<VPointF> tool   = VAbstractTool::data.GeometricObject<VPointF>(m_id);

    const QLineF firstToTool(static_cast<QPointF>(*first), static_cast<QPointF>(*tool));
    const QLineF secondToThird(static_cast<QPointF>(*second), static_cast<QPointF>(*third));
    const QLineF secondToTool(static_cast<QPointF>(*second), static_cast<QPointF>(*tool));
    const QLineF thirdToTool(static_cast<QPointF>(*third), static_cast<QPointF>(*tool));

    const QString toolTip = QString("<table>"
                                     "<tr>"
                                        "<td align ='right'><b>%1: </b></td>"     // Tool name
                                        "<td align ='left'>Length to Line</td>"
                                     "</tr>"
                                     "<tr>"
                                        "<td align ='right'><b>%2: </b></td>"     // Point Name
                                        "<td align ='left'>%6</td>"
                                     "</tr>"
                                     "<tr>"
                                         "<td colspan ='2'><hr></td>"             // Divider
                                     "</tr>"
                                     "<tr>"
                                         "<td align ='right'><b>%2: </b></td>"    // Tool Line name
                                         "<td align ='left'>Line_%7_%6</td>"
                                     "</tr>"
                                     "<tr>"
                                         "<td align ='right'><b>%3: </b></td>"    // Tool Line length
                                         "<td align ='left'>%10 %5</td>"
                                     "</tr>"
                                     "<tr>"
                                         "<td align ='right'><b>%4: </b></td>"    // Tool Line angle
                                         "<td align ='left'>%14°</td>"
                                     "</tr>"
                                     "<tr>"
                                         "<td colspan ='2'><hr></td>"             // Divider
                                     "</tr>"
                                     "<tr>"
                                         "<td align ='right'><b>%2: </b></td>"    // 1st Line name
                                         "<td align ='left'>Line_%8_%9</td>"
                                     "</tr>"
                                     "<tr>"
                                         "<td align ='right'><b>%3: </b></td>"    // 1st Line length
                                         "<td align ='left'>%11 %5</td>"
                                     "</tr>"
                                     "<tr>"
                                         "<td align ='right'><b>%4: </b></td>"    // 1st Line angle
                                         "<td align ='left'>%15°</td>"
                                     "</tr>"
                                     "<tr>"
                                         "<td colspan ='2'><hr></td>"             // Divider
                                     "</tr>"
                                     "<tr>"
                                         "<td align ='right'><b>%2: </b></td>"    // 2nd Line name
                                         "<td align ='left'>Line_%9_%6</td>"
                                     "</tr>"
                                     "<tr>"
                                         "<td align ='right'><b>%3: </b></td>"    // 2nd Line length
                                         "<td align ='left'>%12 %5</td>"
                                     "</tr>"
                                     "<tr>"
                                         "<td align ='right'><b>%4: </b></td>"    // 2nd Line angle
                                         "<td align ='left'>%15°</td>"
                                     "</tr>"
                                     "<tr>"
                                         "<td colspan ='2'><hr></td>"             // Divider
                                     "</tr>"
                                     "<tr>"
                                         "<td align ='right'><b>%2: </b></td>"    // 3rd Line name
                                         "<td align ='left'>Line_%8_%6</td>"
                                     "</tr>"
                                     "<tr>"
                                         "<td align ='right'><b>%3: </b></td>"    // 3rd Line length
                                         "<td align ='left'>%13 %5</td>"
                                     "</tr>"
                                     "<tr>"
                                         "<td align ='right'><b>%4: </b></td>"    // 3rd Line angle
                                         "<td align ='left'>%15°</td>"
                                    "</table>")
                                    .arg(tr("Tool"))                              // 1
                                    .arg(tr("Name"))                              // 2
                                    .arg(tr("Length"))                            // 3
                                    .arg(tr("Angle"))                             // 4
                                    .arg(UnitsToStr(qApp->patternUnit(), true))   // 5 Units
                                    .arg(tool->name())                            // 6 Tool point
                                    .arg(first->name())                           // 7 First point
                                    .arg(second->name())                          // 8 Second point
                                    .arg(third->name())                           // 9 Third point
                                    .arg(qApp->fromPixel(firstToTool.length()))   // 10 Tool line length
                                    .arg(qApp->fromPixel(secondToThird.length())) // 11 1st line length
                                    .arg(qApp->fromPixel(thirdToTool.length()))   // 12 2nd line length
                                    .arg(qApp->fromPixel(secondToTool.length()))  // 13 3rd line length
                                    .arg(firstToTool.angle())                     // 14 Tool ine angle
                                    .arg(secondToTool.angle());                   // 15 Baseline angle
    return toolTip;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
quint32 ShoulderLengthTool::getFirstLinePoint() const
{
    return m_lineP1Id;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void ShoulderLengthTool::setFirstLinePoint(const quint32 &value)
{
    if (value != NULL_ID)
    {
        m_lineP1Id = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(value);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 ShoulderLengthTool::getSecondLinePoint() const
{
    return m_lineP2Id;
}

//---------------------------------------------------------------------------------------------------------------------
void ShoulderLengthTool::setSecondLinePoint(const quint32 &value)
{
    if (value != NULL_ID)
    {
        m_lineP2Id = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(value);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void ShoulderLengthTool::ShowVisualization(bool show)
{
    ShowToolVisualization<ShoulderLengthVisual>(show);
}
