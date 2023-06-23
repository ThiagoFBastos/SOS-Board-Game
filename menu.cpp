#include "menu.hpp"
#include <iostream>
#include <cstdlib>

Menu :: Menu() {
	about = nullptr;
	settings = nullptr;
	game = nullptr;

	set_title("SOS game");
	set_default_size(300, 300);
	set_visible(true);

	draw_widgets();

	set_hierarchy();

	show_all_children();

	has_focus = true;
}

Menu :: ~Menu() {
	if(about) delete about;
	if(settings) delete settings;
	if(game) delete game;
}

void Menu :: on_about_hided() {
	delete about;
	about = nullptr;
	has_focus = true;
}

void Menu :: on_settings_hided() {
	delete settings;
	settings = nullptr;
	has_focus = true;
}

void Menu :: on_play_hided() {
	delete game;
	game = nullptr;
	has_focus = true;
}

void Menu :: on_play_clicked() {
	if(!has_focus) return;
	has_focus = false;
	game = new Game;
	game->signal_hide().connect(sigc :: mem_fun(*this, &Menu :: on_play_hided));
	game->show();
	std :: cout << "apertou o play\n";
}

void Menu :: on_settings_clicked() {
	if(!has_focus) return;
	has_focus = false;
	settings = new Settings;
	settings->signal_hide().connect(sigc :: mem_fun(*this, &Menu :: on_settings_hided));	
	settings->show();
	std :: cout << "apertou o settings button\n";
}

void Menu :: on_about_clicked() {
	if(!has_focus) return;
	has_focus = false;
	about = new About;
	about->signal_hide().connect(sigc :: mem_fun(*this, &Menu :: on_about_hided));
	about->show();
	std :: cout << "apertou o about button\n";	
}

void Menu :: set_hierarchy() {
	
	add(mbox);

	mbox.pack_start(lblSos);
	mbox.pack_start(btnPlay, Gtk :: PACK_SHRINK);
	mbox.pack_start(btnSettings, Gtk :: PACK_SHRINK);
	mbox.pack_start(btnAbout, Gtk :: PACK_SHRINK);

	// btnPlay button

	btnPlay.signal_clicked().connect(sigc :: mem_fun(*this, &Menu :: on_play_clicked));

	//btnSettings button

	btnSettings.signal_clicked().connect(sigc :: mem_fun(*this, &Menu :: on_settings_clicked));

	// btnPlay button

	btnAbout.signal_clicked().connect(sigc :: mem_fun(*this, &Menu :: on_about_clicked));
}

void Menu :: draw_widgets() {
	
	mbox = Gtk :: Box(Gtk :: ORIENTATION_VERTICAL);

	lblSos.set_label("SOS game");

	btnPlay.set_label("Play");

	btnSettings.set_label("Settings");

	btnAbout.set_label("About");	
}
