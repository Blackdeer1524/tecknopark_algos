#ifndef HASHMAP_H
#define HASHMAP_H

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <utility>
#include <vector>

// Вариант 2
// Для разрешения коллизий используйте двойное хеширование.

class HashMap {
 public:
    using hash_function_t = std::function<uint64_t(std::string)>;

    HashMap(hash_function_t main_hash_function,
            hash_function_t resolver_hash_function);

    HashMap(const HashMap &)                     = delete;
    HashMap(HashMap &&)                          = default;
    auto operator=(const HashMap &) -> HashMap & = delete;
    auto operator=(HashMap &&) -> HashMap      & = default;

    auto insert(std::string &&item) -> bool;

    auto contains(const std::string &key) -> bool;

    auto remove(const std::string &item) -> bool;

 private:
    using storage_t = std::vector<std::optional<std::string>>;

    static constexpr int    HASHTABLE_INIT_SIZE = 8;
    static constexpr double GROW_FRACTION       = 0.75;

    hash_function_t         main_hash_function_;
    hash_function_t         resolver_hash_function_;
    storage_t               storage_{HASHTABLE_INIT_SIZE};
    uint64_t                size_ = 0;

    auto                    resize() -> void;

    auto get_insertion_index(const std::string &item, const storage_t &src)
        -> std::pair<bool, uint64_t>;

    auto insert_with_resolution(std::string &&item, storage_t &dst) -> bool;
};

#endif  // !HASHMAP_H
