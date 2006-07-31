#include "PlaylistView.h"
#include <sstream>
#include <iostream>
#include <gdk/gdkkeysyms.h>

PlaylistView::PlaylistView(Library * library, Configurations * config,
	Player * player)
: m_library(library),
  m_config (config),
  m_player(player) {
	add(m_treeView);
	set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	m_refTreeModel = Gtk::ListStore::create(m_columns);
	m_treeView.set_model(m_refTreeModel);

	m_treeView.append_column("Artist", m_columns.m_col_artist_name);
	m_treeView.append_column("Album", m_columns.m_col_album_title);
	m_treeView.append_column("Title", m_columns.m_col_song_title);
	m_treeView.append_column("Length", m_columns.m_col_string_length);

	for (guint i = 0; i < m_treeView.get_columns().size(); ++i) {
		Gtk::TreeView::Column* pColumn = m_treeView.get_column(i);
//		pColumn->set_reorderable(true);
		pColumn->set_sizing(Gtk::TREE_VIEW_COLUMN_FIXED);
		pColumn->set_fixed_width(m_config->get_playlist_column(i));
		pColumn->set_resizable(true);
	}

	m_treeView.get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);

	m_treeView.set_reorderable();

	m_player->signal_song_request.connect(
		sigc::mem_fun(*this, &PlaylistView::on_song_request)
	);
	m_treeView.signal_row_activated().connect(
		sigc::mem_fun(*this, &PlaylistView::on_row_activated)
	);
}

PlaylistView::~PlaylistView() {}

void PlaylistView::clear_list_data() {
	m_refTreeModel->clear();
	cleanup_removing_playing_song();
}

Gtk::TreeView * PlaylistView::get_tree_view() {
	return &m_treeView;
}

bool PlaylistView::is_playlist_empty() {
	return m_refTreeModel->children().empty();
}

void PlaylistView::play_new_list() {
	if (!m_refTreeModel->children().empty()) {
		m_currentSong = m_refTreeModel->children().begin();
		Gtk::TreeModel::Row row = *m_currentSong;
		m_player->stop();
		m_player->play(row[m_columns.m_song_node]);
	}
}

void PlaylistView::restore_playlist() {
	std::vector<std::string> * playlist = m_config->get_playlist();
	for (unsigned int i = 0; i < playlist->size(); ++i) {
		std::string tmp(playlist->at(i));
		int delim_loc1 = 0;
		int delim_loc2 = tmp.find(SEPARATOR);
		std::string artist = tmp.substr(delim_loc1, delim_loc2);

		delim_loc1 = delim_loc2+1;
		delim_loc2 = tmp.find(SEPARATOR, delim_loc1);
		std::string album = tmp.substr(delim_loc1, delim_loc2-delim_loc1);

		delim_loc1 = delim_loc2+1;
		std::string title = tmp.substr(delim_loc1);
		std::cout << artist << ", " << album << ", " << title << std::endl;

		Song * song = m_library->find_song_node(artist, album, title);
		if (song != NULL) {
			add_song(song);
		}
	}
}

void PlaylistView::save_playlist() {
	std::vector<std::string> * playlist = m_config->get_playlist();
	playlist->clear();
	Gtk::TreeNodeChildren children = m_refTreeModel->children();
	Gtk::TreeModel::iterator it;
	for (it = children.begin(); it != children.end(); ++it) {
		Gtk::TreeModel::Row row = *it;
		std::string tmp("");
		tmp += row[m_columns.m_col_artist_name] + SEPARATOR;
		tmp += row[m_columns.m_col_album_title] + SEPARATOR;
		tmp += row[m_columns.m_col_song_title] + "";
		playlist->push_back(tmp);
	}
}

void PlaylistView::set_list_data(Artist * artist) {
	std::vector<Album> * albums = artist->get_album_children();
	std::vector<Album>::iterator it;
	for (it = albums->begin(); it != albums->end(); ++it) {
		set_list_data(&(*it));
	}
}

void PlaylistView::set_list_data(Album * album) {
	std::vector<Song> * songs = album->get_song_children();
	std::vector<Song>::iterator it;
	for (it = songs->begin(); it != songs->end(); ++it) {
		set_list_data(&(*it));
	}
}

void PlaylistView::set_list_data(Song * song) {
	add_song(song);
}

void PlaylistView::show_all_songs() {
	std::vector<Artist> * artists = m_library->get_library_root();
	std::vector<Artist>::iterator it;
	for (it = artists->begin(); it != artists->end(); ++it) {
		set_list_data(&(*it));
	}
}

bool PlaylistView::on_key_press_event(GdkEventKey * event) {
	if (event->keyval == GDK_Delete) {
		std::list<Gtk::TreeModel::Path> paths =
				m_treeView.get_selection()->get_selected_rows();
		if (paths.size() == m_refTreeModel->children().size()) {
			clear_list_data();
		}
		std::list<Gtk::TreeModel::Path>::reverse_iterator it;
		for (it = paths.rbegin(); it != paths.rend(); ++it) {
			Gtk::TreeModel::iterator iter = m_refTreeModel->get_iter(*it);
			if (iter) {
				if (m_currentSong.operator bool() && iter == m_currentSong) {
					cleanup_removing_playing_song();
				}
				m_refTreeModel->erase(iter);
			}
		}
	}
	return Gtk::ScrolledWindow::on_key_press_event(event);
}

void PlaylistView::on_row_activated(const Gtk::TreeModel::Path & path,
		Gtk::TreeViewColumn *) {
	Gtk::TreeModel::iterator iter = m_refTreeModel->get_iter(path);
	if (iter) {
		m_currentSong = iter;
		Gtk::TreeModel::Row row = *iter;
		m_player->stop();
		Song * s = row[m_columns.m_song_node];
		m_player->play(s);
	}
}

void PlaylistView::on_song_request(bool previous) {
	Gtk::TreeNodeChildren children = m_refTreeModel->children();
	if (m_currentSong.operator bool() && !children.empty()) {
		if (previous && m_currentSong != children.begin()) {
			undecorate_current_row();
			--m_currentSong;
			decorate_current_row();
			Gtk::TreeModel::Row row = *m_currentSong;
			m_player->set_next_song(row[m_columns.m_song_node]);
		}
		else if (!previous && m_currentSong != --children.end()) {
			undecorate_current_row();
			++m_currentSong;
			decorate_current_row();
			Gtk::TreeModel::Row row = *m_currentSong;
			m_player->set_next_song(row[m_columns.m_song_node]);
		}
		else {
			m_player->set_next_song(NULL);
		}
	}
	else {
		m_player->set_next_song(NULL);
	}
}

void PlaylistView::add_song(Song * song) {
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

void PlaylistView::cleanup_removing_playing_song() {
	m_player->stop();
	m_player->set_song_loaded(false);
	Gtk::TreeModel::iterator tmp;
	m_currentSong = tmp;
}

std::string PlaylistView::format_time(int length) {
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

void PlaylistView::decorate_current_row() {
	
}

void PlaylistView::undecorate_current_row() {
	
}
