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

#include "maploginhandler.h"
#include "messagein.h"
#include "protocol.h"

#include "../../main.h"

#include "../../core/log.h"

#include "../../core/map/sprite/localplayer.h"

#include "../../core/utils/gettext.h"

MapLoginHandler::MapLoginHandler()
{
    static const Uint16 _messages[] = {
        SMSG_CONNECTION_PROBLEM,
        SMSG_LOGIN_SUCCESS,
        0
    };
    handledMessages = _messages;
}

void MapLoginHandler::handleMessage(MessageIn *msg)
{
    int code;
    unsigned char direction;

    switch (msg->getId())
    {
        case SMSG_CONNECTION_PROBLEM:
            code = msg->readInt8();
            logger->log("Connection problem: %i", code);

            switch (code)
            {
                case 0:
                    errorMessage = _("Authentication failed");
                    break;
                case 2:
                    errorMessage = _("This account is already logged in");
                    break;
                default:
                    errorMessage = _("Unknown connection error");
                    break;
            }
            state = ERROR_STATE;
            break;

        case SMSG_LOGIN_SUCCESS:
            msg->readInt32();   // server tick
            msg->readCoordinates(player_node->mX, player_node->mY, direction);
            msg->skip(2);      // unknown
            logger->log("Protocol: Player start position: (%d, %d), Direction: %d",
                    player_node->mX, player_node->mY, direction);
            state = GAME_STATE;
            break;
    }
}
