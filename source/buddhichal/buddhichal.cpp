#include "../../include/buddhichal/Buddhichal.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <cmath>

int Buddhichal::cirCount = 0;
int Buddhichal::crossCount = 0;

Buddhichal::Buddhichal()
    : viewSize(sf::Vector2f(740 , 740)),
      window(sf::VideoMode(viewSize.x, viewSize.y), "Buddhichal"),
      w(100), h(100), gap(54),
      x((viewSize.x / 2.f) - (1.5*w +gap)), y((viewSize.y/2.f)-(1.5*h +gap)),dragCir(NULL),
      dragCross(NULL), gameOver(false), isDragging(false), isDraw(false),
      noMarkLeft(false),aiPlaying(true), ai('X'), opponent('O'), winner(' '), currPlayer('X'),
      gameBoard  {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}}
{
  if (!mCross.loadFromFile("../assets/buddhichal/textures/cross.jpg")) {
    // error message
    std::cout << "Couldnot load the file" << std::endl;
  }
  if (!bgTexture.loadFromFile("../assets/buddhichal/textures/buddhichal.jpg")) {
    // error message
  }
  if (!boardTexture.loadFromFile(
          "../assets/buddhichal/textures/boardTexture.jpg")) {
    // error message
  }
  if (!titleTexture.loadFromFile(
          "../assets/buddhichal/textures/heading.png")) {
    // error message
  }
  if (!restartTexture.loadFromFile(
          "../assets/buddhichal/textures/restart.jpg")) {
    // error message
  }
  bgSprite.setTexture(bgTexture);
  bgSprite.setPosition(0, 0);
  bgSprite.setScale(1.15, 1.15);
  titlesprite.setTexture(titleTexture,true);
  titlesprite.setPosition(viewSize.x/2.f - (titleTexture.getSize().x)/2.f, 10);
  restartSprite.setTexture(restartTexture);
  restartSprite.setPosition(viewSize.x/2.f - (restartTexture.getSize().x/2.f), viewSize.y - 100);
  text.setFont(font);
}

void ::Buddhichal::run() {
  while (window.isOpen()) {
    if (isDragging) {
      moveMark();
    }
    else{
      processEvents();
    }
    checkWin();
    render();
  }
}

void Buddhichal::drawBoard() {
  for(int i; i<3; i++)
  {
    drawBoardLine(x , y + i * (w + gap) + w/2.f, 0);
    drawBoardLine(x + i * (h + gap) + h/2.f, y, 90);
  }
  drawBoardLine(x,y,45);
  drawBoardLine(x+3*w+2*gap,y,135);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      drawRect(x + i * (h + gap), y + j * (w + gap));
    }
  }
}

void Buddhichal::processEvents() {
  if(aiPlaying && currPlayer == ai && !gameOver)
  {
    sf::Vector2i pos[2]{{-1,-1},{-1,-1}};
    findBestMove(pos , gameBoard);
    gameBoard[pos[0].y][pos[0].x] = ai;
    if(noMarkLeft)
    {
      gameBoard[pos[1].y][pos[1].x] = ' ';
      currPlayer = opponent;
    }
    else if(!gameOver)
    {
      setPlayer();
    }
  }
  else
  {
  sf::Event event;
  while (window.pollEvent(event)) // event takes 1 type at a time
  {
    switch (event.type) {
    case sf::Event::Closed:
      window.close();
      break;
    case sf::Event::MouseButtonPressed:
      if (event.mouseButton.button == sf::Mouse::Button::Left) {
        mousePosition = sf::Mouse::getPosition(window);
        if (mousePosition.x >= x && mousePosition.x <= (x + (3 * w) + 2*gap) &&
            !gameOver && mousePosition.y >= y &&
            mousePosition.y <= (y + (3 * h) + 2*gap)) {
          if (noMarkLeft) {
            selectMark();
            // move mark here
          } else if (!noMarkLeft) {
            // checkMoves();
            placeMark();
          }
        }
        if (cirCount > 2 && crossCount > 2) {
          noMarkLeft = true;
        }
        if (gameOver && restartSprite.getGlobalBounds().contains(
                            mousePosition.x, mousePosition.y)) {
          restartGame();
        }
      }
      break;
    default:
      break;
    }
  }
}
  }
 
void Buddhichal::placeMark() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (mousePosition.x >= x + i * (h + gap) &&
          mousePosition.x <= x + i * (h + gap) + w) {
        if (mousePosition.y >= y + j * (w + gap) &&
            mousePosition.y <= y + j * (w + gap) + h) {
          if (gameBoard[j][i] == ' ') {
            gameBoard[j][i] = currPlayer;
            setPlayer();
          } else {
            processEvents();
          }
        }
      }
    }
  }
}

void Buddhichal::moveMark() {
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    if (currPlayer == 'X' && dragCross != NULL) {
      dragCross->setPosition(sf::Mouse::getPosition(window).x,
                             sf::Mouse::getPosition(window).y);
      window.draw(*dragCross);
    }
    if (currPlayer == 'O' && dragCir != NULL) {
      dragCir->setPosition(sf::Mouse::getPosition(window).x,
                           sf::Mouse::getPosition(window).y);
      window.draw(*dragCir);
    }
  } else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    // sf::Vector2i newPosition = sf::Mouse::getPosition(window);
    // isMoveValid(newPosition);
    isDragging = false;
    updateGameBoard();
    dragCir = NULL;
    dragCross = NULL;
  }
}

void Buddhichal::selectMark() {
  if (currPlayer == 'X') {
    for (int k = 0; k < 3; k++) {
      if (cross[k].getGlobalBounds().contains(mousePosition.x,
                                              mousePosition.y)) {
        for (int i = 0; i < 3; i++) {
          for (int j = 0; j < 3; j++) {
            if (cross[k].getGlobalBounds().contains(x + i * (h + gap) + w / 2,
                                                    y + j * (h + gap) + h / 2)) {
              gameBoard[j][i] = 'D';
              isDragging = true;
              dragCross = &cross[k];
            }
          }
        }
        // dragMe(cross[i]);
      }
      if(isDragging)
      {
        break;
      }
    }
  } else if (currPlayer == 'O') {
    for (int k = 0; k < 3; k++) {
      if (cir[k].getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
        for (int i = 0; i < 3; i++) {
          for (int j = 0; j < 3; j++) {
            if (cir[k].getGlobalBounds().contains(x + i * (h + gap) + w / 2,
                                                  y + j * (h + gap) + h / 2)) {
              gameBoard[j][i] = 'E';
              isDragging = true;
              dragCir = &cir[k];
              window.draw(*dragCir);
            }
          }
        }
      }
      if(isDragging)
      {
        break;
      }
    }
  }
}
// get the current mouse postion
// get the sprite selected
// put the sprite where the mouse is kept

void Buddhichal::drawPlayer() {
  int cirTempCount = 0;
  int crossTempCount = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (gameBoard[j][i] == 'O'|| gameBoard[j][i] == 'E' ) {
        if(gameBoard[j][i] != 'E')
        {
          cir[cirTempCount].setPosition(x + i * (h + gap) + w / 2,
                                      y + j * (h + gap) + h / 2);
          window.draw(cir[cirTempCount]);
        }
        cirTempCount++;
      }
      if (gameBoard[j][i] == 'X' || gameBoard[j][i] == 'D' ) {
        if(gameBoard[j][i] != 'D')
        {
          cross[crossTempCount].setPosition(x + i * (h + gap) + w / 2,
                                            y + j * (h + gap) + h / 2);
          window.draw(cross[crossTempCount]);
        }
        crossTempCount++;
      }
    }
  }
  if (currPlayer == 'X' and dragCross != NULL and isDragging) {
    window.draw(*dragCross);
  }
  if (currPlayer == 'O' and dragCir != NULL and isDragging) {
    window.draw(*dragCir);
  }
}

void Buddhichal::setPlayer() {
  switch (currPlayer) {
  case 'X':
    cross[crossCount].setTexture(mCross);
    cross[crossCount].setScale(0.1, 0.1);
    cross[crossCount].setOrigin(mCross.getSize().x / 2.f,
                                mCross.getSize().y / 2.f);
    currPlayer = 'O';
    crossCount++;
    break;
  case 'O': {
    cir[cirCount].setRadius(38);
    cir[cirCount].setOrigin(cir[0].getRadius(), cir[0].getRadius());
    cir[cirCount].setFillColor(sf::Color::Transparent);
    cir[cirCount].setOutlineThickness(6.f);
    cir[cirCount].setOutlineColor(sf::Color::Red);
    currPlayer = 'X';
    cirCount++;
    break;
  }
  default:
    break;
  }
}

void Buddhichal::updateGameBoard() {
  bool flag = false;
  if (currPlayer == 'X') {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (dragCross->getGlobalBounds().contains(x + i * (h + gap) + w / 2,
                                                  y + j * (h + gap) + h / 2)) {
              if(isValidMove(gameBoard, i,j))
              {
                gameBoard[j][i] = currPlayer;
                currPlayer = 'O';
                flag = true;
              }
                
              }
            }
          }
  }
  else if (currPlayer == 'O') {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (dragCir->getGlobalBounds().contains(x + i * (h + gap) + w / 2,
                                                y + j * (h + gap) + h / 2)) {
          if(isValidMove(gameBoard,i,j))
          {
            gameBoard[j][i] = currPlayer;
            currPlayer = 'X';
            flag = true;
          }
        }
      }
    }
  }
  if(!flag)
  {
    for(int i=0; i<3; i++)
    {
      for(int j=0; j<3; j++)
      {
        if (gameBoard[j][i] == 'D' || gameBoard[j][i] == 'E')
        {
          gameBoard[j][i] = currPlayer;
        }
      }
    }
  }
}

bool Buddhichal::isValidMove(char gameBoard[3][3], int newi, int newj)
{
  for(int i=0; i<3; i++)
  {
    for(int j=0; j<3;j++)
    {
      if((gameBoard[j][i]=='D' || gameBoard[j][i]=='E') && gameBoard[newj][newi]==' ')
      {
        if(newi == i+1 && newj == j)
        {
          gameBoard[j][i] = ' ';
          return true;
        }
        else if(newi == i && newj == j+1)
        {
          gameBoard[j][i] = ' ';
          return true;
        }
        else if(newi == i-1 && newj == j-1 || (newi == i+1) && (newj == j+1))
        {
          if((newi==1 && newj==1) || (i==1 && j==1))
          {
            gameBoard[j][i] = ' ';
            return true;
          }
        }
        else if(newi == i-1 && newj == j)
        {
          gameBoard[j][i] = ' ';
          return true;
        }
        else if(newi == i && newj == j-1)
        {
          gameBoard[j][i] = ' ';
          return true;
        }
        else if((newi == i+1 && newj == j-1)|| (newi == i-1 && newj == j+1))
        {
          if((newi==1 && newj==1) || (i==1 && j==1))
          {
            gameBoard[j][i] = ' ';
            return true;
          }
        }
      }
    }
  }
  return false;
}

void Buddhichal::drawLine(int x, int y, int angle) {
  line.setSize(sf::Vector2f(3 * w + 2*gap, 6));
  line.setPosition(x, y);
  line.setOrigin(0, line.getSize().y / 2);
  line.setFillColor(sf::Color::Blue);
  line.setRotation(angle);

  if (angle == 45 || angle == 135) {
    line.setSize(sf::Vector2f(std::sqrt(2)*(3*w+2*gap), 6));
  }
}

void Buddhichal::drawBoardLine(int x, int y, int angle) {
  boardLine.setSize(sf::Vector2f(3 * w + 2*gap, 12));
  boardLine.setPosition(x, y);
  boardLine.setOrigin(0, boardLine.getSize().y / 2);
  boardLine.setFillColor(sf::Color::Black);
  boardLine.setRotation(angle);

  if (angle == 45 || angle == 135) {
    boardLine.setSize(sf::Vector2f(std::sqrt(2)*(3*w+2*gap)-14, 12));
    if(angle == 45)
    {
      boardLine.setPosition(x+4, y+4);
    }
    else
    {
      boardLine.setPosition(x-4, y+4);
    }
  }
  window.draw(boardLine);
}

void Buddhichal::checkWin() {
  for (int i = 0; i < 3; i++) {
    if (gameBoard[i][0] == gameBoard[i][1] &&
        gameBoard[i][0] == gameBoard[i][2] && gameBoard[i][0] != ' ') {
      winner = gameBoard[i][0];
      gameOver = true;
      drawLine(x, y + i * (h + gap) + h / 2, 0);
      break;
    }
    if (gameBoard[0][i] == gameBoard[1][i] &&
        gameBoard[0][i] == gameBoard[2][i] && gameBoard[0][i] != ' ') {
      winner = gameBoard[0][i];
      gameOver = true;
      drawLine(x + i * (w + gap) + w / 2, y, 90);
      break;
    }
  }
  if (gameBoard[0][0] == gameBoard[1][1] &&
      gameBoard[0][0] == gameBoard[2][2] && gameBoard[0][0] != ' ') {
    winner = gameBoard[0][0];
    gameOver = true;
    drawLine(x, y, 45);
  }
  if (gameBoard[0][2] == gameBoard[1][1] &&
      gameBoard[0][2] == gameBoard[2][0] && gameBoard[0][2] != ' ') {
    winner = gameBoard[0][2];
    gameOver = true;
    drawLine(x + 3 * w + 2*gap, y, 135);
  }
}

void Buddhichal::restartGame() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      gameBoard[i][j] = ' ';
    }
  }
  titleTexture.loadFromFile("../assets/buddhichal/textures/heading.png");
  titlesprite.setTexture(titleTexture,true);
  gameOver = false;
  noMarkLeft = false;
  winner = ' ';
  cirCount = 0;
  crossCount = 0;
  currPlayer = 'X';
  line.setSize(sf::Vector2f(0, 0));
}

void Buddhichal::render() {
  window.clear();
  window.draw(bgSprite);
  window.draw(titlesprite);
  drawBoard();
  drawPlayer();
  if (gameOver) {
      if(winner == 'X')
      {
        titleTexture.loadFromFile("../assets/buddhichal/textures/xwon.jpg");
        titlesprite.setTexture(titleTexture,true);
        titlesprite.setPosition(viewSize.x / 2 - (titleTexture.getSize().x)/2.f, 10);
      }
      else if (winner == 'O') {
        titleTexture.loadFromFile("../assets/buddhichal/textures/owon.png");
        titlesprite.setTexture(titleTexture,true);
        titlesprite.setPosition(viewSize.x / 2 - (titleTexture.getSize().x)/2.f, 10);
      }
    window.draw(line);
    window.draw(restartSprite);
  }
  window.display();
}

void Buddhichal::drawRect(float x, float y) {
  sf::RectangleShape rect(sf::Vector2f(100, 100));
  rect.setPosition(x, y);
  rect.setTexture(&boardTexture);
  window.draw(rect);
}

int Buddhichal::evaluate(char b[3][3])
{
    if (crossCount==0 && cirCount==0)
    {
      if (b[1][1]== ai)
        return 5;
      else if (b[1][1]==opponent)
        return -5;
    }

    if (b[0][0]==b[1][1] && b[1][1]==b[2][2] && b[0][0]!=' ')
    {
        if (b[0][0]==ai)
            return 10;
        else if (b[0][0]==opponent)
            return -10;
    }
 
    if (b[0][2]==b[1][1] && b[1][1]==b[2][0] && b[0][2]!=' ')
    {
        if (b[0][2]==ai)
            return 10;
        else if (b[0][2]==opponent)
            return -10;
    }

    // Checking for Rows for X or O victory.
    for (int row = 0; row<3; row++)
    {
        if (b[row][0]==b[row][1] &&
            b[row][1]==b[row][2] && b[row][0]!= ' ')
        {
            if (b[row][0]== ai)
                return 10;
            else if (b[row][0]==opponent)
                return -10;
        }
    }
 
    // Checking for Columns for X or O victory.
    for (int col = 0; col<3; col++)
    {
        if (b[0][col]==b[1][col] &&
            b[1][col]==b[2][col] && b[0][col]!=' ')
        {
            if (b[0][col]==ai)
                return 10;
 
            else if (b[0][col]==opponent)
                return -10;
        }
    }
 
    // Checking for Diagonals for X or O victory.
 
    // Else if none of them have won then return 0
    return 0;
}
 
int Buddhichal::minimax(char b[3][3], int depth, bool maximizingPlayer, int count, int alpha, int beta)
{
  int score = evaluate(b);
  if (depth == 0)
    return score+depth;

  if(score == 10)
    return score+depth;

  if(score == -10)
    return score-depth;

  if (maximizingPlayer)
  {
    int best = -1000;
    for(int i=0; i<3; i++)
    {
      for (int j=0; j<3; j++)
      {
        if(!noMarkLeft)
        {
          if(b[j][i]==' ')
          {
            b[j][i] = ai;
            best = std::max(best,minimax(b,depth-1,false,count++, alpha, beta));
            b[j][i] = ' ';
            alpha = std::max(alpha, best);
 
            // Alpha Beta Pruning
            if (beta <= alpha)
                break;
          }
        }
        else if(noMarkLeft || count>2)
        {
          if(b[j][i] == ai)
          {
            b[j][i] = 'D';
            for(int m=0; m<3; m++)
            {
              for(int n=0; n<3; n++)
              {
                if(b[n][m] == ' ')
                {
                  if(isValidMove(b,m,n))
                  {
                    b[n][m] = ai;
                    b[j][i] = ' ';
                    best = std::max(best,minimax(b,depth-1,false,count++, alpha, beta));
                    b[j][i] = 'D';
                    b[n][m] = ' ';
                    alpha = std::max(alpha, best);
                    // Alpha Beta Pruning
                    if (beta <= alpha)
                        break;
                  }
                }
              }
            }
            b[j][i] = ai;
          }
        }
      }
    }
    return best;
  }
  else
  {
    int best = 1000;
    for(int i=0; i<3; i++)
    {
      for (int j=0; j<3; j++)
      {
        if(!noMarkLeft)
        {
          if(b[j][i]==' ')
          {
            b[j][i] = opponent;
            best = std::min(best , minimax(b,depth-1,true, count++, alpha, beta));
            b[j][i] = ' ';
            beta = std::min(beta, best);
 
            // Alpha Beta Pruning
            if (beta <= alpha)
                break;
          }
        }
        else if(noMarkLeft || count>2)
        {
          if(b[j][i] == opponent)
          {
            b[j][i] = 'E';
            for(int m=0; m<3; m++)
            {
              for(int n=0; n<3; n++)
              {
                if(b[n][m] == ' ')
                {
                  if(isValidMove(b,m,n))
                  {
                    b[n][m] = opponent;
                    b[j][i] = ' ';
                    best = std::min(best,minimax(b,depth-1,true,count++, alpha, beta));
                    b[j][i] = 'E';
                    b[n][m] = ' ';
                    beta = std::min(beta, best);
                    // Alpha Beta Pruning
                    if (beta <= alpha)
                        break;
                  }
                }
              }
            }
            b[j][i] = opponent;
          }
        }
      }
    }
    return best;
  }
}

void Buddhichal::findBestMove(sf::Vector2i pos[2], char b[3][3]) 
{
  int bestVal = -1000;
  for(int i=0; i<3; i++)
  {
    for (int j=0; j<3; j++)
    {
      if(!noMarkLeft)
      {
        if(b[j][i]==' ')
        {
          b[j][i] = ai;
          int moveVal = minimax(b,8,false,crossCount,-1000,1000);
          b[j][i] = ' ';
          if(moveVal > bestVal)
          {
            pos[0].x = i;
            pos[0].y = j;
            bestVal = moveVal;
            std::cout << moveVal<< std::endl;
          }
        }
      }
      else
      {
        if(b[j][i] == ai)
        {
          b[j][i] = 'D';
          for(int m=0; m<3; m++)
          {
            for(int n=0; n<3; n++)
            {
              if(b[n][m] == ' ')
              {
                if(isValidMove(b,m,n))
                {
                  b[n][m] = ai;
                  b[j][i] = ' ';
                  int moveVal = minimax(b,8,false,crossCount,-1000,1000);
                  b[j][i] = 'D';
                  b[n][m] = ' ';
                  if(moveVal > bestVal)
                  {
                    pos[0].x = m;
                    pos[0].y = n;
                    pos[1].x = i;
                    pos[1].y = j;
                    bestVal = moveVal;
                  }
                }
              }
            }
          }
          b[j][i] = ai;
        }
      }
    }
  }
}
