/*
 *  Support for non-overlapping floating text
 *  Copyright (C) 2008  Douglas Boffey <DougABoffey@netscape.net>
 *  Copyright (C) 2008  The Mana World Development Team
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

#ifndef TEXT_H
#define TEXT_H

#include <guichan/color.hpp>
#include <guichan/graphics.hpp>

#include "guichanfwd.h"

class TextConfigListener;
class TextManager;

class Text
{
    friend class TextManager;

    public:
        /**
         * Constructor creates a text object to display on the screen.
         */
        Text(const std::string &text, int x, int y,
             gcn::Graphics::Alignment alignment,
             const gcn::Color *color);

        /**
         * Destructor. The text is removed from the screen.
         */
        virtual ~Text();

        /**
         * Allows the originator of the text to specify the ideal coordinates.
         */
        void adviseXY(int x, int y);

        /**
         * Resize the width and height when they change in the font.
         */
        void resize();

        /**
         * Draws the text.
         */
        virtual void draw(gcn::Graphics *graphics, int xOff, int yOff);

    private:
        TextConfigListener *mConfigListener;
        gcn::Graphics::Alignment mAlignment;

        int mX;                /**< Actual x-value of left of text written. */
        int mY;                /**< Actual y-value of top of text written. */
        int mWidth;            /**< The width of the text. */
        int mHeight;           /**< The height of the text. */
        int mXOffset;          /**< The offset of mX from the desired x. */
        static int mInstances; /**< Instances of text. */
        std::string mText;     /**< The text to display. */
        const gcn::Color *mColor;     /**< The color of the text. */
};

class FlashText : public Text
{
    public:
        FlashText(const std::string &text, int x, int y,
                  gcn::Graphics::Alignment alignment,
                  const gcn::Color* color);

        /**
         * Remove the text from the screen
         */
        virtual ~FlashText() {}

        /**
         * Flash the text for so many refreshes
         */
        void flash(int time) {mTime = time; }

        /**
         * Draws the text
         */
        virtual void draw(gcn::Graphics *graphics, int xOff, int yOff);

    private:
        int mTime;             /**< Time left for flashing */
};

#endif // TEXT_H
