#ifndef GAME_HPP
#define GAME_HPP

#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include <gtkmm/button.h>
#include <gtkmm/separator.h>
#include "mct.hpp"
#include "board.hpp"

class Game : public Gtk :: Window {

	Gtk :: Grid grid;
	Gtk :: Label lblPoints, lblPlayer;
	Gtk :: Button btnReset, btnConfirm, table[8][8];
	Gtk :: Separator separator;
	int myPoints, yourPoints;
	int focusR, focusC, turn, first;
	bool vs_cpu, frozen[8][8];
	MCT mct;
	Board board;

	void check_data();
	void load_data();
	void save_data();
	void set_hierarchy();
	void draw_widgets();
	void on_cell_clicked(int, int);
	void on_confirm_clicked();
	void on_reset_clicked();
	void put_piece(int, int, int);
	void confirm_piece(int, int, int);

	void vs_cpu_on_confirm_clicked();
	void vs_player_on_confirm_clicked();

	public:

	Game();
	~Game();
};

#endif
