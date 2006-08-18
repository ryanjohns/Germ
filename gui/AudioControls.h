/*******************************************************************************

Media_Player - Linux music library and player.
Copyright (C) 2006  Ryan Johns and Stephen McCarthy

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*******************************************************************************/

#ifndef AUDIOCONTROLS_H_
#define AUDIOCONTROLS_H_

#include "PlaylistView.h"
#include "../player/Player.h"
#include <gtkmm/buttonbox.h>
#include <gtkmm/tooltips.h>
#include <gtkmm/button.h>
#include <gtkmm/image.h>

class AudioControls : public Gtk::HButtonBox {
public:
	AudioControls(PlaylistView *, Player *);
	virtual ~AudioControls();

	virtual void on_button_next();
	virtual void on_button_playpause();
	virtual void on_button_previous();
	virtual void on_button_stop();

private:
	PlaylistView * m_playlistView;
	Player * m_player;
	Gtk::Button m_previous, m_playPause, m_stop, m_next;
	Gtk::Image m_previousIcon, m_playPauseIcon, m_stopIcon, m_nextIcon;
	Gtk::Tooltips m_tooltips;
};

#endif /*AUDIOCONTROLS_H_*/
