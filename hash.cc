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

using hash_function_id_t = unsigned long;

namespace {
struct Hash;

using seq_vector_t = std::vector<uint64_t>;
using hash_table_t = std::unordered_set<seq_vector_t, Hash>;
using hash_tables_t = std::unordered_map<hash_function_id_t, hash_table_t>;

struct Hash {
  hash_function_t function;
  Hash(hash_function_t funct) { function = funct; }

  /// @brief Computes hash of a given sequence
  uint64_t operator()(seq_vector_t seq) const {
    return function(&(seq[0]), seq.size());
  }
};

hash_function_id_t numberOfCreatedHashes = 0;

hash_tables_t &hash_tables() {
  static hash_tables_t *x = new hash_tables_t();
  return *x;
}

/// @brief Prints debug message
/// Prints @p debugMessage if the debug mode is on
/// @param debugMessage
void debug(std::string debugMessage) {
  if (debugModeOn)
    std::cerr << debugMessage << '\n';
}

/// @brief Logs formatted debug message
/// @param functionName
/// @param id
void debugArgumentsInformation(std::string functionName,
                               std::string arguments) {
  debug(functionName + "(" + arguments + ")");
}

/// @brief Logs how function handled it's role
/// Adds formatted log: how function handled it's role considering given hash
/// table
/// @param functionName
/// @param id - id of considered hash table
/// @param debugEnding
void debugFinalInformation(std::string functionName, unsigned long id,
                           std::string debugEnding) {
  debug(functionName + ": hash table #" + std::to_string(id) + debugEnding);
}

/// @brief Logs invalid data for a given function
/// @param not_exist - tells if considered hash table exists
/// @param id - id of considered hash table
/// @param functionName
/// @param seq - pointer to the sequence
/// @param size - length of @p seq
bool invalidData(std::string functionName,
                 uint64_t const *seq, size_t size) {
  bool returned_value = false;

  if (seq == NULL) {
    debug(functionName + ": invalid pointer (NULL)");
    returned_value = true;
  }

  if (size == 0) {
    debug(functionName + ": invalid size (0)");
    returned_value = true;
  }

  return returned_value;
}

bool special_cases(bool not_exist, size_t id, std::string functionName,
                   uint64_t const *seq, size_t size) {
  if (invalidData(functionName, seq, size)) {
    return true;
  }
  
  if (not_exist) {
    debugFinalInformation(functionName, id, " does not exist");
    return true;
  }

  return false;
}

/// @brief Creates string from a sequence
/// @param seq - pointer to a sequence
/// @param size - length of sequence
/// @return "NULL" if seq equals NULL, otherwise sequence formatted sequence
std::string getStringRepresentation(uint64_t const *seq, size_t size) {
  if (seq == NULL) {
    return "NULL";
  } else {
    std::string rep = "\"";
    for (size_t i = 0; i < size; i++) {
      if (i > 0)
        rep += " ";
      rep += std::to_string(seq[i]);
    }
    rep += "\"";
    return rep;
  }
}
} // namespace

namespace jnp1 {
hash_function_id_t hash_create(hash_function_t hash_function) {
  std::string logArguments;
  if (hash_function == NULL) {
    logArguments = "NULL";
  } else {
    std::stringstream ss;
    ss << &hash_function;
    logArguments = ss.str();
  }
  debugArgumentsInformation("hash_create", logArguments);

  numberOfCreatedHashes++;
  hash_table_t hash_table(0, Hash(hash_function));
  hash_tables().insert({numberOfCreatedHashes - 1UL, hash_table});

  debugFinalInformation("hash_create", numberOfCreatedHashes - 1, " created");
  return numberOfCreatedHashes - 1;
}

void hash_delete(hash_function_id_t id) {
  debugArgumentsInformation("hash_delete", std::to_string(id));
  bool wasErased = hash_tables().erase(id);

  std::string debugEnding = wasErased ? " deleted" : " does not exist";
  debugFinalInformation("hash_delete", id, debugEnding);
}

size_t hash_size(unsigned long id) {
  debugArgumentsInformation("hash_size", std::to_string(id));
  auto hashTablesIt = hash_tables().find(id);
  size_t answer = 0;

  if (!(hashTablesIt == hash_tables().end())) {
    answer = (hashTablesIt->second).size();
    std::string debugEnding =
        " contains " + std::to_string(answer) + " element(s)";
    debugFinalInformation("hash_size", id, debugEnding);
  } else {
    debugFinalInformation("hash_size", id, " does not exist");
  }

  return answer;
}

bool hash_insert(hash_function_id_t id, uint64_t const *seq, size_t size) {
  std::string stringRepresentationOfSeq = getStringRepresentation(seq, size);

  debugArgumentsInformation("hash_insert", std::to_string(id) + ", " +
                                               stringRepresentationOfSeq +
                                               ", " + std::to_string(size));
  auto hashTableIt = hash_tables().find(id);

  if (special_cases(hashTableIt == hash_tables().end(), id, "hash_insert", seq,
    size)) {
    return false;
  }
  

  std::vector<uint64_t> copySeq(seq, seq + size);
  bool wasInserted = (hashTableIt->second).insert(copySeq).second;

  std::string debugEnding = ", sequence " + stringRepresentationOfSeq;
  debugEnding += wasInserted ? " inserted" : " was present";
  debugFinalInformation("hash_insert", id, debugEnding);

  return wasInserted;
}

bool hash_remove(hash_function_id_t id, uint64_t const *seq, size_t size) {
  std::string stringRepresentationOfSeq = getStringRepresentation(seq, size);
  debugArgumentsInformation("hash_remove", std::to_string(id) + ", " +
                                               stringRepresentationOfSeq +
                                               ", " + std::to_string(size));

  auto hashTableIt = hash_tables().find(id);

  if (special_cases(hashTableIt == hash_tables().end(), id, "hash_remove", seq,
    size)) {
    return false;
  }

  bool wasRemoved =
      (hashTableIt->second).erase(std::vector<uint64_t>(seq, seq + size));
  std::string debugEnding = ", sequence " + stringRepresentationOfSeq;
  debugEnding += wasRemoved ? " removed" : " was not present";
  debugFinalInformation("hash_remove", id, debugEnding);

  return wasRemoved;
}

void hash_clear(hash_function_id_t id) {
  debugArgumentsInformation("hash_clear", std::to_string(id));
  auto hashTableIt = hash_tables().find(id);
  bool wasCleared = false;

  if (hashTableIt != hash_tables().end()) {
    if (!(hashTableIt->second).empty()) {
      (hashTableIt->second).clear();
      wasCleared = true;
    }
    std::string debugEnding = wasCleared ? " cleared" : " was empty";
    debugFinalInformation("hash_clear", id, debugEnding);
  } else {
    debugFinalInformation("hash_clear", id, " does not exist");
  }
}

bool hash_test(hash_function_id_t id, uint64_t const *seq, size_t size) {
  std::string stringRepresentationOfSeq = getStringRepresentation(seq, size);
  debugArgumentsInformation("hash_test", std::to_string(id) + ", " +
                                             stringRepresentationOfSeq + ", " +
                                             std::to_string(size));
  bool isPresent = false;
  auto hashTableIt = hash_tables().find(id);

  if (special_cases(hashTableIt == hash_tables().end(), id, "hash_test", seq,
    size)) {
    return false;
  }

  hash_table_t hashTable = hashTableIt->second;
  isPresent = hashTable.end() != hashTable.find(seq_vector_t(seq, seq + size));

  std::string debugEnding = ", sequence " + stringRepresentationOfSeq;
  debugEnding += isPresent ? " is present" : " is not present";
  debugFinalInformation("hash_test", id, debugEnding);

  return isPresent;
}
} // namespace jnp1
