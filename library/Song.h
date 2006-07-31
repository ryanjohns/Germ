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
