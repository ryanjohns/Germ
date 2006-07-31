#ifndef ALBUM_H_
#define ALBUM_H_

#include "Song.h"
#include <vector>
#include <string>

class Album {
public:
	Album(std::string, std::string);
	virtual ~Album();

	std::string * get_album_title();
	std::string * get_artist_name();
	std::vector<Song> * get_song_children();

private:
	std::string m_artistName;
	std::string m_albumTitle;
	std::vector<Song> m_songChildren;
};

#endif /*ALBUM_H_*/
