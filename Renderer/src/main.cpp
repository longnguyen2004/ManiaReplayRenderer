#include <cctype>
#include <chrono>
#include <iostream>
#include <ratio>

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

using double_ms = std::chrono::duration<double, std::milli>;

int main(int argc, char const *argv[])
{
#ifdef _WIN32
    // for UTF-8 file names to work
    std::locale::global(std::locale(".65001"));
#endif

    std::string mapFile("G:/osu test/Akasha/xi - Akasha (luyuja) [8K FEVER].osu");
    Map testMap(mapFile);
    std::cout << "[Main] Map information: \n";
    std::cout << "[Main] Title: " << testMap.getMetadata()["Title"] << '\n';
    std::cout << "[Main] Artist: " << testMap.getMetadata()["Artist"] << '\n';
    std::cout << "[Main] Base BPM: " << testMap.getBaseBPM() << '\n';
    std::string skinFolder(
        "C:/Users/nghuu/AppData/Local/osu!/Skins/R Skin v3.0 (Bars)");
    Skin testSkin(skinFolder);
    float scrollSpeed = 33.0;

    unsigned int width = 1280;
    unsigned int height = 720;
    std::cout << "[Main] Creating window with resolution " << width << 'x' << height
              << '\n';
    sf::RenderWindow window(sf::VideoMode(width, height), "Test",
        sf::Style::Titlebar | sf::Style::Close);
    Clock clock(60.0, testMap.getLeadIn());
    Renderer renderer(&window, &clock, &testMap, &testSkin, scrollSpeed);
    sf::Event event;
    double totalFrameTime = 0.0;
    auto start = steady_clock::now();
    clock.setEpoch();
    while (window.isOpen())
    {
        bool drawFrame = renderer.updateState();
        if (drawFrame)
        {
            window.display();
        }
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        clock.tick(true);
        if (drawFrame)
        {
            auto end = steady_clock::now();
            totalFrameTime += double_ms(end - start).count();
            start = end;
        }
        if (clock.getElapsedTime() % 250 == 0)
        {
            std::cout << "\r               "
                      << "\rFPS:"
                      << 1000.0 * renderer.getFrameCount() / totalFrameTime;
        }
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