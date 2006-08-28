/*******************************************************************************

Germ - The infectious music player.
Copyright (C) 2006  Ryan Johns and Stephen McCarthy

This file is part of Germ.
Germ is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

Germ is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with Germ; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*******************************************************************************/

#ifndef LIBRARYFILEBUILDER_H_
#define LIBRARYFILEBUILDER_H_

#include <fstream>
#include <string>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

#define SEPARATOR (char)6

class LibraryFileBuilder	{
public:
	LibraryFileBuilder();
	virtual ~LibraryFileBuilder();

	void generate_library_file(std::string &, std::string &);

private:
	std::ofstream m_libraryFileStream;

	void process_file(std::string &);
	void recurse_directory(std::string &);

	// wrapper functions
	static inline DIR * Opendir(const char * name) {
		DIR * dir;
		if ((dir = opendir(name)) == NULL) {
			perror("opendir() error!");
			exit(EXIT_FAILURE);
		}
		return dir;
	}

	static inline int Closedir(DIR * dir) {
		int rc;
		if ((rc = closedir(dir)) == -1) {
			perror("closedir() error!");
			exit(EXIT_FAILURE);
		}
		return rc;
	}

	static inline struct dirent * Readdir(DIR * dir) {
		struct dirent * ret;
		if ((ret = readdir(dir)) == NULL) {
			if (errno == EBADF) {
				perror("readdir() error!");
				exit(EXIT_FAILURE);
			}
		}
		return ret;
	}

	static inline int Stat(const char * path, struct stat * buf) {
		int rc;
		if ((rc = stat(path, buf)) == -1) {
			perror("stat() error!");
			exit(EXIT_FAILURE);
		}
		return rc;
	}
};

#endif /*LIBRARYFILEBUILDER_H_*/
