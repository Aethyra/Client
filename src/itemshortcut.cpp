/*
 *  Aethyra
 *  Copyright (C) 2007  The Mana World Development Team
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

#include "configuration.h"
#include "inventory.h"
#include "item.h"
#include "itemshortcut.h"

#include "resources/sprite/localplayer.h"

#include "utils/stringutils.h"

ItemShortcut *itemShortcut;

ItemShortcut::ItemShortcut():
    mItemSelected(-1)
{
    load();
}

ItemShortcut::~ItemShortcut()
{
    save();
}

void ItemShortcut::load()
{
    for (int i = 0; i < SHORTCUT_ITEMS; i++)
    {
        const int itemId = (int) config.getValue("shortcut" + toString(i), -1);

        mItems[i] = itemId;
    }
}

void ItemShortcut::save() const
{
    for (int i = 0; i < SHORTCUT_ITEMS; i++)
    {
        const int itemId = mItems[i] ? mItems[i] : -1;
        config.setValue("shortcut" + toString(i), itemId);
    }
}

void ItemShortcut::useItem(int index)
{
    if (mItems[index])
    {
        Item *item = player_node->getInventory()->findItem(mItems[index]);
        if (item && item->getQuantity())
        {
            if (item->isEquipment())
            {
                if (item->isEquipped())
                    player_node->unequipItem(item);
                else
                    player_node->equipItem(item);
            }
            else
            {
                player_node->useItem(item);
            }
        }
    }
}
