#include "LibraryFileBuilder.h"
#include <iostream>
#include <taglib/fileref.h>
#include <taglib/tag.h>

LibraryFileBuilder::LibraryFileBuilder(std::string & path)
: library_file(path.c_str()) {}

LibraryFileBuilder::~LibraryFileBuilder() {
	library_file.close();
}

void LibraryFileBuilder::generate_library_file(std::string & path) {
	if (library_file.is_open()) {
		recurse_directory(path);
	}
	else {
		std::cout << "Could not open library_file stream!" << std::endl;
	}
}

void LibraryFileBuilder::recurse_directory(std::string & path) {
	DIR * dir;
	struct dirent * ent;
	dir = Opendir(path.c_str());
	while ((ent = Readdir(dir))) {
		std::string temp(ent->d_name);
		if (temp == "." || temp == "..") {
			continue;
		}
		std::string file_name(path);
		file_name += "/" + temp;
		struct stat buf;
		Stat(file_name.c_str(), &buf);
		if (S_ISDIR(buf.st_mode)) {
			recurse_directory(file_name);
		}
		else if (S_ISREG(buf.st_mode)) {
			process_file(file_name);
		}
	}
	Closedir(dir);
}

void LibraryFileBuilder::process_file(std::string & path) {
	TagLib::FileRef file(path.c_str());
	if (!file.isNull()) {
		TagLib::String artist_name = file.tag()->artist();
		TagLib::String album_title = file.tag()->album();
		TagLib::String song_title = file.tag()->title();
		TagLib::String genre = file.tag()->genre();
		TagLib::uint year = file.tag()->year();
		TagLib::uint track_number = file.tag()->track();
		TagLib::uint length = file.audioProperties()->length();
		TagLib::uint bitrate = file.audioProperties()->bitrate();
		library_file << artist_name << SEPARATOR
		             << album_title << SEPARATOR
		             << song_title << SEPARATOR
		             << genre << SEPARATOR
		             << year << SEPARATOR
		             << track_number << SEPARATOR
		             << length << SEPARATOR
		             << bitrate << SEPARATOR
		             << path << std::endl;
	}
}
