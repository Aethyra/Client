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

#ifndef BUYSELL_H
#define BUYSELL_H

#include <guichan/actionlistener.hpp>

#include "window.h"

/**
 * A dialog to choose between buying or selling at a shop.
 *
 * \ingroup Interface
 */
class BuySellDialog : public Window, public gcn::ActionListener
{
    public:
        /**
         * Constructor. The action listener passed will receive "sell", "buy"
         * or "cancel" events when the respective buttons are pressed.
         *
         * @see Window::Window
         */
        BuySellDialog();

        /**
<<<<<<< HEAD:src/gui/buysell.h
=======
         * Check for current NPC
         */
        void logic();

        void setVisible(bool visible);

        /**
>>>>>>> f64903f... Fix up the NPC interraction widnows a bit:src/gui/buysell.h
         * Called when receiving actions from the widgets.
         */
        void action(const gcn::ActionEvent &event);
};

extern BuySellDialog *buySellDialog;

#endif
