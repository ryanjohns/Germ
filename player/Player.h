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

	virtual void signal_song_request(bool);
	virtual void signal_update_window_title(Glib::ustring);

	void setNextSong(Song *);
	void play();
	void play(Song *);
	void pause();
	void next();
	void stop();
	void previous();
	void seek(double);
	bool isPlaying();
	bool isSongLoaded();

	sigc::signal<void, bool> m_signal_song_request;
	sigc::signal<void, Glib::ustring> m_signal_update_window_title;

private:
	static gboolean bus_watch(GstBus *, GstMessage *, gpointer);
	gboolean m_isSongLoaded;
	GstElement * pipeline;
	std::string m_nextSong;
	std::string m_backupSong;
	bool m_playing;
};

#endif /*PLAYER_H_*/
