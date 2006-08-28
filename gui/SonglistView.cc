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

#include "SonglistView.h"
#include <sstream>
#include <gdk/gdkkeysyms.h>

SonglistView::SonglistView(Library * library, Configurations * config,
		PlaylistView * playlistView)
: m_library(library),
  m_config(config),
  m_playlistView(playlistView) {
	add(m_treeView);
	set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	m_refTreeModel = Gtk::ListStore::create(m_columns);
	m_treeView.set_model(m_refTreeModel);

	m_treeView.append_column("Track", m_columns.m_col_track_number);
	m_treeView.get_column(0)->set_sort_column(m_columns.m_col_track_number);

	m_treeView.append_column("Title", m_columns.m_col_song_title);
	m_treeView.get_column(1)->set_sort_column(m_columns.m_col_song_title);

	m_treeView.append_column("Artist", m_columns.m_col_artist_name);
	m_treeView.get_column(2)->set_sort_column(m_columns.m_col_artist_name);

	m_treeView.append_column("Album", m_columns.m_col_album_title);
	m_treeView.get_column(3)->set_sort_column(m_columns.m_col_album_title);

	m_treeView.append_column("Length", m_columns.m_col_string_length);
	m_treeView.get_column(4)->set_sort_column(m_columns.m_col_string_length);

	m_treeView.append_column("Bitrate", m_columns.m_col_bitrate);
	m_treeView.get_column(5)->set_sort_column(m_columns.m_col_bitrate);

	m_treeView.append_column("Genre", m_columns.m_col_genre);
	m_treeView.get_column(6)->set_sort_column(m_columns.m_col_genre);

	m_refTreeModel->set_sort_column(m_columns.m_col_track_number,
			Gtk::SORT_ASCENDING);

	guint num_cols = m_treeView.get_columns().size();
	for (guint i = 0; i < num_cols; ++i) {
		Gtk::TreeView::Column* pColumn = m_treeView.get_column(i);
//		pColumn->set_reorderable(true);
		pColumn->set_sizing(Gtk::TREE_VIEW_COLUMN_FIXED);
		pColumn->set_fixed_width(m_config->get_songlist_column(i));
		pColumn->set_resizable(true);
	}

	m_treeView.get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);

	m_treeView.set_reorderable();

	m_treeView.signal_row_activated().connect(
		sigc::mem_fun(*this, &SonglistView::on_row_activated)
	);
}

SonglistView::~SonglistView() {}

void SonglistView::clear_list_data() {
	m_refTreeModel->clear();
}

Gtk::TreeView * SonglistView::get_tree_view() {
	return &m_treeView;
}

void SonglistView::set_list_data(Artist * artist) {
	std::vector<Album> * albums = artist->get_album_children();
	std::vector<Album>::iterator it;
	for (it = albums->begin(); it != albums->end(); ++it) {
		set_list_data(&(*it));
	}
}

void SonglistView::set_list_data(Album * album) {
	std::vector<Song> * songs = album->get_song_children();
	std::vector<Song>::iterator it;
	for (it = songs->begin(); it != songs->end(); ++it) {
		add_song(&(*it));
	}
}

void SonglistView::show_all_songs() {
	std::vector<Artist> * artists = m_library->get_library_root();
	std::vector<Artist>::iterator it;
	for (it = artists->begin(); it != artists->end(); ++it) {
		set_list_data(&(*it));
	}
}

bool SonglistView::on_key_press_event(GdkEventKey * event) {
	if (event->keyval == GDK_Insert) {
		std::list<Gtk::TreeModel::Path> paths =
				m_treeView.get_selection()->get_selected_rows();
		std::list<Gtk::TreeModel::Path>::iterator it;
		for (it = paths.begin(); it != paths.end(); ++it) {
			Gtk::TreeModel::iterator iter = m_refTreeModel->get_iter(*it);
			if (iter) {
				Gtk::TreeModel::Row row = *iter;
				m_playlistView->set_list_data(row[m_columns.m_song_node]);
			}
		}
	}
	return Gtk::ScrolledWindow::on_key_press_event(event);
}

void SonglistView::on_row_activated(const Gtk::TreeModel::Path & path,
		Gtk::TreeViewColumn *) {
	Gtk::TreeModel::iterator iter = m_refTreeModel->get_iter(path);
	if (iter) {
		Gtk::TreeModel::Row row = *iter;
		m_playlistView->clear_list_data();
		m_playlistView->set_list_data(row[m_columns.m_song_node]);
		m_playlistView->play_new_list();
	}
}

void SonglistView::add_song(Song * song) {
	Gtk::TreeModel::Row row = *(m_refTreeModel->append());
	row[m_columns.m_col_artist_name] = *song->get_artist_name();
	row[m_columns.m_col_album_title] = *song->get_album_title();
	row[m_columns.m_col_song_title] = *song->get_song_title();
	row[m_columns.m_col_genre] = *song->get_genre();
	row[m_columns.m_col_year] = song->get_year();
	row[m_columns.m_col_track_number] = song->get_track_number();
	row[m_columns.m_col_length] = song->get_length();
	row[m_columns.m_col_string_length] = format_time(song->get_length());
	row[m_columns.m_col_bitrate] = song->get_bitrate();
	row[m_columns.m_col_path] = *song->get_path();
	row[m_columns.m_song_node] = song;
}

std::string SonglistView::format_time(int length) {
	std::ostringstream oss;
	int minutes = length / 60;
	int seconds = length - minutes * 60;
	oss << minutes << ":";
	if (seconds < 10) {
		oss << 0 << seconds;
	}
	else {
		oss << seconds;
	}
	return oss.str();
}
