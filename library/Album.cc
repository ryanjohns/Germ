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
