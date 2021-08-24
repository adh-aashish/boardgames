#pragma once
#include <SFML/Graphics.hpp>

namespace Engine
{
    struct Square; // forward declaration
}
namespace Type
{
    enum Piece
    {
        White,
        Black,
        WhiteKing,
        BlackKing
    };
}

// abstract class

class Piece
{
public:
    Piece();
    virtual void create(Type::Piece type, const sf::Vector2f &initialPos, int posI, int posJ, sf::Color color, const sf::Vector2f &squareSize);
    virtual ~Piece();

    virtual void move(int posI, int posJ, Engine::Square &sq);
    virtual int getPosI() const { return mPosI; }
    virtual int getPosJ() const { return mPosJ; }
    virtual sf::CircleShape& getShape() { return mShape; }
    virtual Type::Piece getType() const { return mType; }

protected:
    Type::Piece mType;
    sf::CircleShape mShape;
    sf::Vector2f mSquareSize;
    // this stores the row and column of where this piece is:
    // (-1,-1) means it is out of board(died)
    int mPosI;
    int mPosJ;
    // mPosValue = 1 ( rendered at bottom initially ) , -1 (top)
    // this has meaning esp. for normal pieces not king.
    int mPosValue;
};

// by default white pieces are at bottom of board and black pieces at top
class WhitePiece : public Piece
{
public:
    WhitePiece() {}
    virtual void create(const sf::Vector2f &initialPos, int posI, int posJ, const sf::Vector2f &squareSize)
    {
        this->Piece::create(Type::White, initialPos, posI, posJ, sf::Color::White, squareSize);
        mPosValue = 1;
    }
    virtual ~WhitePiece() {}
};

class BlackPiece : public Piece
{
public:
    BlackPiece() {}
    virtual void create(const sf::Vector2f &initialPos, int posI, int posJ, const sf::Vector2f &squareSize)
    {
        this->Piece::create(Type::Black, initialPos, posI, posJ, sf::Color::Black, squareSize);
        mPosValue = -1;
    }
    virtual ~BlackPiece() {}
   
};

class WhiteKing : public WhitePiece
{
public:
    WhiteKing() {}
    void create(const sf::Vector2f &initialPos, int posI, int posJ, const sf::Vector2f &squareSize)
    {
        this->Piece::create(Type::WhiteKing, initialPos, posI, posJ, sf::Color::White, squareSize);
        
        getShape().setOutlineThickness(-10.0f);
        getShape().setOutlineColor(sf::Color::Red);
        // mPosValue = 0 means invalid mPosValue
        mPosValue = 0;
    }
    ~WhiteKing() {}
    // void move(){}
};

class BlackKing : public BlackPiece
{
public:
    BlackKing() {}
    void create(const sf::Vector2f &initialPos, int posI, int posJ, const sf::Vector2f &squareSize)
    {
        this->Piece::create(Type::BlackKing, initialPos, posI, posJ, sf::Color::Black, squareSize);
        
        getShape().setOutlineThickness(-8);
        getShape().setOutlineColor(sf::Color::Red);
        // mPosValue = 0 means invalid mPosValue
        mPosValue = 0;
    }
    ~BlackKing() {}
    // void move(){}
};