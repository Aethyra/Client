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

#include <guichan/exception.hpp>

#include "resizegrip.h"
#include "window.h"
#include "windowcontainer.h"

#include "../gui.h"
#include "../layout.h"
#include "../palette.h"
#include "../skin.h"

#include "../../../configuration.h"
#include "../../../log.h"

#include "../../../resources/image.h"

int Window::instances = 0;
int Window::mouseResize = 0;

Window::Window(const std::string& caption, bool modal, Window *parent, const std::string& skin):
    gcn::Window(caption),
    mOldVisibility(false),
    mGrip(0),
    mParent(parent),
    mLayout(NULL),
    mWindowName("window"),
    mDefaultSkinPath(skin),
    mShowTitle(true),
    mModal(modal),
    mCloseButton(false),
    mMinWinWidth(100),
    mMinWinHeight(40),
    mMaxWinWidth(graphics->getWidth()),
    mMaxWinHeight(graphics->getHeight())
{
    logger->log("Window::Window(\"%s\")", caption.c_str());

    if (!windowContainer)
        throw GCN_EXCEPTION("Window::Window(): no windowContainer set");

    if (!skinLoader)
        skinLoader = new SkinLoader();

    instances++;

    setFrameSize(0);
    setPadding(3);
    setTitleBarHeight(20);

    // Loads the skin
    mSkin = skinLoader->load(skin, mDefaultSkinPath);

    // Add this window to the window container
    windowContainer->add(this);

    if (mModal)
    {
        gui->setCursorType(Gui::CURSOR_POINTER);
        requestModalFocus();
    }

    // Windows are invisible by default
    setVisible(false);

    addWidgetListener(this);
}

Window::~Window()
{
    logger->log("Window::~Window(\"%s\")", getCaption().c_str());

    saveWindowState();

    delete mLayout;

    while (!mWidgets.empty())
        delete mWidgets.front();

    removeWidgetListener(this);

    instances--;

    mSkin->instances--;

    if (instances == 0)
        delete skinLoader;
}

void Window::setWindowContainer(WindowContainer *wc)
{
    windowContainer = wc;
}

void Window::draw(gcn::Graphics *graphics)
{
    Graphics *g = static_cast<Graphics*>(graphics);

    g->drawImageRect(0, 0, getWidth(), getHeight(), mSkin->getBorder());

    // Draw title
    if (mShowTitle)
    {
        g->setColor(guiPalette->getColor(Palette::TEXT));
        g->setFont(getFont());
        g->drawText(getCaption(), 7, 5, gcn::Graphics::LEFT);
    }

    // Draw Close Button
    if (mCloseButton)
    {
        g->drawImage(mSkin->getCloseImage(), getWidth() -
                     mSkin->getCloseImage()->getWidth() - getPadding(),
                     getPadding());
    }

    drawChildren(graphics);
}

void Window::setContentSize(int width, int height)
{
    width = width + 2 * getPadding();
    height = height + getPadding() + getTitleBarHeight();

    if (getMinWidth() > width)
        width = getMinWidth();
    else if (getMaxWidth() < width)
        width = getMaxWidth();
    if (getMinHeight() > height)
        height = getMinHeight();
    else if (getMaxHeight() < height)
        height = getMaxHeight();

    setSize(width, height);
}

void Window::setLocationRelativeTo(gcn::Widget *widget)
{
    int wx, wy;
    int x, y;

    widget->getAbsolutePosition(wx, wy);
    getAbsolutePosition(x, y);

    setPosition(getX() + (wx + (widget->getWidth() - getWidth()) / 2 - x),
                getY() + (wy + (widget->getHeight() - getHeight()) / 2 - y));
}

void Window::setLocationRelativeTo(ImageRect::ImagePosition position,
                                   int offsetX, int offsetY)
{
    if (position == ImageRect::UPPER_LEFT)
    {
    }
    else if (position == ImageRect::UPPER_CENTER)
    {
        offsetX += (graphics->getWidth() - getWidth()) / 2;
    }
    else if (position == ImageRect::UPPER_RIGHT)
    {
        offsetX += graphics->getWidth() - getWidth();
    }
    else if (position == ImageRect::LEFT)
    {
        offsetY += (graphics->getHeight() - getHeight()) / 2;
    }
    else if (position == ImageRect::CENTER)
    {
        offsetX += (graphics->getWidth() - getWidth()) / 2;
        offsetY += (graphics->getHeight() - getHeight()) / 2;
    }
    else if (position == ImageRect::RIGHT)
    {
        offsetX += graphics->getWidth() - getWidth();
        offsetY += (graphics->getHeight() - getHeight()) / 2;
    }
    else if (position == ImageRect::LOWER_LEFT)
    {
        offsetY += graphics->getHeight() - getHeight();
    }
    else if (position == ImageRect::LOWER_CENTER)
    {
        offsetX += (graphics->getWidth() - getWidth()) / 2;
        offsetY += graphics->getHeight() - getHeight();
    }
    else if (position == ImageRect::LOWER_RIGHT)
    {
        offsetX += graphics->getWidth() - getWidth();
        offsetY += graphics->getHeight() - getHeight();
    }

    setPosition(offsetX, offsetY);
}

void Window::setMinWidth(int width)
{
    mMinWinWidth = width > mSkin->getMinWidth() ? width : mSkin->getMinWidth();
}

void Window::setMinHeight(int height)
{
    mMinWinHeight = height > mSkin->getMinHeight() ?
                        height : mSkin->getMinHeight();
}

void Window::setMaxWidth(int width)
{
    mMaxWinWidth = width;
}

void Window::setMaxHeight(int height)
{
    mMaxWinHeight = height;
}

void Window::setResizable(bool r)
{
    if ((bool) mGrip == r) return;

    if (r)
    {
        mGrip = new ResizeGrip();
        mGrip->setX(getWidth() - mGrip->getWidth() - getChildrenArea().x);
        mGrip->setY(getHeight() - mGrip->getHeight() - getChildrenArea().y);
        add(mGrip);
    }
    else
    {
        remove(mGrip);
        delete mGrip;
        mGrip = 0;
    }
}

void Window::widgetResized(const gcn::Event &event)
{
    const gcn::Rectangle area = getChildrenArea();

    if (mGrip)
        mGrip->setPosition(getWidth() - mGrip->getWidth() - area.x,
                           getHeight() - mGrip->getHeight() - area.y);

    if (mLayout)
    {
        int w = area.width;
        int h = area.height;
        mLayout->reflow(w, h);
    }
}

void Window::setCloseButton(bool flag)
{
    mCloseButton = flag;
}

bool Window::isResizable()
{
    return mGrip;
}

void Window::scheduleDelete()
{
    windowContainer->scheduleDelete(this);
}

void Window::close()
{
    setVisible(false);
}

void Window::mousePressed(gcn::MouseEvent &event)
{
    // Let Guichan move window to top and figure out title bar drag
    gcn::Window::mousePressed(event);

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        const int x = event.getX();
        const int y = event.getY();

        // Handle close button
        if (mCloseButton)
        {
            gcn::Rectangle closeButtonRect(
                getWidth() - mSkin->getCloseImage()->getWidth() - getPadding(),
                getPadding(),
                mSkin->getCloseImage()->getWidth(),
                mSkin->getCloseImage()->getHeight());

            if (closeButtonRect.isPointInRect(x, y))
            {
                close();
            }
        }

        // Handle window resizing
        mouseResize = getResizeHandles(event);
    }
}

void Window::mouseReleased(gcn::MouseEvent &event)
{
    if (mGrip && mouseResize)
    {
        mouseResize = 0;
        gui->setCursorType(Gui::CURSOR_POINTER);
    }

    // This should be the responsibility of Guichan (and is from 0.8.0 on)
    mMoved = false;
}

void Window::mouseExited(gcn::MouseEvent &event)
{
    if (mGrip && !mouseResize)
        gui->setCursorType(Gui::CURSOR_POINTER);
}

void Window::mouseMoved(gcn::MouseEvent &event)
{
    int resizeHandles = getResizeHandles(event);

    // Changes the custom mouse cursor based on it's current position.
    switch (resizeHandles)
    {
        case BOTTOM | RIGHT:
            gui->setCursorType(Gui::CURSOR_RESIZE_DOWN_RIGHT);
            break;
        case BOTTOM | LEFT:
            gui->setCursorType(Gui::CURSOR_RESIZE_DOWN_LEFT);
            break;
        case BOTTOM:
            gui->setCursorType(Gui::CURSOR_RESIZE_DOWN);
            break;
        case RIGHT:
        case LEFT:
            gui->setCursorType(Gui::CURSOR_RESIZE_ACROSS);
            break;
        default:
            gui->setCursorType(Gui::CURSOR_POINTER);
    }
}

void Window::mouseDragged(gcn::MouseEvent &event)
{
    // Let Guichan handle title bar drag
    gcn::Window::mouseDragged(event);

    // Keep guichan window inside screen when it may be moved
    if (isMovable() && mMoved)
    {
        int newX = std::max(0, getX());
        int newY = std::max(0, getY());
        newX = std::min(graphics->getWidth() - getWidth(), newX);
        newY = std::min(graphics->getHeight() - getHeight(), newY);
        setPosition(newX, newY);
    }

    if (mouseResize && !mMoved)
    {
        const int dx = event.getX() - mDragOffsetX;
        const int dy = event.getY() - mDragOffsetY;
        gcn::Rectangle newDim = getDimension();

        if (mouseResize & (TOP | BOTTOM))
        {
            int newHeight = newDim.height + ((mouseResize & TOP) ? -dy : dy);
            newDim.height = std::min(mMaxWinHeight,
                                     std::max(mMinWinHeight, newHeight));

            if (mouseResize & TOP)
                newDim.y -= newDim.height - getHeight();
        }

        if (mouseResize & (LEFT | RIGHT))
        {
            int newWidth = newDim.width + ((mouseResize & LEFT) ? -dx : dx);
            newDim.width = std::min(mMaxWinWidth,
                                    std::max(mMinWinWidth, newWidth));

            if (mouseResize & LEFT)
                newDim.x -= newDim.width - getWidth();
        }

        // Keep guichan window inside screen (supports resizing any side)
        if (newDim.x < 0)
        {
            newDim.width += newDim.x;
            newDim.x = 0;
        }
        if (newDim.y < 0)
        {
            newDim.height += newDim.y;
            newDim.y = 0;
        }
        if (newDim.x + newDim.width > graphics->getWidth())
        {
            newDim.width = graphics->getWidth() - newDim.x;
        }
        if (newDim.y + newDim.height > graphics->getHeight())
        {
            newDim.height = graphics->getHeight() - newDim.y;
        }

        // Update mouse offset when dragging bottom or right border
        if (mouseResize & BOTTOM)
        {
            mDragOffsetY += newDim.height - getHeight();
        }
        if (mouseResize & RIGHT)
        {
            mDragOffsetX += newDim.width - getWidth();
        }

        // Set the new window and content dimensions
        setDimension(newDim);
    }
}

void Window::loadWindowState()
{
    const std::string &name = mWindowName;
    const std::string skinName = config.getValue(name + "Skin",
                                                 mSkin->getFilePath());
    assert(!name.empty());

    const int screenheight=graphics->getHeight();
    const int screenwidth=graphics->getWidth();
    const float winx=config.getValue(name+"WinX",mDefaultX);
    const float winy=config.getValue(name+"WinY",mDefaultX);
    if ((winx > 1.0f)||(winy >1.0f))
    { 
       /* old storage , absolute pixel value */
       setPosition((int) winx, (int) winy);
    }
    else
    {
       setPosition((int) (winx*screenwidth), (int) (winy*screenheight));
    }
    setVisible((bool) config.getValue(name + "Visible", false));
    mOldVisibility = (bool) config.getValue(name + "Hidden", false);

    if (skinName.compare(mSkin->getFilePath()) != 0)
    {
        mSkin->instances--;
        mSkin = skinLoader->load(skinName, mDefaultSkinPath);
    }

    if (mGrip)
    {
        const float fwidth = config.getValue(name + "WinWidth", mDefaultWidth);
        const float fheight = config.getValue(name + "WinHeight", mDefaultHeight);
        int width;
        int height;
        if ((fwidth > 1.0f)||(fheight > 1.0f))
        {
           /* old storage , absolute pixel value */
           width = (int) fwidth;
           height = (int) fheight;
        }
        else
        {
           width = (int)(fwidth * screenwidth);
           height = (int)(fheight * screenheight);
        }

        if (getMinWidth() > width)
            width = getMinWidth();
        else if (getMaxWidth() < width)
            width = getMaxWidth();
        if (getMinHeight() > height)
            height = getMinHeight();
        else if (getMaxHeight() < height)
            height = getMaxHeight();

        setSize(width, height);
    }
    else
    {
        setSize(mDefaultWidth, mDefaultHeight);
    }
}

void Window::saveWindowState()
{
    const float screenheight=(float)graphics->getHeight();
    const float screenwidth=(float)graphics->getWidth();
    // Saving X, Y and Width and Height for resizables in the config
    if (!mWindowName.empty() && mWindowName != "window")
    {
        config.setValue(mWindowName + "WinX", (float)getX()/screenwidth);
        config.setValue(mWindowName + "WinY", (float)getY()/screenheight);
        config.setValue(mWindowName + "Visible", isVisible());
        config.setValue(mWindowName + "Skin", mSkin->getFilePath());
        config.setValue(mWindowName + "Hidden", mOldVisibility);

        if (mGrip)
        {
            if (getMinWidth() > getWidth())
                setWidth(getMinWidth());
            else if (getMaxWidth() < getWidth())
                setWidth(getMaxWidth());
            if (getMinHeight() > getHeight())
                setHeight(getMinHeight());
            else if (getMaxHeight() < getHeight())
                setHeight(getMaxHeight());

            config.setValue(mWindowName + "WinWidth",(float)getWidth()/screenwidth);
            config.setValue(mWindowName + "WinHeight",(float)getHeight()/screenheight);
        }
    }
}

void Window::setDefaultSize(int defaultX, int defaultY,
                            int defaultWidth, int defaultHeight)
{
    if (getMinWidth() > defaultWidth)
        defaultWidth = getMinWidth();
    else if (getMaxWidth() < defaultWidth)
        defaultWidth = getMaxWidth();
    if (getMinHeight() > defaultHeight)
        defaultHeight = getMinHeight();
    else if (getMaxHeight() < defaultHeight)
        defaultHeight = getMaxHeight();

    mDefaultX = defaultX;
    mDefaultY = defaultY;
    mDefaultWidth = defaultWidth;
    mDefaultHeight = defaultHeight;
}

void Window::hide()
{
    const bool oldVisibility = isVisible();

    setVisible(mOldVisibility);

    mOldVisibility = oldVisibility;
}

void Window::setDefaultSize(int defaultWidth, int defaultHeight,
                            ImageRect::ImagePosition position,
                            int offsetX, int offsetY)
{
    int x = 0, y = 0;

    if (position == ImageRect::UPPER_LEFT)
    {
    }
    else if (position == ImageRect::UPPER_CENTER)
    {
        x = (graphics->getWidth() - defaultWidth) / 2;
    }
    else if (position == ImageRect::UPPER_RIGHT)
    {
        x = graphics->getWidth() - defaultWidth;
    }
    else if (position == ImageRect::LEFT)
    {
        y = (graphics->getHeight() - defaultHeight) / 2;
    }
    else if (position == ImageRect::CENTER)
    {
        x = (graphics->getWidth() - defaultWidth) / 2;
        y = (graphics->getHeight() - defaultHeight) / 2;
    }
    else if (position == ImageRect::RIGHT)
    {
        x = graphics->getWidth() - defaultWidth;
        y = (graphics->getHeight() - defaultHeight) / 2;
    }
    else if (position == ImageRect::LOWER_LEFT)
    {
        y = graphics->getHeight() - defaultHeight;
    }
    else if (position == ImageRect::LOWER_CENTER)
    {
        x = (graphics->getWidth() - defaultWidth) / 2;
        y = graphics->getHeight() - defaultHeight;
    }
    else if (position == ImageRect::LOWER_RIGHT)
    {
        x = graphics->getWidth() - defaultWidth;
        y = graphics->getHeight() - defaultHeight;
    }

    mDefaultX = x - offsetX;
    mDefaultY = y - offsetY;
    mDefaultWidth = defaultWidth;
    mDefaultHeight = defaultHeight;
}

void Window::resetToDefaultSize()
{
    setPosition(mDefaultX, mDefaultY);
    setSize(mDefaultWidth, mDefaultHeight);
    saveWindowState();
}

int Window::getResizeHandles(gcn::MouseEvent &event)
{
    int resizeHandles = 0;
    const int y = event.getY();

    if (mGrip && y > (int) mTitleBarHeight)
    {
        const int x = event.getX();

        if (!getChildrenArea().isPointInRect(x, y) &&
                event.getSource() == this)
        {
            resizeHandles |= (x > getWidth() - resizeBorderWidth) ? RIGHT :
                              (x < resizeBorderWidth) ? LEFT : 0;
            resizeHandles |= (y > getHeight() - resizeBorderWidth) ? BOTTOM :
                              (y < resizeBorderWidth) ? TOP : 0;
        }

        if (event.getSource() == mGrip)
        {
            mDragOffsetX = x;
            mDragOffsetY = y;
            resizeHandles |= BOTTOM | RIGHT;
        }
    }

    return resizeHandles;
}

int Window::getGuiAlpha()
{
    float alpha = config.getValue("guialpha", 0.8);
    return (int) (alpha * 255.0f);
}

Layout &Window::getLayout()
{
    if (!mLayout) mLayout = new Layout;
    return *mLayout;
}

LayoutCell &Window::place(int x, int y, gcn::Widget *wg, int w, int h)
{
    add(wg);
    return getLayout().place(wg, x, y, w, h);
}

ContainerPlacer Window::getPlacer(int x, int y)
{
    return ContainerPlacer(this, &getLayout().at(x, y));
}

void Window::reflowLayout(int w, int h)
{
    assert(mLayout);
    mLayout->reflow(w, h);
    delete mLayout;
    mLayout = NULL;
    setContentSize(w, h);
}

void Window::adaptToNewSize(int new_w,int new_h, int old_w, int old_h, bool extend,bool save)
{
   int x = getX(); 
   int y = getY();
   int w = getWidth();
   int h = getHeight();
   int xr = x+w;
   int yb = y+h;
   if (xr>old_w*3/4)
   { /* in the right side 
      * Keep the right border distance 
      */
      x+=new_w-old_w;
      if ((isResizable())&&(extend))
      {
         w=(w*new_w)/old_w;
         x-= w-getWidth();
      }
   }
   else
   {
      /* let's glue the left side */
      if ((isResizable())&&(extend))
      {
         w=(w*new_w)/old_w;
      }
   }
   if (yb>old_h*3/4)
   { /* in the bottom side 
      * Keep the bottom border distance 
      */
      y+=new_h-old_h;
      if ((isResizable())&&(extend))
      {
         h=(h*new_h)/old_h;
         y-= h-getHeight();
      }
   }
   else
   {
      /* let's glue the top side */
      if ((isResizable())&&(extend))
      {
         h=(h*new_h)/old_h;
      }
   }

   setPosition(x, y);
   setSize(w, h);
   if (save) saveWindowState();
}
