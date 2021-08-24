#pragma once
#include "Move.hpp"
#include "Engine.hpp"
#include <memory>

namespace Engine
{
    struct GamePlayContext; // forward declaration
}

namespace Engine
{
    class MandatoryMove
    {
    private:
        typedef std::vector<std::vector<Engine::Square>> BOARD;
        typedef std::map<std::vector<int>, std::vector<Move>> Map;
        std::shared_ptr<Engine::GamePlayContext> mGC;
        bool mIsMandatoryMove;
        // normal move is esp. imp for ai part ,and has less
        // meaning with mandatory
        // only use this when move is not mandatory
        Map mNormalMove;
        Map mSingleJump;
        Map mDoubleJump;
        bool misThereADoubleJump;
        // in the first move this should exist( initialized to true ), but after a
        // move we will check obviously
        bool mDoesMoveExist;

    private:
        template <class T>
        void _find(std::vector<T> &pieces);
        // overloading for the ai part
        void _find(const BOARD &Board, Engine::Square::VALUE, Engine::Square::VALUE);
        void resetValues();
        // checks if mandatory piece exists at posI,posJ or not
        bool isClickedOnMandatoryPiece(int posI, int posJ);

    public:
        MandatoryMove(std::shared_ptr<Engine::GamePlayContext> &gc);
        // finds all the mandatory moves for current player
        void findMandatoryMoves();
        // this one is for the ai part
        void findMandatoryMoves(const BOARD &Board, bool isMaxPlayer);
        void manageMove(int posI, int posJ);
        // getters
        bool isMandatoryMove();
        bool doesMoveExist();
        bool doesDoubleJumpExist();
        const Map &getNormalMoves();
        const Map &getSingleJump();
        const Map &getDoubleJump();
        // end of getters
    };
}
