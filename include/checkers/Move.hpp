// all the information regarding a move
#pragma once
#include <vector>
#include "Piece.hpp"
#include <iostream>
using namespace std;

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
        cout << "=============Detail of a Move starts:=============" << endl;
        cout << "Final Pos:" << endl;
        for (int i = 0; i < mFinalPos.size(); i++)
        {
            cout << mFinalPos[i][0] << "," << mFinalPos[i][1] << "\t";
        }
        cout << endl;
        cout << "Path: " << endl;
        for (int i = 0; i < mPath.size(); i++)
        {
            for (int j = 0; j < mPath[i].size(); j++)
            {
                cout << mPath[i][j][0] << "," << mPath[i][j][1] << "  ";
            }
            cout << endl;
        }
        cout << endl;
        cout << "Captured Pos: " << endl;
        for (int i = 0; i < mCapturedPos.size(); i++)
        {
            for (int j = 0; j < mCapturedPos[i].size(); j++)
            {
                cout << mCapturedPos[i][j][0] << "," << mPath[i][j][1] << "  ";
            }
            cout << endl;
        }
        cout << "=============Detail of a Move ends:=============" << endl;
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
