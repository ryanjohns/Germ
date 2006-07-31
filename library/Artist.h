#ifndef ARTIST_H_
#define ARTIST_H_

#include "Album.h"
#include <vector>
#include <string>

class Artist {
public:
	Artist(std::string);
	virtual ~Artist();

	std::vector<Album> * get_album_children();
	std::string * get_artist_name();

private:
	std::string m_artistName;
	std::vector<Album> m_albumChildren;
};

#endif /*ARTIST_H_*/
