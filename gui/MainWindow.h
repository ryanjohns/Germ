/*******************************************************************************

Media_Player - Linux music library and player.
Copyright (C) 2006  Ryan Johns and Stephen McCarthy

This file is part of Media_Player.
Media_Player is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

Media_Player is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with Media_Player; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*******************************************************************************/

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "TreeView.h"
#include "SonglistView.h"
#include "PlaylistView.h"
#include "SeekBar.h"
#include "AudioControls.h"
#include "../library/Library.h"
#include "../config/Configurations.h"
#include "../player/Player.h"
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/paned.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/tooltips.h>

class MainWindow : public Gtk::Window {
public:
	MainWindow();
	virtual ~MainWindow();

protected:
	virtual void on_button_clear();
	virtual void on_button_scan();
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
	Gtk::Button m_clear, m_scan;
	Gtk::HButtonBox m_optionsBox;
	Gtk::Tooltips m_tooltips;
	SeekBar m_seekBar;
	PlaylistView m_playlistView;
	SonglistView m_songlistView;
	TreeView m_treeView;
	AudioControls m_audioControls;
};

#endif /*MAINWINDOW_H_*/
