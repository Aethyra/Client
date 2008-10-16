/*
 *  Aethyra
 *  Copyright 2008 Aethyra Development Team
 *
 *  This file is part of Aethyra.
 *
 *  Aethyra is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  Aethyra is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Aethyra; if not, write to the Free Software Foundation, 
 *  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _AETHYRA_COLOR_MANAGER_H
#define _AETHYRA_COLOR_MANAGER_H

#include <string>
#include <map>

/**
 * The class that holds the color information.
 */
namespace ColorDB
{
    /**
     * Loads the color data from <code>colors.xml</code>.
     */
    void load();

    /**
     * Clear the color data
     */
    void unload();

    std::string& get(int id);

    const int& size();

    // Color DB
    typedef std::map<int, std::string> Colors;
    typedef Colors::iterator ColorIterator;
};

#endif