#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm>
#include <map>
#include <unordered_map>
#include <set>
#endif
#include "wordle.h"
#include "dict-eng.h"
using namespace std;

// prefix set form dictionary to stop rec early, will hopefully fix long wordle test run time
set<string> buildPrefixSet(const set<string> &dict)
{
    set<string> prefixes;           // stores all prefixes
    for (const string &word : dict) // go thru each word in dictionary
    {
        for (size_t len = 1; len <= word.length(); ++len) // prefixes for each length
        {
            prefixes.insert(word.substr(0, len)); // put each prefix into set
        }
    }
    return prefixes; // complete set of prefixes
}

// Add prototypes of helper functions here
// parameters brainstorm:
// const input bc dont change green letter, recursive to not copy on each call
// refrence to floating bc want to change count each time (int char maps to), but dont want to make copy
// results needs to add new words in different calls so pas sby ref so its the same set thru the recursion
// dict is const duh bc you dont want to change it. refrence bc dont want to make a copy each call
// curent needs to be diffe rent each branch so its pass by value not pass by ref
// index pass by val bc each call needs its own copy of curr index bc its dif each branch, just like curr string
void makewords(
    const string &in,
    map<char, int> &floating_ctr,
    set<string> &results,
    const set<string> &dict,
    const set<string> &prefixes,
    string current,
    int idx);

// Definition of primary wordle function
set<string> wordle(
    const string &in,
    const string &floating,
    const set<string> &dict)
{
    // Add your code here
    // in is given to us, but it represents the green letters
    // floating represents the yellow letters
    // dict is a set of all the valid words
    set<string> results;             // return type is a set of strings so need this to be a set of finished word strings
    map<char, int> floating_counter; // yellow letter, times it needs to be used (ints not bools bc ex: what if you have multiple floating e's )
    for (char c : floating)          // put each letter from floating string into floating counter
    {
        floating_counter[c]++;
    }
    // call helpers
    //  need the green letters, the yellow letters, a place (set) to store result word,
    //  the dictionary of all valid words, the current string its building, and the index of the letter its on
    //  start cuurent string off empty
    set<string> prefixes = buildPrefixSet(dict);
    makewords(in, floating_counter, results, dict, prefixes, "", 0);
    return results;
}

// Define any helper functions here
// helper function for making all the words
// recursive funct
void makewords(
    const string &in,
    map<char, int> &floating_ctr,
    set<string> &results,
    const set<string> &dict,
    const set<string> &prefixes,
    string current,
    int idx)
{
    // base case
    if (idx == in.size()) // if curr word is size of input word (aka complete)
    {
        if (dict.find(current) != dict.end()) // if word is valid (found in dictionary)
        {
            bool usedAll = true; // track whether all eles in floating char map are used
            for (const auto &entry : floating_ctr)
            {
                if (entry.second > 0) // check if letter has more times it needs to be used
                {
                    usedAll = false; // not all letters used
                    break;
                }
            }
            if (usedAll) // if all floating chars used
            {
                results.insert(current); // add current word to results set
            }
        }
        return;
    }

    if (in[idx] != '-')
    {
        // green letter go next char:
        // append char at idx of in string to end of current (string concat)
        // move up index by 1 (use +1 bc its recursion, so dont want to modify curr val not ++idx, use new val yk?)
        makewords(in, floating_ctr, results, dict, prefixes, current + in[idx], idx + 1);
    }
    else // if current position is a place holder
    {
        // try all possible letters
        for (char c = 'a'; c <= 'z'; c++)
        {
            string nextCurrent = current + c; // make next vers of curr word
            if (!prefixes.count(nextCurrent)) // check if valid prefix
            {
                continue; // skip recursion if nothing starts with that prefix
            }
            if (floating_ctr.count(c) && floating_ctr[c] > 0) // if its a floating let that needs to be used still
            {
                floating_ctr[c]--;                                                          // decrement count of it
                makewords(in, floating_ctr, results, dict, prefixes, nextCurrent, idx + 1); // call with new word
                floating_ctr[c]++;                                                          // backtrack (put count back)
            }
            else
            {
                // go to next idx
                makewords(in, floating_ctr, results, dict, prefixes, nextCurrent, idx + 1);
            }
        }
    }
}