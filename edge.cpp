#include "edge.h"
#include <cmath>
#define PI 3.1415926535
using namespace std;

Edge::Edge()
{
    extern sf::Font font;
    text.setFont(font);
    text.setFillColor(sf::Color::Cyan);
    text.setCharacterSize(15);
}

void Edge::Create(int nodeA, int nodeB, sf::Vector2f headposition, sf::Vector2f tailposition)
{
    edgeshape.setFillColor(sf::Color::Cyan);
    this->nodeA = nodeA;
    this->nodeB = nodeB;
    weight = 5;
    head_position = headposition;
    tail_position = tailposition;

    float dx = tailposition.x - headposition.x;
    float dy = tailposition.y - headposition.y;
    float edgerotation = atan2(dy, dx);
    rotation = (atan2(dy, dx)) * 180 / PI;

    width = (headposition.y - tailposition.y) * (headposition.y - tailposition.y) + (headposition.x - tailposition.x) * (headposition.x - tailposition.x);
    width = sqrt(width);

    edgeshape.setSize({width, 2});

    edgeshape.setOrigin({edgeshape.getSize().x/2, edgeshape.getSize().y/2});
    edgeshape.setPosition({(headposition.x+tailposition.x)/2 , (headposition.y + tailposition.y)/2});

    edgeshape.setRotation(rotation);

    sf::Vector2f translation;
    if (edgerotation < 0)
    {
        edgerotation += PI;
    }

    if (edgerotation >= 0 && edgerotation <= PI / 2)
    {
        text.setPosition({(head_position.x + tail_position.x) / 2 - text.getCharacterSize() * abs(sin(edgerotation)), (head_position.y + tail_position.y) / 2});
    }
    else
    {
        text.setPosition({(head_position.x + tail_position.x) / 2 + abs(sin(edgerotation)), (head_position.y + tail_position.y) / 2 + 5 * abs(cos(edgerotation))});
    }
}

void Edge::dCreate(sf::Vector2f headposition, sf::Vector2f tailposition)
{
    edgeshape.setFillColor(sf::Color::Magenta);
    float dx = tailposition.x - headposition.x;
    float dy = tailposition.y - headposition.y;
    float rotation = (atan2(dy, dx)) * 180 / PI;

    width = (headposition.y - tailposition.y) * (headposition.y - tailposition.y) + (headposition.x - tailposition.x) * (headposition.x - tailposition.x);
    width = sqrt(width);

    edgeshape.setSize({width, 5});

    edgeshape.setOrigin({edgeshape.getSize().x/2, edgeshape.getSize().y/2});
    edgeshape.setPosition({(headposition.x+tailposition.x)/2 , (headposition.y + tailposition.y)/2});

    edgeshape.setRotation(rotation);
}

void Edge::updateWeight(std::string str)
{
    int i = std::stoi(str);
    weight = i;
    text.setString(str);
}

sf::Vector2f Edge::getPosition()
{
    return edgeshape.getPosition();
}

void Edge::Render(sf::RenderWindow &l_window)
{
    l_window.draw(edgeshape);
    l_window.draw(text);
}