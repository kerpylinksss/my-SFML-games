#include "pers.h"

int main()
{
    sf::RenderWindow win(sf::VideoMode(1200, 800), "Entities");

    Player guy(200.0f, 600.0f, 0.5f, 115, 95, 100.0f, 100.0f, "tiles4.png", "tiles4Rev.png", 185, 40, 600.0f);
    
    vector<int> moveFrames = { 10, 160, 280, 435, 585, 710 };
    vector<int> moveFramesRev = { 40, 165, 315, 470, 590, 740 };

    vector<int> stayFrames = { 185, 330, 470, 615, 765 };
    vector<int> stayFramesRev = { 40, 185, 330, 470, 615 };

    vector<IntRect> jumpFrames = {IntRect(760, 205, 55, 80), IntRect(635, 145, 35, 145), IntRect(475, 165, 50, 115), IntRect(325, 170, 78, 103), IntRect(170, 180, 79, 87)};
    vector<IntRect> jumpFramesRev = {IntRect(50, 205, 55, 80), IntRect(195, 145, 35, 145), IntRect(330, 165, 50, 115), IntRect(460, 170, 78, 103), IntRect(615, 180, 79, 87)};

    vector<IntRect> atackFrames = {};
    vector<IntRect> atackFramesRev = {};

    vector<IntRect> dieFrames = {};
    vector<IntRect> dieFramesRev = {};


    guy.setFrames(335, moveFrames, jumpFrames, atackFrames, dieFrames);
    guy.setFramesRev(335, moveFramesRev, jumpFramesRev, atackFramesRev, dieFramesRev);
    guy.setPersParameters(65, 105);
    guy.setStayFrames(stayFrames, stayFramesRev, 40);

    Clock clock;

    fstream fs;
    fs.open("heightOfJump.txt", fstream::in | fstream::out | fstream::app);

    while (win.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        time /= 800;
        clock.restart();

        sf::Event event;
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                win.close();
        }

        guy.Update(time, fs);

        win.clear();
        win.draw(guy.getSprite());
        win.display();
    }

    fs.close();

    return 0;
}
