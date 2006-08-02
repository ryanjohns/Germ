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
