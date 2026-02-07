#include "Button.h"

Button::Button(const sf::Vector2f& size, const sf::Vector2f& position) : font("Assets/Fonts/pixantiqua.ttf"), label(font) {
    shape.setSize(size);
    label.setFont(font);
    shape.setPosition(position);
    shape.setFillColor(sf::Color(100, 100, 100));
}

void Button::setText(const std::string& text, unsigned int size) {
    label.setString(text);
    label.setCharacterSize(size);
    label.setFillColor(sf::Color::White);

    sf::FloatRect bounds = label.getLocalBounds();
    label.setOrigin(bounds.position + bounds.size / 2.f);
    label.setPosition(
        shape.getPosition() + shape.getSize() / 2.f
    );
}

void Button::update(const sf::RenderWindow& window) {
    clicked = false;

    auto mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
    hovered = shape.getGlobalBounds().contains(mousePos);

    shape.setFillColor(hovered ? sf::Color(150, 150, 150) : sf::Color(100, 100, 100));

    if (hovered && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        shape.setFillColor(sf::Color(200, 200, 200));
        clicked = true;
    }
}

bool Button::isClicked() const {
    return clicked;
}

void Button::draw(sf::RenderWindow& window) const {
    window.draw(shape);
    window.draw(label);
}
