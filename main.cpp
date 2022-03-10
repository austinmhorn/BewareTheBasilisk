#ifndef __cplusplus
#error "Missing c++ libraries"
#elif defined __cplusplus

#include "Game.hpp"

int main()
{
    srand(time(NULL));

    // Create instance of Game Engine
    Game game;
    
    // Initialize engine
    game.init();

    // Run game loop
    while ( !game.over() )
    {
        // Print relative cave information to user
        game.showCaveInfo();
        
        // Handle user input
        game.handleChoice();
    }
    
    return 0;
}

#endif // __cplusplus
