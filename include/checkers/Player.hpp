#pragma once
#include <string>
#include "Engine.hpp"

class Player
{
private:
    bool mIsMyTurn;
    // sign = 'W' => White Player, 'B' => Black Player
    std::string mSign;
    bool mDidIWonTheGame;

    
public:
    Player(std::string sign) : mIsMyTurn(false), mDidIWonTheGame(false)
    {
        this->mSign = sign;
    }
    void setTurn(bool isMyTurn)
    {
        this->mIsMyTurn = isMyTurn;
    }
    bool isMyTurn()
    {
        return mIsMyTurn;
    }
    std::string getSign(){
        return mSign;
    }
    
};

