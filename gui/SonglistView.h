#ifndef SONGLISTVIEW_H_
#define SONGLISTVIEW_H_

#include "../library/Library.h"
#include "../library/Artist.h"
#include "../library/Album.h"
#include "../library/Song.h"
#include "../config/Configurations.h"
#include "ModelColumns.h"
#include "PlaylistView.h"
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>

class SonglistView : public Gtk::ScrolledWindow {
public:
	SonglistView(Library *, Configurations *, PlaylistView *);
	virtual ~SonglistView();
	
	void clearListData();
	void setListData(Artist *);
	void setListData(Album *);
	void showAllSongs();
	Gtk::TreeView * getTreeView();

protected:
	virtual void on_row_activated(const Gtk::TreeModel::Path &,
			Gtk::TreeViewColumn *);
	virtual bool on_key_press_event(GdkEventKey *);
//	virtual void on_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &, Gtk::SelectionData &, guint, guint);
	
	ModelColumns m_Columns;
	Gtk::TreeView m_TreeView;
	Glib::RefPtr<Gtk::ListStore> m_refTreeModel;

private:
	Library * library;
	Configurations * config;
	PlaylistView * playlist_view;
	
	void addSong(Song *);
	std::string formatLength(int);
	void addColumns();
};

#endif /*SONGLISTVIEW_H_*/
