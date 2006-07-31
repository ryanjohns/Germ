#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "TreeView.h"
#include "SonglistView.h"
#include "PlaylistView.h"
#include "SeekBar.h"
#include "../library/Library.h"
#include "../config/Configurations.h"
#include "../player/Player.h"
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/paned.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/image.h>
#include <gtkmm/tooltips.h>

class MainWindow : public Gtk::Window {
public:
	MainWindow();
	virtual ~MainWindow();

protected:
	virtual void on_button_clear();
	virtual void on_button_next();
	virtual void on_button_playpause();
	virtual void on_button_previous();
	virtual void on_button_scan();
	virtual void on_button_stop();
//	virtual bool on_key_press_event(GdkEventKey *);
	virtual bool on_shutdown();
	virtual void on_update_window_title(Glib::ustring);

private:
	Library m_library;
	Configurations m_config;
	Player m_player;
	Gtk::VBox m_vbox;
	Gtk::HBox m_hbox;
	Gtk::HPaned m_hpaned;
	Gtk::VPaned m_vpaned;
	Gtk::Button m_previous, m_playPause, m_stop, m_next, m_clear, m_scan;
	Gtk::Image m_previousIcon, m_playPauseIcon, m_stopIcon, m_nextIcon;
	Gtk::HButtonBox m_controlsBox, m_optionsBox;
	Gtk::Tooltips m_tooltips;
	SeekBar m_seekBar;
	PlaylistView m_playlistView;
	SonglistView m_songlistView;
	TreeView m_treeView;
};

#endif /*MAINWINDOW_H_*/
