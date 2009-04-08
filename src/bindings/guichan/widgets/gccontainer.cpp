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

#include "gccontainer.h"

GCContainer::~GCContainer()
{
    WidgetIterator i = mDeathList.begin();

    while (i != mDeathList.end()) {
        /* Take care _not_ to modify the list in our _announceDeath method */
        gcn::Widget *w = (*i);
        i = mDeathList.erase(i);
        delete w;
    }

    mDeathList.clear();
}

void GCContainer::add(gcn::Widget *w)
{
    mDeathList.push_back(w);
    Container::add(w);
}

void GCContainer::add(gcn::Widget *w, int x, int y)
{
    mDeathList.push_back(w);
    Container::add(w, x, y);
}

void GCContainer::death(const gcn::Event &event)
{
    mDeathList.remove(event.getSource());
    Container::death(event);
}
