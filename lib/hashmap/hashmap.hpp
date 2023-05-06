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

    auto print() -> void;

 private:
    struct Item {
        explicit Item(std::optional<std::string> &&value);

        Item()                                 = default;
        Item(const Item &)                     = default;
        Item(Item &&)                          = default;
        auto operator=(const Item &) -> Item & = default;
        auto operator=(Item &&) -> Item      & = default;

        auto get_value_opt() const -> const std::optional<std::string> &;

        auto is_removed() const -> bool;

        auto remove() -> void;

     private:
        std::optional<std::string> value_;
        bool                       is_removed_;
    };

    using storage_t                             = std::vector<Item>;

    static constexpr int    HASHTABLE_INIT_SIZE = 8;
    static constexpr double GROW_FRACTION       = 0.75;
    hash_function_t         main_hash_function_;
    hash_function_t         resolver_hash_function_;
    storage_t               storage_;
    std::vector<bool>       deleted_;
    uint64_t                size_ = 0;

    auto                    resize() -> void;

    auto get_insertion_index(const std::string &item, const storage_t &src)
        -> bool;

    auto insert_with_resolution(std::string &&item, storage_t &dst) -> bool;

    auto find_item(const std::string &item) -> std::pair<bool, uint64_t>;
};

#endif  // !HASHMAP_H
