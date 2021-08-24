#include "../../include/checkers/StateManager.hpp"

StateEngine::StateManager::StateManager() : mAdd(false), mRemove(false), mReplace(false) {}
StateEngine::StateManager::~StateManager() {}

void StateEngine::StateManager::add(mStatePtr newState, bool replace)
{
    mReplace = replace;
    mAdd = true;
    mNewState = std::move(newState);
}

void StateEngine::StateManager::pop()
{
    mRemove = true;
}

StateEngine::StateManager::mStatePtr &StateEngine::StateManager::top()
{
    if (mStateStack.empty())
    {
        //TODO: see about catching the runtime_error i.e. exception handling
        throw std::runtime_error("Top function called on empty statestack.");
    }
    return mStateStack.top();
}

void StateEngine::StateManager::processStackChange()
{
    if (mAdd)
    {
        if (mReplace and !mStateStack.empty())
        {
            // remove the upper state
            mStateStack.pop();
            mReplace = false;
        }
        
        if (!mStateStack.empty())
        {
            mStateStack.top()->pause();
        }
        mStateStack.push(std::move(mNewState));
        mStateStack.top()->init();
        mStateStack.top()->start();
        mAdd = false;
    }
    if (mRemove and !mStateStack.empty())
    {
        mStateStack.pop();
        if (!mStateStack.empty())
        {
            mStateStack.top()->start();
        }
        mRemove = false;
    }
}
