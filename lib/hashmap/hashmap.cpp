#include "hashmap.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <optional>
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
    uint64_t initial_hash    = insertion_index;
    uint64_t step =
        std::max(resolver_hash_function_(item) % src.capacity(), 1zu);
    for (uint64_t i = 0; i < src.capacity(); ++i) {
        auto insertion_place     = src.at(insertion_index);
        auto insertion_place_opt = insertion_place.get_value_opt();

        if (!insertion_place_opt.has_value()) {
            if (!insertion_place.is_removed()) {
                return {false, insertion_index};
            }
        } else {
            auto value = insertion_place_opt.value();
            if (value == item) {
                return {true, insertion_index};
            }
        }
        insertion_index = (insertion_index + step) % src.size();
    }
    return {false, initial_hash};
}

auto HashMap::resize() -> void {
    storage_t new_storage{storage_.capacity() << 1};
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
