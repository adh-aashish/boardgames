#include "include/chess/def.h"
#include "include/checkers/checkers.hpp"
#include <iostream>
#include "include/buddhichal/Buddhichal.hpp"
#include <string>

using namespace std;
class Menu
{
  sf::RectangleShape btns[3];
  std::string btnValues[3] = {"Buddhichal", "Checkers", "Chess"};
  sf::Text text[3];
  sf::Font font;
  sf::Font headingFont;
  sf::Text headingText;
  sf::Texture texture;
  sf::Sprite bgSprite;

public:
  Menu(sf::RenderWindow &window)
  {
    if (!font.loadFromFile("../assets/checkers/fonts/OtomanopeeOne-Regular.ttf"))
    {
      cout << "could not load the font in first menu" << endl;
    }
    if (!texture.loadFromFile("../assets/firstMenu/main-texture.jpg"))
    {
      cout << "could not load the font in first menu" << endl;
    }
    else
    {
      // bg manipulation
      bgSprite.setTexture(texture);
      bgSprite.setScale(window.getSize().x / float(texture.getSize().x), window.getSize().y / float(texture.getSize().y));
    }

    if (!headingFont.loadFromFile("../assets/firstMenu/fonts/FontsFree-Net-GlaiveRegular.ttf"))
    {
      cout << "error elianto loading" << endl;
    }
    else
    {
      headingText.setFont(headingFont);
      headingText.setCharacterSize(85);
      headingText.setFillColor(sf::Color(166, 237, 225));
      headingText.setString("Board-Games");
      headingText.setOrigin(headingText.getGlobalBounds().width / 2.0f, headingText.getGlobalBounds().height / 2.0f);
      headingText.setPosition(window.getSize().x / 2.0f - 32, window.getSize().y * 0.05);
      headingText.setLetterSpacing(2);
    }

    for (int i = 0; i < 3; i++)
    {
      btns[i].setSize(sf::Vector2f(200, 100));
      btns[i].setFillColor(sf::Color::Transparent);
      btns[i].setOrigin(btns[i].getGlobalBounds().width / 2.0f, btns[i].getGlobalBounds().height / 2.0f);
      btns[i].setPosition(window.getSize().x / 2.0f, 400 + i * 120);

      // text part
      text[i].setFont(font);
      text[i].setString(btnValues[i]);
      text[i].setCharacterSize(30);
      text[i].setFillColor(sf::Color::White);
      text[i].setOrigin(text[i].getGlobalBounds().width / 2.0f, text[i].getGlobalBounds().height / 2.0f);
      text[i].setPosition(window.getSize().x / 2.0f, 400 + i * 120);
    }
  }
  void handleButtonPress(sf::RenderWindow &window, sf::Event::MouseButtonEvent mouseBtn)
  {
    float posX = mouseBtn.x;
    float posY = mouseBtn.y;
    if (btns[0].getGlobalBounds().contains(posX, posY))
    {
      cout << "Clicked in Buddhichal" << endl;
      Buddhichal buddhichal;
      window.close();
      buddhichal.run();
    }
    else if (btns[1].getGlobalBounds().contains(posX, posY))
    {
      cout << "Clicked in Checkers" << endl;
      Checkers checkers;
      window.close();
      checkers.run();
    }
    else if (btns[2].getGlobalBounds().contains(posX, posY))
    {
      cout << "Clicked in Chess" << endl;
      window.close();
      chess();
    }
  }
  void handleButtonHover(sf::RenderWindow &window, sf::Event::MouseMoveEvent mousePos)
  {
    // float posX = sf::Mouse::getPosition().x;
    // float posY = sf::Mouse::getPosition().y;
    float posX = mousePos.x;
    float posY = mousePos.y;
    if (btns[0].getGlobalBounds().contains(posX, posY))
    {
      text[0].setFillColor(sf::Color::Cyan);
    }
    else if (btns[1].getGlobalBounds().contains(posX, posY))
    {
      text[1].setFillColor(sf::Color::Cyan);
    }
    else if (btns[2].getGlobalBounds().contains(posX, posY))
    {
      text[2].setFillColor(sf::Color::Cyan);
    }
    else
    {
      for (int i = 0; i < 3; i++)
      {
        text[i].setFillColor(sf::Color::White);
      }
    }
  }
  void render(sf::RenderWindow &window)
  {
    window.draw(bgSprite);

    for (int i = 0; i < 3; i++)
    {
      window.draw(btns[i]);
      window.draw(text[i]);
    }
    window.draw(headingText);
  }
};

int main()
{
  sf::RenderWindow window(sf::VideoMode(1080, 1080), "BoardGames");
  Menu menu(window);
  while (window.isOpen())
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
        if (event.mouseButton.button == sf::Mouse::Left)
        {
          menu.handleButtonPress(window, event.mouseButton);
        }
        break;
      case sf::Event::MouseMoved:
        menu.handleButtonHover(window, event.mouseMove);
        break;
      default:
        break;
      }
    }
    window.clear(sf::Color::Green);
    menu.render(window);
    window.display();
    // Chess chess;
    // Checkers checkers;
    // Buddhichal buddhichal;

    //chess.run();
    //checkers.run();
    //buddhichal.run();
  }
  return 0;
}
