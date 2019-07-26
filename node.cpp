#include "node.h"
#include <iostream>

Node::Node()
{
    extern sf::Font font;

    nodeshape.setRadius(15);
    nodeshape.setOrigin(nodeshape.getRadius(), nodeshape.getRadius());
    nodeshape.setFillColor(sf::Color::Black);
    nodeshape.setOutlineThickness(2);
    nodeshape.setOutlineColor(sf::Color::Cyan);

    text.setFont(font);
    text.setCharacterSize(15);
    text.setFillColor(sf::Color::Cyan);

    weight.setFont(font);
    weight.setFillColor(sf::Color::Cyan);
    weight.setCharacterSize(15);
}

void Node::Create(float x, float y, int offset)
{
    nodeshape.setPosition({x, y});
    text.setPosition({x - 5, y - 9});
    nodenumber = offset;
    char c = static_cast<char>(65 + offset);
    text.setString(c);
    weight.setPosition({x-5, y-35});
}

void Node::isSelected(bool value)
{
    if (value==false)
    {
        nodeshape.setFillColor(sf::Color::Black);
        text.setFillColor(sf::Color::Cyan);
    }
    if (value == true)
    {
        nodeshape.setFillColor(sf::Color::Cyan);
        text.setFillColor(sf::Color::Black);
    }
}

sf::Vector2f Node::getPosition()
{
    return nodeshape.getPosition();
}

void Node::Render(sf::RenderWindow &l_window)
{
    l_window.draw(nodeshape);
    l_window.draw(text);
    l_window.draw(weight);
}
