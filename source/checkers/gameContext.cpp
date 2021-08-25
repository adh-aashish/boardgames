#include "../../include/checkers/gameContext.hpp"
#include <chrono>
#include <thread>
using namespace std::chrono_literals;

Engine::GamePlayContext::GamePlayContext(std::shared_ptr<Context> &context) : mContext(context), mWpieces(12), mBpieces(12), mWKpieces(0), mBKpieces(0), mRenderBoard(context), mBoard(8, std::vector<Engine::Square>(8, Engine::Square(context->mSquareSize))), mP1("W"), mP2("B")
{
    // TODO: may be manage choosing of white or black later
    // but for now just hardcode one to be white player, other
    // to be black player.
    // and p1 ("White") has first move
    mP1.setTurn(true);
    mBack.createFromTexture(&(mContext->mTextures->getAsset(AssetID::Textures::BackBtn)), sf::Vector2f(15, 2));

    // mGameBG initialization starts
    mGameBg.setTexture(mContext->mTextures->getAsset(AssetID::Textures::GameBG));
    // increase the size of the background image to match the whole window.
    mGameBg.setScale(float(mContext->mWindow->getSize().x) / mGameBg.getTexture()->getSize().x, float(mContext->mWindow->getSize().y) / mGameBg.getTexture()->getSize().y);

    // mGameBG initialization ends
    // the initialization part of mCurrClickedPiece is done in constructor of
    // multiplayer-offline.cpp as it needs the shared_ptr.
}

Player &Engine::GamePlayContext::getCurrPlayer()
{
    if (mP1.isMyTurn())
    {
        return mP1; // "white"
    }
    else
    {
        return mP2; // "black"
    }
}

void Engine::GamePlayContext::changePlayerTurn()
{
    if (getCurrPlayer().getSign() == "W")
    {
        mP1.setTurn(false);
        mP2.setTurn(true);
    }
    else
    {
        mP1.setTurn(true);
        mP2.setTurn(false);
    }
}

// draws current game's board,logic-board(with colors maybe),
// pieces
void Engine::GamePlayContext::draw()
{
    mContext->mWindow->draw(mGameBg);
    mContext->mWindow->draw(mBack);
    // draw the wooden board
    mContext->mWindow->draw(mRenderBoard);
    // draw the logic-board maybe with colors
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {

            mContext->mWindow->draw(mBoard[i][j].square);
        }
    }
    // rendering pieces
    for (int i = 0; i < mWpieces.size(); i++)
    {
        mContext->mWindow->draw(mWpieces[i].getShape());
    }
    for (int i = 0; i < mBpieces.size(); i++)
    {
        mContext->mWindow->draw(mBpieces[i].getShape());
    }
    for (int i = 0; i < mWKpieces.size(); i++)
    {
        mContext->mWindow->draw(mWKpieces[i].getShape());
    }
    for (int i = 0; i < mBKpieces.size(); i++)
    {
        mContext->mWindow->draw(mBKpieces[i].getShape());
    }
}

template <class T>
void eraseFromVector(std::vector<T> &pieces, int posI, int posJ)
{
    for (auto p = pieces.begin(); p != pieces.end(); p++)
    {
        if ((*p).getPosI() == posI and (*p).getPosJ() == posJ)
        {
            pieces.erase(p);
            break;
        }
    }
}

void Engine::GamePlayContext::captureAPiece(int posI, int posJ)
{
    //TODO: (done) add functionality of kings too in this function
    // TODO: do switch case of "mBoard[posI][posJ].value" rather
    // than findpieceatthispos which is very expensive in performance
    auto piece = mEngine->findPieceAtThisPos(posI, posJ);
    mBoard[posI][posJ].value = Engine::Square::EMPTY;
    if (piece->getType() == Type::White)
    {
        eraseFromVector(mWpieces, posI, posJ);
    }
    else if (piece->getType() == Type::Black)
    {
        eraseFromVector(mBpieces, posI, posJ);
    }
    else if (piece->getType() == Type::WhiteKing)
    {
        eraseFromVector(mWKpieces, posI, posJ);
    }
    else if (piece->getType() == Type::BlackKing)
    {
        eraseFromVector(mBKpieces, posI, posJ);
    }
    else
    {
        throw std::runtime_error("Invalid error - piece is of no valid type");
    }
}

// ai part overloaded function
void Engine::GamePlayContext::captureAPiece(BOARD &board, int posI, int posJ)
{
    board[posI][posJ].value = Engine::Square::EMPTY;
}

void Engine::GamePlayContext::captureAPiece(Piece *piece)
{
    //TODO: (done) add functionality of kings too in this function

    mBoard[piece->getPosI()][piece->getPosJ()].value = Engine::Square::EMPTY;

    if (piece->getType() == Type::White)
    {
        eraseFromVector(mWpieces, piece->getPosI(), piece->getPosJ());
    }
    else if (piece->getType() == Type::Black)
    {
        eraseFromVector(mBpieces, piece->getPosI(), piece->getPosJ());
    }
    else if (piece->getType() == Type::WhiteKing)
    {
        eraseFromVector(mWKpieces, piece->getPosI(), piece->getPosJ());
    }
    else if (piece->getType() == Type::BlackKing)
    {
        eraseFromVector(mBKpieces, piece->getPosI(), piece->getPosJ());
    }
    else
    {
        throw std::runtime_error("Invalid error - piece is of no valid type");
    }
}

// returns 1 for white winning, -1 for black winning,0 for no win
int Engine::GamePlayContext::manageGameOver(bool shouldStateChange)
{
    if (getCurrPlayer().getSign() == "W")
    {
        if ((mWpieces.size() == 0 and mWKpieces.size() == 0) or !mMandatory->doesMoveExist())
        {
            // game lost by White ( black won )
            if (shouldStateChange)
            {
                std::this_thread::sleep_for(2000ms);
                mContext->mStates->add(std::make_unique<GameOver>(mContext, "B"), true);
            }
            return -1;
        }
    }
    else
    {
        if ((mBpieces.size() == 0 and mBKpieces.size() == 0) or !mMandatory->doesMoveExist())
        {
            // game lost by Black ( white won )
            if (shouldStateChange)
            {
                std::this_thread::sleep_for(2000ms);
                mContext->mStates->add(std::make_unique<GameOver>(mContext, "W"), true);
            }
            return 1;
        }
    }
    return 0;
}

void Engine::GamePlayContext::handleBackBtnClick(sf::Event::MouseButtonEvent mouse)
{
    if (mContext->isThisButtonPressed(mBack.button.getLocalBounds(), mouse))
    {
        // clicked
        std::cout << "Clicked in back btn" << std::endl;
        mContext->mStates->add(std::make_unique<MainMenu>(mContext), true);
    }
}
