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

#ifndef GUI_NPCINTEGERDIALOG_H
#define GUI_NPCINTEGERDIALOG_H

#include <guichan/actionlistener.hpp>

#include "../bindings/guichan/widgets/window.h"

class IntTextField;

/**
 * The npc integer input dialog.
 *
 * \ingroup Interface
 */
class NpcIntegerDialog : public Window, public gcn::ActionListener
{
    public:
        /**
         * Constructor.
         *
         * @see Window::Window
         */
        NpcIntegerDialog();

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const gcn::ActionEvent &event);

        /**
         * Returns the current value.
         */
        int getValue();

        /**
         * Resets the integer input field.
         */
        void reset();

        /**
         * Prepares the NPC dialog.
         *
         * @param min The minimum value to allow
         * @param max The maximum value to allow
         */
        void setRange(const int min, const int max);

        /**
         * Requests the textfield to take focus for input.
         */
        void requestFocus();

        /**
         * Overridden close() method which cleans up the NPC Integer dialog to
         * clean up after itself on close or lost visibility.
         */
        void close();

    private:
        IntTextField *mValueField;
        gcn::Button *okButton;
        gcn::Button *cancelButton;
        gcn::Button *resetButton;
};

extern NpcIntegerDialog *npcIntegerDialog;

#endif // GUI_NPCINTEGERDIALOG_H
