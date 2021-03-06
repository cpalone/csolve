#include <iostream>
#include <vector>
#include "Puzzle.hpp"

// R U F L D B
// 0 1 2 3 4 5
int base_move_e_ids[6][4] = { { 0, 11, 4, 8 },
                              { 3, 0, 1, 2 },
                              { 1, 8, 5, 9 },
                              { 2, 9, 6, 10 },
                              { 5, 4, 7, 6 },
                              { 3, 10, 7, 11 } };

Location base_move_e_perms[6][4] = {
    { { 11, 0 }, { 4, 0 }, { 8, 0 }, { 0, 0 } },
    { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } },
    { { 8, 1 }, { 5, 1 }, { 9, 1 }, { 1, 1 } },
    { { 9, 0 }, { 6, 0 }, { 10, 0 }, { 2, 0 } },
    { { 4, 0 }, { 7, 0 }, { 6, 0 }, { 5, 0 } },
    { { 10, 1 }, { 7, 1 }, { 11, 1 }, { 3, 1 } }
};

int base_move_c_ids[6][4] = { { 0, 3, 7, 4 },
                              { 2, 3, 0, 1 },
                              { 1, 0, 4, 5 },
                              { 2, 1, 5, 6 },
                              { 5, 4, 7, 6 },
                              { 3, 2, 6, 7 } };

Location base_move_c_perms[6][4] = {
    { { 3, 1 }, { 7, 2 }, { 4, 1 }, { 0, 2 } },
    { { 3, 0 }, { 0, 0 }, { 1, 0 }, { 2, 0 } },
    { { 0, 1 }, { 4, 2 }, { 5, 1 }, { 1, 2 } },
    { { 1, 1 }, { 5, 2 }, { 6, 1 }, { 2, 2 } },
    { { 4, 0 }, { 7, 0 }, { 6, 0 }, { 5, 0 } },
    { { 2, 1 }, { 6, 2 }, { 7, 1 }, { 3, 2 } }
};

Move new_move() {
    Move move;
    for (int i = 0; i < 8; i++) {
        move.corners[i] = Location{ char(i), 0 };
    }
    for (int i = 0; i < 12; i++) {
        move.edges[i] = Location{ char(i), 0 };
    }
    return move;
}

Move compose(const Move &a, const Move &b) {
    Move ab = new_move();
    for (int i = 0; i < 12; i++) {
        Location tmp = a.edges[i];
        tmp = b.edges[tmp.p];
        tmp.o = (tmp.o + a.edges[i].o) % 2;
        ab.edges[i] = tmp;
    };

    for (int i = 0; i < 8; i++) {
        Location tmp = a.corners[i];
        tmp = b.corners[tmp.p];
        tmp.o = (tmp.o + a.corners[i].o) % 3;
        ab.corners[i] = tmp;
    }
    return ab;
}

Move compose_seq(const std::vector<Move> &seq) {
    Move a;
    for (auto &i : seq) {
        a = compose(a, i);
    }
    return a;
}

std::vector<Move> get_3x3_q_turns() {
    std::vector<Move> base_moves;
    for (int i = 0; i < 6; i++) {
        Move tmp = new_move();
        for (int j = 0; j < 4; j++) {
            tmp.corners[base_move_c_ids[i][j]] = base_move_c_perms[i][j];
            tmp.edges[base_move_e_ids[i][j]] = base_move_e_perms[i][j];
        }
        base_moves.push_back(tmp);
    }

    for (int i = 0; i < 6; i++) {
        Move two = compose(base_moves[i], base_moves[i]);
        Move prime = compose(two, base_moves[i]);
        base_moves.push_back(prime);
    }
    return base_moves;
}

std::vector<Move> get_3x3_h_turns() {
    std::vector<Move> base_moves;
    for (int i = 0; i < 6; i++) {
        Move tmp = new_move();
        for (int j = 0; j < 4; j++) {
            tmp.corners[base_move_c_ids[i][j]] = base_move_c_perms[i][j];
            tmp.edges[base_move_e_ids[i][j]] = base_move_e_perms[i][j];
        }
        base_moves.push_back(tmp);
    }

    for (int i = 0; i < 6; i++) {
        Move two = compose(base_moves[i], base_moves[i]);
        Move prime = compose(two, base_moves[i]);
        base_moves.push_back(two);
        base_moves.push_back(prime);
    }
    return base_moves;
}

std::vector<Move> get_2x2_h3gen_turns() {
    std::vector<Move> base_moves;
    for (int i = 0; i < 3; i++) {
        Move tmp = new_move();
        for (int j = 0; j < 4; j++) {
            tmp.corners[base_move_c_ids[i][j]] = base_move_c_perms[i][j];
            tmp.edges[base_move_e_ids[i][j]] = base_move_e_perms[i][j];
        }
        base_moves.push_back(tmp);
    }

    for (int i = 0; i < 3; i++) {
        Move two = compose(base_moves[i], base_moves[i]);
        Move prime = compose(two, base_moves[i]);
        base_moves.push_back(two);
        base_moves.push_back(prime);
    }
    return base_moves;
}

Puzzle<3, 2> get_first_block() {
    Puzzle<3, 2> p;
    Piece c5 = { 5, { 5, 0 } };
    Piece c6 = { 6, { 6, 0 } };
    Piece e6 = { 6, { 6, 0 } };
    Piece e9 = { 9, { 9, 0 } };
    Piece e10 = { 10, { 10, 0 } };
    p.add_corner(0, c5);
    p.add_corner(1, c6);
    p.add_edge(0, e6);
    p.add_edge(1, e9);
    p.add_edge(2, e10);
    return p;
}

Puzzle<4, 0> get_cross() {
    Puzzle<4, 0> p;
    Piece e4 = { 4, { 4, 0 } };
    Piece e5 = { 5, { 5, 0 } };
    Piece e6 = { 6, { 6, 0 } };
    Piece e7 = { 7, { 7, 0 } };
    p.add_edge(0, e4);
    p.add_edge(1, e5);
    p.add_edge(2, e6);
    p.add_edge(3, e7);
    return p;
}

Puzzle<12, 8> get_full() {
    Puzzle<12, 8> p;
    for (char i = 0; i < 12; i++) {
        p.add_edge(i, Piece{ i, { i, 0 } });
    }
    for (char i = 0; i < 8; i++) {
        p.add_corner(i, Piece{ i, { i, 0 } });
    }
    return p;
}

Puzzle<1, 1> get_2x1() {
    Puzzle<1, 1> p;
    p.add_edge(0, Piece{ 6, { 6, 0 } });
    p.add_corner(0, Piece{ 5, { 5, 0 } });
    return p;
}

Puzzle<0, 8> get_corners() {
    Puzzle<0, 8> p;
    for (char i = 0; i < 8; i++) {
        p.add_corner(i, Piece{ i, { i, 0 } });
    }
    return p;
}

Puzzle<0, 4> get_4_corners() {
    Puzzle<0, 4> p;
    for (char i = 0; i < 4; i++) {
        p.add_corner(i, Piece{ i, { i, 0 } });
    }
    return p;
}

bool operator!=(const Piece a, const Piece b) {
    return !(a.id == b.id && a.loc.o == b.loc.o && a.loc.p == b.loc.p);
}
