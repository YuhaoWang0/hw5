#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void wordleHelper(
  const std::string& in, 
  std::string& current, 
  const std::set<std::string>& dict, 
  const std::set<std::string>& prefixes,
  std::set<std::string>& results,
  std::string floatingLeft, 
  size_t index);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::set<std::string> results;
    std::string current = in;

    // Pruning
    std::set<std::string> prefixes;
    for(const std::string& w: dict) {
      if(w.size() != in.size()) {
        continue;
      }
      for(size_t len = 1; len <= w.size(); ++len) {
        prefixes.insert(w.substr(0,len));
      }
    }

    wordleHelper(in, current, dict, prefixes, results, floating, 0);

    return results;
}

// Define any helper functions here
void wordleHelper(
  const std::string& in, 
  std::string& current, 
  const std::set<std::string>& dict, 
  const std::set<std::string>& prefixes,
  std::set<std::string>& results,
  std::string floatingLeft, 
  size_t index) {
    // 1. Base Case
    if(index == in.size()){
      if(floatingLeft.empty() && dict.find(current) != dict.end()){
        results.insert(current);
      }
      return;
    }

    // 1st Pruning  -- Not enough spaces.
    int remaining = in.size() - index;
    int spotLeft = static_cast<int>(remaining);
    if(spotLeft < static_cast<int>(floatingLeft.size())) {
      return;
    }

    if(index > 0) {
      std::string prefix = current.substr(0, index);
      if(prefixes.find(prefix) == prefixes.end()) {
        return;
      }
    }

    // Regular Case
    if(in[index] != '-') {
      current[index] = in[index];
      wordleHelper(in, current, dict, prefixes, results, floatingLeft, index + 1);
      return;
    }

    if(spotLeft == static_cast<int>(floatingLeft.size())) {
      for(int i = 0; i < static_cast<int>(floatingLeft.size()); i ++) {
        char c = floatingLeft[i];
        current[index] = c;

        std::string newFloating = floatingLeft;
        newFloating.erase(i, 1);

        wordleHelper(in, current, dict, prefixes, results, newFloating, index + 1);
      }
    } else {
      for(char c = 'a'; c <='z'; c++) {
        current[index] = c;

        std::string newFloating = floatingLeft;
        size_t pos = newFloating.find(c);
        if(pos != std::string::npos){
          newFloating.erase(pos, 1);
        }

        wordleHelper(in, current, dict, prefixes, results, newFloating, index + 1);
       }
    }
  }
