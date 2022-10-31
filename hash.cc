#include "hash.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <string>

using hash_function_id_t = unsigned long;

const bool debugModeOn = true;

namespace {
  struct Hash;

  using seq_vector_t = std::vector<uint64_t>; 
  using hash_table_t = std::unordered_set<seq_vector_t, Hash>;
  using hash_tables_t = std::unordered_map<hash_function_id_t , hash_table_t>;

  struct Hash{
    hash_function_t function;
    Hash(hash_function_t funct) {
      function = funct;
    }
    uint64_t operator () (seq_vector_t seq) const {
      return function(&(seq[0]), seq.size());
    }
  };

  hash_function_id_t numberOfCreatedHashes = 0;
  hash_tables_t hash_tables;


  void debug(std::string debugMessage) {
      if (debugModeOn)
        std::cerr << debugMessage << '\n';
  }
  
   void debugInformation(std::string functionName, std::string id) {
    debug(functionName + "(" + id + ")");
  }
  
  void debugFinalInformation (std::string functionName, unsigned long id, std::string debugEnding) {
    debug(functionName + ": hash table #" + std::to_string(id) + debugEnding);
  }
  
  void invalidData(std::string function_type, uint64_t const *seq, size_t size) {
    if (seq == NULL) {
      debug(function_type + ": invalid pointer (NULL)");
    }
    
    if (size == 0) {
      debug(function_type + ": invalid size (0)");
    }
  }

  void functionParamsPrintDebug(std::string functionName, std::vector<std::string> params) {
    std::string paramsList;
    // std::vector<std::string> v = {"234", 123}; 
    // debug(functionName + "(" + params)
  }

  std::string getStringRepresentation(uint64_t const *seq, size_t size) {
    std::string rep = "\"";
    for (int i = 0; i < size; i++) {
      if (i > 0)
        rep += " ";
      rep += std::to_string(seq[i]);
    }
    rep += "\"";
    return rep;
  }
}

namespace jnp1 {
  hash_function_id_t hash_create(hash_function_t hash_function) {
      // debug("hash_create(" + hash_function + ")");
      //debugInformation("hash_create", hash_function);
      numberOfCreatedHashes++;
      hash_table_t hash_table(10, Hash(hash_function));
      hash_tables.insert({numberOfCreatedHashes, hash_table}); 

      debugFinalInformation("hash_create", numberOfCreatedHashes, " created");
      return numberOfCreatedHashes;
  }

  void hash_delete(hash_function_id_t id) {
    debugInformation("hash_delete", std::to_string(id));
    bool wasErased = hash_tables.erase(id); // TODO: naruszenie ochrony pamięci w tej linijce, możliwe, że podobny problem co w hash_size()
    
    std::string debugEnding = wasErased ? " deleted" : " does not exist";
    debugFinalInformation("hash_delete", id, debugEnding);
  }

  size_t hash_size(unsigned long id) {
    debugInformation("hash_size", std::to_string(id));
    auto hashTablesIt = hash_tables.find(id);
    size_t answer = 0;

    if (!(hashTablesIt == hash_tables.end())) {
      answer = (hashTablesIt->second).size();
    }

    std::string debugEnding = " contains " + std::to_string(answer) + " elements";
    debugFinalInformation("hash_size", id, debugEnding);
    return answer;
  }

  bool hash_insert(hash_function_id_t id, uint64_t const * seq, size_t size) {
    std::string stringRepresentationOfSeq = getStringRepresentation(seq, size);
    
    debugInformation("hash_insert", std::to_string(id) + ", " + 
    stringRepresentationOfSeq + ", " + std::to_string(size));

    auto hashTableIt = hash_tables.find(id);
    if ((hashTableIt == hash_tables.end()) || (seq == NULL) || (size == 0)) {
      invalidData("hash_insert", seq, size);
      return false;
    }

    hash_table_t hashTable = hashTableIt -> second;

    std::vector<uint64_t> copySeq(seq, seq + size);
    bool wasInserted = hashTable.insert(copySeq).second;
    std::string  debugEnding = ", sequence " + stringRepresentationOfSeq;
    debugEnding += wasInserted ? " inserted" : " was present";
    debugFinalInformation("hash_insert", id, debugEnding);

    return wasInserted;
  }

  bool hash_remove(hash_function_id_t id, uint64_t const * seq, size_t size) {
    debugInformation("hash_remove", std::to_string(id));
    // validateParameters();
    // debug("");
    // if (seq == NULL) {

    // }
    // if (size == 0) {

    // }

    auto hashTableIt = hash_tables.find(id);
    if (hashTableIt == hash_tables.end() || (seq == NULL) || (size == 0)) {
      invalidData("hash_remove", seq, size);
      return false;
    } 
    
    hash_table_t hashTable = hashTableIt -> second;
    bool wasRemoved = hashTable.erase(std::vector<uint64_t>(seq, seq + size));
    std::string debugEnding = ", sequence " + getStringRepresentation(seq, size);
    debugEnding += wasRemoved ? " removed" : " was not present";
    debugFinalInformation("hash_remove", id, debugEnding);

    return wasRemoved;
  }

  void hash_clear(hash_function_id_t id) {
    debugInformation("hash_clear", std::to_string(id));
    auto hashTableIt = hash_tables.find(id);
    hash_table_t hashTable = hashTableIt -> second;
    bool wasCleared = false;
    if (hashTableIt != hash_tables.end()) {
      hashTable.clear();
      wasCleared = true;
    }

    std::string debugEnding = wasCleared ? " cleared" : " was empty";
    debugFinalInformation("hash_clear", id, debugEnding);
  }

  bool hash_test(hash_function_id_t id, uint64_t const * seq, size_t size) {
    debugInformation("hash_test", std::to_string(id));
    bool isPresent = false;
    hash_tables_t::iterator hashTableIt = hash_tables.find(id);
    hash_table_t hashTable = hashTableIt -> second;

    if (hashTableIt == hash_tables.end() || (seq == NULL) || (size == 0)) {
      invalidData("hash_remove", seq, size);
      return false;
    } 
    else {
      isPresent = hashTable.end() != hashTable.find(seq_vector_t(seq, seq + size));
    }

    std::string debugEnding = ", sequence " + getStringRepresentation(seq, size);
    debugEnding += isPresent ? " is present" : " is not present";
    debugFinalInformation("hash_test", id, debugEnding);

    return isPresent;
  }
}
