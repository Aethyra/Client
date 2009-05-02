/*
 *  Extended support for activating emotes
 *  Copyright (C) 2009  Aethyra Development Team
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

#ifndef EMOTECONTAINER_H
#define EMOTECONTAINER_H

#include <list>
#include <vector>

#include <guichan/keylistener.hpp>
#include <guichan/mouselistener.hpp>
#include <guichan/widget.hpp>
#include <guichan/widgetlistener.hpp>

class AnimatedSprite;
class Image;

namespace gcn {
    class SelectionListener;
}

/**
 * An emote container. Used to show emotes in inventory and trade dialog.
 *
 * \ingroup GUI
 */
class EmoteContainer : public gcn::Widget, gcn::KeyListener, gcn::MouseListener,
                              gcn::WidgetListener
{
    public:
        /**
         * Constructor. Initializes the graphic.
         */
        EmoteContainer(const std::string &actionEventId = "",
                       gcn::ActionListener *listener = NULL);

        /**
         * Destructor.
         */
        virtual ~EmoteContainer();

        /**
         * Draws the emotes.
         */
        void draw(gcn::Graphics *graphics);

        /**
         * Called whenever the widget changes size.
         */
        void widgetResized(const gcn::Event &event);

        /**
         * Handles mouse click.
         */
        void mousePressed(gcn::MouseEvent &event);

        /**
         * Returns the selected emote.
         */
        int getSelectedEmote();

        /**
         * Sets selected emote to NULL.
         */
        void selectNone();

        /**
         * Adds a listener to the list that's notified each time a change to
         * the selection occurs.
         */
        void addSelectionListener(gcn::SelectionListener *listener)
        {
            mListeners.push_back(listener);
        }

        /**
         * Removes a listener from the list that's notified each time a change
         * to the selection occurs.
         */
        void removeSelectionListener(gcn::SelectionListener *listener)
        {
            mListeners.remove(listener);
        }

    private:
        // KeyListener
        void keyPressed(gcn::KeyEvent &event);

        /**
         * Sets the currently selected emote.  Invalid (e.g., negative) indices
         * set `no emotr'.
         */
        void setSelectedEmoteIndex(int index);

        /**
         * Find the current emote index by the most recently used emote ID
         */
        void refindSelectedEmote(void);

        /**
         * Determine and set the height of the container.
         */
        void recalculateHeight(void);

        /**
         * Sends out selection events to the list of selection listeners.
         */
        void distributeValueChangedEvent(void);

        std::vector<const AnimatedSprite*> mEmoteImg;
        Image *mSelImg;
        int mSelectedEmoteIndex;

        int mMaxEmote;

        std::list<gcn::SelectionListener*> mListeners;

        static const int gridWidth;
        static const int gridHeight;
};

#endif
