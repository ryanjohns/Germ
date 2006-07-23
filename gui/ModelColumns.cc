#include "ModelColumns.h"

ModelColumns::ModelColumns() {
	add(m_col_artist_name);
	add(m_col_album_title);
	add(m_col_song_title);
	add(m_col_genre);
	add(m_col_year);
	add(m_col_track_number);
	add(m_col_length);
	add(m_col_string_length);
	add(m_col_bitrate);
	add(m_col_path);
	add(m_song_node);
}

ModelColumns::~ModelColumns() {}
