
#ifndef SGSFMLCONTROLLER_H
#define SGSFMLCONTROLLER_H

#include <SFML/Graphics.hpp>
#include "GuardianLogic.h"
#include "SGSFMLView.h"

class SGSFMLController {

    GuardianLogic & guardian;
    SGSFMLView & view;

    int tileSize;

    bool isGameStart;
    bool Triggered;
    bool is_gameOver;

    bool isWarning;
    bool isGameStart_LaserControl;

    bool rIsPressed;

    HidingStatus hidingStatus;

    /// Funkcja obsługuje zdarzenia (Closed, Resized, Key click)
    /// \param window - window
    /// \param event Odpowiedzialny za obsługę zdarzeń (Closed, Resize, Key click)
    void Events(sf::RenderWindow & window, sf::Event & event);

    /// Funkcja odpowiedzialna za rozpoczęcie zegarka n sekundowego po zanończeniu rozgrywki
    /// \param clock Ten zegareczek, który odlicza n sekund
    void clockStart(sf::Clock & clock);

    /// Funkcja rozpoczyna rozgrywkę (odlicza od 3)
    void gameStart(sf::RenderWindow & window, sf::Clock & clockStart);

    /// Funkcja obsługuje kliknięcie klawiatury
    /// \param event Odpowiedzialny za obsługę zdarzeń (Closed, Resized, Key click)
    void keyClick(const sf::Event &event);

    /// Funkcja, która obsługuje wszystkie funkcje wyświetlania w sfml
    void drawAll(sf::RenderWindow & window, bool & save, sf::Clock & clockGameStart);

    /// Funkcja do kontroli laseru i jego uprzedzeń (warnings)
    void laser(sf::Clock & laserClock);

public:
    /// Konstruktor
    SGSFMLController(GuardianLogic & guardian, SGSFMLView & view);

    /// Funckja obsługująca rozgrywkę
    /// \param window odpowiedzialny za obsługę planszy (obsługę wyświetlania)
    void play(sf::RenderWindow &window);
};



#endif //SGSFMLCONTROLLER_H
