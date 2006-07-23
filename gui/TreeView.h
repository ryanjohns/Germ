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
	
	void showArtists();
	void showAlbums();

protected:
	virtual void on_combo_changed();
	virtual void on_row_activated(const Gtk::TreeModel::Path &,
			Gtk::TreeViewColumn *);
	virtual void on_changed();
	virtual bool on_key_press_event(GdkEventKey *);
//	virtual void on_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &, Gtk::SelectionData &, guint, guint);
	
	TreeColumn m_Columns;
	Gtk::TreeView m_TreeView;
	Glib::RefPtr<Gtk::TreeStore> m_refTreeModel;
	Gtk::ComboBoxText m_ComboBox;
	Gtk::HBox m_Box, m_DummyBox;
	Gtk::Label m_ViewLabel;
	Gtk::ScrolledWindow m_ScrollWindow;

private:
	Library * library;
	SonglistView * songlist_view;
	PlaylistView * playlist_view;
	Glib::ustring ARTIST_ALBUM;
	Glib::ustring ALBUM;
	
	void clearTree();
	void expandRoot();
};

#endif /*TREEVIEW_H_*/
