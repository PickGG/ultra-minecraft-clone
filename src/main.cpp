#include "Game.hpp"

int main()
{
    Game* game = Game::GetInstance();
    if(!game->Startup())
        return 1;
    game->Run();
    game->Shutdown();
    return 0;
}
