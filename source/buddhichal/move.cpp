#include "../../include/buddhichal/Buddhichal.hpp"

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
