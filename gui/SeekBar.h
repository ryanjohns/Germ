#ifndef SEEKBAR_H_
#define SEEKBAR_H_

#include "../player/Player.h"
#include <gtkmm/scale.h>

class SeekBar : public Gtk::HScale {
public:
	SeekBar(Player *);
	virtual ~SeekBar();

protected:
	virtual Glib::ustring on_format_value(double);
	virtual void on_move_slider(Gtk::ScrollType);
	virtual void on_update_seek_bar(gint64, gint64);

private:
	Player * m_player;
	gint64 m_length;
	sigc::connection m_seekBarConnection;

	Glib::ustring format_time(gint64);
};

#endif /*SEEKBAR_H_*/
