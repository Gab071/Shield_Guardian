#include <iostream>
#include <cassert>
#include <SFML/Graphics.hpp>

#include "GuardianLogic.h"
#include "Menu.h"
#include "SGSFMLController.h"
#include "SGSFMLView.h"

void test1(const GuardianLogic & guardian);
void test2(GuardianLogic & guardian);
void test3(GuardianLogic & guardian);
void test4(GuardianLogic & guardian);

int main()
{
    sf::RenderWindow menuWindow (sf::VideoMode({800, 600}), "Shield Guardian");;
    Menu menu;
    menu.controlMenu(menuWindow);

    sf::RenderWindow window (sf::VideoMode({800, 600}), "Shield Guardian");
    GuardianLogic guardian (menu);
    SGSFMLView view (guardian);
    SGSFMLController controller (guardian, view);
    controller.play(window);

    // test4(guardian);
}

void test1(const GuardianLogic & guardian)
{
    assert(guardian.getShieldDirection() == RIGHT);
    assert(guardian.getGameState() == RUNNING);
    assert(guardian.getBoardHeight() == 13);
    assert(guardian.getBoardWidth() == 13);
}

void test2(GuardianLogic & guardian)
{
    assert(guardian.getShieldDirection() == RIGHT);
    assert(guardian.getGameState() == RUNNING);
    assert(guardian.getBoardHeight() == 13);
    assert(guardian.getBoardWidth() == 13);
    assert(guardian.getFieldInfo(6,6) == "Guardian");
    assert(guardian.getFieldInfo(0,0) == "LaserCannon");
    assert(guardian.getFieldInfo(12,12) == "LaserCannon");
    assert(guardian.getFieldInfo(6,7) == "Shield");
    assert(guardian.getFieldInfo(0,6) == "Cannon");
}

void test3(GuardianLogic & guardian)
{
    assert(guardian.getShieldDirection() == RIGHT);
    assert(guardian.getGameState() == RUNNING);
    assert(guardian.getBoardHeight() == 13);
    assert(guardian.getBoardWidth() == 13);
    assert(guardian.getFieldInfo(1,6) == "CannonBall");
    guardian.update();
    assert(guardian.getFieldInfo(2,6) == "CannonBall");
}

void test4(GuardianLogic & guardian)
{
    guardian.update();
    guardian.update();
    guardian.update();
    guardian.update();
    guardian.debug_display();
    guardian.update();
    guardian.debug_display();
    assert(guardian.getFieldInfo(1,6) != "CannonBall");
}


