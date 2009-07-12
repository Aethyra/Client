/*
 *  Extended support for activating emotes
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

#ifndef EMOTEWINDOW_H
#define EMOTEWINDOW_H

#include <guichan/actionlistener.hpp>
#include <guichan/selectionlistener.hpp>

#include "../../bindings/guichan/widgets/window.h"

class EmoteContainer;

/**
 * Emote dialog.
 *
 * \ingroup Interface
 */
class EmoteWindow : public Window, gcn::ActionListener,
                                   gcn::SelectionListener
{
    public:
        /**
         * Constructor.
         */
        EmoteWindow();

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const gcn::ActionEvent &event);

        /**
         * Returns the selected item.
         */
        int getSelectedEmote() const;

        void valueChanged(const gcn::SelectionEvent &event);

        void mouseClicked(gcn::MouseEvent &event);

        /**
         * Focuses on the emote container on gaining focus.
         */
        void requestFocus();

        /**
         * Deselects the currently selected item.
         */
        void widgetHidden(const gcn::Event& event);

    private:
        EmoteContainer *mEmotes;

        gcn::Button *mShortcutButton, *mUseButton;
        gcn::ScrollArea *mEmoteScroll;
};

extern EmoteWindow *emoteWindow;

#endif
