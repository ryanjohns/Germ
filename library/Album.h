#ifndef ALBUM_H_
#define ALBUM_H_

#include "Song.h"
#include <vector>
#include <string>

class Album {
public:
	Album(std::string, std::string);
	virtual ~Album();
	std::string * get_artist_name();
	std::string * get_album_title();
	std::vector<Song> * get_song_children();
	
private:
	std::string artist_name;
	std::string album_title;
	std::vector<Song> song_children;
};

#endif /*ALBUM_H_*/
