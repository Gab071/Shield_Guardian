
#ifndef SHVIEW_H
#define SHVIEW_H

#include "GuardianLogic.h"

class SGSFMLView {

    GuardianLogic & guardian;
    sf::Font font;

    int tileSize = 256;
    float scale = 1.0f;

    int score = 0;

    // Tu po prostu kalkulujemy jaki rozmiar będzie posiadała nasza nowa plansza
    int boardWidth = guardian.getBoardWidth();
    int boardHeight = guardian.getBoardHeight();
    int pixelWidth = boardWidth * tileSize;
    int pixelHeight = boardHeight * tileSize + tileSize;

    std::map<std::string, sf::Texture> textures;
    std::map<std::string, sf::Sprite> sprites;

    HidingStatus laserStatus;

    /// Funkcja odpowiedzialna za załadowanie grafik (teraz jest w Konstruktorze)
    void loadTextures();

    /// Funkcja odpowiedzialna za stworzenie sprites (grafika dla sfml)
    void setupSprites();

    /// Funkcja, która przeskalowuje planszę, gdy okno zmienia rozmiar
    /// \param window Odpowiedzialny za obsługę planszy (obsługę wyświetlania)
    /// \param offsetX Wyśrodkowuje planszę po width
    /// \param offsetY Wyśrodkowuje planszę po length
    void scaleBoard(sf::RenderWindow& window, float &offsetX, float &offsetY);

    /// Funkcja, która jest odpowiedzialna za wyświetlenie tekstu GameOver
    /// \param window Odpowiedzialny za obsługę planszy (obsługę wyświetlania)
    /// \param gameOver Odpowiedzialny za ustawienie tekstu GameOver
    void drawGameIsOver(sf::RenderWindow& window, sf::Text & gameOver) const;

    /// Funkcja, która jest odpowiedzialna za wyświetlenie tekstu TopScores
    /// \param window Odpowiedzialny za obsługę planszy (obsługę wyświetlania)
    /// \param score Odpowiedzialny za ustawienie tekstu score
    void drawTopScores(sf::RenderWindow& window, sf::Text & score) const;

    /// Funkcja, która jest odpowiedzialna za wyświetlenie tekstu Your Score
    /// \param window Odpowiedzialny za obsługę planszy (obsługę wyświetlania)
    /// \param youScore Odpowiedzialny za ustawienie tekstu youScore
    void drawYourScore(sf::RenderWindow& window, sf::Text & youScore) const;

    /// Funkcje odpowiedzialne za wyświetlanie 10 najlepszych wyników
    void drawScores(sf::RenderWindow& window, sf::Text & scores) const;
    void openScores(std::string & displayScores) const;

    /// Funkcje do wyświetlania laseru
    void drawLaserTiles(sf::Color laserColor, sf::RenderWindow & window, float offsetX, float offsetY) const;
    void drawLaserBeam(sf::RenderWindow &window, float offsetX, float offsetY);

    /// Funkcja wyświetla tekst "Press R to play again"
    void drawRetry(sf::RenderWindow & window) const;
public:
    /// Konstruktor
    explicit  SGSFMLView(GuardianLogic & guardian);

    /// Funkcja wyświetla planszę
    void draw(sf::RenderWindow& window, bool isWarning);

    /// Funkcja jest odpowiedzialna za wyświetlanie komunikaty po zakończeniu gry
    void drawGameOver(sf::RenderWindow & window);

    /// Prosty getter
    int getTileSize() const;

    /// Funkcje do wyświetlenia cyfr dla odliczenia startu gry
    void draw1(sf::RenderWindow& window) const;
    void draw2(sf::RenderWindow& window) const;
    void draw3(sf::RenderWindow& window) const;
};

#endif //SHVIEW_H
