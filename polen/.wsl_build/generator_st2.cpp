#include <bits/stdc++.h>
using namespace std;

const string fn("polen");

ofstream out(fn + ".in");

#define cout out

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

/// cele fixate
int C = 2;
/// cele random
int N, M, Q, r, c, K, val, cheie;
vector<pair<int, int>> hard_c1_positions;
int hard_c1_pos_idx = 0;
vector<tuple<int, int, int>> hard_c3_inside_queries;
int hard_c3_inside_idx = 0;
vector<pair<int, int>> hard_c3_medium_positions;
int hard_c3_medium_pos_idx = 0;
bool force_safe_int_values = false;

bool is_small_subtask()
{
    return C == 1 || C == 4 || C == 7;
}

bool is_medium_subtask()
{
    return C == 2 || C == 5 || C == 8;
}

bool is_large_subtask()
{
    return C == 3 || C == 6 || C == 9 || C == 10;
}

bool is_c1_variant()
{
    return C >= 1 && C <= 3;
}

bool is_c1_hard_subtask()
{
    return C == 3;
}

bool is_c2_variant()
{
    return C >= 4 && C <= 6;
}

bool is_c2_hard_subtask()
{
    return C == 6;
}

bool allows_outside_matrix()
{
    return C == 10;
}

bool is_c3_hard_inside_subtask()
{
    return C == 9;
}

bool is_c3_hard_outside_subtask()
{
    return C == 10;
}

bool is_c3_medium_center_distinct_subtask()
{
    return C == 8;
}

bool use_centered_c3_hard_mode()
{
    const char *mode = getenv("POLEN_C3_MODE");
    if (mode == nullptr)
    {
        uniform_int_distribution<int> dist(0, 1);
        return dist(rng);
    }
    return string(mode) == "centered";
}

int get_rand(int L, int R)
{
    uniform_int_distribution<int> dist(L, R);
    return dist(rng);
}

int safe_value_limit()
{
    long long limit = INT_MAX / max(1LL, 4LL * Q);
    limit = min(limit, 1000000000LL);
    return (int)max(1LL, limit);
}

int pick_value()
{
    if (force_safe_int_values)
    {
        int limit = safe_value_limit();
        int type = get_rand(0, 4);
        if (type == 0)
            return limit;
        if (type == 1)
            return -limit;
        if (type == 2)
        {
            int small = min(limit, 1000);
            return get_rand(-small, small);
        }
        return get_rand(-limit, limit);
    }

    int type = get_rand(0, 4);
    if (type == 0)
        return 1000000000;
    if (type == 1)
        return -1000000000;
    if (type == 2)
        return get_rand(-1000, 1000);
    return get_rand(-1000000000, 1000000000);
}

int pick_centered_coord(int limit)
{
    int center = (limit + 1) / 2;
    int radius = max(0, limit / 10);
    if (is_medium_subtask())
        radius = max(0, limit / 40);
    if (is_large_subtask())
        radius = max(0, limit / 100);
    int L = max(1, center - radius);
    int R = min(limit, center + radius);
    return get_rand(L, R);
}

int pick_large_k(int limit)
{
    int slack = max(0, limit / 10);
    if (is_medium_subtask())
        slack = max(5, limit / 100);
    if (is_large_subtask())
        slack = max(2, limit / 250);
    int L = max(1, limit - slack);
    return get_rand(L, limit);
}

int pick_close_to_target(int L, int R, int target)
{
    if (L == R)
        return L;
    int radius = max(0, (R - L) / 10);
    int left = max(L, target - radius);
    int right = min(R, target + radius);
    return get_rand(left, right);
}

int pick_c3_hard_outside_mask()
{
    int type = get_rand(0, 99);
    if (type < 20)
    {
        const int masks[] = {1, 2, 4, 8};
        return masks[get_rand(0, 3)];
    }
    if (type < 55)
    {
        const int masks[] = {3, 5, 6, 9, 10, 12};
        return masks[get_rand(0, 5)];
    }
    if (type < 85)
    {
        const int masks[] = {7, 11, 13, 14};
        return masks[get_rand(0, 3)];
    }
    return 15;
}

int pick_c3_hard_inside_mask()
{
    int type = get_rand(0, 99);
    if (type < 25)
    {
        const int masks[] = {1, 2, 4, 8};
        return masks[get_rand(0, 3)];
    }
    if (type < 65)
    {
        const int masks[] = {3, 5, 6, 9, 10, 12};
        return masks[get_rand(0, 5)];
    }

    const int masks[] = {7, 11, 13, 14};
    return masks[get_rand(0, 3)];
}

int max_k_inside_matrix(int qr, int qc, int mask)
{
    if (mask == 0)
        mask = 15;

    int limit = max(N, M);
    if (mask & 1)
        limit = min(limit, min(qr - 1, M - qc));
    if (mask & 2)
        limit = min(limit, min(N - qr, qc - 1));
    if (mask & 4)
        limit = min(limit, min(qr - 1, qc - 1));
    if (mask & 8)
        limit = min(limit, min(N - qr, M - qc));

    return max(0, limit);
}

int pick_c3_hard_inside_k(int required_cells)
{
    int limit = min(N, M) / 2 - 1;
    for (int cand = limit; cand >= 1; cand--)
        if (1LL * (N - 2 * cand) * (M - 2 * cand) >= required_cells)
            return cand;
    return 1;
}

void pick_query_inside_matrix(int mask)
{
    for (int tries = 0; tries < 50; tries++)
    {
        int qr = pick_centered_coord(N);
        int qc = pick_centered_coord(M);
        int max_k = max_k_inside_matrix(qr, qc, mask);
        if (max_k >= 1)
        {
            r = qr;
            c = qc;
            K = pick_large_k(max_k);
            return;
        }
    }

    r = (N + 1) / 2;
    c = (M + 1) / 2;
    K = pick_large_k(max_k_inside_matrix(r, c, mask));
}

void print_query(int qr, int qc, int qK, int qVal, int qKey)
{
    cout << qr << ' ' << qc << ' ' << qK << ' ' << qVal << ' ' << qKey << '\n';
}

void append_c1_hard_layers(int max_coord, int last_layer, int &remaining_pairs)
{
    last_layer = min(last_layer, max_coord);
    for (int layer = 1; layer <= last_layer && remaining_pairs > 0; layer++)
    {
        for (int left = layer; left <= max_coord && remaining_pairs > 0; left++)
        {
            hard_c1_positions.push_back({layer, left});
            remaining_pairs--;
        }
        for (int top = layer + 1; top <= max_coord && remaining_pairs > 0; top++)
        {
            hard_c1_positions.push_back({top, layer});
            remaining_pairs--;
        }
    }
}

void build_c1_hard_positions()
{
    hard_c1_positions.clear();
    hard_c1_pos_idx = 0;

    int remaining_pairs = Q / 2;
    int max_coord = max(1, min(N, M) / 2);

    for (int rep = 0; rep < 11 && remaining_pairs > 0; rep++)
        append_c1_hard_layers(max_coord, 1, remaining_pairs);

    for (int depth = max_coord; depth >= 1 && remaining_pairs > 0; depth /= 2)
        append_c1_hard_layers(max_coord, depth, remaining_pairs);
}

void push_c3_inside_query(int qr, int qc, int qK)
{
    hard_c3_inside_queries.push_back({qr, qc, qK});
}

void append_c3_inside_full_box(int qK)
{
    for (int qr = qK + 1; qr <= N - qK; qr++)
        for (int qc = qK + 1; qc <= M - qK; qc++)
            push_c3_inside_query(qr, qc, qK);
}

void append_c3_inside_ring(int qK)
{
    int r1 = qK + 1, r2 = N - qK;
    int c1 = qK + 1, c2 = M - qK;

    for (int qc = c1; qc <= c2; qc++)
        push_c3_inside_query(r1, qc, qK);
    for (int qr = r1 + 1; qr <= r2; qr++)
        push_c3_inside_query(qr, c2, qK);
    if (r1 < r2)
        for (int qc = c2 - 1; qc >= c1; qc--)
            push_c3_inside_query(r2, qc, qK);
    if (c1 < c2)
        for (int qr = r2 - 1; qr > r1; qr--)
            push_c3_inside_query(qr, c1, qK);
}

void build_c3_hard_inside_queries()
{
    hard_c3_inside_queries.clear();
    hard_c3_inside_idx = 0;

    int start_k = max(1, min(N, M) / 6);
    append_c3_inside_full_box(start_k);
    for (int qK = start_k - 1; qK >= 1; qK--)
        append_c3_inside_ring(qK);
}

void build_c3_medium_positions()
{
    hard_c3_medium_positions.clear();
    hard_c3_medium_pos_idx = 0;

    int side = 71;
    int row_start = (N - side) / 2 + 1;
    int col_start = (M - side) / 2 + 1;
    int mod = side * side;
    int step = mod - 1;
    int offset = get_rand(0, mod - 1);

    for (int i = 0; i < Q; i++)
    {
        int pos = (offset + 1LL * step * i) % mod;
        int qr = row_start + pos / side;
        int qc = col_start + pos % side;
        hard_c3_medium_positions.push_back({qr, qc});
    }
}

void generate_c1_pair()
{
    int top, left, signal;

    if (is_c1_hard_subtask())
    {
        if (hard_c1_pos_idx >= (int)hard_c1_positions.size())
            hard_c1_pos_idx = 0;
        tie(top, left) = hard_c1_positions[hard_c1_pos_idx++];
        K = min(N - top, M - left);
        signal = get_rand(0, 3);
    }
    else
    {
        int max_pair_k = min(N, M) - 1;
        int low_pair_k = max(2, max_pair_k - max(1, max_pair_k / 10));
        if (is_medium_subtask())
            low_pair_k = max(2, max_pair_k - max(5, max_pair_k / 100));
        if (is_large_subtask())
            low_pair_k = max(2, max_pair_k - max(2, max_pair_k / 250));

        K = get_rand(low_pair_k, max_pair_k);
        top = pick_close_to_target(1, N - K, (N - K + 1) / 2);
        left = pick_close_to_target(1, M - K, (M - K + 1) / 2);
        signal = get_rand(0, 3);
    }

    val = pick_value();

    if (signal == 0)
    {
        print_query(top + K, left, K, val, 1);
        print_query(top, left + K, K - 1, val, 2);
    }
    else if (signal == 1)
    {
        print_query(top, left + K, K, val, 2);
        print_query(top + K, left, K - 1, val, 1);
    }
    else if (signal == 2)
    {
        print_query(top + K, left + K, K, val, 4);
        print_query(top, left, K - 1, val, 8);
    }
    else
    {
        print_query(top, left, K, val, 8);
        print_query(top + K, left + K, K - 1, val, 4);
    }
}

signed main()
{
    cout << C << '\n';
    uniform_int_distribution<int> key(0, 15);
    int hard_k = -1, hard_row_start = -1, hard_col_start = -1;
    int hard_height = -1, hard_width = -1, hard_mod = -1, hard_step = -1, hard_offset = -1;
    bool centered_hard_mode = false;
    force_safe_int_values = (getenv("POLEN_SAFE_INT") != nullptr);

    if (is_c3_hard_inside_subtask())
    {
        N = 1000;
        M = 1000;
        build_c3_hard_inside_queries();
        Q = (int)hard_c3_inside_queries.size();
    }
    else if (is_c3_hard_outside_subtask())
    {
        centered_hard_mode = use_centered_c3_hard_mode();
        N = get_rand(980, 1000);
        M = get_rand(980, 1000);
        hard_k = min(N, M) - 1;

        if (centered_hard_mode)
        {
            int low_h = max(1, N / 2 - N / 12);
            int high_h = min(N, N / 2 + N / 12);
            int low_w = max(1, M / 2 - M / 12);
            int high_w = min(M, M / 2 + M / 12);
            hard_height = get_rand(low_h, high_h);
            hard_width = get_rand(low_w, high_w);
            hard_row_start = (N - hard_height) / 2 + 1;
            hard_col_start = (M - hard_width) / 2 + 1;
        }
        else
        {
            hard_row_start = 1;
            hard_col_start = 1;
            hard_height = N;
            hard_width = M;
        }

        hard_mod = hard_height * hard_width;
        Q = hard_mod;
        hard_step = (hard_mod == 1 ? 0 : hard_mod - 1);
        hard_offset = get_rand(0, hard_mod - 1);
    }
    else if (is_c1_hard_subtask())
    {
        N = 1000;
        M = 1000;
        Q = 1000000;
        build_c1_hard_positions();
    }
    else if (is_c2_hard_subtask())
    {
        N = 1000;
        M = 1000;
        hard_height = N - 2;
        hard_width = M - 2;
        hard_row_start = 2;
        hard_col_start = 2;
        hard_mod = hard_height * hard_width;
        Q = hard_mod;
        hard_step = hard_mod - 1;
        hard_offset = get_rand(0, hard_mod - 1);
    }
    else if (is_c3_medium_center_distinct_subtask())
    {
        N = 1000;
        M = 1000;
        Q = 5000;
        build_c3_medium_positions();
    }
    else if (is_small_subtask())
    {
        N = get_rand(180, 200);
        M = get_rand(180, 200);
        Q = is_c1_variant() ? 200 : get_rand(180, 200);
        if (is_c1_variant() && Q % 2 == 1)
            Q--;
    }
    else if (is_medium_subtask())
    {
        N = get_rand(950, 1000);
        M = get_rand(950, 1000);
        Q = get_rand(4900, 5000);
        if (is_c1_variant() && Q % 2 == 1)
            Q--;
    }
    else if (is_large_subtask())
    {
        N = get_rand(980, 1000);
        M = get_rand(980, 1000);
        Q = get_rand(950000, 1000000);
        if (is_c1_variant() && Q % 2 == 1)
            Q--;
    }

    cout << N << ' ' << M << ' ' << Q << '\n';

    for (int i = 1; i <= Q; i++)
    {
        if (is_c1_variant())
        {
            generate_c1_pair();
            i++;
            continue;
        }

        val = pick_value();

        if (is_c3_hard_inside_subtask())
        {
            tie(r, c, K) = hard_c3_inside_queries[hard_c3_inside_idx++];
            static const int hard_masks[] = {7, 11, 13, 14};
            cheie = hard_masks[(i - 1) & 3];
        }
        else if (is_c3_hard_outside_subtask())
        {
            int pos = (hard_offset + 1LL * hard_step * (i - 1)) % hard_mod;
            r = hard_row_start + pos / hard_width;
            c = hard_col_start + pos % hard_width;
            K = hard_k;
            cheie = pick_c3_hard_outside_mask();
        }
        else
        {
            cheie = (is_c2_variant() ? 15 : key(rng));
            if (is_c2_hard_subtask())
            {
                int pos = (hard_offset + 1LL * hard_step * (i - 1)) % hard_mod;
                r = hard_row_start + pos / hard_width;
                c = hard_col_start + pos % hard_width;
                K = max_k_inside_matrix(r, c, cheie);
            }
            else if (is_c3_medium_center_distinct_subtask())
            {
                tie(r, c) = hard_c3_medium_positions[hard_c3_medium_pos_idx++];
                cheie = get_rand(1, 15);
                K = max_k_inside_matrix(r, c, cheie);
            }
            else if (allows_outside_matrix())
            {
                r = pick_centered_coord(N);
                c = pick_centered_coord(M);
                K = pick_large_k(max(N, M));
            }
            else
                pick_query_inside_matrix(cheie);
        }

        print_query(r, c, K, val, cheie);
    }
    return 0;
}
