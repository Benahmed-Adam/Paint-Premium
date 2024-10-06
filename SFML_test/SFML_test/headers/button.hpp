#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class Button {
public:
    Button(sf::RenderWindow& window, const std::string& text, sf::Vector2f size, unsigned int fontSize, sf::Vector2f position);
    void draw();
    void handleEvent(const sf::Event& event);
    bool isButtonClicked() const;
    void desactivateButton();

private:
    sf::RenderWindow& window;
    sf::RectangleShape buttonShape;
    sf::RectangleShape hoverButtonShape;
    sf::Font font;
    sf::Text buttonText;
    sf::Vector2f size;
    sf::Vector2f pos;
    bool isClicked;
};