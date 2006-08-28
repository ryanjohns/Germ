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

#ifndef ARTIST_H_
#define ARTIST_H_

#include "Album.h"
#include <vector>
#include <string>

class Artist {
public:
	Artist(std::string);
	virtual ~Artist();

	std::vector<Album> * get_album_children();
	std::string * get_artist_name();

private:
	std::string m_artistName;
	std::vector<Album> m_albumChildren;
};

#endif /*ARTIST_H_*/
