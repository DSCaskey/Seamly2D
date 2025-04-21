//---------------------------------------------------------------------------------------------------------------------
//  @file   marker_item.h
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

#ifndef MARKER_ITEM_H
#define MARKER_ITEM_H

#include <QtCore/qglobal.h>
#include <QGraphicsObject>

#include "../vmisc/def.h"

class MarkerItem :  public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    explicit              MarkerItem(QObject *parent = nullptr);
    virtual              ~MarkerItem();

    virtual QRectF        boundingRect() const override;
    virtual void          paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                QWidget *widget = nullptr) override;

    virtual void          updateItem() =0;

    void                  reset();

    virtual int           type() const override {return Type;}
    enum                  {Type = UserType + static_cast<int>(Vis::MarkerItem)};

signals:

protected:

private:
                          Q_DISABLE_COPY(MarkerItem)
    QRectF                m_boundingRect;
    QRectF                m_workingRect;
    Mode                  m_mode;
    qreal                 m_inactiveZ;
    qreal                 m_width;
    qreal                 m_length;
    qreal                 m_selvageTopWidth;
    qreal                 m_selvageBottomWidth;

    void                  paintFold(QPainter *painter, const QRectF &sheetRect) const;
    void                  paintSelevages(QPainter *painter, const QRectF &sheetRect) const;};

#endif // MARKER_ITEM_H
