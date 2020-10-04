#include <cctype>
#include <chrono>
#include <iostream>

#ifdef _WIN32
#include <locale> // for UTF-8 trickery
#endif

#include "Clock.hpp"
#include "Map.hpp"
#include "Renderer.hpp"
#include "Skin.hpp"

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

using namespace std::chrono;
using json = nlohmann::json;

int main(int argc, char const *argv[])
{
#ifdef _WIN32
    // for UTF-8 file names to work
    std::locale::global(std::locale(".65001"));
#endif

    std::string mapFile(
        "G:/osu test/Jun Kuroda + AAAA - Ultimate Fate (Kawawa) "
        "[The Apocalypse].osu");
    std::cout << "[Main] Loading map file " << mapFile << '\n';
    Map testMap(mapFile);
    std::string skinFolder(
        "C:/Users/nghuu/AppData/Local/osu!/Skins/R Skin v3.0 (Bars)");
    Skin testSkin(skinFolder);
    std::cout << "[Main] Skin name: ";
    std::cout << "[Main] Creating window with resolution 854x480\n";
    sf::RenderWindow window(sf::VideoMode(854, 480), "Test");
    Clock clock(0.0);
    Renderer renderer(&window, &testMap, &clock);
    sf::Event event;
    auto start = steady_clock::now();
    std::size_t elapsed_time = 0;
    clock.setEpoch();
    while (window.isOpen())
    {
        if (renderer.drawNextFrame())
        {
            window.display();
            auto end = steady_clock::now();
            elapsed_time =
                duration_cast<milliseconds>(end - start).count();
        }
        if (clock.getElapsedTime() % 250 == 0)
        {
            double fps = clock.getFrameCount() * 1000.0 / elapsed_time;
            std::cout << "\r               ";
            std::cout << "\r"
                      << "FPS: " << fps;
        }
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        clock.tick(true);
    }
    std::cout << '\n';
    char input;
    std::cout << "Does it work? (Y/N)";
    do
    {
        input = std::toupper(static_cast<unsigned char>(std::cin.get()));
    } while (input != 'Y' && input != 'N');
    switch (input)
    {
    case 'Y':
        std::cout << "Yay!\n";
        break;
    case 'N':
        std::cout << "Nay :<\n";
        break;
    }
    return 0;
}