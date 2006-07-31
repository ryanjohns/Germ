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
