#pragma once
#include<SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>

class Buddhichal 
{
  private:
    sf::Vector2f viewSize;
    sf::RenderWindow window;
    const float w,h;//width and height of small box of board
    const float gap;
    float x,y;//position of first rectangle of board (used for navigation)
    sf::CircleShape* dragCir;
    sf::Sprite* dragCross;
    bool gameOver;
    bool isDragging;
    bool isDraw;
    bool noMarkLeft;
    bool aiPlaying;
    bool choice;
    char winner;
    char currPlayer;
    char ai;
    char opponent;
    char gameBoard[3][3];
    sf::Font font;
    sf::Text text;
    sf::Text option[3];
    sf::RectangleShape board;
    sf::RectangleShape line;
    sf::RectangleShape boardLine;
    sf::Vector2i mousePosition;
    sf::CircleShape cir[3];
    sf::Texture mCross;
    sf::Texture boardTexture;
    sf::Texture restartTexture;
    sf::Texture backBtnTexture;
    sf::Sprite restartSprite;
    sf::Sprite backBtn;
    sf::Texture bgTexture;
    sf::Sprite cross[3];
    sf::Sprite bgSprite;
    static int cirCount;
    static int crossCount;
    class Menu;
    int minimax(char b[3][3], int depth, bool maximizingPlayer, int count, int alpha, int beta);
    int evaluate(char b[3][3]);
    bool isValidMove(char [3][3], int newi, int newj);
    void findBestMove(sf::Vector2i pos[] , char b[3][3]);
    void chooseOption();
    void menu();
    void drawBoard();
    void drawRect(float x,float y);
    void setPlayer();
    void drawPlayer();
    void drawLine(int,int,int);
    void drawLine(int,int,int,bool);
    void processEvents();
    void updateGameBoard();
    void placeMark();
    void selectMark();
    void moveMark();
    void checkWin();
    void restartGame();
    void render(bool);
    void render();

  public:
    Buddhichal();
    void run();
};
