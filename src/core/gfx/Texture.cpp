﻿/**
 * @file   Texture.cpp
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.12.15
 *
 * @brief  Implementation of the texture resource class.
 */

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "core/ApplicationNodeInternal.h"
#include "core/resources/ResourceManager.h"
#include <stb_image.h>

namespace viscom {

    /**
     * Constructor, creates a texture from file.
     * @param texFilename the filename of the texture file.
     */
    Texture::Texture(const std::string& texFilename, ApplicationNodeInternal* node, bool useSRGB) :
        Resource(texFilename, node),
        textureId_{ 0 },
        descriptor_{ 3, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE },
        width_{ 0 },
        height_{ 0 }
    {
        auto fullFilename = FindResourceLocation(texFilename);

        stbi_set_flip_vertically_on_load(1);

        // Bind Texture and Set Filtering Levels
        glGenTextures(1, &textureId_);
        glBindTexture(GL_TEXTURE_2D, textureId_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (stbi_is_hdr(fullFilename.c_str()) != 0) LoadTextureHDR(fullFilename);
        else LoadTextureLDR(fullFilename, useSRGB);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    /**
     *  Move-constructor.
     *  @param rhs the object to copy.
     */
    Texture::Texture(Texture&& rhs) noexcept :
        Resource(std::move(rhs)),
        textureId_{ std::move(rhs.textureId_) },
        descriptor_{ std::move(rhs.descriptor_) },
        width_{ std::move(rhs.width_) },
        height_{ std::move(rhs.height_) }
    {
        rhs.textureId_ = 0;
    }

    /**
     *  Move-assignment operator.
     *  @param rhs the object to assign.
     *  @return reference to this object.
     */
    Texture& Texture::operator=(Texture&& rhs) noexcept
    {
        if (this != &rhs) {
            this->~Texture();
            Resource* tRes = this;
            *tRes = static_cast<Resource&&>(std::move(rhs));
            textureId_ = std::move(rhs.textureId_);
            descriptor_ = std::move(rhs.descriptor_);
            width_ = std::move(rhs.width_);
            height_ = std::move(rhs.height_);
            rhs.textureId_ = 0;
        }
        return *this;
    }

    /** Destructor. */
    Texture::~Texture() noexcept
    {
        if (textureId_ != 0) {
            glBindTexture(GL_TEXTURE_2D, 0);
            glDeleteTextures(1, &textureId_);
            textureId_ = 0;
        }
    }

    void Texture::LoadTextureLDR(const std::string& filename, bool useSRGB)
    {
        auto imgWidth = 0, imgHeight = 0, imgChannels = 0;
        auto image = stbi_load(filename.c_str(), &imgWidth, &imgHeight, &imgChannels, 0);
        if (!image) {
            LOG(WARNING) << "Failed to load texture (" << filename << ").";
            throw resource_loading_error(filename, "Failed to load texture.");
        }

        width_ = imgWidth;
        height_ = imgHeight;
        descriptor_.type_ = GL_UNSIGNED_BYTE;
        std::tie(descriptor_.bytesPP_, descriptor_.internalFormat_, descriptor_.format_) = FindFormatLDR(filename, imgChannels, useSRGB);
        glTexImage2D(GL_TEXTURE_2D, 0, descriptor_.internalFormat_, imgWidth, imgHeight, 0, descriptor_.format_, descriptor_.type_, image);

        stbi_image_free(image);
    }

    void Texture::LoadTextureHDR(const std::string& filename)
    {
        auto imgWidth = 0, imgHeight = 0, imgChannels = 0;
        auto image = stbi_loadf(filename.c_str(), &imgWidth, &imgHeight, &imgChannels, 0);
        if (!image) {
            LOG(WARNING) << "Failed to load texture (" << filename << ").";
            throw resource_loading_error(filename, "Failed to load texture.");
        }

        width_ = imgWidth;
        height_ = imgHeight;
        descriptor_.type_ = GL_FLOAT;
        std::tie(descriptor_.bytesPP_, descriptor_.internalFormat_, descriptor_.format_) = FindFormatHDR(filename, imgChannels);
        glTexImage2D(GL_TEXTURE_2D, 0, descriptor_.internalFormat_, imgWidth, imgHeight, 0, descriptor_.format_, descriptor_.type_, image);

        stbi_image_free(image);
    }

    std::tuple<unsigned int, int, int> Texture::FindFormatLDR(const std::string& filename, int imgChannels, bool useSRGB) const
    {
        auto bytesPP = 4U;
        auto internalFmt = GL_RGBA8;
        auto fmt = GL_RGBA;
        switch (imgChannels) {
        case 1: bytesPP = 1U; internalFmt = GL_R8; fmt = GL_RED; break;
        case 2: bytesPP = 2U; internalFmt = GL_RG8; fmt = GL_RG; break;
        case 3: bytesPP = 3U; internalFmt = useSRGB ? GL_SRGB8 : GL_RGB8; fmt = GL_RGB; break;
        case 4: bytesPP = 4U; internalFmt = useSRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8; fmt = GL_RGBA; break;
        default:
            LOG(WARNING) << L"Invalid number of texture channels (" << imgChannels << ").";
            throw resource_loading_error(filename, "Invalid number of texture channels.");
        }
        return std::make_tuple(bytesPP, internalFmt, fmt);
    }

    std::tuple<unsigned int, int, int> Texture::FindFormatHDR(const std::string& filename, int imgChannels) const
    {
        auto bytesPP = 16U;
        auto internalFmt = GL_RGBA32F;
        auto fmt = GL_RGBA;
        switch (imgChannels) {
        case 1: bytesPP = 4U; internalFmt = GL_R32F; fmt = GL_RED; break;
        case 2: bytesPP = 8U; internalFmt = GL_RG32F; fmt = GL_RG; break;
        case 3: bytesPP = 12U; internalFmt = GL_RGB32F; fmt = GL_RGB; break;
        case 4: bytesPP = 16U; internalFmt = GL_RGBA32F; fmt = GL_RGBA; break;
        default:
            LOG(WARNING) << L"Invalid number of texture channels (" << imgChannels << ").";
            throw resource_loading_error(filename, "Invalid number of texture channels.");
        }
        return std::make_tuple(bytesPP, internalFmt, fmt);
    }
}
