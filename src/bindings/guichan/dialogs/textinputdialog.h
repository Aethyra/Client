/*
 *  Aethyra
 *  Copyright (C) 2009  Aethyra Development Team
 *
 *  This file is part of Aethyra.
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

#ifndef TEXT_INPUT_DIALOG_H
#define TEXT_INPUT_DIALOG_H

#include <guichan/actionlistener.hpp>

#include "../widgets/window.h"

/**
 * The npc integer input dialog.
 *
 * \ingroup Interface
 */
class TextInputDialog : public Window, public gcn::ActionListener
{
    public:
        /**
         * Constructor.
         *
         * @see Window::Window
         */
        TextInputDialog(std::string caption = "");

        /**
         * Called when receiving actions from the widgets.
         */
        virtual void action(const gcn::ActionEvent &event);

        /**
         * Returns the current value.
         */
        std::string getValue();

        /**
         * Resets the textfield for input.
         */
        void reset();

        /**
         * Chnages the current value.
         *
         * @param value The new value
         */
        void setValue(const std::string &value);

        /**
         * Requests the textfield to take focus for input.
         */
        void requestFocus();

    private:
        gcn::TextField *mValueField;
        gcn::Button *mOkButton;
        gcn::Button *mCancelButton;
};

#endif
