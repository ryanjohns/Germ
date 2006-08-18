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

#include "Song.h"
#include <string>

Song::Song(std::string artistName, std::string albumTitle,
	std::string songTitle, std::string genre, int year,
	int trackNumber, int length, int bitrate, std::string path) 
: m_artistName(artistName),
  m_albumTitle(albumTitle),
  m_songTitle(songTitle),
  m_genre(genre),
  m_year(year),
  m_trackNumber(trackNumber),
  m_length(length),
  m_bitrate(bitrate),
  m_path(path) {}

Song::~Song() {}

std::string * Song::get_album_title() {
	return &m_albumTitle;
}

std::string * Song::get_artist_name() {
	return &m_artistName;
}

int Song::get_bitrate() {
	return m_bitrate;
}

std::string * Song::get_genre() {
	return &m_genre;
}

int Song::get_length() {
	return m_length;
}

std::string * Song::get_path() {
	return &m_path;
}

std::string * Song::get_song_title() {
	return &m_songTitle;
}

int Song::get_track_number() {
	return m_trackNumber;
}

int Song::get_year() {
	return m_year;
}
