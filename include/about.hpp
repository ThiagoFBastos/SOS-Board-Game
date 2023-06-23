#ifndef ABOUT_HPP
#define ABOUT_HPP

#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/linkbutton.h>

class About : public Gtk :: Window {
	
	Gtk :: Box mbox;
	Gtk :: Label lblTitulo, lblAutor, lblAno, lblVersao;
	Gtk :: LinkButton link;

	void set_hierarchy();
	void draw_widgets();

	public:

	About();
	~About();
};

#endif
