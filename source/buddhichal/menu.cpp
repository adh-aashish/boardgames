#include "../../include/buddhichal/Buddhichal.hpp"
#include "../../include/mainMenu.hpp"
#include <string>
#include <iostream>

void Buddhichal::menu()
{
  std::string btnValues[3] = {"Single Player", "MultiPlayer", "Main Menu"};
  for (int i = 0; i < 3; i++)
  {
    option[i].setFont(font);
    option[i].setString(btnValues[i]);
    option[i].setCharacterSize(40);
    option[i].setFillColor(sf::Color::White);
    option[i].setOrigin(option[i].getGlobalBounds().width / 2.0f, option[i].getGlobalBounds().height / 2.0f);
    option[i].setPosition(window.getSize().x / 2.0f, 400 + i * 120);
  }

  while (!choice)
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      switch (event.type)
      {
      case sf::Event::Closed:
        window.close();
        break;
      case sf::Event::MouseButtonPressed:
      {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
          mousePosition = sf::Mouse::getPosition(window);
          float posX = mousePosition.x;
          float posY = mousePosition.y;
          if (option[0].getGlobalBounds().contains(posX, posY))
          {
            aiPlaying = true;
            choice = true;
          }
          else if (option[1].getGlobalBounds().contains(posX, posY))
          {
            aiPlaying = false;
            choice = true;
          }
          else if (option[2].getGlobalBounds().contains(posX, posY))
          {
            choice = false;
            RunGame game;
            window.close();
            game.runGame();
          }
        }
        break;
      }
      case sf::Event::MouseMoved:
      {
        mousePosition = sf::Mouse::getPosition(window);
        float posX = mousePosition.x;
        float posY = mousePosition.y;
        if (option[0].getGlobalBounds().contains(posX, posY))
        {
          option[0].setFillColor(sf::Color::Cyan);
        }
        else if (option[1].getGlobalBounds().contains(posX, posY))
        {
          option[1].setFillColor(sf::Color::Cyan);
        }
        else if (option[2].getGlobalBounds().contains(posX, posY))
        {
          option[2].setFillColor(sf::Color::Cyan);
        }
        else
        {
          for (int i = 0; i < 3; i++)
          {
            option[i].setFillColor(sf::Color::White);
          }
        }
        break;
      }
      default:
        break;
      }
    }
    render(true);
  }
}
