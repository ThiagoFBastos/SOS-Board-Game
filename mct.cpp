#include "mct.hpp"
#include <random>
#include <chrono>
#include <tuple>
#include <climits>
#include <iostream>
#include <algorithm>
#include <cassert>

MCT :: MCT() {
	reset();
	//print();
}

MCT :: MCT(int table[][8]) {
	for(int i = 0; i < 8; ++i) {
		for(int j = 0; j < 8; ++j) {
			if(table[i][j] < 0) continue;
			board.put(i, j, table[i][j]);
		}
	}
	reset();
}

void MCT :: reset() {
	root.reset(new Position);
	root->result = INT_MIN;
	build(1000);
}

int MCT :: p1(int s, int a, int b) {
	int p = board.getPositions();

	if(!p) return max[p][board] = 0;

	auto it = max[p].find(board);

	if(it != max[p].end()) return it->second;

	int result = INT_MIN;

	for(int x = 0; x < 8; ++x) {
		for(int y = 0; y < 8; ++y) {
			if(!board.can(x, y)) continue;
			for(int pi = 0; pi < 2; ++pi) {
				int pts = board.put(x, y, pi);
				int score = pts + p2(s + pts, a, b);
				board.rem(x, y);
				if(b < score + s) return score;
				if(result < score) result = score;
				if(a < score + s) a = score + s;
			}
		}
	}

	return max[p][board] = result;
}

int MCT :: p2(int s, int a, int b) {
	int p = board.getPositions();

	if(!p) return min[p][board] = 0;

	auto it = min[p].find(board);

	if(it != min[p].end()) return it->second;

	int result = INT_MAX;

	for(int x = 0; x < 8; ++x) {
		for(int y = 0; y < 8; ++y) {
			if(!board.can(x, y)) continue;
			for(int pi = 0; pi < 2; ++pi) {
				int pts = board.put(x, y, pi);
				int score = -pts + p1(s - pts, a, b);
				board.rem(x, y);
				if(a > score + s) return score;
				if(result > score) result = score;
				if(b > score + s) b = score + s;
			}
		}
	}
	
	return min[p][board] = result;
}

MCT :: ~MCT() {
	
}

void MCT :: print(std :: shared_ptr<Position>& p) {
	if(!p) return;
	board.print();
	std::cout<<'\n';
	for(auto [x, y, piece, points, next] : p->children) {
		board.put(x, y, piece);
		print(next);
		board.rem(x, y);
	}
}

void MCT :: print() {
    board.print();
}

void MCT :: build(std :: shared_ptr<Position>& p, int max) {
	
	if(board.getPositions() == 0) {
		p->result = 0;
		return;
	}

	p->result = max ? INT_MIN : INT_MAX;

	int x, y, pi;

	board.nextMove(x, y, pi);

	int points = board.put(x, y, pi);

	auto& chlist = p->children;
	std :: shared_ptr<Position> ch;

	auto it = std :: find_if(chlist.begin(), chlist.end(), [x,y,pi](Edge a) {
		return a.x == x && a.y == y && a.piece == pi;
	});

	if(it == chlist.end()) {
		ch.reset(new Position);
		ch->result = max ? INT_MAX : INT_MIN;
		chlist.push_back({x, y, pi, points, ch});
	} else ch = it->next;

	build(ch, max ^ 1);

	board.rem(x, y);

	for(auto [r, c, piece, points, next] : chlist) {
		if(max) p->result = std :: max(p->result, next->result + points);
		else p->result = std :: min(p->result, next->result - points);
	}
}

void MCT :: build(int iter) {
	while(iter--) build(root, 1);
}

int MCT :: do_next_move(int& x, int& y, int& pi) {
	int p = board.getPositions();

	if(p <= 9) {
		int result = p1(0, INT_MIN, INT_MAX);

		for(int i = 0; i < 8; ++i) {
			for(int j = 0; j < 8; ++j) {
				if(!board.can(i, j)) continue;
				for(int p = 0; p < 2; ++p) {
					int pts = board.put(i, j, p);
					auto& htab = min[board.getPositions()];
					auto it = htab.find(board);
					board.rem(i, j);
					if(it != htab.end() && it->second + pts == result) {
						x = i, y = j, pi = p;
						return board.put(i, j, p);
					}
				}
			}
		}

		std :: cout << result << ' ' << "erro: do_next_move não encontrou o próximo movimento\n";
		exit(1);
	}

	auto& chlist = root->children;

	auto it = std :: find_if(chlist.begin(), chlist.end(), [&](Edge a) {
		return a.points + a.next->result == root->result;
	});

	assert(it != chlist.end());

	x = it->x, y = it->y, pi = it->piece;

	auto cur = it->next;

	chlist.erase(it);

	root = cur;

	return board.put(x, y, pi);
}

int MCT :: move_to(int x, int y, int pi) {
	auto& chlist = root->children;

	auto it = std :: find_if(chlist.begin(), chlist.end(), [x,y,pi](Edge a) {
		return a.x == x && a.y == y && a.piece == pi;
	});

	int score = board.put(x, y, pi);

	if(it == chlist.end()) reset();
	else {
		auto cur = it->next;
		chlist.erase(it);
		root = cur;
	}

	return score;
}
