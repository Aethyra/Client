/*
 *  Aethyra
 *  Copyright (c) 2009  Aethyra Development Team 
 *
 *  This file is part of Aethyra based on original code
 *  from GUIChan.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include "label.h"

#include "../palette.h"

Label::Label() :
    gcn::Label()
{
}

Label::Label(const std::string& caption) :
    gcn::Label(caption)
{
}

void Label::draw(gcn::Graphics* graphics)
{
    setForegroundColor(guiPalette->getColor(Palette::TEXT));
    gcn::Label::draw(static_cast<gcn::Graphics*>(graphics));
}
