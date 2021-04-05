/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2017  Seamly, LLC                                       *
 *                                                                         *
 *   https://github.com/fashionfreedom/seamly2d                            *
 *                                                                         *
 ***************************************************************************
 **
 **  Seamly2D is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Seamly2D is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Seamly2D.  If not, see <http://www.gnu.org/licenses/>.
 **
 **************************************************************************

 ************************************************************************
 **
 **  @file   vabstractpoint.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   19 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Seamly2D project
 **  <https://github.com/fashionfreedom/seamly2d> All Rights Reserved.
 **
 **  Seamly2D is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Seamly2D is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Seamly2D.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#include "vabstractpoint.h"

#include <QSharedPointer>

#include "../ifc/exception/vexception.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../vgeometry/vgobject.h"
#include "../vdrawtool.h"

//---------------------------------------------------------------------------------------------------------------------
VAbstractPoint::VAbstractPoint(VAbstractPattern *doc, VContainer *data, quint32 id)
    : VDrawTool(doc, data, id)
{}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractPoint::getTagName() const
{
    return VAbstractPattern::TagPoint;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowTool  highlight tool.
 * @param id object id in container.
 * @param enable enable or disable highlight.
 */
void VAbstractPoint::ShowTool(quint32 id, bool enable)
{
    ShowItem(this, id, enable);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPoint::deletePoint()
{
    try
    {
        deleteTool();
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPoint::SetPointName(quint32 id, const QString &name)
{
    // Don't know if need check name here.
    QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
    obj->setName(name);
    SaveOption(obj);
}
