//
//  ArcGLVertexArray.cpp
//  ArcRender
//
//  Created by Zhiming He on 2018/11/26.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcGLVertexArray.hpp"

ArcGLVertexArray::ArcGLVertexArray() {
    m_vertexArray = 0;
    createVAO();
}

ArcGLVertexArray::~ArcGLVertexArray() {
    glDeleteVertexArrays(1, &m_vertexArray);
}

void ArcGLVertexArray::createVAO() {
    glGenVertexArrays(1, &m_vertexArray);
}

void ArcGLVertexArray::bind() {
    glBindVertexArray(m_vertexArray);
}

void ArcGLVertexArray::unbind() {
    glBindVertexArray(0);
}
