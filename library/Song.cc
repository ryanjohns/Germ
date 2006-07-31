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
