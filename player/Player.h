#ifndef PLAYER_H_
#define PLAYER_H_

#include <gst/gst.h>
#include <string>
#include <sigc++/sigc++.h>
#include "../library/Song.h"

class Player {
public:
	Player();
	virtual ~Player();
	
	virtual void signal_song_request(bool);
	
	void setNextSong(Song *);
	void play();
	void play(Song *);
	void play(char *);
	void pause();
	void next();
	void stop();
	void previous();
	void seek(double);
	bool isPlaying();
	bool isSongLoaded();
	
	sigc::signal<void, bool> m_signal_song_request;

private:
	static gboolean bus_watch(GstBus *, GstMessage *, gpointer);
	gboolean m_isSongLoaded;
	GstElement * pipeline;
	std::string m_nextSong;
	std::string m_backupSong;
	bool m_playing;
};

#endif /*PLAYER_H_*/
