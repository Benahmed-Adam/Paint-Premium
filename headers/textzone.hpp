#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class TextZone {
public:
	TextZone(sf::RenderWindow& window, const std::string& defaultText, sf::Vector2f size, unsigned int fontSize, sf::Vector2f pos, size_t maxLength);
	std::string get() const;
	void draw();
	void handleEvent(const sf::Event& event);
    std::string text;


private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text textShape;
    sf::RectangleShape box;
    std::string defaultText;
    sf::Vector2f pos;
    size_t maxLength;
    bool isWriting;
};