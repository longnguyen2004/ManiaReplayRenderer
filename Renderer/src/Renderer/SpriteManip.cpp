#include "SpriteManip.hpp"

void setOrigin(sf::Sprite &sprite, HorizPos horiz, VertPos vert)
{
    auto [left, top, width, height] = sprite.getTextureRect();
    sf::Vector2f origin;
    switch (horiz)
    {
    case HorizPos::LEFT:
        origin.x = 0.0;
        break;

    case HorizPos::CENTER:
        origin.x = width / 2.0;
        break;

    case HorizPos::RIGHT:
        origin.x = width;
        break;
    }

    switch (vert)
    {
    case VertPos::TOP:
        origin.y = 0.0;
        break;

    case VertPos::CENTER:
        origin.y = height / 2.0;
        break;

    case VertPos::BOTTOM:
        origin.y = height;
        break;
    }
    sprite.setOrigin(origin);
}

void scaleWidth(sf::Sprite &sprite, float width, bool preserveAspectRatio)
{
    float factor = width / sprite.getTextureRect().width;
    if (preserveAspectRatio)
        sprite.setScale(factor, factor);
    else
        sprite.setScale(factor, 1);
}

void scaleHeight(sf::Sprite &sprite, float height, bool preserveAspectRatio)
{
    float factor = height / sprite.getTextureRect().height;
    if (preserveAspectRatio)
        sprite.setScale(factor, factor);
    else
        sprite.setScale(1, factor);
}