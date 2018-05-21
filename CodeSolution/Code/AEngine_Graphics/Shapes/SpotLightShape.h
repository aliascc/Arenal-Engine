/*
* Copyright (c) 2018 <Carlos Chacón>
* All rights reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#pragma once

#ifndef _SPOT_LIGHT_SHAPE_H
#define _SPOT_LIGHT_SHAPE_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "LightShape.h"
#include "Vertex\VertexBuffer.h"
#include "Vertex\Types\VertexPositionColor.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
class SpotLightShape sealed : public LightShape
{
    private:

        uint32_t m_NumEdges = 0;

        float m_NearAttenuation = 0.0f;
        float m_FarAttenuation = 0.0f;
        float m_Angle = 0.0f;
        float m_FallOffAngle = 0.0f;

        uint32_t m_SizeOfInnerCone = 0;
        uint32_t m_SizeOfRings = 0;

        Color m_NearAttColor = AE_LIGHT_SHAPE_DEBUG_DEFAULT_NEAR_COLOR;
        Color m_FarAttColor = AE_LIGHT_SHAPE_DEBUG_DEFAULT_FAR_COLOR;
        Color m_InnerCircleColor = AE_LIGHT_SHAPE_DEBUG_DEFAULT_LINE_INNER_COLOR;
        Color m_LineInnerColor = AE_LIGHT_SHAPE_DEBUG_DEFAULT_INNER_CIRCLE_COLOR;
        Color m_LineNearColor = AE_LIGHT_SHAPE_DEBUG_DEFAULT_LINE_NEAR_COLOR;
        Color m_LineFarColor = AE_LIGHT_SHAPE_DEBUG_DEFAULT_LINE_FAR_COLOR;

        VertexBuffer<VertexPositionColor>* m_InnerConeAndLines = nullptr;
        VertexPositionColor* m_InnerConeAndLinesVtxs = nullptr;

        VertexBuffer<VertexPositionColor>* m_NearFallOfAngleRing = nullptr;
        VertexPositionColor* m_NearFallOfAngleRingVtxs = nullptr;

        VertexBuffer<VertexPositionColor>* m_NearAttenuationRing = nullptr;
        VertexPositionColor* m_NearAttenuationRingVtxs = nullptr;

        VertexBuffer<VertexPositionColor>* m_FarFallOfAngleRing = nullptr;
        VertexPositionColor* m_FarFallOfAngleRingVtxs = nullptr;

        VertexBuffer<VertexPositionColor>* m_FarAttenuationRing = nullptr;
        VertexPositionColor* m_FarAttenuationRingVtxs = nullptr;

        void UpdateInnerConeAndLines();

        void UpdateLines(uint32_t startIndex, glm::vec3 startPoints[4], glm::vec3 endPoints[4], Color color);

        void UpdateNearAttenuationRing();

        void UpdateNearFallOffAttenuationRing();

        void UpdateFarAttenuationRing();

        void UpdateFarFallOffAttenuationRing();

        void UpdateRingInfo(VertexPositionColor vtxVector[], float size, float distance, Color color);

    public:
        //Constructor Destructor.
        SpotLightShape(GraphicDevice* graphicDevice, uint32_t numEdges = AE_LIGHT_SHAPE_DEFAULT_NUM_EDGES);
        virtual ~SpotLightShape();

        AEResult UpdateLightInfo(float nearAttenuation, float farAttenuation, float angle, float fallOfAngle);

        AEResult BuildLightShape() override;
};

#endif

