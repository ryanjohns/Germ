/*******************************************************************************

Media_Player - Linux music library and player.
Copyright (C) 2006  Ryan Johns and Stephen McCarthy

This file is part of Media_Player.
Media_Player is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

Media_Player is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with Media_Player; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*******************************************************************************/

#ifndef SONGLISTVIEW_H_
#define SONGLISTVIEW_H_

#include "../library/Library.h"
#include "../library/Artist.h"
#include "../library/Album.h"
#include "../library/Song.h"
#include "../config/Configurations.h"
#include "ModelColumns.h"
#include "PlaylistView.h"
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>

class SonglistView : public Gtk::ScrolledWindow {
public:
	SonglistView(Library *, Configurations *, PlaylistView *);
	virtual ~SonglistView();

	void clear_list_data();
	Gtk::TreeView * get_tree_view();
	void set_list_data(Artist *);
	void set_list_data(Album *);
	void show_all_songs();

protected:
	virtual bool on_key_press_event(GdkEventKey *);
	virtual void on_row_activated(const Gtk::TreeModel::Path &,
		Gtk::TreeViewColumn *);

private:
	ModelColumns m_columns;
	Gtk::TreeView m_treeView;
	Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
	Library * m_library;
	Configurations * m_config;
	PlaylistView * m_playlistView;

	void add_song(Song *);
	std::string format_time(int);
};

#endif /*SONGLISTVIEW_H_*/
