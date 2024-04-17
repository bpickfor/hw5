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
    unordered_map<char, int> &floating_ctr,
    set<string> &results,
    const set<string> &dict,
    string current,
    int idx);
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
    set<string> results;                       // return type is a set of strings so need this to be a set of finished word strings
    unordered_map<char, int> floating_counter; // yellow letter, times it needs to be used (ints not bools bc ex: what if you have multiple floating e's )
    // put each letter from floating string into floating counter
    // from the start of floating until the end, walk the word
    for (size_t i = 0; i < floating.size(); i++)
    {
        // character c is that letter of floating
        // char c = floating[i];
        // floating counter (char int map) at index c is augmented (can)
        // floating_counter[c]++;
        floating_counter[floating[i]]++;
    }
    // call helper
    // need the green letters, the yellow letters, a place (set) to store result word,
    // the dictionary of all valid words, the current string its building, and the index of the letter its on
    // start cuurent string off empty
    makewords(in, floating_counter, results, dict, "", 0);
    return results;
}
// Define any helper functions here
// helper function for making all the words
void makewords(
    const string &in,
    unordered_map<char, int> &floating_ctr,
    set<string> &results,
    const set<string> &dict,
    string current,
    int idx)
{
    // base case
    if (idx == in.size()) // if curr word is size of input word (aka complete)
    {
        if (dict.find(current) != dict.end()) // if word is valid (found in dictionary)
        {
            bool usedall = true; // track whether all eles in floating char map are used
            for (auto it = floating_ctr.begin(); it != floating_ctr.end(); ++it)
            // for (std::map<char, int>::iterator it = floating_ctr.begin(); it != floating_ctr.end(); ++it)
            {                       // iterator
                if (it->second > 0) // check if letter has more times it needs to be used
                {                   // it->second is the val (remem it->first is key), if val > 0 (aka letter hasnt been fully used)
                    // if it needs to be used more
                    usedall = false; // not all letters used
                    break;
                }
            }
            if (usedall)
            {                            // if all floating chars used
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
        makewords(in, floating_ctr, results, dict, current + in[idx], idx + 1);
    }
    else
    {
        // get how many remaing dashes
        int remainingspaces = count(in.begin() + idx, in.end(), '-');
        int totalfloatingneeded = 0;
        for (const auto &it : floating_ctr)
        // for (std::map<char, int>::const_iterator it = floating_ctr.begin(); it != floating_ctr.end(); ++it)
        {
            // totalfloatingneeded += it->second;
            totalfloatingneeded += it.second;
        }
        if (totalfloatingneeded > remainingspaces)
        {
            return;
        }

        // position has a dash, go thru all the lets
        for (char c = 'a'; c <= 'z'; c++)
        { // does this auto cast to ascii? a=97 z=122 in dec
            // concat char to end of curr, move to next index
            // temporarily use letter
            current.push_back(c);
            // floating_ctr.count(c) returns wheter char c exists in the map or not
            // and floating_ctr[c] > 0 checks that it still needs to be used
            if (floating_ctr.count(c) && floating_ctr[c] > 0)
            {
                floating_ctr[c]--; // letter used, update count, this goes deeper
                // go to next idx
                // dont need to concat c and curr bc already added it to back
                makewords(in, floating_ctr, results, dict, current, idx + 1);
                floating_ctr[c]++; // put count back as if you didnt use it, this backtracks up shallower
            }
            else
            {
                // go to next idx
                // dont need to concat c and curr bc already added it to back
                makewords(in, floating_ctr, results, dict, current, idx + 1);
            }
            current.pop_back(); // remove this temp letter
        }
    }
}
