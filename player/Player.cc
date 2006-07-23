#include <gst/gst.h>
#include <string>
#include <iostream>
#include "Player.h"

Player::Player(){
	m_playing = false;
	m_isSongLoaded = false;
	m_nextSong.clear();
	m_backupSong.clear();
	
	gst_init(NULL, NULL);
	
	pipeline = gst_element_factory_make("playbin", "play");
	
	gst_bus_add_watch(gst_pipeline_get_bus(GST_PIPELINE(pipeline)),
		     Player::bus_watch, (gpointer)this);
}

Player::~Player(){
	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(GST_OBJECT(pipeline));
}

void Player::signal_song_request(bool previous) {
	m_signal_song_request.emit(previous);
}

void Player::setNextSong(Song * song) {
	if (song == NULL) {
		m_nextSong.clear();
	}
	else {
		m_nextSong = *song->get_path();
		m_backupSong = m_nextSong;
	}
}

void Player::play() {
	if (m_nextSong.empty()) {
		m_nextSong = m_backupSong;
	}
	if (!m_isSongLoaded && !m_nextSong.empty()) {
		std::string uri = "file://" + m_nextSong;
		std::cout << "Adding: " << uri << std::endl;
		g_object_set(G_OBJECT(pipeline), "uri", uri.c_str() , NULL);
		m_isSongLoaded = true;
	}
	if (m_isSongLoaded) {
		gst_element_set_state(pipeline, GST_STATE_PLAYING);
		m_nextSong.clear();
		m_playing = true;
	}
}

void Player::play(Song * song) {
	setNextSong(song);
	m_isSongLoaded = false;
	play();
}

void Player::play(char * s) {
	m_nextSong = std::string(s);
	m_backupSong = m_nextSong;
	m_isSongLoaded = false;
	play();
}

void Player::pause() {
	gst_element_set_state(pipeline, GST_STATE_PAUSED);
	m_playing = false;
}

void Player::next() {
	if (m_nextSong.empty()) {
		signal_song_request(false);
	}
	if (!m_nextSong.empty()) {
		m_isSongLoaded = false;
		stop();
		play();
		m_nextSong.clear();
	}
}

void Player::stop() {
	gst_element_set_state(pipeline, GST_STATE_NULL);
	m_playing = false;
}

void Player::previous() {
	signal_song_request(true);
	if (!m_nextSong.empty()) {
		m_isSongLoaded = false;
		stop();
		play();
		m_nextSong.clear();
	}
}

void Player::seek(double p) {
	GstFormat fmt = GST_FORMAT_TIME;
	gint64 len;
	gst_element_query_duration(pipeline, &fmt, &len);
	gst_element_seek(pipeline, 1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH,
		GST_SEEK_TYPE_SET, (gint64)(p * len), GST_SEEK_TYPE_NONE,
		GST_CLOCK_TIME_NONE);
}

gboolean Player::bus_watch(GstBus *, GstMessage * msg, gpointer data) {
	
	Player * player = (Player *)data;

	switch (GST_MESSAGE_TYPE(msg)) {
		case GST_MESSAGE_EOS: {
			g_print("End-of-stream\n");
			player->next();
			break;
		}
		case GST_MESSAGE_ERROR: {
			gchar * debug;
			GError * err;

			gst_message_parse_error(msg, &err, &debug);
 			g_free(debug);

			g_print("Error: %s\n", err->message);
			g_error_free(err);
      
			break;
		}
		default: {
			break;
		}
	}
	return TRUE;
}

bool Player::isPlaying() {
	return m_playing;
}

bool Player::isSongLoaded() {
	return m_isSongLoaded;
}
