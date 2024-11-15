//---------------------------------------------------------------------------------------------------------------------
//  @file   vistoolpointofcontact.cpp
//  @author Douglas S Caskey
//  @date   13 Nov, 2024
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
//  @file   vistoolpointofcontact.cpp
//  @author Roman Telezhynskyi <dismine(at)gmail.com>
//  @date   14 8, 2014
//
//  @brief
//  @copyright
//  This source code is part of the Valentina project, a pattern making
//  program, whose allow create and modeling patterns of clothing.
//  Copyright (C) 2013-2015 Valentina project
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
//---------------------------------------------------------------------------------------------------------------------

#include "vistoolpointofcontact.h"

#include <QGraphicsEllipseItem>
#include <QLineF>
#include <QPointF>
#include <QSharedPointer>
#include <Qt>
#include <new>

#include "../../tools/drawTools/toolpoint/toolsinglepoint/vtoolpointofcontact.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"
#include "../visualization.h"
#include "visline.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolPointOfContact::VisToolPointOfContact(const VContainer *data, QGraphicsItem *parent)
    : VisLine(data, parent)
    , m_lineP2Id(NULL_ID)
    , m_centerId(NULL_ID)
    , m_point(nullptr)
    , m_lineP1(nullptr)
    , m_lineP2(nullptr)
    , m_arc_point(nullptr)
    , m_circle(nullptr)
    , m_radius(0)
    , m_ray1(nullptr)
    , m_ray2(nullptr)
    , m_secondrySupportColor(QColor(qApp->Settings()->getSecondarySupportColor()))
{
    this->setZValue(110);// Show on top real tool
    m_arc_point = InitPoint(supportColor, this);
    m_lineP1    = InitPoint(supportColor, this);
    m_lineP2    = InitPoint(supportColor, this);
    m_circle    = InitItem<QGraphicsEllipseItem>(supportColor, this);
    m_point     = InitPoint(mainColor, this);
    m_ray1      = InitItem<VScaledLine>(supportColor, this);
    m_ray2      = InitItem<VScaledLine>(supportColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfContact::RefreshGeometry()
{
    if (object1Id > NULL_ID)
    {
        const QSharedPointer<VPointF> first = Visualization::data->GeometricObject<VPointF>(object1Id);
        DrawPoint(m_lineP1, static_cast<QPointF>(*first), supportColor);

        if (m_lineP2Id <= NULL_ID)
        {
            DrawLine(this, QLineF(static_cast<QPointF>(*first), Visualization::scenePos), supportColor, lineWeight);
        }
        else
        {
            const QSharedPointer<VPointF> second = Visualization::data->GeometricObject<VPointF>(m_lineP2Id);
            DrawPoint(m_lineP2, static_cast<QPointF>(*second), supportColor);
            DrawLine(this, QLineF(static_cast<QPointF>(*first), static_cast<QPointF>(*second)),
                     supportColor, widthMainLine);

            if (m_centerId <= NULL_ID)
            {
                return;
            }
            else
            {
                const QSharedPointer<VPointF> center = Visualization::data->GeometricObject<VPointF>(m_centerId);
                DrawPoint(m_arc_point, static_cast<QPointF>(*center), supportColor);
                DrawLine(this, QLineF(static_cast<QPointF>(*first), static_cast<QPointF>(*second)),
                         supportColor, widthMainLine);
                DrawRay(m_ray1, static_cast<QPointF>(*second), static_cast<QPointF>(*first), supportColor, Qt::DashLine);
                DrawRay(m_ray2, static_cast<QPointF>(*first), static_cast<QPointF>(*second), supportColor, Qt::DashLine);

                if (!qFuzzyIsNull(m_radius))
                {
                    QPointF toolPoint = VToolPointOfContact::FindPoint(m_radius, static_cast<QPointF>(*center),
                                                                       static_cast<QPointF>(*first),
                                                                       static_cast<QPointF>(*second));
                    DrawPoint(m_point, toolPoint, mainColor);

                    m_circle->setRect(pointRect(m_radius));
                    DrawPoint(m_circle, static_cast<QPointF>(*center), m_secondrySupportColor,
                                                           widthMainLine, Qt::DashLine);

                    const QSharedPointer<VPointF> center = Visualization::data->GeometricObject<VPointF>(m_centerId);
                    DrawLine(this, QLineF(static_cast<QPointF>(*center), toolPoint), m_secondrySupportColor,
                             widthMainLine, Qt::SolidLine);
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfContact::setLineP2Id(const quint32 &value)
{
    m_lineP2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfContact::setRadiusId(const quint32 &value)
{
    m_centerId = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfContact::setRadius(const QString &expression)
{
    m_radius = FindLength(expression, Visualization::data->DataVariables());
}
