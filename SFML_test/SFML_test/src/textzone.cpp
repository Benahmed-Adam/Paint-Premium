#include "../headers/textzone.hpp"

TextZone::TextZone(sf::RenderWindow& window, const std::string& defaultText, sf::Vector2f size, unsigned int fontSize, sf::Vector2f pos, size_t maxLength)
                    : window(window), defaultText(defaultText), text(""), maxLength(maxLength), pos(pos), isWriting(false){

    if (!this->font.loadFromFile("ressources/arial.ttf"))
    {
        std::exit(-1);
    }

    this->textShape.setFont(this->font);
    this->textShape.setCharacterSize(fontSize);
    this->textShape.setFillColor(sf::Color::White);
    this->textShape.setString(this->defaultText);
    this->textShape.setPosition(this->pos);

    this->box.setSize(size);
    this->box.setOrigin(size.x / 2, size.y / 2);
    this->box.setPosition(pos);
    this->box.setFillColor(sf::Color::Transparent);
    this->box.setOutlineThickness(2);
    this->box.setOutlineColor(sf::Color(175, 175, 175));
}

std::string TextZone::get() const {
    return this->text;
}

void TextZone::draw() {
    if (this->isWriting) {
        this->box.setOutlineColor(sf::Color::Green);
    }
    else {
        this->box.setOutlineColor(sf::Color(175, 175, 175));
    }

    this->window.draw(box);

    if (this->text.empty()) {
        this->textShape.setString(this->defaultText);
    }
    else {
        this->textShape.setString(this->text);
    }

    sf::FloatRect textRect = textShape.getLocalBounds();
    this->textShape.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    this->textShape.setPosition(box.getPosition());

    this->window.draw(this->textShape);
}

void TextZone::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(this->window);
        if (this->box.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            this->isWriting = true;
        }
        else {
            this->isWriting = false;
        }
    }

    if (event.type == sf::Event::TextEntered && this->isWriting) {
        if (event.text.unicode >= '0' && event.text.unicode <= '9') {
            if (this->text.size() < this->maxLength) {
                this->text += static_cast<char>(event.text.unicode);
            }
        }
        else if (event.text.unicode == '\b') {
            if (!this->text.empty()) {
                this->text.pop_back();
            }
        }
    }
}
