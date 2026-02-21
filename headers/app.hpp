#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <sstream>
#include "../headers/button.hpp"
#include "../headers/textzone.hpp"

class App {
public:
    sf::RenderWindow m_window;
    bool show_fps = false;

	void run();
    App(const sf::VideoMode& size, const std::string& title);

private:
    std::vector<sf::CircleShape> points;

    sf::VertexArray line;
    sf::Text text;
    sf::Font font;
    sf::Clock clock;
    sf::Time elapsed;
    unsigned int frameCount = 0;
    float fps = 0;

    sf::Vector2i last_mouse_pos;

    unsigned int brush_size = 10;
    sf::Color current_color = sf::Color::Red;
    sf::CircleShape brush;

    Button save_button;
    Button save_color;
    Button erase_button;
    TextZone r;
    TextZone g;
    TextZone b;

    void processEvents();
    void update();
    void render(bool ui);
    void processFPS();

};