//---------------------------------------------------------------------------------------------------------------------
//  @file   vabstractnode.h
//  @author Douglas S Caskey
//  @date   2 Apr, 2025
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
//  along with Seamly2D. if not, see <http://www.gnu.org/licenses/>.
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------//
//  @file   vabstractnode.h
//  @author Roman Telezhynskyi <dismine(at)gmail.com>
//  @date   15 11, 2013
//
//  @brief
//  @copyright
//  This source code is part of the Valentina project, a pattern making
//  program, whose allow create and modeling patterns of clothing.
//  Copyright (C) 2017 Valentina project
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

#ifndef VABSTRACTNODE_H
#define VABSTRACTNODE_H

#include <qcompilerdetection.h>
#include <QColor>
#include <QDomElement>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../vabstracttool.h"

enum class ParentType : bool {Scene, Item};

/**
 * @brief The VAbstractNode class parent class for all detail node.
 */
class VAbstractNode : public VAbstractTool
{
    Q_OBJECT
public:
    VAbstractNode(VAbstractPattern *doc, VContainer *data, const quint32 &id, const quint32 &idNode,
                  const QString &blockName = QString(), const quint32 &idTool = 0, QObject *parent = nullptr);
    virtual      ~VAbstractNode() Q_DECL_EQ_DEFAULT;
    static const QString AttrIdTool;
    virtual void ShowVisualization(bool show) override;
    virtual void incrementReferens() override;
    virtual void decrementReferens() override;

    ParentType   GetParentType() const;
    void         SetParentType(const ParentType &value);

    virtual void GroupVisibility(quint32 object, bool visible) override;

    bool         IsExluded() const;
    void         SetExluded(bool exluded);

protected:
    ParentType   parentType;
    quint32      idNode; /// @brief idNodenode id.
    quint32      idTool; /// @brief idTool id tool.
    QString      m_blockName;
    bool         m_exluded;

    void         AddToModeling(const QDomElement &domElement);
    virtual void ToolCreation(const Source &typeCreation) override;
    virtual void SetVisualization() override {}

    virtual void ShowNode()=0;
    virtual void HideNode()=0;

private:
    Q_DISABLE_COPY(VAbstractNode)
};

#endif // VABSTRACTNODE_H
