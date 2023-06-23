#ifndef MENU_HPP
#define MENU_HPP

#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include "about.hpp"
#include "settings.hpp"
#include "game.hpp"

class Menu : public Gtk :: Window {

	Gtk :: Button btnSettings, btnAbout, btnPlay;
	Gtk :: Label lblSos;
	Gtk :: Box mbox;
	About *about;
	Settings *settings;
	Game *game;
	bool has_focus;

	void set_hierarchy();
	void draw_widgets();
	
	void on_about_hided();
	void on_settings_hided();
	void on_play_hided();

	void on_play_clicked();
	void on_settings_clicked();
	void on_about_clicked();

	public:

	Menu();
	~Menu();
};

#endif
