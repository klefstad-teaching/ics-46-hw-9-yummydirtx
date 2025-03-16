#include <gtest/gtest.h>

#include "dijkstras.h"
#include "ladder.h"

TEST(Dijkstras, BasicTest) {
    Graph G;
    G.numVertices = 4;
    G.resize(4);
    G[0].push_back(Edge(0, 1, 1));
    G[0].push_back(Edge(0, 2, 4));
    G[1].push_back(Edge(1, 2, 2));
    G[1].push_back(Edge(1, 3, 5));
    G[2].push_back(Edge(2, 3, 1));

    vector<int> previous;
    vector<int> dist = dijkstra_shortest_path(G, 0, previous);

    EXPECT_EQ(dist[0], 0);
    EXPECT_EQ(dist[1], 1);
    EXPECT_EQ(dist[2], 3);
    EXPECT_EQ(dist[3], 4);

    vector<int> path = extract_shortest_path(dist, previous, 3);
    EXPECT_EQ(path.size(), 4);
    EXPECT_EQ(path[0], 0);
    EXPECT_EQ(path[1], 1);
    EXPECT_EQ(path[2], 2);
    EXPECT_EQ(path[3], 3);
}

TEST(Ladder, BasicTest) {
    set<string> word_list = {"cat", "bat", "bet", "beg", "bog", "dog"};
    vector<string> ladder = generate_word_ladder("cat", "dog", word_list);
    
    EXPECT_EQ(ladder.size(), 6);
    EXPECT_EQ(ladder[0], "cat");
    EXPECT_EQ(ladder[1], "bat");
    EXPECT_EQ(ladder[2], "bet");
    EXPECT_EQ(ladder[3], "beg");
    EXPECT_EQ(ladder[4], "bog");
    EXPECT_EQ(ladder[5], "dog");
}

TEST(Ladder, VerifyWordLadder) {
    set<string> word_list;
    load_words(word_list, "../src/words.txt");
    
    EXPECT_FALSE(word_list.empty()) << "Dictionary is empty";
    
    EXPECT_EQ(generate_word_ladder("cat", "dog", word_list).size(), 4);
    EXPECT_EQ(generate_word_ladder("marty", "curls", word_list).size(), 6);
    EXPECT_EQ(generate_word_ladder("code", "data", word_list).size(), 6);
    EXPECT_EQ(generate_word_ladder("work", "play", word_list).size(), 6);
    EXPECT_EQ(generate_word_ladder("sleep", "awake", word_list).size(), 8);
    EXPECT_EQ(generate_word_ladder("car", "cheat", word_list).size(), 4);
}
