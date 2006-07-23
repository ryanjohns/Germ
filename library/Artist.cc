#include "Artist.h"
#include "Album.h"
#include <string>
#include <vector>

Artist::Artist(std::string _artist_name)
: artist_name(_artist_name) {}

std::string * Artist::get_artist_name() {
	return &artist_name;
}

std::vector<Album> * Artist::get_album_children() {
	return &album_children;
}

Artist::~Artist() {}
