#include "../../include/checkers/mandatory.hpp"
#include "../../include/checkers/gameContext.hpp"

using namespace Engine;

// checks if mandatory piece exists at posI,posJ or not
bool MandatoryMove::isClickedOnMandatoryPiece(int posI, int posJ)
{
    if (isMandatoryMove())
    {
        try
        {
            mDoubleJump.at({posI, posJ});
            return true;
        }
        catch (const std::out_of_range &)
        {
            try
            {
                mSingleJump.at({posI, posJ});
                return true;
            }

            catch (const std::out_of_range &)
            {
                return false;
            }
        }
    }
    else
    {
        throw std::runtime_error("isClickedOnMandatoryPiece called when there is no mandatory move for say white");
    }
}

MandatoryMove::MandatoryMove(std::shared_ptr<Engine::GamePlayContext> &gc) : mGC(gc), mIsMandatoryMove(false), mDoesMoveExist(true), misThereADoubleJump(false)
{
}

// finds all the mandatory moves for current player
void MandatoryMove::findMandatoryMoves()
{
    resetValues();
    if (mGC->getCurrPlayer().getSign() == "W")
    {
        // search in all the white pieces ( normal + kings )
        _find(mGC->mWpieces);
        _find(mGC->mWKpieces);
    }
    else if (mGC->getCurrPlayer().getSign() == "B")
    {
        // search in all the black pieces ( normal + kings )
        _find(mGC->mBpieces);
        _find(mGC->mBKpieces);
    }
}

// overload for ai part
void MandatoryMove::findMandatoryMoves(const BOARD &Board, bool isMaxPlayer)
{
    resetValues();
    if (isMaxPlayer)
    {
        // search in all the white pieces ( normal + kings )
        _find(Board, Engine::Square::WHITE, Engine::Square::WHITEKING);
    }
    else
    {
        // search in all the black pieces ( normal + kings )
        _find(Board, Engine::Square::BLACK, Engine::Square::BLACKKING);
    }
}

// getters
bool MandatoryMove::isMandatoryMove()
{
    return mIsMandatoryMove;
};
bool MandatoryMove::doesMoveExist()
{
    return mDoesMoveExist;
}
bool MandatoryMove::doesDoubleJumpExist()
{
    return misThereADoubleJump;
}
const MandatoryMove::Map &MandatoryMove::getNormalMoves()
{
    return mNormalMove;
}
const MandatoryMove::Map &MandatoryMove::getSingleJump()
{
    return mSingleJump;
}
const MandatoryMove::Map &MandatoryMove::getDoubleJump()
{
    return mDoubleJump;
}

// manages currclicked piece to update when clicked in mandatory piece
// non-imp for ai , as this is for "click-events"
void MandatoryMove::manageMove(int posI, int posJ)
{
    if (isClickedOnMandatoryPiece(posI, posJ))
    {
        if (mGC->mCurrClickedPiece->getPosI() == posI and mGC->mCurrClickedPiece->getPosJ() == posJ)
        {
        }
        else
        {
            mGC->mCurrClickedPiece->deColorNextMoves();
            if (mDoubleJump.size() != 0)
            {
                mGC->mCurrClickedPiece->setUpdatedValue(posI, posJ, mGC->getCurrPlayer(), mDoubleJump.at({posI, posJ}));
            }
            else
            {
                mGC->mCurrClickedPiece->setUpdatedValue(posI, posJ, mGC->getCurrPlayer(), mSingleJump.at({posI, posJ}));
            }
            mGC->mCurrClickedPiece->colorNextMoves();
        }
    }
}

void MandatoryMove::resetValues()
{
    // first unsetting the value if it was set already in say Black's turn.
    mIsMandatoryMove = false;
    // clearing previous data say of black before putting new data
    mSingleJump.clear();
    mNormalMove.clear();
    mDoubleJump.clear();
    mDoesMoveExist = false;
    misThereADoubleJump = false;
}

template <class T>
void MandatoryMove::_find(std::vector<T> &pieces)
{
    // search in all the white pieces ( normal + kings )
    for (int i = 0; i < pieces.size(); i++)
    {
        auto moves = mGC->mEngine->findLegalNextMoves(pieces[i].getPosI(), pieces[i].getPosJ());
        for (int j = 0; j < moves.size(); j++)
        {
            if (moves[j].mType == Move::NoMove or moves[j].mType == Move::Normal)
            {
                // just ignore
                if (moves[j].mType == Move::Normal)
                {
                    mDoesMoveExist = true;
                    // normal move is for now used only in ai, here kept so that
                    // the first findmandatorymove call after immediately player's
                    // turn ends ,works
                    try
                    {
                        mNormalMove.at({pieces[i].getPosI(), pieces[i].getPosJ()}).push_back(moves[j]);
                    }
                    catch (const std::out_of_range &)
                    {
                        mNormalMove.insert({{pieces[i].getPosI(), pieces[i].getPosJ()}, std::vector<Move>(1, moves[j])});
                    }
                }
            }
            else if (!misThereADoubleJump and moves[j].mType == Move::SingleJump)
            {
                try
                {
                    mSingleJump.at({pieces[i].getPosI(), pieces[i].getPosJ()}).push_back(moves[j]);
                }
                catch (const std::out_of_range &)
                {
                    mIsMandatoryMove = true;
                    mDoesMoveExist = true;
                    mSingleJump.insert({{pieces[i].getPosI(), pieces[i].getPosJ()}, std::vector<Move>(1, moves[j])});
                }
            }
            else if (moves[j].mType == Move::DoubleJump)
            {
                try
                {
                    mDoubleJump.at({pieces[i].getPosI(), pieces[i].getPosJ()}).push_back(moves[j]);
                }
                catch (const std::out_of_range &)
                {
                    // if double jump exists then double jump is mandatory
                    // so we clear the single jump
                    mSingleJump.clear();
                    misThereADoubleJump = true;
                    mIsMandatoryMove = true;
                    mDoesMoveExist = true;
                    mDoubleJump.insert({{pieces[i].getPosI(), pieces[i].getPosJ()}, std::vector<Move>(1, moves[j])});
                }
            }
        }
    }
}

// this one is for ai part
void MandatoryMove::_find(const BOARD &Board, Engine::Square::VALUE nP, Engine::Square::VALUE kP)
{
    //FIXME: magic number for the 8*8 size of board
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            //FIXME: magic number for the ai being black player for now
            if (Board[i][j].value == nP or Board[i][j].value == kP)
            {
                auto moves = mGC->mEngine->findLegalNextMoves(i, j, Board);

                for (int index = 0; index < moves.size(); index++)
                {
                    if (moves[index].mType == Move::NoMove or moves[index].mType == Move::Normal)
                    {
                        // just ignore
                        if (moves[index].mType == Move::Normal)
                        {
                            mDoesMoveExist = true;
                            try
                            {
                                mNormalMove.at({i, j}).push_back(moves[index]);
                            }
                            catch (const std::out_of_range &)
                            {
                                mNormalMove.insert({{i, j}, std::vector<Move>(1, moves[index])});
                            }
                        }
                    }
                    else if (!misThereADoubleJump and moves[index].mType == Move::SingleJump)
                    {
                        try
                        {
                            mSingleJump.at({i, j}).push_back(moves[index]);
                        }
                        catch (const std::out_of_range &)
                        {
                            mIsMandatoryMove = true;
                            mDoesMoveExist = true;
                            mSingleJump.insert({{i, j}, std::vector<Move>(1, moves[index])});
                        }
                    }
                    else if (moves[index].mType == Move::DoubleJump)
                    {
                        try
                        {
                            mDoubleJump.at({i, j}).push_back(moves[index]);
                        }
                        catch (const std::out_of_range &)
                        {
                            // if double jump exists then double jump is mandatory
                            // so we clear the single jump
                            mSingleJump.clear();
                            misThereADoubleJump = true;
                            mIsMandatoryMove = true;
                            mDoesMoveExist = true;
                            mDoubleJump.insert({{i, j}, std::vector<Move>(1, moves[index])});
                        }
                    }
                }
            }
        }
    }
}
