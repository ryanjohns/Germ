#ifndef TREECOLUMN_H_
#define TREECOLUMN_H_

#include "../library/Artist.h"
#include "../library/Album.h"
#include <gtkmm/treemodel.h>

class TreeColumn : public Gtk::TreeModel::ColumnRecord {
public:
	TreeColumn();
	virtual ~TreeColumn();
	
	Gtk::TreeModelColumn<Glib::ustring> m_col_label;
	Gtk::TreeModelColumn<Glib::ustring> m_col_sort;
	Gtk::TreeModelColumn<Artist *> m_artist_node;
	Gtk::TreeModelColumn<Album *> m_album_node;
};

#endif /*TREECOLUMN_H_*/
