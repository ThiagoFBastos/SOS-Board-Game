#include "mct.hpp"
#include <random>
#include <chrono>
#include <tuple>
#include <climits>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <queue>
#include <array>
#include <iterator>

#define INF 1'000'000'000

MCT :: MCT() {

}

MCT :: MCT(const char* filename, int table[][8]) {
	for(int i = 0; i < 8; ++i) {
		for(int j = 0; j < 8; ++j) {
			if(table[i][j] != -1)
				board.put(i, j, table[i][j]);
		}
	}

	std :: ifstream fs(filename, std :: ios_base :: binary);

	if(fs.is_open()) {

		struct RECORD {
			int result;
			int x;
			int y;
			int piece;
			int points;
			int firstChild;
			int sibiling;
		};

		/*
			[result | x | y | piece | points | firstChild | sibiling]
		*/

		auto loadRecord = [&fs](int u)->RECORD {
			unsigned recordBytes = 7 * 4;
			RECORD record;
			fs.seekg(recordBytes * u, std :: ios_base :: beg);
			fs.read((char*)&record.result, 4);
			fs.read((char*)&record.x, 4);
			fs.read((char*)&record.y, 4);
			fs.read((char*)&record.piece, 4);
			fs.read((char*)&record.points, 4);
			fs.read((char*)&record.firstChild, 4);
			fs.read((char*)&record.sibiling, 4);
			return record;
		};

		auto dfs = [&](auto& self, int u)->std :: shared_ptr<Position> {
			auto r = loadRecord(u);

			std :: shared_ptr<Position> ptr(new Position {r.result, {}});

			for(int v = r.firstChild; v != -1; ) {
				auto rC = loadRecord(v);
				ptr->children.push_back({rC.x, rC.y, rC.piece, rC.points, self(self, v)});
				v = rC.sibiling;
			}

			return ptr;
		};

		root = dfs(dfs, 0);

		fs.close();
	} else {
		build(10000);
		persist("data/start_tree.dat");
		system("cp -f data/start_tree.dat data/cache_tree.dat");
	}
}

MCT :: MCT(int iter) {
	build(iter);
}

int MCT :: p1(int s, int a, int b) {
	int p = board.getPositions();

	if(!p) return max[p][board] = 0;

	auto it = max[p].find(board);

	if(it != max[p].end()) return it->second;

	int result = -INF;

	for(int x = 0; x < 8; ++x) {
		for(int y = 0; y < 8; ++y) {
			if(!board.can(x, y)) continue;
			for(int pi = 0; pi < 2; ++pi) {
				int pts = board.put(x, y, pi);
				int score = pts + p2(s + pts, a, b);
				board.rem(x, y);
				if(b < score + s) return INF;
				if(result < score) result = score;
				if(a < score + s) a = score + s;
			}
		}
	}

	if(result <= 4 - INF) return INF;

	return max[p][board] = result;
}

int MCT :: p2(int s, int a, int b) {
	int p = board.getPositions();

	if(!p) return min[p][board] = 0;

	auto it = min[p].find(board);

	if(it != min[p].end()) return it->second;

	int result = INF;

	for(int x = 0; x < 8; ++x) {
		for(int y = 0; y < 8; ++y) {
			if(!board.can(x, y)) continue;
			for(int pi = 0; pi < 2; ++pi) {
				int pts = board.put(x, y, pi);
				int score = -pts + p1(s - pts, a, b);
				board.rem(x, y);
				if(a > score + s) return -INF;
				if(result > score) result = score;
				if(b > score + s) b = score + s;
			}
		}
	}

	if(result >= INF - 4) return -INF;

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

	p->result = max ? -INF : INF;

	int x, y, pi;

	board.nextMove(x, y, pi);

	int points = board.put(x, y, pi);

	auto& chlist = p->children;
	std :: shared_ptr<Position> ch;

	auto it = std :: find_if(chlist.begin(), chlist.end(), [x, y, pi](const Edge& a) {
		return a.x == x && a.y == y && a.piece == pi;
	});

	if(it == chlist.end()) {
		ch.reset(new Position);
		ch->result = max ? INF : -INF;
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
	root.reset(new Position);
	root->result = -INF;
	while(iter--) build(root, 1);
}

int MCT :: do_next_move(int& x, int& y, int& pi) {
	int p = board.getPositions();

	if(p <= MINIMAXN) {
		int result = p1(0, -INF, INF);

		#ifdef DEBUG
		if(p == MINIMAXN || p == MINIMAXN - 1) {
			int tot = 0;
			for(int i = 0; i <= 64; ++i) tot += max[i].size() + min[i].size();
			std :: cout << "ht size = " << tot << '\n';
		}
		#endif

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
	int score = board.put(x, y, pi);

	if(board.getPositions() > MINIMAXN) {
		auto& chlist = root->children;

		#ifdef DEBUG
		std :: cout << "tamanho da lista = " << chlist.size() << '\n';
		#endif

		auto it = std :: find_if(chlist.begin(), chlist.end(), [x, y, pi](Edge a) {
			return a.x == x && a.y == y && a.piece == pi;
		});

		if(it == chlist.end()) {
			#ifdef DEBUG
			std :: cout << "tive que construir de novo\n";
			#endif
			build(936);
		} else {
			auto cur = it->next;
			chlist.erase(it);
			root = cur;
		}

		build(64);
	} else
		root.reset();

	return score;
}

void MCT :: persist(const char* filename) {
	if(!root) {
		#ifdef DEBUG
		std :: cout << "a raíz é nula\n";
		#endif
		return;
	}
	std :: ofstream fs(filename, std :: ios_base :: binary);
	int cur = 1;

	assert(fs.is_open());

	struct RECORD {
		std :: shared_ptr<Position> no;
		int x;
		int y;
		int piece;
		int points;
		int sibiling;
	};

	std :: queue<RECORD> q;

	q.push({
		root,
		-1,
		-1,
		-1,
		-1,
		-1
	});

	/*
		[result | x | y | piece | points | firstChild | sibiling]
	*/

	while(!q.empty()) {
		auto [ptr, x, y, piece, points, sibiling] = q.front();

		q.pop();

		fs.write((const char*)&ptr->result, 4);
		fs.write((const char*)&x, 4);
		fs.write((const char*)&y, 4);
		fs.write((const char*)&piece, 4);
		fs.write((const char*)&points, 4);

		auto& adj = ptr->children;

		int firstChild = adj.empty() ? -1 : cur;

		fs.write((const char*)&firstChild, 4);
		fs.write((const char*)&sibiling, 4);

		for(int i = 0; i < (int)adj.size(); ++i) {
			auto& [x, y, piece, points, child] = adj[i];

			q.push({
				child,
				x,
				y,
				piece,
				points,
				i + 1 < (int)adj.size() ? cur + 1 : -1
			});

			++cur;
		}
	}

	fs.close();
}
