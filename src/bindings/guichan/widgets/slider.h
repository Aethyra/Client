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

#ifndef SLIDER_H
#define SLIDER_H

#include <guichan/widgets/slider.hpp>

class Image;
class ProtectedFocusListener;
class SliderConfigListener;

/**
 * Slider widget. Same as the Guichan slider but with custom look.
 *
 * \ingroup GUI
 */
class Slider : public gcn::Slider
{
    public:
        friend class SliderConfigListener;

        /**
         * Constructor with scale start equal to 0.
         */
        Slider(double scaleEnd = 1.0);

        /**
         * Constructor.
         */
        Slider(double scaleStart, double scaleEnd);

        /**
         * Destructor.
         */
        ~Slider();

        /**
         * Draws the slider.
         */
        void draw(gcn::Graphics *graphics);

        /**
         * Draws the marker.
         */
        void drawMarker(gcn::Graphics *graphics);

        // Inherited from KeyListener

        void keyPressed(gcn::KeyEvent &keyEvent);

    private:
        /**
         * Used to initialize instances.
         */
        void init();

        static int mInstances;
        static Image *hStart, *hMid, *hEnd, *hGrip;
        static Image *vStart, *vMid, *vEnd, *vGrip;
        static Image *hStartHi, *hMidHi, *hEndHi, *hGripHi;
        static Image *vStartHi, *vMidHi, *vEndHi, *vGripHi;

    protected:
        static float mAlpha;
        static SliderConfigListener *mConfigListener;
        ProtectedFocusListener *mProtFocusListener;
};

#endif
