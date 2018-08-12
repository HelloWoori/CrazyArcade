#include "stdafx.h"
#include "designElement.h"

DesignElement::DesignElement(Image* image)
    : GameObject(GameObjectTag::DesignElement)
    , _image(image)
    , _isAlpha(false)
    , _alpha(255)
{
}

DesignElement::DesignElement(Image * image, bool isAlpha)
    : GameObject(GameObjectTag::DesignElement)
    , _image(image)
    , _isAlpha(isAlpha)
    , _alpha(255)
{
}

DesignElement::~DesignElement()
{
}

void DesignElement::init()
{
}

void DesignElement::update()
{
}

void DesignElement::render(HDC hdc)
{
    if (_isAlpha)
    {
        _image->alphaRender(hdc, _alpha);
    }
    else
    {
        _image->render(hdc, static_cast<int>(_image->getX()), static_cast<int>(_image->getY()));
    }
}

void DesignElement::release()
{
}
