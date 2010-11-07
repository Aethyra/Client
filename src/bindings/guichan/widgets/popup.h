/*
 *  Aethyra
 *  Copyright (C) 2009  Aethyra Development Team
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

#ifndef POPUP_H
#define POPUP_H

#include "container.h"

#include "../guichanfwd.h"

class Skin;
class SkinLoader;

/**
 * A rather reduced down version of the Window class that is particularly suited
 * for popup type functionality that doesn't need to be resized or moved around
 * by the mouse once created, but only needs to display some simple content,
 * like a static message. Popups, in general, shouldn't also need to update
 * their content once created, although this is not an explicit requirement to
 * use the popup class.
 *
 * \ingroup GUI
 */
class Popup : public Container
{
    public:
        /**
         * Constructor. Initializes the title to the given text and hooks
         * itself into the popup container.
         *
         * @param name    A human readable name for the popup. Only useful for
         *                debugging purposes.
         * @param skin    The location where the Popup's skin XML can be found.
         * @param parent  The container that will hold this Popup.
         */
        Popup(const std::string &name = "",
              const std::string &skin = "graphics/gui/gui.xml",
              gcn::Container *parent = windowContainer);

        /**
         * Destructor. Deletes all the added widgets.
         */
        ~Popup();

        /**
         * Sets the parent container to be used by this popups.
         */
        void setParentContainer(gcn::Container *container);

        /**
         * Changes the popup's skin to use the skin defined in the saved
         * configuration file.
         */
        void loadPopupConfiguration();

        /**
         * Currently only saves the skin used by the popup so that when the
         * client is reloaded, it can use the saved skin.
         */
        void savePopupConfiguration();

        /**
         * Draws the popup.
         */
        void draw(gcn::Graphics *graphics);

        /**
         * Sets the size of this popup.
         */
        void setContentSize(int width, int height);

        /**
         * Sets the location relative to the given widget.
         */
        void setLocationRelativeTo(gcn::Widget *widget);

        /**
         * Sets the minimum width of the popup.
         */
        void setMinWidth(int width);

        /**
         * Sets the minimum height of the popup.
         */
        void setMinHeight(int height);

        /**
         * Sets the maximum width of the popup.
         */
        void setMaxWidth(int width);

        /**
         * Sets the minimum height of the popup.
         */
        void setMaxHeight(int height);

        /**
         * Gets the minimum width of the popup.
         */
        int getMinWidth() const { return mMinWidth; }

        /**
         * Gets the minimum height of the popup.
         */
        int getMinHeight() const { return mMinHeight; }

        /**
         * Gets the maximum width of the popup.
         */
        int getMaxWidth() const { return mMaxWidth; }

        /**
         * Gets the minimum height of the popup.
         */
        int getMaxHeight() const { return mMaxHeight; }

        /**
         * Gets the padding of the popup. The padding is the distance between
         * the popup border and the content.
         *
         * @return The padding of the popup.
         * @see setPadding
         */
        int getPadding() const { return mPadding; }

        /**
         * Sets the padding of the popup. The padding is the distance between the
         * popup border and the content.
         *
         * @param padding The padding of the popup.
         * @see getPadding
         */
        void setPadding(int padding) { mPadding = padding; }

        /**
         * Returns the name of the popup. This is only useful for debug purposes.
         */
        const std::string& getPopupName() { return mPopupName; }

        /**
         * Sets the name of the popup. This is only useful for debug purposes.
         */
        void setPopupName(const std::string &name) { mPopupName = name; }

        /**
         * Returns the default skin path of the popup.
         */
        const std::string& getDefaultSkinPath() { return mDefaultSkinPath; }

        /**
         * Sets the default skin path of the popup.
         */
        void setDefaultSkinPath(const std::string &path) { mDefaultSkinPath = path; }

        /**
         * Allows for child popup classes to reposition themselves on resize if
         * necessary.
         */
        virtual void adaptToNewSize();

        // Inherited from BasicContainer

        virtual gcn::Rectangle getChildrenArea();

    protected:
        Skin* mSkin;                  /**< Skin in use by this popup */

    private:
        gcn::Container *mParentContainer;

        bool mHasMaxDimensions;       /**< Whether the popup has a maximum
                                           width or height */
        std::string mPopupName;       /**< Name of the popup */
        std::string mDefaultSkinPath; /**< Default skin path for this popup */
        int mMinWidth;                /**< Minimum popup width */
        int mMinHeight;               /**< Minimum popup height */
        int mMaxWidth;                /**< Maximum popup width */
        int mMaxHeight;               /**< Maximum popup height */
        int mPadding;                 /**< Holds the padding of the popup. */
};

#endif
