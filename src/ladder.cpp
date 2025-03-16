#include "ladder.h"

// Print error message for invalid input
void error(string word1, string word2, string msg) {
    cout << "Error: " << msg << endl;
    cout << "Could not find ladder from " << word1 << " to " << word2 << endl;
}

// Check if edit distance between two strings is 1 or less
bool edit_distance_within(const std::string& str1, const std::string& str2, int d) {
    // If length difference is greater than d, return false
    if (abs((int)str1.length() - (int)str2.length()) > d) {
        return false;
    }
    
    // If strings are of same length, count differing characters
    if (str1.length() == str2.length()) {
        int diff_count = 0;
        for (size_t i = 0; i < str1.length(); i++) {
            if (str1[i] != str2[i]) diff_count++;
            if (diff_count > d) return false;
        }
        return true;
    }
    
    // One string is one character longer than the other
    const string& shorter = (str1.length() < str2.length()) ? str1 : str2;
    const string& longer = (str1.length() > str2.length()) ? str1 : str2;
    
    size_t i = 0, j = 0;
    bool used_edit = false;
    
    while (i < shorter.length() && j < longer.length()) {
        if (shorter[i] != longer[j]) {
            if (used_edit) return false;  // Already used one edit
            used_edit = true;
            j++;  // Skip character in longer string
        } else {
            i++;
            j++;
        }
    }
    
    // If there's remaining characters in the longer string
    // and we haven't used our edit yet, we can use it now
    if (j < longer.length() && !used_edit) {
        j++;
        used_edit = true;
    }
    
    // Check if we've processed all characters in both strings
    return (i == shorter.length() && j == longer.length());
}

// Check if two words are adjacent (differ by exactly one edit)
bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

// Generate word ladder using BFS
vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    // Convert words to lowercase for case-insensitive comparison
    string begin = begin_word;
    string end = end_word;
    for (char& c : begin) c = tolower(c);
    for (char& c : end) c = tolower(c);
    
    // Check if begin and end are the same
    if (begin == end) {
        return {begin};
    }
    
    // Initialize queue for BFS
    queue<vector<string>> ladder_queue;
    ladder_queue.push({begin});
    
    // Set to keep track of visited words
    set<string> visited;
    visited.insert(begin);
    
    while (!ladder_queue.empty()) {
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        
        string last_word = ladder.back();
        
        // Check the entire word list for adjacent words
        for (const string& word : word_list) {
            string lower_word = word;
            for (char& c : lower_word) c = tolower(c);
            
            if (is_adjacent(last_word, lower_word)) {
                if (visited.find(lower_word) == visited.end()) {
                    visited.insert(lower_word);
                    
                    vector<string> new_ladder = ladder;
                    new_ladder.push_back(lower_word);
                    
                    if (lower_word == end) {
                        return new_ladder;  // Found the target word
                    }
                    
                    ladder_queue.push(new_ladder);
                }
            }
        }
    }
    
    // No ladder found
    return {};
}

// Load words from dictionary file
void load_words(set<string>& word_list, const string& file_name) {
    ifstream file(file_name);
    string word;
    
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << file_name << endl;
        return;
    }
    
    while (file >> word) {
        // Convert to lowercase before adding
        for (char& c : word) c = tolower(c);
        word_list.insert(word);
    }
    
    file.close();
}

// Print the word ladder
void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No ladder found." << endl;
        return;
    }
    
    for (size_t i = 0; i < ladder.size(); i++) {
        cout << ladder[i];
        if (i < ladder.size() - 1) {
            cout << " -> ";
        }
    }
    cout << endl;
}

// Verify word ladder with test cases
void verify_word_ladder() {
    #define my_assert(e) {cout << #e << ((e) ? " passed": " failed") << endl;}

    set<string> word_list;
    load_words(word_list, "words.txt");

    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}
