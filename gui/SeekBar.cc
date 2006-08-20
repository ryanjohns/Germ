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

#include "SeekBar.h"
#include <sstream>

SeekBar::SeekBar(Player * player)
: m_player(player) {
	m_length = 0;
	m_strLen = "0:00";

	set_range(0.0, 1.0);
	set_increments(0.0001, 0.1);
	set_digits(4);
	set_update_policy(Gtk::UPDATE_DISCONTINUOUS);
	set_value_pos(Gtk::POS_RIGHT);

	m_player->signal_song_length.connect(
		sigc::mem_fun(*this, &SeekBar::on_song_length)
	);
	m_seekBarConnection = m_player->signal_update_seek_bar.connect(
		sigc::mem_fun(*this, &SeekBar::on_update_seek_bar)
	);
}

SeekBar::~SeekBar() {}

bool SeekBar::on_button_press_event(GdkEventButton * event) {
	m_seekBarConnection.disconnect();
	return Gtk::HScale::on_button_press_event(event);
}

bool SeekBar::on_button_release_event(GdkEventButton * event) {
	Gtk::HScale::on_button_release_event(event);
	m_seekBarConnection = m_player->signal_update_seek_bar.connect(
		sigc::mem_fun(*this, &SeekBar::on_update_seek_bar)
	);
	return true;
}

Glib::ustring SeekBar::on_format_value(double val) {
	Glib::ustring str_value;
	str_value = format_time(gint64(m_length * val));
	str_value += "/";
	str_value += m_strLen;
	return str_value;
}

void SeekBar::on_song_length(Glib::ustring str) {
	m_strLen = str;
}

void SeekBar::on_update_seek_bar(gint64 pos, gint64 len) {
	m_length = len;
	double dpos = pos;
	double dlen = len;
	if (pos == 0 && len == 0)
		set_value(0);
	else
		set_value(dpos/dlen);
}

void SeekBar::on_value_changed() {
	if (!m_seekBarConnection.connected()) {
		m_player->seek(get_value());
	}
}

Glib::ustring SeekBar::format_time(gint64 len) {
	if (len == 0)
		return "0:00";
	std::ostringstream oss;
	int secs = len / 1000000000;
	int minutes = secs / 60;
	int seconds = secs - minutes * 60;
	oss << minutes << ":";
	if (seconds < 10) {
		oss << 0 << seconds;
	}
	else {
		oss << seconds;
	}
	return oss.str();
}
