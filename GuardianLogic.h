
#ifndef GUARDIANLOGIC_H
#define GUARDIANLOGIC_H
#include <vector>
#include <string>
#include "Menu.h"

enum GameState {RUNNING, FINISHED};
enum CannonBallCourse {NORTH, SOUTH, EAST, WEST};
enum ShieldDirection {UP, DOWN, LEFT, RIGHT};
enum HidingStatus {RED, BLUE};

struct Coordinates {
    int x;
    int y;

    bool operator==(const Coordinates & shield) const
    {
        return x == shield.x && y == shield.y;
    }
};

class GuardianLogic {

    Menu & menu;

    Coordinates boardDimensions;
    Coordinates shield;
    Coordinates cannonBall;
    Coordinates shieldGuardian;
    std::vector<Coordinates> cannons;
    std::vector<Coordinates> laserCannons;

    ShieldDirection shieldDirection;
    GameState state;
    CannonBallCourse ballCourse;
    GameMode mode;
    HidingStatus hidingStatus;
    HidingStatus laserStatus;

    float timeIs;
    float timeSurvived;
    int score;
    int collision;

    /// Funkcja ustawia poziom rozgrywki \n
    /// EASY, NORMAL, HARD
    void setMode();

    /// Funkcja sprawdza, czy nie powstały błędy lub nie prawidłowe ustawienia konstruktora
    void checkForErrors() const;

    /// Funkcja do wyświetlenia planszy współpracuje z debug_display
    void displayBoard(int nrow, int ncol) const;

    /// Funkcja porusza pociskiem armaty
    void moveCannonBall();

    /// Funkcje do prawidłowego przesunięcia pocisku
    void moveNorth();
    void moveSouth();
    void moveEast();
    void moveWest();

    /// Funkcja zmienia pozycję pocisku
    void moveBy(int dx, int dy);

    /// Funkcja sprawdza, czy pocisk uderzył w tarczę
    void isShieldCollision();

    /// Funkcja sprawdza, czy pocisk potrącił Guardian'a
    void isGuardianCollision();

    /// Funkcja służy do ustawienia nowego pocisku na planszy i wylosowania jego pozycji
    void placeCannonBall();

    /// Funkcja sprawdza, czy dane pole nie jest poza planszą
    /// \param row - sprawdzany wiersz
    /// \param col - sprawdzana kolumna
    bool OutBoard(int row, int col) const;

    /// Funkcja zmieniająca kierunek lotu pocisku
    void changeCannonBallCourse(const Coordinates &position);

    /// Funkcje używane w funkcji getFieldInfo
    bool isGuardian(int row, int col) const;
    bool isShield(int row, int col) const;
    bool isCannonBall(int row, int col) const;
    bool isCannon(int index, int row, int col) const;
    bool isLaserCannon(int row, int col) const;
    bool isWall(int row, int col) const;

public:
    /// Konstruktor \n
    /// nie zmieniać danych inicjalizacyjnych
    explicit GuardianLogic(Menu & menu);

    /// Funkcja wyświetla planszę w formie tekstowej
    void debug_display() const;

    /// Proste gettery
    int getBoardWidth() const;
    int getBoardHeight() const;
    int getScore() const;
    float getTime() const;
    HidingStatus getGuardianHidingStatus() const;
    HidingStatus getLaserStatus() const;

    /// Funkcja zwraca stan rozgrywki \n
    /// RUNNING - rozgrywka się toczy \n
    /// FINISHED - rozgrywka dobiegła końca
    /// \return state
    GameState getGameState() const;

    /// Funkcja zwraca obecny poziom rozgrywki \n
    /// EASY - łatwy \n
    /// NORMAL - normalny \n
    /// HARD  - trudny \n
    /// CLOSE - zamyka okno rozgrywki po zamknięciu okna menu (obecnie ustawione na EASY) \n
    /// (jeżeli będzie czasu to zaimplementować) \n
    /// \return mode
    GameMode getMode() const;

    /// Funkcja zwraca kierunek, w którym tarcza jest skierowana
    /// \return shieldDirection
    ShieldDirection getShieldDirection() const;

    /// Funkcja zwraca informację o żądanym polu
    /// \return - klucz, który wykorzystany zostanie w klasie View dla mapy
    std::string getFieldInfo(int row, int col) const;

    /// Funkcja zmienia kierunek obronny tarczy w zależności od podanego nowego kierunku
    /// \param newDirection - podany kierunek
    void changeShieldDirection(const ShieldDirection &newDirection);

    /// Funkcja zmienia stan Guardian'a, \n
    /// Jeśli Guardian chował się to teraz nie będzie, \n
    /// Jeśli się nie chował to teraz będzie
    void ChangeHidingStatus();

    /// Funkcja zachowuje wyniki rozgrywki
    void saveScores() const;

    /// Funkcje do obsługi laseru
    void guessLaserStatus();
    void checkIfLaserHit();

    /// Funkcja do zresetowania gry
    void reset();

    /// Funkcja po n-sekundach zmienia stan planszy
    void update();
};



#endif //GUARDIANLOGIC_H
