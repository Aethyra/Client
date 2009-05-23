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

#include <sstream>

#include <guichan/font.hpp>

#include "chat.h"
#include "inventorywindow.h"
#include "itemamount.h"
#include "trade.h"

#include "../inventory.h"
#include "../item.h"

#include "../bindings/guichan/layout.h"

#include "../bindings/guichan/widgets/button.h"
#include "../bindings/guichan/widgets/itemcontainer.h"
#include "../bindings/guichan/widgets/label.h"
#include "../bindings/guichan/widgets/scrollarea.h"
#include "../bindings/guichan/widgets/textfield.h"

#include "../net/messageout.h"
#include "../net/protocol.h"

#include "../resources/sprite/localplayer.h"

#include "../utils/gettext.h"
#include "../utils/stringutils.h"

TradeWindow::TradeWindow():
    Window(_("Trade")),
    mMyInventory(new Inventory(INVENTORY_SIZE)),
    mPartnerInventory(new Inventory(INVENTORY_SIZE))
{
    setWindowName("Trade");
    setDefaultSize(342, 209, ImageRect::CENTER);
    setResizable(true);
    setCloseButton(true);

    setMinWidth(342);
    setMinHeight(209);

    std::string longestName = getFont()->getWidth(_("OK")) >
                                   getFont()->getWidth(_("Trade")) ?
                                   _("OK") : _("Trade");

    mAddButton = new Button(_("Add"), "add", this);
    mOkButton = new Button(longestName, "ok", this);

    mMyItemContainer = new ItemContainer(mMyInventory.get(), "showpopupmine", this);
    mMyItemContainer->setWidth(160);
    mMyItemContainer->addSelectionListener(this);

    mMyScroll = new ScrollArea(mMyItemContainer);

    mPartnerItemContainer = new ItemContainer(mPartnerInventory.get(), "showpopuptheirs", this);
    mPartnerItemContainer->setWidth(160);
    mPartnerItemContainer->addSelectionListener(this);

    mPartnerScroll = new ScrollArea(mPartnerItemContainer);

    mMoneyLabel = new Label(strprintf(_("You get %d GP."), 0));
    mMoneyLabel2 = new Label(_("You give:"));
    mMoneyField = new TextField;
    mMoneyField->setWidth(50);

    place(1, 0, mMoneyLabel);
    place(0, 1, mMyScroll).setPadding(3);
    place(1, 1, mPartnerScroll).setPadding(3);
    ContainerPlacer place;
    place = getPlacer(0, 0);
    place(0, 0, mMoneyLabel2);
    place(1, 0, mMoneyField);
    place = getPlacer(0, 2);
    place(6, 0, mAddButton);
    place(7, 0, mOkButton);
    Layout &layout = getLayout();
    layout.extend(0, 2, 2, 1);
    layout.setRowHeight(1, Layout::AUTO_SET);
    layout.setRowHeight(2, 0);
    layout.setColWidth(0, Layout::AUTO_SET);
    layout.setColWidth(1, Layout::AUTO_SET);

    mOkButton->setCaption(_("OK"));

    loadWindowState();
    setVisible(false);
}

TradeWindow::~TradeWindow()
{
}

void TradeWindow::addMoney(int amount)
{
    mMoneyLabel->setCaption(strprintf(_("You get %d GP."), amount));
    mMoneyLabel->adjustSize();
}

void TradeWindow::addItem(int id, bool own, int quantity, bool equipment)
{
    if (own)
    {
        mMyItemContainer->setWidth(mMyScroll->getWidth());
        mMyInventory->addItem(id, quantity, equipment);
    }
    else
    {
        mPartnerItemContainer->setWidth(mPartnerScroll->getWidth());
        mPartnerInventory->addItem(id, quantity, equipment);
    }
}

void TradeWindow::removeItem(int id, bool own)
{
    if (own)
        mMyInventory->removeItem(id);
    else
        mPartnerInventory->removeItem(id);
}

void TradeWindow::changeQuantity(int index, bool own, int quantity)
{
    if (own)
        mMyInventory->getItem(index)->setQuantity(quantity);
    else
        mPartnerInventory->getItem(index)->setQuantity(quantity);
}

void TradeWindow::increaseQuantity(int index, bool own, int quantity)
{
    if (own)
        mMyInventory->getItem(index)->increaseQuantity(quantity);
    else
        mPartnerInventory->getItem(index)->increaseQuantity(quantity);
}

void TradeWindow::reset()
{
    mMyInventory->clear();
    mPartnerInventory->clear();
    mOkButton->setCaption(_("OK"));
    mOkButton->setActionEventId("ok");
    mOkButton->setEnabled(true);
    mOkOther = false;
    mOkMe = false;
    mMoneyLabel->setCaption(strprintf(_("You get %d GP."), 0));
    mMoneyField->setEnabled(true);
    mMoneyField->setText("");
}

void TradeWindow::receivedOk(bool own)
{
    if (own)
    {
        mOkMe = true;
        if (mOkOther)
        {
            mOkButton->setCaption(_("Trade"));
            mOkButton->setActionEventId("trade");
        }
    }
    else
    {
        mOkOther = true;
        if (mOkMe)
        {
            mOkButton->setCaption(_("Trade"));
            mOkButton->setActionEventId("trade");
        }
    }
}

void TradeWindow::tradeItem(Item *item, int quantity)
{
    // TODO: Our newer version of eAthena doesn't register this following
    //       function. Detect the actual server version, and re-enable this
    //       for that version only.
    //addItem(item->getId(), true, quantity, item->isEquipment());
    MessageOut outMsg(CMSG_TRADE_ITEM_ADD_REQUEST);
    outMsg.writeInt16(item->getInvIndex() + INVENTORY_OFFSET);
    outMsg.writeInt32(quantity);
}

void TradeWindow::valueChanged(const gcn::SelectionEvent &event)
{
    const Item *item;

    /* If an item is selected in one container, make sure no item is selected
     * in the other container.
     */
    if (event.getSource() == mMyItemContainer &&
            (item = mMyItemContainer->getSelectedItem()))
        mPartnerItemContainer->selectNone();
    else if ((item = mPartnerItemContainer->getSelectedItem()))
        mMyItemContainer->selectNone();
}

void TradeWindow::action(const gcn::ActionEvent &event)
{
    Item *item = inventoryWindow->getSelectedItem();

    if (event.getId() == "add")
    {
        if (!inventoryWindow->isVisible()) return;

        if (!item)
            return;

        if (mMyInventory->getFreeSlot() < 0)
            return;

        if (mMyInventory->contains(item))
        {
            chatWindow->chatLog(_("Failed adding item. You can not "
                                  "overlap one kind of item on the window."),
                                  BY_SERVER);
            return;
        }

        if (item->getQuantity() == 1)
        {
            tradeItem(item, 1);
        }
        else
        {
            // Choose amount of items to trade
            new ItemAmountWindow(AMOUNT_TRADE_ADD, this, item);
        }
    }
    else if (event.getId() == "cancel")
    {
        MessageOut outMsg(CMSG_TRADE_CANCEL_REQUEST);
    }
    else if (event.getId() == "ok")
    {
        std::stringstream tempMoney(mMoneyField->getText());
        int tempInt;
        if (tempMoney >> tempInt)
        {
            mMoneyField->setText(toString(tempInt));

            MessageOut outMsg(CMSG_TRADE_ITEM_ADD_REQUEST);
            outMsg.writeInt16(0);
            outMsg.writeInt32(tempInt);
        }
        else
        {
            mMoneyField->setText("");
        }
        mMoneyField->setEnabled(false);
        MessageOut outMsg(CMSG_TRADE_ADD_COMPLETE);
    }
    else if (event.getId() == "trade")
    {
        MessageOut outMsg(CMSG_TRADE_OK);
    }
    else if (event.getId() == "showpopupmine")
        mMyItemContainer->showPopup(TRADE, false);
    else if (event.getId() == "showpopuptheirs")
        mPartnerItemContainer->showPopup(TRADE, false);
}

void TradeWindow::close()
{
    MessageOut outMsg(CMSG_TRADE_CANCEL_REQUEST);
}

void TradeWindow::mouseClicked(gcn::MouseEvent &event)
{
    Window::mouseClicked(event);

    if (event.getButton() == gcn::MouseEvent::RIGHT &&
        event.getSource() == mMyItemContainer)
        mMyItemContainer->showPopup(TRADE);

    else if (event.getButton() == gcn::MouseEvent::RIGHT &&
             event.getSource() == mPartnerItemContainer)
        mPartnerItemContainer->showPopup(TRADE);
}

void TradeWindow::requestFocus()
{
    mAddButton->requestFocus();
}
