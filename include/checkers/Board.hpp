#pragma once
#include <SFML/Graphics.hpp>
#include "checkers.hpp"

class Board : public sf::Drawable, public sf::Transformable
{

public:
    
    Board(std::shared_ptr<Context> &context) : mContext(context), sizeOfQuad(sf::Vector2f(mContext->mSquareSize.x, mContext->mSquareSize.y))
    {
        mVertices.setPrimitiveType(sf::Quads);
        // 64 unconnected quads
        mVertices.resize(8 * 8 * 4);
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                sf::Vertex *quad = &mVertices[i * 4 * 8 + j * 4];

                // setting position
                quad[0].position = sf::Vector2f(j * sizeOfQuad.x, i * sizeOfQuad.y);
                quad[1].position = sf::Vector2f((j + 1) * sizeOfQuad.x, i * sizeOfQuad.y);
                quad[2].position = sf::Vector2f((j + 1) * sizeOfQuad.x, (i + 1) * sizeOfQuad.y);
                quad[3].position = sf::Vector2f(j * sizeOfQuad.x, (i + 1) * sizeOfQuad.y);

                // resizing them relative to the board top coordinates
                for (int i = 0; i < 4; i++)
                {
                    quad[i].position = sf::Vector2f(quad[i].position.x + mContext->mBoardTopCoords.x, quad[i].position.y + mContext->mBoardTopCoords.y);
                }

                // For texturing the quads
                // index = 0 => white and index = 1 => brown
                int index;
                if (i % 2 == 0 and j % 2 == 0 or i % 2 == 1 and j % 2 == 1)
                    index = 0;
                else
                    index = 1;

                // VVI: dont change these 125s to squaresize ,as these are texture 
                // coordinates and i only have 125*125 of a square in texture
                // setting texture coordinates to show black or white texture
                quad[0].texCoords = sf::Vector2f(index * 125, 0);
                quad[1].texCoords = sf::Vector2f((index + 1) * 125, 0);
                quad[2].texCoords = sf::Vector2f((index + 1) * 125, 125);
                quad[3].texCoords = sf::Vector2f(index * 125, 125);
            }
        }
    }
    ~Board() {}

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &(mContext->mTextures->getAsset(AssetID::Textures::Board));

        // draw the vertex array
        target.draw(mVertices, states);
    }

private:
    std::shared_ptr<Context> mContext;
    sf::VertexArray mVertices;
    sf::Vector2f sizeOfQuad;
};