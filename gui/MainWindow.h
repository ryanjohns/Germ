#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "TreeView.h"
#include "SonglistView.h"
#include "PlaylistView.h"
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
	Library library;
	Configurations config;
	Player player;
	virtual void on_button_previous();
	virtual void on_button_playpause();
	virtual void on_button_stop();
	virtual void on_button_next();
	virtual void on_button_clear();
	virtual void on_button_scan();
	virtual bool on_shutdown();
	virtual void on_update_window_title(Glib::ustring);
	
	Gtk::VBox m_VBox;
	Gtk::HPaned m_HPaned;
	Gtk::VPaned m_VPaned;
	Gtk::Button m_Previous, m_PlayPause, m_Stop, m_Next, m_Clear, m_Scan;
	Gtk::Image m_PreviousIcon, m_PlayPauseIcon, m_StopIcon, m_NextIcon;
	Gtk::HButtonBox m_ButtonBox;
	Gtk::Tooltips m_Tooltips;
	PlaylistView m_PlaylistView;
	SonglistView m_SonglistView;
	TreeView m_TreeView;
};

#endif /*MAINWINDOW_H_*/
