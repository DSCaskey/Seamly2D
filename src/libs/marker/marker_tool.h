//******************************************************************************
//  @file   pattern_piece_tool.h
//  @author Douglas S Caskey
//  @date   22 Apr, 2025
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
//-----------------------------------------------------------------------------

#ifndef MARKER_TOOL_H
#define MARKER_TOOL_H

#include <QtGlobal>
#include <qcompilerdetection.h>
#include <QObject>
#include <QGraphicsPathItem>

#include "vinteractivetool.h"

#include "../vpatterndb/vpiece.h"
#include "../vwidgets/vtextgraphicsitem.h"
#include "../vwidgets/vgrainlineitem.h"

class DialogTool;
class NonScalingFillPathItem;

class MarkerTool : public VInteractiveTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
    virtual ~MarkerTool() Q_DECL_EQ_DEFAULT;
    static MarkerTool *Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene *scene,
                                      VAbstractPattern *doc, VContainer *data);

    static MarkerTool *Create(quint32 id, VPiece newPiece, QString &width, VMainGraphicsScene *scene,
                                      VAbstractPattern *doc, VContainer *data, const Document &parse,
                                      const Source &typeCreation, const QString &blockName = QString());

    static const quint8  pieceVersion;
    static const QString TagCSA;
    static const QString TagRecord;
    static const QString TagIPaths;
    static const QString TagAnchors;
    static const QString AttrVersion;
    static const QString AttrForbidFlipping;
    static const QString AttrPieceColor;
    static const QString AttrPieceFill;
    static const QString AttrPieceLock;
    static const QString AttrSeamAllowance;
    static const QString AttrHideSeamLine;
    static const QString AttrSeamAllowanceBuiltIn;
    static const QString AttrHeight;
    static const QString AttrUnited;
    static const QString AttrFont;
    static const QString AttrTopLeftAnchor;
    static const QString AttrBottomRightAnchor;
    static const QString AttrCenterAnchor;
    static const QString AttrTopAnchorPoint;
    static const QString AttrBottomAnchorPoint;

    void        Remove(bool ask);

    static void insertNodes(const QVector<VPieceNode> &nodes, quint32 pieceId, VMainGraphicsScene *scene,
                            VContainer *data, VAbstractPattern *doc);

    static void AddAttributes(VAbstractPattern *doc, QDomElement &domElement, quint32 id, const VPiece &piece);
    static void AddCSARecord(VAbstractPattern *doc, QDomElement &domElement, const CustomSARecord &record);
    static void AddCSARecords(VAbstractPattern *doc, QDomElement &domElement, const QVector<CustomSARecord> &records);
    static void AddInternalPaths(VAbstractPattern *doc, QDomElement &domElement, const QVector<quint32> &paths);
    static void addAnchors(VAbstractPattern *doc, QDomElement &domElement, const QVector<quint32> &anchors);
    static void AddPatternPieceData(VAbstractPattern *doc, QDomElement &domElement, const VPiece &piece);
    static void AddPatternInfo(VAbstractPattern *doc, QDomElement &domElement, const VPiece &piece);
    static void AddGrainline(VAbstractPattern *doc, QDomElement &domElement, const VPiece &piece);

    void                 RefreshGeometry();

    virtual int          type() const override {return Type;}
    enum                 { Type = UserType + static_cast<int>(Tool::Piece)};

    virtual QString      getTagName() const override;
    virtual void         ShowVisualization(bool show) override;
    virtual void         GroupVisibility(quint32 object, bool visible) override;
    virtual void         paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                               QWidget *widget) override;

    virtual QRectF       boundingRect() const override;
    virtual QPainterPath shape() const override;

public slots:
    virtual void         FullUpdateFromFile () override;
    void                 EnableToolMove(bool move);
    void                 pieceLockedChanged(quint32 id, bool lock);
    virtual void         AllowHover(bool enabled) override;
    virtual void         AllowSelecting(bool enabled) override;
    virtual void         ResetChildren(QGraphicsItem* pItem);
    virtual void         UpdateAll();
    virtual void         retranslateUi();
    void                 Highlight(quint32 id);
    void                 updatePieceDetails();
    void                 UpdatePieceLabel();
    void                 UpdatePatternLabel();
    void                 UpdateGrainline();
    void                 editPieceProperties();

protected slots:
    void                 saveMovePiece(const QPointF &ptPos);
    void                 saveResizePiece(qreal dLabelW, int iFontSize);
    void                 savePieceRotation(qreal dRot);
    void                 SaveMovePattern(const QPointF& ptPos);
    void                 SaveResizePattern(qreal dLabelW, int iFontSize);
    void                 SaveRotationPattern(qreal dRot);
    void                 SaveMoveGrainline(const QPointF& ptPos);
    void                 SaveResizeGrainline(qreal dLength);
    void                 SaveRotateGrainline(qreal dRot, const QPointF& ptPos);

private slots:
    void                 nodeAngleChanged(quint32 id, PieceNodeAngle notchData);
    void                 notchChanged(quint32 id, NotchData notchData);
    void                 nodeExcluded(quint32 id);
    void                 nodeDeleted(quint32 id);

protected:
    virtual void         AddToFile () override;
    virtual void         RefreshDataInFile() override;
    virtual QVariant     itemChange ( GraphicsItemChange change, const QVariant &value ) override;
    virtual void         mousePressEvent( QGraphicsSceneMouseEvent * event) override;
    virtual void         mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ) override;
    virtual void         hoverEnterEvent ( QGraphicsSceneHoverEvent * event ) override;
    virtual void         hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ) override;
    virtual void         contextMenuEvent (QGraphicsSceneContextMenuEvent * event ) override;
    virtual void         keyReleaseEvent(QKeyEvent * event) override;
    virtual void         SetVisualization() override {}
    virtual void         deleteTool(bool ask = true) override;
    virtual void         ToolCreation(const Source &typeCreation) override;
    virtual void         SetDialog() Q_DECL_FINAL;
    virtual void         SaveDialogChange() Q_DECL_FINAL;

private:
    Q_DISABLE_COPY(MarkerTool)

    QRectF                  m_markerRect;
    VMainGraphicsScene     *m_MarkerScene;  /// @brief pieceScene pointer to the scene. */
    MarkerItem             *m_marker;

                           MarkerTool(VAbstractPattern *doc, VContainer *data, const quint32 &id,
                                              const Source &typeCreation, VMainGraphicsScene *scene,
                                              const QString &blockName, QGraphicsItem * parent = nullptr);

    void                  initializeNodes(const VPiece &piece, VMainGraphicsScene *scene);
    static void           initializeNode(const VPieceNode &node, VMainGraphicsScene *scene, VContainer *data,
                                   VAbstractPattern *doc, MarkerTool *parent);

    static QVector<VPieceNode>   removeDuplicateNodePoints(const VPiece &piece, const QVector<VPieceNode> &nodes, VContainer *data);

    void                  InitCSAPaths(const VPiece &piece);
    void                  InitInternalPaths(const VPiece &piece);
    void                  initializeAnchorPoints(const VPiece &piece);
    bool                  PrepareLabelData(const VPatternLabelData &labelData, VTextGraphicsItem *labelItem,
                                           QPointF &pos, qreal &labelAngle);

    void                  UpdateLabelItem(VTextGraphicsItem *labelItem, QPointF pos, qreal labelAngle);

    void                  toggleInLayout(bool checked);
    void                  togglePieceLock(bool checked);
    void                  toggleFlipping(bool checked);
    void                  toggleSeamLine(bool checked);
    void                  toggleSeamAllowance(bool checked);
    void                  toggleGrainline(bool checked);
    void                  togglePatternLabel(bool checked);
    void                  togglePieceLabel(bool checked);
    void                  renamePiece(VPiece piece);
    void                  showStatus(QString toolTip);
    void                  raiseItemToTop(QGraphicsItem *item);
    void                  lowerItemToBottom(QGraphicsItem *item);
};

#endif // MARKER_TOOL_H
