#include "../../include/checkers/currClickedPiece.hpp"
#include "../../include/checkers/gameContext.hpp"

Engine::CurrentClickedPiece::CurrentClickedPiece(std::shared_ptr<Engine::GamePlayContext> &gc, Player &p) : mGC(gc), mP(p)
{
    // since constructor is called in initial phase of game , so
    // player == piece type can be said initially
    // i.e. no need for king initially
    if (mP.getSign() == "W")
    {
        mType = Type::White;
    }
    else if (mP.getSign() == "B")
    {
        mType = Type::Black;
    }
}

Engine::CurrentClickedPiece::~CurrentClickedPiece() {}

void Engine::CurrentClickedPiece::setUpdatedValue(int posI, int posJ, Player &p, const std::vector<Move> &NLM)
{
    mIsClicked = true;
    mPosI = posI; // these are redundant as mPiece->getPosI() , getPosJ() exist.
    mPosJ = posJ;
    mPiece = mGC->mEngine->findPieceAtThisPos(mPosI, mPosJ);
    if (NLM.size() == 0)
    {
        mNextLegalMoves = mGC->mEngine->findLegalNextMoves(mPosI, mPosJ);
    }
    else
    {
        mNextLegalMoves = NLM;
    }
    mP = p;

    setTypeVal();
}
void Engine::CurrentClickedPiece::setTypeVal()
{
    auto val = mGC->mBoard[mPosI][mPosJ].value;
    if (val == Engine::Square::WHITE)
    {
        mType = Type::White;
    }
    else if (val == Engine::Square::BLACK)
    {
        mType = Type::Black;
    }
    else if (val == Engine::Square::WHITEKING)
    {
        mType = Type::WhiteKing;
    }
    else if (val == Engine::Square::BLACKKING)
    {
        mType = Type::BlackKing;
    }
}

Engine::Square::VALUE Engine::CurrentClickedPiece::typeToSquare()
{
    if (mType == Type::White)
    {
        return Engine::Square::WHITE;
    }
    else if (mType == Type::Black)
    {
        return Engine::Square::BLACK;
    }
    else if (mType == Type::WhiteKing)
    {
        return Engine::Square::WHITEKING;
    }
    else if (mType == Type::BlackKing)
    {
        return Engine::Square::BLACKKING;
    }
    else
    {
        throw std::runtime_error("typeToSquare function called on invalid type.");
    }
}
void Engine::CurrentClickedPiece::colorNextMoves()
{
    mGC->mEngine->colorThePossibleMoves(mNextLegalMoves, sf::Color::Green);
}
void Engine::CurrentClickedPiece::deColorNextMoves()
{
    mGC->mEngine->colorThePossibleMoves(mNextLegalMoves, sf::Color::Transparent);
}
bool Engine::CurrentClickedPiece::isClicked()
{
    return mIsClicked;
}
int Engine::CurrentClickedPiece::getPosI()
{
    return mPosI;
}
int Engine::CurrentClickedPiece::getPosJ()
{
    return mPosJ;
}

void Engine::CurrentClickedPiece::checkForFinalPosClick(int posI, int posJ)
{
    if (isClicked())
    {
        for (int index = 0; index < mNextLegalMoves.size(); index++)
        {
            for (int i = 0; i < mNextLegalMoves[index].mFinalPos.size(); i++)
            {
                if (posI == mNextLegalMoves[index].mFinalPos[i][0] and posJ == mNextLegalMoves[index].mFinalPos[i][1])
                {
                    // move part starts ( i.e. clicked on a final pos )

                    // eating the captured positions starts
                    // as we have right index , so no need to check unnecessary
                    // side, i.e. say left side move is whose final pos is clicked.
                    for (auto itr = mNextLegalMoves[index].mPath.begin(); itr != mNextLegalMoves[index].mPath.end(); itr++)
                    {
                        auto found = std::find(itr->begin(), itr->end(), std::vector<int>({posI, posJ}));
                        if (found != itr->end())
                        {
                            // that means final pos is found on this path of left
                            // side say
                            std::cout << "POS-I=" << (*found)[0] << " POS-J=" << (*found)[1] << std::endl;
                            for (int j = 0; j < (*itr).size(); j++)
                            {
                                if (mGC->mBoard[(*itr)[j][0]][(*itr)[j][1]].value != Engine::Square::EMPTY and !(mPiece->getPosI() == (*itr)[j][0] and mPiece->getPosJ() == (*itr)[j][1]))
                                {
                                    // eatable position
                                    //TODO: (done) add other else if for kings
                                    mGC->captureAPiece((*itr)[j][0], (*itr)[j][1]);
                                }
                            }
                        }
                    }

                    // eating part ends
                    mGC->mBoard[mPiece->getPosI()][mPiece->getPosJ()].value = Engine::Square::EMPTY;
                    mPiece->move(posI, posJ, mGC->mBoard[posI][posJ]);
                    // TODO: may be add for king here
                    // changed
                    // !mGC->mEngine->promoteToKingIfPossible(mPiece, mGC)
                    if (!mGC->mEngine->promoteToKingIfPossible(mPiece))
                    {
                        // we do this way as the currclickedpiece is not updated
                        // after the move ( which has previous values like say of
                        // mType, mPosI,mPosJ, mPiece doesn't exist in memory
                        // after promoted. )
                        mGC->mBoard[posI][posJ].value = typeToSquare();
                    }
                    mGC->mCurrClickedPiece->deColorNextMoves();
                    mGC->changePlayerTurn();
                    // changed starts
                    mGC->mEngine->displayBoard();
                    // changed ends
                    // move part ends
                    // now find the mandatory moves before new move starts
                    mGC->mMandatory->findMandatoryMoves();
                    return;
                }
            }
        }
    }
}
