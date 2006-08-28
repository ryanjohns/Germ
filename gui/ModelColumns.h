/*******************************************************************************

Germ - The infectious music player.
Copyright (C) 2006  Ryan Johns and Stephen McCarthy

This file is part of Germ.
Germ is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

Germ is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with Germ; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*******************************************************************************/

#ifndef MODELCOLUMNS_H_
#define MODELCOLUMNS_H_

#include <gtkmm/treemodel.h>
#include "../library/Song.h"

class ModelColumns : public Gtk::TreeModel::ColumnRecord {
public:
	ModelColumns();
	virtual ~ModelColumns();

	Gtk::TreeModelColumn<Glib::ustring> m_col_artist_name;
	Gtk::TreeModelColumn<Glib::ustring> m_col_album_title;
	Gtk::TreeModelColumn<Glib::ustring> m_col_song_title;
	Gtk::TreeModelColumn<Glib::ustring> m_col_genre;
	Gtk::TreeModelColumn<int> m_col_year;
	Gtk::TreeModelColumn<int> m_col_track_number;
	Gtk::TreeModelColumn<int> m_col_length;
	Gtk::TreeModelColumn<Glib::ustring> m_col_string_length;
	Gtk::TreeModelColumn<int> m_col_bitrate;
	Gtk::TreeModelColumn<Glib::ustring> m_col_path;
	Gtk::TreeModelColumn<Song *> m_song_node;
};

#endif /*MODELCOLUMNS_H_*/
