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

#ifndef ITEM_AMOUNT_WINDOW_H
#define ITEM_AMOUNT_WINDOW_H

#include <guichan/actionlistener.hpp>

#include "../../bindings/guichan/widgets/window.h"

class Icon;
class IntTextField;
class Item;
class ItemPopup;

#define AMOUNT_TRADE_ADD 1
#define AMOUNT_ITEM_DROP 2
#define AMOUNT_STORE_ADD 3
#define AMOUNT_STORE_REMOVE 4

/**
 * Window used for selecting the amount of items to drop or trade.
 *
 * \ingroup Interface
 */
class ItemAmountWindow : public Window, public gcn::ActionListener
{
    public:
        /**
         * Constructor.
         */
        ItemAmountWindow(int usage, Window *parent, Item *item);

        ~ItemAmountWindow();

        /**
         * Called when receiving actions from widget.
         */
        void action(const gcn::ActionEvent &event);

        /**
         * Sets default amount value.
         */
        void resetAmount();

        // MouseListener
        void mouseMoved(gcn::MouseEvent &event);

        /**
         * Schedules the Item Amount window for deletion.
         */
        void close();

        void fontChanged();
    private:
        gcn::Label *mItemAmountLabel;   /**< Item amount caption. */

        gcn::Button *mOkButton;
        gcn::Button *mCancelButton;
        gcn::Button *mAddAllButton;

        Item *mItem;
        Icon *mItemIcon;

        int mMax, mUsage;
        ItemPopup *mItemPopup;

        /**
         * Item Amount buttons.
         */
        gcn::Slider *mItemAmountSlide;
};

#endif /* ITEM_AMOUNT_WINDOW_H */
