//-----------------------------------------------------------------------------
//  @file   marker_dialog.h
//  @author Douglas S Caskey
//  @date   20 Apr, 2025
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
//-----------------------------------------------------------------------------

#ifndef MARKER_DIALOG_H
#define MARKER_DIALOG_H

#include "../vtools/dialogs/tools/dialogtool.h"

namespace Ui
{
    class MarkerDialog;
}

class MarkerDialog : public DialogTool
{
    Q_OBJECT
public:
    explicit          MarkerDialog(const VContainer *data, Marker mode, QWidget *parent = nullptr);
    virtual          ~MarkerDialog();

    void              nameChanged();

    QString           getName() const;
    void              setName(const QString &name);

    bool              convertToPath() const;
    void              setConvertToPath(bool value);

    SpreadType        getType() const;
    void              setType(SpreadType type);

    QString           getMaterial() const;
    void              setMaterial(const QString &material);

    qreal             getFabricWidth() const;
    void              setFabricWidth(const qreal &width);

    qreal             getCutLength() const;
    void              setCutLength(const qreal &length);

    qreal             getSelvageTop() const;
    void              setSelvageTop(const qreal &value);

    qreal             getSelvageBottom() const;
    void              setSelvageBottom(const qreal &value);

    qreal             getPaperHeight() const;
    void              setPaperHeight(const qreal &value);

    qreal             getPaperWidth() const;
    void              setPaperWidth(const qreal &value);

public slots:
    void              DialogAccepted();


protected:
    virtual void      showEvent(QShowEvent *event) override;
    virtual void      CheckState() Q_DECL_FINAL;
    virtual void      closeEvent(QCloseEvent *event) override;

private:
    Q_DISABLE_COPY(MarkerDialog)
    Ui::MarkerDialog *ui;
    Marker            m_mode;
    bool              m_isInitialized;

    void              initialize();
    void              initializeFabricTab();
    void              initializePaperTab();
};

#endif // MARKER_DIALOG_H
