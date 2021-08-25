#include "../../include/checkers/gameContext.hpp"
#include <algorithm>

Engine::Square::Square(const sf::Vector2f &squareSize) : square(squareSize)
{
    square.setFillColor(sf::Color::Transparent);
}

Engine::MainEngine::MainEngine(std::shared_ptr<Engine::GamePlayContext> &gc) : mGC(gc) {}
Engine::MainEngine::~MainEngine() {}

// TEST: testing of white,black piece check is done, if error then debugger
// may help, or check for the logic of "same code but piece check" of kings
//THE DEFAULT ARGUMENT IS FOR THE MINIMAX PART
std::vector<Move> Engine::MainEngine::findLegalNextMoves(int posI, int posJ, const BOARD &board)
{
    const BOARD &b = (board.size() != 0) ? board : mGC->mBoard;
    std::cout << "board.size()=> " << board.size() << std::endl;

    //VVI: moves[0]= left move, moves[1]=right move
    std::vector<Move> moves(2);

    Engine::Square square = b[posI][posJ];

    Engine::Square::VALUE nP; // normal piece
    Engine::Square::VALUE kP; // king piece
    if (square.value == Engine::Square::WHITEKING)
    {
        nP = Engine::Square::BLACK;
        kP = Engine::Square::BLACKKING;
    }
    else if (square.value == Engine::Square::BLACKKING)
    {
        nP = Engine::Square::WHITE;
        kP = Engine::Square::WHITEKING;
    }

    // clicked piece if is white piece
    if (square.value == Engine::Square::WHITE)
    {
        // TODO: whites are initially at the bottom for now i.e. mPosValue=1
        // for left move initially
        if (posI != 0 and posJ != 0)
        {
            if (b[posI - 1][posJ - 1].value == Engine::Square::EMPTY)
            {
                // normal move
                moves[0].mFinalPos = {{posI - 1, posJ - 1}};
                moves[0].mPath = {{{posI, posJ}, {posI - 1, posJ - 1}}};
                moves[0].mType = Move::Normal;
                moves[0].mCapturedPos = {};
            }
            else if (b[posI - 1][posJ - 1].value == Engine::Square::BLACK or
                     b[posI - 1][posJ - 1].value == Engine::Square::BLACKKING)
            {
                if (posI - 1 != 0 and posJ - 1 != 0)
                {
                    if (b[posI - 2][posJ - 2].value == Engine::Square::EMPTY)
                    {
                        // single jump
                        moves[0].mFinalPos = {{posI - 2, posJ - 2}};
                        moves[0].mPath = {{{posI, posJ}, {posI - 1, posJ - 1}, {posI - 2, posJ - 2}}};
                        moves[0].mType = Move::SingleJump;
                        moves[0].mCapturedPos = {{{posI - 1, posJ - 1}}};

                        // will single jump result in double jump?
                        // empty scope for storing some data reqd for
                        // double jump
                        {
                            bool isVectorCleared = false;
                            if (posI - 2 != 0 and posJ - 2 != 0)
                            {
                                // checking for double jump in left part after
                                // single jump of left initially
                                if ((b[posI - 3][posJ - 3].value == Engine::Square::BLACK or b[posI - 3][posJ - 3].value == Engine::Square::BLACKKING) and (posI - 3 != 0 and posJ - 3 != 0) and b[posI - 4][posJ - 4].value == Engine::Square::EMPTY)
                                {
                                    // double move succeeds
                                    isVectorCleared = true;
                                    moves[0].mFinalPos.clear();
                                    moves[0].mFinalPos.push_back({posI - 4, posJ - 4});
                                    moves[0].mPath.clear();
                                    moves[0].mPath.push_back({{posI, posJ}, {posI - 1, posJ - 1}, {posI - 2, posJ - 2}, {posI - 3, posJ - 3}, {posI - 4, posJ - 4}});
                                    moves[0].mType = Move::DoubleJump;
                                    moves[0].mCapturedPos.clear();
                                    moves[0].mCapturedPos.push_back({{posI - 1, posJ - 1}, {posI - 3, posJ - 3}});
                                }
                            }
                            if (posI - 2 != 0 and posJ - 2 != 7)
                            {
                                // double jump in right part after single jump
                                // in left
                                if ((b[posI - 3][posJ - 2 + 1].value == Engine::Square::BLACK or b[posI - 3][posJ - 2 + 1].value == Engine::Square::BLACKKING) and (posI - 3 != 0 and posJ - 2 + 1 != 7) and b[posI - 4][posJ - 2 + 2].value == Engine::Square::EMPTY)
                                {
                                    // double move succeeds in right side
                                    // after single jump of left side initially
                                    if (!isVectorCleared)
                                    {
                                        moves[0].mFinalPos.clear();
                                        moves[0].mPath.clear();
                                        moves[0].mCapturedPos.clear();
                                    }
                                    moves[0].mFinalPos.push_back({posI - 4, posJ});
                                    moves[0].mPath.push_back({{posI, posJ}, {posI - 1, posJ - 1}, {posI - 2, posJ - 2}, {posI - 3, posJ - 2 + 1}, {posI - 4, posJ - 2 + 2}});
                                    moves[0].mType = Move::DoubleJump;
                                    moves[0].mCapturedPos.push_back({{posI - 1, posJ - 1}, {posI - 3, posJ - 1}});
                                }
                            }
                        }
                    }
                    else
                    {
                        // no possible move in left side
                        moves[0].makeItNoMove(posI, posJ);
                    }
                }
                else
                {
                    // no possible space in left side
                    moves[0].makeItNoMove(posI, posJ);
                }
            }
            // posi-1,posj-1 has white/whiteking ( i.e friend piece )
            else
            {
                // no possible move in left side
                moves[0].makeItNoMove(posI, posJ);
            }
        }
        else
        {
            // no possible move in left side
            moves[0].makeItNoMove(posI, posJ);
        }
        // for right move initially
        if (posI != 0 and posJ != 7)
        {
            if (b[posI - 1][posJ + 1].value == Engine::Square::EMPTY)
            {
                // normal move
                moves[1].mFinalPos = {{posI - 1, posJ + 1}};
                moves[1].mPath = {{{posI, posJ}, {posI - 1, posJ + 1}}};
                moves[1].mType = Move::Normal;
                moves[1].mCapturedPos = {};
            }
            else if (b[posI - 1][posJ + 1].value == Engine::Square::BLACK or
                     b[posI - 1][posJ + 1].value == Engine::Square::BLACKKING)
            {
                if (posI - 1 != 0 and posJ + 1 != 7)
                {
                    if (b[posI - 2][posJ + 2].value == Engine::Square::EMPTY)
                    {
                        // single jump
                        moves[1].mFinalPos = {{posI - 2, posJ + 2}};
                        moves[1].mPath = {{{posI, posJ}, {posI - 1, posJ + 1}, {posI - 2, posJ + 2}}};
                        moves[1].mType = Move::SingleJump;
                        moves[1].mCapturedPos = {{{posI - 1, posJ + 1}}};

                        // will single jump result in double jump?
                        {
                            bool isVectorCleared = false;
                            if (posI - 2 != 0 and posJ + 2 != 0)
                            {
                                // checking for double jump in left part after
                                // single jump of right initially
                                if ((b[posI - 3][posJ + 2 - 1].value == Engine::Square::BLACK or b[posI - 3][posJ + 2 - 1].value == Engine::Square::BLACKKING) and (posI - 3 != 0 and posJ + 2 - 1 != 0) and b[posI - 4][posJ + 2 - 2].value == Engine::Square::EMPTY)
                                {
                                    // double move succeeds
                                    isVectorCleared = true;
                                    moves[1].mFinalPos.clear();
                                    moves[1].mFinalPos.push_back({posI - 4, posJ});
                                    moves[1].mPath.clear();
                                    moves[1].mPath.push_back({{posI, posJ}, {posI - 1, posJ + 1}, {posI - 2, posJ + 2}, {posI - 3, posJ + 2 - 1}, {posI - 4, posJ}});
                                    moves[1].mType = Move::DoubleJump;
                                    moves[1].mCapturedPos.clear();
                                    moves[1].mCapturedPos.push_back({{posI - 1, posJ + 1}, {posI - 3, posJ + 1}});
                                }
                            }
                            if (posI - 2 != 0 and posJ + 2 != 7)
                            {
                                // double jump in right part after single jump
                                // of right part
                                if ((b[posI - 3][posJ + 3].value == Engine::Square::BLACK or b[posI - 3][posJ + 3].value == Engine::Square::BLACKKING) and (posI - 3 != 0 and posJ + 3 != 7) and b[posI - 4][posJ + 4].value == Engine::Square::EMPTY)
                                {
                                    // double move succeeds in right side
                                    // after single jump of right side initially
                                    if (!isVectorCleared)
                                    {
                                        moves[1].mFinalPos.clear();
                                        moves[1].mPath.clear();
                                        moves[1].mCapturedPos.clear();
                                    }
                                    moves[1].mFinalPos.push_back({posI - 4, posJ + 4});
                                    moves[1].mPath.push_back({{posI, posJ}, {posI - 1, posJ + 1}, {posI - 2, posJ + 2}, {posI - 3, posJ + 3}, {posI - 4, posJ + 4}});
                                    moves[1].mType = Move::DoubleJump;
                                    moves[1].mCapturedPos.push_back({{posI - 1, posJ + 1}, {posI - 3, posJ + 3}});
                                }
                            }
                        }
                    }
                    else
                    {
                        // no possible move in right side
                        moves[1].makeItNoMove(posI, posJ);
                    }
                }
                else
                {
                    // no possible move in left side
                    moves[1].makeItNoMove(posI, posJ);
                }
            }
            // posi-1,posj-1 has white ( i.e friend piece )
            else
            {
                // no possible move in left side
                moves[1].makeItNoMove(posI, posJ);
            }
        }
        else
        {
            // no possible move in right side
            moves[1].makeItNoMove(posI, posJ);
        }
    }
    // clicked piece if is black piece
    else if (square.value == Engine::Square::BLACK)
    {
        // for right move initially
        if (posI != 7 and posJ != 0)
        {
            // normal move
            if (b[posI + 1][posJ - 1].value == Engine::Square::EMPTY)
            {
                // normal move
                moves[0].mFinalPos = {{posI + 1, posJ - 1}};
                moves[0].mPath = {{{posI, posJ}, {posI + 1, posJ - 1}}};
                moves[0].mType = Move::Normal;
                moves[0].mCapturedPos = {};
            }
            else if (b[posI + 1][posJ - 1].value == Engine::Square::WHITE or b[posI + 1][posJ - 1].value == Engine::Square::WHITEKING)
            {
                if (posI + 1 != 7 and posJ - 1 != 0)
                {
                    if (b[posI + 2][posJ - 2].value == Engine::Square::EMPTY)
                    {
                        // single jump
                        moves[0].mFinalPos = {{posI + 2, posJ - 2}};
                        moves[0].mPath = {{{posI, posJ}, {posI + 1, posJ - 1}, {posI + 2, posJ - 2}}};
                        moves[0].mType = Move::SingleJump;
                        moves[0].mCapturedPos = {{{posI + 1, posJ - 1}}};

                        // will single jump result in double jump?
                        {
                            bool isVectorCleared = false;
                            if (posI + 2 != 7 and posJ - 2 != 0)
                            {
                                // checking for double jump in right part after
                                // single jump of right initially
                                if ((b[posI + 3][posJ - 3].value == Engine::Square::WHITE or b[posI + 3][posJ - 3].value == Engine::Square::WHITEKING) and (posI + 3 != 7 and posJ - 3 != 0) and b[posI + 4][posJ - 4].value == Engine::Square::EMPTY)
                                {
                                    // double move succeeds
                                    isVectorCleared = true;
                                    moves[0].mFinalPos.clear();
                                    moves[0].mFinalPos.push_back({posI + 4, posJ - 4});
                                    moves[0].mPath.clear();
                                    moves[0].mPath.push_back({{posI, posJ}, {posI + 1, posJ - 1}, {posI + 2, posJ - 2}, {posI + 3, posJ - 3}, {posI + 4, posJ - 4}});
                                    moves[0].mType = Move::DoubleJump;
                                    moves[0].mCapturedPos.clear();
                                    moves[0].mCapturedPos.push_back({{posI + 1, posJ - 1}, {posI + 3, posJ - 3}});
                                }
                            }
                            if (posI + 2 != 7 and posJ - 2 != 7)
                            {
                                // double jump in left part after single jump
                                // in right initially
                                if ((b[posI + 3][posJ - 2 + 1].value == Engine::Square::WHITE or b[posI + 3][posJ - 2 + 1].value == Engine::Square::WHITEKING) and (posI + 3 != 7 and posJ - 2 + 1 != 7) and b[posI + 4][posJ - 2 + 2].value == Engine::Square::EMPTY)
                                {
                                    // double move succeeds in right side
                                    // after single jump of left side initially
                                    if (!isVectorCleared)
                                    {
                                        moves[0].mFinalPos.clear();
                                        moves[0].mPath.clear();
                                        moves[0].mCapturedPos.clear();
                                    }
                                    moves[0].mFinalPos.push_back({posI + 4, posJ});
                                    moves[0].mPath.push_back({{posI, posJ}, {posI + 1, posJ - 1}, {posI + 2, posJ - 2}, {posI + 3, posJ - 2 + 1}, {posI + 4, posJ - 2 + 2}});
                                    moves[0].mType = Move::DoubleJump;
                                    moves[0].mCapturedPos.push_back({{posI + 1, posJ - 1}, {posI + 3, posJ - 2 + 1}});
                                }
                            }
                        }
                    }
                    else
                    {
                        // no possible move in left side
                        moves[0].makeItNoMove(posI, posJ);
                    }
                }
                else
                {
                    // no possible move in left side
                    moves[0].makeItNoMove(posI, posJ);
                }
            }
            // posi-1,posj-1 has white ( i.e friend piece )
            else
            {
                // no possible move in left side
                moves[0].makeItNoMove(posI, posJ);
            }
        }
        else
        {
            // no possible move in right side
            moves[0].makeItNoMove(posI, posJ);
        }
        // for left move initially
        if (posI != 7 and posJ != 7)
        {
            if (b[posI + 1][posJ + 1].value == Engine::Square::EMPTY)
            {
                // normal move
                moves[1].mFinalPos = {{posI + 1, posJ + 1}};
                moves[1].mPath = {{{posI, posJ}, {posI + 1, posJ + 1}}};
                moves[1].mType = Move::Normal;
                moves[1].mCapturedPos = {};
            }
            else if (b[posI + 1][posJ + 1].value == Engine::Square::WHITE or b[posI + 1][posJ + 1].value == Engine::Square::WHITEKING)
            {
                if (posI + 1 != 7 and posJ + 1 != 7)
                {
                    if (b[posI + 2][posJ + 2].value == Engine::Square::EMPTY)
                    {
                        // single jump
                        moves[1].mFinalPos = {{posI + 2, posJ + 2}};
                        moves[1].mPath = {{{posI, posJ}, {posI + 1, posJ + 1}, {posI + 2, posJ + 2}}};
                        moves[1].mType = Move::SingleJump;
                        moves[1].mCapturedPos = {{{posI + 1, posJ + 1}}};

                        // will single jump result in double jump?
                        {
                            bool isVectorCleared = false;
                            if (posI + 2 != 7 and posJ + 2 != 0)
                            {
                                // checking for double jump in right part after
                                // single jump of left initially
                                if ((b[posI + 3][posJ + 2 - 1].value == Engine::Square::WHITE or b[posI + 3][posJ + 2 - 1].value == Engine::Square::WHITEKING) and (posI + 3 != 7 and posJ + 2 - 1 != 0) and b[posI + 4][posJ + 2 - 2].value == Engine::Square::EMPTY)
                                {
                                    // double move succeeds
                                    isVectorCleared = true;
                                    moves[1].mFinalPos.clear();
                                    moves[1].mFinalPos.push_back({posI + 4, posJ});
                                    moves[1].mPath.clear();
                                    moves[1].mPath.push_back({{posI, posJ}, {posI + 1, posJ + 1}, {posI + 2, posJ + 2}, {posI + 3, posJ + 2 - 1}, {posI + 4, posJ}});
                                    moves[1].mType = Move::DoubleJump;
                                    moves[1].mCapturedPos.clear();
                                    moves[1].mCapturedPos.push_back({{posI + 1, posJ + 1}, {posI + 3, posJ + 1}});
                                }
                            }
                            if (posI + 2 != 7 and posJ + 2 != 7)
                            {
                                // checking for double jump in left part after
                                // single jump of left initially
                                if ((b[posI + 3][posJ + 3].value == Engine::Square::WHITE or b[posI + 3][posJ + 3].value == Engine::Square::WHITEKING) and (posI + 3 != 7 and posJ + 3 != 7) and b[posI + 4][posJ + 4].value == Engine::Square::EMPTY)
                                {
                                    if (!isVectorCleared)
                                    {
                                        moves[1].mFinalPos.clear();
                                        moves[1].mPath.clear();
                                        moves[1].mCapturedPos.clear();
                                    }
                                    moves[1].mFinalPos.push_back({posI + 4, posJ + 4});
                                    moves[1].mPath.push_back({{posI, posJ}, {posI + 1, posJ + 1}, {posI + 2, posJ + 2}, {posI + 3, posJ + 3}, {posI + 4, posJ + 4}});
                                    moves[1].mType = Move::DoubleJump;
                                    moves[1].mCapturedPos.push_back({{posI + 1, posJ + 1}, {posI + 3, posJ + 3}});
                                }
                            }
                        }
                    }
                    else
                    {
                        // no possible move in right side
                        moves[1].makeItNoMove(posI, posJ);
                    }
                }
                else
                {
                    // no possible move in left side
                    moves[1].makeItNoMove(posI, posJ);
                }
            }
            // posi-1,posj-1 has white ( i.e friend piece )
            else
            {
                // no possible move in left side
                moves[1].makeItNoMove(posI, posJ);
            }
        }
        else
        {
            // no possible move in left side
            moves[1].makeItNoMove(posI, posJ);
        }
    }
    // check if it is white king and black king cases.
    else if (square.value == Engine::Square::WHITEKING || square.value == Engine::Square::BLACKKING)
    {
        moves.resize(4);
        // white's part copied
        // for left move initially
        if (posI != 0 and posJ != 0)
        {
            if (b[posI - 1][posJ - 1].value == Engine::Square::EMPTY)
            {
                // normal move
                moves[0].mFinalPos = {{posI - 1, posJ - 1}};
                moves[0].mPath = {{{posI, posJ}, {posI - 1, posJ - 1}}};
                moves[0].mType = Move::Normal;
                moves[0].mCapturedPos = {};
            }
            else if (b[posI - 1][posJ - 1].value == nP or
                     b[posI - 1][posJ - 1].value == kP)
            {
                if (posI - 1 != 0 and posJ - 1 != 0)
                {
                    if (b[posI - 2][posJ - 2].value == Engine::Square::EMPTY)
                    {
                        // single jump
                        moves[0].mFinalPos = {{posI - 2, posJ - 2}};
                        moves[0].mPath = {{{posI, posJ}, {posI - 1, posJ - 1}, {posI - 2, posJ - 2}}};
                        moves[0].mType = Move::SingleJump;
                        moves[0].mCapturedPos = {{{posI - 1, posJ - 1}}};

                        // will single jump result in double jump?
                        // empty scope for storing some data reqd for
                        // double jump
                        {
                            bool isVectorCleared = false;
                            // for up double jump
                            if (posI - 2 != 0 and posJ - 2 != 0)
                            {
                                // checking for double jump in left part after
                                // single jump of left initially
                                if ((b[posI - 3][posJ - 3].value == nP or b[posI - 3][posJ - 3].value == kP) and (posI - 3 != 0 and posJ - 3 != 0) and b[posI - 4][posJ - 4].value == Engine::Square::EMPTY)
                                {
                                    // double move succeeds
                                    if (!isVectorCleared)
                                    {
                                        moves[0].mFinalPos.clear();
                                        moves[0].mPath.clear();
                                        moves[0].mCapturedPos.clear();
                                        isVectorCleared = true;
                                    }
                                    moves[0].mFinalPos.push_back({posI - 4, posJ - 4});
                                    moves[0].mPath.push_back({{posI, posJ}, {posI - 1, posJ - 1}, {posI - 2, posJ - 2}, {posI - 3, posJ - 3}, {posI - 4, posJ - 4}});
                                    moves[0].mType = Move::DoubleJump;
                                    moves[0].mCapturedPos.push_back({{posI - 1, posJ - 1}, {posI - 3, posJ - 3}});
                                }
                            }
                            if (posI - 2 != 0 and posJ - 2 != 7)
                            {
                                // double jump in right part after single jump
                                // in left
                                if ((b[posI - 3][posJ - 2 + 1].value == nP or b[posI - 3][posJ - 2 + 1].value == kP) and (posI - 3 != 0 and posJ - 2 + 1 != 7) and b[posI - 4][posJ - 2 + 2].value == Engine::Square::EMPTY)
                                {
                                    // double move succeeds in right side
                                    // after single jump of left side initially
                                    if (!isVectorCleared)
                                    {
                                        moves[0].mFinalPos.clear();
                                        moves[0].mPath.clear();
                                        moves[0].mCapturedPos.clear();
                                        isVectorCleared = true;
                                    }
                                    moves[0].mFinalPos.push_back({posI - 4, posJ});
                                    moves[0].mPath.push_back({{posI, posJ}, {posI - 1, posJ - 1}, {posI - 2, posJ - 2}, {posI - 3, posJ - 2 + 1}, {posI - 4, posJ - 2 + 2}});
                                    moves[0].mType = Move::DoubleJump;
                                    moves[0].mCapturedPos.push_back({{posI - 1, posJ - 1}, {posI - 3, posJ - 2 + 1}});
                                }
                            }
                            // for down double jump
                            // right down jump
                            if (posI - 2 != 7 and posJ - 2 != 0)
                            {
                                // checking for double jump in right part after
                                // single up jump of left initially
                                if ((b[posI - 2 + 1][posJ - 3].value == nP or b[posI - 1][posJ - 3].value == kP) and (posI - 1 != 7 and posJ - 3 != 0) and b[posI][posJ - 4].value == Engine::Square::EMPTY)
                                {
                                    // double move succeeds
                                    if (!isVectorCleared)
                                    {
                                        moves[0].mFinalPos.clear();
                                        moves[0].mPath.clear();
                                        moves[0].mCapturedPos.clear();
                                        isVectorCleared = true;
                                    }
                                    moves[0].mFinalPos.push_back({posI, posJ - 4});
                                    moves[0].mPath.push_back({{posI, posJ}, {posI - 1, posJ - 1}, {posI - 2, posJ - 2}, {posI - 1, posJ - 3}, {posI, posJ - 4}});
                                    moves[0].mType = Move::DoubleJump;
                                    moves[0].mCapturedPos.push_back({{posI - 1, posJ - 1}, {posI - 1, posJ - 3}});
                                }
                            }
                        }
                    }
                    else
                    {
                        // no possible move in left side
                        moves[0].makeItNoMove(posI, posJ);
                    }
                }
                else
                {
                    // no possible move in left side
                    moves[0].makeItNoMove(posI, posJ);
                }
            }
            // posi-1,posj-1 has white ( i.e friend piece )
            else
            {
                // no possible move in left side
                moves[0].makeItNoMove(posI, posJ);
            }
        }
        else
        {
            // no possible move in left side
            moves[0].makeItNoMove(posI, posJ);
        }
        // for right move initially
        if (posI != 0 and posJ != 7)
        {
            if (b[posI - 1][posJ + 1].value == Engine::Square::EMPTY)
            {
                // normal move
                moves[1].mFinalPos = {{posI - 1, posJ + 1}};
                moves[1].mPath = {{{posI, posJ}, {posI - 1, posJ + 1}}};
                moves[1].mType = Move::Normal;
                moves[1].mCapturedPos = {};
            }
            else if (b[posI - 1][posJ + 1].value == nP or
                     b[posI - 1][posJ + 1].value == kP)
            {
                if (posI - 1 != 0 and posJ + 1 != 7)
                {

                    if (b[posI - 2][posJ + 2].value == Engine::Square::EMPTY)
                    {
                        // single jump
                        moves[1].mFinalPos = {{posI - 2, posJ + 2}};
                        moves[1].mPath = {{{posI, posJ}, {posI - 1, posJ + 1}, {posI - 2, posJ + 2}}};
                        moves[1].mType = Move::SingleJump;
                        moves[1].mCapturedPos = {{{posI - 1, posJ + 1}}};

                        // will single jump result in double jump?
                        {
                            bool isVectorCleared = false;
                            if (posI - 2 != 0 and posJ + 2 != 0)
                            {
                                // checking for double jump in left part after
                                // single jump of right initially
                                if ((b[posI - 3][posJ + 2 - 1].value == nP or b[posI - 3][posJ + 2 - 1].value == kP) and (posI - 3 != 0 and posJ + 2 - 1 != 0) and b[posI - 4][posJ + 2 - 2].value == Engine::Square::EMPTY)
                                {
                                    // double move succeeds
                                    if (!isVectorCleared)
                                    {
                                        moves[1].mFinalPos.clear();
                                        moves[1].mPath.clear();
                                        moves[1].mCapturedPos.clear();
                                        isVectorCleared = true;
                                    }
                                    moves[1].mFinalPos.push_back({posI - 4, posJ});
                                    moves[1].mPath.push_back({{posI, posJ}, {posI - 1, posJ + 1}, {posI - 2, posJ + 2}, {posI - 3, posJ + 2 - 1}, {posI - 4, posJ}});
                                    moves[1].mType = Move::DoubleJump;
                                    moves[1].mCapturedPos.push_back({{posI - 1, posJ + 1}, {posI - 3, posJ + 1}});
                                }
                            }
                            if (posI - 2 != 0 and posJ + 2 != 7)
                            {
                                // double jump in right part after single jump
                                if ((b[posI - 3][posJ + 3].value == nP or b[posI - 3][posJ + 3].value == kP) and (posI - 3 != 0 and posJ + 3 != 7) and b[posI - 4][posJ + 4].value == Engine::Square::EMPTY)
                                {
                                    // double move succeeds in right side
                                    // after single jump of right side initially
                                    if (!isVectorCleared)
                                    {
                                        moves[1].mFinalPos.clear();
                                        moves[1].mPath.clear();
                                        moves[1].mCapturedPos.clear();
                                        isVectorCleared = true;
                                    }
                                    moves[1].mFinalPos.push_back({posI - 4, posJ + 4});
                                    moves[1].mPath.push_back({{posI, posJ}, {posI - 1, posJ + 1}, {posI - 2, posJ + 2}, {posI - 3, posJ + 3}, {posI - 4, posJ + 4}});
                                    moves[1].mType = Move::DoubleJump;
                                    moves[1].mCapturedPos.push_back({{posI - 1, posJ + 1}, {posI - 3, posJ + 3}});
                                }
                            }
                            // for down double jump
                            // left down jump
                            if (posI - 2 != 7 and posJ + 2 != 7)
                            {
                                if ((b[posI - 2 + 1][posJ + 2 + 1].value == nP or b[posI - 1][posJ + 3].value == kP) and (posI - 1 != 7 and posJ + 3 != 7) and b[posI][posJ + 4].value == Engine::Square::EMPTY)
                                {

                                    if (!isVectorCleared)
                                    {
                                        moves[1].mFinalPos.clear();
                                        moves[1].mPath.clear();
                                        moves[1].mCapturedPos.clear();
                                        isVectorCleared = true;
                                    }
                                    moves[1].mFinalPos.push_back({posI, posJ + 4});
                                    moves[1].mPath.push_back({{posI, posJ}, {posI - 1, posJ + 1}, {posI - 2, posJ + 2}, {posI - 1, posJ + 3}, {posI, posJ + 4}});
                                    moves[1].mType = Move::DoubleJump;
                                    moves[1].mCapturedPos.push_back({{posI - 1, posJ + 1}, {posI - 1, posJ + 2 + 1}});
                                }
                            }
                        }
                    }
                    else
                    {
                        // no possible move in right side
                        moves[1].makeItNoMove(posI, posJ);
                    }
                }
                else
                {
                    // no possible move in left side
                    moves[1].makeItNoMove(posI, posJ);
                }
            }
            // posi-1,posj-1 has white ( i.e friend piece )
            else
            {
                // no possible move in left side
                moves[1].makeItNoMove(posI, posJ);
            }
        }
        else
        {
            // no possible move in right side
            moves[1].makeItNoMove(posI, posJ);
        }
        // black piece logic for white king is same except for piece check

        // for right move initially
        if (posI != 7 and posJ != 0)
        {
            // normal move
            if (b[posI + 1][posJ - 1].value == Engine::Square::EMPTY)
            {
                // normal move
                moves[2].mFinalPos = {{posI + 1, posJ - 1}};
                moves[2].mPath = {{{posI, posJ}, {posI + 1, posJ - 1}}};
                moves[2].mType = Move::Normal;
                moves[2].mCapturedPos = {};
            }
            else if (b[posI + 1][posJ - 1].value == nP or b[posI + 1][posJ - 1].value == kP)
            {
                if (posI + 1 != 7 and posJ - 1 != 0)
                {
                    if (b[posI + 2][posJ - 2].value == Engine::Square::EMPTY)
                    {
                        // single jump
                        moves[2].mFinalPos = {{posI + 2, posJ - 2}};
                        moves[2].mPath = {{{posI, posJ}, {posI + 1, posJ - 1}, {posI + 2, posJ - 2}}};
                        moves[2].mType = Move::SingleJump;
                        moves[2].mCapturedPos = {{{posI + 1, posJ - 1}}};

                        // will single jump result in double jump?
                        {
                            bool isVectorCleared = false;
                            if (posI + 2 != 7 and posJ - 2 != 0)
                            {
                                // checking for double jump in right part after
                                // single jump of right initially
                                if ((b[posI + 3][posJ - 3].value == nP or b[posI + 3][posJ - 3].value == kP) and (posI + 3 != 7 and posJ - 3 != 0) and b[posI + 4][posJ - 4].value == Engine::Square::EMPTY)
                                {
                                    // double move succeeds
                                    if (!isVectorCleared)
                                    {
                                        moves[2].mFinalPos.clear();
                                        moves[2].mPath.clear();
                                        moves[2].mCapturedPos.clear();
                                        isVectorCleared = true;
                                    }
                                    moves[2].mFinalPos.push_back({posI + 4, posJ - 4});
                                    moves[2].mPath.push_back({{posI, posJ}, {posI + 1, posJ - 1}, {posI + 2, posJ - 2}, {posI + 3, posJ - 3}, {posI + 4, posJ - 4}});
                                    moves[2].mType = Move::DoubleJump;
                                    moves[2].mCapturedPos.push_back({{posI + 1, posJ - 1}, {posI + 3, posJ - 3}});
                                }
                            }
                            if (posI + 2 != 7 and posJ - 2 != 7)
                            {
                                // double jump in left part after single jump
                                // in right initially
                                if ((b[posI + 3][posJ - 2 + 1].value == nP or b[posI + 3][posJ - 2 + 1].value == kP) and (posI + 3 != 7 and posJ - 2 + 1 != 7) and b[posI + 4][posJ - 2 + 2].value == Engine::Square::EMPTY)
                                {
                                    // double move succeeds in right side
                                    // after single jump of left side initially
                                    if (!isVectorCleared)
                                    {
                                        moves[2].mFinalPos.clear();
                                        moves[2].mPath.clear();
                                        moves[2].mCapturedPos.clear();
                                        isVectorCleared = true;
                                    }
                                    moves[2].mFinalPos.push_back({posI + 4, posJ});
                                    moves[2].mPath.push_back({{posI, posJ}, {posI + 1, posJ - 1}, {posI + 2, posJ - 2}, {posI + 3, posJ - 2 + 1}, {posI + 4, posJ - 2 + 2}});
                                    moves[2].mType = Move::DoubleJump;
                                    moves[2].mCapturedPos.push_back({{posI + 1, posJ - 1}, {posI + 3, posJ - 2 + 1}});
                                }
                            }
                            // upward jump for double jump after downward single
                            // jump ( i.e. left jump of white )
                            if (posI + 2 != 0 and posJ - 2 != 0)
                            {
                                // checking for double jump in left part of up
                                // after  single jump of right of down
                                if ((b[posI + 2 - 1][posJ - 2 - 1].value == nP or b[posI + 2 - 1][posJ - 3].value == kP) and (posI + 1 != 0 and posJ - 3 != 0) and b[posI][posJ - 4].value == Engine::Square::EMPTY)
                                {
                                    // double move succeeds
                                    if (!isVectorCleared)
                                    {
                                        moves[2].mFinalPos.clear();
                                        moves[2].mPath.clear();
                                        moves[2].mCapturedPos.clear();
                                        isVectorCleared = true;
                                    }
                                    moves[2].mFinalPos.push_back({posI, posJ - 4});
                                    moves[2].mPath.push_back({{posI, posJ}, {posI + 1, posJ - 1}, {posI + 2, posJ - 2}, {posI + 1, posJ - 3}, {posI, posJ - 4}});
                                    moves[2].mType = Move::DoubleJump;
                                    moves[2].mCapturedPos.push_back({{posI + 1, posJ - 1}, {posI + 1, posJ - 3}});
                                }
                            }
                        }
                    }
                    else
                    {
                        // no possible move in left side
                        moves[2].makeItNoMove(posI, posJ);
                    }
                }
                else
                {
                    // no possible move in left side
                    moves[2].makeItNoMove(posI, posJ);
                }
            }
            // posi-1,posj-1 has white ( i.e friend piece )
            else
            {
                // no possible move in left side
                moves[2].makeItNoMove(posI, posJ);
            }
        }
        else
        {
            // no possible move in right side
            moves[2].makeItNoMove(posI, posJ);
        }
        // for left move initially
        if (posI != 7 and posJ != 7)
        {
            if (b[posI + 1][posJ + 1].value == Engine::Square::EMPTY)
            {
                // normal move
                moves[3].mFinalPos = {{posI + 1, posJ + 1}};
                moves[3].mPath = {{{posI, posJ}, {posI + 1, posJ + 1}}};
                moves[3].mType = Move::Normal;
                moves[3].mCapturedPos = {};
            }
            else if (b[posI + 1][posJ + 1].value == nP or b[posI + 1][posJ + 1].value == kP)
            {
                if (posI + 1 != 7 and posJ + 1 != 7)
                {

                    if (b[posI + 2][posJ + 2].value == Engine::Square::EMPTY)
                    {
                        // single jump
                        moves[3].mFinalPos = {{posI + 2, posJ + 2}};
                        moves[3].mPath = {{{posI, posJ}, {posI + 1, posJ + 1}, {posI + 2, posJ + 2}}};
                        moves[3].mType = Move::SingleJump;
                        moves[3].mCapturedPos = {{{posI + 1, posJ + 1}}};

                        // will single jump result in double jump?
                        {
                            bool isVectorCleared = false;
                            if (posI + 2 != 7 and posJ + 2 != 0)
                            {
                                // checking for double jump in right part after
                                // single jump of left initially
                                if ((b[posI + 3][posJ + 2 - 1].value == nP or b[posI + 3][posJ + 2 - 1].value == kP) and (posI + 3 != 7 and posJ + 2 - 1 != 0) and b[posI + 4][posJ + 2 - 2].value == Engine::Square::EMPTY)
                                {
                                    // double move succeeds
                                    if (!isVectorCleared)
                                    {
                                        moves[3].mFinalPos.clear();
                                        moves[3].mPath.clear();
                                        moves[3].mCapturedPos.clear();
                                        isVectorCleared = true;
                                    }
                                    moves[3].mFinalPos.push_back({posI + 4, posJ});
                                    moves[3].mPath.push_back({{posI, posJ}, {posI + 1, posJ + 1}, {posI + 2, posJ + 2}, {posI + 3, posJ + 2 - 1}, {posI + 4, posJ}});
                                    moves[3].mType = Move::DoubleJump;
                                    moves[3].mCapturedPos.push_back({{posI + 1, posJ + 1}, {posI + 3, posJ + 1}});
                                }
                            }
                            if (posI + 2 != 7 and posJ + 2 != 7)
                            {
                                // checking for double jump in left part after
                                // single jump of left initially
                                if ((b[posI + 3][posJ + 3].value == nP or b[posI + 3][posJ + 3].value == kP) and (posI + 3 != 7 and posJ + 3 != 7) and b[posI + 4][posJ + 4].value == Engine::Square::EMPTY)
                                {
                                    if (!isVectorCleared)
                                    {
                                        moves[3].mFinalPos.clear();
                                        moves[3].mPath.clear();
                                        moves[3].mCapturedPos.clear();
                                        isVectorCleared = true;
                                    }
                                    moves[3].mFinalPos.push_back({posI + 4, posJ + 4});
                                    moves[3].mPath.push_back({{posI, posJ}, {posI + 1, posJ + 1}, {posI + 2, posJ + 2}, {posI + 3, posJ + 3}, {posI + 4, posJ + 4}});
                                    moves[3].mType = Move::DoubleJump;
                                    moves[3].mCapturedPos.push_back({{posI + 1, posJ + 1}, {posI + 3, posJ + 3}});
                                }
                            }
                            // now right jump of up also needs to be checked
                            if (posI + 2 != 0 and posJ + 2 != 7)
                            {
                                if ((b[posI + 2 - 1][posJ + 2 + 1].value == nP or b[posI + 2 - 1][posJ + 3].value == kP) and (posI + 1 != 0 and posJ + 2 + 1 != 7) and b[posI][posJ + 2 + 2].value == Engine::Square::EMPTY)
                                {
                                    // double move succeeds in right side
                                    // after single down jump of left initially
                                    if (!isVectorCleared)
                                    {
                                        moves[3].mFinalPos.clear();
                                        moves[3].mPath.clear();
                                        moves[3].mCapturedPos.clear();
                                        isVectorCleared = true;
                                    }
                                    moves[3].mFinalPos.push_back({posI, posJ + 4});
                                    moves[3].mPath.push_back({{posI, posJ}, {posI + 1, posJ + 1}, {posI + 2, posJ + 2}, {posI + 1, posJ + 2 + 1}, {posI, posJ + 2 + 2}});
                                    moves[3].mType = Move::DoubleJump;
                                    moves[3].mCapturedPos.push_back({{posI + 1, posJ + 1}, {posI + 1, posJ + 3}});
                                }
                            }
                        }
                    }
                    else
                    {
                        // no possible move in right side
                        moves[3].makeItNoMove(posI, posJ);
                    }
                }
                else
                {
                    // no possible move in left side
                    moves[3].makeItNoMove(posI, posJ);
                }
            }
            // posi-1,posj-1 has white ( i.e friend piece )
            else
            {
                // no possible move in left side
                moves[3].makeItNoMove(posI, posJ);
            }
        }
        else
        {
            // no possible move in left side
            moves[3].makeItNoMove(posI, posJ);
        }
    }
    else
    {
        throw std::runtime_error("findLegalNextMoves called on empty pos (i.e. no piece exists here ) ");
    }
    return moves;
}

void Engine::MainEngine::displayBoard()
{
    std::cout << "{" << std::endl;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            std::cout << mGC->mBoard[i][j].value << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << "}" << std::endl;
}

Piece *Engine::MainEngine::findPieceAtThisPos(int posI, int posJ)
{

    //TODO: (done) do for kings also
    if (mGC->mBoard[posI][posJ].value == Engine::Square::WHITE)
    {
        for (int i = 0; i < mGC->mWpieces.size(); i++)
        {
            if (mGC->mWpieces[i].getPosI() == posI and mGC->mWpieces[i].getPosJ() == posJ)
            {
                return &(mGC->mWpieces[i]);
            }
        }
    }
    else if (mGC->mBoard[posI][posJ].value == Engine::Square::BLACK)
    {
        for (int i = 0; i < mGC->mBpieces.size(); i++)
        {
            if (mGC->mBpieces[i].getPosI() == posI and mGC->mBpieces[i].getPosJ() == posJ)
            {
                return &(mGC->mBpieces[i]);
            }
        }
    }
    else if (mGC->mBoard[posI][posJ].value == Engine::Square::WHITEKING)
    {
        for (int i = 0; i < mGC->mWKpieces.size(); i++)
        {
            if (mGC->mWKpieces[i].getPosI() == posI and mGC->mWKpieces[i].getPosJ() == posJ)
            {
                return &(mGC->mWKpieces[i]);
            }
        }
    }
    else if (mGC->mBoard[posI][posJ].value == Engine::Square::BLACKKING)
    {
        for (int i = 0; i < mGC->mBKpieces.size(); i++)
        {
            if (mGC->mBKpieces[i].getPosI() == posI and mGC->mBKpieces[i].getPosJ() == posJ)
            {
                return &(mGC->mBKpieces[i]);
            }
        }
    }

    // if code flow reaches here then there is no piece at (posI,posJ)
    std::cout << mGC->mBoard[posI][posJ].value << std::endl;
    throw std::runtime_error("findPieceAtThisPos function called on bad (posI,posJ)= " + std::to_string(posI) + "," + std::to_string(posJ) + " where there is no piece ");
}

void Engine::MainEngine::colorThePossibleMoves(const std::vector<Move> &moves, sf::Color color)
{
    //TODO: color other pos too like path later.
    // colouring the final pos:
    for (int index = 0; index < moves.size(); index++)
    {
        for (int i = 0; i < moves[index].mFinalPos.size(); i++)
        {
            std::cout << moves[index].mFinalPos[i][0] << "," << moves[index].mFinalPos[i][1] << "\t";

            // mGC->mBoard[moves[index].mFinalPos[i][0]][moves[index].mFinalPos[i][1]].square.setOutlineColor(sf::Color::White);
            mGC->mBoard[moves[index].mFinalPos[i][0]][moves[index].mFinalPos[i][1]].square.setFillColor(color);
            // mGC->mBoard[moves[index].mFinalPos[i][0]][moves[index].mFinalPos[i][1]].square.setOutlineColor(sf::Color::Transparent);
        }
        std::cout << std::endl;
    }
}

// returns true if promoted else false
bool Engine::MainEngine::promoteToKingIfPossible(Piece *piece)
{

    auto type = piece->getType();
    if (type == Type::WhiteKing or type == Type::BlackKing)
    {
        // it is already a king
        return false;
    }
    else if (type == Type::White and piece->getPosI() == 0)
    {
        // promote it
        // delete the current white piece at that position

        WhiteKing newKing;
        newKing.create(sf::Vector2f(mGC->mBoard[piece->getPosI()][piece->getPosJ()].square.getPosition().x + mGC->mContext->mSquareSize.x / 2.f, mGC->mBoard[piece->getPosI()][piece->getPosJ()].square.getPosition().y + mGC->mContext->mSquareSize.y / 2.f), piece->getPosI(), piece->getPosJ(), mGC->mContext->mSquareSize);

        // works because the findpieceatthispos func. first returns
        // a white piece not a white king, so deletes that white piece
        mGC->captureAPiece(piece);
        mGC->mBoard[newKing.getPosI()][newKing.getPosJ()].value = Engine::Square::WHITEKING;
        std::cout << "size of wpieces = " << mGC->mWpieces.size() << std::endl;
        mGC->mWKpieces.push_back(newKing);
        return true;
    }
    else if (type == Type::Black and piece->getPosI() == 7)
    {
        // promote it
        // delete the current white piece at that position

        BlackKing newKing;
        newKing.create(sf::Vector2f(mGC->mBoard[piece->getPosI()][piece->getPosJ()].square.getPosition().x + mGC->mContext->mSquareSize.x / 2.f, mGC->mBoard[piece->getPosI()][piece->getPosJ()].square.getPosition().y + mGC->mContext->mSquareSize.y / 2.f), piece->getPosI(), piece->getPosJ(), mGC->mContext->mSquareSize);

        // works because the findpieceatthispos func. first returns
        // a black piece not a black king, so deletes that black piece
        mGC->captureAPiece(piece);
        mGC->mBoard[newKing.getPosI()][newKing.getPosJ()].value = Engine::Square::BLACKKING;
        mGC->mBKpieces.push_back(newKing);
        return true;
    }
    else
    {
        return false;
    }
}

// returns true if promoted else false
// this one is for minimax part
bool Engine::MainEngine::promoteToKingIfPossible(BOARD &tempBoard, int posI, int posJ)
{
    if (tempBoard[posI][posJ].value == Engine::Square::WHITE and posI == 0)
    {
        // promotable to white king
        tempBoard[posI][posJ].value = Engine::Square::WHITEKING;
        return true;
    }
    else if (tempBoard[posI][posJ].value == Engine::Square::BLACK and posI == 7)
    {
        // promotable to black king
        tempBoard[posI][posJ].value = Engine::Square::BLACKKING;
        return true;
    }
    return false;
}

// ai part of engine
int Engine::MainEngine::minimax(BOARD board, int depth, bool isMaxPlayer)
{
    // base case of recursion starts
    int eval = staticEval(board, depth,isMaxPlayer);
    if (eval != -1111111)
    {
        return eval;
    }
    // base case of recursion ends

    if (isMaxPlayer)
    {
        long int maxEval = -9999999;
        auto map = (mGC->mMandatory->isMandatoryMove()) ? ((mGC->mMandatory->doesDoubleJumpExist()) ? mGC->mMandatory->getDoubleJump() : mGC->mMandatory->getSingleJump()) : mGC->mMandatory->getNormalMoves();
        for (auto i = map.begin(); i != map.end(); i++)
        {
            // iterating over each doubleJump element of map of such pieces
            for (auto j = i->second.begin(); j != i->second.end(); j++)
            {
                // iterating over each double jump moves of a piece
                auto aMove = *j;
                for (int p = 0; p < aMove.mPath.size(); p++)
                {
                    // make a temporary copy board on which we change data for minimax
                    BOARD tempBoard = board;
                    auto initialPosVal = tempBoard[aMove.mPath[p][0][0]][aMove.mPath[p][0][1]].value;
                    for (int n = 0; n < aMove.mPath[p].size(); n++)
                    {
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

                    mGC->mMandatory->findMandatoryMoves(tempBoard, false);
                    // apply minimax to newly created parent:
                    int score = mGC->mEngine->minimax(tempBoard, depth + 1, false);
                    if (score > maxEval)
                    {
                        maxEval = score;
                    }
                }
            }
        }
        return maxEval;
    }
    else
    {
        long int minEval = 9999999;
        auto map = (mGC->mMandatory->isMandatoryMove()) ? ((mGC->mMandatory->doesDoubleJumpExist()) ? mGC->mMandatory->getDoubleJump() : mGC->mMandatory->getSingleJump()) : mGC->mMandatory->getNormalMoves();
        for (auto i = map.begin(); i != map.end(); i++)
        {
            // iterating over each doubleJump element of map of such pieces
            for (auto j = i->second.begin(); j != i->second.end(); j++)
            {
                // iterating over each double jump moves of a piece
                auto aMove = *j;
                for (int p = 0; p < aMove.mPath.size(); p++)
                {
                    // make a temporary copy board on which we change data for minimax
                    BOARD tempBoard = board;
                    auto initialPosVal = tempBoard[aMove.mPath[p][0][0]][aMove.mPath[p][0][1]].value;
                    for (int n = 0; n < aMove.mPath[p].size(); n++)
                    {
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
                    mGC->mMandatory->findMandatoryMoves(tempBoard, true);
                    // apply minimax to newly created parent:
                    int score = mGC->mEngine->minimax(tempBoard, depth + 1, true);
                    if (score < minEval)
                    {
                        minEval = score;
                    }
                }
            }
        }
        return minEval;
    }
}

int Engine::MainEngine::staticEval(const BOARD &board, int depth, bool isMaxPlayer)
{
    struct Data
    {
        int whites = 0;
        int wks = 0;
        int blacks = 0;
        int bks = 0;
    };
    Data values;
    // FIXME: magic numbers (8,8) of width and height of board
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            switch (board[i][j].value)
            {
            case Engine::Square::WHITE:
                values.whites++;
                break;
            case Engine::Square::WHITEKING:
                values.wks++;
                break;
            case Engine::Square::BLACK:
                values.blacks++;
                break;
            case Engine::Square::BLACKKING:
                values.bks++;
                break;
            }
        }
    }

    // check if game is over
    if (values.blacks == 0 and values.bks == 0)
    {
        // no black piece on tempBoard means black has lost (white won)
        return 1000 - depth;
    }

    if (values.whites == 0 and values.wks == 0)
    {
        // black won
        return -1000 + depth;
    }

    if (!mGC->mMandatory->doesMoveExist())
    {
        if (isMaxPlayer)
        {
            return -1000 + depth;
        }
        else
        {
            return +1000 - depth;
        }
    }
    // code flow here means game is not over
    if (depth == 3)
    {
        return values.whites + values.wks * 2 - values.blacks - values.bks * 2;
    }
    else
    {
        return -1111111;
    }
}
