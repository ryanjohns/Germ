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

#include "AudioControls.h"

AudioControls::AudioControls(PlaylistView * playlistView, Player * player)
: m_playlistView(playlistView),
  m_player(player),
  m_previousIcon("/usr/local/share/pixmaps/media_player/previous.xpm"),
  m_playPauseIcon("/usr/local/share/pixmaps/media_player/play_pause.xpm"),
  m_stopIcon("/usr/local/share/pixmaps/media_player/stop.xpm"),
  m_nextIcon("/usr/local/share/pixmaps/media_player/next.xpm") {
	set_layout(Gtk::BUTTONBOX_START);

	pack_start(m_previous);
	pack_start(m_playPause);
	pack_start(m_stop);
	pack_start(m_next);

	// add icons and tooltips to buttons
	m_previous.set_image(m_previousIcon);
	m_playPause.set_image(m_playPauseIcon);
	m_stop.set_image(m_stopIcon);
	m_next.set_image(m_nextIcon);
	m_tooltips.set_tip(m_previous, "Previous");
	m_tooltips.set_tip(m_playPause, "Play/Pause");
	m_tooltips.set_tip(m_stop, "Stop");
	m_tooltips.set_tip(m_next, "Next");

	// connect button signals
	m_previous.signal_clicked().connect(
		sigc::mem_fun(*this, &AudioControls::on_button_previous)
	);
	m_playPause.signal_clicked().connect(
		sigc::mem_fun(*this, &AudioControls::on_button_playpause)
	);
	m_stop.signal_clicked().connect(
		sigc::mem_fun(*this, &AudioControls::on_button_stop)
	);
	m_next.signal_clicked().connect(
		sigc::mem_fun(*this, &AudioControls::on_button_next)
	);
}

AudioControls::~AudioControls() {}

void AudioControls::on_button_next() {
	m_player->next();
}

void AudioControls::on_button_playpause() {
	if (m_player->is_playing())
		m_player->pause();
	else if (m_player->is_song_loaded())
		m_player->play();
	else
		m_playlistView->play_new_list();
}

void AudioControls::on_button_previous() {
	m_player->previous();
}

void AudioControls::on_button_stop() {
	m_player->stop();
}
