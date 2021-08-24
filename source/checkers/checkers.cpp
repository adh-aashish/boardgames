#include "../../include/checkers/checkers.hpp"
#include "../../include/checkers/MainMenu.hpp"

Checkers::Checkers() : mContext(std::make_shared<Context>())
{
    loadAssets();
    // const sf::Vector2f screenSize(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
    // mContext->mWindow->create(sf::VideoMode(screenSize.x, screenSize.y), "Checkers", sf::Style::Titlebar | sf::Style::Close);
    mContext->mStates->add(std::make_unique<MainMenu>(mContext));
}

Checkers::~Checkers() {}

void Checkers::run()
{
    sf::Clock clock;

    while (mContext->mWindow->isOpen())
    {
        sf::Time elapsedTime = clock.restart();
        mContext->mStates->processStackChange();
        mContext->mStates->top()->processEvents();
        mContext->mStates->top()->update(elapsedTime.asSeconds());
        mContext->mStates->top()->render();
    }
}

// loads almost all the assets which are required in the game.
// this makes sure that all loading happens in start of game rather than lazy
// loading, which may lag the game in game play
void Checkers::loadAssets()
{
    // loading of textures
    mContext->mTextures->load(AssetID::Textures::MenuBG, "../assets/checkers/images/checkers-menu-bg.jpg");

    mContext->mTextures->load(AssetID::Textures::Board, "../assets/checkers/images/checkers-board.png");
    
    mContext->mTextures->load(AssetID::Textures::BackBtn, "../assets/checkers/images/backBtn.png");

    mContext->mTextures->load(AssetID::Textures::GameBG, "../assets/checkers/images/game-bg.jpeg");

    // loading of fonts
    mContext->mFonts->load(AssetID::Fonts::Otomano, "../assets/checkers/fonts/OtomanopeeOne-Regular.ttf");
}
