#include "Window.h"
#include "node.h"
#include "edge.h"
#include <SFML/Network.hpp>
#include <list>
#include <TGUI/TGUI.hpp>

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

	void changeActivity(std::string);

	void HandleMovements();
	void Update();
	void Render();

	void CreateMatrix();
	void Dijkstra();
	void clearAll(bool);

	Window *GetWindow();

private:
	Window m_window;

	int total_nodes;
	Node nodes[20];
	Edge edges[30];
	Edge dedges[20];
	int total_edges;
	int total_dedges;

	int initial;

	Path p[20];

	sf::Vector2f prev_position;
	std::list<Node> selected_nodes;

	double Matrix[20][20];
	double M[20][20];
	sf::Time delaytime;
	sf::Clock mouseclock;
	sf::Clock keyclock;

	float dist(sf::Vector2f, sf::Vector2f);

	tgui::Gui gui;
	tgui::Theme theme;
	tgui::EditBox::Ptr weightBox;
	tgui::TextBox::Ptr promptBox;
	tgui::Label::Ptr activity;
	tgui::Label::Ptr promptInitialSelect;
	tgui::Button::Ptr clear;
	tgui::TextBox::Ptr clearinfo;

	sf::Thread dijkstra;
};