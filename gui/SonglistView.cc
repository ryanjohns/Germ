#include "SonglistView.h"
#include <iostream>
#include <sstream>
#include <gdk/gdkkeysyms.h>

SonglistView::SonglistView(Library * _library, Configurations * _config,
		PlaylistView * _playlist_view)
: library(_library),
  config(_config),
  playlist_view(_playlist_view) {
	add(m_TreeView);
	set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	
	m_refTreeModel = Gtk::ListStore::create(m_Columns);
	m_TreeView.set_model(m_refTreeModel);
	
	addColumns();
	
	m_TreeView.append_column("Track", m_Columns.m_col_track_number);
	m_TreeView.get_column(0)->set_sort_column(m_Columns.m_col_track_number);
	
	m_TreeView.append_column("Title", m_Columns.m_col_song_title);
	m_TreeView.get_column(1)->set_sort_column(m_Columns.m_col_song_title);
	
	m_TreeView.append_column("Artist", m_Columns.m_col_artist_name);
	m_TreeView.get_column(2)->set_sort_column(m_Columns.m_col_artist_name);
	
	m_TreeView.append_column("Album", m_Columns.m_col_album_title);
	m_TreeView.get_column(3)->set_sort_column(m_Columns.m_col_album_title);
	
	m_TreeView.append_column("Length", m_Columns.m_col_string_length);
	m_TreeView.get_column(4)->set_sort_column(m_Columns.m_col_string_length);
	
	m_TreeView.append_column("Bitrate", m_Columns.m_col_bitrate);
	m_TreeView.get_column(5)->set_sort_column(m_Columns.m_col_bitrate);
	
	m_TreeView.append_column("Genre", m_Columns.m_col_genre);
	m_TreeView.get_column(6)->set_sort_column(m_Columns.m_col_genre);
	
	m_refTreeModel->set_sort_column(m_Columns.m_col_track_number,
			Gtk::SORT_ASCENDING);
	
	guint num_cols = m_TreeView.get_columns().size();
	for (guint i = 0; i < num_cols; ++i) {
		Gtk::TreeView::Column* pColumn = m_TreeView.get_column(i);
//		pColumn->set_reorderable(true);
		pColumn->set_sizing(Gtk::TREE_VIEW_COLUMN_FIXED);
		pColumn->set_fixed_width(config->get_songlist_column(i));
		pColumn->set_resizable(true);
	}
	
	m_TreeView.get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);
	
	m_TreeView.set_reorderable();
	
//	std::list<Gtk::TargetEntry> list_targets;
//	list_targets.push_back(Gtk::TargetEntry("PlaylistView"));
//	m_TreeView.enable_model_drag_source(/*list_targets, */Gdk::MODIFIER_MASK, Gdk::ACTION_COPY);
	
	m_TreeView.signal_row_activated().connect(
		sigc::mem_fun(*this, &SonglistView::on_row_activated)
	);
	m_TreeView.signal_key_press_event().connect(
		sigc::mem_fun(*this, &SonglistView::on_key_press_event), false
	);
/*	m_TreeView.signal_drag_data_get().connect(
		sigc::mem_fun(*this, &SonglistView::on_drag_data_get)
	);*/
}

SonglistView::~SonglistView() {}

void SonglistView::on_row_activated(const Gtk::TreeModel::Path & path,
		Gtk::TreeViewColumn *) {
	Gtk::TreeModel::iterator iter = m_refTreeModel->get_iter(path);
	if (iter) {
		Gtk::TreeModel::Row row = *iter;
		playlist_view->clearListData();
		playlist_view->setListData(row[m_Columns.m_song_node]);
		playlist_view->playNewList();
	}
}

bool SonglistView::on_key_press_event(GdkEventKey * event) {
	if (event->keyval == GDK_Insert) {
		std::list<Gtk::TreeModel::Path> paths =
				m_TreeView.get_selection()->get_selected_rows();
		std::list<Gtk::TreeModel::Path>::iterator it;
		for (it = paths.begin(); it != paths.end(); ++it) {
			Gtk::TreeModel::iterator iter = m_refTreeModel->get_iter(*it);
			if (iter) {
				Gtk::TreeModel::Row row = *iter;
				playlist_view->setListData(row[m_Columns.m_song_node]);
			}
		}
	}
	return true;
}

/*void SonglistView::on_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &, Gtk::SelectionData & selection_data, guint, guint) {
//	std::list<Gtk::TreeModel::Path> paths = m_TreeView.get_selection()->get_selected_rows();
//	Gtk::TreeModel::iterator iter = m_refTreeModel->get_iter(paths.front());
//	Gtk::TreeModel::Row row = *iter;
//	Song * song = row[m_Columns.m_song_node];
//	selection_data.set(selection_data.get_target(), 8, (const guint8*)song, sizeof(song));
	Glib::RefPtr<const Gtk::TreeSelection> sel = m_TreeView.get_selection();
	selection_data.set(selection_data.get_target(), 8, (const guint8*)sel, sizeof(Glib::RefPtr<Gtk::TreeSelection>));
}*/

void SonglistView::clearListData() {
	m_refTreeModel->clear();
}

void SonglistView::setListData(Artist * artist) {
	std::vector<Album> * albums = artist->get_album_children();
	std::vector<Album>::iterator it;
	for (it = albums->begin(); it != albums->end(); ++it) {
		setListData(&(*it));
	}
}

void SonglistView::setListData(Album * album) {
	std::vector<Song> * songs = album->get_song_children();
	std::vector<Song>::iterator it;
	for (it = songs->begin(); it != songs->end(); ++it) {
		addSong(&(*it));
	}
}

void SonglistView::showAllSongs() {
	std::vector<Artist> * artists = library->get_library_root();
	std::vector<Artist>::iterator it;
	for (it = artists->begin(); it != artists->end(); ++it) {
		setListData(&(*it));
	}
}

void SonglistView::addSong(Song * song) {
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

Gtk::TreeView * SonglistView::getTreeView() {
	return &m_TreeView;
}

std::string SonglistView::formatLength(int length) {
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

void SonglistView::addColumns() {
	
}
