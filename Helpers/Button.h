#pragma once
#include <SFML/Graphics.hpp>

class Button {
public:
    Button(const sf::Vector2f& size, const sf::Vector2f& position);

    void setText(const std::string& text, unsigned int size = 24);

    void update(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;

    bool isClicked() const;

private:
    sf::RectangleShape shape;
    sf::Font font;
    sf::Text label;

    bool hovered = false;
    bool clicked = false;
};