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

#ifndef SCROLLAREA_H
#define SCROLLAREA_H

#include <guichan/widgets/scrollarea.hpp>

class Image;
class ImageRect;
class ScrollAreaConfigListener;

/**
 * A scroll area.
 *
 * \ingroup GUI
 */
class ScrollArea : public gcn::ScrollArea
{
    public:
        friend class ScrollAreaConfigListener;

        /**
         * Constructor that takes no content. Needed for use with the DropDown
         * class.
         */
        ScrollArea(bool gc = true, bool opaque = true);

        /**
         * Constructor.
         *
         * @param content the initial content to show in the scroll area
         */
        ScrollArea(gcn::Widget *content, bool gc = true, bool opaque = true);

        /**
         * Destructor. Also deletes the content.
         */
        ~ScrollArea();

        /**
         * Logic function optionally adapts width or height of contents. This
         * depends on the scrollbar settings.
         */
        void logic();

        /**
         * Draws the scroll area.
         */
        void draw(gcn::Graphics *graphics);

        /**
         * Draws the background and border of the scroll area.
         */
        void drawFrame(gcn::Graphics *graphics);

        /**
         * Sets whether the widget should draw its background or not.
         */
        void setOpaque(bool opaque);

        /**
         * Returns whether the widget draws its background or not.
         */
        bool isOpaque() const { return mOpaque; }

        /**
         * Scrolls the scroll area by the scroll amount each call, based on
         * which button is being held.
         */
        virtual void scroll();

        // Inherited from MouseListener

        virtual void mousePressed(gcn::MouseEvent& mouseEvent);

    protected:
        enum BUTTON_DIR {
            UP,
            DOWN,
            LEFT,
            RIGHT
        };

        /**
         * Initializes the scroll area.
         */
        void init();

        void drawButton(gcn::Graphics *graphics, BUTTON_DIR dir);
        void drawUpButton(gcn::Graphics *graphics);
        void drawDownButton(gcn::Graphics *graphics);
        void drawLeftButton(gcn::Graphics *graphics);
        void drawRightButton(gcn::Graphics *graphics);
        void drawVBar(gcn::Graphics *graphics);
        void drawHBar(gcn::Graphics *graphics);
        void drawVMarker(gcn::Graphics *graphics);
        void drawHMarker(gcn::Graphics *graphics);

        static float mAlpha;
        static ScrollAreaConfigListener *mConfigListener;

        static int instances;
        static ImageRect background;
        static ImageRect vMarker;
        static Image *buttons[4][2];

        bool mOpaque;
        bool mGC;

        int mLastUpdate;
};

#endif
