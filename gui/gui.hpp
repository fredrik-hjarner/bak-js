#pragma once

#include "graphics/glm.hpp"

#include <glm/glm.hpp>

#include <functional>
#include <vector>
#include <unordered_map>

namespace Gui {

struct GuiElement
{
    GuiElement(
        bool pressed,
        bool highlighted,
        unsigned image,
        unsigned pressedImage,
        glm::vec3 position,
        glm::vec3 dims,
        glm::vec3 scale,
        std::function<void()>&& released)
    :
        mPressed{pressed},
        mHighlighted{highlighted},
        mImage{image},
        mPressedImage{pressedImage},
        mPosition{position},
        mDims{dims},
        mScale{scale},
        mReleasedCallback{std:move(released)}
    {
        assert(mReleasedCallback);
    }

    bool Within(glm::vec3 click)
    {
        return Graphics::PointWithinRectangle(
            glm::vec2{click},
            glm::vec2{mPosition},
            glm::vec2{mDims});
    }

    void MousePress(glm::vec3 click)
    {
        if (Within(click))
        {
            mPressed = true;
        }
    }

    void MouseRelease(glm::vec3 click)
    {
        mPressed = false;
        if (Within(click))
            std::invoke(mReleasedCallback);
    }

    void MouseMoved(glm::vec3 pos)
    {
        if (Within(pos))
            mHighlighted = true;
        else
            mHighlighted = false;
    }

    bool mPressed;
    bool mHighlighted;
    unsigned mImage;
    unsigned mPressedImage;
    glm::vec3 mPosition;
    glm::vec3 mDims;
    glm::vec3 mScale;
    const std::function<void()> mReleasedCallback;
};

class Frame
{
public:
    Frame(glm::vec3 pos, glm::vec3 dim)
    :
        mPosition{pos},
        mDims{dim},
        mChildren{}
    {
    }

    void MousePress(glm::vec3 click)
    {
        for (auto& c : mChildren)
            c.MousePress(click - mPosition);
    }

    void MouseRelease(glm::vec3 click)
    {
        for (auto& c : mChildren)
            c.MouseRelease(click - mPosition);
    }

    void MouseMoved(glm::vec3 pos)
    {
        for (auto& c : mChildren)
            c.MouseMoved(pos - mPosition);
    }

    void Render()
    {

    }

    glm::vec3 mPosition;
    glm::vec3 mDims;
    std::vector<GuiElement> mChildren;
};

class ScreenFactory
{
    
};

class GDSScreen
{
    Graphics::TextureStore mTextures;
    std::unordered_map<unsigned, unsigned> mSlotToOffset;

};

}



//for (unsigned i = 0; i < request.GetSize(); i++)
    //{
    //    auto data = request.GetRequestData(i);
    //    switch (data.widget)
    //    {
    //    case REQ_USERDEFINED:
    //    {
    //        if (data.action != 1) break;
    //        int x = data.xpos + request.GetRectangle().GetXPos() + request.GetXOff();
    //        int y = data.ypos + request.GetRectangle().GetYPos() + request.GetYOff();
    //        //elements.emplace_back(data.action, false, data.image + 1, data.image + 1, glm::vec3{x, y, 0}, glm::vec3{data.width, data.height, 0});
    //    }
    //        break;
    //    case REQ_IMAGEBUTTON:
    //    {
    //        int x = data.xpos + request.GetRectangle().GetXPos() + request.GetXOff();
    //        int y = data.ypos + request.GetRectangle().GetYPos() + request.GetYOff();
    //        elements.emplace_back(data.action, false, data.image + off, data.image + off2, glm::vec3{x, y, 0}, glm::vec3{data.width, data.height, 0});
    //    }
    //        break;
    //    default:
    //        logger.Info() << "Unhandled: " << i << "\n";
    //        break;
    //    }
    //}
