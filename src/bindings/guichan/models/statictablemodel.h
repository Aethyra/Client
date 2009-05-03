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

#ifndef STATIC_TABLE_MODEL_H
#define STATIC_TABLE_MODEL_H

#include <vector>

#include "tablemodel.h"

#include "../guichanfwd.h"

class StaticTableModel : public TableModel
{
    public:
        StaticTableModel(int width, int height);
        virtual ~StaticTableModel(void);

        /**
         * Inserts a widget into the table model.
         * The model is resized to accomodate the widget's width and height,
         * unless column width / row height have been fixed.
         */
        virtual void set(int row, int column, gcn::Widget *widget);

        /**
         * Fixes the column width for a given column; this overrides dynamic
         * width inference.
         *
         * Semantics are undefined for width 0.
         */
        virtual void fixColumnWidth(int column, int width);

        /**
         * Fixes the row height; this overrides dynamic height inference.
         *
         * Semantics are undefined for width 0.
         */
        virtual void fixRowHeight(int height);

        /**
         * Resizes the table model
         */
        virtual void resize(void);

        virtual int getRows(void);
        virtual int getColumns(void);
        virtual int getRowHeight(void);
        virtual int getWidth(void);
        virtual int getHeight(void);
        virtual int getColumnWidth(int index);
        virtual gcn::Widget *getElementAt(int row, int column);

    protected:
        int mRows, mColumns;
        int mHeight;
        std::vector<gcn::Widget *> mTableModel;
        std::vector<int> mWidths;
};

#endif /* !defined(STATIC_TABLE_MODEL_H) */
