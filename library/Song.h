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

#ifndef SONG_H_
#define SONG_H_

#include <string>

class Song {
public:
	Song(std::string, std::string, std::string,
		std::string, int, int, int, int, std::string);
	virtual ~Song();

	std::string * get_album_title();
	std::string * get_artist_name();
	int get_bitrate();
	std::string * get_genre();
	int get_length();
	std::string * get_path();
	std::string * get_song_title();
	int get_track_number();
	int get_year();

private:
	std::string m_artistName;
	std::string m_albumTitle;
	std::string m_songTitle;
	std::string m_genre;
	int m_year;
	int m_trackNumber;
	int m_length;
	int m_bitrate;
	std::string m_path;
};

#endif /*SONG_H_*/
