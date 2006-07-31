#ifndef LIBRARY_H_
#define LIBRARY_H_

#include "Artist.h"
#include "Album.h"
#include "Song.h"
#include <string>
#include <vector>

#define SEPARATOR (char)6

class Library {
public:
	Library();
	virtual ~Library();

	void build_library(std::string &);
	void clear_library();
	Song * find_song_node(std::string &, std::string &, std::string &);
	std::vector<Artist> * get_library_root();

private:
	std::vector<Artist> m_libraryRoot;

	void create_song(std::string &, std::string &,
		std::string &, std::string &, int, int, int,
		int, std::string &, Album *);
	Album * handle_album(std::string &, Artist *);
	Artist * handle_artist(std::string &);
	void print_library();
};

#endif /*LIBRARY_H_*/
