#pragma once

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include <memory>
#include <vector>
#include "checkers.hpp"
#include "gameContext.hpp"
#include "context.hpp"


class Multiplayer : public StateEngine::State
{
public:
    Multiplayer(std::shared_ptr<Context> &context);
    ~Multiplayer();
    void init() override;
    void processEvents() override;
    void update(float dt) override;
    void render() override;
    std::shared_ptr<Context>& getContext() override;

private:
    std::shared_ptr<Context> mContext;
    std::shared_ptr<Engine::GamePlayContext> mGC;
};
