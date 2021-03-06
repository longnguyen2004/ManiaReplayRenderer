#ifndef RENDERERLIB_SPRITEMANIP_HPP
#define RENDERERLIB_SPRITEMANIP_HPP

#include <SFML/Graphics.hpp>

enum class HorizPos
{
    LEFT,
    CENTER,
    RIGHT
};

enum class VertPos
{
    TOP,
    CENTER,
    BOTTOM
};

void setOrigin(sf::Sprite &sprite,
    HorizPos horiz = HorizPos::LEFT,
    VertPos vert = VertPos::TOP);
void scaleWidth(sf::Sprite &sprite, float newWidth, bool preserveAspectRatio = true);
void scaleHeight(
    sf::Sprite &sprite, float newHeight, bool preserveAspectRatio = true);

#endif