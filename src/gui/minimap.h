/*
 *  Aethyra
 *  Copyright (C) 2004-2005  The Mana World Development Team
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

#ifndef MINIMAP_H
#define MINIMAP_H

#include "../bindings/guichan/widgets/window.h"

class Image;

/**
 * Minimap dialog.
 *
 * \ingroup Interface
 */
class Minimap : public Window
{
    public:
        /**
         * Constructor.
         */
        Minimap();

        /**
         * Destructor.
         */
        ~Minimap();

        /**
         * Sets the map image that should be displayed.
         */
        void setMapImage(Image *img);

        /**
         * Sets the map proportion (1 means 1 tile to one pixel, .5 means 2 tiles to 1 pixel, etc.)
         */
        void setProportion(float proportion) { mProportion = proportion; }

        /**
         * Toggles the displaying of the minimap.
         */
        void toggle();

        /**
         * Hides/unhides the minimap
         */
        void hide();

        /**
         * Draws the minimap.
         */
        void draw(gcn::Graphics *graphics);

    private:
        Image *mMapImage;
        float mProportion;
        static bool mShow;
};

extern Minimap *minimap;

#endif
