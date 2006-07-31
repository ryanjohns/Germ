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
