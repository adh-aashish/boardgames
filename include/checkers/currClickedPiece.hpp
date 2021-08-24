#pragma once
#include "Piece.hpp"
#include "Player.hpp"
#include <vector>
#include "Move.hpp"
#include <memory>
#include "Engine.hpp"

namespace Engine{
    struct GamePlayContext; // forward declaration
}

namespace Engine
{
    class CurrentClickedPiece
    {
    private:
        // this is the I,J position of the clicked piece
        int mPosI, mPosJ;
        Piece *mPiece;
        std::vector<Move> mNextLegalMoves;

        // i.e does this instance have current clicked piece
        // or is it just a placeholder for future clicks.
        bool mIsClicked = false;
        std::shared_ptr<Engine::GamePlayContext> mGC;

    public:
        // public data member to avoid getter ( no big reason )
        // this piece is of player p;
        // this is a copy of the player, so changing this doesn't change
        // actual player, for that use gamecontext logic
        mutable Player mP;
        Type::Piece mType;
        CurrentClickedPiece(std::shared_ptr<Engine::GamePlayContext> &gc, Player &p);
        ~CurrentClickedPiece();
        void setUpdatedValue(int posI, int posJ, Player &p,const std::vector<Move>& NLM ={});
        Engine::Square::VALUE typeToSquare();
        void colorNextMoves();
        void deColorNextMoves();
        bool isClicked();
        int getPosI();
        int getPosJ();
        // checks and makes move too if clicked in final pos
        void checkForFinalPosClick(int posI, int posJ);
        void setTypeVal();
        
    };
}
