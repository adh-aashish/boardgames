#pragma once

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include <memory>
#include <vector>
#include "checkers.hpp"
#include "gameContext.hpp"
#include "context.hpp"

class Computer : public StateEngine::State
{
public:
    typedef std::vector<std::vector<Engine::Square>> BOARD;
    typedef std::map<std::vector<int>, std::vector<Move>> Map;
    Computer(std::shared_ptr<Context> &context);
    ~Computer();
    void init() override;
    void processEvents() override;
    void update(float dt) override;
    void render() override;
    std::shared_ptr<Context> &getContext() override;

private:
    void handleAIMove();

private:
    std::shared_ptr<Context> mContext;
    std::shared_ptr<Engine::GamePlayContext> mGC;
};
