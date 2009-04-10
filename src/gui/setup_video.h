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

#ifndef GUI_SETUP_VIDEO_H
#define GUI_SETUP_VIDEO_H

#include <guichan/actionlistener.hpp>
#include <guichan/keylistener.hpp>

#include "../bindings/guichan/widgets/setuptabcontainer.h"

class ModeListModel;

class Setup_Video : public SetupTabContainer, public gcn::ActionListener,
                    public gcn::KeyListener
{
    public:
        Setup_Video();

        void apply();
        void cancel();
        void logic();

        void action(const gcn::ActionEvent &event);

        /** Called when key is pressed */
        void keyPressed(gcn::KeyEvent &event);

    private:
        void updateSliders(bool originalValues);

        int updateSlider(gcn::Slider *slider, gcn::TextField *field,
                         const std::string &configName);

        bool mFullScreenEnabled;
        bool mOpenGLEnabled;
        bool mCustomCursorEnabled;
        bool mParticleEffectsEnabled;
        bool mNameEnabled;
        bool mPickupChatEnabled;
        bool mPickupParticleEnabled;
        double mOpacity;
        int mFps;
        int mSpeechMode;

        ModeListModel *mModeListModel;

        gcn::Label *speechLabel;
        gcn::Label *alphaLabel;
        gcn::Label *scrollRadiusLabel;
        gcn::Label *scrollLazinessLabel;
        gcn::Label *overlayDetailLabel;
        gcn::Label *particleDetailLabel;

        gcn::ListBox *mModeList;
        gcn::CheckBox *mFsCheckBox;
        gcn::CheckBox *mOpenGLCheckBox;
        gcn::CheckBox *mCustomCursorCheckBox;
        gcn::CheckBox *mParticleEffectsCheckBox;
        gcn::CheckBox *mNameCheckBox;

        gcn::Slider *mSpeechSlider;
        gcn::Label *mSpeechLabel;
        gcn::Slider *mAlphaSlider;
        gcn::CheckBox *mFpsCheckBox;
        gcn::Slider *mFpsSlider;
        gcn::TextField *mFpsField;
        gcn::Label *mFpsLabel;

        int mOriginalScrollLaziness;
        gcn::Slider *mScrollLazinessSlider;
        gcn::TextField *mScrollLazinessField;

        int mOriginalScrollRadius;
        gcn::Slider *mScrollRadiusSlider;
        gcn::TextField *mScrollRadiusField;

        int mOverlayDetail;
        gcn::Slider *mOverlayDetailSlider;
        gcn::Label *mOverlayDetailField;

        int mParticleDetail;
        gcn::Slider *mParticleDetailSlider;
        gcn::Label *mParticleDetailField;

        gcn::Label *mPickupNotifyLabel;
        gcn::CheckBox *mPickupChatCheckBox;
        gcn::CheckBox *mPickupParticleCheckBox;
};

#endif
