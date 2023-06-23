#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/frame.h>

class Settings : public Gtk :: Window {
	/*
		vs pc ou outro player		
		colocar num arquivo
	*/

	Gtk :: Frame framePlayer;
	Gtk :: RadioButton vsPlayer, vsCpu;
	Gtk :: Button btnSave, btnReset;
	Gtk :: Box outerBox, boxAction, boxVs;
	
	void set_hierarchy();
	void draw_widgets();
	void on_save_clicked();
	void on_reset_clicked();

	public:
		
	Settings();
	~Settings();
};

#endif
