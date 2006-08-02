#include "Player.h"
#include <sstream>

Player::Player(){
	m_playing = false;
	m_songLoaded = false;
	m_nextSong.clear();
	m_backupSong.clear();

	gst_init(NULL, NULL);

	m_pipeline = gst_element_factory_make("playbin", "play");

	gst_bus_add_watch(gst_pipeline_get_bus(GST_PIPELINE(m_pipeline)),
		Player::bus_watch, (gpointer)this);

	g_timeout_add(1000, Player::song_position_watch, (gpointer)this);
}

Player::~Player(){
	gst_element_set_state(m_pipeline, GST_STATE_NULL);
	gst_object_unref(GST_OBJECT(m_pipeline));
}

bool Player::is_playing() {
	return m_playing;
}

bool Player::is_song_loaded() {
	return m_songLoaded;
}

void Player::next() {
	if (m_nextSong.empty()) {
		send_song_request(false);
	}
	if (!m_nextSong.empty()) {
		m_songLoaded = false;
		stop();
		play();
		m_nextSong.clear();
	}
}

void Player::pause() {
	gst_element_set_state(m_pipeline, GST_STATE_PAUSED);
	m_playing = false;
}

void Player::play() {
	if (m_nextSong.empty()) {
		m_nextSong = m_backupSong;
	}
	if (!m_songLoaded && !m_nextSong.empty()) {
		std::string uri = "file://" + m_nextSong;
		g_object_set(G_OBJECT(m_pipeline), "uri", uri.c_str() , NULL);
		m_songLoaded = true;
	}
	if (m_songLoaded) {
		gst_element_set_state(m_pipeline, GST_STATE_PLAYING);
		m_nextSong.clear();
		m_playing = true;
	}
}

void Player::play(Song * song) {
	set_next_song(song);
	m_songLoaded = false;
	play();
}

void Player::previous() {
	send_song_request(true);
	if (!m_nextSong.empty()) {
		m_songLoaded = false;
		stop();
		play();
		m_nextSong.clear();
	}
}

void Player::seek(double p) {
	GstFormat fmt = GST_FORMAT_TIME;
	gint64 len;
	gst_element_query_duration(m_pipeline, &fmt, &len);
	gst_element_seek(m_pipeline, 1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH,
		GST_SEEK_TYPE_SET, (gint64)(p * len), GST_SEEK_TYPE_NONE,
		GST_CLOCK_TIME_NONE);
}

void Player::set_next_song(Song * song) {
	if (song == NULL) {
		m_nextSong.clear();
	}
	else {
		m_nextSong = *song->get_path();
		m_backupSong = m_nextSong;
		Glib::ustring tmp(*song->get_artist_name());
		tmp += " - " + *song->get_song_title();
		send_update_window_title(tmp);
		int len = song->get_length();
		send_song_length(format_time(len));
		send_update_seek_bar(0, 0);
	}
}

void Player::set_song_loaded(bool loaded) {
	m_songLoaded = loaded;
}

void Player::stop() {
	gst_element_set_state(m_pipeline, GST_STATE_NULL);
	m_playing = false;
	send_update_seek_bar(0, 0);
}

Glib::ustring Player::format_time(int length) {
	std::ostringstream oss;
	int minutes = length / 60;
	int seconds = length - minutes * 60;
	oss << minutes << ":";
	if (seconds < 10) {
		oss << 0 << seconds;
	}
	else {
		oss << seconds;
	}
	return oss.str();
}

GstElement * Player::get_pipeline() {
	return m_pipeline;
}

void Player::send_song_length(Glib::ustring str) {
	signal_song_length.emit(str);
}

void Player::send_song_request(bool previous) {
	signal_song_request.emit(previous);
}

void Player::send_update_seek_bar(gint64 pos, gint64 len) {
	signal_update_seek_bar.emit(pos, len);
}

void Player::send_update_window_title(Glib::ustring title) {
	signal_update_window_title.emit(title);
}

gboolean Player::bus_watch(GstBus *, GstMessage * msg, gpointer data) {
	Player * player = (Player *)data;

	switch (GST_MESSAGE_TYPE(msg)) {
		case GST_MESSAGE_EOS: {
			player->stop();
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

gboolean Player::song_position_watch(gpointer data) {
	GstFormat fmt = GST_FORMAT_TIME;
	gint64 pos, len;
	Player * player = (Player *)data;
	GstElement * pipe = player->get_pipeline();
	if (player->is_playing()) {
		if (gst_element_query_position(pipe, &fmt, &pos) &&
				gst_element_query_duration(pipe, &fmt, &len)) {
			player->send_update_seek_bar(pos, len);
		}
	}
	return TRUE;
}
