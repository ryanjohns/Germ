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

#ifndef SEEKBAR_H_
#define SEEKBAR_H_

#include "../player/Player.h"
#include <gtkmm/scale.h>

class SeekBar : public Gtk::HScale {
public:
	SeekBar(Player *);
	virtual ~SeekBar();

protected:
	virtual bool on_button_press_event(GdkEventButton *);
	virtual bool on_button_release_event(GdkEventButton *);
	virtual Glib::ustring on_format_value(double);
	virtual void on_song_length(Glib::ustring);
	virtual void on_update_seek_bar(gint64, gint64);
	virtual void on_value_changed();

private:
	Player * m_player;
	gint64 m_length;
	Glib::ustring m_strLen;
	sigc::connection m_seekBarConnection;

	Glib::ustring format_time(gint64);
};

#endif /*SEEKBAR_H_*/
