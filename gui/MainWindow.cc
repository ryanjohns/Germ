#include "MainWindow.h"
#include "../library/LibraryFileBuilder.h"
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/stock.h>
#include <gdk/gdkkeysyms.h>

MainWindow::MainWindow()
: m_clear("Clear"),
  m_scan("Scan"),
  m_previousIcon("/usr/local/share/pixmaps/media_player/previous.xpm"),
  m_playPauseIcon("/usr/local/share/pixmaps/media_player/play_pause.xpm"),
  m_stopIcon("/usr/local/share/pixmaps/media_player/stop.xpm"),
  m_nextIcon("/usr/local/share/pixmaps/media_player/next.xpm"),
  m_seekBar(&m_player),
  m_playlistView(&m_library, &m_config, &m_player),
  m_songlistView(&m_library, &m_config, &m_playlistView),
  m_treeView(&m_library, &m_songlistView, &m_playlistView) {
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
	m_hbox.pack_start(m_controlsBox, Gtk::PACK_SHRINK);
	m_hbox.pack_start(m_seekBar);
	m_hbox.pack_start(m_optionsBox, Gtk::PACK_SHRINK);

	// setup and fill the media controls button box
	m_controlsBox.set_layout(Gtk::BUTTONBOX_START);
	m_controlsBox.pack_start(m_previous);
	m_controlsBox.pack_start(m_playPause);
	m_controlsBox.pack_start(m_stop);
	m_controlsBox.pack_start(m_next);

	// setup and fill the options button box
	m_optionsBox.set_layout(Gtk::BUTTONBOX_END);
	m_optionsBox.set_spacing(3);
	m_optionsBox.pack_start(m_clear);
	m_optionsBox.pack_start(m_scan);

	// add icons and tooltips to buttons
	m_previous.set_image(m_previousIcon);
	m_playPause.set_image(m_playPauseIcon);
	m_stop.set_image(m_stopIcon);
	m_next.set_image(m_nextIcon);
	m_tooltips.set_tip(m_previous, "Previous");
	m_tooltips.set_tip(m_playPause, "Play/Pause");
	m_tooltips.set_tip(m_stop, "Stop");
	m_tooltips.set_tip(m_next, "Next");
	m_tooltips.set_tip(m_clear, "Clear Playlist");
	m_tooltips.set_tip(m_scan, "Scan for new music");

	// connect button signals
	m_previous.signal_clicked().connect(
		sigc::mem_fun(*this, &MainWindow::on_button_previous)
	);
	m_playPause.signal_clicked().connect(
		sigc::mem_fun(*this, &MainWindow::on_button_playpause)
	);
	m_stop.signal_clicked().connect(
		sigc::mem_fun(*this, &MainWindow::on_button_stop)
	);
	m_next.signal_clicked().connect(
		sigc::mem_fun(*this, &MainWindow::on_button_next)
	);
	m_clear.signal_clicked().connect(
		sigc::mem_fun(*this, &MainWindow::on_button_clear)
	);
	m_scan.signal_clicked().connect(
		sigc::mem_fun(*this, &MainWindow::on_button_scan)
	);
/*	m_controlsBox.signal_key_press_event.connect(
		sigc::mem_fun(*this, &MainWindow::on_key_press_event), false
	);*/

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

void MainWindow::on_button_next() {
	m_player.next();
}

void MainWindow::on_button_playpause() {
	if (m_player.is_playing())
		m_player.pause();
	else if (m_player.is_song_loaded())
		m_player.play();
	else
		m_playlistView.play_new_list();
}

void MainWindow::on_button_previous() {
	m_player.previous();
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

void MainWindow::on_button_stop() {
	m_player.stop();
}

/*bool MainWindow::on_key_press_event(GdkEventKey * event) {
	if (event->keyval == GDK_Delete) {
		
	}
	return true;
}*/

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
