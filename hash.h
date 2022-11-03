#ifndef HASH_H
#define HASH_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
#include <iostream>
using hash_function_t = uint64_t(*)(uint64_t const *, size_t);
namespace jnp1 {
extern "C" {
#else
typedef uint64_t (*hash_function_t)(uint64_t const *, size_t);
#endif


/// @brief Creates hash table
/// @param hash_function - pointer to hash function used in a created hash table
/// @return id of created hash table
unsigned long hash_create(hash_function_t hash_function);

/// @brief Deletes hash table
/// @param id - id of hash table that will be deleted
void hash_delete(unsigned long id);

/// @brief Determines size of hash table
/// @param id - id of hash table that is considered
/// @return size of considered hash table or 0, if such table does not exist
size_t hash_size(unsigned long id);

/// @brief Inserts sequence into hash table
/// Inserts sequence into hash table and provides information if the operation
/// was successful
/// @param id - id of considered hash table
/// @param seq - pointer on sequence that will be inserted
/// @param size - length of the @p seq
/// @return false if seq equals NULL or size equals 0, otherwise true
bool hash_insert(unsigned long id, uint64_t const *seq, size_t size);

/// @brief Removes sequence from hash table
/// @param id - id of considered hash table
/// @param seq - pointer to sequence that will be removed
/// @param size - length of the @p seq
/// @return false if there is no hash table with given @p id or considered
/// hash table does not contain @p seq or @p seq equals NULL or
/// @p size equals 0, otherwise true
bool hash_remove(unsigned long id, uint64_t const *seq, size_t size);

/// @brief Clears hash table
/// Clears hash table if such exists, otherwise does nothing
/// @param id - id of considered hash table
void hash_clear(unsigned long id);

/// @brief Tests if hash table contains provided sequence
/// @param id - id of considered hash table
/// @param seq - pointer to sequence
/// @param size - length of @p seq
/// @return true if such sequence exists, false if such sequence does not exist
/// or @p seq is NULL or @p size equals 0
bool hash_test(unsigned long id, uint64_t const *seq, size_t size);

#ifdef __cplusplus
}
}
#endif

#endif
