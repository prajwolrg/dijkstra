#include "Window.h"
#include "node.h"
#include "edge.h"
#include <SFML/Network.hpp>
#include <list>

class Game
{
public:
	Game();
	~Game();

	struct Path
	{
		std::list<int> nodepath;
		int weight;
		bool fixed;
	};

	void HandleMovements();
	void Update();
	void Render();

	void CreateMatrix();
	void Dijkstra(double A[10][10], int);

	Window *GetWindow();

private:
	Window m_window;

	int total_nodes;
	Node nodes[10];
	Edge edges[10];
	int total_edges;

	Path p[10];

	sf::Vector2f prev_position;
	std::list<Node> selected_nodes;

	double Matrix[10][10];
	sf::Time delaytime;
	sf::Clock clock;

	float dist(sf::Vector2f, sf::Vector2f);
};