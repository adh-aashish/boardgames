#pragma once
#include <stack>
#include <memory>
#include <stdexcept>
#include "State.hpp"

namespace StateEngine
{
    class StateManager
    {
    public:
        // pointer to a state
        typedef std::unique_ptr<StateEngine::State> mStatePtr;
        StateManager();
        ~StateManager();

        void add(mStatePtr newState, bool replace = false);
        void pop();
        mStatePtr &top();
        void processStackChange();

    private:
        std::stack<mStatePtr> mStateStack;
        mStatePtr mNewState;

        bool mAdd;
        bool mReplace;
        bool mRemove;
    };
}
