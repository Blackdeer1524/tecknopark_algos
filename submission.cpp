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

    auto find_item(const std::string &item, const storage_t &src)
        -> std::pair<bool, uint64_t>;
};

#endif  // !HASHMAP_H

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

HashMap::HashMap(hash_function_t main_hash_function,
                 hash_function_t resolver_hash_function)
    : main_hash_function_(std::move(main_hash_function)),
      resolver_hash_function_(std::move(resolver_hash_function)),
      storage_(HASHTABLE_INIT_SIZE), deleted_(HASHTABLE_INIT_SIZE) {
}

HashMap::Item::Item(std::optional<std::string> &&value)
    : value_(value), is_removed_(false) {
}

auto HashMap::Item::get_value_opt() const
    -> const std::optional<std::string> & {
    return value_;
}

auto HashMap::Item::is_removed() const -> bool {
    return is_removed_;
}

auto HashMap::Item::remove() -> void {
    value_.reset();
    is_removed_ = true;
}

auto HashMap::find_item(const std::string &item, const storage_t &src)
    -> std::pair<bool, uint64_t> {
    uint64_t insertion_index = main_hash_function_(item) % src.capacity();
    uint64_t step =
        std::max(resolver_hash_function_(item) % src.capacity(), 1zu);

    uint64_t first_empty_index = insertion_index;
    auto     found_empty       = false;
    for (uint64_t i = 0; i < src.capacity(); ++i) {
        auto insertion_place     = src.at(insertion_index);
        auto insertion_place_opt = insertion_place.get_value_opt();
        if (!insertion_place_opt.has_value()) {
            if (!insertion_place.is_removed()) {
                return {false, insertion_index};
            }
            if (!found_empty) {
                found_empty       = true;
                first_empty_index = insertion_index;
            }
        } else {
            auto value = insertion_place_opt.value();
            if (value == item) {
                return {true, insertion_index};
            }
        }
        insertion_index = (insertion_index + step) % src.capacity();
    }
    // сработает если в мапе все когда-то были удалены / заполнены
    return {!found_empty, first_empty_index};
}

// all right
auto HashMap::resize() -> void {
    storage_t new_storage(storage_.capacity() << 1);
    for (auto &item : storage_) {
        const auto &item_opt = item.get_value_opt();
        if (item_opt.has_value()) {
            auto val = item_opt.value();
            insert_with_resolution(std::move(val), new_storage);
        }
    }
    storage_ = std::move(new_storage);
}

auto HashMap::remove(const std::string &item) -> bool {
    if (storage_.empty()) {
        return true;
    }
    auto [found, index] = find_item(item, storage_);
    if (!found) {
        return true;
    }
    storage_.at(index).remove();
    --size_;
    return false;
}

auto HashMap::insert(std::string &&item) -> bool {
    if (static_cast<double>(size_) / static_cast<double>(storage_.capacity()) >=
        GROW_FRACTION) {
        resize();
    }
    auto found_existing = insert_with_resolution(std::move(item), storage_);
    if (found_existing) {
        return true;
    }
    ++size_;
    return false;
}

// Вариант 2
// Для разрешения коллизий используйте двойное хеширование.
auto HashMap::insert_with_resolution(std::string &&item, storage_t &dst)
    -> bool {
    auto [found_existing, index] = find_item(item, dst);
    if (found_existing) {
        return true;
    }
    dst.at(index) = std::move(HashMap::Item(std::move(item)));
    return false;
}

auto HashMap::contains(const std::string &key) -> bool {
    auto [found, _] = find_item(key, storage_);
    return found;
}

#include <iomanip>  // std::setw

auto HashMap::print() -> void {
    std::cout << "Map(" << size_ << ", " << storage_.capacity() << ")"
              << std::endl
              << "{";
    for (auto &item : storage_) {
        if (item.get_value_opt().has_value()) {
            std::cout << std::setw(4) << item.get_value_opt().value() << " ";
        } else {
            std::cout << std::setw(4) << 'X' << " ";
        }
    }
    std::cout << "}" << std::endl << "{";

    for (auto &item : storage_) {
        std::cout << std::setw(4) << item.is_removed() << " ";
    }
    std::cout << "}";
}

#include <cstdint>
#include <iostream>
#include <numeric>

auto hash1(const std::string &str) -> uint64_t {
    static uint64_t a   = 13;
    uint64_t        res = 0;
    for (unsigned char c : str) {
        res = res * a + c;
    }
    // return 0zu;
    return res;
}

auto hash2(const std::string &str) -> uint64_t {
    uint64_t res = 0;
    for (unsigned char c : str) {
        res = res + c;
    }
    // return 1;
    return 2 * res + 1;
}

auto main() -> int {
    auto map = HashMap(hash1, hash2);
    while (std::cin) {
        char op;
        std::cin >> op;
        if (!std::cin) {
            break;
        }
        std::string t;
        std::cin >> t;
        switch (op) {
            case '+': {
                auto flag = map.insert(std::move(t));
                if (flag) {
                    std::cout << "FAIL" << std::endl;
                } else {
                    std::cout << "OK" << std::endl;
                }
                // map.print();
                // std::cout << std::endl;
            } break;
            case '?': {
                auto flag = map.contains(t);
                if (flag) {
                    std::cout << "OK" << std::endl;
                } else {
                    std::cout << "FAIL" << std::endl;
                }
            } break;
            case '-': {
                auto flag = map.remove(t);
                if (flag) {
                    std::cout << "FAIL" << std::endl;
                } else {
                    std::cout << "OK" << std::endl;
                }
                // map.print();
                // std::cout << std::endl;
            } break;
            default:
                std::cout << "Unknown operation" << std::endl;
                break;
        }
    }
}
