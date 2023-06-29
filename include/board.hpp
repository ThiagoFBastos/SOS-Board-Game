#ifndef BOARD_HPP
#define BOARD_HPP

#define EMPTY -1
#define S 0
#define O 1

#include <cstddef>
#include <cstdint>

class Board;

struct BHash {
    size_t operator()(const Board&) const;
};

class Board {
    friend struct BHash;

    char tab[8][8];
    char positions;

    bool check(int, int) const;
    bool checkODistraction(int, int) const;
    bool checkSDistraction(int, int) const;

    public:

    Board();

    void nextMove(int&, int&, int&);
    bool can(int, int) const;
    int getPositions() const;
    int put(int, int, int);
    void rem(int, int);
    int getPoints(int, int) const;
    bool operator==(const Board&) const;
    void print() const;
};

#endif
