#include "node.h"
#include <iostream>

Node::Node()
{
    extern sf::Font font;
    nodeshape.setRadius(5);
    nodeshape.setOrigin(nodeshape.getRadius(), nodeshape.getRadius());
    nodeshape.setFillColor(sf::Color::Cyan);
    text.setFont(font);
    text.setFillColor(sf::Color::Cyan);
    text.setCharacterSize(15);
}

void Node::Create(float x, float y, int offset)
{
    nodeshape.setPosition({x, y});
    nodenumber = offset;
    char c = static_cast<char>(65 + offset);
    text.setString(c);
    text.setPosition({x,y-25});
}

sf::Vector2f Node::getPosition()
{
    return nodeshape.getPosition();
}

void Node::Render(sf::RenderWindow &l_window)
{
    l_window.draw(nodeshape);
    l_window.draw(text);
}
