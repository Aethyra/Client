/*
 *  The Mana World
 *  Copyright 2004 The Mana World Development Team
 *
 *  This file is part of The Mana World.
 *
 *  The Mana World is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  The Mana World is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with The Mana World; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  $Id$
 */

#ifndef _TMW_BUDDYLIST_H
#define _TMW_BUDDYLIST_H

#include <guichan.hpp>
#include <list>
#include <string>

class BuddyList : public gcn::ListModel {
    public:
        /**
         * Constructor
         */
        BuddyList();

        /**
         * Destructor
         */
        virtual ~BuddyList();

        /**
         * Adds buddy to the list
         */
        bool addBuddy(const std::string buddy);

        /**
         * Removes buddy from the list
         */
        bool removeBuddy(const std::string buddy);

        /**
         * Returns the number of buddy on the list
         */
        int  getNumberOfElements(void);

        /**
         * Returns the buddy of the number or null
         */
        std::string getElementAt(int number);

    private:
        /**
         * Save buddy to file
         */
        void saveFile(void);

        /**
         * Load buddy from file
         */
        void loadFile(void);

        std::list<std::string> buddylist;               /**< Buddy list */
        std::list<std::string>::iterator buddyit;       /**< Iterator */
        std::string *filename;                          /* File to work with */
};

#endif /* _TMW_BUDDYLIST_H */
