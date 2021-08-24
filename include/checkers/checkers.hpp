#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "context.hpp"

class Checkers
{
public:
  Checkers();
  ~Checkers();
  void run();

private:
  std::shared_ptr<Context> mContext;
  void loadAssets();
};
