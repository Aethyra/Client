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

#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <string>
#include <vector>

#include <guichan/widget.hpp>

#include <SDL_types.h>

#include "../guichanfwd.h"

class ImageRect;

/**
 * A progress bar.
 *
 * \ingroup GUI
 */
class ProgressBar : public gcn::Widget
{
    public:
        /**
         * Constructor, initializes the progress with the given value.
         */
        ProgressBar(float progress = 0.0f,
                    unsigned int width = 40, unsigned int height = 7,
                    Uint8 red = 150, Uint8 green = 150, Uint8 blue = 150);

        /**
         * Destructor.
         */
        ~ProgressBar();

        /**
         * Performs progress bar logic (fading colors)
         */
        void logic();

        /**
         * Draws the progress bar.
         */
        void draw(gcn::Graphics *graphics);

        /**
         * Adds a color to the progress bar. The progress bar will then
         * transition between the added color and all previously added colors.
         */
        void addColor(Uint8, Uint8 green, Uint8 blue);

        /**
         * Sets the current progress.
         */
        void setProgress(float progress);

        /**
         * Returns the current progress.
         */
        float getProgress() const { return mProgress; }

        /**
         * Returns the red value of color.
         */
        Uint8 getRed() const { return mRed; }

        /**
         * Returns the green value of color.
         */
        Uint8 getGreen() const { return mGreen; }

        /**
         * Returns the blue value of color.
         */
        Uint8 getBlue() const { return mBlue; }

        /**
         * Sets the text shown on the progress bar.
         */
        void setText(const std::string &text)
        { mText = text; }

        /**
         * Returns the text shown on the progress bar.
         */
        const std::string &text() const
        { return mText; }

        /**
         * Set wether the progress is moved smoothly
         */
        void setSmoothProgress(bool smoothProgress)
        { mSmoothProgress = smoothProgress; }

        /**
         * Set wether the color changing is made smoothly
         */
        void setSmoothColorChange(bool smoothColorChange)
        { mSmoothColorChange = smoothColorChange; }


    private:

        float mProgress, mProgressToGo;
        bool mSmoothProgress;

        std::vector<gcn::Color*> mColors;
        int mCurrentColor;

        Uint8 mRed, mGreen, mBlue;
        Uint8 mRedToGo, mGreenToGo, mBlueToGo;
        bool mSmoothColorChange;

        std::string mText;
        bool mUpdated;

        static ImageRect mBorder;
        static int mInstances;
        static float mAlpha;
};

#endif
