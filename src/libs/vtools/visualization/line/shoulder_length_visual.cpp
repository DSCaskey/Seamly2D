//-----------------------------------------------------------------------------
//  @file   shoulder_length_visual.cpp
//  @author Douglas S Caskey
//  @date   9 Aug, 2024
//
//  @brief
//  @copyright
//  This source code is part of the Seamly2D project, a pattern making
//  program, whose allow create and modeling patterns of clothing.
//  Copyright (C) 2013-2024 Seamly2D project
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
//  @file   vistoolshoulderpoint.cpp
//  @author Roman Telezhynskyi <dismine(at)gmail.com>
//  @date   25 7, 2014
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

#include "shoulder_length_visual.h"

#include <QGraphicsLineItem>
#include <QLineF>
#include <QPointF>
#include <QSharedPointer>
#include <Qt>
#include <new>

#include "../../tools/drawTools/toolpoint/toolsinglepoint/toollinepoint/shoulder_length_tool.h"
#include "../ifc/ifcdef.h"
#include "../vmisc/vabstractapplication.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"
#include "../visualization.h"
#include "visline.h"

//---------------------------------------------------------------------------------------------------------------------
ShoulderLengthVisual::ShoulderLengthVisual(const VContainer *data, QGraphicsItem *parent)
    : VisLine(data, parent)
    , m_lineP1Id(NULL_ID)
    , m_lineP2Id(NULL_ID)
    , m_neckPoint(nullptr)
    , m_toolPoint(nullptr)
    , m_baselineP1(nullptr)
    , m_baselineP2(nullptr)
    , m_line1(nullptr)
    , m_line2(nullptr)
    , m_line3(nullptr)
    , m_length(0)
{
    m_neckPoint  = InitPoint(supportColor, this);
    m_line1      = InitItem<VScaledLine>(supportColor, this);
    m_baselineP1 = InitPoint(supportColor, this);
    m_line2      = InitItem<VScaledLine>(supportColor, this);
    m_baselineP2 = InitPoint(supportColor, this);
    m_line3      = InitItem<VScaledLine>(mainColor, this);
    m_toolPoint  = InitPoint(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
void ShoulderLengthVisual::RefreshGeometry()
{
    if (object1Id > NULL_ID)
    {
        const QSharedPointer<VPointF> neckPoint = Visualization::data->GeometricObject<VPointF>(object1Id);
        DrawPoint(m_neckPoint, static_cast<QPointF>(*neckPoint), supportColor);

        if (m_lineP1Id <= NULL_ID)
        {
            DrawLine(m_line1, QLineF(static_cast<QPointF>(*neckPoint), Visualization::scenePos), supportColor, lineWeight);
        }
        else
        {
            const QSharedPointer<VPointF> linePoint1 = Visualization::data->GeometricObject<VPointF>(m_lineP1Id);
            DrawLine(m_line1, QLineF(static_cast<QPointF>(*neckPoint), static_cast<QPointF>(*linePoint1)),
                                   supportColor, lineWeight, Qt::SolidLine);

            DrawPoint(m_baselineP1, static_cast<QPointF>(*linePoint1), supportColor);

            if (m_lineP2Id <= NULL_ID)
            {
                DrawLine(m_line2, QLineF(static_cast<QPointF>(*linePoint1), Visualization::scenePos),
                                       supportColor, lineWeight);
            }
            else
            {
                const QSharedPointer<VPointF> linePoint2 = Visualization::data->GeometricObject<VPointF>(m_lineP2Id);
                DrawPoint(m_baselineP2, static_cast<QPointF>(*linePoint2), supportColor);

                DrawLine(m_line2, QLineF(static_cast<QPointF>(*linePoint1), static_cast<QPointF>(*linePoint2)),
                                         supportColor, lineWeight);

                if (!qFuzzyIsNull(m_length))
                {
                    QPointF toolPoint = ShoulderLengthTool::findPoint(static_cast<QPointF>(*linePoint1),
                                                                   static_cast<QPointF>(*linePoint2),
                                                                   static_cast<QPointF>(*neckPoint), m_length);
                    QLineF rayLine = QLineF(static_cast<QPointF>(*linePoint2), toolPoint);
                    DrawLine(m_line3, rayLine, supportColor, lineWeight, Qt::DashLine);

                    DrawPoint(m_toolPoint, toolPoint, mainColor);
                    QLineF toolLine = QLineF(static_cast<QPointF>(*neckPoint), toolPoint);
                    DrawLine(this, toolLine, mainColor, lineWeight, Qt::SolidLine);
                }
                else if (GetMode() == Mode::Creation)
                {
                    QLineF const cursorLine(static_cast<QPointF>(*neckPoint), Visualization::scenePos);

                    qreal const length = cursorLine.length();
                    QPointF const toolPoint = ShoulderLengthTool::findPoint(static_cast<QPointF>(*linePoint1),
                                                                            static_cast<QPointF>(*linePoint2),
                                                                            static_cast<QPointF>(*neckPoint), length);
                    QLineF const mainLine = QLineF(static_cast<QPointF>(*linePoint1), toolPoint);
                    DrawLine(this, mainLine, mainColor, lineWeight, lineStyle);

                    DrawPoint(m_neckPoint, static_cast<QPointF>(*neckPoint), supportColor);
                    DrawLine(m_line3, QLineF(static_cast<QPointF>(*neckPoint), mainLine.p2()), supportColor,
                                           lineWeight, Qt::DashLine);

                    static const QString prefix = UnitsToStr(qApp->patternUnit(), true);
                    Visualization::toolTip = tr("<b>Length to Line</b>: length = %1%2, "
                                                "<b>Click mouse/b> to finish selecting the length, "
                                                "Press <b>ENTER</b> to skip")
                                                .arg(qApp->translateVariables()->FormulaToUser(QString::number(qApp->fromPixel(length)),
                                                     qApp->Settings()->getOsSeparator()), prefix);
                }
                else
                {
                    qreal angle = QLineF(static_cast<QPointF>(*linePoint1), static_cast<QPointF>(*linePoint2)).angle();
                    QPointF endRay = Ray(static_cast<QPointF>(*linePoint1), angle);
                    QLineF rayLine = VGObject::BuildLine(static_cast<QPointF>(*linePoint1),
                                                          QLineF(static_cast<QPointF>(*linePoint1), endRay).length(),
                                                          angle);
                    DrawLine(this, rayLine, mainColor, lineWeight, lineStyle);
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void ShoulderLengthVisual::setLineP1Id(const quint32 &value)
{
    m_lineP1Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void ShoulderLengthVisual::setLineP2Id(const quint32 &value)
{
    m_lineP2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void ShoulderLengthVisual::setLength(const QString &expression)
{
    m_length = FindLength(expression, Visualization::data->DataVariables());
}
