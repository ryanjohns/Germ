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

	sigc::signal<void, bool> signal_song_request;
	sigc::signal<void, gint64, gint64> signal_update_seek_bar;
	sigc::signal<void, Glib::ustring> signal_update_window_title;

private:
	gboolean m_songLoaded;
	GstElement * m_pipeline;
	std::string m_nextSong;
	std::string m_backupSong;
	bool m_playing;

	GstElement * get_pipeline();
	void send_song_request(bool);
	void send_update_seek_bar(gint64, gint64);
	void send_update_window_title(Glib::ustring);

	static gboolean bus_watch(GstBus *, GstMessage *, gpointer);
	static gboolean song_position_watch(gpointer);
};

#endif /*PLAYER_H_*/
