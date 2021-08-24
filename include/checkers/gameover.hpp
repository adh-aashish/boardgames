#pragma once

#include "State.hpp"
#include <memory>
#include "context.hpp"
#include "MainMenu.hpp"

class GameOver : public StateEngine::State
{
private:
    std::shared_ptr<Context> mContext;
    std::string mWhoWonTheGame;
    Button mBackToMenu;
    sf::Text mWhoWonInfo;

public:
    GameOver(std::shared_ptr<Context> &context, const std::string &winner);
    void init();
    void processEvents();
    void update(float dt);
    void render();
    std::shared_ptr<Context> &getContext();
};