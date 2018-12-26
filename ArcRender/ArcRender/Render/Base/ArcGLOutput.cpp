//
//  ArcGLOutput.cpp
//  ArcRender
//
//  Created by zhiming he on 2018/11/15.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcGLOutput.hpp"
#include <iostream>
#include "ArcGLInput.hpp"

ArcGLOutput::ArcGLOutput() {
    m_outputSize = {0};
    m_outFrameBuffer = nullptr;
    m_complete = nullptr;
}

ArcGLOutput::~ArcGLOutput() {
    
}

int ArcGLOutput::addTarget(ArcGLInput *target, int location) {
    if(target == NULL) {
        std::cout<<"addTarget NULL!"<<std::endl;
        return -1;
    }
    
    unsigned targetCount = targetsCount();
    for (int i=0; i < targetCount; i++) {
        // 如果查找到target重置location
        if (m_targets[i] == target) {
            m_location[i] = location;
            return location;
        }
    }
    
    target -> addSource(this);
    m_targets.push_back(target);
    m_location.push_back(location);
    return location;
}



bool ArcGLOutput::removeTarget(ArcGLInput *target, int* location) {
    
    int i = (int)m_targets.size();
    if(i <= 0) {
        std::cout<<"GPUOutput::removeTarget() from empty target list!"<<std::endl;
        return false;
    }
    
    vector<ArcGLInput*>::iterator targetIter = m_targets.begin();
    vector<int>::iterator locationIter = m_location.begin();
    for (; targetIter!=m_targets.end(); targetIter++,locationIter++) {
        if (*targetIter == target) {
            m_targets.erase(targetIter);
            m_location.erase(locationIter);
            return true;
        }
    }
    
    return false;
}

bool ArcGLOutput::removeAllTargets() {
    for (int i=0; i<m_targets.size(); i++) {
        m_targets[i]->removeSource(this);
    }
    
    m_targets.clear();
    m_location.clear();
    return true;
}

void ArcGLOutput::setOutputSize(ArcGLSize size) {
    m_outputSize = size;
}

void ArcGLOutput::informTargets() {
    int n = targetsCount();
    for(int i = 0; i < n; i++) {
        ArcGLInput* input = m_targets[i];
        input->setInputFrameBuffer(m_outFrameBuffer);
        input->newFrame();
    }
}
