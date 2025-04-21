//-----------------------------------------------------------------------------
//  @file   marker_piece.cpp
//  @author Douglas S Caskey
//  @date  13 Apr, 2025
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
//-----------------------------------------------------------------------------

#include "marker_piece.h"
#include "ui_marker_piece.h"

MarkerPiece::MarkerPiece(VContainer *data, VAbstractPattern *doc, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PiecesWidget)
{

}

MarkerPiece::~MarkerPiece(){}
