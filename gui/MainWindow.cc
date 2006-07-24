#include "MainWindow.h"
#include "../library/LibraryFileBuilder.h"
#include <iostream>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/stock.h>

MainWindow::MainWindow()
: m_Clear("Clear"),
  m_Scan("Scan"),
  m_PreviousIcon("/usr/local/share/pixmaps/media_player/previous.xpm"),
  m_PlayPauseIcon("/usr/local/share/pixmaps/media_player/play_pause.xpm"),
  m_StopIcon("/usr/local/share/pixmaps/media_player/stop.xpm"),
  m_NextIcon("/usr/local/share/pixmaps/media_player/next.xpm"),
  m_PlaylistView(&library, &config, &player),
  m_SonglistView(&library, &config, &m_PlaylistView),
  m_TreeView(&library, &m_SonglistView, &m_PlaylistView) {
	// build the library
	library.build_library(*config.get_library_file());

	// setup the main window and connect shutdown hooks
	set_title("Media Player");
	set_default_size(config.get_width(), config.get_height());
	set_border_width(1);
	Gtk::Main::signal_quit().connect(
		sigc::mem_fun(this, &MainWindow::on_shutdown)
	);

	// setup, add, and fill the main VBox
	add(m_VBox);
	m_VBox.set_spacing(5);
	m_VBox.pack_start(m_HPaned);
	m_VBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK);

	// setup and fill the HButtonBox
	m_ButtonBox.set_layout(Gtk::BUTTONBOX_START);
	m_ButtonBox.set_spacing(5);
	m_ButtonBox.pack_start(m_Previous);
	m_ButtonBox.pack_start(m_PlayPause);
	m_ButtonBox.pack_start(m_Stop);
	m_ButtonBox.pack_start(m_Next);
	m_ButtonBox.pack_end(m_Clear);
	m_ButtonBox.pack_end(m_Scan);
	m_ButtonBox.set_child_secondary(m_Clear);
	m_ButtonBox.set_child_secondary(m_Scan);

	// add icons and tooltips to buttons
	m_Previous.set_image(m_PreviousIcon);
	m_PlayPause.set_image(m_PlayPauseIcon);
	m_Stop.set_image(m_StopIcon);
	m_Next.set_image(m_NextIcon);
	m_Tooltips.set_tip(m_Previous, "Previous");
	m_Tooltips.set_tip(m_PlayPause, "Play/Pause");
	m_Tooltips.set_tip(m_Stop, "Stop");
	m_Tooltips.set_tip(m_Next, "Next");
	m_Tooltips.set_tip(m_Clear, "Clear Playlist");
	m_Tooltips.set_tip(m_Scan, "Scan for new music");

	// connect button signals
	m_Previous.signal_clicked().connect(
		sigc::mem_fun(*this, &MainWindow::on_button_previous)
	);
	m_PlayPause.signal_clicked().connect(
		sigc::mem_fun(*this, &MainWindow::on_button_playpause)
	);
	m_Stop.signal_clicked().connect(
		sigc::mem_fun(*this, &MainWindow::on_button_stop)
	);
	m_Next.signal_clicked().connect(
		sigc::mem_fun(*this, &MainWindow::on_button_next)
	);
	m_Clear.signal_clicked().connect(
		sigc::mem_fun(*this, &MainWindow::on_button_clear)
	);
	m_Scan.signal_clicked().connect(
		sigc::mem_fun(*this, &MainWindow::on_button_scan)
	);

	// setup and fill the HPaned
	m_HPaned.set_position(config.get_hpaned_pos());
	m_HPaned.add1(m_TreeView);
	m_HPaned.add2(m_VPaned);

	// setup and fill the VPaned
	m_VPaned.set_position(config.get_vpaned_pos());
	m_VPaned.add1(m_SonglistView);
	m_VPaned.add2(m_PlaylistView);

	// initialize the views
	m_TreeView.showArtists();
	m_PlaylistView.restorePlaylist();

	// make everything visible
	show_all_children();
}

MainWindow::~MainWindow() {}

void MainWindow::on_button_previous() {
	player.previous();
}

void MainWindow::on_button_playpause() {
	if (player.isPlaying())
		player.pause();
	else if (player.isSongLoaded())
		player.play();
	else
		m_PlaylistView.playNewList();
}

void MainWindow::on_button_stop() {
	player.stop();
}

void MainWindow::on_button_next() {
	player.next();
}

void MainWindow::on_button_clear() {
	m_PlaylistView.clearListData();
}

void MainWindow::on_button_scan() {
	Gtk::FileChooserDialog fcd(*this, "Choose Root Music Directory",
			Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
	fcd.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	fcd.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
	fcd.set_current_folder(*config.get_home_path());
	int result = fcd.run();
	if (result == Gtk::RESPONSE_OK) {
		config.set_library_root(fcd.get_filename());
		LibraryFileBuilder lfb;
		lfb.generate_library_file(*config.get_library_file(),
				*config.get_library_root());
		library.clear_library();
		library.build_library(*config.get_library_file());
		m_SonglistView.clearListData();
		m_PlaylistView.clearListData();
		m_TreeView.showArtists();
	}
}

bool MainWindow::on_shutdown() {
	player.stop();
	Gtk::TreeView * treeview = m_SonglistView.getTreeView();
	for (guint i = 0; i < treeview->get_columns().size(); ++i) {
		config.set_songlist_column(treeview->get_column(i)->get_width(), i);
	}
	treeview = m_PlaylistView.getTreeView();
	for (guint i = 0; i < treeview->get_columns().size(); ++i) {
		config.set_playlist_column(treeview->get_column(i)->get_width(), i);
	}
	config.set_hpaned_pos(m_HPaned.get_position());
	config.set_vpaned_pos(m_VPaned.get_position());
	config.set_height(get_height());
	config.set_width(get_width());
	m_PlaylistView.savePlaylist();
	config.write_config_file();
	return true;
}
