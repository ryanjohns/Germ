#include "PlaylistView.h"
#include <iostream>
#include <sstream>
#include <gdk/gdkkeysyms.h>

PlaylistView::PlaylistView(Library * _library, Configurations * _config,
	Player * _player)
: library(_library),
  config (_config),
  player(_player) {
	add(m_TreeView);
	set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	m_refTreeModel = Gtk::ListStore::create(m_Columns);
	m_TreeView.set_model(m_refTreeModel);

	m_TreeView.append_column("Artist", m_Columns.m_col_artist_name);
	m_TreeView.append_column("Album", m_Columns.m_col_album_title);
	m_TreeView.append_column("Title", m_Columns.m_col_song_title);
	m_TreeView.append_column("Length", m_Columns.m_col_string_length);

	for (guint i = 0; i < m_TreeView.get_columns().size(); ++i) {
		Gtk::TreeView::Column* pColumn = m_TreeView.get_column(i);
//		pColumn->set_reorderable(true);
		pColumn->set_sizing(Gtk::TREE_VIEW_COLUMN_FIXED);
		pColumn->set_fixed_width(config->get_playlist_column(i));
		pColumn->set_resizable(true);
	}

	m_TreeView.get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);

	m_TreeView.set_reorderable();

//	std::list<Gtk::TargetEntry> list_targets;
//	list_targets.push_back(Gtk::TargetEntry("PlaylistView"));
//	m_TreeView.enable_model_drag_source(/*list_targets, */Gdk::MODIFIER_MASK, Gdk::ACTION_MOVE);
//	m_TreeView.enable_model_drag_dest(/*list_targets, */Gdk::ACTION_COPY | Gdk::ACTION_MOVE);

	player->m_signal_song_request.connect(
		sigc::mem_fun(*this, &PlaylistView::on_song_request)
	);
	m_TreeView.signal_row_activated().connect(
		sigc::mem_fun(*this, &PlaylistView::on_row_activated)
	);
	m_TreeView.signal_key_press_event().connect(
		sigc::mem_fun(*this, &PlaylistView::on_key_press_event), false
	);
/*	m_TreeView.signal_drag_data_received().connect(
		sigc::mem_fun(*this, &PlaylistView::on_drag_data_received)
	);*/
}

PlaylistView::~PlaylistView() {}

/*
 * TODO
 * If you move the playing song and then select next or
 * previous there is a crash.
*/
void PlaylistView::on_song_request(bool previous) {
	Gtk::TreeNodeChildren children = m_refTreeModel->children();
	if (current_song.operator bool() && !children.empty()) {
		if (previous && current_song != children.begin()) {
			undecorateCurrentRow();
			--current_song;
			decorateCurrentRow();
			Gtk::TreeModel::Row row = *current_song;
			player->setNextSong(row[m_Columns.m_song_node]);
		}
		else if (!previous && current_song != --children.end()) {
			undecorateCurrentRow();
			++current_song;
			decorateCurrentRow();
			Gtk::TreeModel::Row row = *current_song;
			player->setNextSong(row[m_Columns.m_song_node]);
		}
		else {
			player->setNextSong(NULL);
		}
	}
	else {
		player->setNextSong(NULL);
	}
}

void PlaylistView::on_row_activated(const Gtk::TreeModel::Path & path,
		Gtk::TreeViewColumn *) {
	Gtk::TreeModel::iterator iter = m_refTreeModel->get_iter(path);
	if (iter) {
		current_song = iter;
		Gtk::TreeModel::Row row = *iter;
		player->stop();
		Song * s = row[m_Columns.m_song_node];
		player->play(s);
	}
}

bool PlaylistView::on_key_press_event(GdkEventKey * event) {
	if (event->keyval == GDK_Delete) {
		std::list<Gtk::TreeModel::Path> paths =
				m_TreeView.get_selection()->get_selected_rows();
		std::list<Gtk::TreeModel::Path>::reverse_iterator it;
		for (it = paths.rbegin(); it != paths.rend(); ++it) {
			Gtk::TreeModel::iterator iter = m_refTreeModel->get_iter(*it);
			if (iter) {
				if (iter == current_song) {
					player->stop();
				}
				m_refTreeModel->erase(iter);
			}
		}
	}
	return true;
}

/*void PlaylistView::on_drag_data_received(const Glib::RefPtr<Gdk::DragContext> & context, int x, int y, const Gtk::SelectionData & selection_data, guint info, guint time) {
	std::cout << "selected data type: " << selection_data.get_data_type() << std::endl;
	context->drag_finish(true, false, time);
}*/

void PlaylistView::clearListData() {
	m_refTreeModel->clear();
	player->stop();
}

void PlaylistView::setListData(Artist * artist) {
	std::vector<Album> * albums = artist->get_album_children();
	std::vector<Album>::iterator it;
	for (it = albums->begin(); it != albums->end(); ++it) {
		setListData(&(*it));
	}
}

void PlaylistView::setListData(Album * album) {
	std::vector<Song> * songs = album->get_song_children();
	std::vector<Song>::iterator it;
	for (it = songs->begin(); it != songs->end(); ++it) {
		setListData(&(*it));
	}
}

void PlaylistView::setListData(Song * song) {
	addSong(song);
}

void PlaylistView::showAllSongs() {
	std::vector<Artist> * artists = library->get_library_root();
	std::vector<Artist>::iterator it;
	for (it = artists->begin(); it != artists->end(); ++it) {
		setListData(&(*it));
	}
}

Gtk::TreeView * PlaylistView::getTreeView() {
	return &m_TreeView;
}

void PlaylistView::playNewList() {
	if (!m_refTreeModel->children().empty()) {
		current_song = m_refTreeModel->children().begin();
		Gtk::TreeModel::Row row = *current_song;
		player->stop();
		player->play(row[m_Columns.m_song_node]);
	}
}

void PlaylistView::restorePlaylist() {
	std::vector<std::string> * playlist = config->get_playlist();
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

		Song * song = library->find_song_node(artist, album, title);
		if (song != NULL) {
			addSong(song);
		}
	}
}

void PlaylistView::savePlaylist() {
	std::vector<std::string> * playlist = config->get_playlist();
	playlist->clear();
	Gtk::TreeNodeChildren children = m_refTreeModel->children();
	Gtk::TreeModel::iterator it;
	for (it = children.begin(); it != children.end(); ++it) {
		Gtk::TreeModel::Row row = *it;
		std::string tmp("");
		tmp += row[m_Columns.m_col_artist_name] + SEPARATOR;
		tmp += row[m_Columns.m_col_album_title] + SEPARATOR;
		tmp += row[m_Columns.m_col_song_title] + "";
		playlist->push_back(tmp);
	}
}

void PlaylistView::addSong(Song * song) {
	Gtk::TreeModel::Row row = *(m_refTreeModel->append());
	row[m_Columns.m_col_artist_name] = *song->get_artist_name();
	row[m_Columns.m_col_album_title] = *song->get_album_title();
	row[m_Columns.m_col_song_title] = *song->get_song_title();
	row[m_Columns.m_col_genre] = *song->get_genre();
	row[m_Columns.m_col_year] = song->get_year();
	row[m_Columns.m_col_track_number] = song->get_track_number();
	row[m_Columns.m_col_length] = song->get_length();
	row[m_Columns.m_col_string_length] = formatLength(song->get_length());
	row[m_Columns.m_col_bitrate] = song->get_bitrate();
	row[m_Columns.m_col_path] = *song->get_path();
	row[m_Columns.m_song_node] = song;
}

std::string PlaylistView::formatLength(int length) {
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

void PlaylistView::decorateCurrentRow() {
	/*Gtk::CellRendererText * text;
	int x = m_TreeView.get_columns().size();
	for (int i = 0; i < x; ++i) {
		if ((text = dynamic_cast<Gtk::CellRendererText*>
				(m_TreeView.get_column_cell_renderer(i)))) {
			text->property_weight() = 1000;
		}
	}*/
}

void PlaylistView::undecorateCurrentRow() {
	
}
