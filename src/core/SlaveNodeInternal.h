/**
 * @file   SlaveNodeInternal.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.11.25
 *
 * @brief  Declaration of the ApplicationNodeImplementation for the slave node.
 */

#pragma once

#include "app/ApplicationNodeImplementation.h"
#include "core/CalibrationVertices.h"

namespace viscom {

    class SlaveNodeInternal : public ApplicationNodeImplementation
    {
    public:
        explicit SlaveNodeInternal(ApplicationNodeInternal* appNode);
        virtual ~SlaveNodeInternal() override;

        void InitOpenGL() override;
        void DrawFrame(FrameBuffer& fbo) override;
        void Draw2D(FrameBuffer& fbo) override;
        void CleanUp() override;

    private:
        void CreateProjectorFBO(size_t windowId, const glm::ivec2& fboSize);


        /** Holds the viewport for rendering directly to the projector. */
        std::vector<Viewport> projectorViewport_;
        /** Holds the vertex coordinates for the screen aligned quad to render to (in projector space). */
        std::vector<CalbrationProjectorQuadVertex> quadCoordsProjector_;


        /** Holds the shader program for applying the calibration to a rendered scene. */
        std::shared_ptr<GPUProgram> calibrationProgram_;
        /** Holds the location of the use alpha test flag. */
        GLint calibrationAlphaTexLoc_ = -1;
        /** Holds the location of the use alpha test flag. */
        GLint calibrationSceneTexLoc_ = -1;

        /** Holds the vertex buffer for the projector quads. */
        GLuint vboProjectorQuads_ = 0;
        /** Holds the vertex array object for the projector quads. */
        GLuint vaoProjectorQuads_ = 0;
        /** Holds the frame buffers for rendering the scene into. */
        std::vector<FrameBuffer> sceneFBOs_;
        /** Holds the alpha textures. */
        std::vector<GLuint> alphaTextures_;
    };
}
