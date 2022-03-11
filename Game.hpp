#ifndef Game_hpp
#define Game_hpp

#include "Actors.hpp"

class Game
{
private:
    // Number of vertices of planar dodecahedron
    int n = 20;
    // Edges of planar dodecahedron
    vector<Edge> edges =
    {
        // Outer Layer
        {1,2,0}, {1,5,0}, {1,8,0},
        {2,1,0}, {2,3,0}, {2,10,0},
        {3,2,0}, {3,4,0}, {3,12,0},
        {4,3,0}, {4,5,0}, {4,14,0},
        {5,1,0}, {5,4,0}, {5,6,0},
        // Middle Layer
        {6,5,0}, {6,7,0}, {6,15,0},
        {7,6,0}, {7,8,0}, {7,17,0},
        {8,1,0}, {8,7,0}, {8,9,0},
        {9,8,0}, {9,10,0}, {9,18,0},
        {10,2,0}, {10,9,0}, {10,11,0},
        {11,10,0}, {11,12,0}, {11,19,0},
        {12,3,0}, {12,11,0}, {12,13,0},
        {13,12,0}, {13,14,0}, {13,20,0},
        {14,4,0}, {14,13,0}, {14,15,0},
        {15,6,0}, {15,14,0}, {15,16,0},
        // Inner Layer
        {16,15,0}, {16,17,0}, {16,20,0},
        {17,7,0}, {17,16,0}, {17,18,0},
        {18,9,0}, {18,17,0}, {18,19,0},
        {19,11,0}, {19,18,0}, {19,20,0},
        {20,13,0}, {20,16,0}, {20,19,0},
    };
    // Create instance of class object Graph to represent game map
    Graph map = Graph(edges, n);
    // Create instance of class object Player to represent user's character
    Player player;
    // Create instance of class object Blasilisk to represent type of hazard that object Player can encounter
    Basilisk basilisk;
    // Create instance of class object Pit to represent type of hazard that object Player can encounter
    Pit pit;
    // Create instance of class object Minerail to represent random repositioning of object Player
    Minerail minerail;
    // Create instance of class object Jackal to represent mobile hazard that object Player can encounter
    Jackal jackal;
    // Create instance of class object Jackal to represent mobile hazard that object Player can encounter
    Jackal jackal2;
    // Store cave postitions where a magic arrow randomly spawned
    vector<int> cavesWithArrow;
    // Control game loop
    bool isOver = false;
    // Control turn based events
    int timeStep = 0;
public:
    Game(){}
    
    bool over() { return isOver; }
    
    // Activates if two objects have spawned in the same cave (List of illegal duplicate cave assignments for gameplay listed below)
    // Attempts to reassign unique cave locations to each class object variable
    // MARK: ........................................................................
    // FUNCTION: Repeats if any of the following situations are true:
    //      - Player and Basilisk are positioned in same cave
    //      - Player and Pit are positioned in same cave
    //      - Player and Jackal are positioned in same cave
    //      - Basilisk and Pit are positioned in the same cave
    //      - Basilisk and Minerail Source Cave are positioned in the same cave
    //      - Basilisk and Minerail Destination Cave are positioned in the same cave
    //      - Basilisk and Jackal are positioned in same cave
    //      - Pit and Minerail Source Cave are positioned in the same cave
    //      - Pit and Minerail Destination Cave are positioned in the same cave
    //      - Pit and Jackal are positioned in the same cave
    // MARK: ........................................................................
    //  NOTES:
    //      - Does not create new Minerail cave positions so that accidental duplicate
    //      src/dest cave assignment remains impossible. (Prevented initially in constructor for Minerail)
    void init()
    {
        // Check for duplicate spawn points
        while( player.getCavePos()   == basilisk.getCavePos()     ||
               player.getCavePos()   == pit.getCavePos()          ||
               player.getCavePos()   == jackal.getCavePos()       ||
               player.getCavePos()   == jackal2.getCavePos()      ||
               basilisk.getCavePos() == pit.getCavePos()          ||
               basilisk.getCavePos() == minerail.getSrcCavePos()  ||
               basilisk.getCavePos() == minerail.getDestCavePos() ||
               basilisk.getCavePos() == jackal.getCavePos()       ||
               basilisk.getCavePos() == jackal2.getCavePos()      ||
               pit.getCavePos()      == minerail.getSrcCavePos()  ||
               pit.getCavePos()      == minerail.getDestCavePos() ||
               pit.getCavePos()      == jackal.getCavePos()       ||
               pit.getCavePos()      == jackal2.getCavePos() )
        {
            // Store random values representing cave spawn location
            int randPlayerCave   = rand() % 20 + 1;
            int randBasiliskCave = rand() % 20 + 1;
            int randPitCave      = rand() % 20 + 1;
            int randJackalCave   = rand() % 20 + 1;
            int randJackal2Cave  = rand() % 20 + 1;
            
            // Assign random spawns
            player.setCavePos(randPlayerCave);
            basilisk.setCavePos(randBasiliskCave);
            pit.setCavePos(randPitCave);
            jackal.setCavePos(randJackalCave);
            jackal2.setCavePos(randJackal2Cave);
        }
        
        cout << "\033[31m\033[1mWARNING\033[0m: \033[4mIn European bestiaries and legends, a basilisk is a legendary reptile reputed to be a serpent king, who can cause death with a single glance.\033[0m" << endl;
        
        cout << endl;
        
        cout << "Press \033[33m\033[1mENTER\033[0m to continue...";
        cin.get();
        
        cout << endl;
        cout << "\033[1mGAME START\033[0m" << endl;
    }
    void checkShouldSpawnArrow()
    {
        // Spawn arrow in cave every 7 timesteps such that less than 3 are currently in existence
        // Prevents spawn on time step zero
        if ( (timeStep % 7 == 0) && (cavesWithArrow.size() <= 3) )
        {
            bool isValid = false;
            int randCaveForArrow = 0;
            
            do
            {
                randCaveForArrow = rand() % 20 + 1;
                // Loop runs while random cave for arrow is equal to:
                // - 0 (Initiated value)
                // - Player cave position
                // - Pit cave position
                if ( randCaveForArrow != 0 && randCaveForArrow != basilisk.getCavePos() && randCaveForArrow != pit.getCavePos() )
                {
                    isValid = true;
                }
                else isValid = false;
                
                if ( cavesWithArrow.size() > 0 )
                {
                    for ( auto caves : cavesWithArrow )
                    {
                        if (randCaveForArrow != caves)
                        {
                            isValid = true;
                        }
                        else
                        {
                            isValid = false;
                        }
                    }
                }
                else continue;
                
            } while ( !isValid );
            
            cavesWithArrow.push_back(randCaveForArrow);
            
            // Print message to user indicating arrow spawn
            cout << "\033[34mA magical sound chimes somewhere...\033[0m" << endl;
        }
    }
    void checkForArrowFound()
    {
        //
        int index = 0;
        // Parse vector storing caves with a magic arrow for player to find
        for (auto caves : cavesWithArrow )
        {
            // Player has moved into cave with an arrow
            if ( player.getCavePos() == caves )
            {
                // Print message to terminal
                cout << "\033[33m\033[1mYou come across a dropped arrow from a previous hunter.\033[0m" << endl;
                
                // Erase element from vector
                cavesWithArrow.erase( cavesWithArrow.begin() + index );
                
                // Increment player's magic arrow count
                player.addArrow();
                
                // Display number of arrows remaining in player inventory
                cout << "\033[3mYou have " << player.getArrowCount() << " magic arrows remaining.\033[0m" << endl;
            }
            index++;
        }
    }
    void showMinerailFoundMessage()
    {
        cout << "\033[1m\033[33mYou discover an abandoned mineshaft with a minecart. It still works...\033[0m" << endl;
    }
    void showNeighborCaveMessages()
    {
        bool jackalNear  = false;
        bool jackal2Near = false;
        bool jackalApproaching  = false;
        bool jackal2Approaching = false;
        
        for (int i = 1; i <= 20; i++)
        {
            for ( Pair v : map.adjList[i] )
            {
                // MARK: Player is adjacent to basilisk and EITHER:
                // Hasn't disturbed basilisk this game
                // Or, DID search cave where basilisk was disturbed
                if ( i == player.getCavePos() && v.first == basilisk.getCavePos() && basilisk.getCaveWhereDisturbed() == 0 )
                {
                    cout << "\033[34mYou hear a hissing sound...\033[0m" << endl;
                }
                
                // MARK: Player is adjacent to basilisk and BOTH:
                // Has disturbed basilisk this game
                // AND, DID NOT search save where he was was disturbed
                if ( i       == player.getCavePos() &&
                     v.first == basilisk.getCavePos() &&
                     v.first != basilisk.getCaveWhereDisturbed() &&
                     basilisk.getCaveWhereDisturbed() != 0 )
                {
                    cout << "\033[34mSomething feels wrong...\033[0m" << endl;
                }
                
                // MARK: Player is next to cave where basilisk was disturbed and cave is unsearched since disturbance
                if ( i == player.getCavePos() &&
                     v.first == basilisk.getCaveWhereDisturbed() &&
                     basilisk.getCaveWhereDisturbed() != 0 )
                {
                    cout << "\033[34mIt's dead silent...\033[0m" << endl;
                }
                
                // MARK: Player is adjacent to a pit
                if ( i == player.getCavePos() && v.first == pit.getCavePos() )
                {
                    cout << "\033[34mIt's so cold in here...\033[0m" << endl;
                }
                
                // MARK: Player is adjacent to minerail source cave
                if ( i == player.getCavePos() && v.first == minerail.getSrcCavePos() )
                {
                    cout << "\033[34mThe smell of rusting metal permeates the air...\033[0m" << endl;
                }
                
                // MARK: Player is in an adjacent cave to Jackal
                if ( i == player.getCavePos() && v.first == jackal.getCavePos() )
                {
                    jackalNear = true;
                }
                
                // MARK: Player is in an adjacent cave to Jackal2
                if ( i == player.getCavePos() && v.first == jackal2.getCavePos() )
                {
                    jackal2Near = true;
                }
                
                // MARK: Print message if Jackal is headed towards Player
                if ( i == player.getCavePos() && v.first == jackal.getCavePos() && jackal.getNextCavePos() == player.getCavePos() )
                {
                    jackalApproaching = true;
                }
                
                // MARK: Print message if Jackal2 is headed towards Player
                if ( i == player.getCavePos() && v.first == jackal2.getCavePos() && jackal2.getNextCavePos() == player.getCavePos() )
                {
                    jackal2Approaching = true;
                }
            }
        }
        
        // Check if one or both Jackals are near Player
        if ( jackalNear && jackal2Near )
        {
            cout << "\033[34mFootsteps echo from multiple directions...\033[0m" << endl;
        }
        else if ( (jackalNear && !jackal2Near) || (!jackalNear && jackal2Near) )
        {
            cout << "\033[34mFaint footsteps can be heard in the distance...\033[0m" << endl;
        }
        
        // Check if one or both Jackals are approaching Player
        if ( jackalApproaching && jackal2Approaching )
        {
            cout << "\033[34mThe echoes surrond you...\033[0m" << endl;
        }
        else if ( (jackalApproaching && !jackal2Approaching) || (!jackalApproaching && jackal2Approaching) )
        {
            cout << "\033[34mThe footsteps grow louder...\033[0m" << endl;
        }
    }
    void showCaveInfo()
    {
        
        // Check if Player and Jackal encountered each other
        if ( jackal.getCavePos() == player.getCavePos() )
        {
            jackal.attackPlayer( &player, basilisk.getCavePos(), pit.getCavePos(), map, n );
            
            if ( jackal.didMovePlayer )
            {
                this->showPlayerMoveDialogue( player.getCavePos(), true );
                if (isOver)
                {
                    return;
                }
            }
        }
        
        // Check if Player and Jackal2 encountered each other
        if ( jackal2.getCavePos() == player.getCavePos() )
        {
            jackal2.attackPlayer( &player, basilisk.getCavePos(), pit.getCavePos(), map, n );
            
            if ( jackal2.didMovePlayer )
            {
                this->showPlayerMoveDialogue( player.getCavePos(), true );
                if (isOver)
                {
                    return;
                }
            }
        }
        
        cout << "\033[1m----------------------------------------------------------\033[0m" << endl;
        
        // Determine Jackal's next cave position
        jackal.findNextCave( basilisk.getCavePos(), pit.getCavePos(), map, n );
        
        // Determine Jackal's next cave position
        jackal2.findNextCave( basilisk.getCavePos(), pit.getCavePos(), map, n );
        
        // Check if player has encountered randomly spawned magic arrow
        this->checkForArrowFound();
        
        // Randomly spawn magic arrow on map every 7 time steps, excluding time step zero
        if ( timeStep > 0 )
        {
            // Check if magic arrow should randomly spawn
            this->checkShouldSpawnArrow();
        }
        
        // Print minerail message if player has found minerail source cave position
        if ( player.getCavePos() == minerail.getSrcCavePos() )
        {
            this->showMinerailFoundMessage();
        }
        
        // Print harmful neighbor cave information if relevant
        this->showNeighborCaveMessages();
                
        // Print current cave information to terminal
        cout << "\033[1m\033[4mYou are in cave: " << '(' << player.getCavePos() << ')' << "\033[0m" << endl;
        
        // Print adjacent caves values
        cout << "A sign tells you that neighboring caves are: ";
        for (auto &edge : edges)
        {
            if ( edge.src == player.getCavePos() )
            {
                cout << '(' << edge.dest << ')';
            }
        }
        cout << endl;
    }
    PlayerOptions getPlayerMoveOrShoot()
    {
        string line;
        char choice;
        if ( player.getCavePos() == minerail.getSrcCavePos() )
        {
            cout << "\033[3m*** Enter 'R' to Ride Minerail ***\033[0m" << endl;
            cout << "Will you Move, Shoot, or Ride Minerail? (M/S/R): ";
        }
        else
        {
            cout << "Will you Move or Shoot? (M/S): ";
        }
        getline(cin, line);
        if ( line.size() == 1 )
        {
            // Access and assigned
            choice = line.at(0);
        }
        else
        {
            // String was empty or contained too many characters; assigned 'i' to represent 'invalid'
            choice = 'i';
        }
        
        return static_cast<PlayerOptions>(static_cast<char>(toupper(static_cast<unsigned int>(choice))));
    }
    
    int getPlayerMoveDestination()
    {
        string line;
        int caveMoveDest;
        cout << "Enter a cave to move to: ";
        getline(cin, line);
        if ( line.size() > 0 && line.size() < 3 )
        {
            caveMoveDest = static_cast<int>( stoi( line ) );
        }
        else
        {
            caveMoveDest = 0;
        }
        
        // Exit function if move destination is invalid
        if ( !caveMoveDest )
        {
            cout << "Invalid entry" << endl;
            return 0;
        }
        
        return caveMoveDest;
    }
    
    int getPlayerShootDestination()
    {
        string line;
        int caveShootDest;
        cout << "Enter a cave to shoot into: ";
        getline(cin, line);
        if ( line.size() > 0 && line.size() < 3 )
        {
            
            // Access and assign
            caveShootDest = static_cast<int>( stoi( line ) );
        }
        else
        {
            // String was empty or contained too many characters; assigned '0' to represent 'invalid'
            caveShootDest = 0;
        }
        
        // Exit function if shoot destination is invalid
        if ( !caveShootDest )
        {
            cout << "Invalid entry" << endl;
            return 0;
        }
        
        return caveShootDest;
    }
    
    void handleChoice()
    {
        // Store cave position before move
        int prevPlayerCavePos = player.getCavePos();
        
        switch( this->getPlayerMoveOrShoot() )
        {
            case MOVE:
                // Move Player
                this->playerMove();
                
                // Move Jackal
                jackal.moveJackal( prevPlayerCavePos, player.getCavePos() );
                
                // Move Jackal2
                jackal2.moveJackal( prevPlayerCavePos, player.getCavePos() );
                
                // Increment time step
                timeStep++;
                
                break;
                
            case SHOOT:
                
                // Player shoot magic arrow
                this->playerShoot();
                
                // Move Jackal
                jackal.moveJackal( prevPlayerCavePos, player.getCavePos() );
                
                // Move Jackal2
                jackal2.moveJackal( prevPlayerCavePos, player.getCavePos() );

                // Increment time step
                timeStep++;
                
                // Prevents enter press when player has won
                if ( !isOver )
                {
                    cout << "Press \033[33m\033[1mENTER\033[0m to continue...";
                    cin.get();
                }
                
                break;
                
            case RIDE:
                
                // Player ride minecart
                this->playerRide();
                
                // Move Jackal
                jackal.moveJackal( prevPlayerCavePos, player.getCavePos() );
                
                // Move Jackal2
                jackal2.moveJackal( prevPlayerCavePos, player.getCavePos() );

                // Increment time step
                timeStep++;
                
                cout << "Press \033[33m\033[1mENTER\033[0m to continue...";
                cin.get();
                
                break;
                
            default:
                cout << "Enter 'M'/'m' to move, or enter 'S'/'s' to shoot." << endl;
                
                // Wait for user to continue
                cout << "Press \033[33m\033[1mENTER\033[0m to continue...";
                cin.get();
                
                break;
        }
        
        if ( isOver )
        {
            cout << "\033[1mGAME OVER\033[0m" << endl;
        }
    }
    void showPlayerMoveDialogue(int caveMoveDest, bool ignoreConfirmMessage = false)
    {
        if ( !ignoreConfirmMessage )
        {
            // Print message to player to confirm move choice
            cout << "\033[1m\033[4mYou stagger through a pitch black tunnel towards cave " << '(' << caveMoveDest << ')' << "\033[0m" << endl;
            
            // Wait for user to continue
            cout << "Press \033[33m\033[1mENTER\033[0m to continue...";
            cin.get();
        }
        
        //  Player is moving into cave where basilisk resides and cave where disturbed was checked or irrelevant
        if ( caveMoveDest == basilisk.getCavePos() && basilisk.getCaveWhereDisturbed() == 0)
        {
            isOver = true;
            cout << "\033[31mAs you walk in, you look up and lock eyes with the Basilisk.\033[0m" << endl;
            cout << "\033[31m\033[1m\033[4mAt least it was quick...\033[0m" << endl;
        }
        //  Player is moving into cave where basilisk resides and basilisk has been disturbed without player checking cave
        else if ( caveMoveDest == basilisk.getCavePos() && basilisk.getCaveWhereDisturbed() != 0)
        {
            isOver = true;
            cout << "\033[31m\033[1mThe Basilisk jumps out from nowhere and tears you to shreds...\033[0m" << endl;
            cout << "\033[4mBasilisk hunters must act fearless in the face of danger...\033[0m" << endl;
        }
        // If player if moving to where basilisk was disturbed and basilisk has moved, reset variables
        else if( caveMoveDest == basilisk.getCaveWhereDisturbed() && caveMoveDest != basilisk.getCavePos() )
        {
            cout << "\033[1m\033[4mAs you enter, you hear a loud crash echo from one of the tunnels...\033[0m" << endl;
            cout << "\033[1m\033[4mWhatever it was must've ran... It was wise to come check.\033[0m" << endl;
            basilisk.setCaveWhereDisturbed(0);
            
            // Set player cave to position new cave
            player.setCavePos(caveMoveDest);
            
            cout << "Press \033[33m\033[1mENTER\033[0m to continue...";
            cin.get();
        }
        // Player is moving into cave where a pit resides
        else if ( caveMoveDest == pit.getCavePos() )
        {
            isOver = true;
            cout << "\033[1m\033[1m\033[4mYou didn't even have time to react before stumbling into a pit.\033[0m" << endl;
            cout << "\033[1m\033[4mNot too savvy...\033[0m" << endl;
        }
        // Player can safely move into new cave
        else
        {
            // Set player cave to position new cave
            player.setCavePos(caveMoveDest);
        }
    }
    void playerMove()
    {
        // Fetch cave move destination from user
        int caveMoveDest = getPlayerMoveDestination();
        
        // Bad input given
        if ( !caveMoveDest ) { return; }
        
        for (auto &edge : edges)
        {
            // Found edge player is traveling across
            if ( edge.src == player.getCavePos() && edge.dest == caveMoveDest )
            {
                // Print dialogue for scenario
                showPlayerMoveDialogue(caveMoveDest);
                
                // Exit function loop to prevent further unneccesary execution
                return;
            }
        }
    }
    void playerShoot()
    {
        // Player has 1 or more arrows
        if ( player.getArrowCount() )
        {
            int caveShootDest = getPlayerShootDestination();
            
            // Bad input given
            if ( !caveShootDest ) { return; }
            
            // Parse edges
            for (auto &edge : edges)
            {
                // Find player's current cave and respective cave destination for magic dart
                if ( edge.src == player.getCavePos() && edge.dest == caveShootDest )
                {
                    
                    // Print message to user confirming choice to shoot magic arrow
                    cout << "\033[1m\033[4mYou fire a magic arrow into cave " << '(' << caveShootDest << ')' << "\033[0m" << endl;
                    
                    // Wait for user to continue
                    cout << "Press \033[33m\033[1mENTER\033[0m to continue...";
                    cin.get();
                    
                    // Check if basilisk resides in cave being shot into
                    if ( caveShootDest == basilisk.getCavePos() )
                    {
                        // Create variable to represent 25% chance of hitting basilisk (Range: 0 to 3) Hit on 0
                        int wasHit = rand() % 4;
                        
                        // Create variable to represent 50% chance of basilisk moving to new cave (Range: 0 to 3) Relocate on 0 and 1
                        int willMove = rand() % 4;
                        
                        // Basilisk was hit
                        if (wasHit == 0)
                        {
                            // Player won!
                            isOver = true;
                            cout << "\033[32m\033[1m\033[4mYou killed the basilisk. You are a remarkable hunter!\033[0m" << endl;
                            return;
                        }
                        // Basilisk evaded arrow
                        else
                        {
                            // Basilisk scurries away into a random cave
                            if( willMove < 2 )
                            {
                                // Store new cave position for basilisk
                                int newCavePos;
                                bool isValid = false;
                                
                                // Continue generating new position if it matches the player's current cave position
                                do
                                {
                                    newCavePos = rand() % 20 + 1;
                                    // Check if new cave location already contains any of the following:
                                    //      - Player
                                    //      - Basilisk (old cave position)
                                    //      - Pit
                                    //      - Minerail (Source OR Destination)
                                    //      - Jackal
                                    // If new cave position is occupied, it's invalid and loop must run again
                                    if ( newCavePos != player.getCavePos()       &&
                                         newCavePos != basilisk.getCavePos()     &&
                                         newCavePos != pit.getCavePos()          &&
                                         newCavePos != minerail.getSrcCavePos()  &&
                                         newCavePos != minerail.getDestCavePos() &&
                                         newCavePos != jackal.getCavePos()       &&
                                         newCavePos != jackal2.getCavePos() )
                                    {
                                        isValid = true;
                                    }
                                    // Next, verify that it is not a cave with an arrow
                                    for (auto caves : cavesWithArrow)
                                    {
                                        if ( caves == newCavePos )
                                        {
                                            isValid = false;
                                            break;
                                        }
                                        else
                                        {
                                            isValid = true;
                                        }
                                    }
                                } while ( !isValid );
                                
                                // Reposition basilisk
                                basilisk.setCavePos(newCavePos);
                            }
                            // Regardless if basilisk moved, his status is set to disturbed and same message is given
                            cout << "\033[1m\033[3mAn uproar bellows from the cave...\033[0m" << endl;
                            basilisk.setCaveWhereDisturbed(caveShootDest);
                        }
                        
                        // Exit loop to prevent further unneccesary execution
                        break;
                    }
                    // Player shot arrow into 'empty' cave
                    else
                    {
                        cout << "\033[1m\033[4mNothing happens... You lose your arrow in the darkness.\033[0m" << endl;
                    }
                    
                    // Exit loop to prevent further unneccesary execution
                    break;
                }
            }
            // Decrement player arrows by one
            player.removeArrow();
            
            // Display number of arrows remaining in player inventory
            cout << "\033[3mYou have " << player.getArrowCount() << " magic arrows remaining.\033[0m" << endl;
        }
        // Player has 0 magic arrows remaining
        else
        {
            cout << "You're out of arrows!" << endl;
            return;
        }
    }
    
    void playerRide()
    {
        cout << "You get in the minecart and hunker down. It's not exactly made for comfort..." << endl;
        
        int destCave = minerail.getDestCavePos();
        
        player.setCavePos( destCave );
        
        cout << "\033[1m\033[4mYou find yourself arriving in cave " << '(' << destCave << ')' << "\033[0m" << endl;
        
        bool isValid = false;
        int randNewSrcCave;
        int randNewDestCave;
        
        while ( !isValid )
        {
            randNewSrcCave = rand() % 20 + 1;
            randNewDestCave = rand() % 20 + 1;
            
            if ( randNewSrcCave != basilisk.getCavePos() &&
                 randNewSrcCave != pit.getCavePos() &&
                 randNewSrcCave != player.getCavePos() )
            {
                isValid = true;
            }
        }
        
        minerail.setSrcCavePos(randNewSrcCave);
        minerail.setDestCavePos(randNewDestCave);
    }
};

#endif // Game_hpp
