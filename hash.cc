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
  
   void debugInformation(std::string functionName, unsigned long id) {
    debug(functionName + "(" + std::to_string(id) + ")");
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

      debug("hash_create: hash table #" + std::to_string(numberOfCreatedHashes) + " created");

      return numberOfCreatedHashes;
  }

  void hash_delete(hash_function_id_t id) {
    //debugInformation("hash_delete", id);
    debug("hash_delete(" + std::to_string(id) + ")");
    bool wasErased = hash_tables.erase(id); // TODO: naruszenie ochrony pamięci w tej linijce, możliwe, że podobny problem co w hash_size()
    
    std::string debugEnding = wasErased ? " deleted" : " does not exist";
    debug("hash_delete: hash table #" + std::to_string(id) + debugEnding);
  }

  size_t hash_size(unsigned long id) {
    //debugInformation("hash_size", id);
    auto hashTablesIt = hash_tables.find(id);

    if (hashTablesIt == hash_tables.end())
      return 0; 
    //TODO: wydaje mi się, że tutaj trzeba zwrócić wielkość konkretnej tablicy hashującej, nie liczbę tablic hashujących
    
    //Wtedy są problemy z kompilacją, wydaje mi się, że to kwestia tego, że nie do końca rozumiem jak skonstruowany jest set w Twoim programie w hash_create.
    return (hashTablesIt->second).size();
  }

  bool hash_insert(hash_function_id_t id, uint64_t const * seq, size_t size) {
    //debugInformation("hash_insert", id);
    std::string stringRepresntationOfSeq = getStringRepresentation(seq, size);
    debug("hash_insert(" + std::to_string(id) + ", " + 
      stringRepresntationOfSeq + ", " + std::to_string(size) + ")"); 

    auto hashTableIt = hash_tables.find(id);
    if (hashTableIt == hash_tables.end()) {
      // debug("hash_insert: hash table #" << id << "");
      return false;
    }

    hash_table_t hashTable = hashTableIt -> second;

    std::vector<uint64_t> copySeq(seq, seq + size);
    bool wasInserted = hashTable.insert(copySeq).second;

    // debug("");// was Inserted or not

    // return wasInserted;

    return true;
  }

  bool hash_remove(hash_function_id_t id, uint64_t const * seq, size_t size) {
    //debugInformation("hash_remove", id);
    // validateParameters();
    // debug("");
    // if (seq == NULL) {

    // }
    // if (size == 0) {

    // }

    auto hashTableIt = hash_tables.find(id);
    if (hashTableIt == hash_tables.end()) {
      debug("");
      return false;
    } 
    
    hash_table_t hashTable = hashTableIt -> second;
    bool wasRemoved = hashTable.erase(std::vector<uint64_t>(seq, seq + size));
    
    debug("");

    return wasRemoved;
  }

  void hash_clear(hash_function_id_t id) {
    //debugInformation("hash_clear", id);
    auto hashTableIt = hash_tables.find(id);
    hash_table_t hashTable = hashTableIt -> second;
    if (hashTableIt != hash_tables.end())
      hashTable.clear();
  }

  bool hash_test(hash_function_id_t id, uint64_t const * seq, size_t size) {
    //debugInformation("hash_test", id);
    hash_tables_t::iterator hashTableIt = hash_tables.find(id);
    hash_table_t hashTable = hashTableIt -> second;
    if (hashTableIt != hash_tables.end())
      return hashTable.end() != hashTable.find(seq_vector_t(seq, seq + size));

    return false;
  }
}
