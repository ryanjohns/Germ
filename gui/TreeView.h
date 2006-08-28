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

#ifndef TREEVIEW_H_
#define TREEVIEW_H_

#include "TreeColumn.h"
#include "SonglistView.h"
#include "PlaylistView.h"
#include "../library/Library.h"
#include <gtkmm/box.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treestore.h>
#include <gtkmm/label.h>
#include <gtkmm/scrolledwindow.h>

class TreeView : public Gtk::VBox {
public:
	TreeView(Library *, SonglistView *, PlaylistView *);
	virtual ~TreeView();

	void show_albums();
	void show_artists();

protected:
	virtual void on_changed();
	virtual void on_combo_changed();
	virtual bool on_key_press_event(GdkEventKey *);
	virtual void on_row_activated(const Gtk::TreeModel::Path &,
			Gtk::TreeViewColumn *);

private:
	TreeColumn m_columns;
	Gtk::TreeView m_treeView;
	Glib::RefPtr<Gtk::TreeStore> m_refTreeModel;
	Gtk::ComboBoxText m_comboBox;
	Gtk::HBox m_box, m_dummyBox;
	Gtk::Label m_viewLabel;
	Gtk::ScrolledWindow m_scrolledWindow;
	Library * m_library;
	SonglistView * m_songlistView;
	PlaylistView * m_playlistView;
	Glib::ustring ARTIST_ALBUM;
	Glib::ustring ALBUM;

	void clear_tree();
	void expand_root();
};

#endif /*TREEVIEW_H_*/
