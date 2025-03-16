#include "ladder.h"
#include <unordered_set>
#include <unordered_map>

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

// Generate word ladder using optimized BFS
vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    // Convert words to lowercase for case-insensitive comparison
    string begin = begin_word;
    string end = end_word;
    for (char& c : begin) c = tolower(c);
    for (char& c : end) c = tolower(c);
    
    // Check if begin and end are the same
    if (begin == end) {
        return {};  // Return empty vector when words are identical
    }
    
    // Pre-process dictionary: create a vector of lowercase words
    vector<string> dict_words;
    dict_words.reserve(word_list.size());
    for (const string& word : word_list) {
        string lower_word = word;
        for (char& c : lower_word) c = tolower(c);
        dict_words.push_back(lower_word);
    }
    
    // Build adjacency list for common word lengths
    unordered_map<int, vector<int>> length_to_indices;
    for (size_t i = 0; i < dict_words.size(); i++) {
        int len = dict_words[i].length();
        length_to_indices[len].push_back(i);
        length_to_indices[len-1].push_back(i);  // For words one letter shorter
        length_to_indices[len+1].push_back(i);  // For words one letter longer
    }
    
    // Use a more efficient BFS approach
    queue<int> word_queue;  // Store indices instead of strings
    unordered_map<string, int> word_to_index;
    for (size_t i = 0; i < dict_words.size(); i++) {
        word_to_index[dict_words[i]] = i;
    }
    
    // Map to track parent words (for reconstructing path)
    unordered_map<string, string> parent;
    
    // Track visited words
    unordered_set<string> visited;
    visited.insert(begin);
    
    // Start BFS
    queue<string> q;
    q.push(begin);
    
    bool found = false;
    while (!q.empty() && !found) {
        string current = q.front();
        q.pop();
        
        int current_len = current.length();
        
        // Only check words with similar length (current_len, current_len+1, current_len-1)
        for (int len = max(1, current_len - 1); len <= current_len + 1; len++) {
            if (length_to_indices.find(len) == length_to_indices.end()) continue;
            
            for (int idx : length_to_indices[len]) {
                const string& next_word = dict_words[idx];
                
                // Skip already visited words
                if (visited.count(next_word)) continue;
                
                // Check if adjacent
                if (is_adjacent(current, next_word)) {
                    visited.insert(next_word);
                    parent[next_word] = current;
                    
                    if (next_word == end) {
                        found = true;
                        break;
                    }
                    
                    q.push(next_word);
                }
            }
            
            if (found) break;
        }
    }
    
    // Reconstruct path if found
    if (!visited.count(end)) {
        return {};  // No path found
    }
    
    vector<string> ladder;
    for (string word = end; !word.empty(); word = parent[word]) {
        ladder.push_back(word);
    }
    
    // Add start word if not already in the path
    if (ladder.back() != begin) {
        ladder.push_back(begin);
    }
    
    // Reverse to get path from start to end
    reverse(ladder.begin(), ladder.end());
    
    return ladder;
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
        cout << "No word ladder found." << endl;
        return;
    }
    
    cout << "Word ladder found: ";
    for (size_t i = 0; i < ladder.size(); i++) {
        cout << ladder[i] << " ";
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
