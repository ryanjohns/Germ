#include "Configurations.h"
#include <iostream>

Configurations::Configurations() {
	initialize_configs();
	check_files_and_dir();
	read_config_file();
}

Configurations::~Configurations() {}

void Configurations::check_files_and_dir() {
	std::string path = home_path + "/.media_player";
	DIR * dir = Opendir(path.c_str());
	if (dir == NULL) {
		int pid = Fork();
		if (pid == 0) {
			Execlp("mkdir", "mkdir", path.c_str());
		}
		else {
			Wait(NULL);
		}
	}
	else {
		Closedir(dir);
	}
	path += "/mp.conf";
	std::ifstream tmp(path.c_str());
	tmp.close();
}

void Configurations::read_config_file() {
	std::string path = home_path + "/.media_player/mp.conf";
	config_file.open(path.c_str(), std::ios_base::in);
	std::string line;
	while (config_file.good()) {
		getline(config_file, line);
		if (line.length() != 0 && line[0] != '#') {
			unsigned int loc = line.find('=');
			if (loc != std::string::npos) {
				std::string var = line.substr(0, loc);
				std::string val = line.substr(loc+1, std::string::npos);
				if (var == "SONGLIST_COLS") {
					loc = 0;
					unsigned int loc2 = val.find(',', loc);
					for (int i = 0; i < 7; ++i) {
						songlist_columns[i] = atoi(val.substr(loc, loc2).c_str());
						loc = loc2 + 1;
						loc2 = val.find(',', loc);
					}
				}
				else if (var == "PLAYLIST_COLS") {
					loc = 0;
					unsigned int loc2 = val.find(',', loc);
					for (int i = 0; i < 7; ++i) {
						playlist_columns[i] = atoi(val.substr(loc, loc2).c_str());
						loc = loc2 + 1;
						loc2 = val.find(',', loc);
					}
				}
				else if (var == "HPANED_POS") {
					hpaned_pos = atoi(val.c_str());
				}
				else if (var == "VPANED_POS") {
					vpaned_pos = atoi(val.c_str());
				}
				else if (var == "WIDTH") {
					width = atoi(val.c_str());
				}
				else if (var == "HEIGHT") {
					height = atoi(val.c_str());
				}
				else if (var == "LIBRARY_PATH") {
					library_path = val;
				}
				else if (var == "PLAYLIST") {
					continue;
				}
				else {
					playlist.push_back(val);
				}
			}
		}
	}
	config_file.close();
}

void Configurations::write_config_file() {
	std::string path = home_path + "/.media_player/mp.conf";
	config_file.open(path.c_str(), std::ios_base::out | std::ios_base::trunc);
	config_file << "# This file was generated automatically by media_player.\n"
	            << "# DO NOT modify the contents of this file!" << std::endl;
	config_file << "LIBRARY_PATH=" << library_path << std::endl;
	config_file << "HPANED_POS=" << hpaned_pos << std::endl;
	config_file << "VPANED_POS=" << vpaned_pos << std::endl;
	config_file << "WIDTH=" << width << std::endl;
	config_file << "HEIGHT=" << height << std::endl;
	config_file << "SONGLIST_COLS=";
	for (int i = 0; i < 7; ++i) {
		config_file << songlist_columns[i] << ",";
	}
	config_file << std::endl;
	config_file << "PLAYLIST_COLS=";
	for (int i = 0; i < 7; ++i) {
		config_file << playlist_columns[i] << ",";
	}
	config_file << std::endl;
	config_file << "PLAYLIST=everything below this line" << std::endl;
	for (unsigned int i = 0; i < playlist.size(); ++i) {
		config_file << "=" << playlist[i] << std::endl;
	}
	config_file.close();
}

int Configurations::get_songlist_column(int index) {
	return songlist_columns[index];
}

int Configurations::get_playlist_column(int index) {
	return playlist_columns[index];
}

int Configurations::get_hpaned_pos() {
	return hpaned_pos;
}

int Configurations::get_vpaned_pos() {
	return vpaned_pos;
}

int Configurations::get_width() {
	return width;
}

int Configurations::get_height() {
	return height;
}

std::string * Configurations::get_library_path() {
	return &library_path;
}

std::string * Configurations::get_home_path() {
	return &home_path;
}

std::vector<std::string> * Configurations::get_playlist() {
	return &playlist;
}

void Configurations::set_songlist_column(int val, int index) {
	songlist_columns[index] = val;
}

void Configurations::set_playlist_column(int val, int index) {
	playlist_columns[index] = val;
}

void Configurations::set_hpaned_pos(int _hpaned_pos) {
	hpaned_pos = _hpaned_pos;
}

void Configurations::set_vpaned_pos(int _vpaned_pos) {
	vpaned_pos = _vpaned_pos;
}

void Configurations::set_width(int _width) {
	width = _width;
}

void Configurations::set_height(int _height) {
	height = _height;
}

void Configurations::set_library_path(std::string _library_path) {
	library_path = _library_path;
}

void Configurations::initialize_configs() {
	width = 1024;
	height = 768;
	hpaned_pos = width / 4;
	vpaned_pos = (height / 12) * 5;
	for (int i = 0; i < 7; ++i) {
		songlist_columns[i] = ((width - hpaned_pos) / 7) - 1;
		playlist_columns[i] = ((width - hpaned_pos) / 4) - 2;
	}
	library_path = "UNSPECIFIED";
	home_path = Getenv("HOME");
}
