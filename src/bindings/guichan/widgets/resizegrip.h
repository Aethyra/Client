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

#ifndef RESIZEGRIP_H
#define RESIZEGRIP_H

#include <guichan/widget.hpp>

class Image;
class ResizeGripConfigListener;

/**
 * Resize grip. The resize grip is part of a resizable Window. It relies on the
 * fact that uncaught mouse events are automatically routed to the parent
 * window.
 *
 * TODO: Make this widget load its skin through the Skin class, since which
 *       resize grip to use should be a skin property.
 *
 * \ingroup GUI
 */
class ResizeGrip : public gcn::Widget
{
    public:
        friend class ResizeGripConfigListener;

        /**
         * Constructor.
         */
        ResizeGrip();

        /**
         * Destructor.
         */
        ~ResizeGrip();

        /**
         * Draws the resize grip.
         */
        void draw(gcn::Graphics *graphics);

    private:
        static Image *mGripImage;   /**< Resize grip image */
        static int mInstances;     /**< Number of resize grip instances */

    protected:
        static float mAlpha;
        static ResizeGripConfigListener *mConfigListener;
};

#endif
