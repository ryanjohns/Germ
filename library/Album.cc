/*******************************************************************************

Media_Player - Linux music library and player.
Copyright (C) 2006  Ryan Johns and Stephen McCarthy

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*******************************************************************************/

#include "Album.h"
#include "Song.h"
#include <string>
#include <vector>

Album::Album(std::string artistName, std::string albumTitle)
: m_artistName(artistName),
  m_albumTitle(albumTitle) {}

Album::~Album() {}

std::string * Album::get_album_title() {
	return &m_albumTitle;
}

std::string * Album::get_artist_name() {
	return &m_artistName;
}

std::vector<Song> * Album::get_song_children() {
	return &m_songChildren;
}
