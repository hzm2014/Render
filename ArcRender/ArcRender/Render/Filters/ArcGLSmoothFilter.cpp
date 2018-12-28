//
//  ArcGLSmoothFilter.cpp
//  ArcRender
//
//  Created by Zhiming He on 2018/12/24.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcGLSmoothFilter.hpp"

const static char* K_Smooth_Filter = SHADER300_STRING (
                                                       
#define x_a 640.0
#define y_a 1136.0
  
in lowp vec2 textureCoordinate;
uniform sampler2D inputImageTexture;
uniform mediump float strength;
out vec4 out_color;

void main() {
   
   vec3 centralColor;
   
   float mul_x = 2.0 / x_a;
   float mul_y = 2.0 / y_a;
   
   vec2 blurCoordinates0 = textureCoordinate + vec2(0.0 * mul_x,-10.0 * mul_y);
   vec2 blurCoordinates1 = textureCoordinate + vec2(5.0 * mul_x,-8.0 * mul_y);
   vec2 blurCoordinates2 = textureCoordinate + vec2(8.0 * mul_x,-5.0 * mul_y);
   vec2 blurCoordinates3 = textureCoordinate + vec2(10.0 * mul_x,0.0 * mul_y);
   vec2 blurCoordinates4 = textureCoordinate + vec2(8.0 * mul_x,5.0 * mul_y);
   vec2 blurCoordinates5 = textureCoordinate + vec2(5.0 * mul_x,8.0 * mul_y);
   vec2 blurCoordinates6 = textureCoordinate + vec2(0.0 * mul_x,10.0 * mul_y);
   vec2 blurCoordinates7 = textureCoordinate + vec2(-5.0 * mul_x,8.0 * mul_y);
   vec2 blurCoordinates8 = textureCoordinate + vec2(-8.0 * mul_x,5.0 * mul_y);
   vec2 blurCoordinates9 = textureCoordinate + vec2(-10.0 * mul_x,0.0 * mul_y);
   vec2 blurCoordinates10 = textureCoordinate + vec2(-8.0 * mul_x,-5.0 * mul_y);
   vec2 blurCoordinates11 = textureCoordinate + vec2(-5.0 * mul_x,-8.0 * mul_y);
   
   mul_x = 1.6 / x_a;
   mul_y = 1.6 / y_a;
   
   vec2 blurCoordinates12 = textureCoordinate + vec2(0.0 * mul_x,-6.0 * mul_y);
   vec2 blurCoordinates13 = textureCoordinate + vec2(-4.0 * mul_x,-4.0 * mul_y);
   vec2 blurCoordinates14 = textureCoordinate + vec2(-6.0 * mul_x,0.0 * mul_y);
   vec2 blurCoordinates15 = textureCoordinate + vec2(-4.0 * mul_x,4.0 * mul_y);
   vec2 blurCoordinates16 = textureCoordinate + vec2(0.0 * mul_x,6.0 * mul_y);
   vec2 blurCoordinates17 = textureCoordinate + vec2(4.0 * mul_x,4.0 * mul_y);
   vec2 blurCoordinates18 = textureCoordinate + vec2(6.0 * mul_x,0.0 * mul_y);
   vec2 blurCoordinates19 = textureCoordinate + vec2(4.0 * mul_x,-4.0 * mul_y);
   
   float central;
   float gaussianWeightTotal;
   float sum;
   float sample;
   float distanceFromCentralColor;
   float gaussianWeight;
   
   float distanceNormalizationFactor = 3.6;
   
   central = texture(inputImageTexture, textureCoordinate).g;
   gaussianWeightTotal = 0.2;
   sum = central * 0.2;
   
   sample = texture(inputImageTexture, blurCoordinates0).g;
   distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
   gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
   gaussianWeightTotal += gaussianWeight;
   sum += sample * gaussianWeight;
   
   sample = texture(inputImageTexture, blurCoordinates1).g;
   distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
   gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
   gaussianWeightTotal += gaussianWeight;
   sum += sample * gaussianWeight;
   
   sample = texture(inputImageTexture, blurCoordinates2).g;
   distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
   gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
   gaussianWeightTotal += gaussianWeight;
   sum += sample * gaussianWeight;
   
   sample = texture(inputImageTexture, blurCoordinates3).g;
   distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
   gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
   gaussianWeightTotal += gaussianWeight;
   sum += sample * gaussianWeight;
   
   sample = texture(inputImageTexture, blurCoordinates4).g;
   distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
   gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
   gaussianWeightTotal += gaussianWeight;
   sum += sample * gaussianWeight;
   
   sample = texture(inputImageTexture, blurCoordinates5).g;
   distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
   gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
   gaussianWeightTotal += gaussianWeight;
   sum += sample * gaussianWeight;
   
   sample = texture(inputImageTexture, blurCoordinates6).g;
   distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
   gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
   gaussianWeightTotal += gaussianWeight;
   sum += sample * gaussianWeight;
   
   sample = texture(inputImageTexture, blurCoordinates7).g;
   distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
   gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
   gaussianWeightTotal += gaussianWeight;
   sum += sample * gaussianWeight;
   
   sample = texture(inputImageTexture, blurCoordinates8).g;
   distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
   gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
   gaussianWeightTotal += gaussianWeight;
   sum += sample * gaussianWeight;
   
   sample = texture(inputImageTexture, blurCoordinates9).g;
   distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
   gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
   gaussianWeightTotal += gaussianWeight;
   sum += sample * gaussianWeight;
   
   sample = texture(inputImageTexture, blurCoordinates10).g;
   distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
   gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
   gaussianWeightTotal += gaussianWeight;
   sum += sample * gaussianWeight;
   
   sample = texture(inputImageTexture, blurCoordinates11).g;
   distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
   gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
   gaussianWeightTotal += gaussianWeight;
   sum += sample * gaussianWeight;
   
   sample = texture(inputImageTexture, blurCoordinates12).g;
   distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
   gaussianWeight = 0.1 * (1.0 - distanceFromCentralColor);
   gaussianWeightTotal += gaussianWeight;
   sum += sample * gaussianWeight;
   
   sample = texture(inputImageTexture, blurCoordinates13).g;
   distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
   gaussianWeight = 0.1 * (1.0 - distanceFromCentralColor);
   gaussianWeightTotal += gaussianWeight;
   sum += sample * gaussianWeight;
   
   sample = texture(inputImageTexture, blurCoordinates14).g;
   distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
   gaussianWeight = 0.1 * (1.0 - distanceFromCentralColor);
   gaussianWeightTotal += gaussianWeight;
   sum += sample * gaussianWeight;
   
   sample = texture(inputImageTexture, blurCoordinates15).g;
   distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
   gaussianWeight = 0.1 * (1.0 - distanceFromCentralColor);
   gaussianWeightTotal += gaussianWeight;
   sum += sample * gaussianWeight;
   
   sample = texture(inputImageTexture, blurCoordinates16).g;
   distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
   gaussianWeight = 0.1 * (1.0 - distanceFromCentralColor);
   gaussianWeightTotal += gaussianWeight;
   sum += sample * gaussianWeight;
   
   sample = texture(inputImageTexture, blurCoordinates17).g;
   distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
   gaussianWeight = 0.1 * (1.0 - distanceFromCentralColor);
   gaussianWeightTotal += gaussianWeight;
   sum += sample * gaussianWeight;
   
   sample = texture(inputImageTexture, blurCoordinates18).g;
   distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
   gaussianWeight = 0.1 * (1.0 - distanceFromCentralColor);
   gaussianWeightTotal += gaussianWeight;
   sum += sample * gaussianWeight;
   
   sample = texture(inputImageTexture, blurCoordinates19).g;
   distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
   gaussianWeight = 0.1 * (1.0 - distanceFromCentralColor);
   gaussianWeightTotal += gaussianWeight;
   sum += sample * gaussianWeight;
   
   sum = sum/gaussianWeightTotal;
   
   centralColor = texture(inputImageTexture, textureCoordinate).rgb;
   
   sample = centralColor.g - sum + 0.5;
   
   for(int i = 0; i < 5; ++i)
   {
       if(sample <= 0.5)
       {
           sample = sample * sample * 2.0;
       }
       else
       {
           sample = 1.0 - ((1.0 - sample)*(1.0 - sample) * 2.0);
       }
   }
   
   float aa = 1.0 + pow(sum, 0.3)*0.09;
   vec3 smoothColor = centralColor*aa - vec3(sample)*(aa-1.0);// get smooth color
   smoothColor = clamp(smoothColor,vec3(0.0),vec3(1.0));//make smooth color right
   
   smoothColor = mix(centralColor, smoothColor, pow(centralColor.g, 0.33));
   smoothColor = mix(centralColor, smoothColor, pow(centralColor.g, 0.39));
   
   smoothColor = mix(centralColor, smoothColor, strength);
   
   out_color = vec4(pow(smoothColor, vec3(0.96)), 1.0);
 }
);

static string vertexShader = ArcGLFilter::g_vertext300_shader;
static string fragmentShader = K_Smooth_Filter;

ArcGLSmoothFilter::ArcGLSmoothFilter(float s): ArcGLFilter(vertexShader, fragmentShader) {
    m_name = "ArcGLSmoothFilter";
    setInputs(1);
    setSmooth(s);
}

ArcGLSmoothFilter::~ArcGLSmoothFilter() {
    
}

void ArcGLSmoothFilter::setSmooth(float s) {
    
    if(s < 0 || s > 1) {
        return;
    }
    m_smoothValue = s;
    setFloat("strength", s);
}
