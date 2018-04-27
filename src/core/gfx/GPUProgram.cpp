/**
 * @file   GPUProgram.cpp
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.12.14
 *
 * @brief  Implementation of the GPU program class.
 */

#include "GPUProgram.h"
#include <iostream>
#include "core/ApplicationNodeInternal.h"
#include "core/gfx/Shader.h"
#include "core/open_gl.h"

namespace viscom {

    /**
     * Constructor.
     * @param theProgramName the name of the program used to identify during logging.
     * @param theShaderNames the filenames of all shaders to use in this program.
     */
    GPUProgram::GPUProgram(const std::string& theProgramName, ApplicationNodeInternal* node, std::initializer_list<std::string> theShaderNames) :
        GPUProgram{ theProgramName, node, theShaderNames, std::vector<std::string>() }
    {
    }

    /**
     * Constructor.
     * @param theProgramName the name of the program used to identify during logging.
     * @param theShaderNames the filenames of all shaders to use in this program.
     */
    GPUProgram::GPUProgram(const std::string& theProgramName, ApplicationNodeInternal* node, std::vector<std::string> theShaderNames) :
        GPUProgram{ theProgramName, node, theShaderNames, std::vector<std::string>{} }
    {
    }

    GPUProgram::GPUProgram(const std::string& programName, ApplicationNodeInternal* node, std::vector<std::string> shaderNames, const std::vector<std::string>& defines) :
        Resource(programName, node),
        programName_(programName),
        shaderNames_(shaderNames),
        program_(0),
        defines_{ defines }
    {
        for (const auto& shaderName : shaderNames_) {
            shaders_.emplace_back(std::make_unique<Shader>(shaderName, node, defines_));
        }
        program_ = linkNewProgram(programName_, shaders_, [](const std::unique_ptr<Shader>& shdr) noexcept { return shdr->getShaderId(); });
    }

    GPUProgram::GPUProgram(std::string programName, ApplicationNodeInternal *node, std::vector<std::unique_ptr<Shader>> shaders) :
        Resource(programName, node),
        programName_(std::move(programName)),
        shaderNames_{},
        program_(0),
        shaders_{std::move(shaders)},
        defines_{ }
        {
            for (const auto& shader : shaders_) {
                shaderNames_.emplace_back(shader->getFileName());
                //shaders_.emplace_back(std::make_unique<Shader>(fs::path{shader->getFileName()}, node, defines_));
            }
            program_ = linkNewProgram(programName_, shaders_, [](const std::unique_ptr<Shader>& shdr) noexcept { return shdr->getShaderId(); });
        }
    /**
     * Move-constructor.
     * @param rhs the object to move.
     */
    GPUProgram::GPUProgram(GPUProgram&& rhs) noexcept :
        Resource(std::move(rhs)),
        programName_(std::move(rhs.programName_)),
        shaderNames_(std::move(rhs.shaderNames_)),
        program_(std::move(rhs.program_)),
        shaders_(std::move(rhs.shaders_)),
        defines_(std::move(rhs.defines_))
    {
        rhs.program_ = 0;
    }

    /**
     * Move-assignment operator.
     * @param rhs the object to move.
     * @return reference to this object.
     */
    GPUProgram& GPUProgram::operator =(GPUProgram&& rhs) noexcept
    {
        if (this != &rhs) {
            this->~GPUProgram();
            Resource* tRes = this;
            *tRes = static_cast<Resource&&>(std::move(rhs));
            programName_ = std::move(rhs.programName_);
            shaderNames_ = std::move(rhs.shaderNames_);
            program_ = rhs.program_;
            rhs.program_ = 0;
            shaders_ = std::move(rhs.shaders_);
            defines_ = std::move(rhs.defines_);
        }
        return *this;
    }

    /** Destructor. */
    GPUProgram::~GPUProgram() noexcept
    {
        unload();
    }

    void GPUProgram::unload() noexcept
    {
        if (this->program_ != 0) {
            glDeleteProgram(this->program_);
            this->program_ = 0;
        }
    }

    // ReSharper disable CppDoxygenUnresolvedReference
    /**
     *  Links a new program.
     *  @param T the type of the shaders list.
     *  @param SHAcc the type of the shaders list accessor to the shader id.
     *  @param name the name of the program.
     *  @param shaders a list of shaders used for creating the program.
     *  @param shaderAccessor function to access the shader id from the shader object in list.
     */
    // ReSharper restore CppDoxygenUnresolvedReference
    template<typename T, typename SHAcc>
    GLuint GPUProgram::linkNewProgram(const std::string& name, const std::vector<T>& shaders, SHAcc shaderAccessor)
    {
        auto program = glCreateProgram();
        if (program == 0) {
            std::cerr << "Could not create GPU program!";
            throw std::runtime_error("Could not create GPU program!");
        }
        for (const auto& shader : shaders) {
            glAttachShader(program, shaderAccessor(shader));
        }
        glLinkProgram(program);

        GLint status;
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        if (status == GL_FALSE) {
            GLint infoLogLength;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

            std::string strInfoLog;
            strInfoLog.resize(static_cast<std::size_t>(infoLogLength + 1));
            glGetProgramInfoLog(program, infoLogLength, nullptr, strInfoLog.data());
            std::cerr << "Linker failure: " << strInfoLog;

            for (const auto& shader : shaders) {
                glDetachShader(program, shaderAccessor(shader));
            }
            glDeleteProgram(program);

            throw shader_compiler_error(name, strInfoLog);
        }
        for (const auto& shader : shaders) {
            glDetachShader(program, shaderAccessor(shader));
        }
        return program;
    }

    /**
     *  Recompiles the program.
     */
    void GPUProgram::recompileProgram()
    {
        std::vector<GLuint> newOGLShaders(shaderNames_.size(), 0);

        for (std::size_t i = 0; i < shaderNames_.size(); ++i) {
            try {
                newOGLShaders[i] = shaders_[i]->recompileShader();
            } catch (shader_compiler_error&) {
                releaseShaders(newOGLShaders);
                throw;
            }
        }

        GLuint tempProgram = 0;
        try {
            tempProgram = linkNewProgram(programName_, newOGLShaders, [](GLuint shdr) noexcept { return shdr; });
        } catch (shader_compiler_error&) {
            releaseShaders(newOGLShaders);
            throw;
        }

        unload();
        for (std::size_t i = 0; i < shaders_.size(); ++i) {
            shaders_[i]->resetShader(newOGLShaders[i]);
        }
        program_ = tempProgram;
    }

    GLint GPUProgram::getUniformLocation(const std::string& name) const
    {
        return glGetUniformLocation(program_, name.c_str());
    }

    std::vector<GLint> GPUProgram::getUniformLocations(const std::initializer_list<std::string>& names) const
    {
        return GetUniformLocations(names);
    }

    std::vector<GLint> GPUProgram::GetUniformLocations(const std::vector<std::string>& names) const
    {
        std::vector<GLint> result;
        result.reserve(names.size());
        for (const auto& name : names) {
            result.push_back(glGetUniformLocation(program_, name.c_str()));
        }
        return result;
    }

    GLint GPUProgram::getAttributeLocation(const std::string& name) const
    {
        return glGetAttribLocation(program_, name.c_str());
    }

    std::vector<GLint> GPUProgram::getAttributeLocations(const std::initializer_list<std::string>& names) const
    {
        return GetAttributeLocations(names);
    }

    std::vector<GLint> GPUProgram::GetAttributeLocations(const std::vector<std::string>& names) const
    {
        std::vector<GLint> result;
        result.reserve(names.size());
        for (const auto& name : names) {
            result.push_back(glGetAttribLocation(program_, name.c_str()));
        }
        return result;
    }

    void GPUProgram::releaseShaders(const std::vector<GLuint>& shaders) noexcept
    {
        for (auto shader : shaders) {
            if (shader != 0) glDeleteShader(shader);
        }
    }


}
