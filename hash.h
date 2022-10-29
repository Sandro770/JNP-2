#ifndef HASH_H
#define HASH_H

#include <cstddef>
#include <cstdint>

using hash_function_t = uint64_t(*)(uint64_t*, size_t);

unsigned long hash_create(hash_function_t hash_function);
void hash_delete(unsigned long id);
size_t hash_size(unsigned long id);
bool hash_insert(unsigned long id, uint64_t const * seq, size_t size);
bool hash_remove(unsigned long id, uint64_t const * seq, size_t size);
void hash_clear(unsigned long id);
bool hash_test(unsigned long id, uint64_t const * seq, size_t size);

#endif