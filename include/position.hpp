#ifndef POSITION_HPP
#define POSITION_HPP

#include <ext/pb_ds/assoc_container.hpp>
#include <vector>
#include <memory>

using namespace __gnu_pbds;

template <class K, class V, class H>
using ht = gp_hash_table
<K, V, H, std :: equal_to<K>,
direct_mask_range_hashing<>,
linear_probe_fn<>,
hash_standard_resize_policy<
hash_exponential_size_policy<>,
hash_load_check_resize_trigger
<>, true>>;

struct Position;

struct Edge {
    int x, y, piece, points;
    std :: shared_ptr<Position> next;
};

struct Position {
    int result;
    std :: vector<Edge> children;
};

#endif
