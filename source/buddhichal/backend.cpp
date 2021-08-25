#include "../../include/buddhichal/Buddhichal.hpp"
   
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
  text.setString("BUDDHICHAL");
  text.setOrigin(text.getGlobalBounds().width / 2.0f, text.getGlobalBounds().height / 2.0f);
  text.setPosition(window.getSize().x / 2.0f , window.getSize().y * 0.05);
  gameOver = false;
  noMarkLeft = false;
  winner = ' ';
  cirCount = 0;
  crossCount = 0;
  currPlayer = 'X';
  line.setSize(sf::Vector2f(0, 0));
}

