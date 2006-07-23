#include "Album.h"
#include "Song.h"
#include <string>
#include <vector>

Album::Album(std::string _artist_name, std::string _album_title)
: artist_name(_artist_name),
  album_title(_album_title) {}

std::string * Album::get_artist_name() {
	return &artist_name;
}

std::string * Album::get_album_title() {
	return &album_title;
}

std::vector<Song> * Album::get_song_children() {
	return &song_children;
}

Album::~Album() {}
