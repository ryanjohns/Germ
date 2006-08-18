/*******************************************************************************

Media_Player - Linux music library and player.
Copyright (C) 2006  Ryan Johns and Stephen McCarthy

This file is part of Media_Player.
Media_Player is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

Media_Player is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with Media_Player; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*******************************************************************************/

#include "LibraryFileBuilder.h"
#include <iostream>
#include <taglib/fileref.h>
#include <taglib/tag.h>

LibraryFileBuilder::LibraryFileBuilder() {}

LibraryFileBuilder::~LibraryFileBuilder() {}

void LibraryFileBuilder::generate_library_file(std::string & library_file,
			std::string & root) {
	m_libraryFileStream.open(library_file.c_str());
	recurse_directory(root);
	m_libraryFileStream.close();
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
		m_libraryFileStream << artist_name << SEPARATOR
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
