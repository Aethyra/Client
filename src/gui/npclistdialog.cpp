/*
 *  The Mana World
 *  Copyright (C) 2004  The Mana World Development Team
 *
 *  This file is part of The Mana World.
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

#include <sstream>

#include "listbox.h"
#include "npclistdialog.h"

#include "widgets/layout.h"

#include "../npc.h"

#include "../utils/gettext.h"

NpcListDialog::NpcListDialog():
    Window(_("NPC"))
{
    setResizable(true);

    setMinWidth(200);
    setMinHeight(150);

    setDefaultSize(0, 0, 260, 200);

    mItemList = new ListBox(this);
    scrollArea = new ScrollArea(mItemList);
    okButton = new Button(_("OK"), "ok", this);
    cancelButton = new Button(_("Cancel"), "cancel", this);

    setContentSize(260, 175);
    scrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);

    place(0, 0, scrollArea, 5).setPadding(3);
    place(3, 1, okButton);
    place(4, 1, cancelButton);

    Layout &layout = getLayout();
    layout.setRowHeight(0, Layout::AUTO_SET);

    loadWindowState();
    setLocationRelativeTo(getParent());
}

int NpcListDialog::getNumberOfElements()
{
    return mItems.size();
}

std::string NpcListDialog::getElementAt(int i)
{
    return mItems[i];
}

void NpcListDialog::parseItems(const std::string &itemString)
{
    std::istringstream iss(itemString);

    std::string tmp;
    while (getline(iss, tmp, ':'))
        mItems.push_back(tmp);
}

void NpcListDialog::reset()
{
    mItems.clear();
}

void NpcListDialog::action(const gcn::ActionEvent &event)
{
    int choice = 0;

    if (event.getId() == "ok")
    {
        // Send the selected index back to the server
        int selectedIndex = mItemList->getSelected();
        if (selectedIndex > -1)
        {
            choice = selectedIndex + 1;
        }
    }
    else if (event.getId() == "cancel")
    {
        choice = 0xff; // 0xff means cancel
    }

    if (choice)
    {
        setVisible(false);
        reset();
        current_npc->dialogChoice(choice);
        current_npc = 0;
    }
}
