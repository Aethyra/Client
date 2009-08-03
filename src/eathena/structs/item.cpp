/*
 *  Aethyra
 *  Copyright (C) 2004  The Mana World Development Team
 *
 *  This file is part of Aethyra based on original code
 *  from The Mana World.
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

#include "item.h"

#include "../../core/resourcemanager.h"

#include "../../core/image/image.h"

Item::Item(const int id, const int quantity, const bool equipment,
           const bool equipped) :
    mImage(0),
    mQuantity(quantity),
    mEquipment(equipment),
    mEquipped(equipped)
{
    setId(id);
}

Item::~Item()
{
    if (mImage)
        mImage->decRef();
}

void Item::setId(const int id)
{
    mId = id;

    // Load the associated image
    if (mImage)
        mImage->decRef();

    ResourceManager *resman = ResourceManager::getInstance();
    std::string imagePath = "graphics/items/" + getInfo().getImageName();
    mImage = resman->getImage(imagePath);

    if (!mImage)
        mImage = resman->getImage("graphics/gui/unknown-item.png");
}
