#include "Library.h"
#include "Artist.h"
#include "Album.h"
#include "Song.h"
#include <fstream>
#include <iostream>
#include <vector>

Library::Library() {}

Library::~Library() {}

void Library::build_library(std::string & path) {
	std::ifstream library_file(path.c_str());
	while (library_file.good()) {
		std::string current_line;
		getline(library_file, current_line);
		if (current_line.length() == 0) {
			continue;
		}
		int delim_loc1 = 0;
		int delim_loc2 = current_line.find(SEPARATOR);
		std::string artist_name = current_line.substr(delim_loc1, delim_loc2);
		
		delim_loc1 = delim_loc2+1;
		delim_loc2 = current_line.find(SEPARATOR, delim_loc1);
		std::string album_title = current_line.substr(delim_loc1,
				delim_loc2-delim_loc1);
		
		delim_loc1 = delim_loc2+1;
		delim_loc2 = current_line.find(SEPARATOR, delim_loc1);
		std::string song_title = current_line.substr(delim_loc1,
				delim_loc2-delim_loc1);
		
		delim_loc1 = delim_loc2+1;
		delim_loc2 = current_line.find(SEPARATOR, delim_loc1);
		std::string genre = current_line.substr(delim_loc1,
			delim_loc2-delim_loc1);
		
		delim_loc1 = delim_loc2+1;
		delim_loc2 = current_line.find(SEPARATOR, delim_loc1);
		int year = atoi(current_line.substr(delim_loc1,
			delim_loc2-delim_loc1).c_str());
		
		delim_loc1 = delim_loc2+1;
		delim_loc2 = current_line.find(SEPARATOR, delim_loc1);
		int track_number = atoi(current_line.substr(delim_loc1,
			delim_loc2-delim_loc1).c_str());
		
		delim_loc1 = delim_loc2+1;
		delim_loc2 = current_line.find(SEPARATOR, delim_loc1);
		int length = atoi(current_line.substr(delim_loc1,
			delim_loc2-delim_loc1).c_str());
		
		delim_loc1 = delim_loc2+1;
		delim_loc2 = current_line.find(SEPARATOR, delim_loc1);
		int bitrate = atoi(current_line.substr(delim_loc1,
			delim_loc2-delim_loc1).c_str());
		
		delim_loc1 = delim_loc2+1;
		std::string path = current_line.substr(delim_loc1);
		
		Artist * artist_node = handle_artist(artist_name);
		Album * album_node = handle_album(album_title, artist_node);
		create_song(artist_name, album_title, song_title, genre, year,
			track_number, length, bitrate, path, album_node);
	}
	library_file.close();
}

void Library::print_library() {
	std::vector<Artist>::iterator it;
	for (it = library_root.begin(); it != library_root.end(); ++it) {
		std::cout << *((*it).get_artist_name()) << std::endl;
		std::vector<Album>::iterator it2;
		for (it2 = (*it).get_album_children()->begin();
				it2 != (*it).get_album_children()->end(); ++it2) {
			std::cout << "  |->" << *((*it2).get_album_title()) << std::endl;
			std::vector<Song>::iterator it3;
			for (it3 = (*it2).get_song_children()->begin();
					it3 != (*it2).get_song_children()->end(); ++it3) {
				std::cout << "     |->" << *((*it3).get_song_title())
				          << std::endl;
			}
		}
	}
}

std::vector<Artist> * Library::get_library_root() {
	return &library_root;
}

Artist * Library::handle_artist(std::string & artist_name) {
	std::vector<Artist>::iterator it;
	for (it = library_root.begin(); it != library_root.end(); ++it) {
		if (artist_name == *((*it).get_artist_name())) {
			return &*it;
		}
	}
	Artist artist(artist_name);
	library_root.push_back(artist);
	return &(library_root.back());
}

Album * Library::handle_album(std::string & album_title, Artist * artist_node) {
	std::vector<Album>::iterator it;
	for (it = artist_node->get_album_children()->begin();
			it != artist_node->get_album_children()->end(); ++it) {
		if (album_title == *((*it).get_album_title())) {
			return &*it;
		}
	}
	Album album(*artist_node->get_artist_name(), album_title);
	artist_node->get_album_children()->push_back(album);
	return &(artist_node->get_album_children()->back());
}

void Library::create_song(std::string & artist_name, std::string & album_title,
		std::string & song_title, std::string & genre, int year,
		int track_number, int length, int bitrate, std::string & path,
		Album * album_node) {
	Song song(artist_name, album_title, song_title, genre, year,
		track_number, length, bitrate, path);
	album_node->get_song_children()->push_back(song);
}

void Library::clear_library() {
	library_root.clear();
}

Song * Library::find_song_node(std::string & art, std::string & alb,
		std::string & title) {
	std::vector<Artist>::iterator it;
	for (it = library_root.begin(); it != library_root.end(); ++it) {
		Artist artist = *it;
		if (*artist.get_artist_name() == art) {
			std::cout << "found artist " << art << std::endl;
			std::vector<Album> * albums = artist.get_album_children();
			std::vector<Album>::iterator it2;
			for (it2 = albums->begin(); it2 != albums->end(); ++it2) {
				Album album = *it2;
				if (*album.get_album_title() == alb) {
					std::cout << "found album " << alb << std::endl;
					std::vector<Song> * songs = album.get_song_children();
					std::vector<Song>::iterator it3;
					for (it3 = songs->begin(); it3 != songs->end(); ++it3) {
						Song song = *it3;
						if (*song.get_song_title() == title) {
							std::cout << "found song " << title << std::endl;
							return &(*it3);
						}
					}
				}
			}
		}
	}
	return NULL;
}
