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

#include "MainWindow.h"
#include "../library/LibraryFileBuilder.h"
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/stock.h>

MainWindow::MainWindow()
: m_clear("Clear"),
  m_scan("Scan"),
  m_seekBar(&m_player),
  m_playlistView(&m_library, &m_config, &m_player),
  m_songlistView(&m_library, &m_config, &m_playlistView),
  m_treeView(&m_library, &m_songlistView, &m_playlistView),
  m_audioControls(&m_playlistView, &m_player) {
	// build the library
	m_library.build_library(*m_config.get_library_file());

	// setup the main window and connect window signals
	set_title("Media Player");
	set_icon_from_file("/usr/local/share/pixmaps/media_player.png");
	set_default_size(m_config.get_width(), m_config.get_height());
	set_border_width(1);
	Gtk::Main::signal_quit().connect(
		sigc::mem_fun(this, &MainWindow::on_shutdown)
	);
	m_player.signal_update_window_title.connect(
		sigc::mem_fun(*this, &MainWindow::on_update_window_title)
	);

	// setup, add, and fill the main VBox
	add(m_vbox);
	m_vbox.set_spacing(5);
	m_vbox.pack_start(m_hpaned);
	m_vbox.pack_start(m_hbox, Gtk::PACK_SHRINK);

	// setup and fill the lower HBox
	m_hbox.set_spacing(5);
	m_hbox.pack_start(m_audioControls, Gtk::PACK_SHRINK);
	m_hbox.pack_start(m_seekBar);
	m_hbox.pack_start(m_optionsBox, Gtk::PACK_SHRINK);

	// setup and fill the options button box
	m_optionsBox.set_layout(Gtk::BUTTONBOX_END);
	m_optionsBox.set_spacing(3);
	m_optionsBox.pack_start(m_clear);
	m_optionsBox.pack_start(m_scan);

	// add tooltips to buttons
	m_tooltips.set_tip(m_clear, "Clear Playlist");
	m_tooltips.set_tip(m_scan, "Scan for new music");

	// connect button signals
	m_clear.signal_clicked().connect(
		sigc::mem_fun(*this, &MainWindow::on_button_clear)
	);
	m_scan.signal_clicked().connect(
		sigc::mem_fun(*this, &MainWindow::on_button_scan)
	);

	// setup and fill the HPaned
	m_hpaned.set_position(m_config.get_hpaned_pos());
	m_hpaned.add1(m_treeView);
	m_hpaned.add2(m_vpaned);

	// setup and fill the VPaned
	m_vpaned.set_position(m_config.get_vpaned_pos());
	m_vpaned.add1(m_songlistView);
	m_vpaned.add2(m_playlistView);

	// initialize the views
	m_treeView.show_artists();
//	m_playlistView.restore_playlist();

	// make everything visible
	show_all_children();
}

MainWindow::~MainWindow() {}

void MainWindow::on_button_clear() {
	m_playlistView.clear_list_data();
}

void MainWindow::on_button_scan() {
	Gtk::FileChooserDialog fcd(*this, "Choose Root Music Directory",
			Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
	fcd.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	fcd.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
	fcd.set_current_folder(*m_config.get_home_path());
	int result = fcd.run();
	if (result == Gtk::RESPONSE_OK) {
		m_config.set_library_root(fcd.get_filename());
		LibraryFileBuilder lfb;
		lfb.generate_library_file(*m_config.get_library_file(),
				*m_config.get_library_root());
		m_library.clear_library();
		m_library.build_library(*m_config.get_library_file());
		m_songlistView.clear_list_data();
		m_playlistView.clear_list_data();
		m_treeView.show_artists();
	}
}

bool MainWindow::on_shutdown() {
	m_player.stop();
	Gtk::TreeView * treeview = m_songlistView.get_tree_view();
	for (guint i = 0; i < treeview->get_columns().size(); ++i) {
		m_config.set_songlist_column(treeview->get_column(i)->get_width(), i);
	}
	treeview = m_playlistView.get_tree_view();
	for (guint i = 0; i < treeview->get_columns().size(); ++i) {
		m_config.set_playlist_column(treeview->get_column(i)->get_width(), i);
	}
	m_config.set_hpaned_pos(m_hpaned.get_position());
	m_config.set_vpaned_pos(m_vpaned.get_position());
	m_config.set_height(get_height());
	m_config.set_width(get_width());
	m_playlistView.save_playlist();
	m_config.write_config_file();
	return true;
}

void MainWindow::on_update_window_title(Glib::ustring title) {
	set_title(title + " :: Media Player");
}
