#include "../../include/checkers/Engine.hpp"


Piece::Piece() {}
Piece::~Piece() {}
void Piece::create(Type::Piece type, const sf::Vector2f &initialPos, int posI, int posJ, sf::Color color, const sf::Vector2f &squareSize)
{
    mSquareSize = squareSize;
    mType = type;
    mPosI = posI;
    mPosJ = posJ;
    // FIXME: magic number place
    mShape.setRadius(squareSize.x / 2.0f - 14);
    mShape.setPointCount(60);
    mShape.setOrigin(mShape.getLocalBounds().width / 2.0f, mShape.getLocalBounds().height / 2.0f);
    // initial position where position is of center of circle.
    mShape.setPosition(initialPos);
    mShape.setFillColor(color);
}

void Piece::move(int posI, int posJ, Engine::Square &sq)
{
    mPosI = posI;
    mPosJ = posJ;
    mShape.setPosition(sq.square.getPosition().x + mSquareSize.x / 2.0f, sq.square.getPosition().y + mSquareSize.y / 2.0f);
};