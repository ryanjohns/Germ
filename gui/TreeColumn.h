/*******************************************************************************

Media_Player - Linux music library and player.
Copyright (C) 2006  Ryan Johns and Stephen McCarthy

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*******************************************************************************/

#ifndef TREECOLUMN_H_
#define TREECOLUMN_H_

#include "../library/Artist.h"
#include "../library/Album.h"
#include <gtkmm/treemodel.h>

class TreeColumn : public Gtk::TreeModel::ColumnRecord {
public:
	TreeColumn();
	virtual ~TreeColumn();

	Gtk::TreeModelColumn<Glib::ustring> m_col_label;
	Gtk::TreeModelColumn<Glib::ustring> m_col_sort;
	Gtk::TreeModelColumn<Artist *> m_artist_node;
	Gtk::TreeModelColumn<Album *> m_album_node;
};

#endif /*TREECOLUMN_H_*/
