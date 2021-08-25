#include "../../include/buddhichal/Buddhichal.hpp"
#include <cmath>

void Buddhichal::drawBoard() {
  for(int i; i<3; i++)
  {
    drawLine(x , y + i * (w + gap) + w/2.f, 0,true);
    drawLine(x + i * (h + gap) + h/2.f, y, 90,true);
  }
  drawLine(x,y,45,true);
  drawLine(x+3*w+2*gap,y,135,true);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      drawRect(x + i * (h + gap), y + j * (w + gap));
    }
  }
}


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

void Buddhichal::drawLine(int x, int y, int angle, bool val) {
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


void Buddhichal::render() {
  window.clear();
  window.draw(bgSprite);
  /* window.draw(titlesprite); */
  window.draw(text);
  drawBoard();
  drawPlayer();
  if (gameOver) {
      if(winner == 'X')
      {
        
        text.setString("X WON");
        text.setOrigin(text.getGlobalBounds().width / 2.0f, text.getGlobalBounds().height / 2.0f);
        text.setPosition(window.getSize().x / 2.0f, window.getSize().y * 0.05);
      }
      else if (winner == 'O') {
        text.setString("O WON");
        text.setOrigin(text.getGlobalBounds().width / 2.0f, text.getGlobalBounds().height / 2.0f);
        text.setPosition(window.getSize().x / 2.0f , window.getSize().y * 0.05);
      }
    window.draw(backBtn);
    window.draw(line);
    window.draw(restartSprite);
  }
  window.display();
}

void Buddhichal::render(bool val)
{
  window.clear();
  window.draw(bgSprite);
  window.draw(text);
  for(int i=0; i<3; i++)
  {
    window.draw(option[i]);
  }
  window.display();
}

void Buddhichal::drawRect(float x, float y) {
  sf::RectangleShape rect(sf::Vector2f(100, 100));
  rect.setPosition(x, y);
  rect.setTexture(&boardTexture);
  window.draw(rect);
}
