#pragma once
#include "State.hpp"
#include <memory>
#include "context.hpp"

class MainMenu : public StateEngine::State
{
public:
    MainMenu(std::shared_ptr<Context> &context);
    ~MainMenu();

    void init() override;
    void processEvents() override;
    void update(float dt) override;
    void render() override;

    std::shared_ptr<Context> &getContext() override;

private:
    std::shared_ptr<Context> mContext;
    sf::Sprite mMenuBg;
    sf::Text mTitle;
    Button mButton[2];
};
