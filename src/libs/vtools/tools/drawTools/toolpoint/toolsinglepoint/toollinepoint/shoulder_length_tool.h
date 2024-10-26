//-----------------------------------------------------------------------------
//  @file   shoulder_length_tool.h
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
//  @file   vtoolshoulderpoint.h
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

#ifndef SHOULDER_LENGTH_TOOL_H
#define SHOULDER_LENGTH_TOOL_H

#include <qcompilerdetection.h>
#include <QDomElement>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QtGlobal>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "vtoollinepoint.h"

template <class T> class QSharedPointer;


/// @brief The ShoulderLengthTool class tool for creation of point  Length to Line. This tool for special situation, when you
/// want find point along line, but have only length from another point (shoulder).
class ShoulderLengthTool : public VToolLinePoint
{
    Q_OBJECT
public:
    virtual void               setDialog() Q_DECL_OVERRIDE;
    static QPointF             findPoint(const QPointF &lineP1, const QPointF &lineP2, const QPointF &neckPoint,
                                         const qreal &length);

    static ShoulderLengthTool *Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene  *scene,
                                      VAbstractPattern *doc, VContainer *data);
    static ShoulderLengthTool *Create(const quint32 _id, QString &formula,
                                      quint32 neckPoint, quint32 lineP1, quint32 lineP2,
                                      const QString &typeLine, const QString &lineWeight, const QString &lineColor,
                                      const QString &pointName, qreal mx, qreal my, bool showPointName,
                                      VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data,
                                      const Document &parse,
                                      const Source &typeCreation);

    static const QString       ToolType;
    virtual int                type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::ShoulderPoint) };

    QString                    linePoint1Name() const;
    QString                    linePoint2Name() const;

    quint32                    getFirstLinePoint() const;
    void                       setFirstLinePoint(const quint32 &value);

    quint32                    getSecondLinePoint() const;
    void                       setSecondLinePoint(const quint32 &value);

    virtual void               ShowVisualization(bool show) Q_DECL_OVERRIDE;

protected slots:
    virtual void               showContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id=NULL_ID) Q_DECL_OVERRIDE;


protected:
    virtual void               RemoveReferens() Q_DECL_OVERRIDE;
    virtual void               SaveDialog(QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void               SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;
    virtual void               ReadToolAttributes(const QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void               SetVisualization() Q_DECL_OVERRIDE;
    virtual QString            makeToolTip() const Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(ShoulderLengthTool)

    quint32                    m_lineP1Id;     /** @brief lineP2 id first line point. */
    quint32                    m_lineP2Id;     /** @brief lineP2 id second line point. */

    ShoulderLengthTool(VAbstractPattern *doc, VContainer *data, const quint32 &id,
                       const QString &typeLine, const QString &lineWeight,
                       const QString &lineColor, const QString &formula,
                       const quint32 &neckPoint, const quint32 &lineP1, const quint32 &lineP2,
                       const Source &typeCreation, QGraphicsItem * parent = nullptr);
};

#endif // SHOULDER_LENGTH_TOOL_H
