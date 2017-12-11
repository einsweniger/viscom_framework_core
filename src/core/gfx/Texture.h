/**
 * @file   Texture.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.12.15
 *
 * @brief  Declaration of a 2D texture resource class.
 */

#pragma once

#include "core/main.h"
#include "core/open_gl.h"
#include "core/resources/Resource.h"

namespace viscom {

    class ApplicationNodeInternal;
    typedef unsigned char cType;
    /** Describes the format of a texture. */
    struct TextureDescriptor
    {
        TextureDescriptor(unsigned int btsPP, GLint intFmt, GLenum fmt, GLenum tp, int w, int h, int c) noexcept : bytesPP_(btsPP),
                                                                                              internalFormat_(intFmt),
                                                                                              format_(fmt), type_(tp),
                                                                                              width(w), height(h),
                                                                                              channels(c)
        {
        };

        /** Holds the bytes per pixel of the format. */
        unsigned int bytesPP_;
        /** Holds the internal format. */
        GLint internalFormat_;
        /** Holds the format. */
        GLenum format_;
        /** Holds the type. */
        GLenum type_;
        /** image width. */
        int width;
        /** image height. */
        int height;
        /** image channels. */
        int channels;
        /* returns length of data array*/
		[[deprecated("There will  be soon replaced.")]]
        int length() const { return width * height * channels; }
    };

    /**
    * Helper class for loading an OpenGL texture from file.
    */
    class Texture final : public Resource
    {
    public:
        Texture(const std::string& texFilename, ApplicationNodeInternal* node, bool useSRGB = true);
		[[deprecated("This will be soon replaced.")]]
    	Texture(const std::string& texId, ApplicationNodeInternal* node, const TextureDescriptor info, std::vector<cType> img_data);
        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;
        Texture(Texture&&) noexcept;
        Texture& operator=(Texture&&) noexcept;
        virtual ~Texture() noexcept override;
        TextureDescriptor getDescriptor() const { return descriptor_; }
        /** Returns the size of the texture. */
        glm::uvec2 getDimensions() const noexcept { return glm::uvec2(descriptor_.width, descriptor_.height); }
        /** Returns the OpenGL texture id. */
        GLuint getTextureId() const noexcept { return textureId_; }
        std::vector<cType> getImageData() const { return img_data_; }
        cType* data() { return img_data_.data(); }

    private:
        void LoadTextureLDR(const std::string& filename, bool useSRGB);
        void LoadTextureHDR(const std::string& filename);
        std::tuple<int, int> FindFormat(const std::string& filename, int imgChannels, bool useSRGB = false) const;

        /** Holds the OpenGL texture id. */
        GLuint textureId_;
        /** Holds the texture descriptor. */
        TextureDescriptor descriptor_;
        std::vector<cType> img_data_;
    };
}
