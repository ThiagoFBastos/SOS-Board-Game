#include "game.hpp"
#include <fstream>
#include <chrono>
#include <iostream>
#include <cstring>
#include <random>
#include <gtkmm/cssprovider.h>
#include "storage.hpp"

Game :: Game() {

	set_title("SOS game");
	set_default_size(400, 300);
	set_visible(true);
	set_resizable(false);

	draw_widgets();

	set_hierarchy();

	show_all_children();

	load_data();

	this->signal_hide().connect(sigc :: mem_fun(*this, &Game :: save_data));
}

Game :: ~Game() {
	#ifdef DEBUG
	std :: cout << "saindo do jogo\n";
	#endif
}

void Game :: load_data() {
	std :: mt19937 rng(std :: chrono::steady_clock::now().time_since_epoch().count());
	Storage st("data/cache.dat");
	int data[8][8];

	focusR = focusC = -1;
	vs_cpu = st.getEnemy();
	first = st.getFirst();
	turn = 0;

	st.getScore(myPoints, yourPoints);
	st.getTable(data);

	for(int i = 0; i < 8; ++i) {
		for(int j = 0; j < 8; ++j) {
			put_piece(i, j, data[i][j]);
			frozen[i][j] = data[i][j] != -1;
			turn += data[i][j] != -1;
		}
	}

	if(vs_cpu) {

		mct = MCT(turn ? "data/cache_tree.dat" : "data/start_tree.dat", data);

		lblPlayer.set_label("Seu turno");

		if(turn == 0) {
			double P = std :: uniform_real_distribution<double>(0, 1)(rng);

			if(P > 0.5) {
				int r, c, p;
				yourPoints = mct.do_next_move(r, c, p);
				frozen[r][c] = true;
				put_piece(r, c, p);
				turn = first = 1;
			}
		}

		lblPoints.set_label(std :: to_string(myPoints) + " (VOCÊ) x " + std :: to_string(yourPoints) + " (CPU)");
	} else {
		board = Board();

		for(int i = 0; i < 8; ++i) {
			for(int j = 0; j < 8; ++j) {
				if(data[i][j] == -1) continue;
				board.put(i, j, data[i][j]);
			}
		}

		lblPlayer.set_label((first ^ turn) % 2 ? "Player B" : "Player A");

		if(turn == 0) {
			double P = std :: uniform_real_distribution<double>(0,1)(rng);

			if(P < 0.5) {
				lblPlayer.set_label("Player A");
				first = 0;
			} else {
				lblPlayer.set_label("Player B");
				first = 1;
			}
		}

		lblPoints.set_label(std :: to_string(myPoints) + " (A) x " + std :: to_string(yourPoints) + " (B)");
	}

	st.close();
}

void Game :: save_data() {
	Storage st("data/cache.dat");
	int data[8][8];

	for(int i = 0; i < 8; ++i) {
		for(int j = 0; j < 8; ++j) {
			auto piece = table[i][j].get_label();

			if(piece == "X") data[i][j] = -1;
			else if(piece == "S") data[i][j] = 0;
			else data[i][j] = 1;
		}
	}

	if(vs_cpu) mct.persist("data/cache_tree.dat");

	st.saveScore(myPoints, yourPoints);
	st.saveTable(data);
	st.saveFirst(first);

	#ifdef DEBUG
	std :: cout << "salvou o estado atual\n";
	#endif

	st.close();
}

void Game :: put_piece(int x, int y, int pi) {
	Glib :: RefPtr<Gtk :: CssProvider> css = Gtk :: CssProvider :: create();

	if(pi == -1) {
		css->load_from_data("button {color: black;background-image: image(white);} button:hover {background-image: image(#9fff8f);}");
		table[x][y].set_label("X");
	} else if(pi == 0) {
		css->load_from_data("button {color: red;background-image: image(white);} button:hover {background-image: image(#9fff8f);}");
		table[x][y].set_label("S");
	} else {
		css->load_from_data("button {color: blue;background-image: image(white);} button:hover {background-image: image(#9fff8f);}");
		table[x][y].set_label("O");
	}

	table[x][y].get_style_context()->add_provider(css, GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void Game :: on_reset_clicked() {
	std :: mt19937 rng(std :: chrono::steady_clock::now().time_since_epoch().count());
	double P = std :: uniform_real_distribution<double>(0,1)(rng);

	focusR = focusC = -1;
	myPoints = yourPoints = 0;
	turn = 0;
	first = P > 0.5;

	for(int i = 0; i < 8; ++i) {
		for(int j = 0; j < 8; ++j) {
			put_piece(i, j, -1);
			frozen[i][j] = false;
		}
	}

	if(vs_cpu) {
		int table[8][8];

		memset(&table[0][0], -1, sizeof table);

		mct = MCT("data/start_tree.dat", table);

		if(first) {
			int r, c, p;
			yourPoints = mct.do_next_move(r, c, p);
			put_piece(r, c, p);
			frozen[r][c] = true;
			++turn;
		}

		lblPlayer.set_label("Seu turno");
		lblPoints.set_label("0 (VOCÊ) x " + std :: to_string(yourPoints) + " (CPU)");
	} else {
		board = Board();
		lblPlayer.set_label(first ? "Player B" : "Player A");
		lblPoints.set_label("0 (A) x 0 (B)");
	}
}

void Game :: on_cell_clicked(int r, int c) {

	if(vs_cpu) {

		if((turn ^ first) % 2 || frozen[r][c]) return;

		if(focusR != -1 && (r != focusR || c != focusC))
			put_piece(focusR, focusC, -1);

		focusR = r;
		focusC = c;

		auto piece = table[r][c].get_label();

		if(piece == "X" || piece == "O")
			put_piece(r, c, 0);
		else if(piece == "S")
			put_piece(r, c, 1);
	} else if(!frozen[r][c]) {

		if(focusR != -1 && (r != focusR || c != focusC))
			put_piece(focusR, focusC, -1);

		auto piece = table[r][c].get_label();

		if(piece == "X" || piece == "O")
			put_piece(r, c, 0);
		else if(piece == "S")
			put_piece(r, c, 1);

		focusR = r;
		focusC = c;
	}
}

void Game :: vs_cpu_on_confirm_clicked() {
	if((turn ^ first) % 2 || focusR < 0 || focusC < 0) return;

	++turn;

	int r, c, p;
	auto my_piece = table[focusR][focusC].get_label();

	frozen[focusR][focusC] = true;

	myPoints += mct.move_to(focusR, focusC, my_piece == "O");

	if(turn == 64) {
		if(myPoints < yourPoints) lblPlayer.set_label("CPU venceu!");
		else if(myPoints > yourPoints) lblPlayer.set_label("VOCÊ venceu!");
		else lblPlayer.set_label("Empate!");
		return;
	}

	yourPoints += mct.do_next_move(r, c, p);

	frozen[r][c] = true;

	put_piece(r, c, p);

	lblPoints.set_label(std :: to_string(myPoints) + " (VOCÊ) x " + std :: to_string(yourPoints) + " (CPU)");

	focusR = focusC = -1;

	if(turn == 63) {
		if(myPoints < yourPoints) lblPlayer.set_label("CPU venceu!");
		else if(myPoints > yourPoints) lblPlayer.set_label("VOCÊ venceu!");
		else lblPlayer.set_label("Empate!");
	}

	++turn;
}

void Game :: vs_player_on_confirm_clicked() {
	if(focusR < 0 || focusC < 0) return;

	int player = (turn ^ first) % 2;

	auto piece = table[focusR][focusC].get_label();

	frozen[focusR][focusC] = true;

	if(player == 0) myPoints += board.put(focusR, focusC, piece == "O");
	else yourPoints += board.put(focusR, focusC, piece == "O");

	focusR = focusC = -1;
	++turn;

	lblPoints.set_label(std :: to_string(myPoints) + " (A) x " + std :: to_string(yourPoints) + " (B)");

	if(turn == 64) {
		if(myPoints > yourPoints)
			lblPlayer.set_label("O player A venceu!");
		else if(myPoints < yourPoints)
			lblPlayer.set_label("O player B venceu!");
		else
			lblPlayer.set_label("Empate!");
	} else if(player == 0)
		lblPlayer.set_label("Player B");
	else
		lblPlayer.set_label("Player A");
}

void Game :: on_confirm_clicked() {
	if(vs_cpu) vs_cpu_on_confirm_clicked();
	else vs_player_on_confirm_clicked();
}

void Game :: set_hierarchy() {

	//grid
	add(grid);
	grid.set_column_spacing(5);
	grid.set_row_spacing(5);

	//lblPlayer
	grid.attach(lblPlayer, 0, 0, 4, 2);

	//lblPoints
	grid.attach(lblPoints, 0, 2, 4, 2);

	//btnReset
	grid.attach(btnReset, 0, 6, 4);
	btnReset.signal_clicked().connect(sigc :: mem_fun(*this, &Game :: on_reset_clicked));

	//btnConfirm
	grid.attach(btnConfirm, 0, 7, 4);
	btnConfirm.signal_clicked().connect(sigc :: mem_fun(*this, &Game :: on_confirm_clicked));

	//separator
	grid.attach(separator, 4, 0, 1, 8);

	//table
	for(int i = 0; i < 8; ++i) {
		for(int j = 0; j < 8; ++j) {
			grid.attach(table[i][j], 5 + j * 2, i, 2);
			table[i][j].signal_clicked().connect(sigc :: bind(sigc :: mem_fun(*this, &Game :: on_cell_clicked), i, j));
		}
	}
}

void Game :: draw_widgets() {

	// separator
	separator = Gtk :: Separator(Gtk :: ORIENTATION_VERTICAL);

	//btnReset
	btnReset.set_label("reset");

	//btnConfirm
	btnConfirm.set_label("confirmar");
}
