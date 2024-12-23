#include "bak/spriteRenderer.hpp"

#include "bak/image.hpp"
#include "bak/palette.hpp"

#include "bak/sceneData.hpp"

#include "graphics/texture.hpp"

#include <optional>

namespace BAK {

SpriteRenderer::SpriteRenderer()
:
    mForegroundLayer{320, 200, 320, 200},
    mBackgroundLayer{320, 200, 320, 200},
    mSavedImagesLayer0{320, 200, 320, 200},
    mSavedImagesLayer1{320, 200, 320, 200},
    mSavedImagesLayerBG{320, 200, 320, 200}
{
}

void SpriteRenderer::SetColors(std::uint8_t fg, std::uint8_t bg)
{
    mForegroundColor = fg;
    mBackgroundColor = bg;
}

void SpriteRenderer::RenderTexture(
    const Graphics::Texture& texture,
    glm::ivec2 pos,
    Graphics::Texture& layer)
{
    for (int x = 0; x < static_cast<int>(texture.GetWidth()); x++)
    {
        for (int y = 0; y < static_cast<int>(texture.GetHeight()); y++)
        {
            const auto pixelPos = pos + glm::ivec2{x, y};
            if (mClipRegion)
            {
                if (pixelPos.x < mClipRegion->mTopLeft.x || pixelPos.x > mClipRegion->mBottomRight.x
                    || pixelPos.y < mClipRegion->mTopLeft.y || pixelPos.y > mClipRegion->mBottomRight.y)
                {
                    continue;
                }
            }
            const auto color = texture.GetPixel(x, y);
            if (std::abs(0.0 - color.a) < .0001) continue;
            if (pixelPos.x > 320 || pixelPos.y > 200) continue;

            layer.SetPixel(
                pixelPos.x,
                pixelPos.y,
                color);
        }
    }
}

void SpriteRenderer::RenderSprite(
    BAK::Image sprite,
    const BAK::Palette& palette,
    glm::ivec2 pos,
    bool flipped,
    Graphics::Texture& layer)
{
    for (int x = 0; x < static_cast<int>(sprite.GetWidth()); x++)
    {
        for (int y = 0; y < static_cast<int>(sprite.GetHeight()); y++)
        {
            const auto pixelPos = pos + glm::ivec2{
                flipped ? sprite.GetWidth() - x : x,
                y};
            if (mClipRegion)// && !background)
            {
                if (pixelPos.x < mClipRegion->mTopLeft.x || pixelPos.x > mClipRegion->mBottomRight.x
                    || pixelPos.y < mClipRegion->mTopLeft.y || pixelPos.y > mClipRegion->mBottomRight.y)
                {
                    continue;
                }
            }
            else if (pixelPos.x < 0 || pixelPos.x > 320
                    || pixelPos.y < 0 || pixelPos.y > 200)
            {
                continue;
            }
            const auto color = palette.GetColor(sprite.GetPixel(x, y));
            if (color.a == 0) continue;
            if (pixelPos.x > 320
                || pixelPos.y > 200) continue;

            layer.SetPixel(
                pixelPos.x,
                pixelPos.y,
                color);
        }
    }
}

void SpriteRenderer::DrawRect(glm::ivec2 pos, glm::ivec2 dims, const BAK::Palette& palette, Graphics::Texture& layer)
{
    for (int x = 0; x < static_cast<int>(dims.x); x++)
    {
        for (int y = 0; y < static_cast<int>(dims.y); y++)
        {
            const auto pixelPos = pos + glm::ivec2{x, y};
            const auto color = palette.GetColor(mForegroundColor);
            if (color.a == 0) continue;
            if (pixelPos.x > 320
                || pixelPos.y > 200) continue;

            layer.SetPixel(
                pixelPos.x,
                pixelPos.y,
                color);
        }
    }
}

void SpriteRenderer::Clear()
{
    mForegroundLayer = Graphics::Texture{320, 200, 320, 200};
    mBackgroundLayer = Graphics::Texture{320, 200, 320, 200};
    mSavedImagesLayer0 = Graphics::Texture{320, 200, 320, 200};
    mSavedImagesLayer1 = Graphics::Texture{320, 200, 320, 200};
    mSavedImagesLayerBG = Graphics::Texture{320, 200, 320, 200};
}

Graphics::Texture& SpriteRenderer::GetForegroundLayer()
{
    return mForegroundLayer;
}

Graphics::Texture& SpriteRenderer::GetBackgroundLayer()
{
    return mBackgroundLayer;
}

Graphics::Texture& SpriteRenderer::GetSavedImagesLayerBG()
{
    return mSavedImagesLayerBG;
}

Graphics::Texture& SpriteRenderer::GetSavedImagesLayer0()
{
    return mSavedImagesLayer0;
}

Graphics::Texture& SpriteRenderer::GetSavedImagesLayer1()
{
    return mSavedImagesLayer1;
}

void SpriteRenderer::SetClipRegion(BAK::ClipRegion clipRegion)
{
    mClipRegion = clipRegion;
}

void SpriteRenderer::ClearClipRegion()
{
    mClipRegion.reset();
}

Graphics::Texture SpriteRenderer::SaveImage(glm::ivec2 pos, glm::ivec2 dims, unsigned layer)
{
    auto image = Graphics::Texture{
        static_cast<unsigned>(dims.x),
        static_cast<unsigned>(dims.y),
        static_cast<unsigned>(dims.x),
        static_cast<unsigned>(dims.y)};
    for (int x = 0; x < dims.x; x++)
    {
        for (int y = 0; y < dims.y; y++)
        {
            image.SetPixel(x, y, mForegroundLayer.GetPixel(x + pos.x, y + pos.y));
        }
    }

    RenderTexture(image, pos, GetSaveLayer(layer));
    return image;
}

Graphics::Texture& SpriteRenderer::GetSaveLayer(unsigned layer)
{
    if (layer == 0)
    {
        return mSavedImagesLayer0;
    }
    else if (layer == 1)
    {
        return mSavedImagesLayer1;
    }
    else if (layer == 2)
    {
        return mSavedImagesLayerBG;
    }
    assert(false);
    return mSavedImagesLayer0;
}

}
