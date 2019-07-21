#include "Game.h"
#include <math.h>
#include <iostream>
#include <limits>
#include <list>
using namespace std;
#define PI 3.1415926

Game::Game() : m_window("Graph Creator Window", sf::Vector2u(500, 600)), total_nodes(0), total_edges(0), prev_position({0, 0})
{
    delaytime = sf::seconds(0.5);
    double l = numeric_limits<double>::infinity();
    for (int i=0; i<10; i++)
    {
        for (int j=0; j<10; j++)
        {
            if (i==j)
            {
                Matrix[i][j]=0;
            }
            else
            {
                Matrix[i][j]= l;
            }
        }
    }
}

Game::~Game() {}

Window *Game::GetWindow() { return &m_window; }

void Game::HandleMovements()
{
    if (m_window.Active() && sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        float x = sf::Mouse::getPosition(*m_window.GetRenderWindow()).x;
        float y = sf::Mouse::getPosition(*m_window.GetRenderWindow()).y;
        float xx = pow(x - prev_position.x, 2);
        float yy = pow(y - prev_position.y, 2);
        float dist = xx + yy;
        dist = sqrt(dist);
        if (dist > 25)
        {
            nodes[total_nodes++].Create(x, y, total_nodes - 1);
            prev_position = {x, y};
        }
    }
    if (m_window.Active() && sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        float x = sf::Mouse::getPosition(*m_window.GetRenderWindow()).x;
        float y = sf::Mouse::getPosition(*m_window.GetRenderWindow()).y;

        //Use Clock Later
        sf::sleep(delaytime);
        //Use Clock Later

        for (int i = 0; i < total_nodes; i++)
        {
            float dx = nodes[i].getPosition().x - x;
            float dy = nodes[i].getPosition().y - y;
            float d = sqrt(dx * dx + dy * dy);
            char c;

            if (d < 15)
            {
                c = 65 + i;
                selected_nodes.push_back(nodes[i]);
                cout << "Node " << c << " selected. Nodes " << selected_nodes.size() << endl;
                break;
            }
        }

        if (selected_nodes.size() == 2)
        {
            //Create edge between selected nodes.
            Node A = selected_nodes.front();
            sf::Vector2f headposition = A.getPosition();
            selected_nodes.pop_front();
            Node B = selected_nodes.back();
            sf::Vector2f tailposition = B.getPosition();
            selected_nodes.pop_front();

            edges[total_edges++].Create(A.nodenumber, B.nodenumber, headposition, tailposition);
        }
    }

    if (m_window.Active() && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
    {
        int initial;
        CreateMatrix();
        sf::sleep(delaytime);
        cout << "Enter initial node : ";
        cin >> initial;
        Dijkstra(Matrix, initial);
    }
}

void Game::Dijkstra(double M[10][10], int initial)
{
    list<int> available;
    for (int i = 0; i < total_nodes; i++)
    {
        if (i == initial)
        {
            p[i].fixed = true;
        }
        else
        {
            available.push_back(i);
            p[i].fixed = false;
        }
        p[i].weight = M[initial][i];
        p[i].nodepath.push_back(initial);
    }

    int u = rand()%total_nodes;
    while (u == initial)
    {
        u = rand()%total_nodes;
    }

    for (int i = 0; i < total_nodes; i++)
    {
        //Select node
        for (int j = 0; j < total_nodes; j++)
        {
            if (j != initial && p[j].fixed == false)
            {
                if (M[initial][j] < M[initial][u])
                {
                    u = j;
                }
            }
        }
        cout << u << " is selected. " << endl;
        p[u].nodepath.push_back(u);
        p[u].fixed = true;
        p[u].weight = M[initial][u];

        //Modify other nodes
        for (int j = 0; j < total_nodes; j++)
        {
            if (j != initial && p[j].fixed == false)
            {
                if (M[initial][j] > M[initial][u] + M[u][j])
                {
                    M[initial][j] = M[initial][u] + M[u][j];
                    p[j].nodepath = p[u].nodepath;
                }
            }
        }

        //Change u to anything but initial and previous u
        available.remove(u);
        if (available.empty()) { break; }
        else { u = available.front(); }
    }

    for (int i = 0; i <total_nodes; i++)
    {
        char c;
        cout << "Initial Node: " << initial << "\t Final Node: " << i << " \tWeight: " << p[i].weight << "\tPath: ";
        while (!p[i].nodepath.empty())
        {
            c = p[i].nodepath.front() + 65;
            cout << c << "  ";
            p[i].nodepath.pop_front();
        }
        cout << endl;
    }
}

void Game::Update() { m_window.Update(); }

void Game::CreateMatrix()
{
    for (int i=0; i<total_edges; i++)
    {
        int a = edges[i].nodeA;
        int b = edges[i].nodeB;

        Matrix[a][b] = edges[i].weight;
        Matrix[b][a] = edges[i].weight;
    }

    cout << endl << "The Matrix form of the given graph is: " <<endl;
    for (int i=0; i<total_nodes; i++)
    {
        for (int j=0; j<total_nodes; j++)
        {
            cout << Matrix[i][j] << "\t";
        }
        cout << endl;
    }
}

void Game::Render()
{
    m_window.BeginDraw();
    for (int i = 0; i < total_nodes; i++)
    {
        nodes[i].Render(*m_window.GetRenderWindow());
    }
    for (int i = 0; i < total_edges; i++)
    {
        edges[i].Render(*m_window.GetRenderWindow());
    }
    m_window.EndDraw();
}