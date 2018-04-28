/**
 * @file   GPUProgram.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.12.14
 *
 * @brief  Declaration of a GPU program resource class.
 */

#pragma once

#include "core/resources/Resource.h"
#include "core/open_gl_fwd.h"

namespace viscom {

    class ApplicationNodeInternal;
    class Shader;

    /**
     * Complete GPU program with multiple Shader objects working together.
     */
    class GPUProgram final : Resource
    {
    public:
        [[deprecated("Use the vector version instead.")]]
        GPUProgram(const std::string& programName, ApplicationNodeInternal* node, std::initializer_list<std::string> shaderNames);
        GPUProgram(const std::string& programName, ApplicationNodeInternal* node, std::vector<std::string> shaderNames);
        GPUProgram(const std::string& programName, ApplicationNodeInternal* node, std::vector<std::string> shaderNames, const std::vector<std::string>& defines);
        GPUProgram(const GPUProgram& orig) = delete;
        GPUProgram& operator=(const GPUProgram&) = delete;
        GPUProgram(GPUProgram&&) noexcept;
        GPUProgram& operator=(GPUProgram&&) noexcept;
        virtual ~GPUProgram() noexcept override;

        void recompileProgram();
        /** Returns the OpenGL program id. */
        GLuint getProgramId() const noexcept { return program_; }
        /** Returns the Program name */
        const std::string& GetProgramName() const noexcept { return programName_;}
        /** Returns a uniform locations. */
        GLint getUniformLocation(const std::string& name) const;
        /** Returns a list of uniform locations. */
        [[deprecated("Use the vector version instead.")]]
        std::vector<GLint> getUniformLocations(const std::initializer_list<std::string>& names) const;
        std::vector<GLint> GetUniformLocations(const std::vector<std::string>& names) const;

        /** Returns a attribute locations. */
        GLint getAttributeLocation(const std::string& name) const;
        /** Returns a list of attribute locations. */
        [[deprecated("Use the vector version instead.")]]
        std::vector<GLint> getAttributeLocations(const std::initializer_list<std::string>& names) const;
        std::vector<GLint> GetAttributeLocations(const std::vector<std::string>& names) const;

        const std::vector<std::unique_ptr<Shader>>& GetShaders() const noexcept {return shaders_;}
    private:
        using ShaderList = std::vector<std::unique_ptr<Shader>>;

        /** Holds the program name. */
        std::string programName_;
        /** Holds the shader names. */
        std::vector<std::string> shaderNames_;
        /** Holds the program. */
        GLuint program_;
        /** Holds a list of shaders used internally. */
        ShaderList shaders_;
        /** Holds the defines used in the program. */
        std::vector<std::string> defines_;

        void unload() noexcept;
        template<typename T, typename SHAcc> static GLuint linkNewProgram(const std::string& name,
            const std::vector<T>& shaders, SHAcc shaderAccessor);
        static void releaseShaders(const std::vector<GLuint>& shaders) noexcept;
    };
}
