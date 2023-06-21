#include <algorithm>
#include <array>
#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <ostream>
#include <queue>
#include <stdexcept>
#include <vector>

struct Position {
    Position(int64_t row, int64_t col) : row_(row), col_(col) {
    }

    [[nodiscard]] auto row() const -> int64_t {
        return row_;
    }

    [[nodiscard]] auto col() const -> int64_t {
        return col_;
    }

 private:
    int64_t row_;
    int64_t col_;
};

using Field_T = std::array<std::array<int64_t, 4>, 4>;

enum class SwapDirection { UP, DOWN, LEFT, RIGHT };

auto swap_direction_to_str(SwapDirection dir) -> char {
    switch (dir) {
        case SwapDirection::UP:
            return 'U';
        case SwapDirection::DOWN:
            return 'D';
        case SwapDirection::LEFT:
            return 'L';
        case SwapDirection::RIGHT:
            return 'R';
    }
    throw std::runtime_error("Unknown SwapDirection");
}

// Состояние поля
struct State {
    State(Field_T field, Position empty_tile_pos)
        : field(field), empty_tile_pos(empty_tile_pos) {
    }

    Field_T                    field;
    Position                   empty_tile_pos;
    std::vector<SwapDirection> steps{};

    auto                       operator<(const State &other) const -> bool {
        auto this_heuristic = steps.size() + heuristic();
        auto other_heuristic = other.steps.size() + other.heuristic();
        return this_heuristic > other_heuristic;
    }

    [[nodiscard]] auto heuristic() const -> int64_t {
        int64_t res = 0;
        for (int64_t i = 0; i < 4; ++i) {
            for (int64_t j = 0; j < 4; ++j) {
                if (field[i][j] == 0) {
                    continue;
                }
                int64_t i2  = (field[i][j] - 1) / 4;
                int64_t j2  = (field[i][j] - 1) % 4;
                res        += abs(i - i2) + abs(j - j2);
            }
        }
        return res;
    }

    [[nodiscard]] auto solved() const -> bool {
        return heuristic() == 0;
    }
};

// Хранение посещенных состояний
std::map<Field_T, bool>                  visited;

std::vector<std::pair<int64_t, int64_t>> moves = {
    {-1,  0},
    { 1,  0},
    { 0, -1},
    { 0,  1}
};

// Проверка возможности хода
auto valid_move(int64_t i, int64_t j) -> bool {
    return i >= 0 && j >= 0 && i < 4 && j < 4;
}

// Выполнение хода
auto make_move(State s, Position empty, Position actual) -> State {
    std::swap(s.field[empty.row()][empty.col()],
              s.field[actual.row()][actual.col()]);

    auto row_diff = actual.row() - empty.row();
    auto col_diff = actual.col() - empty.col();

    if (row_diff < 0) {
        s.steps.emplace_back(SwapDirection::DOWN);
    } else if (row_diff > 0) {
        s.steps.emplace_back(SwapDirection::UP);
    } else if (col_diff > 0) {
        s.steps.emplace_back(SwapDirection::LEFT);
    } else {
        s.steps.emplace_back(SwapDirection::RIGHT);
    }
    s.empty_tile_pos = actual;
    return s;
}

auto solve(State start) -> std::optional<std::vector<SwapDirection>> {
    std::priority_queue<State> pq;

    pq.push(start);
    visited[start.field] = true;

    while (!pq.empty()) {
        State cur = pq.top();
        pq.pop();

        auto &empty_tile_pos = cur.empty_tile_pos;

        if (cur.solved()) {
            return cur.steps;
        }

        for (auto &move : moves) {
            int64_t ni = empty_tile_pos.row() + move.first;
            int64_t nj = empty_tile_pos.col() + move.second;
            if (valid_move(ni, nj)) {
                State next = make_move(cur, empty_tile_pos, {ni, nj});
                if (!visited[next.field]) {
                    visited[next.field] = true;
                    pq.push(next);
                }
            }
        }
    }

    // Решение не найдено
    return std::nullopt;
}

auto main() -> int {
    // int64_t test[4][4]{
    //     { 1,  2,  3,  4},
    //     { 5,  6,  7,  8},
    //     { 9, 10, 11, 12},
    //     {13, 14, 15,  0}
    // };

    Field_T  field;
    Position empty{0, 0};
    // start.field = std::array<std::array<int64_t, 4>, 4>{};
    for (int64_t i = 0; i < 4; ++i) {
        for (int64_t j = 0; j < 4; ++j) {
            std::cin >> field[i][j];
            // field[i][j] = test[i][j];
            if (field[i][j] == 0) {
                empty = {i, j};
            }
        }
    }
    State start{field, empty};
    auto  res_option = solve(start);
    if (!res_option.has_value()) {
        std::cout << "-1" << std::endl;
        return 0;
    }
    auto res = res_option.value();
    std::cout << res.size() << std::endl;
    for (auto direction : res) {
        std::cout << swap_direction_to_str(direction) << ' ';
    }

    return 0;
}
