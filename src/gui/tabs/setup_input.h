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

#ifndef GUI_SETUP_INPUT_H
#define GUI_SETUP_INPUT_H

#include <string>

#include <guichan/actionlistener.hpp>

#include "setuptabcontainer.h"

#include "../../bindings/guichan/guichanfwd.h"

class Setup_Input : public SetupTabContainer, public gcn::ActionListener
{
    public:
        /**
         * Constructor
         */
        Setup_Input();

        /**
         * Destructor
         */
        ~Setup_Input();

        void apply();
        void cancel();

        void action(const gcn::ActionEvent &event);

        /**
         * Get an update on the assigned key.
         */
        void refreshAssignedKey(int index);

        /**
         * The callback function when a new key has been pressed.
         */
        void newKeyCallback(int index);

        /**
         * Shorthand method to update all the keys.
         */
        void refreshKeys();

        /**
         * If a key function is unresolved, then this reverts it.
         */
        void keyUnresolved();

    private:
        class KeyListModel *mKeyListModel;

        gcn::Label *mCalibrateLabel;
        gcn::CheckBox *mJoystickCheckbox;

        gcn::ListBox *mKeyList;

        gcn::Button *mCalibrateButton;
        gcn::Button *mAssignKeyButton;
        gcn::Button *mMakeDefaultButton;

        bool mKeySetting; /**< flag to check if key being set. */
};

#endif
