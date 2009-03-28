/*
 *  A chat recorder
 *  Copyright (C) 2008  Lloyd Bryant <lloyd_bryant@netzero.net>
 *
 *  This file is part of Aethyra.
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

#include <physfs.h>

#include "button.h"
#include "chat.h"
#include "recorder.h"
#include "windowcontainer.h"

#include "widgets/layout.h"

#include "../utils/stringutils.h"

Recorder::Recorder(ChatWindow *chat, const std::string &title,
                   const std::string &buttonTxt) :
    Window(title)
{
    setWindowName("Recorder");
    const int offsetX = 2 * getPadding() + 10;
    const int offsetY = getTitleBarHeight() + getPadding() + 10;

    mChat = chat;
    Button *button = new Button(buttonTxt, "activate", this);

    // 123 is the default chat window height. If you change this in Chat, please
    // change it here as well
    setDefaultSize(button->getWidth() + offsetX, button->getHeight() +
                   offsetY, ImageRect::LOWER_LEFT, 0, 123);

    place(0, 0, button);

    Layout &layout = getLayout();
    layout.setRowHeight(0, Layout::AUTO_SET);

    loadWindowState();
}

Recorder::~Recorder()
{
}

void Recorder::record(const std::string &msg)
{
    if (mStream.is_open())
        mStream << msg << std::endl;
}

void Recorder::changeRecordingStatus(const std::string &msg)
{
    std::string msgCopy = msg;
    trim(msgCopy);

    if (msgCopy.empty())
    {
        if (mStream.is_open())
        {
            mStream.close();
            setVisible(false);

            /*
             * Message should go after mStream is closed so that it isn't
             * recorded.
             */
            mChat->chatLog(_("Finishing recording."), BY_SERVER);
        }
        else
        {
            mChat->chatLog(_("Not currently recording."), BY_SERVER);
        }
    }
    else if (mStream.is_open())
    {
        mChat->chatLog(_("Already recording."), BY_SERVER);
    }
    else
    {
        /*
         * Message should go before mStream is opened so that it isn't
         * recorded.
         */
        mChat->chatLog(_("Starting to record..."), BY_SERVER);
        std::string file = std::string(PHYSFS_getUserDir()) + "/.aethyra/" + msgCopy;

        mStream.open(file.c_str(), std::ios_base::trunc);

        if (mStream.is_open())
            setVisible(true);
        else
            mChat->chatLog(_("Failed to start recording."), BY_SERVER);
    }
}

void Recorder::action(const gcn::ActionEvent &event)
{
    changeRecordingStatus("");
}
