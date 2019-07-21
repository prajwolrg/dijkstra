#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>

class Edge
{
  public:
    Edge();

    void Create(int, int, sf::Vector2f, sf::Vector2f);

    sf::Vector2f getPosition();

    void Render(sf::RenderWindow& l_window);
    int nodeA, nodeB;
    int weight;

  private:
    sf::Vector2f head_position;
    sf::Vector2f tail_position;
    sf::RectangleShape edgeshape;
    float width;
    float rotation;
    sf::Text text;
};