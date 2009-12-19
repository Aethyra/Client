/*
 *  Aethyra
 *  Copyright (C) 2008  The Mana World Development Team
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

#include <algorithm>

#include "beingmanager.h"
#include "playerrelations.h"

#include "../bindings/guichan/graphics.h"

#include "../core/configuration.h"

#include "../core/map/sprite/being.h"
#include "../core/map/sprite/player.h"

#include "../core/utils/dtor.h"

#define PLAYER_IGNORE_STRATEGY_NOP "nop"
#define PLAYER_IGNORE_STRATEGY_EMOTE0 "emote0"
#define DEFAULT_IGNORE_STRATEGY PLAYER_IGNORE_STRATEGY_EMOTE0

#define NAME "name" // constant for xml serialisation
#define RELATION "relation" // constant for xml serialisation

#define IGNORE_EMOTE_TIME 100

// (De)serialisation class
class PlayerConfSerialiser : public ConfigurationListManager<RelationPair, RelationMap* >
{
    virtual ConfigurationObject *writeConfigItem(RelationPair value,
                                                 ConfigurationObject *cobj)
    {
        if (!value.second)
            return NULL;

        cobj->setValue(NAME, value.first);
        cobj->setValue(RELATION, value.second->mRelation);

        return cobj;
    }

    virtual RelationMap *readConfigItem(ConfigurationObject *cobj,
                                        RelationMap *container)
    {
        std::string name = cobj->getValue(NAME, "");

        if (name.empty())
            return container;

        if (!(*container)[name])
        {
            int v = (int)cobj->getValue(RELATION, PlayerRelation::NEUTRAL);
            (*container)[name] = new PlayerRelation(static_cast<PlayerRelation::relation>(v));
        }
        // otherwise ignore the duplicate entry

        return container;
    }
};

static PlayerConfSerialiser player_conf_serialiser; // stateless singleton

const unsigned int PlayerRelation::RELATION_PERMISSIONS[RELATIONS_NR] = {
    /* NEUTRAL */    0, // we always fall back to the defaults anyway
    /* FRIEND  */    EMOTE | SPEECH_FLOAT | SPEECH_LOG | WHISPER | TRADE,
    /* DISREGARDED*/ EMOTE | SPEECH_FLOAT,
    /* IGNORED */    0
};

PlayerRelation::PlayerRelation(relation relation)
{
    mRelation = relation;
}

PlayerRelationsManager::PlayerRelationsManager() :
    mPersistIgnores(false),
    mDefaultPermissions(PlayerRelation::DEFAULT),
    mIgnoreStrategy(NULL)
{
}

PlayerRelationsManager::~PlayerRelationsManager()
{
    delete_all(mIgnoreStrategies);
}

void PlayerRelationsManager::clear()
{
    PlayerNames *names = getPlayers();
    for (PlayerNamesIterator it = names->begin(); it != names->end(); it++)
        removePlayer(*it);
    destroy(names);
}

#define PERSIST_IGNORE_LIST "persistent-player-list"
#define PLAYER_IGNORE_STRATEGY "player-ignore-strategy"
#define DEFAULT_PERMISSIONS "default-player-permissions"

int PlayerRelationsManager::getPlayerIgnoreStrategyIndex(const std::string &name)
{
    IgnoreStrategies *strategies = getPlayerIgnoreStrategies();
    for (unsigned int i = 0; i < strategies->size(); i++)
        if ((*strategies)[i]->mShortName == name)
            return i;

    return -1;
}

void PlayerRelationsManager::load()
{
    clear();

    mPersistIgnores = config.getValue(PERSIST_IGNORE_LIST, 1);
    mDefaultPermissions = config.getValue(DEFAULT_PERMISSIONS,
                                                mDefaultPermissions);

    std::string ignore_strategy_name = config.getValue(PLAYER_IGNORE_STRATEGY,
                                                       DEFAULT_IGNORE_STRATEGY);
    int ignore_strategy_index = getPlayerIgnoreStrategyIndex(ignore_strategy_name);

    if (ignore_strategy_index >= 0)
        setPlayerIgnoreStrategy((*getPlayerIgnoreStrategies())[ignore_strategy_index]);

    config.getList<RelationPair, RelationMap *>("player",  &(mRelations),
                                                &player_conf_serialiser);
}


void PlayerRelationsManager::init()
{
    load();

    if (!mPersistIgnores)
        clear(); // Yes, we still keep them around in the config file until the
                 // next update.
}

void PlayerRelationsManager::store() const
{
    config.setList<RelationMapIterator, RelationPair, RelationMap *>
        ("player", mRelations.begin(), mRelations.end(), &player_conf_serialiser);

    config.removeValue("persist-player-list");
    config.setValue(DEFAULT_PERMISSIONS, mDefaultPermissions);
    config.setValue(PERSIST_IGNORE_LIST, mPersistIgnores);
    config.setValue(PLAYER_IGNORE_STRATEGY, (mIgnoreStrategy) ?
                    mIgnoreStrategy->mShortName : DEFAULT_IGNORE_STRATEGY);

    config.write();
}

void PlayerRelationsManager::signalUpdate(const std::string &name)
{
    store();

    for (RelationListenersIterator it = mListeners.begin(); it != mListeners.end(); it++)
        (*it)->updatedPlayer(name);
}

unsigned int PlayerRelationsManager::checkPermissionSilently(const std::string &player_name, const unsigned int flags)
{
    PlayerRelation *r = mRelations[player_name];

    if (!r)
        return mDefaultPermissions & flags;
    else
    {
        unsigned int permissions = PlayerRelation::RELATION_PERMISSIONS[r->mRelation];

        switch (r->mRelation)
        {
            case PlayerRelation::NEUTRAL:
                permissions = mDefaultPermissions;
                break;

            case PlayerRelation::FRIEND:
                permissions |= mDefaultPermissions; // widen
                break;

            default:
                permissions &= mDefaultPermissions; // narrow
        }

        return permissions & flags;
    }
}

bool PlayerRelationsManager::hasPermission(Being *being, const unsigned int flags)
{
    return (being->getType() == Being::PLAYER ? 
            hasPermission(being->getName(), flags) == flags : true);
}

bool PlayerRelationsManager::hasPermission(const std::string &name, const unsigned int flags)
{
    const unsigned int rejections = flags & ~checkPermissionSilently(name, flags);
    const bool permitted = rejections == 0;

    if (!permitted)
    {
        // execute `ignore' strategy, if possible
        if (mIgnoreStrategy)
        {
            Player *to_ignore = dynamic_cast<Player *>(beingManager->findBeingByName(name, Being::PLAYER));

            if (to_ignore)
                mIgnoreStrategy->ignore(to_ignore, rejections);
        }
    }

    return permitted;
}

PlayerNames *PlayerRelationsManager::getPlayers() const
{
    PlayerNames *retval = new PlayerNames();

    for (RelationMapIterator it = mRelations.begin(); it != mRelations.end();
         it++)
    {
        if (it->second)
            retval->push_back(it->first);
    }

    sort(retval->begin(), retval->end());

    return retval;
}

PlayerRelation::relation PlayerRelationsManager::getRelation(const std::string &name)
{
    if (mRelations[name])
        return mRelations[name]->mRelation;

    return PlayerRelation::NEUTRAL;
}

void PlayerRelationsManager::setRelation(const std::string &player_name,
                                         PlayerRelation::relation relation)
{
    PlayerRelation *r = mRelations[player_name];

    if (r == NULL)
        mRelations[player_name] = new PlayerRelation(relation);
    else
        r->mRelation = relation;

    signalUpdate(player_name);
}

void PlayerRelationsManager::removePlayer(const std::string &name)
{
    if (mRelations[name])
        destroy(mRelations[name]);

    mRelations.erase(name);

    signalUpdate(name);
}

////////////////////////////////////////
// defaults

unsigned int PlayerRelationsManager::getDefault() const
{
    return mDefaultPermissions;
}

void PlayerRelationsManager::setDefault(const unsigned int permissions)
{
    mDefaultPermissions = permissions;

    store();
    signalUpdate("");
}


////////////////////////////////////////
// ignore strategies


class PIS_nothing : public PlayerIgnoreStrategy
{
public:
    PIS_nothing()
    {
        mDescription = "completely ignore";
        mShortName = PLAYER_IGNORE_STRATEGY_NOP;
    }

    virtual void ignore(Player *player, const unsigned int flags)
    {
    }
};

class PIS_dotdotdot : public PlayerIgnoreStrategy
{
public:
    PIS_dotdotdot()
    {
        mDescription = "print '...'";
        mShortName = "dotdotdot";
    }

    virtual void ignore(Player *player, const unsigned int flags)
    {
        player->setSpeech("...", 500);
    }
};


class PIS_blinkname : public PlayerIgnoreStrategy
{
public:
    PIS_blinkname()
    {
        mDescription = "blink name";
        mShortName = "blinkname";
    }

    virtual void ignore(Player *player, const unsigned int flags)
    {
        player->flash(200);
    }
};

class PIS_emote : public PlayerIgnoreStrategy
{
public:
    PIS_emote(int emote_nr, const std::string &description,
              const std::string &shortname) :
        mEmotion(emote_nr)
    {
        mDescription = description;
        mShortName = shortname;
    }

    virtual void ignore(Player *player, const unsigned int flags)
    {
        player->setEmote(mEmotion, IGNORE_EMOTE_TIME);
    }
private:
    int mEmotion;
};

static IgnoreStrategies player_ignore_strategies;

IgnoreStrategies *PlayerRelationsManager::getPlayerIgnoreStrategies()
{
    if (mIgnoreStrategies.size() == 0)
    {
        // not initialised yet?
        mIgnoreStrategies.push_back(new PIS_emote(FIRST_IGNORE_EMOTE,
                                                  "floating '...' bubble",
                                                   PLAYER_IGNORE_STRATEGY_EMOTE0));
        mIgnoreStrategies.push_back(new PIS_emote(FIRST_IGNORE_EMOTE + 1,
                                                  "floating bubble",
                                                  "emote1"));
        mIgnoreStrategies.push_back(new PIS_nothing());
        mIgnoreStrategies.push_back(new PIS_dotdotdot());
        mIgnoreStrategies.push_back(new PIS_blinkname());
    }
    return &mIgnoreStrategies;
}

PlayerRelationsManager player_relations;
