#pragma once
#include <memory>
#include "StateManager.hpp"
#include "assetsManager.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace AssetID
{
    namespace Textures
    {
        enum ID
        {
            MenuBG,
            Board,
            BackBtn,
            GameBG
        };
    }
    namespace Fonts
    {
        enum ID
        {
            Otomano,
            Regular
        };
    }
    namespace Sounds
    {
        enum ID
        {
            Bold,
            Regular
        };
    }
}

// This will be shared by any of the game screens/states like menu,game play,game
// over,etc.
struct Context
{
    std::unique_ptr<StateEngine::StateManager> mStates;
    std::unique_ptr<sf::RenderWindow> mWindow;
    std::unique_ptr<AssetsManager<sf::Texture, AssetID::Textures::ID>> mTextures;
    std::unique_ptr<AssetsManager<sf::Font, AssetID::Fonts::ID>> mFonts;
    std::unique_ptr<AssetsManager<sf::SoundBuffer, AssetID::Sounds::ID>> mSoundBuffers;

    sf::Vector2f mBoardSize;
    sf::Vector2f mSquareSize;
    sf::Vector2f mBoardTopCoords;

    float mSpaceUDBoard;
    float mSpaceLRBoard;

    // member functions
    Context();

    bool isThisButtonPressed(sf::Rect<float> button, sf::Event::MouseButtonEvent mouse);
};

struct Button : public sf::Drawable
{
    sf::RectangleShape button;
    sf::Text label;
    bool isLabel;
    // member functions
    Button()
    {
        isLabel = true;
    }
    template <class T> // T can be like MainMenu,GameOver ( i.e. state )
    void create(T *screen, const sf::Vector2f &size, const std::string &content, const sf::Vector2f &pos);
    void createFromTexture(const sf::Texture *texture, const sf::Vector2f &pos);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

// BUTTON IMPLEMENTATION
template <class T>
inline void Button::create(T *screen, const sf::Vector2f &size, const std::string &content, const sf::Vector2f &pos)
{
    button.setSize(size);
    button.setFillColor(sf::Color::Magenta);
    button.setOrigin(button.getSize().x / 2.0f, button.getSize().y / 2.0f);
    button.setPosition(pos);

    this->label.setFont(screen->getContext()->mFonts->getAsset(AssetID::Fonts::Otomano));
    this->label.setString(content);
    this->label.setCharacterSize(30);
    this->label.setFillColor(sf::Color::White);
    this->label.setOrigin(this->label.getGlobalBounds().width / 2.f, this->label.getGlobalBounds().height / 2.f);
    //FIXME: magic number place
    this->label.setPosition(pos);
}

inline void Button::createFromTexture(const sf::Texture *texture, const sf::Vector2f &pos)
{
    button.setTexture(texture);
    button.setPosition(pos);
    button.setSize(sf::Vector2f(60, 60));
    isLabel = false;
}

inline void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(button);
    if (isLabel)
    {
        target.draw(label);
    }
}
// BUTTON IMPLEMENTATION ENDS

inline bool Context::isThisButtonPressed(sf::Rect<float> button, sf::Event::MouseButtonEvent mouse)
{
    if (mouse.button == sf::Mouse::Left)
    {
        // mouse is pressed in some area of window
        if (mouse.x > button.left and mouse.x < button.left + button.width and mouse.y > button.top and mouse.y < button.top + button.height)
        {
            // mouse is pressed in given rect/button
            return true;
        }
    }
    return false;
}

inline Context::Context()
{

    mFonts = std::make_unique<AssetsManager<sf::Font, AssetID::Fonts::ID>>();
    mSoundBuffers = std::make_unique<AssetsManager<sf::SoundBuffer, AssetID::Sounds::ID>>();
    mTextures = std::make_unique<AssetsManager<sf::Texture, AssetID::Textures::ID>>();
    mStates = std::make_unique<StateEngine::StateManager>();
    mWindow = std::make_unique<sf::RenderWindow>();

    // creation of window
    sf::Vector2f screenSize(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
    // changed starts - for temp. size of screen
    screenSize = sf::Vector2f(930, 1046);
    // changed ends
    mWindow->create(sf::VideoMode(screenSize.x, screenSize.y), "Checkers", sf::Style::Titlebar | sf::Style::Close);

    mBoardSize = sf::Vector2f(mWindow->getSize().x * (30 / 31.0f), mWindow->getSize().y * (450 / 523.0f));

    mSquareSize = sf::Vector2f(mBoardSize.x / 8.0f, mBoardSize.y / 8.0f);

    // managing board render position
    mBoardTopCoords.x = (mWindow->getSize().x - mBoardSize.x) / 2.0f;
    mBoardTopCoords.y = (mWindow->getSize().y - mBoardSize.y) / 2.0f;

    // setting values for space around the board for window
    mSpaceUDBoard = mBoardTopCoords.y;
    mSpaceLRBoard = mBoardTopCoords.x;
}