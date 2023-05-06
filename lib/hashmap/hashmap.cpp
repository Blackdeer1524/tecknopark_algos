#include "hashmap.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <utility>

HashMap::HashMap(hash_function_t main_hash_function,
                 hash_function_t resolver_hash_function)
    : main_hash_function_(std::move(main_hash_function)),
      resolver_hash_function_(std::move(resolver_hash_function)) {
}

auto HashMap::resize() -> void {
    storage_t new_storage{storage_.size() << 1};
    for (auto &item : storage_) {
        if (item.has_value()) {
            auto val = std::exchange(item, std::nullopt).value();
            insert_with_resolution(std::move(val), new_storage);
        }
    }
    storage_ = std::move(new_storage);
}

auto HashMap::remove(const std::string &item) -> bool {
    if (storage_.empty()) {
        return true;
    }
    auto [found, index] = get_insertion_index(item, storage_);
    if (!found) {
        return true;
    }
    auto &found_item = storage_.at(index);
    std::exchange(found_item, std::nullopt);
    --size_;
    return false;
}

auto HashMap::insert(std::string &&item) -> bool {
    if (static_cast<double>(size_) / static_cast<double>(storage_.size()) >=
        GROW_FRACTION) {
        resize();
    }
    ++size_;
    return insert_with_resolution(std::move(item), storage_);
}

auto HashMap::get_insertion_index(const std::string &item, const storage_t &src)
    -> std::pair<bool, uint64_t> {
    uint64_t insertion_index = main_hash_function_(item) % src.size();
    uint64_t step            = resolver_hash_function_(item) % src.size();
    while (true) {
        auto insertion_place = src.at(insertion_index);
        if (!insertion_place.has_value()) {
            return std::make_pair(false, insertion_index);
        }
        auto value = insertion_place.value();
        if (value == item) {
            break;
        }
        insertion_index = (insertion_index + step) % src.size();
    }
    return std::make_pair(true, 0);
}

auto HashMap::insert_with_resolution(std::string &&item, storage_t &dst)
    -> bool {
    // Вариант 2
    // Для разрешения коллизий используйте двойное хеширование.
    auto [found, insertion_index] = get_insertion_index(item, dst);
    if (found) {
        return true;
    }
    dst.at(insertion_index) = item;
    return false;
}

auto HashMap::contains(const std::string &key) -> bool {
    auto [found, _] = get_insertion_index(key, storage_);
    return found;
}
