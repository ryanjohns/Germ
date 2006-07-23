#include "TreeView.h"
#include <vector>
#include <iostream>
#include <gdk/gdkkeysyms.h>

TreeView::TreeView(Library * _library, SonglistView * _songlist_view,
		PlaylistView * _playlist_view)
: library(_library),
  songlist_view(_songlist_view),
  playlist_view(_playlist_view) {
	ARTIST_ALBUM = "Artist/Album";
	ALBUM = "Album";
	
	pack_start(m_Box, Gtk::PACK_SHRINK);
	
	m_Box.pack_start(m_ViewLabel, Gtk::PACK_SHRINK);
	m_ViewLabel.set_text(" View by: ");
	
	m_Box.pack_start(m_ComboBox, Gtk::PACK_SHRINK);
	m_ComboBox.append_text(ARTIST_ALBUM);
	m_ComboBox.append_text(ALBUM);
	m_ComboBox.set_active_text(ARTIST_ALBUM);
	
	m_ComboBox.signal_changed().connect(
		sigc::mem_fun(*this, &TreeView::on_combo_changed)
	);
	
	pack_start(m_ScrollWindow);
	m_ScrollWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	
	m_ScrollWindow.add(m_TreeView);
	
	m_refTreeModel = Gtk::TreeStore::create(m_Columns);
	m_TreeView.set_model(m_refTreeModel);
	
	m_TreeView.append_column("", m_Columns.m_col_label);
	m_TreeView.get_column(0)->set_widget(m_DummyBox);
	m_refTreeModel->set_sort_column(m_Columns.m_col_sort, Gtk::SORT_ASCENDING);
	
//	std::list<Gtk::TargetEntry> list_targets;
//	list_targets.push_back(Gtk::TargetEntry("PlaylistView"));
//	m_TreeView.enable_model_drag_source(/*list_targets, */Gdk::MODIFIER_MASK, Gdk::ACTION_COPY);
	
	m_TreeView.signal_row_activated().connect(
		sigc::mem_fun(*this, &TreeView::on_row_activated)
	);
	m_TreeView.get_selection()->signal_changed().connect(
		sigc::mem_fun(*this, &TreeView::on_changed)
	);
/*	m_TreeView.signal_drag_data_get().connect(
		sigc::mem_fun(*this, &TreeView::on_drag_data_get)
	);*/
}

TreeView::~TreeView() {}

void TreeView::on_combo_changed() {
	if (m_ComboBox.get_active_text() == ARTIST_ALBUM) {
		showArtists();
	}
	else {
		showAlbums();
	}
}

void TreeView::on_row_activated(const Gtk::TreeModel::Path & path,
		Gtk::TreeViewColumn *) {
	if (m_TreeView.row_expanded(path))
		m_TreeView.collapse_row(path);
	else
		m_TreeView.expand_row(path, false);
}

void TreeView::on_changed() {
	Gtk::TreeModel::iterator iter = m_TreeView.get_selection()->get_selected();
	if (iter) {
		Gtk::TreeModel::Row row = *iter;
		if (row[m_Columns.m_artist_node] != NULL) {
			songlist_view->clearListData();
			songlist_view->setListData(row[m_Columns.m_artist_node]);
		}
		else if (row[m_Columns.m_album_node] != NULL) {
			songlist_view->clearListData();
			songlist_view->setListData(row[m_Columns.m_album_node]);
		}
	}
}

bool TreeView::on_key_press_event(GdkEventKey * event) {
	if (event->keyval == GDK_Insert) {
		Gtk::TreeModel::iterator iter =
				m_TreeView.get_selection()->get_selected();
		if (iter) {
			Gtk::TreeModel::Row row = *iter;
			if (row[m_Columns.m_artist_node] != NULL) {
				playlist_view->setListData(row[m_Columns.m_artist_node]);
			}
			else if (row[m_Columns.m_album_node] != NULL) {
				playlist_view->setListData(row[m_Columns.m_album_node]);
			}
			else {
				playlist_view->showAllSongs();
			}
		}
	}
	return true;
}

/*void TreeView::on_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &, Gtk::SelectionData & selection_data, guint, guint) {
	
}*/

void TreeView::showArtists() {
	m_ComboBox.set_active_text(ARTIST_ALBUM);
	clearTree();
	Gtk::TreeModel::Row row = *(m_refTreeModel->append());
	row[m_Columns.m_col_label] = "Artists";
	row[m_Columns.m_artist_node] = NULL;
	row[m_Columns.m_album_node] = NULL;
	std::vector<Artist> * artists = library->get_library_root();
	std::vector<Artist>::iterator it;
	for (it = artists->begin(); it != artists->end(); ++it) {
		std::string artist = *(*it).get_artist_name();
		Gtk::TreeModel::Row artistrow = *(m_refTreeModel->append(
				row.children()));
		if (artist.length() == 0) {
			artistrow[m_Columns.m_col_label] = "Unknown";
			artistrow[m_Columns.m_col_sort] = "Unknown";
		}
		else {
			unsigned int loc = artist.find("The ");
			if (loc == 0) {
				std::string tmp = artist.substr(4, std::string::npos);
				artistrow[m_Columns.m_col_sort] = tmp;
			}
			else {
				artistrow[m_Columns.m_col_sort] = artist;
			}
			artistrow[m_Columns.m_col_label] = artist;
		}
		artistrow[m_Columns.m_artist_node] = &(*it);
		artistrow[m_Columns.m_album_node] = NULL;
		std::vector<Album> * albums = (*it).get_album_children();
		std::vector<Album>::iterator it2;
		for (it2 = albums->begin(); it2 != albums->end(); ++it2) {
			std::string title = *(*it2).get_album_title();
			Gtk::TreeModel::Row albumrow = *(m_refTreeModel->append(
					artistrow.children()));
			if (title.length() == 0) {
				albumrow[m_Columns.m_col_label] = "Unknown";
				albumrow[m_Columns.m_col_sort] = "Unknown";
			}
			else {
				albumrow[m_Columns.m_col_label] = title;
				albumrow[m_Columns.m_col_sort] = title;
			}
			albumrow[m_Columns.m_artist_node] = NULL;
			albumrow[m_Columns.m_album_node] = &(*it2);
		}
	}
	expandRoot();
}

void TreeView::showAlbums() {
	clearTree();
	Gtk::TreeModel::Row row = *(m_refTreeModel->append());
	row[m_Columns.m_col_label] = "Albums";
	row[m_Columns.m_artist_node] = NULL;
	row[m_Columns.m_album_node] = NULL;
	std::vector<Artist> * artists = library->get_library_root();
	std::vector<Artist>::iterator it;
	for (it = artists->begin(); it != artists->end(); ++it) {
		std::vector<Album> * albums = (*it).get_album_children();
		std::vector<Album>::iterator it2;
		for (it2 = albums->begin(); it2 != albums->end(); ++it2) {
			std::string title = *(*it2).get_album_title();
			if (title.length() > 0) {
				Gtk::TreeModel::Row albumrow = *(m_refTreeModel->append(
						row.children()));
				albumrow[m_Columns.m_col_label] = title;
				albumrow[m_Columns.m_col_sort] = title;
				albumrow[m_Columns.m_artist_node] = NULL;
				albumrow[m_Columns.m_album_node] = &(*it2);
			}
		}
	}
	expandRoot();
}

void TreeView::clearTree() {
	m_refTreeModel->clear();
}

void TreeView::expandRoot() {
	Gtk::TreeIter iter = m_refTreeModel->children().begin();
	m_TreeView.expand_row(m_refTreeModel->get_path(iter), false);
}
