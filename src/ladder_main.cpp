#include "ladder.h"

int main() {
    // Load dictionary
    set<string> word_list;
    load_words(word_list, "words.txt");
    
    if (word_list.empty()) {
        cerr << "Dictionary is empty. Cannot proceed." << endl;
        return 1;
    }
    
    string begin_word, end_word;
    
    cout << "Enter start word: ";
    cin >> begin_word;
    
    cout << "Enter end word: ";
    cin >> end_word;
    
    // Convert to lowercase for comparison
    string lower_begin = begin_word;
    string lower_end = end_word;
    for (char& c : lower_begin) c = tolower(c);
    for (char& c : lower_end) c = tolower(c);
    
    // Check if start and end words are the same
    if (lower_begin == lower_end) {
        error(begin_word, end_word, "Start and end words are the same.");
        return 1;
    }
    
    // Check if end word is in dictionary
    if (word_list.find(lower_end) == word_list.end()) {
        error(begin_word, end_word, "End word is not in the dictionary.");
        return 1;
    }
    
    // Generate word ladder
    vector<string> ladder = generate_word_ladder(begin_word, end_word, word_list);
    
    // Print result
    if (ladder.empty()) {
        cout << "No word ladder found from " << begin_word << " to " << end_word << "." << endl;
    } else {
        cout << "Word ladder from " << begin_word << " to " << end_word << ":" << endl;
        print_word_ladder(ladder);
    }
    
    return 0;
}
