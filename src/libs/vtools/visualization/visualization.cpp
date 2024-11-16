 //  @file   visualization.cpp
 //  @author Douglas S Caskey
 //  @date   17 Sep, 2023
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

 /************************************************************************
 **  @file   visualization.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 8, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Seamly2D.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#include "visualization.h"

#include "global.h"
#include "../ifc/ifcdef.h"
#include "../qmuparser/qmuparsererror.h"
#include "../tools/drawTools/vdrawtool.h"
#include "../vmisc/vcommonsettings.h"
#include "../vpatterndb/calculator.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../vwidgets/scalesceneitems.h"
#include "../vwidgets/vcurvepathitem.h"
#include "../vwidgets/vmaingraphicsscene.h"

#include <qnumeric.h>
#include <QBrush>
#include <QColor>
#include <QGraphicsEllipseItem>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QLineF>
#include <QMessageLogger>
#include <QPen>
#include <QPointF>
#include <QRectF>
#include <QScopedPointer>
#include <QString>
#include <Qt>
#include <QtDebug>

template <class K, class V> class QHash;

Q_LOGGING_CATEGORY(vVis, "v.visualization")

//---------------------------------------------------------------------------------------------------------------------
Visualization::Visualization(const VContainer *data)
    : QObject()
    , data(data)
    , scenePos(QPointF())
    , mainColor(Qt::red)
    , supportColor(QColor(qApp->Settings()->getPrimarySupportColor()))
    , lineStyle(Qt::SolidLine)
    , lineWeight(0.35)
    , object1Id(NULL_ID)
    , toolTip(QString())
    , mode(Mode::Creation)
{}

//---------------------------------------------------------------------------------------------------------------------
void Visualization::setObject1Id(const quint32 &value)
{
    object1Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void Visualization::setLineStyle(const Qt::PenStyle &value)
{
    lineStyle = value;
    initPen();
}

//---------------------------------------------------------------------------------------------------------------------
void Visualization::setLineWeight(const QString &value)
{
    lineWeight = ToPixel(value.toDouble(), Unit::Mm);
    initPen();
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void Visualization::setScenePos(const QPointF &value)
{
    scenePos = value;
}

//---------------------------------------------------------------------------------------------------------------------
void Visualization::VisualMode(const quint32 &pointId)
{
    VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(qApp->getCurrentScene());
    SCASSERT(scene != nullptr)

    this->object1Id = pointId;
    this->scenePos = scene->getScenePos();
    RefreshGeometry();

    AddOnScene();
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void Visualization::setMainColor(const QColor &value)
{
    mainColor = value;
    initPen();
}

//---------------------------------------------------------------------------------------------------------------------
const VContainer *Visualization::getData() const
{
    return data;
}

//---------------------------------------------------------------------------------------------------------------------
void Visualization::setData(const VContainer *data)
{
    this->data = data;
}

//---------------------------------------------------------------------------------------------------------------------
void Visualization::mousePos(const QPointF &scenePos)
{
    this->scenePos = scenePos;
    RefreshGeometry();
    if (toolTip.isEmpty() == false)
    {
        emit ToolTip(toolTip);
    }
}

//---------------------------------------------------------------------------------------------------------------------
VScaledEllipse *Visualization::InitPoint(const QColor &color, QGraphicsItem *parent, qreal z) const
{
    return initPointItem(color, parent, z);
}

//---------------------------------------------------------------------------------------------------------------------
qreal Visualization::FindLength(const QString &expression,
                                const QHash<QString, QSharedPointer<VInternalVariable> > *vars)
{
    return qApp->toPixel(FindVal(expression, vars));
}

//---------------------------------------------------------------------------------------------------------------------
qreal Visualization::FindVal(const QString &expression,
                             const QHash<QString, QSharedPointer<VInternalVariable> > *vars)
{
    qreal val = 0;
    if (expression.isEmpty())
    {
        val = 0;
    }
    else
    {
        try
        {
            // Replace line return with spaces for calc if exist
            QString formula = expression;
            formula.replace("\n", " ");
            formula = qApp->translateVariables()->FormulaFromUser(formula, qApp->Settings()->getOsSeparator());
            QScopedPointer<Calculator> cal(new Calculator());
            val = cal->EvalFormula(vars, formula);

            if (qIsInf(val) || qIsNaN(val))
            {
                val = 0;
            }
        }
        catch (qmu::QmuParserError &error)
        {
            val = 0;
            qDebug() << "\nMath parser error:\n"
                       << "--------------------------------------\n"
                       << "Message:     " << error.GetMsg()  << "\n"
                       << "Expression:  " << error.GetExpr() << "\n"
                       << "--------------------------------------";
        }
    }
    return val;
}

//---------------------------------------------------------------------------------------------------------------------
void Visualization::DrawPoint(QGraphicsEllipseItem *point, const QPointF &pos, const QColor &color,
                              const qreal &lineWeight, Qt::PenStyle style)
{
    SCASSERT (point != nullptr)

    point->setPos(pos);

    QPen visPen = point->pen();
    visPen.setColor(color);
    visPen.setStyle(style);
    visPen.setWidthF(scaleWidth(lineWeight, sceneScale(qApp->getCurrentScene())));

    point->setPen(visPen);
    point->setVisible(true);
}

//---------------------------------------------------------------------------------------------------------------------
void Visualization::DrawLine(VScaledLine *lineItem, const QLineF &line, const QColor &color,
                             const qreal &lineWeight, Qt::PenStyle style)
{
    SCASSERT (lineItem != nullptr)

    QPen visPen = lineItem->pen();
    visPen.setColor(color);
    visPen.setStyle(style);
    visPen.setWidthF(scaleWidth(lineWeight, sceneScale(qApp->getCurrentScene())));

    lineItem->setPen(visPen);
    lineItem->setLine(line);
    lineItem->setVisible(true);
}

//---------------------------------------------------------------------------------------------------------------------
void Visualization::DrawPath(VCurvePathItem *pathItem, const QPainterPath &path, const QColor &color,
                             Qt::PenStyle style, const qreal &lineWeight, Qt::PenCapStyle cap)
{
    DrawPath(pathItem, path, QVector<DirectionArrow>(), color, style, lineWeight, cap);
}

//---------------------------------------------------------------------------------------------------------------------
void Visualization::DrawPath(VCurvePathItem *pathItem, const QPainterPath &path,
                             const QVector<DirectionArrow> &directionArrows, const QColor &color, Qt::PenStyle style,
                             const qreal &lineWeight, Qt::PenCapStyle cap)
{
    SCASSERT (pathItem != nullptr)

    QPen visPen = pathItem->pen();
    visPen.setColor(color);
    visPen.setStyle(style);
    visPen.setWidthF(scaleWidth(lineWeight, sceneScale(qApp->getCurrentScene())));
    visPen.setCapStyle(cap);

    pathItem->setPen(visPen);
    pathItem->setPath(path);
    pathItem->SetDirectionArrows(directionArrows);
    pathItem->setVisible(true);
}


//---------------------------------------------------------------------------------------------------------------------
void Visualization::drawArrowedLine(ArrowedLineItem *item, const QLineF &line, const QColor &color,
                                    Qt::PenStyle style)
{
    SCASSERT (item != nullptr)

    QPen visPen = item->pen();
    visPen.setColor(color);
    visPen.setStyle(style);

    item->setPen(visPen);
    item->setLine(line);
    item->setVisible(true);
}

//---------------------------------------------------------------------------------------------------------------------
VScaledEllipse *Visualization::GetPointItem(QVector<VScaledEllipse *> &points, quint32 i,
                                            const QColor &color, QGraphicsItem *parent)
{
    if (not points.isEmpty() && static_cast<quint32>(points.size() - 1) >= i)
    {
        return points.at(static_cast<int>(i));
    }
    else
    {
        auto point = initPointItem(color, parent);
        points.append(point);
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
VScaledEllipse *Visualization::initPointItem(const QColor &color, QGraphicsItem *parent, qreal z)
{
    VScaledEllipse *point = new VScaledEllipse(parent);
    point->setZValue(1);
    point->setBrush(QBrush(Qt::NoBrush));

    QPen visPen = point->pen();
    visPen.setColor(color);

    point->setPen(visPen);
    point->setRect(pointRect(defPointRadiusPixel));
    point->setPos(QPointF());
    point->setFlags(QGraphicsItem::ItemStacksBehindParent);
    point->setZValue(z);
    point->setVisible(false);
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
Mode Visualization::GetMode() const
{
    return mode;
}

//---------------------------------------------------------------------------------------------------------------------
void Visualization::SetMode(const Mode &value)
{
    mode = value;
}
