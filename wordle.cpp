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
// parameters brainstorm:
// const input bc dont change green letter, recursive to not copy on each call
// refrence to floating bc want to change count each time (int char maps to), but dont want to make copy
// results needs to add new words in different calls so pas sby ref so its the same set thru the recursion
// dict is const duh bc you dont want to change it. refrence bc dont want to make a copy each call
// curent needs to be different each branch so its pass by value not pass by ref
// index pass by val bc each call needs its own copy of curr index bc its dif each branch, just like curr string
void makewords(
    const string &in,
    map<char, int> &floating_ctr,
    set<string> &results,
    const set<string> &dict,
    string current,
    int idx;

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string &in,
    const std::string &floating,
    const std::set<std::string> &dict)
{
    // Add your code here
    // in is given to us, but it represents the green letters
    // floating represents the yellow letters
    // dict is a set of all the valid words
    set<string> results;             // return type is a set of strings so need this to be a set of finished word strings
    map<char, int> floating_counter; // yellow letter, times it needs to be used (ints not bools bc ex: what if you have multiple floating e's )
    // put each letter from floating string into floating counter
    // from the start of floating until the end, walk the word
    for (size_t i = 0; i < floating.size(); i++)
    {
        // character c is that letter of floating
        char c = floating[i];
        // floating counter (char int map) at index c is augmented (can)
        floating_counter[c]++;
    }

    // call helper
    // need the green letters, the yellow letters, a place (set) to store result word,
    // the dictionary of all valid words, the current string its building, and the index of the letter its on
    makewords(in, floating_counter, results, dict, "", 0) return results;
}

// Define any helper functions here
// helper function for making all the words
void makewords(
    const string &in,
    map<char, int> &floating_ctr,
    set<string> &results,
    const set<string> &dict,
    string current,
    int idx)
{
    // base case
    if (idx == in.size())
    {                            // if curr word is size of input word (aka complete)
        results.insert(current); // add current word to results set
        return;
    }

    // TODO: deal with floating chars?
    if (in[idx] != '-')
    {
        // green letter go next char:
        // append char at idx of in string to end of current (string concat)
        // move up index by 1 (use +1 bc its recursion, so dont want to modify curr val not ++idx, use new val yk?)
        makewords(in, floating_ctr, results, dict, current + in[idx], idx + 1)
    }
    else
    {
        // position has a dash, go thru all the lets
        for (char c = 'a'; c <= 'z'; c++)
        { // does this auto cast to ascii? a=97 z=122 in dec
            // concat char to end of curr, move to next index
            makewords(in, floating_ctr, results, dict, current + c, idx + 1);
        }
    }
}
