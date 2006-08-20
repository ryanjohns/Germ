/*******************************************************************************

Germ - The infectious music player.
Copyright (C) 2006  Ryan Johns and Stephen McCarthy

This file is part of Media_Player.
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

#ifndef CONFIGURATIONS_H_
#define CONFIGURATIONS_H_

#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <vector>

class Configurations {
public:
	Configurations();
	virtual ~Configurations();

	int get_height();
	std::string * get_home_path();
	int get_hpaned_pos();
	std::string * get_library_file();
	std::string * get_library_root();
	std::vector<std::string> * get_playlist();
	int get_playlist_column(int);
	int get_songlist_column(int);
	int get_vpaned_pos();
	int get_width();
	void set_height(int);
	void set_hpaned_pos(int);
	void set_library_root(std::string);
	void set_playlist_column(int, int);
	void set_songlist_column(int, int);
	void set_vpaned_pos(int);
	void set_width(int);
	void write_config_file();

private:
	int m_songlistColumns[7];
	int m_playlistColumns[7];
	int m_width;
	int m_height;
	int m_hpanedPos;
	int m_vpanedPos;
	std::string m_libraryRoot;
	std::string m_homePath;
	std::string m_confDir;
	std::string m_libraryFile;
	std::string m_confFile;
	std::vector<std::string> m_playlist;

	void check_files_and_dir();
	void initialize_configs();
	void read_config_file();

	// wrapper functions
	static inline pid_t Fork(void) {
		pid_t pid;
		if ((pid = fork()) == -1) {
			perror("fork() error!");
			exit(EXIT_FAILURE);
		}
		return pid;
	}

	static inline int Execlp(const char * file, const char * arg0,
			const char * arg1) {
		int rc;
		if ((rc = execlp(file, arg0, arg1, (char *)NULL)) == -1) {
			perror("execlp() error!");
			exit(EXIT_FAILURE);
		}
		return rc;
	}

	static inline pid_t Wait(int * status) {
		pid_t pid;
		if ((pid = wait(status)) == -1) {
			perror("wait() error!");
			exit(EXIT_FAILURE);
		}
		return pid;
	}

	static inline char * Getenv(const char * name) {
		char * ret;
		if ((ret = getenv(name)) == NULL) {
			perror("getenv() error!");
			exit(EXIT_FAILURE);
		}
		return ret;
	}

	static inline DIR * Opendir(const char * name) {
		DIR * dir;
		if ((dir = opendir(name)) == NULL) {
			if (errno != ENOENT) {
				perror("opendir() error!");
				exit(EXIT_FAILURE);
			}
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
};

#endif /*CONFIGURATIONS_H_*/
