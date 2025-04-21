//---------------------------------------------------------------------------------------------------------------------
//  @file   marker_item.cpp
//  @author Douglas S Caskey
//  @date   2 May, 2025
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
//  along with Seamly2D. If not, see <http://www.gnu.org/licenses/>.
//---------------------------------------------------------------------------------------------------------------------

#include "marker_item.h"

#include <QGraphicsScene>
#include <QRectF>
#include <Qt>
#include <QGraphicsItem>

//---------------------------------------------------------------------------------------------------------------------
MarkerItem::MarkerItem(QObject *parent)
    : QObject(parent)
    , m_boundingRect(QRectF())
    , m_workingRect(QRectF())
    , m_mode(Mode::Normal)
    , m_inactiveZ(1)
    , m_width()
    , m_length()
    , m_selvageTopWidth()
    , m_selvageBottomWidth()
{
    m_boundingRect.setTopLeft(QPointF(0, 0));
    setAcceptHoverEvents(true);
}

//---------------------------------------------------------------------------------------------------------------------
MarkerItem::~MarkerItem()
{
}

//---------------------------------------------------------------------------------------------------------------------
void VPGraphicsSheet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

}

//---------------------------------------------------------------------------------------------------------------------
/// @brief boundingRect returns the item bounding box
/// @return item bounding box
//---------------------------------------------------------------------------------------------------------------------
QRectF MarkerItem::boundingRect() const
{
    return m_boundingRect;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief reset resets the item, putting the mode and z coordinate to normal and redraws it
//---------------------------------------------------------------------------------------------------------------------
void MarkerItem::reset()
{
    if (QGraphicsScene *toolScene = scene())
    {
        toolScene->clearSelection();
    }

    updateItem();
    setZValue(m_inactiveZ);
}
