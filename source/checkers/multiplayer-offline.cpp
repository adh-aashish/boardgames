#include "../../include/checkers/multiplayer-offline.hpp"
#include <cmath>

Multiplayer::Multiplayer(std::shared_ptr<Context> &context) : mContext(context)
{
    mGC = std::make_shared<Engine::GamePlayContext>(context);
    // CHANGED
    mGC->mCurrClickedPiece = std::make_unique<Engine::CurrentClickedPiece>(mGC, mGC->getCurrPlayer());
    mGC->mEngine = std::make_unique<Engine::MainEngine>(mGC);
    mGC->mMandatory = std::make_unique<Engine::MandatoryMove>(mGC);
}

Multiplayer::~Multiplayer() {}

// sets up value of each square and then position of each square.
void Multiplayer::init()
{
    // setting position of each square of board
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            //FIXME: magic number place
            mGC->mBoard[i][j].square.setPosition(sf::Vector2f(mContext->mBoardTopCoords.x + j * mContext->mSquareSize.x, mContext->mBoardTopCoords.y + i * mContext->mSquareSize.y));
        }
    }

    // initial board setup by giving values(piece) to squares
    // and setting initial position of the pieces
    // white(1) is below and black (-1) is above, 0 = empty pos
    int x = 0, y = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (i >= 5)
            {
                if (i % 2 == 1)
                {
                    if (j % 2 == 0)
                    {
                        // fill with white piece
                        //FIXME: magic number place ( 125 and 1000 are such numbers anywhere )
                        mGC->mBoard[i][j].value = Engine::Square::WHITE;
                        mGC->mWpieces[x].create(sf::Vector2f(mGC->mBoard[i][j].square.getPosition().x + mContext->mSquareSize.x / 2.f, mGC->mBoard[i][j].square.getPosition().y + mContext->mSquareSize.y / 2.f), i, j, mContext->mSquareSize);
                        x++;
                    }
                }
                else
                {
                    if (j % 2 == 1)
                    {
                        // fill with white piece
                        mGC->mBoard[i][j].value = Engine::Square::WHITE;
                        mGC->mWpieces[x].create(sf::Vector2f(mGC->mBoard[i][j].square.getPosition().x + mContext->mSquareSize.x / 2.f, mGC->mBoard[i][j].square.getPosition().y + mContext->mSquareSize.y / 2.f), i, j, mContext->mSquareSize);
                        x++;
                    }
                }
            }
            else if (i <= 2)
            {
                if (i % 2 == 0)
                {
                    if (j % 2 == 1)
                    {
                        // fill with black piece
                        mGC->mBoard[i][j].value = Engine::Square::BLACK;
                        mGC->mBpieces[y].create(sf::Vector2f(mGC->mBoard[i][j].square.getPosition().x + mContext->mSquareSize.x / 2.f, mGC->mBoard[i][j].square.getPosition().y + mContext->mSquareSize.y / 2.f), i, j, mContext->mSquareSize);
                        y++;
                    }
                }
                else
                {
                    if (j % 2 == 0)
                    {
                        // fill with black
                        mGC->mBoard[i][j].value = Engine::Square::BLACK;
                        mGC->mBpieces[y].create(sf::Vector2f(mGC->mBoard[i][j].square.getPosition().x + mContext->mSquareSize.x / 2.f, mGC->mBoard[i][j].square.getPosition().y + mContext->mSquareSize.y / 2.f), i, j, mContext->mSquareSize);
                        y++;
                    }
                }
            }
        }
    }

    // changed
    mGC->mEngine->displayBoard();
}

void Multiplayer::processEvents()
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
            //FIXME: magic number place

            // check passes if clicked position is in board
            if ((event.mouseButton.x >= mContext->mBoardTopCoords.x and event.mouseButton.x <= mContext->mBoardTopCoords.x + mContext->mBoardSize.x) and (event.mouseButton.y >= mContext->mBoardTopCoords.y and event.mouseButton.y <= mContext->mBoardTopCoords.y + mContext->mBoardSize.y))
            {
                int PosI = floor((event.mouseButton.y - mContext->mSpaceUDBoard) / mContext->mSquareSize.y);
                int PosJ = floor((event.mouseButton.x - mContext->mSpaceLRBoard) / mContext->mSquareSize.x);
                cout << "This: i= " << PosI << ",j= " << PosJ << endl;

                // i.e. a piece was clicked
                if (mGC->mBoard[PosI][PosJ].value != Engine::Square::EMPTY)
                {
                    // check passes if current player clicks its own piece
                    if ((mGC->mBoard[PosI][PosJ].value == Engine::Square::WHITE or mGC->mBoard[PosI][PosJ].value == Engine::Square::WHITEKING) and mGC->getCurrPlayer().getSign() == "W" or (mGC->mBoard[PosI][PosJ].value == Engine::Square::BLACK or mGC->mBoard[PosI][PosJ].value == Engine::Square::BLACKKING) and mGC->getCurrPlayer().getSign() == "B")
                    {
                        // TODO: place mandatory here
                        if (mGC->mMandatory->isMandatoryMove())
                        {
                            mGC->mMandatory->manageMove(PosI, PosJ);
                        }
                        else
                        {
                            if (mGC->mCurrClickedPiece->isClicked())
                            {
                                // again clicked in same piece then no need to update again
                                if (mGC->mCurrClickedPiece->getPosI() == PosI and mGC->mCurrClickedPiece->getPosJ() == PosJ)
                                {
                                }
                                else
                                {
                                    mGC->mCurrClickedPiece->deColorNextMoves();
                                    mGC->mCurrClickedPiece->setUpdatedValue(PosI, PosJ, mGC->getCurrPlayer());

                                    mGC->mCurrClickedPiece->colorNextMoves();
                                }
                            }
                            // first time click
                            else
                            {
                                mGC->mCurrClickedPiece->setUpdatedValue(PosI, PosJ, mGC->getCurrPlayer());
                                mGC->mCurrClickedPiece->colorNextMoves();
                            }
                        }

                        // Engine::MainEngine::colorThePossibleMoves(mBoard, legalMoves,);
                    }
                }
                // click is done in other part of board ( not piece )
                // check for turn of say white and white piece is clicked
                else if (mGC->mCurrClickedPiece->isClicked() and mGC->getCurrPlayer().getSign() == mGC->mCurrClickedPiece->mP.getSign())
                {
                    // check for the move ( i.e. click in finalpos )
                    // posI,posJ are clicked pos which are checked if
                    // are final pos of the currclickedpiece.
                    mGC->mCurrClickedPiece->checkForFinalPosClick(PosI, PosJ);
                }
            }
        
            mGC->handleBackBtnClick(event.mouseButton);
        }
    }
}

// update the position of the pieces , kill pieces
void Multiplayer::update(float dt)
{
}
void Multiplayer::render()
{
    mContext->mWindow->clear(sf::Color::Cyan);
    
    mGC->draw();
    mContext->mWindow->display();
    mGC->manageGameOver();
}

std::shared_ptr<Context>& Multiplayer::getContext(){
    return mContext;
}