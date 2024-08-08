//-----------------------------------------------------------------------------
//  @file   vtoolshoulderpoint.cpp
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

#include "vtoolshoulderpoint.h"

#include <QLineF>
#include <QMessageLogger>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QtDebug>
#include <new>

#include "../../../../../dialogs/tools/dialogshoulderpoint.h"
#include "../../../../../dialogs/tools/dialogtool.h"
#include "../../../../../visualization/visualization.h"
#include "../../../../../visualization/line/vistoolshoulderpoint.h"
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

const QString VToolShoulderPoint::ToolType = QStringLiteral("shoulder");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolShoulderPoint constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param lineType line type.
 * @param lineWeight line weight.
 * @param lineColor line color.
 * @param formula string with formula length.
 * @param lineP1 id first line point.
 * @param lineP2 id second line point.
 * @param pShoulder id shoulder point.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolShoulderPoint::VToolShoulderPoint(VAbstractPattern *doc, VContainer *data, const quint32 &id,
                                       const QString &lineType, const QString &lineWeight,
                                       const QString &lineColor, const QString &formula, const quint32 &lineP1,
                                       const quint32 &lineP2, const quint32 &pShoulder, const Source &typeCreation,
                                       QGraphicsItem * parent)
    : VToolLinePoint(doc, data, id, lineType, lineWeight, lineColor, formula, pShoulder, 0, parent)
    , lineP2(lineP2)
    , pShoulder(pShoulder)
{
    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolShoulderPoint::setDialog()
{
    SCASSERT(!m_dialog.isNull())
    QSharedPointer<DialogShoulderPoint> dialogTool = m_dialog.objectCast<DialogShoulderPoint>();
    SCASSERT(!dialogTool.isNull())
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(m_id);
    dialogTool->setLineType(m_lineType);
    dialogTool->setLineColor(lineColor);
    dialogTool->setFormula(formulaLength);
    dialogTool->setFirstPoint(pShoulder);
    dialogTool->setSecondPoint(basePointId);
    dialogTool->setThirdPoint(lineP2);
    dialogTool->setPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief findPoint find point.
 * @param lineP1 first point of line.
 * @param lineP2 second point of line.
 * @param pShoulder shoulder point.
 * @param length length form shoulder point to our.
 * @return point.
 */
QPointF VToolShoulderPoint::findPoint(const QPointF &lineP1, const QPointF &lineP2, const QPointF &pShoulder,
                                      const qreal &length)
{
    QPointF toolPoint = lineP2; // Tool Point if result was not found
    if (length <= 0)
    {
        return toolPoint;
    }

    QLineF line = QLineF(lineP1, lineP2);
    const qreal baseLength = line.length();
    //const int baseAngle = qRound(line.angle());
    line.setLength(length*2);

    QPointF p1;
    QPointF p2;

    const qint32 result = VGObject::LineIntersectCircle(pShoulder, length, line, p1, p2);

    if (result == 1 || result == 2)
    {
        const QLineF line1 = QLineF(lineP1, p1);
        const QLineF line2 = QLineF(lineP1, p2);
        //if (line1.length() > baseLength && baseAngle == qRound(line1.angle()))
        if (result == 1 && (line1.length() > baseLength))
        {
            toolPoint = p1;
        }
        //else if (result == 2 && line2.length() > baseLength && baseAngle == qRound(line2.angle()))
        else if (result == 2 && (line2.length() > baseLength))
        {
            toolPoint = p2;
        }
    }

    return toolPoint;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool from GUI.
 * @param dialog dialog.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @return the created tool
 */
VToolShoulderPoint* VToolShoulderPoint::Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene *scene,
                                               VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(!dialog.isNull())
    QSharedPointer<DialogShoulderPoint> dialogTool = dialog.objectCast<DialogShoulderPoint>();
    SCASSERT(!dialogTool.isNull())
    QString formula          = dialogTool->getFormula();
    const quint32 pShoulder  = dialogTool->getFirstPoint();
    const quint32 lineP1     = dialogTool->getSecondPoint();
    const quint32 lineP2     = dialogTool->getThirdPoint();
    const QString lineType   = dialogTool->getLineType();
    const QString lineWeight = dialogTool->getLineWeight();
    const QString lineColor  = dialogTool->getLineColor();
    const QString pointName  = dialogTool->getPointName();
    VToolShoulderPoint * point = Create(0, formula, lineP1, lineP2, pShoulder, lineType, lineWeight, lineColor, pointName,
                                        5, 10, true, scene, doc, data, Document::FullParse, Source::FromGui);
    if (point != nullptr)
    {
        point->m_dialog = dialogTool;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param _id tool id, 0 if tool doesn't exist yet.
 * @param formula string with formula length.
 * @param lineP1 id first line point.
 * @param lineP2 id second line point.
 * @param pShoulder id shoulder point.
 * @param lineType line type.
 * @param lineWeight line weight.
 * @param lineColor line color.
 * @param pointName point name.
 * @param mx label bias x axis.
 * @param my label bias y axis.
 * @param showPointName show/hide point name text.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 * @return the created tool
 */
VToolShoulderPoint* VToolShoulderPoint::Create(const quint32 _id, QString &formula, quint32 lineP1,
                                               quint32 lineP2, quint32 pShoulder, const QString &lineType,
                                               const QString &lineWeight,
                                               const QString &lineColor, const QString &pointName, qreal mx, qreal my,
                                               bool showPointName, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                               VContainer *data, const Document &parse, const Source &typeCreation)
{
    const QSharedPointer<VPointF> firstPoint = data->GeometricObject<VPointF>(lineP1);
    const QSharedPointer<VPointF> secondPoint = data->GeometricObject<VPointF>(lineP2);
    const QSharedPointer<VPointF> shoulderPoint = data->GeometricObject<VPointF>(pShoulder);

    const qreal result = CheckFormula(_id, formula, data);

    QPointF fPoint = VToolShoulderPoint::findPoint(static_cast<QPointF>(*firstPoint),
                                                   static_cast<QPointF>(*secondPoint),
                                                   static_cast<QPointF>(*shoulderPoint), qApp->toPixel(result));
    quint32 id =  _id;
    VPointF *p = new VPointF(fPoint, pointName, mx, my);
    p->setShowPointName(showPointName);

    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(p);
        data->AddLine(lineP1, id);
        data->AddLine(lineP2, id);
        data->AddLine(pShoulder, id);
    }
    else
    {
        data->UpdateGObject(id, p);
        data->AddLine(lineP1, id);
        data->AddLine(lineP2, id);
        data->AddLine(pShoulder, id);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }

    if (parse == Document::FullParse)
    {
        VDrawTool::AddRecord(id, Tool::ShoulderPoint, doc);
        VToolShoulderPoint *point = new VToolShoulderPoint(doc, data, id, lineType, lineWeight, lineColor, formula,
                                                           lineP1, lineP2, pShoulder,
                                                           typeCreation);
        scene->addItem(point);
        InitToolConnections(scene, point);
        VAbstractPattern::AddTool(id, point);
        doc->IncrementReferens(firstPoint->getIdTool());
        doc->IncrementReferens(secondPoint->getIdTool());
        doc->IncrementReferens(shoulderPoint->getIdTool());
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolShoulderPoint::thirdPointName() const
{
    return VAbstractTool::data.GetGObject(lineP2)->name();
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolShoulderPoint::firstPointName() const
{
    return VAbstractTool::data.GetGObject(pShoulder)->name();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolShoulderPoint::showContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id)
{
    try
    {
        ContextMenu<DialogShoulderPoint>(event, id);
    }
    catch(const VExceptionToolWasDeleted &error)
    {
        Q_UNUSED(error)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolShoulderPoint::RemoveReferens()
{
    const auto p2 = VAbstractTool::data.GetGObject(lineP2);
    const auto pS = VAbstractTool::data.GetGObject(pShoulder);

    doc->DecrementReferens(p2->getIdTool());
    doc->DecrementReferens(pS->getIdTool());
    VToolLinePoint::RemoveReferens();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolShoulderPoint::SaveDialog(QDomElement &domElement)
{
    SCASSERT(!m_dialog.isNull())
    QSharedPointer<DialogShoulderPoint> dialogTool = m_dialog.objectCast<DialogShoulderPoint>();
    SCASSERT(!dialogTool.isNull())

    doc->SetAttribute(domElement, AttrName,       dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrLineType,   dialogTool->getLineType());
    doc->SetAttribute(domElement, AttrLineWeight, dialogTool->getLineWeight());
    doc->SetAttribute(domElement, AttrLineColor,  dialogTool->getLineColor());
    doc->SetAttribute(domElement, AttrLength,     dialogTool->getFormula());
    doc->SetAttribute(domElement, AttrPShoulder,  QString().setNum(dialogTool->getFirstPoint()));
    doc->SetAttribute(domElement, AttrP1Line,     QString().setNum(dialogTool->getSecondPoint()));
    doc->SetAttribute(domElement, AttrP2Line,     QString().setNum(dialogTool->getThirdPoint()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolShoulderPoint::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolLinePoint::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType,      ToolType);
    doc->SetAttribute(tag, AttrLength,    formulaLength);
    doc->SetAttribute(tag, AttrP1Line,    basePointId);
    doc->SetAttribute(tag, AttrP2Line,    lineP2);
    doc->SetAttribute(tag, AttrPShoulder, pShoulder);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolShoulderPoint::ReadToolAttributes(const QDomElement &domElement)
{
    m_lineType    = doc->GetParametrString(domElement, AttrLineType,   LineTypeSolidLine);
    m_lineWeight  = doc->GetParametrString(domElement, AttrLineWeight, "0.35");
    lineColor     = doc->GetParametrString(domElement, AttrLineColor,  ColorBlack);
    formulaLength = doc->GetParametrString(domElement, AttrLength,     "");
    basePointId   = doc->GetParametrUInt(domElement,   AttrP1Line,     NULL_ID_STR);
    lineP2        = doc->GetParametrUInt(domElement,   AttrP2Line,     NULL_ID_STR);
    pShoulder     = doc->GetParametrUInt(domElement,   AttrPShoulder,  NULL_ID_STR);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolShoulderPoint::SetVisualization()
{
    if (!vis.isNull())
    {
        VisToolShoulderPoint *visual = qobject_cast<VisToolShoulderPoint *>(vis);
        SCASSERT(visual != nullptr)

        visual->setObject1Id(pShoulder);
        visual->setLineP1Id(basePointId);
        visual->setLineP2Id(lineP2);
        visual->setLength(qApp->translateVariables()->FormulaToUser(formulaLength, qApp->Settings()->getOsSeparator()));
        visual->setLineStyle(lineTypeToPenStyle(m_lineType));
        visual->setLineWeight(m_lineWeight);
        visual->SetMode(Mode::Show);
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolShoulderPoint::makeToolTip() const
{

    const QSharedPointer<VPointF> first = VAbstractTool::data.GeometricObject<VPointF>(pShoulder);
    const QSharedPointer<VPointF> second = VAbstractTool::data.GeometricObject<VPointF>(basePointId);
    const QSharedPointer<VPointF> third = VAbstractTool::data.GeometricObject<VPointF>(lineP2);
    const QSharedPointer<VPointF> current = VAbstractTool::data.GeometricObject<VPointF>(m_id);

    const QLineF firstToCur(static_cast<QPointF>(*first), static_cast<QPointF>(*current));
    const QLineF secondToThird(static_cast<QPointF>(*second), static_cast<QPointF>(*third));
    const QLineF secondToCur(static_cast<QPointF>(*second), static_cast<QPointF>(*current));
    const QLineF thirdToCur(static_cast<QPointF>(*third), static_cast<QPointF>(*current));

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
                                    .arg(current->name())                         // 6 Current point
                                    .arg(first->name())                           // 7 First point
                                    .arg(second->name())                          // 8 Second point
                                    .arg(third->name())                           // 9 Third point
                                    .arg(qApp->fromPixel(firstToCur.length()))    // 10 Tool line length
                                    .arg(qApp->fromPixel(secondToThird.length())) // 11 1st line length
                                    .arg(qApp->fromPixel(thirdToCur.length()))    // 12 2nd line length
                                    .arg(qApp->fromPixel(secondToCur.length()))   // 13 3rd line length
                                    .arg(firstToCur.angle())                      // 14 Tool ine angle
                                    .arg(secondToCur.angle());                    // 15 Baseline angle
    return toolTip;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
quint32 VToolShoulderPoint::getFirstPoint() const
{
    return pShoulder;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void VToolShoulderPoint::setFirstPoint(const quint32 &value)
{
    if (value != NULL_ID)
    {
        pShoulder = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(m_id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolShoulderPoint::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolShoulderPoint>(show);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolShoulderPoint::getThirdPoint() const
{
    return lineP2;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolShoulderPoint::setThirdPoint(const quint32 &value)
{
    if (value != NULL_ID)
    {
        lineP2 = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(m_id);
        SaveOption(obj);
    }
}
