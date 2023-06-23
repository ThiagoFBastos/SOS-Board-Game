#include "about.hpp"
#include <iostream>

About :: About() {
	set_title("about");
	set_default_size(300, 300);
	set_visible(true);
	draw_widgets();
	set_hierarchy();
	show_all_children();
}

About :: ~About() {
	
}


void About :: set_hierarchy() {
	add(mbox);
	mbox.pack_start(lblTitulo, Gtk :: PACK_SHRINK);
	mbox.pack_start(lblVersao, Gtk :: PACK_SHRINK);
	mbox.pack_start(lblAutor, Gtk :: PACK_SHRINK);
	mbox.pack_start(lblAno, Gtk :: PACK_SHRINK);
	mbox.pack_start(link, Gtk :: PACK_SHRINK);
}

void About :: draw_widgets() {
	//mbox
	mbox = Gtk :: Box(Gtk :: ORIENTATION_VERTICAL);

	//lblTitulo
	lblTitulo.set_label("SOS Board Game");

	//lblAutor
	lblAutor.set_label("Thiago F. B. da Silva");

	//lblAno
	lblAno.set_label("2023");

	//lblVersão
	lblVersao.set_label("Versão 0.99");

	//lblLink
	link.set_label("Site");
	link.set_uri("https://github.com/ThiagoFBastos/SOS-Board-Game.git");
}

