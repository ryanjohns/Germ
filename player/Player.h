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

#ifndef PLAYER_H_
#define PLAYER_H_

#include "../library/Song.h"
#include <gst/gst.h>
#include <string>
#include <sigc++/sigc++.h>
#include <glibmm/ustring.h>

class Player {
public:
	Player();
	virtual ~Player();

	bool is_playing();
	bool is_song_loaded();
	void next();
	void pause();
	void play();
	void play(Song *);
	void previous();
	void seek(double);
	void set_next_song(Song *);
	void set_song_loaded(bool);
	void stop();

	sigc::signal<void, Glib::ustring> signal_song_length;
	sigc::signal<void, bool> signal_song_request;
	sigc::signal<void, gint64, gint64> signal_update_seek_bar;
	sigc::signal<void, Glib::ustring> signal_update_window_title;

private:
	gboolean m_songLoaded;
	GstElement * m_pipeline;
	std::string m_nextSong;
	std::string m_backupSong;
	bool m_playing;

	Glib::ustring format_time(int);
	GstElement * get_pipeline();
	void send_song_length(Glib::ustring);
	void send_song_request(bool);
	void send_update_seek_bar(gint64, gint64);
	void send_update_window_title(Glib::ustring);

	static gboolean bus_watch(GstBus *, GstMessage *, gpointer);
	static gboolean song_position_watch(gpointer);
};

#endif /*PLAYER_H_*/
