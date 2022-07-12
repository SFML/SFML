////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/SpriteBatch.hpp>
#include <SFML/System/Err.hpp>

#include <algorithm>
#include <cassert>
#include <numeric>

namespace sf
{
////////////////////////////////////////////////////////////
void SpriteBatch::setBatchMode(BatchMode batchMode)
{
    m_batchMode      = batchMode;
    m_updateRequired = true;
}


////////////////////////////////////////////////////////////
void SpriteBatch::batch(const Batchable& drawable, float depth)
{
    drawable.batch(*this, depth);
}


////////////////////////////////////////////////////////////
void SpriteBatch::batch(const Vertex*    vertices,
                        std::size_t      count,
                        PrimitiveType    type,
                        const Texture*   texture,
                        const Transform& transform,
                        float            depth)
{
    const bool unsupportedType = type != PrimitiveType::TriangleFan && type != PrimitiveType::TriangleStrip &&
                                 type != PrimitiveType::Triangles;

    if (unsupportedType)
    {
        err() << "SpriteBatch supports only triangle-based primitive types.";
        return;
    }

    // Anything to batch?
    if (count == 0)
        return;

    // Split into triangles
    switch (type)
    {
        case PrimitiveType::TriangleStrip:
            for (std::size_t i = 2; i < count; i++)
                pushTriangle(vertices[i - 2], vertices[i - 1], vertices[i], transform, texture, depth);
            break;

        case PrimitiveType::TriangleFan:
            for (std::size_t i = 2; i < count; i++)
                pushTriangle(vertices[0], vertices[i - 1], vertices[i], transform, texture, depth);
            break;

        case PrimitiveType::Triangles:
            for (std::size_t i = 2; i < count; i += 3)
                pushTriangle(vertices[i - 2], vertices[i - 1], vertices[i], transform, texture, depth);
            break;

        default:
            err() << "A non-triangle primitive type was encountered when decomposing into triangles." << std::endl;
            assert(false);
            break;
    }
}


////////////////////////////////////////////////////////////
void SpriteBatch::draw(RenderTarget& target, const RenderStates& states) const
{
    if (m_updateRequired)
    {
        updateBatch();
        m_updateRequired = false;
    }

    RenderStates statesCopy = states;

    std::size_t startTriangle = 0;
    for (const auto& batch : m_batches)
    {
        statesCopy.texture = batch.texture;
        target.draw(&m_vertices[startTriangle], batch.vertexCount, PrimitiveType::Triangles, statesCopy);
        startTriangle += batch.vertexCount;
    }
}


////////////////////////////////////////////////////////////
void SpriteBatch::pushTriangle(const Vertex&    a,
                               const Vertex&    b,
                               const Vertex&    c,
                               const Transform& transform,
                               const Texture*   texture,
                               float            depth)
{
    m_triangles.emplace_back(texture, depth);

    m_unsortedVertices.emplace_back(transform * a.position, a.color, a.texCoords);
    m_unsortedVertices.emplace_back(transform * b.position, b.color, b.texCoords);
    m_unsortedVertices.emplace_back(transform * c.position, c.color, c.texCoords);

    m_updateRequired = true;
}


////////////////////////////////////////////////////////////
void SpriteBatch::updateBatch() const
{
    if (m_batchMode == BatchMode::Deferred)
    {
        // Batch based on texture change
        std::size_t    startIndex  = 0;
        std::size_t    nextIndex   = 0;
        const Texture* lastTexture = nullptr;

        while (nextIndex < m_triangles.size())
        {
            const Texture* nextTexture = m_triangles[nextIndex].texture;
            if (nextTexture != lastTexture)
            {
                m_batches.emplace_back(lastTexture, (nextIndex - startIndex) * 3);
                lastTexture = nextTexture;
                startIndex  = nextIndex;
            }

            nextIndex++;
        }

        // Deal with leftovers
        if (startIndex != m_triangles.size())
            m_batches.emplace_back(lastTexture, (m_triangles.size() - startIndex) * 3);

        m_vertices = m_unsortedVertices;
    }

    else if (m_batchMode == BatchMode::TextureSort || m_batchMode == BatchMode::DepthSort)
    {
        std::vector<std::size_t> indices(m_triangles.size());

        std::iota(indices.begin(), indices.end(), 0);

        if (m_batchMode == BatchMode::TextureSort)
        {
            const auto comp = [this](const std::size_t a, const std::size_t b)
            { return m_triangles[a].texture < m_triangles[b].texture; };

            std::stable_sort(indices.begin(), indices.end(), comp);
        }

        else
        {
            const auto comp = [this](const std::size_t a, const std::size_t b)
            {
                if (m_triangles[a].depth != m_triangles[b].depth)
                    return m_triangles[a].depth > m_triangles[b].depth;
                else
                    return m_triangles[a].texture < m_triangles[b].texture;
            };

            std::stable_sort(indices.begin(), indices.end(), comp);
        }

        // Create the array of sorted vertices
        // We need them sorted so that we feed chunks to RenderTarget
        m_vertices.resize(m_unsortedVertices.size());

        for (std::size_t i = 0; i < indices.size(); i++)
        {
            const std::size_t newPos = i * 3;
            const std::size_t oldPos = indices[i] * 3;
            m_vertices[newPos]       = m_unsortedVertices[oldPos];
            m_vertices[newPos + 1]   = m_unsortedVertices[oldPos + 1];
            m_vertices[newPos + 2]   = m_unsortedVertices[oldPos + 2];
        }

        // Generate batches
        std::size_t    startIndex  = 0;
        std::size_t    nextIndex   = 0;
        const Texture* lastTexture = nullptr;

        while (nextIndex < m_triangles.size())
        {
            const Texture* nextTexture = m_triangles[indices[nextIndex]].texture;

            if (nextTexture != lastTexture)
            {
                m_batches.emplace_back(lastTexture, (nextIndex - startIndex) * 3);
                lastTexture = nextTexture;
                startIndex  = nextIndex;
            }

            nextIndex++;
        }

        // Deal with leftovers
        if (startIndex != m_triangles.size())
            m_batches.emplace_back(m_triangles[indices[startIndex]].texture, (m_triangles.size() - startIndex) * 3);
    }
}


////////////////////////////////////////////////////////////
void SpriteBatch::clear()
{
    m_unsortedVertices.clear();
    m_triangles.clear();
    m_vertices.clear();
    m_batches.clear();

    m_updateRequired = false;
}

} // namespace sf
