//
//  ArcBlendForEncodeFilter.cpp
//  ArcRender
//
//  Created by Zhiming He on 2018/12/5.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcBlendForEncodeFilter.hpp"

ArcBlendForEncodeFilter::ArcBlendForEncodeFilter(ArcGLRect rect, ArcGLSize viewSize): ArcBlendImageFilter(rect, viewSize) {
    m_name = "ArcBlendForEncodeFilter";
}

ArcBlendForEncodeFilter::~ArcBlendForEncodeFilter() {
    
}

void ArcBlendForEncodeFilter::calculateImageRect(ArcGLRect rect) {
    ArcGLRect rectForEncode = rect;
    
    if(((rect.point.x + rect.size.width / 2.0) <= m_viewSize.width / 2.0) && (rect.point.y + rect.size.height / 2.0) <= m_viewSize.height / 2.0) { //左上角
        
        rectForEncode.point.x = rect.point.x;
        rectForEncode.point.y = rect.point.y;
    }
    
    if(((rect.point.x + rect.size.width / 2.0) > m_viewSize.width / 2.0) && (rect.point.y + rect.size.height / 2.0) <= m_viewSize.height / 2.0) { //右上角
        float delta = m_viewSize.width - rect.point.x;
        rectForEncode.point.x = m_adjustedViewSize.width - delta;
        rectForEncode.point.y = rect.point.y;
    }
    
    if(((rect.point.x + rect.size.width / 2.0) <= m_viewSize.width / 2.0) && (rect.point.y + rect.size.height / 2.0) > m_viewSize.height / 2.0) { //左下角
        float delta = m_viewSize.height - rect.point.y;
        rectForEncode.point.x = rect.point.x;
        rectForEncode.point.y = m_adjustedViewSize.height - delta;
    }
    
    if(((rect.point.x + rect.size.width / 2.0) > m_viewSize.width / 2.0) && (rect.point.y + rect.size.height / 2.0) > m_viewSize.height / 2.0) { //右下角
        float deltaX = m_viewSize.width - rect.point.x;
        float deltaY = m_viewSize.height - rect.point.y;
        rectForEncode.point.x = m_adjustedViewSize.width - deltaX;
        rectForEncode.point.y = m_adjustedViewSize.height - deltaY;
    }
    
    m_imageRect = rectForEncode;
}


