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

#include "Configurations.h"
#include <fstream>

Configurations::Configurations() {
	initialize_configs();
	check_files_and_dir();
	read_config_file();
}

Configurations::~Configurations() {}

int Configurations::get_height() {
	return m_height;
}

std::string * Configurations::get_home_path() {
	return &m_homePath;
}

int Configurations::get_hpaned_pos() {
	return m_hpanedPos;
}

std::string * Configurations::get_library_file() {
	return &m_libraryFile;
}

std::string * Configurations::get_library_root() {
	return &m_libraryRoot;
}

std::vector<std::string> * Configurations::get_playlist() {
	return &m_playlist;
}

int Configurations::get_playlist_column(int index) {
	return m_playlistColumns[index];
}

int Configurations::get_songlist_column(int index) {
	return m_songlistColumns[index];
}

int Configurations::get_vpaned_pos() {
	return m_vpanedPos;
}

int Configurations::get_width() {
	return m_width;
}

void Configurations::set_height(int height) {
	m_height = height;
}

void Configurations::set_hpaned_pos(int hpanedPos) {
	m_hpanedPos = hpanedPos;
}

void Configurations::set_library_root(std::string libraryRoot) {
	m_libraryRoot = libraryRoot;
}

void Configurations::set_playlist_column(int val, int index) {
	m_playlistColumns[index] = val;
}

void Configurations::set_songlist_column(int val, int index) {
	m_songlistColumns[index] = val;
}

void Configurations::set_vpaned_pos(int vpanedPos) {
	m_vpanedPos = vpanedPos;
}

void Configurations::set_width(int width) {
	m_width = width;
}

void Configurations::write_config_file() {
	std::ofstream conf_file_stream(m_confFile.c_str());
	conf_file_stream << "# This file was generated automatically "
	                 << "by Germ.\n# DO NOT modify the contents "
	                 << "of this file!" << std::endl;
	conf_file_stream << "LIBRARY_ROOT=" << m_libraryRoot << std::endl;
	conf_file_stream << "HPANED_POS=" << m_hpanedPos << std::endl;
	conf_file_stream << "VPANED_POS=" << m_vpanedPos << std::endl;
	conf_file_stream << "WIDTH=" << m_width << std::endl;
	conf_file_stream << "HEIGHT=" << m_height << std::endl;
	conf_file_stream << "SONGLIST_COLS=";
	for (int i = 0; i < 7; ++i) {
		conf_file_stream << m_songlistColumns[i] << ",";
	}
	conf_file_stream << std::endl;
	conf_file_stream << "PLAYLIST_COLS=";
	for (int i = 0; i < 7; ++i) {
		conf_file_stream << m_playlistColumns[i] << ",";
	}
	conf_file_stream << std::endl;
	conf_file_stream << "PLAYLIST=everything below this line" << std::endl;
	for (unsigned int i = 0; i < m_playlist.size(); ++i) {
		conf_file_stream << "=" << m_playlist[i] << std::endl;
	}
	conf_file_stream.close();
}

void Configurations::check_files_and_dir() {
	DIR * dir = Opendir(m_confDir.c_str());
	if (dir == NULL) {
		int pid = Fork();
		if (pid == 0) {
			Execlp("mkdir", "mkdir", m_confDir.c_str());
		}
		else {
			Wait(NULL);
		}
	}
	else {
		Closedir(dir);
	}
}

void Configurations::initialize_configs() {
	m_width = 1024;
	m_height = 768;
	m_hpanedPos = m_width / 4;
	m_vpanedPos = (m_height / 12) * 5;
	for (int i = 0; i < 7; ++i) {
		m_songlistColumns[i] = ((m_width - m_hpanedPos) / 7) - 1;
		m_playlistColumns[i] = ((m_width - m_hpanedPos) / 4) - 2;
	}
	m_libraryRoot = "UNSPECIFIED";
	m_homePath = Getenv("HOME");
	m_confDir = m_homePath + "/.germ";
	m_confFile = m_confDir + "/germ.conf";
	m_libraryFile = m_confDir + "/library.dat";
}

void Configurations::read_config_file() {
	std::ifstream conf_file_stream(m_confFile.c_str());
	while (conf_file_stream.good()) {
		std::string line;
		getline(conf_file_stream, line);
		if (line.length() != 0 && line[0] != '#') {
			unsigned int loc = line.find('=');
			if (loc != std::string::npos) {
				std::string var = line.substr(0, loc);
				std::string val = line.substr(loc+1, std::string::npos);
				if (var == "SONGLIST_COLS") {
					loc = 0;
					unsigned int loc2 = val.find(',', loc);
					for (int i = 0; i < 7; ++i) {
						m_songlistColumns[i] = atoi(val.substr(loc, loc2).c_str());
						loc = loc2 + 1;
						loc2 = val.find(',', loc);
					}
				}
				else if (var == "PLAYLIST_COLS") {
					loc = 0;
					unsigned int loc2 = val.find(',', loc);
					for (int i = 0; i < 7; ++i) {
						m_playlistColumns[i] = atoi(val.substr(loc, loc2).c_str());
						loc = loc2 + 1;
						loc2 = val.find(',', loc);
					}
				}
				else if (var == "HPANED_POS") {
					m_hpanedPos = atoi(val.c_str());
				}
				else if (var == "VPANED_POS") {
					m_vpanedPos = atoi(val.c_str());
				}
				else if (var == "WIDTH") {
					m_width = atoi(val.c_str());
				}
				else if (var == "HEIGHT") {
					m_height = atoi(val.c_str());
				}
				else if (var == "LIBRARY_ROOT") {
					m_libraryRoot = val;
				}
				else if (var == "PLAYLIST") {
					continue;
				}
				else {
					m_playlist.push_back(val);
				}
			}
		}
	}
	conf_file_stream.close();
}
