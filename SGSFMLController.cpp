//
// Created by kamin on 3.07.2025.
//

#include "SGSFMLController.h"

void SGSFMLController::Events(sf::RenderWindow &window, sf::Event &event)
{
    if (event.type == sf::Event::Closed)
        window.close();
    if (event.type == sf::Event::Resized)
    {
        sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        window.setView(sf::View(visibleArea));
    }
    if (event.type == sf::Event::KeyPressed)
    {
        keyClick(event);
    }
}

void SGSFMLController::clockStart(sf::Clock &clock)
{
    if (guardian.getGameState() != RUNNING && !Triggered)
    {
        clock.restart();
        Triggered = true;
    }

    if (Triggered && clock.getElapsedTime().asSeconds() >= 2.f)
    {
        is_gameOver = true;
    }
}

void SGSFMLController::gameStart(sf::RenderWindow &window, sf::Clock &clockStart)
{
    if(clockStart.getElapsedTime().asSeconds() < 1.f)
        view.draw3(window);

    else if(clockStart.getElapsedTime().asSeconds() < 2.f)
        view.draw2(window);

    else if(clockStart.getElapsedTime().asSeconds() < 3.f)
        view.draw1(window);

    else
        isGameStart = false;
}

void SGSFMLController::keyClick(const sf::Event &event)
{
    if(event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
        guardian.changeShieldDirection(UP);

    if(event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
        guardian.changeShieldDirection(DOWN);

    if(event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left)
        guardian.changeShieldDirection(LEFT);

    if(event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right)
        guardian.changeShieldDirection(RIGHT);

    if(event.key.code == sf::Keyboard::H)
    {
        guardian.ChangeHidingStatus();
        hidingStatus = guardian.getGuardianHidingStatus();
    }
    if (event.key.code == sf::Keyboard::R && is_gameOver)
    {
        guardian.reset();
        is_gameOver = false;
        isGameStart = true;
        Triggered = false;
        isWarning = false;
        isGameStart_LaserControl = true;
        rIsPressed = true;
    }
}

void SGSFMLController::drawAll(sf::RenderWindow &window, bool &save, sf::Clock &clockGameStart)
{
    if(isGameStart)
        gameStart(window, clockGameStart);


    if(!is_gameOver && !isGameStart)
        view.draw(window, isWarning);

    if(save && is_gameOver)
    {
        guardian.saveScores();
        save = false;
    }

    if(is_gameOver)
        view.drawGameOver(window);
}

void SGSFMLController::laser(sf::Clock &laserClock)
{
    if(rIsPressed)
    {
        laserClock.restart();
        rIsPressed = false;
    }

    const float elapsed = laserClock.getElapsedTime().asSeconds();
    float add_time = 0;
    if(isGameStart_LaserControl)
        add_time = 3;
    else
        add_time = 0;

    if (!isWarning && elapsed > 3.0f + add_time)
    {
        guardian.guessLaserStatus();
        isWarning = true;
    }

    if (elapsed > 4.0f + add_time)
        guardian.checkIfLaserHit();

    if (elapsed > 5.0f + add_time)
    {
        isWarning = false;
        laserClock.restart();
        isGameStart_LaserControl = false;
    }
}

SGSFMLController::SGSFMLController(GuardianLogic &guardian, SGSFMLView &view)
: guardian(guardian), view(view), isGameStart(true), Triggered(false), is_gameOver(false),
isWarning(false), isGameStart_LaserControl(true), rIsPressed(false)
{
    tileSize = view.getTileSize();
    hidingStatus = guardian.getGuardianHidingStatus();
}

void SGSFMLController::play(sf::RenderWindow &window)
{
    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(30);

    sf::Clock clockGameStart;
    sf::Clock clock;
    sf::Clock gameOverClock;
    sf::Clock laserClock;

    bool save = true;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            Events(window, event);
        }

        if(clock.getElapsedTime().asSeconds() >= guardian.getTime() && !isGameStart)
        {
            guardian.update();
            clock.restart();
        }
        clockStart(gameOverClock);
        laser(laserClock);

        window.clear();
        drawAll(window, save, clockGameStart);
        window.display();
    }
}
