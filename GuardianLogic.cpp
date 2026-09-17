
#include "GuardianLogic.h"

#include <fstream>
#include <iostream>
#include <iomanip>

// Tu w konstruktorze wszytko musi pozostać nie zmienione
GuardianLogic::GuardianLogic(Menu & menu)
    : menu(menu), boardDimensions({13, 13}),
    shield({6, 5}), cannonBall({6, 1}),
    shieldGuardian({6,6}), shieldDirection(UP),
    state(RUNNING), ballCourse(SOUTH), mode(CLOSE), hidingStatus(RED),
    timeSurvived(0), score(0), collision(0)
{
    srand(time(nullptr));

    cannons.push_back({6, 0});
    cannons.push_back({0, 6});
    cannons.push_back({6, 12});
    cannons.push_back({12, 6});

    laserCannons.push_back({0, 0});
    laserCannons.push_back({12, 0});
    laserCannons.push_back({0, 12});
    laserCannons.push_back({12, 12});

    checkForErrors();
    setMode();
}

void GuardianLogic::debug_display() const
{
    for (int ncol = 0; ncol < boardDimensions.y; ncol++)
    {
        if(ncol == 0)
            std::cout << std::setw(4) << ncol;

        else
            std::cout << std::setw(3) << ncol;
    }
    std::cout << std::endl;

    for(int nrow = 0; nrow < boardDimensions.y; ++nrow)
    {
        std::cout << nrow;
        for(int ncol = 0; ncol < boardDimensions.x; ++ncol)
        {
            displayBoard(nrow, ncol);
        }
        std::cout << std::endl;
    }
}

int GuardianLogic::getBoardWidth() const
{
    return boardDimensions.x;
}

int GuardianLogic::getBoardHeight() const
{
    return boardDimensions.y;
}

int GuardianLogic::getScore() const
{
    return score;
}

float GuardianLogic::getTime() const
{
    return timeIs;
}

HidingStatus GuardianLogic::getGuardianHidingStatus() const
{
    return hidingStatus;
}

HidingStatus GuardianLogic::getLaserStatus() const
{
    return laserStatus;
}

GameState GuardianLogic::getGameState() const
{
    return state;
}

GameMode GuardianLogic::getMode() const
{
    return mode;
}

ShieldDirection GuardianLogic::getShieldDirection() const
{
    return shieldDirection;
}

std::string GuardianLogic::getFieldInfo(const int row, const int col) const
{
    if(OutBoard(row, col))
        return "#";

    if(isGuardian(row, col) && hidingStatus == RED)
        return "RedGuardian";

    if(isGuardian(row, col) && hidingStatus == BLUE)
        return "BlueGuardian";

    if(isShield(row, col))
        return "Shield";

    if(isCannonBall(row, col))
        return "CannonBall";

    for (int i = 0; i < cannons.size(); ++i)
    {
        if (isCannon(i, row, col))
            return "Cannon" + std::to_string(i + 1);
    }

    if(isLaserCannon(row, col))
        return "LaserCannon";

    if(isWall(row, col))
        return "Wall";

    return" ";
}

void GuardianLogic::changeShieldDirection(const ShieldDirection &newDirection)
{
    if (shieldDirection == newDirection)
        return;

    shieldDirection = newDirection;

    switch (newDirection)
    {
        case UP:    shield = {6, 5}; break;
        case DOWN:  shield = {6, 7}; break;
        case LEFT:  shield = {5, 6}; break;
        case RIGHT: shield = {7, 6}; break;
        default: std::cout << "Wrong direction" << std::endl; break;
    }
}

void GuardianLogic::setMode()
{
    mode = menu.getGameMode();
    if(mode == EASY)
        timeIs = 0.25;

    else if(mode == NORMAL)
        timeIs = 0.2;

    else if(mode == HARD)
        timeIs = 0.15;

    else
        std::exit(0);
}

void GuardianLogic::checkForErrors() const
{
    if(shieldGuardian.x != 6 || shieldGuardian.y != 6)
        std::exit(0);

    if(boardDimensions.x != 13 || boardDimensions.y != 13)
        std::exit(0);

    if(cannonBall.x != 6 || cannonBall.y != 1)
        std::exit(0);
}

void GuardianLogic::displayBoard(const int nrow, const int ncol) const
{
    bool isCannon = false;
    bool isLaserCannon = false;
    for(const auto & cannon : cannons)
    {
        if(cannon.x == ncol && cannon.y == nrow)
            isCannon = true;
    }

    for(const auto & laserCannon : laserCannons)
    {
        if(laserCannon.x == ncol && laserCannon.y == nrow)
            isLaserCannon = true;
    }

    if(nrow == shield.y && ncol == shield.x)
        std::cout << std::setw(3) << 'S';

    else if(nrow == shieldGuardian.y && ncol == shieldGuardian.x)
        std::cout << std::setw(3) << 'G';

    else if(nrow == cannonBall.y && ncol == cannonBall.x)
        std::cout << std::setw(3) << 'o';

    else if(isCannon)
        std::cout << std::setw(3) << 'c';

    else if(isLaserCannon)
        std::cout << std::setw(3) << 'l';

    else
        std::cout << std::setw(3) << '.';
}

void GuardianLogic::moveCannonBall()
{
    if(state == FINISHED)
        return;

    if(ballCourse == NORTH)
        moveNorth();

    else if(ballCourse == SOUTH)
        moveSouth();

    else if(ballCourse == EAST)
        moveEast();

    else if(ballCourse == WEST)
        moveWest();
}

void GuardianLogic::moveNorth()
{
    moveBy(0, -1);
}

void GuardianLogic::moveSouth()
{
    moveBy(0, 1);
}

void GuardianLogic::moveEast()
{
    moveBy(1, 0);
}

void GuardianLogic::moveWest()
{
    moveBy(-1, 0);
}

void GuardianLogic::moveBy(const int dx, const int dy)
{
    cannonBall.x += dx;
    cannonBall.y += dy;
}

void GuardianLogic::isShieldCollision()
{
    if(cannonBall == shield)
        placeCannonBall();
}

void GuardianLogic::isGuardianCollision()
{
    if(shieldGuardian == cannonBall)
        state = FINISHED;
}

void GuardianLogic::placeCannonBall()
{
    const std::vector<Coordinates> positions = {
        {1, 6},
        {6, 1},
        {11, 6},
        {6, 11}
    };
    collision++;
    const int index = std::rand() % positions.size();
    cannonBall = positions[index];
    changeCannonBallCourse(positions[index]);
}

bool GuardianLogic::OutBoard(int row, int col) const
{
    if(row >= boardDimensions.y || col >= boardDimensions.x)
        return true;

    return false;
}

void GuardianLogic::changeCannonBallCourse(const Coordinates& position)
{
    if(position.x == 1 && position.y == 6)
        ballCourse = EAST;

    else if(position.x == 6 && position.y == 1)
        ballCourse = SOUTH;

    else if(position.x == 11 && position.y == 6)
        ballCourse = WEST;

    else if(position.x == 6 && position.y == 11)
        ballCourse = NORTH;
}


bool GuardianLogic::isGuardian(const int row, const int col) const
{
    if(OutBoard(row, col))
        return false;

    if(row == shieldGuardian.y && col == shieldGuardian.x)
        return true;

    return false;
}

bool GuardianLogic::isShield(int row, int col) const
{
    if(OutBoard(row, col))
        return false;

    if(row == shield.y && col == shield.x)
        return true;

    return false;
}

bool GuardianLogic::isCannonBall(int row, int col) const
{
    if(OutBoard(row, col))
        return false;

    if(row == cannonBall.y && col == cannonBall.x)
        return true;

    return false;
}

bool GuardianLogic::isCannon(int index, int row, int col) const
{
    if (OutBoard(row, col) || index < 0 || index >= cannons.size())
        return false;

    return row == cannons[index].y && col == cannons[index].x;
}

bool GuardianLogic::isLaserCannon(int row, int col) const
{
    if(OutBoard(row, col))
        return false;

    return std::any_of(laserCannons.begin(), laserCannons.end(), [row, col](const Coordinates pos)
        { return pos.y == row && pos.x == col; });
}

bool GuardianLogic::isWall(int row, int col) const
{
    if(OutBoard(row, col))
        return false;

    if(row == 0 || col == 0)
        return true;

    if(row == boardDimensions.y - 1 || col == boardDimensions.x - 1)
        return true;

    return false;
}

void GuardianLogic::ChangeHidingStatus()
{
    if(hidingStatus == RED)
        hidingStatus = BLUE;

    else if(hidingStatus == BLUE)
        hidingStatus = RED;
}

void GuardianLogic::saveScores() const
{
    if (state != RUNNING)
    {
        std::ofstream file("scores.txt", std::ios::app);
        if (file)
            file << score << std::endl;
        else
            std::cerr << "Unable to open file" << std::endl;

        file.close();
    }
}

void GuardianLogic::guessLaserStatus()
{
    const int random = rand() % 2;
    if (random == 0)
        laserStatus = RED;
    else
        laserStatus = BLUE;
}

void GuardianLogic::checkIfLaserHit()
{
    if(hidingStatus == RED && laserStatus == BLUE)
        state = FINISHED;

    else if(hidingStatus == BLUE && laserStatus == RED)
        state = FINISHED;
}

void GuardianLogic::reset()
{
    shield = {6, 5};
    cannonBall = {6, 1};
    shieldGuardian = {6, 6};
    shieldDirection = UP;
    state = RUNNING;
    ballCourse = SOUTH;
    hidingStatus = RED;
    timeSurvived = 0;
    score = 0;
    collision = 0;
    cannons = {{6, 0}, {0, 6}, {6, 12}, {12, 6}};
    laserCannons = {{0, 0}, {12, 0}, {0, 12}, {12, 12}};
}

void GuardianLogic::update()
{
    moveCannonBall();
    isShieldCollision();
    isGuardianCollision();

    timeSurvived += timeIs;
    if(state == RUNNING)
        score = static_cast<int>(timeSurvived*2) + collision * 4;
}
