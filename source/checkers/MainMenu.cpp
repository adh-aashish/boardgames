#include "../../include/checkers/MainMenu.hpp"
#include "../../mainMenu.hpp"
#include "../../include/checkers/multiplayer-offline.hpp"
#include "../../include/checkers/computer.hpp"

MainMenu::MainMenu(std::shared_ptr<Context> &context) : mContext(context)
{
    // creation of buttons starts
    std::string btnText[3] = {"Single Player", "Multiplayer", "Main Menu"};
    for (int i = 0; i < 3; i++)
    {
        mButton[i].create(this, sf::Vector2f(270, 70), btnText[i], sf::Vector2f(mContext->mWindow->getSize().x / 2.0f, 300 + i * 92));
    }
    // creation of buttons ends
}
MainMenu::~MainMenu() {}

void MainMenu::init()
{
    // mMenubg initialization starts
    mMenuBg.setTexture(mContext->mTextures->getAsset(AssetID::Textures::MenuBG));
    // increase the size of the background image to match the whole window.
    mMenuBg.setScale(float(mContext->mWindow->getSize().x) / mMenuBg.getTexture()->getSize().x, float(mContext->mWindow->getSize().y) / mMenuBg.getTexture()->getSize().y);
    // mMenubg initialization ends

    // Title initialization starts
    mTitle.setFont(mContext->mFonts->getAsset(AssetID::Fonts::Otomano));
    mTitle.setString("CHECKERS");
    mTitle.setCharacterSize(40);
    mTitle.setFillColor(sf::Color::White);
    mTitle.setOrigin(mTitle.getGlobalBounds().width / 2.f, mTitle.getGlobalBounds().height / 2.f);
    //FIXME: magic number place
    mTitle.setPosition(mContext->mWindow->getSize().x / 2.f, 80);
    // Title initialization ends
}

void MainMenu::processEvents()
{
    sf::Event event;
    while (mContext->mWindow->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            mContext->mWindow->close();
        }
        else if (event.type == sf::Event::MouseButtonPressed)
        {
            if (mContext->isThisButtonPressed(mButton[0].button.getGlobalBounds(), event.mouseButton))
            {
                std::cout << "Single player pressed" << std::endl;
                mContext->mStates->add(std::make_unique<Computer>(mContext), true);
            }
            else if (mContext->isThisButtonPressed(mButton[1].button.getGlobalBounds(), event.mouseButton))
            {
                std::cout << "Multiplayer pressed" << std::endl;
                mContext->mStates->add(std::make_unique<Multiplayer>(mContext), true);
            }
            else if (mContext->isThisButtonPressed(mButton[2].button.getGlobalBounds(), event.mouseButton))
            {
                std::cout << "MainMenu pressed" << std::endl;
                RunGame game;
                mContext->mWindow->close();
                game.runGame();
            }
        }
    }
}
void MainMenu::update(float dt)
{
}
void MainMenu::render()
{
    mContext->mWindow->clear();
    mContext->mWindow->draw(mMenuBg);
    mContext->mWindow->draw(mTitle);
    //FIXME: magic number place
    for (int i = 0; i < 3; i++)
    {
        mContext->mWindow->draw(mButton[i]);
    }
    mContext->mWindow->display();
}

std::shared_ptr<Context> &MainMenu::getContext()
{
    return mContext;
}