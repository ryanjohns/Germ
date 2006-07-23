#ifndef ARTIST_H_
#define ARTIST_H_

#include "Album.h"
#include <vector>
#include <string>

class Artist {
public:
	Artist(std::string);
	virtual ~Artist();
	std::string * get_artist_name();
	std::vector<Album> * get_album_children();
	
private:
	std::string artist_name;
	std::vector<Album> album_children;
};

#endif /*ARTIST_H_*/
