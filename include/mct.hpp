#ifndef MCT_HPP
#define MCT_HPP

#include "board.hpp"
#include "position.hpp"
#include <memory>

class MCT {
	ht<Board, int, BHash> max[65], min[65];
    Board board;
    std :: shared_ptr<Position> root;

    void build(std :: shared_ptr<Position>&, int);
    void print(std :: shared_ptr<Position>&);
	void reset();
	int p1(int, int, int);
	int p2(int, int, int);

    public:

    MCT();
    MCT(int[][8]);

    ~MCT();

    void build(int);
    void print();
	int do_next_move(int&, int&, int&);
	int move_to(int, int, int);
};

#endif
