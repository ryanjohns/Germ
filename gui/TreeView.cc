#include "TreeView.h"
#include <vector>
#include <iostream>
#include <gdk/gdkkeysyms.h>

TreeView::TreeView(Library * library, SonglistView * songlistView,
		PlaylistView * playlistView)
: m_library(library),
  m_songlistView(songlistView),
  m_playlistView(playlistView) {
	ARTIST_ALBUM = "Artist/Album";
	ALBUM = "Album";

	pack_start(m_box, Gtk::PACK_SHRINK);

	m_box.pack_start(m_viewLabel, Gtk::PACK_SHRINK);
	m_viewLabel.set_text(" View by: ");
	
	m_box.pack_start(m_comboBox, Gtk::PACK_SHRINK);
	m_comboBox.append_text(ARTIST_ALBUM);
	m_comboBox.append_text(ALBUM);
	m_comboBox.set_active_text(ARTIST_ALBUM);

	m_comboBox.signal_changed().connect(
		sigc::mem_fun(*this, &TreeView::on_combo_changed)
	);

	pack_start(m_scrolledWindow);
	m_scrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	m_scrolledWindow.add(m_treeView);

	m_refTreeModel = Gtk::TreeStore::create(m_columns);
	m_treeView.set_model(m_refTreeModel);

	m_treeView.append_column("", m_columns.m_col_label);
	m_treeView.get_column(0)->set_widget(m_dummyBox);
	m_refTreeModel->set_sort_column(m_columns.m_col_sort, Gtk::SORT_ASCENDING);

	m_treeView.signal_row_activated().connect(
		sigc::mem_fun(*this, &TreeView::on_row_activated)
	);
	m_treeView.get_selection()->signal_changed().connect(
		sigc::mem_fun(*this, &TreeView::on_changed)
	);
}

TreeView::~TreeView() {}

void TreeView::show_albums() {
	clear_tree();
	Gtk::TreeModel::Row row = *(m_refTreeModel->append());
	row[m_columns.m_col_label] = "Albums";
	row[m_columns.m_artist_node] = NULL;
	row[m_columns.m_album_node] = NULL;
	std::vector<Artist> * artists = m_library->get_library_root();
	std::vector<Artist>::iterator it;
	for (it = artists->begin(); it != artists->end(); ++it) {
		std::vector<Album> * albums = (*it).get_album_children();
		std::vector<Album>::iterator it2;
		for (it2 = albums->begin(); it2 != albums->end(); ++it2) {
			std::string title = *(*it2).get_album_title();
			if (title.length() > 0) {
				Gtk::TreeModel::Row albumrow = *(m_refTreeModel->append(
						row.children()));
				albumrow[m_columns.m_col_label] = title;
				albumrow[m_columns.m_col_sort] = title;
				albumrow[m_columns.m_artist_node] = NULL;
				albumrow[m_columns.m_album_node] = &(*it2);
			}
		}
	}
	expand_root();
}

void TreeView::show_artists() {
	m_comboBox.set_active_text(ARTIST_ALBUM);
	clear_tree();
	Gtk::TreeModel::Row row = *(m_refTreeModel->append());
	row[m_columns.m_col_label] = "Artists";
	row[m_columns.m_artist_node] = NULL;
	row[m_columns.m_album_node] = NULL;
	std::vector<Artist> * artists = m_library->get_library_root();
	std::vector<Artist>::iterator it;
	for (it = artists->begin(); it != artists->end(); ++it) {
		std::string artist = *(*it).get_artist_name();
		Gtk::TreeModel::Row artistrow = *(m_refTreeModel->append(
				row.children()));
		if (artist.length() == 0) {
			artistrow[m_columns.m_col_label] = "Unknown";
			artistrow[m_columns.m_col_sort] = "Unknown";
		}
		else {
			unsigned int loc = artist.find("The ");
			if (loc == 0) {
				std::string tmp = artist.substr(4, std::string::npos);
				artistrow[m_columns.m_col_sort] = tmp;
			}
			else {
				artistrow[m_columns.m_col_sort] = artist;
			}
			artistrow[m_columns.m_col_label] = artist;
		}
		artistrow[m_columns.m_artist_node] = &(*it);
		artistrow[m_columns.m_album_node] = NULL;
		std::vector<Album> * albums = (*it).get_album_children();
		std::vector<Album>::iterator it2;
		for (it2 = albums->begin(); it2 != albums->end(); ++it2) {
			std::string title = *(*it2).get_album_title();
			Gtk::TreeModel::Row albumrow = *(m_refTreeModel->append(
					artistrow.children()));
			if (title.length() == 0) {
				albumrow[m_columns.m_col_label] = "Unknown";
				albumrow[m_columns.m_col_sort] = "Unknown";
			}
			else {
				albumrow[m_columns.m_col_label] = title;
				albumrow[m_columns.m_col_sort] = title;
			}
			albumrow[m_columns.m_artist_node] = NULL;
			albumrow[m_columns.m_album_node] = &(*it2);
		}
	}
	expand_root();
}

void TreeView::on_changed() {
	Gtk::TreeModel::iterator iter = m_treeView.get_selection()->get_selected();
	if (iter) {
		Gtk::TreeModel::Row row = *iter;
		if (row[m_columns.m_artist_node] != NULL) {
			m_songlistView->clear_list_data();
			m_songlistView->set_list_data(row[m_columns.m_artist_node]);
		}
		else if (row[m_columns.m_album_node] != NULL) {
			m_songlistView->clear_list_data();
			m_songlistView->set_list_data(row[m_columns.m_album_node]);
		}
	}
}

void TreeView::on_combo_changed() {
	if (m_comboBox.get_active_text() == ARTIST_ALBUM) {
		show_artists();
	}
	else {
		show_albums();
	}
}

bool TreeView::on_key_press_event(GdkEventKey * event) {
	if (event->keyval == GDK_Insert) {
		Gtk::TreeModel::iterator iter =
				m_treeView.get_selection()->get_selected();
		if (iter) {
			Gtk::TreeModel::Row row = *iter;
			if (row[m_columns.m_artist_node] != NULL) {
				m_playlistView->set_list_data(row[m_columns.m_artist_node]);
			}
			else if (row[m_columns.m_album_node] != NULL) {
				m_playlistView->set_list_data(row[m_columns.m_album_node]);
			}
			else {
				m_playlistView->show_all_songs();
			}
		}
	}
	return true;
}

void TreeView::on_row_activated(const Gtk::TreeModel::Path & path,
		Gtk::TreeViewColumn *) {
	if (m_treeView.row_expanded(path))
		m_treeView.collapse_row(path);
	else
		m_treeView.expand_row(path, false);
}

void TreeView::clear_tree() {
	m_refTreeModel->clear();
}

void TreeView::expand_root() {
	Gtk::TreeIter iter = m_refTreeModel->children().begin();
	m_treeView.expand_row(m_refTreeModel->get_path(iter), false);
}
