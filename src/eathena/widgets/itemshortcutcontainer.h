/*
 *  Aethyra
 *  Copyright (C) 2007  The Mana World Development Team
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

#ifndef ITEMSHORTCUTCONTAINER_H
#define ITEMSHORTCUTCONTAINER_H

#include "../../bindings/guichan/widgets/shortcutcontainer.h"

class Image;
class Item;
class ItemShortcutContainerConfigListener;
class PopupMenu;

/**
 * An item shortcut container. Used to quickly use items.
 *
 * \ingroup GUI
 */
class ItemShortcutContainer : public ShortcutContainer
{
    public:
        friend class ItemShortcutContainerConfigListener;

        /**
         * Constructor. Initializes the graphic.
         */
        ItemShortcutContainer();

        /**
         * Destructor.
         */
        virtual ~ItemShortcutContainer();

        /**
         * Draws the items.
         */
        void draw(gcn::Graphics *graphics);

        /**
         * Handles mouse when pressed.
         */
        void mousePressed(gcn::MouseEvent &event);

    private:
        static ItemShortcutContainerConfigListener *mConfigListener;
        static int mInstances;

        static PopupMenu *mPopupMenu;
};

#endif
