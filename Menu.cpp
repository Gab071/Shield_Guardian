
#include "Menu.h"

#include <SFML/Graphics.hpp>
#include <iostream>

Menu::Menu() : choose_mode(CLOSE), exitMenu(false)
{
    if (!font.loadFromFile("PressStart2P-Regular.ttf"))
    {
        std::cerr << "Could not load font\n";
    }

    if (!texture.loadFromFile("red.png"))
        std::cerr << "Could not load redGuardian.png\n";
}

void Menu::controlMenu(sf::RenderWindow & menuWindow)
{
    menuWindow.setVerticalSyncEnabled(false);
    menuWindow.setFramerateLimit(60);

    while (menuWindow.isOpen() and !exitMenu)
    {
        sf::Event event;
        while (menuWindow.pollEvent(event))
        {
            Events(event, menuWindow);
        }
        menuWindow.clear(sf::Color{50,50,50});
        drawMenu(menuWindow);
        menuWindow.display();
    }
    menuWindow.close();
}

void Menu::Events(sf::Event & event, sf::RenderWindow & menuWindow)
{
    if (event.type == sf::Event::Closed)
        menuWindow.close();
    if (event.type == sf::Event::Resized)
    {
        const sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        menuWindow.setView(sf::View(visibleArea));
    }
    if (event.type == sf::Event::KeyPressed)
    {
        if(keyClick(event))
            exitMenu = true;
    }
}

bool Menu::keyClick(sf::Event &event)
{
    if(event.key.code == sf::Keyboard::Num1)
    {
        choose_mode = EASY;
        return true;
    }

    if(event.key.code == sf::Keyboard::Num2)
    {
        choose_mode = NORMAL;
        return true;
    }

    if(event.key.code == sf::Keyboard::Num3)
    {
        choose_mode = HARD;
        return true;
    }
    return false;
}

void Menu::drawDiff(sf::Text & diff, sf::RenderWindow & menuWindow) const
{
    const sf::Vector2u size = menuWindow.getSize();

    diff.setFont(font);
    diff.setString("Choose a difficulty:");
    diff.setCharacterSize(50);
    diff.setFillColor(sf::Color::Blue);
    diff.setStyle(sf::Text::Bold);
    sf::FloatRect diffBounds = diff.getLocalBounds();
    diff.setOrigin(diffBounds.left + diffBounds.width / 2.0f, 0);
    diff.setPosition(size.x / 2.0f, size.y / 30.0f);

    menuWindow.draw(diff);
}

void Menu::drawEasy(sf::Text & easy, sf::RenderWindow &menuWindow) const
{
    const sf::Vector2u size = menuWindow.getSize();

    easy.setFont(font);
    easy.setString("Easy: press 1");
    easy.setCharacterSize(50);
    easy.setFillColor(sf::Color::Green);
    easy.setStyle(sf::Text::Bold);
    sf::FloatRect easyBounds = easy.getLocalBounds();
    easy.setOrigin(easyBounds.left + easyBounds.width / 2.0f, easyBounds.top + easyBounds.height / 2.0f);
    easy.setPosition(size.x / 2.0f, size.y / 4.0f);

    menuWindow.draw(easy);
}

void Menu::drawNormal(sf::Text &normal, sf::RenderWindow &menuWindow) const
{
    const sf::Vector2u size = menuWindow.getSize();

    normal.setFont(font);
    normal.setString("Normal: press 2");
    normal.setCharacterSize(50);
    normal.setFillColor(sf::Color::Yellow);
    normal.setStyle(sf::Text::Bold);
    sf::FloatRect normalBounds = normal.getLocalBounds();
    normal.setOrigin(normalBounds.left + normalBounds.width / 2.0f, normalBounds.top + normalBounds.height / 2.0f);
    normal.setPosition(size.x / 2.0f, size.y / 2.5f);

    menuWindow.draw(normal);
}

void Menu::drawHard(sf::Text &hard, sf::RenderWindow &menuWindow) const
{
    const sf::Vector2u size = menuWindow.getSize();

    hard.setFont(font);
    hard.setString("Hard: press 3");
    hard.setCharacterSize(50);
    hard.setFillColor(sf::Color::Red);
    hard.setStyle(sf::Text::Bold);
    sf::FloatRect hardBounds = hard.getLocalBounds();
    hard.setOrigin(hardBounds.left + hardBounds.width / 2.0f, hardBounds.top + hardBounds.height / 2.0f);
    hard.setPosition(size.x / 2.0f, size.y / 1.75f);

    menuWindow.draw(hard);
}

void Menu::drawHide(sf::RenderWindow &menuWindow) const
{
    sf::Text hideText;
    hideText.setFont(font);
    hideText.setString("H - Hide");
    hideText.setCharacterSize(20); // smaller text size
    hideText.setFillColor(sf::Color::White);
    hideText.setStyle(sf::Text::Regular);

    const sf::FloatRect textBounds = hideText.getLocalBounds();
    hideText.setOrigin(textBounds.left + textBounds.width, textBounds.top + textBounds.height);

    const sf::Vector2u windowSize = menuWindow.getSize();
    hideText.setPosition(static_cast<float>(windowSize.x) - 10.0f, static_cast<float>(windowSize.y) - 10.0f);

    menuWindow.draw(hideText);
}

void Menu::drawMenu(sf::RenderWindow & menuWindow) const
{
    sf::Text diff;
    drawDiff(diff, menuWindow);

    sf::Text easy;
    drawEasy(easy, menuWindow);

    sf::Text normal;
    drawNormal(normal, menuWindow);

    sf::Text hard;
    drawHard(hard, menuWindow);

    sf::Sprite sprite;
    sprite.setTexture(texture);

    sf::Vector2u windowSize = menuWindow.getSize();
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    float y = windowSize.y - spriteBounds.height;
    sprite.setPosition(0, y);
    menuWindow.draw(sprite);

    drawHide(menuWindow);
}

GameMode Menu::getGameMode() const
{
    return choose_mode;
}