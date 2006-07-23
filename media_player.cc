#include "gui/MainWindow.h"
#include <gtkmm/main.h>

int main(int argc, char * argv[]) {	
	Gtk::Main kit(argc, argv);
	MainWindow main_window;
	Gtk::Main::run(main_window);
	return 0;
}
