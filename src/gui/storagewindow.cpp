/*
 *  The Mana World
 *  Copyright (C) 2009  The Mana World Development Team
 *
 *  This file is part of The Mana World.
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

#include <string>

#include <guichan/font.hpp>
#include <guichan/mouseinput.hpp>

#include "inventorywindow.h"
#include "itemamount.h"
#include "storagewindow.h"
#include "viewport.h"

#include "../inventory.h"
#include "../item.h"

#include "../bindings/guichan/layout.h"

#include "../bindings/guichan/widgets/button.h"
#include "../bindings/guichan/widgets/itemcontainer.h"
#include "../bindings/guichan/widgets/label.h"
#include "../bindings/guichan/widgets/progressbar.h"
#include "../bindings/guichan/widgets/scrollarea.h"

#include "../net/messageout.h"
#include "../net/network.h"
#include "../net/protocol.h"

#include "../resources/db/iteminfo.h"

#include "../resources/sprite/localplayer.h"

#include "../utils/gettext.h"
#include "../utils/stringutils.h"

StorageWindow::StorageWindow(int invSize):
    Window(_("Storage")),
    mMaxSlots(invSize),
    mItemDesc(false)
{
    setWindowName("Storage");
    setResizable(true);
    setCloseButton(true);

    // If you adjust these defaults, don't forget to adjust the trade window's.
    setDefaultSize(375, 300, ImageRect::CENTER);

    mStoreButton = new Button(_("Store"), "store", this);
    mRetrieveButton = new Button(_("Retrieve"), "retrieve", this);

    mItems = new ItemContainer(player_node->getStorage());
    mItems->addSelectionListener(this);

    mInvenScroll = new ScrollArea(mItems);
    mInvenScroll->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);

    mUsedSlots = player_node->getStorage()->getNumberOfSlotsUsed();

    mSlotsLabel = new Label(_("Slots: "));

    mSlotsBar = new ProgressBar(1.0f, 100, 20, gcn::Color(225, 200, 25));
    mSlotsBar->setText(strprintf("%d/%d", mUsedSlots, mMaxSlots));
    mSlotsBar->setProgress((float) mUsedSlots / mMaxSlots);

    setMinHeight(130);
    setMinWidth(200);

    place(0, 0, mSlotsLabel).setPadding(3);
    place(1, 0, mSlotsBar, 3);
    place(0, 1, mInvenScroll, 4, 4);
    place(2, 5, mStoreButton);
    place(3, 5, mRetrieveButton);

    Layout &layout = getLayout();
    layout.setRowHeight(0, mStoreButton->getHeight());

    loadWindowState();
    setVisible(false);
}

StorageWindow::~StorageWindow()
{
    delete mItems;
}

void StorageWindow::logic()
{
    if (!isVisible())
        return;

    Window::logic();

    const int usedSlots = player_node->getStorage()->getNumberOfSlotsUsed();

    if (mUsedSlots != usedSlots)
    {
        mUsedSlots = usedSlots;

        mSlotsBar->setProgress((float) mUsedSlots / mMaxSlots);

        mSlotsBar->setText(strprintf("%d/%d", mUsedSlots, mMaxSlots));
    }
}

void StorageWindow::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "store")
    {
        if (!inventoryWindow->isVisible()) return;

        Item *item = inventoryWindow->getSelectedItem();

        if (!item)
            return;

        if (item->getQuantity() == 1)
        {
            addStore(item, 1);
        }
        else
        {
            // Choose amount of items to trade
            new ItemAmountWindow(AMOUNT_STORE_ADD, this, item);
        }
    }
    else if (event.getId() == "retrieve")
    {
        Item *item = mItems->getSelectedItem();

        if (!item)
            return;

        if (item->getQuantity() == 1)
        {
            removeStore(item, 1);
        }
        else
        {
            // Choose amount of items to trade
            new ItemAmountWindow(AMOUNT_STORE_REMOVE, this, item);
        }
    }
}

void StorageWindow::mouseClicked(gcn::MouseEvent &event)
{
    Window::mouseClicked(event);

    if (event.getButton() == gcn::MouseEvent::RIGHT)
    {
        Item *item = mItems->getSelectedItem();

        if (!item)
        {
            mRetrieveButton->setEnabled(false);
            return;
        }

        mRetrieveButton->setEnabled(true);

        /* Convert relative to the window coordinates to absolute screen
         * coordinates.
         */
        const int mx = event.getX() + getX();
        const int my = event.getY() + getY();
        viewport->showPopup(mx, my, item);
    }
}

Item* StorageWindow::getSelectedItem() const
{
    return mItems->getSelectedItem();
}

void StorageWindow::addStore(Item *item, int ammount)
{
    MessageOut outMsg(CMSG_MOVE_TO_STORAGE);
    outMsg.writeInt16(item->getInvIndex() + INVENTORY_OFFSET);
    outMsg.writeInt32(ammount);
}

void StorageWindow::removeStore(Item *item, int ammount)
{
    MessageOut outMsg(CSMG_MOVE_FROM_STORAGE);
    outMsg.writeInt16(item->getInvIndex() + STORAGE_OFFSET);
    outMsg.writeInt32(ammount);
}

void StorageWindow::close()
{
    MessageOut outMsg(CMSG_CLOSE_STORAGE);
}
