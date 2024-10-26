//-----------------------------------------------------------------------------
//  @file   shoulder_length_dialog.h
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
//  @file   dialogshoulderpoint.h
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

#ifndef SHOULDER_LENGTH_DIALOG_H
#define SHOULDER_LENGTH_DIALOG_H

#include <qcompilerdetection.h>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "dialogtool.h"

namespace Ui
{
    class ShoulderLengthDialog;
}


/// @brief The ShoulderLengthDialog class dialog for ToolShoulderPoint. Help create point and edit option.

class ShoulderLengthDialog : public DialogTool
{
    Q_OBJECT
public:
                   ShoulderLengthDialog(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    virtual       ~ShoulderLengthDialog() Q_DECL_OVERRIDE;

    void           setPointName(const QString &value);

    QString        getLineType() const;
    void           setLineType(const QString &value);

    QString        getLineWeight() const;
    void           setLineWeight(const QString &value);

    QString        getLineColor() const;
    void           setLineColor(const QString &value);

    QString        getFormula() const;
    void           setFormula(const QString &value);


    quint32        getNeckPoint() const;
    void           setNeckPoint(const quint32 &value);

    quint32        getFirstLinePoint() const;
    void           setFirstLinePoint(const quint32 &value);

    quint32        getSecondLinePoint() const;
    void           setSecondLinePoint(const quint32 &value);

    void           ShowDialog(bool click) override;

public slots:
    virtual void   ChosenObject(quint32 id, const SceneObject &type) Q_DECL_OVERRIDE;

    /// @brief DeployFormulaTextEdit grow or shrink formula input
    void           DeployFormulaTextEdit();

    /// @brief formulaTextChanged when formula text changes for validation and calc
    void           formulaTextChanged();
    virtual void   PointNameChanged() Q_DECL_OVERRIDE;
    void           FXLength();

protected:
    virtual void   ShowVisualization() Q_DECL_OVERRIDE;
    virtual void   SaveData() Q_DECL_OVERRIDE;
    virtual void   closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(ShoulderLengthDialog)

    /// @brief ui keeps information about user interface */
    Ui::ShoulderLengthDialog *ui;

    /// @brief formula formula */
    QString        m_formula;

    /// @brief formulaBaseHeight base height defined by dialogui */
    int            m_formulaBaseHeight;
    bool           m_firstRelease;

    void           FinishCreating();
};

#endif // SHOULDER_LENGTH_DIALOG_H
