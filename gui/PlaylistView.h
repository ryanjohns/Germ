/*******************************************************************************

Germ - The infectious music player.
Copyright (C) 2006  Ryan Johns and Stephen McCarthy

This file is part of Media_Player.
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

#ifndef PLAYLISTVIEW_H_
#define PLAYLISTVIEW_H_

#include "ModelColumns.h"
#include "../library/Library.h"
#include "../library/Artist.h"
#include "../library/Album.h"
#include "../library/Song.h"
#include "../config/Configurations.h"
#include "../player/Player.h"
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>

#define SEPARATOR (char)6

class PlaylistView : public Gtk::ScrolledWindow {
public:
	PlaylistView(Library *, Configurations *, Player *);
	virtual ~PlaylistView();

	void clear_list_data();
	Gtk::TreeView * get_tree_view();
	bool is_playlist_empty();
	void play_new_list();
	void restore_playlist();
	void save_playlist();
	void set_list_data(Artist *);
	void set_list_data(Album *);
	void set_list_data(Song *);
	void show_all_songs();

protected:
	virtual bool on_key_press_event(GdkEventKey *);
	virtual void on_row_activated(const Gtk::TreeModel::Path &,
		Gtk::TreeViewColumn *);
	virtual void on_song_request(bool);

private:
	ModelColumns m_columns;
	Gtk::TreeView m_treeView;
	Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
	Library * m_library;
	Configurations * m_config;
	Player * m_player;
	Gtk::TreeModel::iterator m_currentSong;

	void add_song(Song *);
	void cleanup_removing_playing_song();
	std::string format_time(int);
	void decorate_current_row();
	void undecorate_current_row();
};

#endif /*PLAYLISTVIEW_H_*/
