#include "bak/coordinates.hpp"
#include "bak/constants.hpp"

namespace BAK {

std::ostream& operator<<(std::ostream& os, const GamePositionAndHeading& pah)
{
    os << "{ pos: " << pah.mPosition << ", angle: " << pah.mHeading << "}";
    return os;
}

glm::vec<2, unsigned> GetTile(
    glm::vec<2, unsigned> pos)
{
    return glm::floor(glm::cast<float>(pos)
        / glm::vec2{BAK::gTileSize});
}

GamePosition MakeGamePositionFromTileAndOffset(
    glm::vec<2, unsigned> tile,
    glm::vec<2, std::uint8_t> offset)
{
    const auto Convert = [](auto t, auto o)
    {
        return t * BAK::gTileSize + gOffsetScale * o;
    };
    
    return GamePosition{
        Convert(tile.x, offset.x),
        Convert(tile.y, offset.y)};
}

// Convert a 16 bit BAK angle to radians
glm::vec3 ToGlAngle(const Vector3D& angle)
{
    return ToGlCoord<float>(angle) 
        / static_cast<float>(0xffff)
        * 2.0f * glm::pi<float>();
}

double NormaliseRadians(double angle)
{
    if (angle > glm::pi<double>())
        return angle - (glm::pi<double>() * 2);
    else if (angle < -glm::pi<double>())
        return angle + (glm::pi<double>() * 2);
    return angle;
}

// Convert an 8 bit BAK angle to radians
glm::vec2 ToGlAngle(GameHeading heading)
{
    constexpr auto divider = static_cast<float>(0xff);
    const auto ratio = static_cast<float>(heading) / divider;
    const auto angle = ratio * 2.0f * glm::pi<float>();
    return glm::vec2{angle + glm::pi<float>(), 0};
}

}
