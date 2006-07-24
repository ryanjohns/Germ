#include "Configurations.h"
#include <fstream>

Configurations::Configurations() {
	initialize_configs();
	check_files_and_dir();
	read_config_file();
}

Configurations::~Configurations() {}

void Configurations::check_files_and_dir() {
	DIR * dir = Opendir(conf_dir.c_str());
	if (dir == NULL) {
		int pid = Fork();
		if (pid == 0) {
			Execlp("mkdir", "mkdir", conf_dir.c_str());
		}
		else {
			Wait(NULL);
		}
	}
	else {
		Closedir(dir);
	}
}

void Configurations::read_config_file() {
	std::ifstream conf_file_stream(conf_file.c_str());
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
				else if (var == "LIBRARY_ROOT") {
					library_root = val;
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
	conf_file_stream.close();
}

void Configurations::write_config_file() {
	std::ofstream conf_file_stream(conf_file.c_str());
	conf_file_stream << "# This file was generated automatically "
	                 << "by media_player.\n# DO NOT modify the "
	                 << "contents of this file!" << std::endl;
	conf_file_stream << "LIBRARY_ROOT=" << library_root << std::endl;
	conf_file_stream << "HPANED_POS=" << hpaned_pos << std::endl;
	conf_file_stream << "VPANED_POS=" << vpaned_pos << std::endl;
	conf_file_stream << "WIDTH=" << width << std::endl;
	conf_file_stream << "HEIGHT=" << height << std::endl;
	conf_file_stream << "SONGLIST_COLS=";
	for (int i = 0; i < 7; ++i) {
		conf_file_stream << songlist_columns[i] << ",";
	}
	conf_file_stream << std::endl;
	conf_file_stream << "PLAYLIST_COLS=";
	for (int i = 0; i < 7; ++i) {
		conf_file_stream << playlist_columns[i] << ",";
	}
	conf_file_stream << std::endl;
	conf_file_stream << "PLAYLIST=everything below this line" << std::endl;
	for (unsigned int i = 0; i < playlist.size(); ++i) {
		conf_file_stream << "=" << playlist[i] << std::endl;
	}
	conf_file_stream.close();
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

std::string * Configurations::get_library_root() {
	return &library_root;
}

std::string * Configurations::get_home_path() {
	return &home_path;
}

std::string * Configurations::get_library_file() {
	return &library_file;
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

void Configurations::set_library_root(std::string _library_root) {
	library_root = _library_root;
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
	library_root = "UNSPECIFIED";
	home_path = Getenv("HOME");
	conf_dir = home_path + "/.media_player";
	conf_file = conf_dir + "/mp.conf";
	library_file = conf_dir + "/library.dat";
}
