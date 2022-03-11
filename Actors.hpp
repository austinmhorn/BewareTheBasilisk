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

// ROB   : Steal arrow from player
// SCARE : Force player into random adjacent cave
// KO    : Knockout player and wake up in random cave
enum JackalAttack { ROB = 0, SCARE = 1, KO = 2 };

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
    // Store current cave position
    int cavePos;
    // Store next cave position
    int nextCavePos;
public:
    // Controls dialogue
    bool attackInTunnel;
    bool didMovePlayer;
    
    Jackal()
    {
        cavePos        = rand() % 20 + 1;
        nextCavePos    = -1;
        attackInTunnel = false;
        didMovePlayer  = false;
    }
    
    void setCavePos(int newCavePos) { cavePos = newCavePos; }
    void setNextCavePos(int newNextCavePos) { nextCavePos = newNextCavePos; }
    
    int getCavePos() { return cavePos; }
    int getNextCavePos() { return nextCavePos; }
    
    void findNextCave(int basiliskPos, int pitPos, Graph const &graph, int n)
    {
        int caveOpt1 = 0;
        int caveOpt2 = 0;
        int caveOpt3 = 0;
        int counter  = 1;
        
        // Find Jackal position in adjacency list
        for (int i = 0; i <= n; i++)
        {
            // Found Jackal's current cave position
            if ( i == cavePos )
            {
                // Iterate through each edge of vertex 'i'
                for (Pair v: graph.adjList[i])
                {
                    // Assign each possible destination vertex from current vertex
                    switch (counter)
                    {
                        case 1:
                            caveOpt1 = v.first;
                            break;
                        case 2:
                            caveOpt2 = v.first;
                            break;
                        case 3:
                            caveOpt3 = v.first;
                            break;
                    }
                    // Increase counter controlling which random variable is assigned
                    counter++;
                }
            }
        }
        
        //cout << "Jackal is in cave " << cavePos << endl;
        
        do
        {
            int randMove = rand() % 3 + 1;

            // Assign next cave position based on random integer
            switch ( randMove )
            {
                case 1:
                    nextCavePos = caveOpt1;
                    break;
                    
                case 2:
                    nextCavePos = caveOpt2;
                    break;
                    
                case 3:
                    nextCavePos = caveOpt3;
                    break;
            }
        } while ( nextCavePos == basiliskPos || nextCavePos == pitPos );
        
        //cout << "Jackal will move to cave " << nextCavePos << endl;
    }
    
    void moveJackal(int playerOldCave, int playerNewCave)
    {
        // Player and Jackal are swapping caves (traveling on same edge towards each other)
        if ( cavePos == playerNewCave && nextCavePos == playerOldCave )
        {
            // Set public variable to control dialogue in attackPlayer()
            attackInTunnel = true;
            
            cout << "\033[35m\033[1mYou collide with something in the pitch blackness and get into a scuffle...\033[0m" << endl;
            
            // Wait for user to continue
            cout << "Press \033[33m\033[1mENTER\033[0m to continue...";
            cin.get();
        }
        else if ( cavePos != playerNewCave && nextCavePos == playerNewCave )
        {
            // Set public variable to control dialogue in attackPlayer()
            attackInTunnel = false;
            
            cout << "\033[35m\033[1mAs you enter, a Jackal charges in from cave " << '(' << cavePos << ')' << "...\033[0m" << endl;
            
            // Move Jackal
            cavePos = nextCavePos;
            
            // Wait for user to continue
            cout << "Press \033[33m\033[1mENTER\033[0m to continue...";
            cin.get();
        }
        else
        {
            // Move Jackal normally
            cavePos = nextCavePos;
            
            // Best place for reset
            didMovePlayer = false;
        }
    }
    
    void attackPlayer(Player* player, int basiliskPos, int pitPos, Graph const &graph, int n)
    {
        JackalAttack randAttack;
        bool attackIsValid = false;
        int fleeCave1 = 0;
        int fleeCave2 = 0;
        int fleeCave3 = 0;
        int counter   = 1;
        int playerRandCave = 0;
        int randMove = 0;
        
        do
        {
            // Randomly generate integer from 0 to 2 (inclusively), and cast it into enumerated type JackalAttack
            randAttack = static_cast<JackalAttack>( rand() % 3 );
            
            // Handle JackalAttack attack scenario cases
            switch ( randAttack )
            {
                // MARK: Jackal robs player of arrow
                case ROB:
                    // Player has at least one arrow able to be lost
                    if ( player->getArrowCount() > 0 )
                    {
                        // This attack is valid since player has an arrow to lose
                        attackIsValid = true;
                        
                        // Check if this attack is on an edge or a vertex
                        if ( attackInTunnel )
                        {
                            cout << "\033[35mYou break away and miraculously locate a small nook to hide in...\033[0m" << endl;
                            cout << "\033[35mWhatever it was scurries away, but you notice an arrow is missing from your quiver.\033[0m" << endl;
                        }
                        else
                        {
                            cout << "\033[35mBefore you can react, the Jackal tackles you and wrestles you to ground!\033[0m" << endl;
                            cout << "\033[35mYou reach for a magic arrow from your quiver and stab the Jackal, causing him to flee...\033[0m" << endl;
                        }
                        
                        // Remove arrow from Player
                        player->removeArrow();
                        
                        // Display number of arrows remaining in player inventory
                        cout << "\033[3mYou have " << player->getArrowCount() << " magic arrows remaining.\033[0m" << endl;
                        
                        // Wait for user to continue
                        cout << "Press \033[33m\033[1mENTER\033[0m to continue...";
                        cin.get();
                        
                        // Don't move Jackal into same cave as Player following attack
                        if ( nextCavePos != player->getCavePos() )
                        {
                            // Move Jackal
                            cavePos = nextCavePos;
                        }
                    }
                    // Player does not have an arrow to lose, attack is invalid and loop must repeat in attempt to generate new attack
                    else
                    {
                        attackIsValid = false;
                    }
                    break;
                
                // MARK: Jackal scares player into adjacent cave
                case SCARE:
                    attackIsValid = true;
                    
                    // Check if this attack is on an edge or a vertex
                    if ( attackInTunnel )
                    {
                        cout << "\033[35mThe entity grapples with you, but it is far weaker.. \033[0m" << endl;
                        cout << "\033[35mYou escape from its grip and make a run for it in a random direction...\033[0m" << endl;
                    }
                    else
                    {
                        cout << "\033[35mThe Jackal hurtles toward you, but you dive out of the way just in time!\033[0m" << endl;
                        cout << "\033[35mYou make a bolt for the closest tunnel to escape... \033[0m" << endl;
                    }
                    
                    // Find Player position in adjacency list
                    for (int i = 0; i <= n; i++)
                    {
                        // Found Player's current cave position
                        if ( i == player->getCavePos() )
                        {
                            // Iterate through each edge of vertex 'i'
                            for (Pair v: graph.adjList[i])
                            {
                                // Assign each possible destination vertex from current vertex
                                switch (counter)
                                {
                                    case 1:
                                        fleeCave1 = v.first;
                                        break;
                                    case 2:
                                        fleeCave2 = v.first;
                                        break;
                                    case 3:
                                        fleeCave3 = v.first;
                                        break;
                                }
                                // Increase counter controlling which random variable is assigned
                                counter++;
                            }
                        }
                    }
                    
                    // Randomly generate
                    randMove = rand() % 3 + 1;
                    
                    // Assign next cave position based on random integer
                    switch ( randMove )
                    {
                        case 1:
                            playerRandCave = fleeCave1;
                            break;
                            
                        case 2:
                            playerRandCave = fleeCave2;
                            break;
                            
                        case 3:
                            playerRandCave = fleeCave3;
                            break;
                    }
                    
                    cout << "\033[35m\033[1m\033[4mAfter barrelling through the darkness, you find yourself wandering into cave " << '(' << playerRandCave << ')' << "...\033[0m" << endl;
                    
                    // Wait for user to continue
                    cout << "Press \033[33m\033[1mENTER\033[0m to continue...";
                    cin.get();
                    
                    // Relocate player
                    player->setCavePos(playerRandCave);
                    
                    // Assign variable true since player was relocated
                    didMovePlayer = true;
                    
                    // Don't move Jackal into same cave as Player following attack
                    if ( nextCavePos != player->getCavePos() )
                    {
                        // Move Jackal
                        cavePos = nextCavePos;
                    }

                    break;
                    
                // Jackal knocks player out and drags them to random cave
                case KO:
                    attackIsValid = true;
                    
                    if ( attackInTunnel )
                    {
                        cout << "\033[35mDuring the struggle, you're knocked unconscious...\033[0m" << endl;
                    }
                    else
                    {
                        cout << "\033[35mThe Jackal dashes at you with a heavy club...\033[0m" << endl;
                        cout << "\033[35mYou try to fight back, but you get knocked out cold...\033[0m" << endl;
                    }
                    
                    int randWakeCave = rand() % 20 + 1;
                    
                    while ( randWakeCave == player->getCavePos() || randWakeCave == basiliskPos || randWakeCave == pitPos )
                    {
                        randWakeCave = rand() % 20 + 1;
                    }
                    
                    cout << "\033[35mSome time later, you awake in a daze... you determine you're in cave " << '(' << randWakeCave << ')' << "...\033[0m" << endl;
                    
                    // Relocate player
                    player->setCavePos(randWakeCave);
                    
                    // Assign variable true since player was relocated
                    didMovePlayer = true;
                    
                    // Don't move Jackal into same cave as Player following attack
                    if ( nextCavePos != player->getCavePos() )
                    {
                        // Move Jackal
                        cavePos = nextCavePos;
                    }
                    
                    break;
            }
            
        } while( !attackIsValid );
    }
};

#endif // Actors_hpp
