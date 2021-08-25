#include "../../include/buddhichal/Buddhichal.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>

int Buddhichal::cirCount = 0;
int Buddhichal::crossCount = 0;

Buddhichal::Buddhichal()
    : viewSize(sf::Vector2f(740 , 740)),
      window(sf::VideoMode(viewSize.x, viewSize.y), "Buddhichal"),
      w(100), h(100), gap(54),
      x((viewSize.x / 2.f) - (1.5*w +gap)), y((viewSize.y/2.f)-(1.5*h +gap)),dragCir(NULL),
      dragCross(NULL), gameOver(false), isDragging(false), isDraw(false),
      noMarkLeft(false),aiPlaying(true),choice(false), ai('X'), opponent('O'), winner(' '), currPlayer('X'),
      gameBoard  {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}}
{
  if (!mCross.loadFromFile("../assets/buddhichal/textures/cross.jpg")) {
    // error message
    std::cout << "Couldnot load the file" << std::endl;
  }
  if(!backBtnTexture.loadFromFile("../assets/checkers/images/backBtn.png"))
  {
    //error
  }
  else
  {
    backBtn.setTexture(backBtnTexture);
    backBtn.setScale(0.5,0.5);
    backBtn.setPosition(0,0);
  }
  if (!bgTexture.loadFromFile("../assets/buddhichal/textures/buddhichal.jpg")) {
    // error message
    std::cout << "Couldnot load the file" << std::endl;
  }
  else
  {
    bgSprite.setTexture(bgTexture);
    bgSprite.setPosition(0, 0);
    bgSprite.setScale(1.15, 1.15);
  }
  if (!boardTexture.loadFromFile(
          "../assets/buddhichal/textures/boardTexture.jpg")) {
    // error message
    std::cout << "Couldnot load the file" << std::endl;
  }
  if (!restartTexture.loadFromFile(
          "../assets/buddhichal/textures/restart.jpg")) {
    // error message
    std::cout << "Couldnot load the file" << std::endl;
  }
  else
  {
    restartSprite.setTexture(restartTexture);
    restartSprite.setPosition(viewSize.x/2.f - (restartTexture.getSize().x/2.f), viewSize.y - 100);
  }
  if (!font.loadFromFile("../assets/firstMenu/fonts/FontsFree-Net-GlaiveRegular.ttf"))
  {
    //error message
    std::cout << "Couldnot load the file" << std::endl;
  }
  else
  {
    text.setFont(font);
    text.setCharacterSize(85);
    text.setFillColor(sf::Color(166, 237, 225));
    text.setString("Buddhichal");
    text.setOrigin(text.getGlobalBounds().width / 2.0f, text.getGlobalBounds().height / 2.0f);
    text.setPosition(window.getSize().x / 2.0f - 32, window.getSize().y * 0.05);
    text.setLetterSpacing(2);
  }
}

void ::Buddhichal::run() {
  while (window.isOpen()) {
    if(choice)
    {
      if (isDragging) {
        moveMark();
      }
      else{
        processEvents();
      }
      checkWin();
      render();
    }
    else
    {
      menu();
    }
  }
}

 



