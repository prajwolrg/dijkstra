#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>

class Node
{
  public:
    Node();

    void Create(float, float, int);

    sf::Vector2f getPosition();

    void Render(sf::RenderWindow& l_window);
    void isSelected(bool);
    int nodenumber;

  private:
    sf::Vector2f m_position;
    sf::CircleShape nodeshape;

    sf::Text text;
};