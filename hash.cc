#include "hash.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <string>

using hash_function_id_t = unsigned long;

bool initDebug() {
  return true;
}

const bool debugModeOn = initDebug();

namespace {
  struct Hash;

  using seq_vector_t = std::vector<uint64_t>; 
  using hash_table_t = std::unordered_set<seq_vector_t, Hash>;
  using hash_tables_t = std::unordered_map<hash_function_id_t , hash_table_t>;

  struct Hash {
    hash_function_t function;
    Hash(hash_function_t funct) {
      function = funct;
    }
    uint64_t operator () (seq_vector_t seq) const {
      return function(&(seq[0]), seq.size());
    }
  };

  hash_function_id_t numberOfCreatedHashes = 0;

  hash_tables_t& hash_tables() {
    static hash_tables_t *x = new hash_tables_t();
    return *x;
  }

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
  
  void invalidData(bool not_exist, size_t id, std::string function_type, uint64_t const *seq, size_t size) {
    if (not_exist) {
      debugFinalInformation(function_type, id, " does not exist");
    }

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
    if (seq == NULL) {
      return "NULL";
    }
    else {
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
}

namespace jnp1 {
  hash_function_id_t hash_create(hash_function_t hash_function) {
    std::cerr <<"[" << &hash_function << "]\n";
      // debug("hash_create(" + hash_function + ")");
      //debugInformation("hash_create", hash_function);
      numberOfCreatedHashes++;
      hash_table_t hash_table(0, Hash(hash_function));
      // int tmpasfsad = 5;
      hash_tables().insert({(unsigned long)(numberOfCreatedHashes - 1), hash_table}); 

      debugFinalInformation("hash_create", numberOfCreatedHashes - 1, " created");
      return numberOfCreatedHashes - 1;
  }

  void hash_delete(hash_function_id_t id) {
    debugInformation("hash_delete", std::to_string(id));
    bool wasErased = hash_tables().erase(id); // TODO: naruszenie ochrony pamięci w tej linijce, możliwe, że podobny problem co w hash_size()
    
    std::string debugEnding = wasErased ? " deleted" : " does not exist";
    debugFinalInformation("hash_delete", id, debugEnding);
  }

  size_t hash_size(unsigned long id) {
    debugInformation("hash_size", std::to_string(id));
    auto hashTablesIt = hash_tables().find(id);
    size_t answer = 0;

    if (!(hashTablesIt == hash_tables().end())) {
      answer = (hashTablesIt->second).size();
      std::string debugEnding = " contains " + std::to_string(answer) + " element(s)";
      debugFinalInformation("hash_size", id, debugEnding);
    }
    else {
      debugFinalInformation("hash_size", id, " does not exist");  
    }

    return answer;
  }

  bool hash_insert(hash_function_id_t id, uint64_t const * seq, size_t size) {
    std::string stringRepresentationOfSeq = getStringRepresentation(seq, size);
    
    debugInformation("hash_insert", std::to_string(id) + ", " + 
    stringRepresentationOfSeq + ", " + std::to_string(size));
    auto hashTableIt = hash_tables().find(id);
    if ((hashTableIt == hash_tables().end()) || (seq == NULL) || (size == 0)) {
      invalidData(hashTableIt == hash_tables().end(), id, "hash_insert", seq, size);
      return false;
    }

 //   hash_table_t hashTable = hashTableIt -> second;

    std::vector<uint64_t> copySeq(seq, seq + size);
    bool wasInserted = (hashTableIt->second).insert(copySeq).second; //hashTable.insert(copySeq).second;
    // Wcześniejsza wersja operowała na kopii, dlatego prawdopodobnie ciąg nie był wklejony do struktury danych
    std::string  debugEnding = ", sequence " + stringRepresentationOfSeq;
    debugEnding += wasInserted ? " inserted" : " was present";
    debugFinalInformation("hash_insert", id, debugEnding);

    return wasInserted;
  }

  bool hash_remove(hash_function_id_t id, uint64_t const * seq, size_t size) {
    std::string stringRepresentationOfSeq = getStringRepresentation(seq, size);
    debugInformation("hash_remove", std::to_string(id) + ", " + 
    stringRepresentationOfSeq + ", " + std::to_string(size));
    // validateParameters();
    // debug("");
    // if (seq == NULL) {

    // }
    // if (size == 0) {

    // }

    auto hashTableIt = hash_tables().find(id);
    if (hashTableIt == hash_tables().end() || (seq == NULL) || (size == 0)) {
      invalidData(hashTableIt == hash_tables().end(), id, "hash_remove", seq, size);
      return false;
    } 
    
    //hash_table_t hashTable = hashTableIt -> second;
    bool wasRemoved = (hashTableIt->second).erase(std::vector<uint64_t>(seq, seq + size));
    std::string debugEnding = ", sequence " + stringRepresentationOfSeq;
    debugEnding += wasRemoved ? " removed" : " was not present";
    debugFinalInformation("hash_remove", id, debugEnding);

    return wasRemoved;
  }

  void hash_clear(hash_function_id_t id) {
    debugInformation("hash_clear", std::to_string(id));
    auto hashTableIt = hash_tables().find(id);
    //hash_table_t hashTable = hashTableIt -> second;
    bool wasCleared = false;
    if (hashTableIt != hash_tables().end()) {
      if (!(hashTableIt->second).empty()) {
        (hashTableIt->second).clear();
        wasCleared = true;
      }
      std::string debugEnding = wasCleared ? " cleared" : " was empty";
      debugFinalInformation("hash_clear", id, debugEnding);
    }
    else {
      debugFinalInformation("hash_clear", id, " does not exist");
    }
  }

  bool hash_test(hash_function_id_t id, uint64_t const * seq, size_t size) {
    std::string stringRepresentationOfSeq = getStringRepresentation(seq, size);
    debugInformation("hash_test", std::to_string(id) + ", " + 
    stringRepresentationOfSeq + ", " + std::to_string(size));
    bool isPresent = false;
    hash_tables_t::iterator hashTableIt = hash_tables().find(id);

    if (hashTableIt == hash_tables().end() || (seq == NULL) || (size == 0)) {
      invalidData(hashTableIt == hash_tables().end(), id, "hash_test", seq, size);
      return false;
    } 
    else {
      hash_table_t hashTable = hashTableIt -> second;
      isPresent = hashTable.end() != hashTable.find(seq_vector_t(seq, seq + size));
    }

    std::string debugEnding = ", sequence " + stringRepresentationOfSeq;
    debugEnding += isPresent ? " is present" : " is not present";
    debugFinalInformation("hash_test", id, debugEnding);

    return isPresent;
  }
}
