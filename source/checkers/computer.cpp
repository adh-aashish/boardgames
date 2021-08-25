#include "../../include/checkers/computer.hpp"
#include <cmath>
#include <iostream>
#include <chrono>
#include <thread>
using namespace std::chrono_literals;

Computer::Computer(std::shared_ptr<Context> &context) : mContext(context)
{
    mGC = std::make_shared<Engine::GamePlayContext>(context);
    // CHANGED
    mGC->mCurrClickedPiece = std::make_unique<Engine::CurrentClickedPiece>(mGC, mGC->getCurrPlayer());
    mGC->mEngine = std::make_unique<Engine::MainEngine>(mGC);
    mGC->mMandatory = std::make_unique<Engine::MandatoryMove>(mGC);
}

Computer::~Computer() {}

// sets up value of each square and then position of each square.
void Computer::init()
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

void Computer::handleAIMove()
{
    long int bestScore = 9999999;
    std::vector<std::vector<int>> bestMovePath;
    // findmandatory move was done after player(human) moved,so we can check here
    if (mGC->mMandatory->doesMoveExist())
    {
        //changed
        //TODO: should i refer i.e. auto& map or auto map
        auto map = (mGC->mMandatory->isMandatoryMove()) ? ((mGC->mMandatory->doesDoubleJumpExist()) ? mGC->mMandatory->getDoubleJump() : mGC->mMandatory->getSingleJump()) : mGC->mMandatory->getNormalMoves();

        if (mGC->mMandatory->isMandatoryMove())
        {
            // sleep for some time
            std::this_thread::sleep_for(1000ms);
        }
        std::cout << "map size: " << map.size() << std::endl;
        for (auto i = map.begin(); i != map.end(); i++)
        {
            // iterating over each such pieces which have mandatory movement
            for (auto j = i->second.begin(); j != i->second.end(); j++)
            {
                // iterating over each moves of a piece
                auto aMove = *j;
                for (int p = 0; p < aMove.mPath.size(); p++)
                {
                    // iterating over each path of a move
                    // make a temporary copy board on which we change data for minimax
                    BOARD tempBoard = mGC->mBoard;
                    auto initialPosVal = tempBoard[aMove.mPath[p][0][0]][aMove.mPath[p][0][1]].value;

                    for (int n = 0; n < aMove.mPath[p].size(); n++)
                    {
                        // iterating over each pos of a path
                        if (tempBoard[aMove.mPath[p][n][0]][aMove.mPath[p][n][1]].value != Engine::Square::EMPTY)
                        {
                            // eatable position
                            mGC->captureAPiece(tempBoard, aMove.mPath[p][n][0], aMove.mPath[p][n][1]);
                        }
                        else if (n == aMove.mPath[p].size() - 1)
                        {
                            // this is final pos
                            tempBoard[aMove.mPath[p][n][0]][aMove.mPath[p][n][1]].value = initialPosVal;

                            //check for promotion
                            mGC->mEngine->promoteToKingIfPossible(tempBoard, aMove.mPath[p][n][0], aMove.mPath[p][n][1]);
                        }
                    }
                    // apply minimax to newly created parent:
                    //TOCHECK: can we make reference var in minimax( also modify above changes in minimax function too )
                    mGC->mMandatory->findMandatoryMoves(tempBoard, true);
                    long int score = mGC->mEngine->minimax(tempBoard, 0, true);
                    if (score < bestScore)
                    {
                        bestScore = score;
                        bestMovePath = aMove.mPath[p];
                    }
                }
            }
        }

        // make the real move of the AI and then currPlayerChange,findmandatory
        // eating part starts
        for (int j = 0; j < bestMovePath.size(); j++)
        {
            if (j != 0 and mGC->mBoard[bestMovePath[j][0]][bestMovePath[j][1]].value != Engine::Square::EMPTY)
            {
                // eatable position
                //TODO: (done) add other else if for kings
                mGC->captureAPiece(bestMovePath[j][0], bestMovePath[j][1]);
            }
        }
        // eating part ends

        // move of ai piece starts
        Piece *piece = mGC->mEngine->findPieceAtThisPos(bestMovePath[0][0], bestMovePath[0][1]);
        piece->move(bestMovePath[bestMovePath.size() - 1][0], bestMovePath[bestMovePath.size() - 1][1], mGC->mBoard[bestMovePath[bestMovePath.size() - 1][0]][bestMovePath[bestMovePath.size() - 1][1]]);

        if (!mGC->mEngine->promoteToKingIfPossible(piece))
        {
            // we do this way as the currclickedpiece is not updated
            // after the move ( which has previous values like say of
            // mType, mPosI,mPosJ, mPiece doesn't exist in memory
            // after promoted. )
            mGC->mBoard[bestMovePath[bestMovePath.size() - 1][0]][bestMovePath[bestMovePath.size() - 1][1]].value = mGC->mBoard[bestMovePath[0][0]][bestMovePath[0][1]].value;
        }
        mGC->mBoard[bestMovePath[0][0]][bestMovePath[0][1]].value = Engine::Square::EMPTY;
        mGC->changePlayerTurn();
        // now find the mandatory moves before new move starts
        mGC->mMandatory->findMandatoryMoves();
        // move of ai piece ends
    }
    else
    {
        // that means no move exists for ai, this means game is over.
        // so, show the gameover screen
        mGC->manageGameOver();
    }
}

void Computer::processEvents()
{
    if (mGC->getCurrPlayer().getSign() == "B")
    {
        // ai's turn
        handleAIMove();
    }
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
                std::cout << "This: i= " << PosI << ",j= " << PosJ << std::endl;

                // i.e. a piece was clicked
                if (mGC->mBoard[PosI][PosJ].value != Engine::Square::EMPTY)
                {
                    // check passes if current player clicks its own piece
                    if ((mGC->mBoard[PosI][PosJ].value == Engine::Square::WHITE or mGC->mBoard[PosI][PosJ].value == Engine::Square::WHITEKING) and mGC->getCurrPlayer().getSign() == "W")
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
                else if (mGC->mCurrClickedPiece->isClicked() and (mGC->getCurrPlayer().getSign() == mGC->mCurrClickedPiece->mP.getSign()) and (mGC->getCurrPlayer().getSign() == "W"))
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
void Computer::update(float dt)
{
}
void Computer::render()
{
    mContext->mWindow->clear(sf::Color::Cyan);
    mGC->draw();
    mContext->mWindow->display();
    mGC->manageGameOver();
}

std::shared_ptr<Context> &Computer::getContext()
{
    return mContext;
}