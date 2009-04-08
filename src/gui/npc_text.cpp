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

#include "npc_text.h"

#include "../npc.h"

#include "../bindings/guichan/layout.h"

#include "../bindings/guichan/widgets/button.h"
#include "../bindings/guichan/widgets/scrollarea.h"
#include "../bindings/guichan/widgets/textbox.h"

#include "../net/messageout.h"
#include "../net/protocol.h"

#include "../utils/gettext.h"

NpcTextDialog::NpcTextDialog():
     Window(_("NPC"))
{
    setWindowName("NPC");
    setResizable(true);

    setMinWidth(200);
    setMinHeight(150);

    setDefaultSize(260, 200, ImageRect::CENTER);

    mTextBox = new TextBox;
    mTextBox->setEditable(false);
    mTextBox->setOpaque(false);

    mScrollArea = new ScrollArea(mTextBox);
    mButton = new Button(_("OK"), "", this);
    mButton->setActionEventId("ok");

    mScrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    mScrollArea->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_ALWAYS);

    place(0, 0, mScrollArea, 5).setPadding(3);
    place(4, 1, mButton);

    Layout &layout = getLayout();
    layout.setRowHeight(0, Layout::AUTO_SET);

    loadWindowState();
}

void NpcTextDialog::setText(const std::string &text)
{
    mText = text;
    mTextBox->setTextWrapped(mText, mScrollArea->getWidth() - 15);
}

void NpcTextDialog::addText(const std::string &text)
{
    setText(mText + text + "\n");
}

void NpcTextDialog::clearText()
{
    NPC::mTalking = false;
    setText("");
}

void NpcTextDialog::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "ok")
    {
        clearText();
        setVisible(false);
        saveWindowState();

        if (current_npc)
            nextDialog();

        current_npc = 0;
    }
}

void NpcTextDialog::nextDialog(int npcID)
{
    MessageOut outMsg(CMSG_NPC_NEXT_REQUEST);
    outMsg.writeInt32(npcID);
}

void NpcTextDialog::closeDialog(int npcID)
{
    MessageOut outMsg(CMSG_NPC_CLOSE);
    outMsg.writeInt32(npcID);
}

void NpcTextDialog::widgetResized(const gcn::Event &event)
{
    Window::widgetResized(event);

    setText(mText);
}

void NpcTextDialog::requestFocus()
{
    loadWindowState();
    setVisible(true);
}

