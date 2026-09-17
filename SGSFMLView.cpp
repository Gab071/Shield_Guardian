
#include "SGSFMLView.h"

#include <fstream>
#include <iostream>

void SGSFMLView::loadTextures()
{
    std::vector<std::pair<std::string, std::string>> texture =
    {
        {" ", "grass.png"}, {"LaserCannon", "laserOrb.png"},
        {"RedGuardian", "redGuardian.png"}, {"BlueGuardian", "blueGuardian.png"},
        {"Shield", "shield.png"}, {"CannonBall", "cannonBall.png"},
        {"Cannon1", "cannon1.png"}, {"Cannon2", "cannon2.png"},
        {"Cannon3", "cannon3.png"}, {"Cannon4", "cannon4.png"},
        {"Wall", "wall.png"}
    };

    for (auto& [key, filename] : texture)
    {
        sf::Texture tex;
        if (!tex.loadFromFile(filename))
        {
            std::cerr << "Failed to load " << filename << std::endl;
        }
        else
        {
            textures[key] = tex; // Jeżeli operacja w if się udaje to dla key zachowujemy obiekt tex
        }
    }
}

void SGSFMLView::setupSprites()
{
    sprites.clear(); // Funkcja do czyszczenia mapy

    for (auto& [key, tex] : textures)
    {
        sf::Sprite sprite;
        sprite.setTexture(tex);
        const float scaleX = static_cast<float>(tileSize) / tex.getSize().x;
        const float scaleY = static_cast<float>(tileSize) / tex.getSize().y;
        sprite.setScale(scaleX, scaleY);

        sprites[key] = sprite; // Asocjuje klucz z obiektem sprite
    }
}

void SGSFMLView::scaleBoard(sf::RenderWindow &window, float &offsetX, float &offsetY)
{
    sf::Vector2u winSize = window.getSize(); // Dostajemy rozmiar sfml screen (by def 800*600)
    // Potrzebne, gdy nasza plansza będzie za duża dla sfml screen

    float scaleX = static_cast<float>(winSize.x) / pixelWidth;
    float scaleY = static_cast<float>(winSize.y) / pixelHeight;
    scale = std::min(scaleX, scaleY); // Funkcja wybiera mniejszy wariant
    tileSize = static_cast<int>(tileSize * scale);
    setupSprites();

    // Robimy to po raz drugi, bo tileSize mógł się zmienić
    pixelWidth = boardWidth * tileSize;
    pixelHeight = boardHeight * tileSize;

    offsetX = (winSize.x - pixelWidth) / 2.0f;
    offsetY = ((winSize.y - pixelHeight) / 2.0f);
}

void SGSFMLView::drawGameIsOver(sf::RenderWindow &window, sf::Text &gameOver) const
{
    gameOver.setFont(font);
    gameOver.setString("Game over");
    gameOver.setCharacterSize(40);
    gameOver.setFillColor(sf::Color::Red);
    gameOver.setStyle(sf::Text::Bold);
    sf::FloatRect bounds = gameOver.getLocalBounds();
    const int winWidth = window.getSize().x / 2;
    const int winHeight = window.getSize().y / 20.f;
    const int tWidth = winWidth - bounds.width / 2.f;
    gameOver.setPosition(tWidth, winHeight);
}

void SGSFMLView::drawTopScores(sf::RenderWindow &window, sf::Text &score) const
{
    score.setFont(font);
    score.setString("Top Scores:");
    score.setCharacterSize(40);
    score.setFillColor(sf::Color::Green);
    score.setStyle(sf::Text::Bold);
    sf::FloatRect bounds = score.getLocalBounds();
    const int winWidth = window.getSize().x / 2;
    const int winHeight = window.getSize().y / 3.5f;
    const int tWidth = winWidth - bounds.width / 2.f;
    score.setPosition(tWidth, winHeight);
}

void SGSFMLView::drawYourScore(sf::RenderWindow &window, sf::Text &youScore) const
{
    youScore.setFont(font);
    youScore.setString("Yours Score: " + std::to_string(score));
    youScore.setCharacterSize(40);
    youScore.setFillColor(sf::Color::Green);
    youScore.setStyle(sf::Text::Bold);
    sf::FloatRect bounds = youScore.getLocalBounds();
    int winWidth = window.getSize().x / 2;
    int winHeight = window.getSize().y / 6.f;
    int tWidth = winWidth - bounds.width / 2.f;
    youScore.setPosition(tWidth, winHeight);
}

void SGSFMLView::drawScores(sf::RenderWindow &window, sf::Text &scores) const
{
    std::string displayScores;
    openScores(displayScores);

    scores.setFont(font);
    scores.setString(displayScores);
    scores.setCharacterSize(30);
    scores.setFillColor(sf::Color::Blue);
    scores.setStyle(sf::Text::Bold);
    const sf::FloatRect bounds = scores.getLocalBounds();
    scores.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    scores.setPosition(window.getSize().x / 2.f, window.getSize().y / 1.5f);

    window.draw(scores);
}

void SGSFMLView::openScores(std::string &displayScores) const
{
    std::ifstream file("scores.txt");
    std::vector<int> scoreList;
    int s;

    while (file >> s)
        scoreList.push_back(s);
    file.close();

    std::sort(scoreList.begin(), scoreList.end(), std::greater<int>());

    for (size_t i = 0; i < std::min<size_t>(10, scoreList.size()); ++i)
    {
        if(i<9)
            displayScores += " " + std::to_string(i + 1) + ": " + std::to_string(scoreList[i]) + "\n";
        else
            displayScores +=  std::to_string(i + 1) + ": " + std::to_string(scoreList[i]) + "\n";
    }
}

void SGSFMLView::drawLaserTiles(const sf::Color laserColor, sf::RenderWindow &window, const float offsetX, const float offsetY) const
{
    auto drawTile = [&](const int row, const int col)
    {
        if (row == 6 && col == 6)
            return;

        sf::RectangleShape rect(sf::Vector2f(tileSize, tileSize));
        rect.setFillColor(laserColor);
        rect.setPosition(col * tileSize + offsetX, row * tileSize + offsetY);
        window.draw(rect);
    };

    // From (0,0) to (12,12)
    for (int i = 1; i < std::min(boardWidth, boardHeight) - 1; ++i)
        drawTile(i, i);

    // From (12,12) to (0,0)
    for (int i = 1; i < std::min(boardWidth, boardHeight) - 1; ++i)
        drawTile(12 - i, 12 - i);

    // From (0,12) to (12,0)
    for (int i = 1; i < std::min(boardWidth, boardHeight) - 1; ++i)
        drawTile(i, 12 - i);

    // From (12,0) to (0,12)
    for (int i = 1; i < std::min(boardWidth, boardHeight) - 1; ++i)
        drawTile(12 - i, i);
}



SGSFMLView::SGSFMLView(GuardianLogic &guardian) : guardian(guardian)
{
    loadTextures();
    if (!font.loadFromFile("PressStart2P-Regular.ttf"))
    {
        std::cerr << "Could not load font\n";
    }
    setupSprites();
    laserStatus = guardian.getLaserStatus();
}

void SGSFMLView::draw(sf::RenderWindow &window, const bool isWarning)
{
    float offsetX = 0, offsetY = 0, x, y;

    scaleBoard(window, offsetX, offsetY);

    for (int row = 0; row < boardHeight; ++row)
    {
        for (int col = 0; col < boardWidth; ++col)
        {
            x = col * tileSize + offsetX;
            y = row * tileSize + offsetY;

            std::string info = guardian.getFieldInfo(row, col);
            if (sprites.count(info))
            {
                sf::Sprite sprite = sprites[info];
                sprite.setPosition(x, y);
                window.draw(sprite);
            }
        }
    }
    if(isWarning)
        drawLaserBeam(window, offsetX, offsetY);
}

void SGSFMLView::drawGameOver(sf::RenderWindow &window)
{
    score = guardian.getScore();
    if(guardian.getGameState() != RUNNING)
    {
        sf::Text gameOver;
        drawGameIsOver(window, gameOver);
        window.draw(gameOver);

        sf::Text score1;
        drawTopScores(window, score1);
        window.draw(score1);

        sf::Text yourScore;
        drawYourScore(window, yourScore);
        window.draw(yourScore);

        sf::Text scores;
        drawScores(window, scores);

        drawRetry(window);
    }
}

int SGSFMLView::getTileSize() const
{
    return tileSize;
}

void SGSFMLView::draw1(sf::RenderWindow &window) const
{
    sf::Vector2u windowSize = window.getSize();
    sf::Text text;
    text.setFont(font);
    text.setString("1");
    text.setCharacterSize(100);
    text.setFillColor(sf::Color::Blue);

    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
                   textRect.top + textRect.height / 2.0f);
    text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

    window.draw(text);
}

void SGSFMLView::draw2(sf::RenderWindow &window) const
{
    sf::Vector2u windowSize = window.getSize();
    sf::Text text;
    text.setFont(font);
    text.setString("2");
    text.setCharacterSize(100);
    text.setFillColor(sf::Color::Blue);

    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
                   textRect.top + textRect.height / 2.0f);
    text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

    window.draw(text);
}

void SGSFMLView::draw3(sf::RenderWindow &window) const
{
    const sf::Vector2u windowSize = window.getSize();
    sf::Text text;
    text.setFont(font);
    text.setString("3");
    text.setCharacterSize(100);
    text.setFillColor(sf::Color::Blue);

    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
                   textRect.top + textRect.height / 2.0f);
    text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

    window.draw(text);
}

void SGSFMLView::drawLaserBeam(sf::RenderWindow &window, const float offsetX, const float offsetY)
{
    laserStatus = guardian.getLaserStatus();

    sf::Color laserColor;

    if (laserStatus == RED)
        laserColor = sf::Color(255, 100, 100);
    else if (laserStatus == BLUE)
        laserColor = sf::Color(100, 100, 255);
    else
        laserColor = sf::Color::White;

    drawLaserTiles(laserColor, window, offsetX, offsetY);
}

void SGSFMLView::drawRetry(sf::RenderWindow & window) const
{
    sf::Text retryText;
    retryText.setFont(font);
    retryText.setString("Press R to Play Again");
    retryText.setCharacterSize(30);
    retryText.setFillColor(sf::Color::White);
    const sf::FloatRect retryBounds = retryText.getLocalBounds();
    retryText.setPosition(window.getSize().x / 2 - retryBounds.width / 2,
                          window.getSize().y / 1.1f);
    window.draw(retryText);
}
