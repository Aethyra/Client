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

#ifndef HELP_H
#define HELP_H

#include <guichan/actionlistener.hpp>

#include "../bindings/guichan/handlers/linkhandler.h"
#include "../bindings/guichan/widgets/window.h"

class BrowserBox;

/**
 * The help dialog.
 */
class HelpWindow : public Window, public LinkHandler,
                   public gcn::ActionListener
{
    public:
        /**
         * Constructor.
         */
        HelpWindow();

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const gcn::ActionEvent &event);

        /**
         * Handles link action.
         */
        void handleLink(const std::string& link);

        /**
         * Loads help in the dialog.
         */
        void loadHelp(const std::string &helpFile);

        /**
         * Focuses on the close button on gaining focus.
         */
        void requestFocus();

        /**
         * Loads help index file on hiding.
         */
        void widgetHidden(const gcn::Event& event);

    private:
        void loadFile(const std::string &file);

        BrowserBox* mBrowserBox;
        gcn::ScrollArea *mScrollArea;
        gcn::Button *mOkButton;
};

extern HelpWindow *helpWindow;

#endif
