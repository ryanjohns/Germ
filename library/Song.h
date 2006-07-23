#ifndef SONG_H_
#define SONG_H_

#include <string>

class Song {
public:
	Song(std::string, std::string, std::string,
		std::string, int, int, int, int, std::string);
	virtual ~Song();
	std::string * get_artist_name();
	std::string * get_album_title();
	std::string * get_song_title();
	std::string * get_genre();
	int get_year();
	int get_track_number();
	int get_length();
	int get_bitrate();
	std::string * get_path();
	
private:
	std::string artist_name;
	std::string album_title;
	std::string song_title;
	std::string genre;
	int year;
	int track_number;
	int length;
	int bitrate;
	std::string path;
};

#endif /*SONG_H_*/
