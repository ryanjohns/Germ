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

	void clear_list_data();
	Gtk::TreeView * get_tree_view();
	void set_list_data(Artist *);
	void set_list_data(Album *);
	void show_all_songs();

protected:
	virtual bool on_key_press_event(GdkEventKey *);
	virtual void on_row_activated(const Gtk::TreeModel::Path &,
		Gtk::TreeViewColumn *);

private:
	ModelColumns m_columns;
	Gtk::TreeView m_treeView;
	Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
	Library * m_library;
	Configurations * m_config;
	PlaylistView * m_playlistView;

	void add_song(Song *);
	std::string format_time(int);
};

#endif /*SONGLISTVIEW_H_*/
