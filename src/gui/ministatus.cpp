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

#include "ministatus.h"

#include "../localplayer.h"

#include "../bindings/guichan/gui.h"
#include "../bindings/guichan/graphics.h"

#include "../bindings/guichan/widgets/progressbar.h"

#include "../utils/stringutils.h"

MiniStatusWindow::MiniStatusWindow():
    Popup("MiniStatus")
{
    setVisible(true);

    mHpBar = new ProgressBar(1.0f, 100, 20, 0, 171, 34);
    mMpBar = new ProgressBar(1.0f, 100, 20, 26, 102, 230);
    mXpBar = new ProgressBar(1.0f, 100, 20, 143, 192, 211);

    mHpBar->setPosition(0, 3);
    mMpBar->setPosition(mHpBar->getWidth() + 3, 3);
    mXpBar->setPosition(mMpBar->getX() + mMpBar->getWidth() + 3, 3);

    add(mHpBar);
    add(mMpBar);
    add(mXpBar);

    setContentSize(mXpBar->getX() + mXpBar->getWidth(),
                   mXpBar->getY() + mXpBar->getHeight());
}

void MiniStatusWindow::update()
{
    // HP Bar coloration
    if (player_node->mHp < int(player_node->mMaxHp / 3))
        mHpBar->setColor(223, 32, 32); // Red
    else if (player_node->mHp < int((player_node->mMaxHp / 3) * 2))
        mHpBar->setColor(230, 171, 34); // Orange
    else
        mHpBar->setColor(0, 171, 34); // Green

    float xp = (float) player_node->getXp() / player_node->mXpForNextLevel;

    if (xp != xp) xp = 0.0f; // check for NaN
    if (xp < 0.0f) xp = 0.0f; // make sure the experience isn't negative (uninitialized pointer most likely)
    if (xp > 1.0f) xp = 1.0f;

    mHpBar->setProgress((float) player_node->mHp / player_node->mMaxHp);
    mMpBar->setProgress((float) player_node->mMp / player_node->mMaxMp);
    mXpBar->setProgress(xp);

    // Update labels
    mHpBar->setText(toString(player_node->mHp));
    mMpBar->setText(toString(player_node->mMp));

    std::string updatedText = strprintf("%2.2f", 100 * xp) + "%";

    // Displays the number of monsters to next lvl
    // (disabled for now but interesting idea)
    /*
    if (config.getValue("xpBarMonsterCounterExp", 0)!=0)
    {
        updatedText << " | "
            << (int)(((float)player_node->mXpForNextLevel - (float)player_node->mXp)
             / (float)config.getValue("xpBarMonsterCounterExp", 0))
            << " "
            << config.getValue("xpBarMonsterCounterName", "Monsters") <<" left...";
    }
    */

    mXpBar->setText(updatedText);
}

void MiniStatusWindow::draw(gcn::Graphics *graphics)
{
    update();
    drawChildren(graphics);
}
