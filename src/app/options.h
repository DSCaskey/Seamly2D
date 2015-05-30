/************************************************************************
 **
 **  @file   options.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses.
 **
 *************************************************************************/

#ifndef OPTIONS_H
#define OPTIONS_H

#include "../libs/ifc/ifcdef.h"
#include "../libs/vgeometry/vgeometrydef.h"
#include "../libs/qmuparser/qmudef.h"
#include "../../utils/def.h"

#ifdef Q_OS_WIN32
#   if defined( Q_CC_MSVC )        // MSVC USED
#       ifndef NOMINMAX
#           define NOMINMAX        // DISABLE min/max MACROS
#       endif
#   endif /*Q_CC_MSVC*/
#   include <windows.h>

#endif /*Q_OS_WIN32*/

class QString;
class QStringList;

#define SceneSize 50000
#define DefPointRadius 1.5//mm

extern const QString degreeSymbol;

extern const QString cursorArrowOpenHand;
extern const QString cursorArrowCloseHand;

enum class SceneObject : char { Point, Line, Spline, Arc, SplinePath, Detail, Unknown };
enum class Tool : unsigned char
{
    Arrow,
    Point,
    LinePoint,
    AbstractSpline,
    Cut,
    SinglePoint,
    EndLine,
    Line,
    AlongLine,
    ShoulderPoint,
    Normal,
    Bisector,
    LineIntersect,
    Spline,
    CutSpline,
    CutArc,
    Arc,
    SplinePath,
    CutSplinePath,
    PointOfContact,
    Detail,
    NodePoint,
    NodeArc,
    NodeSpline,
    NodeSplinePath,
    Height,
    Triangle,
    LineIntersectAxis,
    PointOfIntersectionArcs,
    PointOfIntersectionCircles,
    CurveIntersectAxis,
    PointOfIntersection,
    UnionDetails // 32
};

enum class Vis : unsigned char
{
    ControlPointSpline = 33, // increase this value if need more positions in Tool enum
    GraphicsSimpleTextItem,
    SimpleSplinePath,
    Line,
    Path,
    ToolAlongLine,
    ToolArc,
    ToolBisector,
    ToolCutArc,
    ToolEndLine,
    ToolHeight,
    ToolLine,
    ToolLineIntersect,
    ToolNormal,
    ToolPointOfContact,
    ToolPointOfIntersection,
    ToolPointOfIntersectionArcs,
    ToolPointOfIntersectionCircles,
    ToolShoulderPoint,
    ToolSpline,
    ToolTriangle,
    ToolCutSpline,
    ToolSplinePath,
    ToolCutSplinePath,
    ToolLineIntersectAxis,
    ToolCurveIntersectAxis
};

enum class Source : char { FromGui, FromFile, FromTool };
enum class NodeDetail : char { Contour, Modeling };
enum class Contour : char { OpenContour, CloseContour };
enum class VarType : char { Measurement, Increment, LineLength, SplineLength, ArcLength, LineAngle, Unknown };

enum class GHeights : unsigned char { ALL,
                                      H92=92,   H98=98,   H104=104, H110=110, H116=116, H122=122, H128=128, H134=134,
                                      H140=140, H146=146, H152=152, H158=158, H164=164, H170=170, H176=176, H182=182,
                                      H188=188, H194=194};

enum class GSizes : unsigned char { ALL,
                                    S22=22, S24=24, S26=26, S28=28, S30=30, S32=32, S34=34, S36=36, S38=38, S40=40,
                                    S42=42, S44=44, S46=46, S48=48, S50=50, S52=52, S54=54, S56=56 };

// measurements
//head and neck
extern const QString headGirth_M;
extern const QString midNeckGirth_M;
extern const QString neckBaseGirth_M;
extern const QString headAndNeckLength_M;
//torso
extern const QString centerFrontWaistLength_M;
extern const QString centerBackWaistLength_M;
extern const QString shoulderLength_M;
extern const QString sideWaistLength_M;
extern const QString trunkLength_M;
extern const QString shoulderGirth_M;
extern const QString upperChestGirth_M;
extern const QString bustGirth_M;
extern const QString underBustGirth_M;
extern const QString waistGirth_M;
extern const QString highHipGirth_M;
extern const QString hipGirth_M;
extern const QString upperFrontChestWidth_M;
extern const QString frontChestWidth_M;
extern const QString acrossFrontShoulderWidth_M;
extern const QString acrossBackShoulderWidth_M;
extern const QString upperBackWidth_M;
extern const QString backWidth_M;
extern const QString bustpointToBustpoint_M;
extern const QString halterBustpointToBustpoint_M;
extern const QString neckToBustpoint_M;
extern const QString crotchLength_M;
extern const QString riseHeight_M;
extern const QString shoulderDrop_M;
extern const QString shoulderSlopeDegrees_M;
extern const QString frontShoulderSlopeLength_M;
extern const QString backShoulderSlopeLength_M;
extern const QString frontShoulderToWaistLength_M;
extern const QString backShoulderToWaistLength_M;
extern const QString frontNeckArc_M;
extern const QString backNeckArc_M;
extern const QString frontUpperChestArc_M;
extern const QString backUpperChestArc_M;
extern const QString frontWaistArc_M;
extern const QString backWaistArc_M;
extern const QString frontUpperHipArc_M;
extern const QString backUpperHipArc_M;
extern const QString frontHipArc_M;
extern const QString backHipArc_M;
extern const QString chestSlope_M;
extern const QString backSlope_M;
extern const QString frontWaistSlope_M;
extern const QString backWaistSlope_M;
extern const QString frontNeckToUpperChestHeight_M;
extern const QString frontNeckToBustHeight_M;
extern const QString frontWaistToUpperChest_M;
extern const QString frontWaistToLowerBreast_M;
extern const QString backWaistToUpperChest_M;
extern const QString strapLength_M;
//arm
extern const QString armscyeGirth_M;
extern const QString elbowGirth_M;
extern const QString upperArmGirth_M;
extern const QString wristGirth_M;
extern const QString scyeDepth_M;
extern const QString shoulderAndArmLength_M;
extern const QString underarmLength_M;
extern const QString cervicaleToWristLength_M;
extern const QString shoulderToElbowLength_M;
extern const QString armLength_M;
//hand
extern const QString handWidth_M;
extern const QString handLength_M;
extern const QString handGirth_M;
//leg
extern const QString thighGirth_M;
extern const QString midThighGirth_M;
extern const QString kneeGirth_M;
extern const QString calfGirth_M;
extern const QString ankleGirth_M;
extern const QString kneeHeight_M;
extern const QString ankleHeight_M;
//foot
extern const QString footWidth_M;
extern const QString footLength_M;
//heights
extern const QString height_M;
extern const QString cervicaleHeight_M;
extern const QString cervicaleToKneeHeight_M;
extern const QString waistHeight_M;
extern const QString highHipHeight_M;
extern const QString hipHeight_M;
extern const QString waistToHipHeight_M;
extern const QString waistToKneeHeight_M;
extern const QString crotchHeight_M;
//extended measurements
extern const QString size_M;
extern const QString heightFrontNeckBasePoint_M;
extern const QString heightBaseNeckSidePoint_M;
extern const QString heightShoulderPoint_M;
extern const QString heightNipplePoint_M;
extern const QString heightBackAngleAxilla_M;
extern const QString heightScapularPoint_M;
extern const QString heightUnderButtockFolds_M;
extern const QString hipsExcludingProtrudingAbdomen_M;
extern const QString girthFootInstep_M;
extern const QString sideWaistToFloor_M;
extern const QString frontWaistToFloor_M;
extern const QString arcThroughGroinArea_M;
extern const QString waistToPlaneSeat_M;
extern const QString neckToRadialPoint_M;
extern const QString neckToThirdFinger_M;
extern const QString neckToFirstLineChestCircumference_M;
extern const QString frontWaistLength_M;
extern const QString arcThroughShoulderJoint_M;
extern const QString neckToBackLineChestCircumference_M;
extern const QString waistToNeckSide_M;
extern const QString arcLengthUpperBody_M;
extern const QString chestWidth_M;
extern const QString anteroposteriorDiameterHands_M;
extern const QString heightClavicularPoint_M;
extern const QString heightArmholeSlash_M;
extern const QString slashShoulderHeight_M;
extern const QString halfGirthNeck_M;
extern const QString halfGirthNeckForShirts_M;
extern const QString halfGirthChestFirst_M;
extern const QString halfGirthChestSecond_M;
extern const QString halfGirthChestThird_M;
extern const QString halfGirthWaist_M;
extern const QString halfGirthHipsConsideringProtrudingAbdomen_M;
extern const QString halfGirthHipsExcludingProtrudingAbdomen_M;
extern const QString girthKneeFlexedFeet_M;
extern const QString neckTransverseDiameter_M;
extern const QString frontSlashShoulderHeight_M;
extern const QString neckToFrontWaistLine_M;
extern const QString handVerticalDiameter_M;
extern const QString neckToKneePoint_M;
extern const QString waistToKnee_M;
extern const QString shoulderHeight_M;
extern const QString headHeight_M;
extern const QString bodyPosition_M;
extern const QString arcBehindShoulderGirdle_M;
extern const QString neckToNeckBase_M;
extern const QString depthWaistFirst_M;
extern const QString depthWaistSecond_M;

// variables name
extern const QString line_;
extern const QString angleLine_;

// functions
extern const QString sin_F;
extern const QString cos_F;
extern const QString tan_F;
extern const QString asin_F;
extern const QString acos_F;
extern const QString atan_F;
extern const QString sinh_F;
extern const QString cosh_F;
extern const QString tanh_F;
extern const QString asinh_F;
extern const QString acosh_F;
extern const QString atanh_F;
extern const QString log2_F;
extern const QString log10_F;
extern const QString log_F;
extern const QString ln_F;
extern const QString exp_F;
extern const QString sqrt_F;
extern const QString sign_F;
extern const QString rint_F;
extern const QString abs_F;
extern const QString min_F;
extern const QString max_F;
extern const QString sum_F;
extern const QString avg_F;
extern const QString fmod_F;

extern const QStringList builInFunctions;

// Postfix operators
extern const QString cm_Oprt;
extern const QString mm_Oprt;
extern const QString in_Oprt;

#endif // OPTIONS_H
