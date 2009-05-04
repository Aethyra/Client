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

#ifndef PLAYER_H
#define PLAYER_H

#include "being.h"

class FlashText;
class Graphics;
class Map;

/**
 * A player being. Players have their name drawn beneath them. This class also
 * implements player-specific loading of base sprite, hair sprite and equipment
 * sprites.
 */
class Player : public Being
{
    public:
        Player(int id, int job, Map *map);

        ~Player();

        /**
         * Set up mName to be the character's name
         */
        virtual void setName(const std::string &name);

        virtual void logic();

        virtual Type getType() const;

        virtual void setGender(Gender gender);

        /**
         * Whether or not this player is a GM.
         */
        bool isGM() const { return mIsGM; }

        /**
         * Triggers whether or not to show the name as a GM name.
         */
        virtual void setGM() { mIsGM = true; }

        /**
         * Sets the hair style and color for this player.
         *
         * Only for convenience in 0.0 client. When porting
         * this to the trunk remove this function and
         * call setSprite directly instead. The server should
         * provide the hair ID and coloring in the same way
         * it does for other equipment pieces.
         *
         */
        void setHairStyle(int style, int color);

        /**
         * Sets visible equipments for this player.
         */
        virtual void setSprite(int slot, int id, std::string color = "");

        /**
         * Flash the player's name
         */
        void flash(int time);

    protected:
        virtual void updateCoords();

        FlashText *mName;

        bool mIsGM;

    private:
        bool mInParty;
};

#endif
