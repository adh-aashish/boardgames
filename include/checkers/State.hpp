#pragma once
#include <memory>

struct Context; // forward declaration

namespace StateEngine
{
    class State
    {
    public:
        State(){};
        virtual ~State(){};

        virtual void init() = 0;
        virtual void processEvents() = 0;
        virtual void update(float dt) = 0;
        virtual void render() = 0;
        virtual std::shared_ptr<Context>& getContext()=0;

        virtual void pause(){};
        virtual void start(){};
    };
}
