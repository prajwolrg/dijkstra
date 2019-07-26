#include "Game.h"
#include <math.h>
#include <iostream>
#include <limits>
#include <list>
using namespace std;
#define PI 3.1415926

Game::Game() : m_window("Graph Creator Window", sf::Vector2u(800, 600)), total_nodes(0), total_edges(0), total_dedges(0), prev_position({0, 0}), dijkstra(&Game::Dijkstra, this)
{
    extern sf::Event event;
    delaytime = sf::seconds(1);
    mouseclock.restart();
    keyclock.restart();

    double l = abs(numeric_limits<double>::infinity()) / 2;
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            if (i == j)
            {
                Matrix[i][j] = 0;
                M[i][j] = 0;
            }
            else
            {
                Matrix[i][j] = l;
                M[i][j] = l;
            }
        }
    }
    gui.setTarget(*m_window.GetRenderWindow());
    theme.load("theme/Black.txt");

    weightBox = tgui::EditBox::create();
    weightBox->setSize({50, 30});
    weightBox->setDefaultText("Enter Weight");
    weightBox->setVisible(false);
    weightBox->setRenderer(theme.getRenderer("EditBox"));
    gui.add(weightBox);

    clearinfo = tgui::TextBox::create();
    clearinfo->setText("This clears all the edges and the nodes that are displayed on the screen.");
    clearinfo->setSize({200, 50});
    clearinfo->setRenderer(theme.getRenderer("TextBox"));

    clear = tgui::Button::create();
    clear->setToolTip(clearinfo);
    clear->setRenderer(theme.getRenderer("ClearButton"));
    clear->setPosition({10, 10});
    clear->setSize(30, 30);
    gui.add(clear);

    promptBox = tgui::TextBox::create();
    promptBox->setRenderer(theme.getRenderer("TextBox"));
    promptBox->setText("Initial");
    promptBox->setVisible(false);
    promptBox->setSize({200, 30});
    promptBox->setPosition({m_window.GetWindowSize().x - promptBox->getSize().x, 10});
    gui.add(promptBox);

    activity = tgui::Label::create();
    activity->setText("Recent Activity: Graph Creator Initialized");
    activity->setPosition({(m_window.GetWindowSize().x - activity->getSize().x) / 2, m_window.GetWindowSize().y - 30});
    activity->setRenderer(theme.getRenderer("Label"));
    gui.add(activity);

    promptInitialSelect = tgui::Label::create();
    promptInitialSelect->setVisible(false);
}

void Game::clearAll(bool val)
{
    for (int i=0; i<total_nodes; i++)
    {
        nodes[i].weight.setString(" ");
        nodes[i].isSelected(false);
    }
    total_nodes = 0;
    total_edges = 0;
    total_dedges = 0;

    promptInitialSelect->setVisible(false);

    double l = abs(numeric_limits<double>::infinity()) / 2;
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            if (i == j)
            {
                Matrix[i][j] = 0;
                M[i][j] = 0;
            }
            else
            {
                Matrix[i][j] = l;
                M[i][j] = l;
            }
        }
    }
}

Game::~Game() {}

Window *Game::GetWindow() { return &m_window; }

float Game::dist(sf::Vector2f pos1, sf::Vector2f pos2)
{
    float x1 = pos1.x;
    float x2 = pos2.x;
    float y1 = pos1.y;
    float y2 = pos2.y;

    float dist = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
    return (sqrt(dist));
}

void Game::changeActivity(std::string message)
{
    std::string default_string = "Recent Activity: ";
    message = default_string + message;
    activity->setText(message);
}

void Game::HandleMovements()
{
    if (m_window.Active() && sf::Mouse::isButtonPressed(sf::Mouse::Right) && !weightBox->isVisible())
    {
        float x = sf::Mouse::getPosition(*m_window.GetRenderWindow()).x;
        float y = sf::Mouse::getPosition(*m_window.GetRenderWindow()).y;
        for (int i = 0; i < total_nodes; i++)
        {
            if (dist(nodes[i].getPosition(), sf::Vector2f(x, y)) < 50)
            {
                return;
            }
        }
        nodes[total_nodes++].Create(x, y, total_nodes - 1);
        changeActivity("New node created");
    }

    if (m_window.Active() && sf::Mouse::isButtonPressed(sf::Mouse::Left) && (!weightBox->isVisible() || promptInitialSelect->isVisible()))
    {
        float x = sf::Mouse::getPosition(*m_window.GetRenderWindow()).x;
        float y = sf::Mouse::getPosition(*m_window.GetRenderWindow()).y;

        if (mouseclock.getElapsedTime() < sf::seconds(0.5))
        {
            return;
        }
        if (clear->mouseOnWidget({x, y}))
        {
            changeActivity("Screen has been cleared.");
            clearAll(true);
        }
        for (int i = 0; i < total_nodes; i++)
        {
            char c;
            std::string message = "Node ";
            if (dist(nodes[i].getPosition(), sf::Vector2f(x, y)) < 25)
            {
                c = 65 + i;
                message.push_back(c);
                if (selected_nodes.front().getPosition() == nodes[i].getPosition())
                {
                    message = message + " is already selected.";
                    changeActivity(message);
                    break;
                }
                message += " is selected.";
                nodes[i].isSelected(true);
                changeActivity(message);
                selected_nodes.push_back(nodes[i]);
                if (promptInitialSelect->isVisible())
                {
                    message = message + " is selected as initial node.";
                    changeActivity(message);
                    nodes[i].isSelected(true);
                    promptInitialSelect->setVisible(false);
                    initial = i;
                    dijkstra.launch();
                    selected_nodes.clear();
                }
                break;
            }
        }
        mouseclock.restart();
    }

    if (selected_nodes.size() == 2 && !promptInitialSelect->isVisible())
    {
        //Create edge between selected nodes.
        Node A = selected_nodes.front();
        sf::Vector2f headposition = A.getPosition();
        selected_nodes.pop_front();
        Node B = selected_nodes.back();
        sf::Vector2f tailposition = B.getPosition();
        selected_nodes.pop_front();

        nodes[A.nodenumber].isSelected(false);
        nodes[B.nodenumber].isSelected(false);

        std::string message = "New edge created between ";
        message.push_back(A.nodenumber + 65);
        message = message + " and ";
        message.push_back(B.nodenumber + 65);
        changeActivity(message);

        edges[total_edges++].Create(A.nodenumber, B.nodenumber, headposition, tailposition);

        float dx = tailposition.x - headposition.x;
        float dy = tailposition.y - headposition.y;
        float edgerotation = atan2(dy, dx);
        if (edgerotation < 0)
        {
            edgerotation += PI;
        }
        if (edgerotation >= 0 && edgerotation <= PI / 2)
        {
            weightBox->setPosition({(headposition.x + tailposition.x) / 2 - 15 * abs(sin(edgerotation)), (headposition.y + tailposition.y) / 2});
        }
        else
        {
            weightBox->setPosition({(headposition.x + tailposition.x) / 2 + abs(sin(edgerotation)), (headposition.y + tailposition.y) / 2 + 5 * abs(cos(edgerotation))});
        }
        weightBox->setVisible(true);
    }

    if (m_window.Active() && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
    {
        if (keyclock.getElapsedTime() < sf::seconds(0.5))
        {
            return;
        }
        if (!weightBox->isVisible())
        {
            dijkstra.terminate();
            changeActivity("WAITING FOR THE USER TO SELECT INITIAL NODE.");
            CreateMatrix();
            promptInitialSelect->setVisible(true);
        }
        if (weightBox->isVisible())
        {
            string s = weightBox->getText();
            edges[total_edges - 1].updateWeight(s);
            weightBox->setVisible(false);
            changeActivity("Weight added to the edge");
        }
        keyclock.restart();
    }
}

void Game::Dijkstra()
{
    sf::sleep(delaytime);
    changeActivity("Dijkstra Algorithm Started");
    list<int> available;
    for (int i = 0; i < total_nodes; i++)
    {
        p[i].nodepath.clear();
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

    int u = rand() % total_nodes;
    while (u == initial)
    {
        u = rand() % total_nodes;
    }
    nodes[initial].isSelected(true);

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
        nodes[u].isSelected(true);

        std::string w;

        double weight = M[initial][u];
        if (weight >= abs(numeric_limits<double>::infinity() / 2))
        {
            w = std::to_string(weight);
            nodes[u].weight.setString(w);
        }
        else
        {
            int iweight = static_cast<int>(weight);
            w = std::to_string(iweight);
            nodes[u].weight.setString(w);
        }

        std::string messagenode = "Node ";
        messagenode.push_back(u + 65);
        messagenode += " is selected. ";
        changeActivity(messagenode);

        std::cout << "Create an edge between nodes " << p[u].nodepath.back() << " and " << u << endl;

        dedges[total_dedges++].dCreate(nodes[u].getPosition(), nodes[p[u].nodepath.back()].getPosition());

        sf::sleep(delaytime);
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

                    if (weight >= abs(numeric_limits<double>::infinity()) / 2)
                    {
                        double weight = M[initial][j];
                        std::string w = std::to_string(weight);
                        nodes[j].weight.setString(w);
                    }
                    else
                    {
                        int weight = M[initial][j];
                        std::string w = std::to_string(weight);
                        nodes[j].weight.setString(w);
                    }

                    std::string messageweight = "Weight to reach node ";
                    messageweight.push_back(j + 65);
                    messageweight = messageweight + " is modified to " + w;
                    changeActivity(messageweight);

                    sf::sleep(delaytime);
                }
            }
        }

        //Change u to anything but initial and previous u
        available.remove(u);
        if (available.empty()) { break; }
        else { u = available.front(); }
    }

    for (int i = 0; i < total_nodes; i++)
    {
        if (i != initial)
        {
            nodes[i].isSelected(false);
        }
        for (int j = 0; j < total_nodes; j++)
        {
            M[i][j] = Matrix[i][j];
        }
        char c, j, k;
        j = i + 65;
        k = initial + 65;
        std::cout << "Initial Node: " << k << "\t Final Node: " << j << " \tWeight: " << p[i].weight << "\tPath: ";
        while (!p[i].nodepath.empty())
        {
            c = p[i].nodepath.front() + 65;
            std::cout << c << "  ";
            p[i].nodepath.pop_front();
        }
        std::cout << endl;
    }
    changeActivity("Dijkstra's Algorithm Completed!");
}

void Game::Update()
{
    m_window.Update(gui);
    activity->setPosition({(m_window.GetWindowSize().x - activity->getSize().x) / 2, m_window.GetWindowSize().y - 30});
}

void Game::CreateMatrix()
{
    for (int i = 0; i < total_edges; i++)
    {
        int a = edges[i].nodeA;
        int b = edges[i].nodeB;

        Matrix[a][b] = edges[i].weight;
        M[a][b] = edges[i].weight;

        Matrix[b][a] = edges[i].weight;
        M[b][a] = edges[i].weight;
    }
    for (int i=0; i<total_nodes; i++)
    {
        nodes[i].weight.setString(" ");
    }
    selected_nodes.clear();
    total_dedges = 0;

    std::cout << endl
              << "The Matrix form of the given graph is: " << endl;
    for (int i = 0; i < total_nodes; i++)
    {
        nodes[i].isSelected(false);
        for (int j = 0; j < total_nodes; j++)
        {
            std::cout << Matrix[i][j] << "\t";
        }
        std::cout << endl;
    }
}

void Game::Render()
{
    m_window.BeginDraw();
    for (int i = 0; i < total_edges; i++)
    {
        edges[i].Render(*m_window.GetRenderWindow());
    }
    for (int i = 0; i < total_dedges; i++)
    {
        dedges[i].Render(*m_window.GetRenderWindow());
    }
    for (int i = 0; i < total_nodes; i++)
    {
        nodes[i].Render(*m_window.GetRenderWindow());
    }
    gui.draw();
    m_window.EndDraw();
}