#ifndef Actors_hpp
#define Actors_hpp

#include <iostream>
#include <vector>
#include <ctime>
#include <cctype>  // std::toupper
#include <cstdlib> // std::atoi
#include <string>
using namespace std;

enum PlayerOptions { MOVE = 'M', SHOOT = 'S', RIDE = 'R' };

enum Attack { ROB = 0, SCARE = 1, KO = 2 };

struct Edge { int src, dest, weight; };

typedef pair<int, int> Pair;
 
// A class to represent a graph object
class Graph
{
public:
    // a vector of vectors of Pairs to represent an adjacency list
    vector<vector<Pair>> adjList;
 
    // Graph Constructor
    Graph(vector<Edge> const &edges, int n)
    {
        // resize the vector to hold `n` elements of type vector<Edge>
        adjList.resize(n);
 
        // add edges to the directed graph
        for (auto &edge: edges)
        {
            int src = edge.src;
            int dest = edge.dest;
            int weight = edge.weight;
 
            // insert at the end
            adjList[src].push_back(make_pair(dest, weight));
 
            // uncomment the following code for undirected graph
            // adjList[dest].push_back(make_pair(src, weight));
        }
    }
};

// Function to print adjacency list representation of a graph
void printGraph(Graph const &graph, int n)
{
    for (int i = 0; i < n; i++)
    {
        // Function to print all neighboring vertices of a given vertex
        for (Pair v: graph.adjList[i]) {
            cout << "(" << i << ", " << v.first << ", " << v.second << ") ";
        }
        cout << endl;
    }
}

// A class to represent the player of the game
class Player
{
private:
    // Stores number of magic arrows remaining in player inventory
    int arrows;
    // Stores 'edge.src' of cave where player is currently positioned
    int cavePos;
public:
    Player()
    {
        // Default number of arrows at start of game
        arrows = 3;
        // Spawn player in random cave
        cavePos = rand() % 20 + 1;
    }
    
    // Mutators
    void setCavePos (int newCavePos) { cavePos = newCavePos; }
    void removeArrow() { arrows--; }
    void addArrow() { arrows++; }
    
    // Accessors
    int getCavePos() const { return cavePos; }
    int getArrowCount() const { return arrows; }
    
};

// A class representing Basilisk Enemy
// Basilisk - The King of serpents, has the power to cause death with a single glance (Europe)
class Basilisk
{
private:
    // Stores 'edge.src' of cave where basilisk is currently positioned
    int cavePos;
    int caveWhereDisturbed;
public:
    Basilisk()
    {
        cavePos = rand() % 20 + 1;
        caveWhereDisturbed = 0;
    }
    void setCavePos(int newCavePos) { cavePos = newCavePos; }
    void setCaveWhereDisturbed(int cave) { caveWhereDisturbed = cave; }
    
    int getCavePos() const { return cavePos; }
    int getCaveWhereDisturbed() const { return caveWhereDisturbed; }
};

class Pit
{
private:
    int cavePos;
public:
    Pit()
    {
        cavePos = rand() % 20 + 1;
    }
    void setCavePos(int newCavePos) { cavePos = newCavePos; }
    int getCavePos() const { return cavePos; }
};

class Minerail
{
private:
    int srcCave, destCave;
public:
    Minerail()
    {
        do
        {
            srcCave  = rand() % 20 + 1;
            destCave = rand() % 20 + 1;
        } while ( srcCave == destCave );
    }
    void setSrcCavePos(int newSrcCave) { srcCave = newSrcCave; }
    void setDestCavePos(int newDestCave) { destCave = newDestCave; }
    int getSrcCavePos() const { return srcCave; }
    int getDestCavePos() const { return destCave; }
};

class Jackal
{
private:
    int cavePos;
    int nextCavePos;
    Attack attack;
public:
    Jackal()
    {
        cavePos     = rand() % 20 + 1;
        nextCavePos = -1;
    }
    
    void setCavePos(int newCavePos) { cavePos = newCavePos; }
    void setNextCavePos(int newNextCavePos) { nextCavePos = newNextCavePos; }
    
    int getCavePos() { return cavePos; }
    int getNextCavePos() { return nextCavePos; }
    
    void moveJackal(int playerPos, int basiliskPos, int pitPos)
    {
        
    }
    
};

#endif // Actors_hpp
