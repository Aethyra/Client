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

#include <string>

#include <guichan/font.hpp>

#include "charselect.h"

#include "../db/colordb.h"

#include "../net/charserverhandler.h"
#include "../net/messageout.h"

#include "../../main.h"

#include "../../bindings/guichan/layout.h"

#include "../../bindings/guichan/dialogs/confirmdialog.h"
#include "../../bindings/guichan/dialogs/okdialog.h"

#include "../../bindings/guichan/widgets/beingbox.h"
#include "../../bindings/guichan/widgets/button.h"
#include "../../bindings/guichan/widgets/label.h"
#include "../../bindings/guichan/widgets/textfield.h"

#include "../../core/map/sprite/localplayer.h"

#include "../../core/utils/gettext.h"
#include "../../core/utils/lockedarray.h"
#include "../../core/utils/stringutils.h"

// Defined in main.cpp, used here for setting the char create dialog
extern CharServerHandler charServerHandler;

/**
 * Listener for confirming character deletion.
 */
class CharDeleteConfirm : public ConfirmDialog
{
    public:
        CharDeleteConfirm(CharSelectDialog *master);
        void action(const gcn::ActionEvent &event);
    private:
        CharSelectDialog *master;
};

CharDeleteConfirm::CharDeleteConfirm(CharSelectDialog *m):
    ConfirmDialog(_("Confirm Character Delete"),
                  _("Are you sure you want to delete this character?"), m),
    master(m)
{
}

void CharDeleteConfirm::action(const gcn::ActionEvent &event)
{
    //ConfirmDialog::action(event);
    if (event.getId() == "yes")
    {
        master->attemptCharDelete();
    }
    ConfirmDialog::action(event);
}

CharSelectDialog::CharSelectDialog(LockedArray<LocalPlayer*> *charInfo,
                                   Gender gender):
    Window(_("Select Character")),
    mCharInfo(charInfo),
    mGender(gender),
    mCharSelected(false)
{
    // Control that shows the Player
    mBeingBox = new BeingBox();
    mBeingBox->setWidth(74);

    mNameLabel = new Label(strprintf(_("Name: %s"), ""));
    mLevelLabel = new Label(strprintf(_("Level: %d"), 0));
    mJobLevelLabel = new Label(strprintf(_("Job Level: %d"), 0));
    mMoneyLabel = new Label(strprintf(_("Money: %d"), 0));

    const std::string tempString = getFont()->getWidth(_("New")) <
                                   getFont()->getWidth(_("Delete")) ?
                                   _("Delete") : _("New");

    mPreviousButton = new Button(_("Previous"), "previous", this);
    mNextButton = new Button(_("Next"), "next", this);
    mNewDelCharButton = new Button(tempString, "newdel", this);
    mSelectButton = new Button(_("OK"), "ok", this);
    mCancelButton = new Button(_("Cancel"), "cancel", this);

    ContainerPlacer place;
    place = getPlacer(0, 0);

    place(0, 0, mBeingBox, 1, 6).setPadding(3);
    place(1, 0, mNewDelCharButton);
    place(1, 1, mNameLabel, 5);
    place(1, 2, mLevelLabel, 5);
    place(1, 3, mJobLevelLabel, 5);
    place(1, 4, mMoneyLabel, 5);
    place.getCell().matchColWidth(1, 4);
    place = getPlacer(0, 2);
    place(0, 0, mPreviousButton);
    place(1, 0, mNextButton);
    place(4, 0, mCancelButton);
    place(5, 0, mSelectButton);

    reflowLayout(250, 0);

    setLocationRelativeTo(getParent());
    setVisible(true);
    mSelectButton->requestFocus();
    updatePlayerInfo();
}

void CharSelectDialog::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "ok" && n_character > 0)
    {
        // Start game
        mNewDelCharButton->setEnabled(false);
        mSelectButton->setEnabled(false);
        mPreviousButton->setEnabled(false);
        mNextButton->setEnabled(false);
        mCancelButton->setEnabled(false);
        mCharSelected = true;
        attemptCharSelect();
    }
    else if (event.getId() == "cancel")
    {
        state = EXIT_STATE;
    }
    else if (event.getId() == "newdel")
    {
        // Check for a character
        if (mCharInfo->getEntry())
        {
            new CharDeleteConfirm(this);
        }
        else if (n_character <= MAX_SLOT)
        {
            // Start new character dialog
            CharCreateDialog *charCreateDialog =
                new CharCreateDialog(this, mCharInfo->getPos(), mGender);
            charServerHandler.setCharCreateDialog(charCreateDialog);
        }
    }
    else if (event.getId() == "previous")
    {
        mCharInfo->prev();
    }
    else if (event.getId() == "next")
    {
        mCharInfo->next();
    }
}

void CharSelectDialog::updatePlayerInfo()
{
    LocalPlayer *pi = mCharInfo->getEntry();

    if (pi)
    {
        mNameLabel->setCaption(strprintf(_("Name: %s"), pi->getName().c_str()));
        mLevelLabel->setCaption(strprintf(_("Level: %d"), pi->mLevel));
        mJobLevelLabel->setCaption(strprintf(_("Job Level: %d"), pi->mJobLevel));
        mMoneyLabel->setCaption(strprintf(_("Gold: %d"), pi->mGp));
        if (!mCharSelected)
        {
            mNewDelCharButton->setCaption(_("Delete"));
            mSelectButton->setEnabled(true);
        }
    }
    else
    {
        mNameLabel->setCaption(strprintf(_("Name: %s"), ""));
        mLevelLabel->setCaption(strprintf(_("Level: %d"), 0));
        mJobLevelLabel->setCaption(strprintf(_("Job Level: %d"), 0));
        mMoneyLabel->setCaption(strprintf(_("Money: %s"), ""));
        mNewDelCharButton->setCaption(_("New"));
        mSelectButton->setEnabled(false);
    }

    mBeingBox->setBeing(pi);
}

void CharSelectDialog::attemptCharDelete()
{
    // Request character deletion
    MessageOut outMsg(0x0068);
    outMsg.writeInt32(mCharInfo->getEntry()->mCharId);
    outMsg.writeString("a@a.com", 40);
    mCharInfo->lock();
}

void CharSelectDialog::attemptCharSelect()
{
    // Request character selection
    MessageOut outMsg(0x0066);
    outMsg.writeInt8(mCharInfo->getPos());
    mCharInfo->lock();
}

void CharSelectDialog::logic()
{
    updatePlayerInfo();
}

bool CharSelectDialog::selectByName(const std::string &name)
{
    if (mCharInfo->isLocked())
        return false;

    unsigned int oldPos = mCharInfo->getPos();

    mCharInfo->select(0);
    do
    {
        LocalPlayer *player = mCharInfo->getEntry();

        if (player && player->getName() == name)
            return true;

        mCharInfo->next();
    } while (mCharInfo->getPos());

    mCharInfo->select(oldPos);

    return false;
}

CharCreateDialog::CharCreateDialog(Window *parent, int slot, Gender gender):
    Window(_("Create Character"), true, parent),
    mSlot(slot)
{
    mPlayer = new Player(0, 0, NULL);
    mPlayer->setGender(gender);

    int numberOfHairColors = ColorDB::size();

    mPlayer->setHairStyle(rand() % mPlayer->getNumOfHairstyles(),
                          rand() % numberOfHairColors);

    mNameField = new TextField("");
    mNameLabel = new Label(_("Name:"));
    mNextHairColorButton = new Button(">", "nextcolor", this);
    mPrevHairColorButton = new Button("<", "prevcolor", this);
    mHairColorLabel = new Label(_("Hair Color:"));
    mNextHairStyleButton = new Button(">", "nextstyle", this);
    mPrevHairStyleButton = new Button("<", "prevstyle", this);
    mHairStyleLabel = new Label(_("Hair Style:"));
    mCreateButton = new Button(_("Create"), "create", this);
    mCancelButton = new Button(_("Cancel"), "cancel", this);
    mBeingBox = new BeingBox(mPlayer);

    mBeingBox->setWidth(74);

    mNameField->setActionEventId("create");
    mNameField->addActionListener(this);

    ContainerPlacer place;
    place = getPlacer(0, 0);

    place(0, 0, mNameLabel, 1);
    place(1, 0, mNameField, 6);
    place(0, 1, mHairStyleLabel, 1);
    place(1, 1, mPrevHairStyleButton);
    place(2, 1, mBeingBox, 1, 8).setPadding(3);
    place(3, 1, mNextHairStyleButton);
    place(0, 2, mHairColorLabel, 1);
    place(1, 2, mPrevHairColorButton);
    place(3, 2, mNextHairColorButton);
    place.getCell().matchColWidth(0, 2);
    place = getPlacer(0, 2);
    place(4, 0, mCancelButton);
    place(5, 0, mCreateButton);

    reflowLayout(225, 0);

    setLocationRelativeTo(getParent());
    setVisible(true);
    mNameField->requestFocus();
}

CharCreateDialog::~CharCreateDialog()
{
    delete mPlayer;

    // Make sure the char server handler knows that we're gone
    charServerHandler.setCharCreateDialog(0);
}

void CharCreateDialog::action(const gcn::ActionEvent &event)
{
    const int numberOfColors = ColorDB::size();
    const int numberOfHair = mPlayer->getNumOfHairstyles();
    if (event.getId() == "create")
    {
        if (getName().length() >= 4)
        {
            // Attempt to create the character
            mCreateButton->setEnabled(false);
            attemptCharCreate();
        }
        else
        {
            new OkDialog("Error",
                    "Your name needs to be at least 4 characters.", this);
        }
    }
    else if (event.getId() == "cancel")
        scheduleDelete();
    else if (event.getId() == "nextcolor")
        mPlayer->setHairStyle(mPlayer->getHairStyle(),
                             (mPlayer->getHairColor() + 1) % numberOfColors);
    else if (event.getId() == "prevcolor")
        mPlayer->setHairStyle(mPlayer->getHairStyle(), (mPlayer->getHairColor() +
                              numberOfColors - 1) % numberOfColors);
    else if (event.getId() == "nextstyle")
        mPlayer->setHairStyle((mPlayer->getHairStyle() + 1) % numberOfHair,
                              mPlayer->getHairColor());
    else if (event.getId() == "prevstyle")
        mPlayer->setHairStyle((mPlayer->getHairStyle() + numberOfHair - 1) %
                               numberOfHair, mPlayer->getHairColor());
}

std::string CharCreateDialog::getName()
{
    std::string name = mNameField->getText();
    trim(name);
    return name;
}

void CharCreateDialog::unlock()
{
    mCreateButton->setEnabled(true);
}

void CharCreateDialog::attemptCharCreate()
{
    // Send character infos
    MessageOut outMsg(0x0067);
    outMsg.writeString(getName(), 24);
    outMsg.writeInt8(5);
    outMsg.writeInt8(5);
    outMsg.writeInt8(5);
    outMsg.writeInt8(5);
    outMsg.writeInt8(5);
    outMsg.writeInt8(5);
    outMsg.writeInt8(mSlot);
    outMsg.writeInt16(mPlayer->getHairColor());
    outMsg.writeInt16(mPlayer->getHairStyle());
}
