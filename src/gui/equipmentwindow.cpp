/*
 *  Aethyra
 *  Copyright (C) 2004  The Mana World Development Team
 *
 *  This file is part of Aethyra derived from original code
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

#define BOX_WIDTH 36
#define BOX_HEIGHT 36

#include <guichan/font.hpp>

#include "equipmentwindow.h"
#include "itempopup.h"
#include "popupmenu.h"

#include "../configlistener.h"
#include "../configuration.h"
#include "../equipment.h"
#include "../inventory.h"
#include "../item.h"

#include "../bindings/guichan/graphics.h"
#include "../bindings/guichan/palette.h"

#include "../bindings/guichan/widgets/button.h"
#include "../bindings/guichan/widgets/icon.h"
#include "../bindings/guichan/widgets/itemcontainer.h"
#include "../bindings/guichan/widgets/scrollarea.h"
#include "../bindings/guichan/widgets/playerbox.h"

#include "../resources/resourcemanager.h"

#include "../resources/db/iteminfo.h"

#include "../resources/sprite/localplayer.h"

#include "../utils/gettext.h"
#include "../utils/stringutils.h"

// Positions of the boxes, 2nd dimension is X and Y respectively.
static const int boxPosition[][2] = {
    {50, 208},   // EQUIP_LEGS_SLOT
    {8, 123},    // EQUIP_FIGHT1_SLOT
    {8, 78},     // EQUIP_GLOVES_SLOT
    {129, 168},  // EQUIP_RING2_SLOT
    {8, 168},    // EQUIP_RING1_SLOT
    {129, 123},  // EQUIP_FIGHT2_SLOT
    {90, 208},   // EQUIP_FEET_SLOT
    {50, 40},    // EQUIP_CAPE_SLOT
    {70, 0},     // EQUIP_HEAD_SLOT
    {90, 40},    // EQUIP_TORSO_SLOT
    {129, 78}    // EQUIP_AMMO_SLOT
};

int EquipmentWindow::mInstances = 0;
bool EquipmentWindow::mShowItemInfo = false;
ItemPopup *EquipmentWindow::mItemPopup = NULL;
PopupMenu *EquipmentWindow::mPopupMenu = NULL;
EquipmentConfigListener *EquipmentWindow::mConfigListener = NULL;

class EquipmentConfigListener : public ConfigListener
{
    public:
        EquipmentConfigListener()
        {}

        void optionChanged(const std::string &name)
        {
            bool show = config.getValue("showItemPopups", true);

            if (name == "showItemPopups")
                equipmentWindow->mShowItemInfo = show;
        }
};

EquipmentWindow::EquipmentWindow():
    Window(_("Equipment")),
    mSelected(-1),
    mEquipUnequipState(STATE_NEITHER)
{
    setWindowName("Equipment");
    setCloseButton(true);

    if (mInstances == 0)
    {
        mConfigListener = new EquipmentConfigListener();
        config.addListener("showItemPopups", mConfigListener);

        mShowItemInfo = config.getValue("showItemPopups", true);

        mItemPopup = new ItemPopup();
        mItemPopup->setOpaque(false);

        mPopupMenu = new PopupMenu(INVENTORY);
    }

    mInstances++;

    // Control that shows the Player
    mPlayerBox = new PlayerBox();
    mPlayerBox->setDimension(gcn::Rectangle(50, 80, 74, 123));
    mPlayerBox->setPlayer(player_node);

    const gcn::Rectangle &area = getChildrenArea();
    const int &padding = 2 * getPadding();

    std::string longestUseString = getFont()->getWidth(_("Equip")) >
                                   getFont()->getWidth(_("Unequip")) ?
                                   _("Equip") : _("Unequip");

    mEquipButton = new Button(longestUseString, "equipUnequip", this);
    mEquipButton->setPosition((area.width  - mEquipButton->getWidth()  - padding),
                               area.height - mEquipButton->getHeight() - padding);
    mEquipButton->setEnabled(false);

    add(mPlayerBox);

    for (int i = EQUIP_LEGS_SLOT; i < EQUIP_VECTOREND; i++)
    {
        mEquipIcon[i] = new Icon(NULL, true);
        mEquipIcon[i]->setX(boxPosition[i][0]);
        mEquipIcon[i]->setY(boxPosition[i][1]);
        mEquipIcon[i]->setWidth(BOX_WIDTH);
        mEquipIcon[i]->setHeight(BOX_HEIGHT);
        add(mEquipIcon[i]);
    }

    mEquipment = player_node->mEquipment.get();
    mInventory = player_node->getInventory();

    // Control that shows equippable items
    mItems = new ItemContainer(mInventory, "showpopupmenu", this);
    mItems->addSelectionListener(this);

    mInvenScroll = new ScrollArea(mItems);
    mInvenScroll->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    mInvenScroll->setDimension(gcn::Rectangle(175, 20, 90, 220));

    add(mInvenScroll);
    add(mEquipButton);

    setDefaultSize(280, 300, ImageRect::CENTER);
    loadWindowState();
}

EquipmentWindow::~EquipmentWindow()
{
    mInstances--;

    if (mInstances == 0)
    {
        delete mItemPopup;
        delete mPopupMenu;

        config.removeListener("showItemPopups", mConfigListener);
        delete mConfigListener;
    }
}

void EquipmentWindow::draw(gcn::Graphics *graphics)
{
    // Draw window graphics
    Window::draw(graphics);

    Item* item;

    graphics->pushClipArea(getChildrenArea());

    // A bitmask showing which slots will be affected
    // by the equip / unequip button.
    int highlightSlots = 0;
    if (mSelected != -1)
        highlightSlots = (1 << mSelected);
    Item* wouldEquip = mItems->getSelectedItem();
    if (wouldEquip)
        highlightSlots |= wouldEquip->getInfo().getEquipSlots();

    for (int i = EQUIP_LEGS_SLOT; i < EQUIP_VECTOREND; i++)
    {
        const gcn::Rectangle &rect = mEquipIcon[i]->getDimension();

        if (highlightSlots & (1 << i))
        {
            const gcn::Color color = guiPalette->getColor(Palette::HIGHLIGHT);

            // Set color to the highligh color
            graphics->setColor(gcn::Color(color.r, color.g, color.b, getGuiAlpha()));
            graphics->fillRectangle(rect);
        }

        // Set color black.
        graphics->setColor(gcn::Color(0, 0, 0));
        // Draw box border.
        graphics->drawRectangle(rect);

        item = (i != EQUIP_AMMO_SLOT) ?
               mInventory->getItem(mEquipment->getEquipment(i)) :
               mInventory->getItem(mEquipment->getArrows());
        if (item)
        {
            // Draw Item.
            Image* image = item->getImage();

            if (image != mEquipIcon[i]->getImage())
                mEquipIcon[i]->setImage(image);

            if (i == EQUIP_AMMO_SLOT)
            {
                graphics->setColor(guiPalette->getColor(Palette::TEXT));
                graphics->drawText(toString(item->getQuantity()),
                                   mEquipIcon[i]->getX() + (mEquipIcon[i]->getWidth() / 2),
                                   mEquipIcon[i]->getY() - getFont()->getHeight(),
                                   gcn::Graphics::CENTER);
            }
        }
        else if (mEquipIcon[i]->getImage())
            mEquipIcon[i]->setImage(NULL);
    }
    graphics->popClipArea();

    Window::drawChildren(graphics);
}

void EquipmentWindow::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "equipUnequip")
    {
        switch(mEquipUnequipState)
        {
            case STATE_EQUIP:
            {
                Item* item = mItems->getSelectedItem();

                if (item)
                    player_node->equipItem(item);

                break;
            }
            case STATE_UNEQUIP:
            {
                if (mSelected > -1)
                {
                    Item* item = (mSelected != EQUIP_AMMO_SLOT) ?
                                 mInventory->getItem(mEquipment->getEquipment(mSelected)) :
                                 mInventory->getItem(mEquipment->getArrows());
                    player_node->unequipItem(item);
                }
                break;
            }
            default:
                // Should be unreachable, as the button should be disabled.
                break;
        }
    }
    else if (event.getId() == "showpopupmenu")
        mItems->showPopupMenu(INVENTORY, false);
}

Item* EquipmentWindow::getItem(const int &x, const int &y)
{
    for (int i = EQUIP_LEGS_SLOT; i < EQUIP_VECTOREND; i++)
    {
        const int windowX = x - getPadding();
        const int windowY = y - getTitleBarHeight();

        if (mEquipIcon[i]->getDimension().isPointInRect(windowX, windowY))
        {
            return (i != EQUIP_AMMO_SLOT) ?
                    mInventory->getItem(mEquipment->getEquipment(i)) :
                    mInventory->getItem(mEquipment->getArrows());
        }
    }
    return NULL;
}

void EquipmentWindow::mousePressed(gcn::MouseEvent& mouseEvent)
{
    Window::mousePressed(mouseEvent);

    if (mouseEvent.getButton() == gcn::MouseEvent::LEFT)
    {
        // Checks if any of the presses were in the equip boxes.
        for (int i = EQUIP_LEGS_SLOT; i < EQUIP_VECTOREND; i++)
        {            
            if (mouseEvent.getSource() == mEquipIcon[i])
            {
                setSelected(i);
                mItems->setEquipSlotsFilter(1 << i);
                break;  // No point checking the other boxes
            }
        }

        // Clicking in the item box should deselect the slot,
        // even if it doesn't select an item.  (If it does
        // select an item, valueChanged will be called too).
        if (mouseEvent.getSource() == mItems)
            setSelected(-1);

        // Clicking on the window background will reset the
        // items window to showing all equipment.
        // (Intuitive to me, but maybe it should be a button)
        if (mouseEvent.getSource() == this)
        {
            setSelected(-1);
            mItems->setEquipSlotsFilter((1 << EQUIP_VECTOREND) - 1);
        }
    }
    else if (mouseEvent.getButton() == gcn::MouseEvent::RIGHT &&
             mouseEvent.getSource() == mItems)
    {
        mItems->showPopupMenu(INVENTORY);
    }
    else if (mouseEvent.getButton() == gcn::MouseEvent::RIGHT)
    {
        const int x = mouseEvent.getX();
        const int y = mouseEvent.getY();

        Item* item = getItem(x, y);
        if (!item)
            return;

        /* Convert relative to the window coordinates to absolute screen
         * coordinates.
         */
        const int mx = x + getX();
        const int my = y + getY();

        mPopupMenu->setItem(item);
        mPopupMenu->showPopup(mx, my);
    }
}

// Show ItemTooltip
void EquipmentWindow::mouseMoved(gcn::MouseEvent &event)
{
    if (!mShowItemInfo)
    {
        mItemPopup->setVisible(false);
        return;
    }

    const int x = event.getX();
    const int y = event.getY();

    Item* item = getItem(x, y);

    if (item)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        if (item->getInfo().getName() != mItemPopup->getItemName())
            mItemPopup->setItem(item->getInfo());

        mItemPopup->updateColors();
        mItemPopup->view(x + getX(), y + getY());
    }
    else
        mItemPopup->setVisible(false);
}

// Hide ItemTooltip
void EquipmentWindow::mouseExited(gcn::MouseEvent &event)
{
    mItemPopup->setVisible(false);
}

void EquipmentWindow::setSelected(int index)
{
    mSelected = index;

    if (index == -1 && mEquipUnequipState == STATE_UNEQUIP)  // no slot selected
        setEquipUnequipState(STATE_NEITHER);
    else
        setEquipUnequipState(STATE_UNEQUIP);
}

void EquipmentWindow::valueChanged(const gcn::SelectionEvent &event)
{
    if (event.getSource() == mItems && mItems->getSelectedItem())
    {
        setSelected(-1);
        setEquipUnequipState(STATE_EQUIP);
    }
}

void EquipmentWindow::setEquipUnequipState(EquipUnequipState state)
{
    switch (state)
    {
        case STATE_NEITHER:
            mEquipButton->setEnabled(false);
            // leave it with the previous caption
            break;
        case STATE_EQUIP:
            mEquipButton->setEnabled(true);
            mEquipButton->setCaption(_("Equip"));
            break;
        case STATE_UNEQUIP:
            mEquipButton->setEnabled(true);
            mEquipButton->setCaption(_("Unequip"));
            break;
        default:
            mEquipUnequipState = STATE_NEITHER;
            return;
    }

    mEquipUnequipState = state;
}

void EquipmentWindow::requestFocus()
{
    mItems->requestFocus();
}

void EquipmentWindow::widgetShown(const gcn::Event& event)
{
    mSelected = -1;
    mItems->selectNone();
    mItems->setEquipSlotsFilter((1 << EQUIP_VECTOREND) - 1);  // show all equippable items

    Window::widgetShown(event);
}
