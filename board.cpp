#include "board.hpp"
#include <cstring>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <random>
#include <chrono>
#include <cassert>

size_t BHash :: operator()(const Board& board) const {
    // melhorar o hash depois
    uint64_t h[8] = {0}, ans {};
    const int MOD = 1e9 + 7;
    const int P = 19191;
    const int origin[][2] = {{0, 0}, {0, 7}, {7, 0}, {7, 7}};
	int checked[8][8];

	for(int i = 0; i < 8; ++i)
		for(int j = 0; j < 8; ++j)
			checked[i][j] = board.check(i, j);

    for(int i = 0; i < 8; ++i) {
        for(int j = 0; j < 8; ++j) {

            for(int k = 0; k < 4; ++k) {
                int x0 = origin[k][0], y0 = origin[k][1];
                int dx = x0 ? -1 : 1, dy = y0 ? -1 : 1;

                for(int t = 0; t < 2; ++t) {
                    int x = x0 + dx * i, y = y0 + dy * j;

                    if(t) std :: swap(x, y);

					int ch = checked[x][y] ? 4 : 2 + board.tab[x][y];

                    h[2 * k + t] = (P * h[2 * k + t] + ch) % MOD;
                }
            }
        }
    }

    for(int i = 0; i < 8; ++i) if(ans < h[i]) ans = h[i];

    return ans;
}

Board :: Board() {
    positions = 64;
    memset(&tab[0][0], EMPTY, sizeof tab);
}

bool Board :: check(int x, int y) const {
	if(tab[x][y] == EMPTY) return false;

    bool util {};

    const int delta[][2] = {
		{1, 0},
		{0, 1},
		{1, 1},
		{1, -1}
	};

	for(int k = 0; k < 3; ++k) {
		for(int i = 0; i < 4; ++i) {
			int dx = delta[i][0], dy = delta[i][1];
			int x0 = x - k * dx, y0 = y - k * dy;
			util = util || (!(x0 < 0 || y0 < 0 || x0 >= 8 || y0 >= 8 ||
			x0 + 2 * dx < 0 || y0 + 2 * dy < 0 ||
			x0 + 2 * dx >= 8 || y0 + 2 * dy >= 8) &&
			(tab[x0][y0] != O) &&
            (tab[x0+dx][y0+dy] != S) &&
            (tab[x0+2*dx][y0+2*dy] != O));
		}
	}

    return !util;
}

void Board :: print() const {
    for(int i = 0; i < 8; ++i) {
        for(int j = 0; j < 8; ++j) {
            if(tab[i][j] == EMPTY) std :: cout << "*";
            else if(tab[i][j] == S) std :: cout << "S";
            else std :: cout << "O";
        }
        std :: cout << "\n";
    }
}

int Board :: getPositions() const {
    return positions;
}

bool Board :: can(int x, int y) const {
    return tab[x][y] == EMPTY;
}

int Board :: put(int x, int y, int piece) {
    tab[x][y] = piece;
	--positions;
    return getPoints(x, y);
}

void Board :: rem(int x, int y) {
    tab[x][y] = EMPTY;
	++positions;
}

void Board :: nextMove(int& x, int& y, int& pi) {
	std :: vector<std :: tuple<int, int, int>> options;
	std :: vector<int> w;

	for(int i = 0; i < 8; ++i) {
		for(int j = 0; j < 8; ++j) {
			if(tab[i][j] != EMPTY) continue;

			for(int p = 0; p < 2; ++p) {
				tab[i][j] = p;

				int score = getPoints(i, j);

				if(score) {
					options.emplace_back(i, j, p);
					w.push_back(100000 * score);
				} else if(p == 0) {
					options.emplace_back(i, j, 0);
					if(checkSDistraction(i, j))
						w.push_back(1000);
					else
						w.push_back(1);
				} else {
					options.emplace_back(i, j, 1);
					if(checkODistraction(i, j))
						w.push_back(1000);
					else
						w.push_back(1);
				}
			}

			tab[i][j] = EMPTY;
		}
	}

	std :: mt19937 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());
	std :: discrete_distribution dis(w.begin(), w.end());
	std :: tie(x, y, pi) = options[dis(rng)];
}

bool Board :: checkODistraction(int x, int y) const {
	const int delta[][2] =  {
		{1, 0},
		{0, 1},
		{1, 1},
		{1, -1}
	};

	bool can {};

	if(tab[x][y] != EMPTY) return false;

	for(int k = 0; k < 4; ++k) {
		int x0 = x - delta[k][0], y0 = y - delta[k][1];
		int x2 = x + delta[k][0], y2 = y + delta[k][1];
		can = can || (x0 >= 0 && y0 >= 0 && x0 < 8 && y0 < 8 && x2 >= 0 && y2 >= 0 && x2 < 8 && y2 < 8
			&& tab[x0][y0] != O && tab[x2][y2] != O
		);
	}

	return !can;
}

bool Board :: checkSDistraction(int x, int y) const {
	const int delta[][2] =  {
		{1, 0},
		{0, 1},
		{1, 1},
		{1, -1}
	};

	bool can {};

	if(tab[x][y] != EMPTY) return false;

	for(int i = 0; i < 4; ++i) {
		for(int k : {-1, 1}) {
			int x1 = x + k * delta[i][0], y1 = y + k * delta[i][1];
			int x2 = x + 2 * k * delta[i][0], y2 = y + 2 * k * delta[i][1];

			can = can || (x1 >= 0 && x1 < 8 && y1 >= 0 && y1 < 8 && x2 >= 0 && x2 < 8 &&
			y2 >= 0 && y2 < 8 && tab[x1][y1] != S && tab[x2][y2] != O);
		}
	}

	return !can;
}

int Board :: getPoints(int x, int y) const {
    const int delta[][2] = {
		{1, 0},
		{0, 1},
		{1, 1},
		{1, -1}
	};
	int points {};
	for(int k = 0; k < 3; ++k) {
		for(int i = 0; i < 4; ++i) {
			int dx = delta[i][0], dy = delta[i][1];
			int x0 = x - k * dx, y0 = y - k * dy;
			points += !(x0 < 0 || y0 < 0 || x0 >= 8 || y0 >= 8 ||
			 x0 + 2 * dx < 0 || y0 + 2 * dy < 0 ||
			 x0 + 2 * dx >= 8 || y0 + 2 * dy >= 8) &&
			tab[x0][y0] == S &&
			tab[x0+dx][y0+dy] == O &&
			tab[x0+2*dx][y0+2*dy] == S;
		}
	}
	return points;
}

bool Board :: operator==(const Board& other) const {
    int checked[2][8][8];
    const int origin[][2] = {{0, 0}, {0, 7}, {7, 0}, {7, 7}};

    for(int i = 0; i < 8; ++i) {
        for(int j = 0; j < 8; ++j) {
            checked[0][i][j] = check(i, j);
            checked[1][i][j] = other.check(i, j);
        }
    }

    for(int k = 0; k < 4; ++k) {
        for(int t = 0; t < 2; ++t) {
            int x0 = origin[k][0], y0 = origin[k][1];
            int dx = x0 ? -1 : 1, dy = y0 ? -1 : 1;
            bool is_equal {true};

            for(int i = 0; i < 8; ++i) {
                for(int j = 0; j < 8; ++j) {
                    int x = x0 + dx * i, y = y0 + dy * j;

					if(t) std :: swap(x, y);

					int a = checked[0][x][y] ? 2 : tab[x][y];
					int b = checked[1][i][j] ? 2 : tab[i][j];

					is_equal = is_equal && a == b;
                }
            }

            if(is_equal) return true;
        }
    }

    return false;
}
