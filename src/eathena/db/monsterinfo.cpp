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

#include "monsterinfo.h"

#include "../../core/utils/dtor.h"

MonsterInfo::MonsterInfo()
{
}

MonsterInfo::~MonsterInfo()
{
    // kill vectors in mSoundEffects
    delete_all(mSounds);
    mSounds.clear();
}

void MonsterInfo::addSound(const MonsterSoundEvent &event,
                           const std::string &filename)
{
    if (mSounds.find(event) == mSounds.end())
        mSounds[event] = new std::vector<std::string>;

    mSounds[event]->push_back("sfx/" + filename);
}

std::string MonsterInfo::getSound(const MonsterSoundEvent &event) const
{
    std::map<MonsterSoundEvent, std::vector<std::string>* >::const_iterator i;

    i = mSounds.find(event);

    return (i == mSounds.end() ? "" :
                                 i->second->at(rand() % i->second->size()));
}

void MonsterInfo::addParticleEffect(const std::string &filename)
{
    mParticleEffects.push_back(filename);
}
