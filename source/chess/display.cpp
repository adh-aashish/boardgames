#include <SFML/Graphics.hpp>
#include "../../include/mainMenu.hpp"
#include <SFML/Audio.hpp>
#include <iostream>
#include <chrono>
#include "../../include/chess/def.h"
#include "../../include/chess/boardstate.h"
#include "../../include/chess/texture.h"

constexpr int window_width = 930;
constexpr int window_height = window_width * (1 + 1.0 / 8);
constexpr int board_height = window_width * 1;
constexpr int board_width = window_width * 1;
constexpr int square_height = board_height / 8;
constexpr int square_width = board_width / 8;

bool movelistcontains(unsigned int mo, unsigned int movelist[256])
{
    for (int i{0}; i < 256; i++)
    {
        if ((movelist[i] & 0b111111111111) == (mo & 0b111111111111))
        {
            //std::cout << "movelist contains mo" << std::endl;
            return true;
        }
        if (movelist[i] == 0)
        {
            break;
        }
    }
    return false;
}

int displaychess(boardstate *brd)
{

    // U1 positionarray[8][8];                         //printing is done purely based on this array . link between display module and rule module
    // brd->getarray(positionarray);

    sf::RectangleShape *pce; // 64 member shape array for piece on each square. values assigned at runtime according to positionarray[]
    pce = new sf::RectangleShape[64];

    for (int i = 0; i < 64; i++)
    { //dynamically declared to avoid stackoverflow
        pce[i].setSize(sf::Vector2f(square_width, square_height));
    }

    sf::Texture boardtexture; //loading textures from hereon.loaded from project folder. understand from the code.
    boardtexture.loadFromFile("../assets/chess/img/chessboard3.jpg");

    sf::RectangleShape board; //board texture assigned to board shape.
    board.setSize(sf::Vector2f(board_width, board_height));
    board.setTexture(&boardtexture);
    // board.setPosition(sf::Vector2f(square_width, square_height));

    sf::Texture bktexture;
    bktexture.loadFromFile("../assets/chess/img/bk.png");

    sf::Texture bptexture;
    bptexture.loadFromFile("../assets/chess/img/bp.png");

    sf::Texture bqtexture;
    bqtexture.loadFromFile("../assets/chess/img/bq.png");

    sf::Texture brtexture;
    brtexture.loadFromFile("../assets/chess/img/br.png");

    sf::Texture bbtexture;
    bbtexture.loadFromFile("../assets/chess/img/bb.png");

    sf::Texture bntexture;
    bntexture.loadFromFile("../assets/chess/img/bn.png");

    sf::Texture wktexture;
    wktexture.loadFromFile("../assets/chess/img/wk.png");

    sf::Texture wptexture;
    wptexture.loadFromFile("../assets/chess/img/wp.png");

    sf::Texture wqtexture;
    wqtexture.loadFromFile("../assets/chess/img/wq.png");

    sf::Texture wrtexture;
    wrtexture.loadFromFile("../assets/chess/img/wr.png");

    sf::Texture wbtexture;
    wbtexture.loadFromFile("../assets/chess/img/wb.png");

    sf::Texture wntexture;
    wntexture.loadFromFile("../assets/chess/img/wn.png");

    sf::Texture background1; //backgrounds and texts
    background1.loadFromFile("../assets/chess/img/background1.jpg");

    sf::Texture wwtexture, bwtexture, smtexture, appdrawtexture, resigntexture, drawtexture;
    wwtexture.loadFromFile("../assets/chess/img/whitewins.jpg");
    bwtexture.loadFromFile("../assets/chess/img/blackwins.jpg");
    smtexture.loadFromFile("../assets/chess/img/stalemate.jpg");
    appdrawtexture.loadFromFile("../assets/chess/img/appealdraw.jpg");
    resigntexture.loadFromFile("../assets/chess/img/resign.jpg");
    drawtexture.loadFromFile("../assets/chess/img/draw.jpg");
    sf::RectangleShape whitewins, blackwins, stalemate, appealdraw, resign, draw;
    whitewins.setTexture(&wwtexture);
    //whitewins.setFillColor(sf::Color(255, 255, 255, 130));
    whitewins.setSize(sf::Vector2f(window_width * (6.0 / 8), window_width / 8));
    whitewins.setPosition(window_width / 8, window_width);
    blackwins.setTexture(&bwtexture);
    //blackwins.setFillColor(sf::Color(255, 255, 255, 130));
    blackwins.setSize(sf::Vector2f(window_width * (6.0 / 8), window_width / 8));
    blackwins.setPosition(window_width / 8, window_width);
    stalemate.setTexture(&smtexture);
    //stalemate.setFillColor(sf::Color(255, 255, 255, 130));
    stalemate.setSize(sf::Vector2f(window_width * (6.0 / 8), window_width / 8));
    stalemate.setPosition(window_width / 8, window_width);
    appealdraw.setTexture(&appdrawtexture);
    appealdraw.setSize(sf::Vector2f(window_width / 2, window_width / 16));
    appealdraw.setPosition(0, window_width * (1 + 1.0 / 32));
    resign.setTexture(&resigntexture);
    resign.setSize(sf::Vector2f(window_width / 2, window_width / 16));
    resign.setPosition(window_width / 4, window_width * (1 + 1.0 / 32));
    draw.setTexture(&drawtexture);
    draw.setSize(sf::Vector2f(window_width * (6.0 / 8), window_width / 8));
    draw.setPosition(window_width / 8, window_width);

    sf::Texture frametexture;
    frametexture.loadFromFile("../assets/chess/img/frame.png");
    sf::RectangleShape framefrom, frameto;
    framefrom.setTexture(&frametexture);
    framefrom.setSize(sf::Vector2f(square_width, square_height));
    frameto.setTexture(&frametexture);
    frameto.setSize(sf::Vector2f(square_width, square_height));

    sf::RectangleShape background;
    background.setTexture(&background1);
    background.setSize(sf::Vector2f(window_width, window_height));
    background.setFillColor(sf::Color(150, 150, 255, 128));

    sf::SoundBuffer move; //move sound effect.
    move.loadFromFile("../assets/chess/sound/movesound.wav");
    sf::Sound movesound{move};

    sf::Font font;
    font.loadFromFile("../assets/chess/font/pricedown.otf");

    int x1, y1, x2, y2;                                                                                                   //to store mouse coordinates later.
    bool generate_flag{false}, draw_bit{true}, mouse_bit{true}, gamestarted{false}, playerselect{true}, breakloop{false}; //'switches' to activate or deactivate certain functions based on a sequence of events when pieces need to be moved
    bool stalemateflag{false}, blackwinsflag{false}, whitewinsflag{false};
    unsigned int mo{0};
    int posindex{0};
    unsigned int movelist[256];

    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "CHESS", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(30);
    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (!gamestarted)
        {

            sf::RectangleShape choose0, choose1;
            choose0.setSize(sf::Vector2f(window_width / 4, window_width / 4));
            choose0.setPosition(sf::Vector2f(0 + window_width / 8, (window_width / 2)));
            choose0.setTexture(&wktexture);
            choose1.setSize(sf::Vector2f((window_width / 4), (window_width / 4)));
            choose1.setPosition(sf::Vector2f(window_width / 2 + window_width / 8, window_width / 2));
            choose1.setTexture(&bktexture);

            sf::RectangleShape background;
            background.setSize(sf::Vector2f(board_width, window_height));
            background.setTexture(&background1);
            background.setFillColor(sf::Color(150, 150, 255, 128));

            sf::Text text;
            sf::Font font;
            font.loadFromFile("../assets/chess/font/pricedown.otf");
            text.setFont(font);
            text.setString("CHOOSE SIDE");
            text.setFillColor(sf::Color::White);
            text.setCharacterSize(window_width / 8);
            text.setPosition(window_width * (3.0 / 16), window_width / 8);

            window.clear();
            window.draw(background);
            window.draw(text);
            window.draw(choose0);
            window.draw(choose1);
            window.display();
            while (1)
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    if (sf::Mouse::getPosition(window).x < window_width / 2 && sf::Mouse::getPosition(window).y > window_width / 2)
                    {
                        playerselect = true;
                        gamestarted = true;
                        break;
                    }
                    if (sf::Mouse::getPosition(window).x > window_width / 2 && sf::Mouse::getPosition(window).y > window_width / 2)
                    {
                        playerselect = false;
                        mo = minimaxinit(0, *brd, brd->getturn());
                        brd->makemove(mo);
                        movesound.play();
                        gamestarted = true;
                        break;
                    }
                }
            }
        }
        window.clear();
        //window.draw(background);
        window.draw(board);

        window.draw(resign);

        if (mo)
        {
            framefrom.setPosition(gfromj(mo) * square_width, (7 - gfromi(mo)) * square_width);
            frameto.setPosition(gtoj(mo) * square_width, (7 - gtoi(mo)) * square_width);
            window.draw(framefrom);
            window.draw(frameto);
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouse_bit)
        { //when mouse is pressed first.
            sf::Vector2i mousepos = sf::Mouse::getPosition(window);
            x1 = mousepos.x / square_width; //x1 and y1 for where mouse is clicked
            y1 = mousepos.y / square_height;

            if (y1 == 8)
            {

                if ((x1 > 2) && (x1 <= 6))
                {
                    std::cout << "resigned" << std::endl;
                    while (1)
                    {
                        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        {
                            if (playerselect)
                            {
                                window.draw(blackwins);
                                window.display();
                                std::cout << "blackwins drawn" << std::endl;
                                break;
                            }
                            else
                            {
                                window.draw(whitewins);
                                window.display();
                                std::cout << "whitewins drawn" << std::endl;
                                break;
                            }
                        }
                    }
                    while (1)
                    {
                        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        {
                            std::cout << "value returned from resign" << std::endl;
                            RunGame game;
                            window.close();
                            game.runGame();
                            return 0;
                        }
                    }
                }
            }

            draw_bit = false;
            mouse_bit = false; //made false to enable next mouse functions
        }
        if (event.type == sf::Event::MouseMoved && !mouse_bit)
        { //when mouse is held
            pce[x1 * 8 + y1].setPosition(sf::Vector2f(event.mouseMove.x - square_height / 2, event.mouseMove.y - square_width / 2));
        }
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && !mouse_bit)
        { //when mouse is released
            sf::Vector2i mousepos{sf::Mouse::getPosition(window)};
            x2 = mousepos.x / square_width; //x2 and y2 for where mouse is released
            y2 = mousepos.y / square_height;
            mo = 0;
            setmove(mo, 7 - y1, x1, 7 - y2, x2, 0, 0, 0, 0, 0);
            if (movelistcontains(mo, movelist))
            {

                brd->makemove(mo);
                generate_flag = true;
                movesound.play();
                // pce[x1 * 8 + y1].setPosition(sf::Vector2f((x2 * square_width), (y2 * square_height)));
                // window.draw(pce[x1 * 8 + y1]);
            }
            else
            {
                mo = 0;
            }
            draw_bit = true;  //draw bit set to true to reassign and reposition according to new values.
            mouse_bit = true; //made true at end of mouse clicking cycle
        }

        for (int i = 0; i < 8; i++)
        { //to draw (and reassign if pieces are moved).
            for (int j = 0; j < 8; j++)
            {
                posindex = i * 8 + j;
                if (piece(brd->getsquare((7 - j), i)) != empty)
                {

                    if (draw_bit)
                    { //draw bit controls whether to reassign or simply draw as previous.

                        switch (piece(brd->getsquare((7 - j), i)))
                        {
                        case (r):
                            pce[posindex].setTexture(&brtexture);
                            break;
                        case (n):
                            pce[posindex].setTexture(&bntexture);
                            break;
                        case (b):
                            pce[posindex].setTexture(&bbtexture);
                            break;
                        case (q):
                            pce[posindex].setTexture(&bqtexture);
                            break;
                        case (k):
                            pce[posindex].setTexture(&bktexture);
                            break;
                        case (p):
                            pce[posindex].setTexture(&bptexture);
                            break;
                        case (R):
                            pce[posindex].setTexture(&wrtexture);
                            break;
                        case (N):
                            pce[posindex].setTexture(&wntexture);
                            break;
                        case (B):
                            pce[posindex].setTexture(&wbtexture);
                            break;
                        case (Q):
                            pce[posindex].setTexture(&wqtexture);
                            break;
                        case (K):
                            pce[posindex].setTexture(&wktexture);
                            break;
                        case (P):
                            pce[posindex].setTexture(&wptexture);
                            break;
                            // case(0):
                            //     piece[i].setTexture(&bptexture);
                        default:
                            break;
                        }

                        pce[posindex].setPosition(sf::Vector2f((i * square_width), (j * square_height))); //position set/reset according to index in RectangleShape array
                    }
                    window.draw(pce[posindex]);
                }
            }
        }

        window.display();

        if (draw_bit)
        {
            draw_bit = false;
            for (int i{0}; i < 256; i++)
            {
                movelist[i] = 0;
            }
            brd->getmove(movelist);

            if (movelist[0] == 0 && !brd->ischecked(playerselect))
            {
                window.display();
                window.draw(stalemate);
                window.display();
                while (1)
                {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouse_bit)
                    {
                        RunGame game;
                        window.close();
                        game.runGame();
                        return 0;
                    }
                }
            }
            if (movelist[0] == 0 && playerselect)
            {
                window.display();
                window.draw(blackwins);
                window.display();
                brd->printboard();
                while (1)
                {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouse_bit)
                    {
                        RunGame game;
                        window.close();
                        game.runGame();
                        return 0;
                    }
                }
            }
            if (movelist[0] == 0 && !playerselect)
            {
                window.display();
                window.draw(whitewins);
                window.display();
                while (1)
                {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouse_bit)
                    {
                        RunGame game;
                        window.close();
                        game.runGame();
                        return 0;
                    }
                }
            }
        }
        if (generate_flag)
        {
            mo = minimaxinit(0, *brd, brd->getturn());

            if (mo == 0b0001000000000000 && playerselect)
            {
                window.display();
                window.draw(blackwins);
                window.display();
                while (1)
                {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouse_bit)
                    {
                        RunGame game;
                        window.close();
                        game.runGame();
                        return 0;
                    }
                }
            }
            if (mo == 0b0010000000000000 && !playerselect)
            {
                window.display();
                window.draw(whitewins);
                window.display();
                while (1)
                {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouse_bit)
                    {
                        RunGame game;
                        window.close();
                        game.runGame();
                        return 0;
                    }
                }
            }
            if (mo == 0)
            {
                window.display();
                window.draw(stalemate);
                window.display();
                while (1)
                {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouse_bit)
                    {
                        RunGame game;
                        window.close();
                        game.runGame();
                        return 0;
                    }
                }
            }

            brd->makemove(mo);
            movesound.play();

            draw_bit = true;
            generate_flag = false;
        }
    }
    delete[] pce;
    RunGame game;
    window.close();
    game.runGame();
    return 0;
}
