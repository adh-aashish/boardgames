#include "../../include/buddhichal/Buddhichal.hpp"
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
  while (window.pollEvent(event)) 
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
        if (gameOver && backBtn.getGlobalBounds().contains(
                            mousePosition.x, mousePosition.y)) {
          choice = false;
          restartGame();
          menu();
        }
      }
      break;
    default:
      break;
    }
  }
}
}
