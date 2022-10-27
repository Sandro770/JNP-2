#include "hash.h"
#include <unordered_map>

using hash_function_id_t = unsigned long;

using {
    using pair_seq_size_t = std::pair<int64_t*, size_t>;
    using hash_table_t = std::unordered_set<pair_seq_size_t>;
    size_t numberOfCreatedHashes = 0;//może static?
    size_t numberOfRemovedHashes = 0;
    std::unordered_map<hash_function_id_t , hash_table_t> hash_tables;
}

hash_function_id_t hash_create(hash_function_t hash_function) {
    numberOfCreatedHashes++;
    std::unordered_set<pair_seq_size_t, decltype(hash_function)> hash_table_t(5, hash_function);
    hash_tables[numberOfCreatedHashes] = set;

    return numberOfCreatedHashes;
}

void hash_delete(hash_function_id_t id) {
  hash_functions.erase(id);
}

size_t hash_size(unsigned long id) {
    return hash_tables.size();
}

bool hash_insert(unsigned long id, uint64_t const * seq, size_t size) {
    return true;
}
bool hash_remove(unsigned long id, uint64_t const * seq, size_t size) {
  return true;
}
void hash_clear(unsigned long id) {
  return true;
}
bool hash_test(unsigned long id, uint64_t const * seq, size_t size) {
  return true;
}
