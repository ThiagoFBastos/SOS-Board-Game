#include <iostream>
#include <gtkmm/application.h>
#include "menu.hpp"

int main(int argc, char *argv[]) {
	auto app = Gtk::Application::create(argc, argv, "org.sos.gtkmm");
	Menu menu;
	return app->run(menu);
}
