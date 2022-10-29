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
  // using pair_seq_size_t = std::pair<int64_t*, size_t>;
  using hash_table_t = std::unordered_set<seq_vector_t, Hash>;
  using hash_tables_t = std::unordered_map<hash_function_id_t , hash_table_t>;

  struct Hash{
    hash_function_t function;
    Hash(hash_function_t funct) {
      function = funct;
    }
    uint64_t operator () (seq_vector_t seq) {
      return function(&(seq[0]), seq.size());
    }
  };

  hash_function_id_t numberOfCreatedHashes = 0;
  hash_tables_t hash_tables;


  void debug(std::string debugMessage) {
      if (debugModeOn)
        std::cerr << debugMessage << '\n';
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
      rep += std::to_string(seq[0]);
    }
    rep += "\"";
    return rep;
  }
}

hash_function_id_t hash_create(hash_function_t hash_function) {
    // debug("hash_create(" + hash_function + ")");

    numberOfCreatedHashes++;
    hash_table_t hash_table(10, Hash(hash_function));
    hash_tables[numberOfCreatedHashes] = hash_table;
    
    debug("hash_create: hash table #" + std::to_string(numberOfCreatedHashes) + " created");

    return numberOfCreatedHashes;
}

// void hash_delete(hash_function_id_t id) {
//   debug("hash_delete(" + std::to_string(id) + ")");
//   bool wasErased = hash_tables.erase(id);
  
//   std::string debugEnding = wasErased ? " deleted" : " does not exist";
//   debug("hash_delete: hash table #" + std::to_string(id) + debugEnding);
// }

// // size_t hash_size(unsigned long id) {
// //     return hash_tables.size();
// // }

// bool hash_insert(hash_function_id_t id, uint64_t const * seq, size_t size) {
//   std::string stringRepresntationOfSeq = getStringRepresentation(seq, size);
//   debug("hash_insert(" + std::to_string(id) + ", " + 
//      stringRepresntationOfSeq + ", " + std::to_string(size) + ")"); 

//   auto hashTableIt = hash_tables.find(id);
//   if (hashTableIt == hash_tables.end()) {
//     // debug("hash_insert: hash table #" << id << "");
//     return false;
//   }

//   hash_table_t hashTable = hashTableIt -> second;

//   std::vector<uint64_t> copySeq(seq, seq + size);
//   bool wasInserted = hashTable.insert(copySeq).second;

//   debug("");// was Inserted or not

//   return wasInserted;
// }

// bool hash_remove(hash_function_id_t id, uint64_t const * seq, size_t size) {
//   // validateParameters();
//   // debug("");
//   // if (seq == NULL) {

//   // }
//   // if (size == 0) {

//   // }

//   auto hashTableIt = hash_tables.find(id);
//   if (hashTableIt == hash_tables.end()) {
//     debug("");
//     return false;
//   } 
  
//   hash_table_t hashTable = hashTableIt -> second;
//   bool wasRemoved = hashTable.erase(std::vector<uint64_t>(seq, seq + size));
  
//   debug("");

//   return wasRemoved;
// }

// void hash_clear(hash_function_id_t id) {
//   auto hashTableIt = hash_tables.find(id);
//   hash_table_t hashTable = hashTableIt -> second;
//   if (hashTableIt != hash_tables.end())
//     hashTable.clear();
// }

// bool hash_test(hash_function_id_t id, uint64_t const * seq, size_t size) {
//   hash_tables_t::iterator hashTableIt = hash_tables.find(id);
//   hash_table_t hashTable = hashTableIt -> second;
//   if (hashTableIt != hash_tables.end())
//     return hashTable.end() != hashTable.find(seq_vector_t(seq, seq + size));

//   return false;
// }


// unsigned long hash_create(hash_function_t hash_function) {
//   return 0;
// }
void hash_delete(unsigned long id){return;}
size_t hash_size(unsigned long id) {return 0;}
bool hash_insert(unsigned long id, uint64_t const * seq, size_t size) {
  return false;
}
bool hash_remove(unsigned long id, uint64_t const * seq, size_t size) {
  return false;
}
void hash_clear(unsigned long id) {
  return;
}
bool hash_test(unsigned long id, uint64_t const * seq, size_t size) {
  return false;
}
