#ifndef MULTIPLERENDERTEXTURE_H
#define MULTIPLERENDERTEXTURE_H

#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Context.hpp>

namespace sf
{

class SFML_GRAPHICS_API MultipleRenderTexture : public RenderTarget, GlResource
{
public:

    MultipleRenderTexture();
    virtual ~MultipleRenderTexture();

    bool create(unsigned int width, unsigned int height);
    bool addRenderTarget(unsigned int renderingLocation, bool useFloat = false);
    bool addDepthBuffer();
    bool addDepthStencilBuffer();
    bool removeRenderTarget(unsigned int renderingLocation);
    bool removeDepthBuffer();
    //bool removeStencilBuffer();

    bool copyDepthBuffer(MultipleRenderTexture *);
    bool copyDepthBuffer(MultipleRenderTexture *, const sf::FloatRect &sourceRect, const sf::FloatRect &targetRect);
    bool copyDepthStencilBuffer(MultipleRenderTexture *);
    bool copyDepthStencilBuffer(MultipleRenderTexture *, const sf::FloatRect &sourceRect, const sf::FloatRect &targetRect);
    bool copyBuffer(MultipleRenderTexture *, unsigned int sourceLocation, const sf::FloatRect &sourceRect,
                                             unsigned int targetLocation, const sf::FloatRect &targetRect);
    bool copyBuffer(MultipleRenderTexture *, unsigned int sourceLocation, unsigned int targetLocation);

    void setSmooth(unsigned int renderingLocation, bool smooth);
    bool isSmooth(unsigned int renderingLocation);
    void setRepeated(unsigned int renderingLocation, bool repeated);
    bool isRepeated(unsigned int renderingLocation);
    bool generateMipmap(unsigned int renderingLocation);

    bool setActive(bool active);
    void display(bool doFlush = true);

    virtual sf::Vector2u getSize() const;
    Texture* getTexture(unsigned int renderingLocation);


protected:
    std::vector<unsigned int>::iterator findRenderTarget(unsigned int renderingLocation);
    bool createFBO(unsigned int width, unsigned int height);
    bool addTargetToFBO(unsigned int renderingLocation, unsigned int);

    static unsigned int getMaxColorAttachments();

private:
    //priv::MultipleRenderTextureImpl* m_impl;
    Texture *m_textures;
    std::vector<unsigned int> m_activeTextures;
    Vector2u m_size;

    Context*     m_context;
    unsigned int m_frameBuffer;
    unsigned int m_depthBuffer;
};

}


#endif // MULTIPLERENDERTEXTURE_H
