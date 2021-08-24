#pragma once

#include <SFML/Graphics.hpp>
#include "Piece.hpp"
#include "Move.hpp"
#include <memory>

namespace Engine
{
    struct GamePlayContext; // forward declaration
}

namespace Engine
{
    struct Square
    {
        // This current square has which of the following
        // pieces - white or black or nothing
        enum VALUE
        {
            WHITE,
            BLACK,
            EMPTY,
            WHITEKING,
            BLACKKING
        };
        Square::VALUE value = Square::EMPTY;
        // TODO: this square wont be drawn but used for logic manipulation
        // it only has size and position but not fillcolor
        sf::RectangleShape square;

    public:
        Square(const sf::Vector2f &squareSize);
    };

    class MainEngine
    {
    private:
        typedef std::vector<std::vector<Engine::Square>> BOARD;
        std::shared_ptr<Engine::GamePlayContext> mGC;

    public:
        MainEngine(std::shared_ptr<Engine::GamePlayContext> &gc);
        ~MainEngine();

        // finds the legal moves if mouse clicked on piece at board[i][j]
        // VVI: make sure to call this function on a square with clicked piece ( i.e not empty )
        std::vector<Move> findLegalNextMoves(int posI, int posJ, const BOARD &board = std::vector<std::vector<Engine::Square>>());
        // which piece instance exists at this pos is found out
        // VVI: Remember to not call on empty pos
        Piece *findPieceAtThisPos(int posI, int posJ);

        // just colors the possible moves for a piece "which is clicked"
        void colorThePossibleMoves(const std::vector<Move> &moves, sf::Color color);
        bool promoteToKingIfPossible(Piece *piece);
        // for ai part
        bool promoteToKingIfPossible(BOARD &tempBoard, int posI, int posJ);

        // displays board in console for debugging purposes say
        void displayBoard();

        // AI manipulation part
        int minimax(BOARD board, int depth, bool isMaxPlayer);
        int staticEval(const BOARD &board, int depth, bool isMaxPlayer);
    };

}
