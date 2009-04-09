/*
 *  Aethyra
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2008  The Mana World Development Team
 *
 *  This file is part of Aethyra based on original code
 *  from The Legend of Mazzeroth.
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

#include <guichan/font.hpp>

#include <guichan/widgets/label.hpp>

#include "itempopup.h"

#include "../bindings/guichan/graphics.h"
#include "../bindings/guichan/gui.h"
#include "../bindings/guichan/palette.h"

#include "../bindings/guichan/widgets/scrollarea.h"
#include "../bindings/guichan/widgets/textbox.h"

#include "../resources/db/iteminfo.h"

#include "../utils/gettext.h"
#include "../utils/stringutils.h"

ItemPopup::ItemPopup():
    Popup("ItemPopup")
{
    mItemType = "";

    // Item Name
    mItemName = new gcn::Label("");
    mItemName->setFont(boldFont);
    mItemName->setPosition(2, 2);

    // Item Description
    mItemDesc = new TextBox();
    mItemDesc->setEditable(false);
    mItemDescScroll = new ScrollArea(mItemDesc);

    const int fontHeight = getFont()->getHeight();

    mItemDescScroll->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    mItemDescScroll->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    mItemDescScroll->setDimension(gcn::Rectangle(0, 0, 196, fontHeight));
    mItemDescScroll->setOpaque(false);
    mItemDescScroll->setPosition(2, fontHeight);

    // Item Effect
    mItemEffect = new TextBox();
    mItemEffect->setEditable(false);
    mItemEffectScroll = new ScrollArea(mItemEffect);

    mItemEffectScroll->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    mItemEffectScroll->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    mItemEffectScroll->setDimension(gcn::Rectangle(0, 0, 196, fontHeight));
    mItemEffectScroll->setOpaque(false);
    mItemEffectScroll->setPosition(2, (2 * fontHeight) + (2 * getPadding()));

    // Item Weight
    mItemWeight = new TextBox();
    mItemWeight->setEditable(false);
    mItemWeightScroll = new ScrollArea(mItemWeight);

    mItemWeightScroll->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    mItemWeightScroll->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    mItemWeightScroll->setDimension(gcn::Rectangle(0, 0, 196, fontHeight));
    mItemWeightScroll->setOpaque(false);
    mItemWeightScroll->setPosition(2, 3 * fontHeight + 4 * getPadding());

    add(mItemName);
    add(mItemDescScroll);
    add(mItemEffectScroll);
    add(mItemWeightScroll);

    loadPopupConfiguration();
}

ItemPopup::~ItemPopup()
{
}

void ItemPopup::setItem(const ItemInfo &item)
{
    if (item.getName() == mItemName->getCaption())
        return;

    mItemName->setCaption(item.getName());
    mItemName->setWidth(boldFont->getWidth(item.getName()));
    mItemDesc->setTextWrapped(item.getDescription(), 196);
    mItemEffect->setTextWrapped(item.getEffect(), 196);
    mItemWeight->setTextWrapped(_("Weight: ") + toString(item.getWeight()) +
                                _(" grams"), 196);
    mItemType = item.getType();

    int minWidth = mItemName->getWidth();

    if (mItemDesc->getMinWidth() > minWidth)
        minWidth = mItemDesc->getMinWidth();
    if (mItemEffect->getMinWidth() > minWidth)
        minWidth = mItemEffect->getMinWidth();
    if (mItemWeight->getMinWidth() > minWidth)
        minWidth = mItemWeight->getMinWidth();

    minWidth += 8;
    setWidth(minWidth);

    const int numRowsDesc = mItemDesc->getNumberOfRows();
    const int numRowsEffect = mItemEffect->getNumberOfRows();
    const int numRowsWeight = mItemWeight->getNumberOfRows();
    const int height = getFont()->getHeight();

    mItemDescScroll->setDimension(gcn::Rectangle(2, 0, minWidth, numRowsDesc *
                                                 height));

    mItemEffectScroll->setDimension(gcn::Rectangle(2, 0, minWidth,
                                                   numRowsEffect * height));

    mItemWeightScroll->setDimension(gcn::Rectangle(2, 0, minWidth,
                                                   numRowsWeight * height));

    if (item.getEffect().empty())
    {
        setContentSize(minWidth, (numRowsDesc + 3) * height);

        mItemWeightScroll->setPosition(2, (numRowsDesc + 2) * height);
    }
    else
    {
        setContentSize(minWidth, (numRowsDesc + numRowsEffect + 3) * height);

        mItemWeightScroll->setPosition(2, (numRowsDesc + numRowsEffect + 2) *
                                       height);
    }

    mItemDescScroll->setPosition(2, 20);
    mItemEffectScroll->setPosition(2, (numRowsDesc + 2) * height);
}

void ItemPopup::updateColors()
{
    mItemName->setForegroundColor(getColor(mItemType));
    graphics->setColor(guiPalette->getColor(Palette::TEXT));
}

gcn::Color ItemPopup::getColor(const std::string& type)
{
    gcn::Color color;

    if (type.compare("generic") == 0)
        color = guiPalette->getColor(Palette::GENERIC);
    else if (type.compare("equip-head") == 0)
        color = guiPalette->getColor(Palette::HEAD);
    else if (type.compare("usable") == 0)
        color = guiPalette->getColor(Palette::USABLE);
    else if (type.compare("equip-torso") == 0)
        color = guiPalette->getColor(Palette::TORSO);
    else if (type.compare("equip-1hand") == 0)
        color = guiPalette->getColor(Palette::ONEHAND);
    else if (type.compare("equip-legs") == 0)
        color = guiPalette->getColor(Palette::LEGS);
    else if (type.compare("equip-feet") == 0)
        color = guiPalette->getColor(Palette::FEET);
    else if (type.compare("equip-2hand") == 0)
        color = guiPalette->getColor(Palette::TWOHAND);
    else if (type.compare("equip-shield") == 0)
        color = guiPalette->getColor(Palette::SHIELD);
    else if (type.compare("equip-ring") == 0)
        color = guiPalette->getColor(Palette::RING);
    else if (type.compare("equip-arms") == 0)
        color = guiPalette->getColor(Palette::ARMS);
    else if (type.compare("equip-ammo") == 0)
        color = guiPalette->getColor(Palette::AMMO);
    else
        color = guiPalette->getColor(Palette::UNKNOWN_ITEM);

    return color;
}

std::string ItemPopup::getItemName() const
{
    return mItemName->getCaption();
}

unsigned int ItemPopup::getNumRows() const
{
    return mItemDesc->getNumberOfRows() + mItemEffect->getNumberOfRows() +
           mItemWeight->getNumberOfRows();
}

void ItemPopup::view(int x, int y)
{
    if (graphics->getWidth() < (x + getWidth() + 5))
	x = graphics->getWidth() - getWidth();
    if ((y - getHeight() - 10) < 0)
	y = 0;
    else
        y = y - getHeight() - 10;
    setPosition(x, y);
    setVisible(true);
    requestMoveToTop();
}
