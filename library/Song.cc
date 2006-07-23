#include "Song.h"
#include <string>

Song::Song(std::string _artist_name, std::string _album_title,
	std::string _song_title, std::string _genre, int _year,
	int _track_number, int _length, int _bitrate, std::string _path) 
: artist_name(_artist_name),
  album_title(_album_title),
  song_title(_song_title),
  genre(_genre),
  year(_year),
  track_number(_track_number),
  length(_length),
  bitrate(_bitrate),
  path(_path) {}

Song::~Song() {}

std::string * Song::get_artist_name() {
	return &artist_name;
}

std::string * Song::get_album_title() {
	return &album_title;
}

std::string * Song::get_song_title() {
	return &song_title;
}

std::string * Song::get_genre() {
	return &genre;
}

int Song::get_year() {
	return year;
}

int Song::get_track_number() {
	return track_number;
}

int Song::get_length() {
	return length;
}

int Song::get_bitrate() {
	return bitrate;
}

std::string * Song::get_path() {
	return &path;
}
