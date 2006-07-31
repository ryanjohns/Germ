#include "Artist.h"
#include "Album.h"
#include <string>
#include <vector>

Artist::Artist(std::string artistName)
: m_artistName(artistName) {}

Artist::~Artist() {}

std::vector<Album> * Artist::get_album_children() {
	return &m_albumChildren;
}

std::string * Artist::get_artist_name() {
	return &m_artistName;
}
