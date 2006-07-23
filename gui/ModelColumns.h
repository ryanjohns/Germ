#ifndef MODELCOLUMNS_H_
#define MODELCOLUMNS_H_

#include <gtkmm/treemodel.h>
#include "../library/Song.h"

class ModelColumns : public Gtk::TreeModel::ColumnRecord {
public:
	ModelColumns();
	virtual ~ModelColumns();
	
	Gtk::TreeModelColumn<Glib::ustring> m_col_artist_name;
	Gtk::TreeModelColumn<Glib::ustring> m_col_album_title;
	Gtk::TreeModelColumn<Glib::ustring> m_col_song_title;
	Gtk::TreeModelColumn<Glib::ustring> m_col_genre;
	Gtk::TreeModelColumn<int> m_col_year;
	Gtk::TreeModelColumn<int> m_col_track_number;
	Gtk::TreeModelColumn<int> m_col_length;
	Gtk::TreeModelColumn<Glib::ustring> m_col_string_length;
	Gtk::TreeModelColumn<int> m_col_bitrate;
	Gtk::TreeModelColumn<Glib::ustring> m_col_path;
	Gtk::TreeModelColumn<Song *> m_song_node;
};

#endif /*MODELCOLUMNS_H_*/
