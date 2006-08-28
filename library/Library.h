/*******************************************************************************

Germ - The infectious music player.
Copyright (C) 2006  Ryan Johns and Stephen McCarthy

This file is part of Germ.
Germ is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

Germ is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with Germ; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*******************************************************************************/

#ifndef LIBRARY_H_
#define LIBRARY_H_

#include "Artist.h"
#include "Album.h"
#include "Song.h"
#include <string>
#include <vector>

#define SEPARATOR (char)6

class Library {
public:
	Library();
	virtual ~Library();

	void build_library(std::string &);
	void clear_library();
	Song * find_song_node(std::string &, std::string &, std::string &);
	std::vector<Artist> * get_library_root();

private:
	std::vector<Artist> m_libraryRoot;

	void create_song(std::string &, std::string &,
		std::string &, std::string &, int, int, int,
		int, std::string &, Album *);
	Album * handle_album(std::string &, Artist *);
	Artist * handle_artist(std::string &);
	void print_library();
};

#endif /*LIBRARY_H_*/
