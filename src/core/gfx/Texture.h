/**
 * @file   Texture.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.12.15
 *
 * @brief  Declaration of a 2D texture resource class.
 */

#pragma once

#include "core/main.h"
#include "core/open_gl_fwd.h"
#include "core/resources/Resource.h"

namespace viscom {

    class ApplicationNodeInternal;

    /** Describes the format of a texture. */
    struct TextureDescriptor
    {
        TextureDescriptor(unsigned int btsPP, GLint intFmt, GLenum fmt, GLenum tp) noexcept : bytesPP_(btsPP), internalFormat_(intFmt), format_(fmt), type_(tp) {};

        /** Holds the bytes per pixel of the format. */
        unsigned int bytesPP_;
        /** Holds the internal format. */
        GLint internalFormat_;
        /** Holds the format. */
        GLenum format_;
        /** Holds the type. */
        GLenum type_;
    };

    /**
    * Helper class for loading an OpenGL texture from file.
    */
    class Texture final : public Resource
    {
    public:
        Texture(const std::string& texFilename, ApplicationNodeInternal* node, bool useSRGB = true);
        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;
        Texture(Texture&&) noexcept;
        Texture& operator=(Texture&&) noexcept;
        virtual ~Texture() noexcept override;

        /** Returns the size of the texture. */
        glm::uvec2 getDimensions() const noexcept { return glm::uvec2(width_, height_); }
        /** Returns the OpenGL texture id. */
        GLuint getTextureId() const noexcept { return textureId_; }
        /** Returns the texture descriptor. */
        const TextureDescriptor& getDescriptor() const { return descriptor_; }

    private:
        void LoadTextureLDR(const std::string& filename, bool useSRGB);
        void LoadTextureHDR(const std::string& filename);
        std::tuple<unsigned int, int, int> FindFormatLDR(const std::string& filename, int imgChannels, bool useSRGB = false) const;
        std::tuple<unsigned int, int, int> FindFormatHDR(const std::string& filename, int imgChannels) const;

        /** Holds the OpenGL texture id. */
        GLuint textureId_;
        /** Holds the texture descriptor. */
        TextureDescriptor descriptor_;

        /** Holds the width. */
        unsigned int width_;
        /** Holds the height. */
        unsigned int height_;
    };
}
