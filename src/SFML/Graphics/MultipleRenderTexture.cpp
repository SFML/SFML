

#include <SFML/Graphics/MultipleRenderTexture.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#include <SFML/System/Err.hpp>



namespace sf
{

unsigned int MultipleRenderTexture::getMaxColorAttachments()
{
    GLint maxAttach = 0;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &maxAttach);
    return maxAttach;
}

MultipleRenderTexture::MultipleRenderTexture() :
    //m_context    (NULL),
    m_frameBuffer(0),
    m_depthBuffer(0)//,
   // m_stencilBuffer(0)
{
    m_size = Vector2u(0,0);
    m_textures = NULL;
}

MultipleRenderTexture::~MultipleRenderTexture()
{

    //if(m_context != NULL)
    {
       // m_context->setActive(true);

        if(m_textures != NULL)
            delete m_textures;

        removeDepthBuffer();

        if (m_frameBuffer)
        {
            GLuint frameBuffer = static_cast<GLuint>(m_frameBuffer);
            glCheck(GLEXT_glDeleteFramebuffers(1, &frameBuffer));
        }

        //delete m_context;
    }
}


bool MultipleRenderTexture::create(unsigned int width, unsigned int height)
{
    m_size = Vector2u(width, height);

  //  if(m_context != NULL)
    {
      //  m_context->setActive(true);
        removeDepthBuffer();

        if (m_frameBuffer)
        {
            GLuint frameBuffer = static_cast<GLuint>(m_frameBuffer);
            glCheck(GLEXT_glDeleteFramebuffers(1, &frameBuffer));
            m_frameBuffer = 0;
        }
      //  delete m_context;
    }

    //m_context = new Context();

    //m_context->setActive(true);

    if(m_textures != NULL)
        delete m_textures;

    m_activeTextures.clear();

    m_textures = new Texture[getMaxColorAttachments()];

    if(!createFBO(width,height))
        return false;

    return true;
}

bool MultipleRenderTexture::addRenderTarget(unsigned int renderingLocation, bool useFloat)
{
    std::vector<unsigned int>::iterator renderIt;
    renderIt = findRenderTarget(renderingLocation);

    if(renderIt != m_activeTextures.end())
    {
        err()<< "Impossible to add render target (location already taken)" <<std::endl;
        return false;
    }

    if(renderingLocation >= getMaxColorAttachments())
    {
        err()<< "Impossible to add render target (location not available)" <<std::endl;
        return false;
    }

    Texture* curTexture = &m_textures[renderingLocation];

    if (!curTexture->create(m_size.x, m_size.y, useFloat))
    {
        err() << "Impossible to add render texture (failed to create the target texture)" << std::endl;
        return false;
    }

    curTexture->setSmooth(false);

    /*// Create the implementation
    delete m_impl;
    if (priv::RenderTextureImplFBO::isAvailable())
    {
        // Use frame-buffer object (FBO)
        m_impl = new priv::RenderTextureImplFBO;

        // Mark the texture as being a framebuffer object attachment
        m_texture.m_fboAttachment = true;
    }
    else
    {
        // Use default implementation
        m_impl = new priv::RenderTextureImplDefault;
    }*/

    curTexture->m_fboAttachment = true;

    if (!addTargetToFBO(renderingLocation,curTexture->m_texture))
        return false;

    m_activeTextures.push_back(renderingLocation+GL_COLOR_ATTACHMENT0_EXT);

    GLEXT_glDrawBuffers(m_activeTextures.size(),m_activeTextures.data());

    return true;
}

bool MultipleRenderTexture::addDepthBuffer()
{
    if(m_depthBuffer)
        removeDepthBuffer();

    if(m_frameBuffer && !m_depthBuffer)
    {
        setActive(true);
        GLuint depth = 0;
        glCheck(GLEXT_glGenRenderbuffers(1, &depth));
        m_depthBuffer = static_cast<unsigned int>(depth);
        if (!m_depthBuffer)
        {
            err() << "Impossible to add depth buffer (failed to create the attached depth buffer)" << std::endl;
            return false;
        }
        glCheck(GLEXT_glBindRenderbuffer(GLEXT_GL_RENDERBUFFER, m_depthBuffer));
        glCheck(GLEXT_glRenderbufferStorage(GLEXT_GL_RENDERBUFFER, GLEXT_GL_DEPTH_COMPONENT, m_size.x, m_size.y));
        glCheck(GLEXT_glFramebufferRenderbuffer(GLEXT_GL_FRAMEBUFFER, GLEXT_GL_DEPTH_ATTACHMENT, GLEXT_GL_RENDERBUFFER, m_depthBuffer));
    }
}

bool MultipleRenderTexture::addDepthStencilBuffer()
{
    if(m_depthBuffer)
        removeDepthBuffer();

    if(m_frameBuffer && !m_depthBuffer)
    {
        setActive(true);
        GLuint depth = 0;
        glCheck(GLEXT_glGenRenderbuffers(1, &depth));
        m_depthBuffer = static_cast<unsigned int>(depth);
        if (!m_depthBuffer)
        {
            err() << "Impossible to add depth buffer (failed to create the attached depth buffer)" << std::endl;
            return false;
        }
        glCheck(GLEXT_glBindRenderbuffer(GLEXT_GL_RENDERBUFFER, m_depthBuffer));
        glCheck(GLEXT_glRenderbufferStorage(GLEXT_GL_RENDERBUFFER, GLEXT_GL_DEPTH24_STENCIL8, m_size.x, m_size.y));
        glCheck(GLEXT_glFramebufferRenderbuffer(GLEXT_GL_FRAMEBUFFER, GLEXT_GL_DEPTH_STENCIL_ATTACHMENT, GLEXT_GL_RENDERBUFFER, m_depthBuffer));
    }
}

bool MultipleRenderTexture::removeRenderTarget(unsigned int renderingLocation)
{
    if(m_activeTextures.size() <= 1)
    {
        err() << "Impossible to remove render texture (cannot remove all render textures)" << std::endl;
        return false;
    }

    std::vector<unsigned int>::iterator renderIt;
    renderIt = findRenderTarget(renderingLocation);
    if(renderIt == m_activeTextures.end())
    {
        err() << "Impossible to remove render texture (cannot find render texture)" << std::endl;
        return false;
    }

    m_activeTextures.erase(renderIt);
    return true;
}

bool MultipleRenderTexture::removeDepthBuffer()
{
    if (m_depthBuffer)
    {
        GLuint depthBuffer = static_cast<GLuint>(m_depthBuffer);
        glCheck(GLEXT_glDeleteFramebuffers(1, &depthBuffer));
    }
}

/*bool MultipleRenderTexture::removeStencilBuffer()
{
    if (m_stencilBuffer)
    {
        GLuint stencilBuffer = static_cast<GLuint>(m_stencilBuffer);
        glCheck(GLEXT_glDeleteFramebuffers(1, &stencilBuffer));
    }
}*/


bool MultipleRenderTexture::copyDepthBuffer(MultipleRenderTexture *source, const sf::FloatRect &sourceRect,
                                             const sf::FloatRect &targetRect)
{
    if(source != NULL)
    {
        setActive(true);
        GLEXT_glBindFramebuffer(GLEXT_GL_READ_FRAMEBUFFER, source->m_frameBuffer);
        GLEXT_glBindFramebuffer(GLEXT_GL_DRAW_FRAMEBUFFER, m_frameBuffer);
        GLEXT_glBlitFramebuffer(
            sourceRect.left, source->getSize().y-(sourceRect.height+sourceRect.top),
            sourceRect.left+sourceRect.width, source->getSize().y-sourceRect.top,
            targetRect.left, this->getSize().y-(targetRect.height+targetRect.top),
            targetRect.left+targetRect.width, this->getSize().y-targetRect.top,
             GL_DEPTH_BUFFER_BIT, GL_NEAREST
        );
        GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, m_frameBuffer);

        return true;
    }

    return false;
}

bool MultipleRenderTexture::copyDepthBuffer(MultipleRenderTexture *source)
{
    if(source != NULL)
    {
        return copyDepthBuffer(source,
                        sf::FloatRect(0,0,source->getSize().x,source->getSize().y),
                        sf::FloatRect(0,0,getSize().x,getSize().y));
    }
    return false;
}

bool MultipleRenderTexture::copyDepthStencilBuffer(MultipleRenderTexture *source, const sf::FloatRect &sourceRect,
                                             const sf::FloatRect &targetRect)
{
    if(source != NULL)
    {
        setActive(true);
        GLEXT_glBindFramebuffer(GLEXT_GL_READ_FRAMEBUFFER, source->m_frameBuffer);
        GLEXT_glBindFramebuffer(GLEXT_GL_DRAW_FRAMEBUFFER, m_frameBuffer);
        GLEXT_glBlitFramebuffer(
            sourceRect.left, source->getSize().y-(sourceRect.height+sourceRect.top),
            sourceRect.left+sourceRect.width, source->getSize().y-sourceRect.top,
            targetRect.left, this->getSize().y-(targetRect.height+targetRect.top),
            targetRect.left+targetRect.width, this->getSize().y-targetRect.top,
             GL_DEPTH_BUFFER_BIT, GL_NEAREST
        );
        GLEXT_glBlitFramebuffer(
            sourceRect.left, source->getSize().y-(sourceRect.height+sourceRect.top),
            sourceRect.left+sourceRect.width, source->getSize().y-sourceRect.top,
            targetRect.left, this->getSize().y-(targetRect.height+targetRect.top),
            targetRect.left+targetRect.width, this->getSize().y-targetRect.top,
             GL_STENCIL_BUFFER_BIT, GL_NEAREST
        );
        GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, m_frameBuffer);

        return true;
    }

    return false;
}

bool MultipleRenderTexture::copyDepthStencilBuffer(MultipleRenderTexture *source)
{
    if(source != NULL)
    {
        return copyDepthStencilBuffer(source,
                        sf::FloatRect(0,0,source->getSize().x,source->getSize().y),
                        sf::FloatRect(0,0,getSize().x,getSize().y));
    }
    return false;
}


bool MultipleRenderTexture::copyBuffer(MultipleRenderTexture *source,
                                        unsigned int sourceLocation, const sf::FloatRect &sourceRect,
                                        unsigned int targetLocation, const sf::FloatRect &targetRect)
{
    if(source != NULL)
    {
        this->setActive(true);

       // Texture* sourceTexture = source->getTexture(sourceLocation);
       // Texture* targetTexture = this->getTexture(targetLocation);


        std::vector<unsigned int>::iterator sourceIt, targetIt;
        sourceIt = source->findRenderTarget(sourceLocation);
        if(sourceIt == source->m_activeTextures.end())
        {
            err() << "Impossible to copy render texture (cannot find render texture in source)" << std::endl;
            return false;
        }
        targetIt = this->findRenderTarget(targetLocation);
        if(targetIt == this->m_activeTextures.end())
        {
            err() << "Impossible to copy render texture (cannot find render texture in target)" << std::endl;
            return false;
        }


        //if(sourceTexture != NULL && targetTexture != NULL)
        //{
            GLEXT_glBindFramebuffer(GLEXT_GL_READ_FRAMEBUFFER, source->m_frameBuffer);
            GLEXT_glBindFramebuffer(GLEXT_GL_DRAW_FRAMEBUFFER, m_frameBuffer);

            glReadBuffer(*sourceIt);
            glDrawBuffer(*targetIt);

            GLEXT_glBlitFramebuffer(
                sourceRect.left, source->getSize().y-(sourceRect.height+sourceRect.top),
                sourceRect.left+sourceRect.width, source->getSize().y-sourceRect.top,
                targetRect.left, this->getSize().y-(targetRect.height+targetRect.top),
                targetRect.left+targetRect.width, this->getSize().y-targetRect.top,
                 GL_COLOR_BUFFER_BIT, GL_NEAREST
            );
            GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, m_frameBuffer);
            GLEXT_glDrawBuffers(m_activeTextures.size(),m_activeTextures.data());
        //}

        /*targetTexture->update(*sourceTexture,targetLocation.x, targetLocation);*/
        return true;
    }

    return false;
}

bool MultipleRenderTexture::copyBuffer(MultipleRenderTexture *source, unsigned int sourceLocation, unsigned int targetLocation)
{
    if(source != NULL)
    {
       return  copyBuffer(source,
                    sourceLocation, sf::FloatRect(0,0,source->getSize().x,source->getSize().y),
                    targetLocation, sf::FloatRect(0,0,getSize().x,getSize().y));
    }
    return false;
}

void MultipleRenderTexture::setSmooth(unsigned int renderingLocation, bool smooth)
{
    std::vector<unsigned int>::iterator renderIt;
    renderIt = findRenderTarget(renderingLocation);

    if(renderIt != m_activeTextures.end())
        m_textures[*renderIt-GL_COLOR_ATTACHMENT0_EXT].setSmooth(smooth);
}

bool MultipleRenderTexture::isSmooth(unsigned int renderingLocation)
{
    std::vector<unsigned int>::iterator renderIt;
    renderIt = findRenderTarget(renderingLocation);

    if(renderIt != m_activeTextures.end())
        return m_textures[*renderIt-GL_COLOR_ATTACHMENT0_EXT].isSmooth();
    return false;
}

void MultipleRenderTexture::setRepeated(unsigned int renderingLocation, bool repeated)
{

    std::vector<unsigned int>::iterator renderIt;
    renderIt = findRenderTarget(renderingLocation);

    if(renderIt != m_activeTextures.end())
        m_textures[*renderIt-GL_COLOR_ATTACHMENT0_EXT].setRepeated(repeated);
}

bool MultipleRenderTexture::isRepeated(unsigned int renderingLocation)
{

    std::vector<unsigned int>::iterator renderIt;
    renderIt = findRenderTarget(renderingLocation);

    if(renderIt != m_activeTextures.end())
        return m_textures[*renderIt-GL_COLOR_ATTACHMENT0_EXT].isRepeated();
    return false;
}

bool MultipleRenderTexture::generateMipmap(unsigned int renderingLocation)
{

    std::vector<unsigned int>::iterator renderIt;
    renderIt = findRenderTarget(renderingLocation);

    if(renderIt != m_activeTextures.end())
        m_textures[*renderIt-GL_COLOR_ATTACHMENT0_EXT].generateMipmap();
}

bool MultipleRenderTexture::setActive(bool active)
{
    //if(m_context == NULL)
      //  return false;

    //if(!m_context->setActive(active))
        //return false;


   // return true;


    if(active)
    {
        glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, m_frameBuffer));
    } else {
        glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, 0));
    }

   return RenderTarget::setActive(active);
}

void MultipleRenderTexture::display()
{
   // if (doFlush && setActive(true))
     //   glCheck(glFlush());

    std::vector<unsigned int>::iterator it;
    for(it = m_activeTextures.begin() ; it != m_activeTextures.end() ; ++it)
    {
        m_textures[*it-GL_COLOR_ATTACHMENT0_EXT].m_pixelsFlipped = true;
        m_textures[*it-GL_COLOR_ATTACHMENT0_EXT].invalidateMipmap();
    }
}


Vector2u MultipleRenderTexture::getSize() const
{
    return m_size;
}

Texture* MultipleRenderTexture::getTexture(unsigned int renderingLocation)
{
    //this->setActive(true);
    if(renderingLocation >= getMaxColorAttachments())
    {
        err()<< "Impossible to get texture from multiple render texture (location not available)" <<std::endl;
        return NULL;
    }

    return &m_textures[renderingLocation];
}


std::vector<unsigned int>::iterator MultipleRenderTexture::findRenderTarget(unsigned int renderingLocation)
{
    std::vector<unsigned int>::iterator it;
    it = m_activeTextures.begin();

    while(it != m_activeTextures.end() && *it != renderingLocation + GL_COLOR_ATTACHMENT0_EXT)
        ++it;

    return it;
}

bool MultipleRenderTexture::createFBO(unsigned int width, unsigned int height)
{
    GLuint frameBuffer = 0;
    glCheck(GLEXT_glGenFramebuffers(1, &frameBuffer));
    m_frameBuffer = static_cast<unsigned int>(frameBuffer);
    if (!m_frameBuffer)
    {
        err() << "Impossible to create mutliple render texture (failed to create the frame buffer object)" << std::endl;
        return false;
    }
    glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, m_frameBuffer));

    RenderTarget::initialize();

    return true;
}


bool MultipleRenderTexture::addTargetToFBO(unsigned int renderingLocation, unsigned int textureId)
{
    if(renderingLocation >= getMaxColorAttachments())
    {
        err()<< "Impossible to add render target (location not available)" <<std::endl;
        return false;
    }

    setActive(true);

    glCheck(GLEXT_glFramebufferTexture2D(GLEXT_GL_FRAMEBUFFER, GLEXT_GL_COLOR_ATTACHMENT0+renderingLocation
                                         , GL_TEXTURE_2D, textureId, 0));

    // A final check, just to be sure...
    GLenum status;
    glCheck(status = GLEXT_glCheckFramebufferStatus(GLEXT_GL_FRAMEBUFFER));
    if (status != GLEXT_GL_FRAMEBUFFER_COMPLETE)
    {
        glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, 0));
        err() << "Impossible to create render texture (failed to link the target texture to the frame buffer)" << std::endl;
        return false;
    }

    return true;
}

}
