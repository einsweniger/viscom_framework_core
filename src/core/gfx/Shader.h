/**
 * @file   Shader.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.12.14
 *
 * @brief  Declaration of the shader helper class.
 */

#pragma once

#include "core/main.h"
#include "core/open_gl_fwd.h"

namespace viscom {

    class ApplicationNodeInternal;

    /**
     * Exception class for shader compiler errors.
     */
    class shader_compiler_error : public std::exception
    {
    public:
        shader_compiler_error(const std::string& shader, const std::string& errors) noexcept;
        shader_compiler_error(const shader_compiler_error&) noexcept;
        shader_compiler_error& operator=(const shader_compiler_error&) noexcept;
        shader_compiler_error(shader_compiler_error&&) noexcept;
        shader_compiler_error& operator=(shader_compiler_error&&) noexcept;
        const char* what() const noexcept override;

    private:
        /** Holds the shader name. */
        std::vector<char> shr_;
        /** Holds the errors. */
        std::vector<char> errs_;
        /** holds the error message. */
        std::vector<char> myWhat_;
    };

    /**
     * Wrapper class for shader loading.
     */
    class Shader final
    {
    public:
        Shader(const std::string& shaderFilename, const ApplicationNodeInternal* node);
        Shader(const std::string& shaderFilename, const ApplicationNodeInternal* node, const std::vector<std::string>& defines);
        Shader(const Shader& orig) = delete;
        Shader& operator=(const Shader&) = delete;
        Shader(Shader&& orig) noexcept;
        Shader& operator=(Shader&& orig) noexcept;
        ~Shader() noexcept;

        /** Returns the OpenGL id of the shader. */
        GLuint getShaderId() const noexcept { return shader_; }
        void resetShader(GLuint newShader);
        GLuint recompileShader() const;
        GLenum GetType() const { return type_;}
        const std::string& GetFileName() const {return filename_;}

    private:
        /** Holds the shader file name. */
        std::string filename_;
        /** Holds the compiled shader. */
        GLuint shader_;
        /** Holds the shader type. */
        GLenum type_;
        /** Holds the shader type as a string. */
        std::string strType_;
        /** Holds the defines used in the shader. */
        std::vector<std::string> defines_;

        static GLuint compileShader(const std::string& filename, GLenum type, const std::string& strType, const std::vector<std::string>& defines);
        static std::string LoadShaderFile(const std::string &filename, const std::vector<std::string> &defines, unsigned int &fileId, unsigned int recursionDepth);
        void unload() noexcept;
    };
}
