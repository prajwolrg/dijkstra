#include "game.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

using namespace std;

sf::Font font;

int main()
{
    if (!font.loadFromFile("arial.ttf"))
    {
        std::cout << "Error!!!" << endl;
    }
    Game game;

    while (!game.GetWindow()->IsDone())
    {
        game.HandleMovements();
        game.Update();
        game.Render();
    }
}
