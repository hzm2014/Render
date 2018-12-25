//
//  ArcGLTwoPassFilter.cpp
//  ArcRender
//
//  Created by Zhiming He on 2018/12/25.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcGLTwoPassFilter.hpp"
#include "ArcGLContext.hpp"

ArcGLTwoPassFilter::ArcGLTwoPassFilter(const string& vertexShader, const string& fragmentShader, const string& secVertexShader, const string& secFragmentShader): ArcGLFilter(vertexShader, fragmentShader) {
    m_name = "ArcGLTwoPassFilter";
    setInputs(1);
}

ArcGLTwoPassFilter::~ArcGLTwoPassFilter() {
    
}

void ArcGLTwoPassFilter::activeSecOutFrameBuffer() {
    m_secOutFrameBuffer = m_context -> frameBuffer(m_outputSize, m_option);
    m_secOutFrameBuffer -> active();
    glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}

void ArcGLTwoPassFilter::newFrame() {
    
    ArcGLFilter::newFrame();
    
    
    
    /* TODO
    m_outFrameBuffer -> retainForUse();
    m_context -> useProgram(m_secVertexShader, m_secFragmentShader);
    activeSecOutFrameBuffer();
    m_outFrameBuffer -> active(GL_TEXTURE0);
    m_vertexArray -> bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    for (int i = 0; i < m_inputsNum; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    m_vertexArray -> unbind();
    m_outFrameBuffer -> retainForUse();
    m_secOutFrameBuffer -> deactive();
     */
}

