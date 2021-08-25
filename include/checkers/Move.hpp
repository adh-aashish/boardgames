// all the information regarding a move
#pragma once
#include <vector>
#include "Piece.hpp"
#include <iostream>


class Move
{
public:
    Move() {}
    ~Move() {}
    // make the piece at posI,posJ to have nomove "may be say of left side"
    void makeItNoMove(int posI, int posJ)
    {
        mFinalPos = {};
        mType = Move::NoMove;
        mPath = {{{posI, posJ}}};
        mCapturedPos = {};
    }
    // displays the move in console
    void displayMove()
    {
        std::cout << "=============Detail of a Move starts:=============" << std::endl;
        std::cout << "Final Pos:" << std::endl;
        for (int i = 0; i < mFinalPos.size(); i++)
        {
            std::cout << mFinalPos[i][0] << "," << mFinalPos[i][1] << "\t";
        }
        std::cout << std::endl;
        std::cout << "Path: " << std::endl;
        for (int i = 0; i < mPath.size(); i++)
        {
            for (int j = 0; j < mPath[i].size(); j++)
            {
                std::cout << mPath[i][j][0] << "," << mPath[i][j][1] << "  ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        std::cout << "Captured Pos: " << std::endl;
        for (int i = 0; i < mCapturedPos.size(); i++)
        {
            for (int j = 0; j < mCapturedPos[i].size(); j++)
            {
                std::cout << mCapturedPos[i][j][0] << "," << mPath[i][j][1] << "  ";
            }
            std::cout << std::endl;
        }
        std::cout << "=============Detail of a Move ends:=============" << std::endl;
        ;
    }
    enum MoveType
    {
        NoMove,
        Normal,
        SingleJump,
        DoubleJump,
        TripleJump
    };
    MoveType mType;
    // the mFinalPos can be two possibilities if double jump to two ways say from initially a jump in left side say
    std::vector<std::vector<int>> mFinalPos;
    std::vector<std::vector<std::vector<int>>> mPath;
    std::vector<std::vector<std::vector<int>>> mCapturedPos;
};
