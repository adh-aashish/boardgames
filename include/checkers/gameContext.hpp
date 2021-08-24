#pragma once
#include <memory>
#include "context.hpp"
#include "Piece.hpp"
#include "Board.hpp"
#include "Player.hpp"
#include "currClickedPiece.hpp"
#include "Engine.hpp"
#include "mandatory.hpp"
#include "gameover.hpp"

namespace Engine
{
    struct GamePlayContext
    {
    private:
        Button mBack;
        sf::Sprite mGameBg;
    public:
        typedef std::vector<std::vector<Engine::Square>> BOARD;
        std::shared_ptr<Context> mContext;
        // pieces
        std::vector<WhitePiece> mWpieces;
        std::vector<BlackPiece> mBpieces;
        std::vector<WhiteKing> mWKpieces;
        std::vector<BlackKing> mBKpieces;
        // Mandatory move
        std::unique_ptr<Engine::MandatoryMove> mMandatory;

        // Board
        Board mRenderBoard;

        // This is the board from where logic manipulation will be done
        // but for rendering part of the board we will have mRenderBoard
        BOARD mBoard;

        // engine is basically all the logic which will be repeated for
        // three games features - multiplayer offline,singleplayer,
        // multiplayer online
        std::unique_ptr<Engine::MainEngine> mEngine;

        // players
        // mP1 = "white" and mP2 = "black"
        Player mP1, mP2;

        // game status ( not used till now )
        bool mIsGameWon = false;
        bool mIsGameDrawn = false;

        // currentClickedPiece info
        std::unique_ptr<Engine::CurrentClickedPiece> mCurrClickedPiece;

        // functions
        // constructor
        GamePlayContext(std::shared_ptr<Context> &context);
        Player &getCurrPlayer();
        void changePlayerTurn();
        void draw();
        // captures a piece at posI,posJ , so only call at such positions
        // where piece exists
        void captureAPiece(int posI, int posJ);
        void captureAPiece(Piece *piece);
        // for ai part (changes the temporary board)
        void captureAPiece(BOARD &board, int posI, int posJ);
        int manageGameOver(bool shouldStateChange = true);
        void handleBackBtnClick(sf::Event::MouseButtonEvent mouse);
    };

}
