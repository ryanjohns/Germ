#ifndef PLAYLISTVIEW_H_
#define PLAYLISTVIEW_H_

#include "ModelColumns.h"
#include "../library/Library.h"
#include "../library/Artist.h"
#include "../library/Album.h"
#include "../library/Song.h"
#include "../config/Configurations.h"
#include "../player/Player.h"
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <sigc++/sigc++.h>

#define SEPARATOR (char)6

class PlaylistView : public Gtk::ScrolledWindow {
public:
	PlaylistView(Library *, Configurations *, Player *);
	virtual ~PlaylistView();
	
	void clearListData();
	void setListData(Artist *);
	void setListData(Album *);
	void setListData(Song *);
	void showAllSongs();
	Gtk::TreeView * getTreeView();
	void playNewList();
	void restorePlaylist();
	void savePlaylist();

protected:
	virtual void on_song_request(bool);
	virtual void on_row_activated(const Gtk::TreeModel::Path &,
			Gtk::TreeViewColumn *);
	virtual bool on_key_press_event(GdkEventKey *);
//	virtual void on_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	
	ModelColumns m_Columns;
	Gtk::TreeView m_TreeView;
	Glib::RefPtr<Gtk::ListStore> m_refTreeModel;

private:
	Library * library;
	Configurations * config;
	Player * player;
	Gtk::TreeModel::iterator current_song;
	
	void addSong(Song *);
	std::string formatLength(int);
	void decorateCurrentRow();
	void undecorateCurrentRow();
};

#endif /*PLAYLISTVIEW_H_*/
