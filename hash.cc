#include "hash.h"
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifndef NDEBUG
const bool debugModeOn = true;
#else
const bool debugModeOn = false;
#endif

namespace {
struct Hash;

using seq_vector_t = std::vector<uint64_t>;
using hash_table_t = std::unordered_set<seq_vector_t, Hash>;
using hash_table_id_t = unsigned long;
using hash_tables_t = std::unordered_map<hash_table_id_t, hash_table_t>;

struct Hash {
  const hash_function_t function;
  Hash(hash_function_t funct) : function(funct) {}

  /// @brief Computes hash of a given sequence
  uint64_t operator()(seq_vector_t const &seq) const {
    return function(seq.data(), seq.size());
  }
};

/// It's used as id provider for created hash tables
hash_table_id_t numberOfCreatedHashes = 0;

hash_tables_t &hash_tables() {
  static hash_tables_t *new_table = new hash_tables_t();
  return *new_table;
}

template <typename... Args> void log(Args &&...args) {
  if (debugModeOn)
    (std::cerr << ... << args);
}

void logArray(uint64_t const *&seq, size_t &size) {
  if (debugModeOn) {
    if (seq == NULL) {
      std::cerr << "NULL";
    } else {
      std::cerr << "\"";
      for (size_t i = 0; i < size; i++) {
        if (i == size - 1) {
          std::cerr << seq[i];
        } else {
          std::cerr << seq[i] << ' ';
        }
      }
      std::cerr << "\"";
    }
  }
}

bool invalidHashClearInput(hash_tables_t::iterator hashTableIt,
                           hash_table_id_t id) {
  if (hashTableIt == hash_tables().end()) {
    log("hash_clear: hash table #", id, " does not exist\n");
    return true;
  }
  return false;
}

bool invalidInputHashCreate(hash_function_t &hash_function) {
  if (hash_function == NULL) {
    log("hash_create(NULL)\n");
    log("hash_create: invalid pointer (NULL)\n");
    return true;
  } else {
    log("hash_create(", hash_function, ")\n");
    return false;
  }
}

/// @brief Checks if the input is valid
/// Checks if the input is valid: seq is not null, size is not zero.
/// If the input is correct, then it checks if the considered hash table exists.
/// @param hashTableIt - considered hash table
/// @param id - id of considered hash table
/// @param seq
/// @param size
/// @param functionName
/// @return false if the input is incorrect or considered hash table does not
/// exist, otherwise true
bool invalidInput(hash_tables_t::iterator hashTableIt, hash_table_id_t id,
                  uint64_t const *seq, size_t size,
                  const std::string &functionName) {
  bool isInvalid = false;

  if (seq == NULL) {
    log(functionName, ": invalid pointer (NULL)\n");
    isInvalid = true;
  }

  if (size == 0) {
    log(functionName, ": invalid size (0)\n");
    isInvalid = true;
  }

  if (!isInvalid && hashTableIt == hash_tables().end()) {
    log(functionName, ": hash table #", id, " does not exist\n");
    isInvalid = true;
  }

  return isInvalid;
}

bool hashTableExists(hash_tables_t::iterator it) {
  return it != hash_tables().end();
}

bool insert(hash_tables_t::iterator hashTableIt, uint64_t const *seq,
            size_t size) {
  std::vector<uint64_t> copySeq(seq, seq + size);
  bool wasInserted = (hashTableIt->second).insert(copySeq).second;

  log("hash_insert: hash table #", hashTableIt->first, ", sequence ");
  logArray(seq, size);
  if (wasInserted) {
    log(" inserted\n");
  } else {
    log(" was present\n");
  }

  return wasInserted;
}

bool clear(hash_tables_t::iterator hashTableIt) {
  log("hash_clear: hash table #", hashTableIt->first);

  if (!(hashTableIt->second).empty()) {
    (hashTableIt->second).clear();

    log(" cleared\n");
    return true;
  }

  log(" was empty\n");
  return false;
}

bool test(hash_tables_t::iterator hashTableIt, uint64_t const *seq,
          size_t size) {
  hash_table_t hashTable = hashTableIt->second;
  bool isPresent =
      hashTable.end() != hashTable.find(seq_vector_t(seq, seq + size));

  log("hash_test: hash table #", hashTableIt->first, ", sequence ");
  logArray(seq, size);
  if (isPresent) {
    log(" is present\n");
  } else {
    log(" is not present\n");
  }

  return isPresent;
}

bool remove(hash_tables_t::iterator hashTableIt, uint64_t const *seq,
            size_t size) {
  bool wasRemoved =
      (hashTableIt->second).erase(std::vector<uint64_t>(seq, seq + size));

  log("hash_remove: hash table #", hashTableIt->first, ", sequence ");
  logArray(seq, size);
  if (wasRemoved) {
    log(" removed\n");
  } else {
    log(" was not present\n");
  }

  return wasRemoved;
}

} // namespace

namespace jnp1 {
hash_table_id_t hash_create(hash_function_t hash_function) {
  if (invalidInputHashCreate(hash_function)) {
    return 0;
  }

  hash_table_t hash_table(0, Hash(hash_function));
  hash_tables().insert({numberOfCreatedHashes, hash_table});

  log("hash_create: hash table #", numberOfCreatedHashes, " created\n");

  return numberOfCreatedHashes++;
}

void hash_delete(hash_table_id_t id) {
  log("hash_delete(", id, ")\n");

  bool wasErased = hash_tables().erase(id);

  if (wasErased) {
    log("hash_delete: hash table #", id, " deleted\n");
  } else {
    log("hash_delete: hash table #", id, " does not exist\n");
  }
}

size_t hash_size(unsigned long id) {
  log("hash_size(", id, ")\n");

  auto hashTablesIt = hash_tables().find(id);
  size_t answer = 0;

  if (hashTableExists(hashTablesIt)) {
    answer = (hashTablesIt->second).size();

    log("hash_size: hash table #", id, " contains ", answer, " element(s)\n");
  } else {
    log("hash_size: hash table #", id, " does not exist\n");
  }

  return answer;
}

bool hash_insert(hash_table_id_t id, uint64_t const *seq, size_t size) {
  log("hash_insert(", id, ", ");
  logArray(seq, size);
  log(", ", size, ")\n");

  auto hashTableIt = hash_tables().find(id);

  if (invalidInput(hashTableIt, id, seq, size, "hash_insert")) {
    return false;
  }

  return insert(hashTableIt, seq, size);
}

bool hash_remove(hash_table_id_t id, uint64_t const *seq, size_t size) {
  log("hash_remove(", id, ", ");
  logArray(seq, size);
  log(", ", size, ")\n");

  auto hashTableIt = hash_tables().find(id);

  if (invalidInput(hashTableIt, id, seq, size, "hash_remove")) {
    return false;
  }

  return remove(hashTableIt, seq, size);
}

void hash_clear(hash_table_id_t id) {
  log("hash_clear(", id, ")\n");

  auto hashTableIt = hash_tables().find(id);

  if (invalidHashClearInput(hashTableIt, id)) {
    return;
  }

  clear(hashTableIt);
}

bool hash_test(hash_table_id_t id, uint64_t const *seq, size_t size) {
  log("hash_test(", id, ", ");
  logArray(seq, size);
  log(", ", size, ")\n");

  auto hashTableIt = hash_tables().find(id);

  if (invalidInput(hashTableIt, id, seq, size, "hash_test")) {
    return false;
  }

  return test(hashTableIt, seq, size);
}
} // namespace jnp1
