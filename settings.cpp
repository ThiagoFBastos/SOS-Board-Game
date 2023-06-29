#include "settings.hpp"
#include <iostream>
#include <fstream>
#include <cstring>
#include "storage.hpp"

Settings :: Settings() {
	set_title("settings");
	set_default_size(300, 300);
	set_visible(true);
	set_resizable(false);
	set_position(Gtk::WIN_POS_CENTER_ALWAYS);

	draw_widgets();
	set_hierarchy();
	show_all_children();

	Storage st("data/cache.dat");
	vsPlayer.set_active(!st.getEnemy());
}

Settings :: ~Settings() {
	#ifdef DEBUG
	std :: cout << "saindo dou settings\n";
	#endif
}

void Settings :: on_save_clicked() {
	Storage st("data/cache.dat");

	if(vsCpu.get_active() != st.getEnemy()) {
		int p1 {}, p2 {}, first {}, table[8][8];

		memset(&table[0][0], -1, sizeof table);

		st.saveEnemy(vsCpu.get_active());
		st.saveTable(table);
		st.saveScore(p1, p2);
		st.saveFirst(first);

		#ifdef DEBUG
		std :: cout << "você salvou isso\n";
		#endif
	}

	st.close();
}

void Settings :: on_reset_clicked() {
	vsPlayer.set_active(true);
	#ifdef DEBUG
	std :: cout << "você resetou isso\n";
	#endif
}

void Settings :: set_hierarchy() {
	add(outerBox);

	outerBox.pack_start(framePlayer, Gtk :: PACK_SHRINK);
	outerBox.pack_end(boxAction, Gtk :: PACK_SHRINK);

	//framePlayer
	framePlayer.add(boxVs);
	boxVs.pack_start(vsPlayer);
	boxVs.pack_start(vsCpu);
	vsPlayer.join_group(vsCpu);

	//boxSave
	boxAction.pack_end(btnSave, Gtk :: PACK_SHRINK);
	boxAction.pack_end(btnReset, Gtk :: PACK_SHRINK);

	//btnSave
	btnSave.signal_clicked().connect(sigc :: mem_fun(*this, &Settings :: on_save_clicked));

	//btnReset
	btnReset.signal_clicked().connect(sigc :: mem_fun(*this, &Settings :: on_reset_clicked));
}

void Settings :: draw_widgets() {

	outerBox = Gtk :: Box(Gtk :: ORIENTATION_VERTICAL);
	boxAction = Gtk :: Box(Gtk :: ORIENTATION_HORIZONTAL);
	boxVs = Gtk :: Box(Gtk :: ORIENTATION_HORIZONTAL);

	btnSave.set_label("salvar");
	btnReset.set_label("resetar");
	framePlayer.set_label("Escolha seu inimigo");
	vsPlayer.set_label("player 2");
	vsCpu.set_label("cpu");
}
