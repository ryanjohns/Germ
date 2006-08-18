/*******************************************************************************

Media_Player - Linux music library and player.
Copyright (C) 2006  Ryan Johns and Stephen McCarthy

This file is part of Media_Player.
Media_Player is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

Media_Player is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with Media_Player; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*******************************************************************************/

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
