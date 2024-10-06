#include "../headers/button.hpp"

Button::Button(sf::RenderWindow& window, const std::string& text, sf::Vector2f size, unsigned int fontSize, sf::Vector2f position)
    : window(window), size(size), pos(position), isClicked(false){

    if (!this->font.loadFromFile("ressources/arial.ttf"))
    {
        std::exit(-1);
    }


    this->buttonText.setFont(this->font);
    this->buttonText.setString(text);
    this->buttonText.setCharacterSize(fontSize);
    this->buttonText.setFillColor(sf::Color::Black);

    sf::FloatRect textRect = this->buttonText.getLocalBounds();
    this->buttonText.setOrigin(textRect.width / 2, textRect.height / 2);
    this->buttonText.setPosition(this->pos);

    this->buttonShape.setSize(this->size);
    this->buttonShape.setFillColor(sf::Color(170, 170, 170, 200));
    this->buttonShape.setPosition(this->pos.x - size.x / 2, this->pos.y - size.y / 2);

    this->hoverButtonShape.setSize(this->size);
    this->hoverButtonShape.setFillColor(sf::Color(100, 100, 100, 170));
    this->hoverButtonShape.setPosition(this->pos.x - size.x / 2, this->pos.y - size.y / 2);
}

void Button::draw() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(this->window);
    sf::FloatRect buttonBounds = this->buttonShape.getGlobalBounds();

    if (buttonBounds.contains(static_cast<sf::Vector2f>(mousePos))) {
        this->window.draw(this->hoverButtonShape);
    }
    else {
        this->window.draw(this->buttonShape);
    }

    this->window.draw(this->buttonText);
}

void Button::handleEvent(const sf::Event& event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(this->window);
    sf::FloatRect buttonBounds = this->buttonShape.getGlobalBounds();

    if (event.type == sf::Event::MouseButtonPressed) {
        if (buttonBounds.contains(static_cast<sf::Vector2f>(mousePos))) {
            this->isClicked = true;
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased) {
        this->isClicked = false;
    }
}

bool Button::isButtonClicked() const {
    return this->isClicked;
}

void Button::desactivateButton() {
    this->isClicked = false;
}