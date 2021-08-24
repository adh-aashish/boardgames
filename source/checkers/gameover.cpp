#include "../../include/checkers/gameover.hpp"

GameOver::GameOver(std::shared_ptr<Context> &context, const std::string &winner) : mContext(context), mWhoWonTheGame(winner) {}

void GameOver::init()
{
    mBackToMenu.create(this, sf::Vector2f(400, 100), "Back to Main Menu", sf::Vector2f(mContext->mWindow->getSize().x / 2.0f, mContext->mWindow->getSize().y / 2.0f));

    // information on who won
    mWhoWonInfo.setFont(mContext->mFonts->getAsset(AssetID::Fonts::Otomano));
    std::string winner = (mWhoWonTheGame == "W") ? "White" : "Black";
    mWhoWonInfo.setString("Winner: " + winner + " !!!");
    mWhoWonInfo.setCharacterSize(45);
    mWhoWonInfo.setFillColor(sf::Color::White);
    mWhoWonInfo.setOrigin(mWhoWonInfo.getLocalBounds().width/2.0f,mWhoWonInfo.getLocalBounds().height/2.0f);
    mWhoWonInfo.setPosition(mContext->mWindow->getSize().x / 2.0f, 300);
}

void GameOver::processEvents()
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
            if (mContext->isThisButtonPressed(mBackToMenu.button.getGlobalBounds(), event.mouseButton))
            {
                cout << "Pressed back to menu" << endl;
                mContext->mStates->add(std::make_unique<MainMenu>(mContext), true);
            }
        }
    }
}
void GameOver::update(float dt) {}
void GameOver::render()
{
    mContext->mWindow->clear(sf::Color::Cyan);
    mContext->mWindow->draw(mBackToMenu);
    mContext->mWindow->draw(mWhoWonInfo);
    mContext->mWindow->display();
}

std::shared_ptr<Context> &GameOver::getContext()
{
    return mContext;
}
