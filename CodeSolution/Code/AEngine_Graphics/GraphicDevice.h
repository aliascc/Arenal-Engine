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

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GraphicsDefs.h"
#include "Shaders\ShaderDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class Shader;
class Sampler;
class Texture;
class Viewport;
class Texture2D;
class HullShader;
class IndexBuffer;
class QuadShape2D;
class PixelShader;
class TextureArray;
class ShaderBuffer;
class RenderTarget;
class VertexShader;
class DomainShader;
class IVertexBuffer;
class GraphicDevice;
class ComputeShader;
class GeometryShader;
class ConstantBuffer;
class Texture2DArray;
class DepthStencilSurface;
class SamplerStateCollection;

/*****************
*   Class Decl   *
******************/

/// <summary>
/// Graphic Device Class
/// </summary>
class GraphicDevice sealed : public AEObject
{
private:

    /// <summary>
    /// DirectX 11 Device Pointer
    /// </summary>
    ID3D11Device* m_DeviceDX = nullptr;

    /// <summary>
    /// DirectX 11 Device Context
    /// </summary>
    ID3D11DeviceContext* m_DeviceContextDX = nullptr;

    /// <summary>
    /// DirectX 11 Swap Chain
    /// </summary>
    IDXGISwapChain* m_SwapChainDX = nullptr;

    /// <summary>
    /// DirectX 11 Game Render Target
    /// </summary>
    ID3D11RenderTargetView* m_GameRenderTargetViewDX = nullptr;

    /// <summary>
    /// DirectX 11 Game Depth Stencil Buffer
    /// </summary>
    ID3D11Texture2D* m_GameDepthStencilBufferDX = nullptr;

    /// <summary>
    /// DirectX 11 Game Depth Stencil View
    /// </summary>
    ID3D11DepthStencilView* m_GameDepthStencilViewDX = nullptr;

    /// <summary>
    /// Max Render Targets that can be set
    /// </summary>
    uint32_t m_MaxNumRenderTargets = 0;

    /// <summary>
    /// The Highest Stage ID of a render target present. 
    /// Use to determine how many views we will set when we call the set Render Targets and DS Function
    /// </summary>
    uint32_t m_HighestRenderTargetStage = 0;

    /// <summary>
    /// DirectX 11 Current Render Targets
    /// </summary>
    ID3D11RenderTargetView** m_CurrentRenderTargetViewsDX = nullptr;

    /// <summary>
    /// DirectX 11 Current Depth Stencil View
    /// </summary>
    ID3D11DepthStencilView* m_CurrentDepthStencilViewDX = nullptr;

    /// <summary>
    /// DirectX 11 Default Screen View port
    /// </summary>
    D3D11_VIEWPORT m_GameViewportDX = { 0 };

    /// <summary>
    /// DirectX Swap Chain Description
    /// </summary>
    DXGI_SWAP_CHAIN_DESC m_SwapChainDescDX;

#ifdef AE_EDITOR_MODE

    /// <summary>
    /// DirectX 11 Game Render Target Buffer
    /// </summary>
    ID3D11Texture2D* m_GameRenderTargetBufferDX = nullptr;

    /// <summary>
    /// DirectX Shader Resource use to bind the Texture to a Shader
    /// </summary>
    ID3D11ShaderResourceView* m_GameRenderTargetSRV = nullptr;

    /// <summary>
    /// DirectX 11 Editor Render Targets
    /// </summary>
    ID3D11RenderTargetView* m_EditorRenderTargetViewDX = nullptr;

    /// <summary>
    /// DirectX 11 Editor Screen View port
    /// </summary>
    D3D11_VIEWPORT m_EditorViewportDX = { 0 };

#endif

    Texture2D* m_DefaultTexture2D = nullptr;

    bool m_IsReady = false;

    glm::vec2 m_HalfPixel = AEMathHelpers::Vec2fZero;

    QuadShape2D* m_QuadShape2D = nullptr;

    SamplerStateCollection* m_SamplerStateCol = nullptr;

    GraphicsPresentationParameters m_GraphicPP;

#if defined(AE_GRAPHIC_DEBUG_DEVICE)

    ID3DUserDefinedAnnotation*    m_UserDefinedAnnotationDX = nullptr;

#endif

    AEResult CreateDefaultTextures();

    void ReleaseDefaultTextures();

    AEResult InitDXConfiguration();

    void InitSwapChainDesc();

    AEResult CreateSwapChain();

    /// <summary>
    /// Initializes the Game Viewport
    /// </summary>
    void InitGameViewport();

    AEResult InitGameRTDS();

    AEResult CreateGameDepthStencil();

    AEResult CreateGameRenderTarget();

    AEResult InitPreGraphicObjects();

    AEResult InitGraphicObjects();

    void InitBasicVertexTypes();

    void CleanUpBasicVertexTypes();

    void ResetHalfPixel();

    void OnLostDevice();

    void OnResetDevice();

    void SetShaderResourceView(ShaderType type, uint32_t index, ID3D11ShaderResourceView* srv);

    void SetShaderResourceViews(ShaderType type, uint32_t index, uint32_t numView, ID3D11ShaderResourceView** srv);

#ifdef AE_EDITOR_MODE

    /// <summary>
    /// Creates the Render Targets needed for the Editor
    /// </summary>
    /// <returns>AEResult::Ok if successful, otherwise an error code</returns>
    AEResult CreateEditorRenderTarget();

    /// <summary>
    /// Resets the Game Render Target View
    /// </summary>
    void ResetGameRenderTargetView();

    /// <summary>
    /// Initializes the Editor Viewport
    /// </summary>
    void InitEditorViewport();

    /// <summary>
    /// Cleans up the Editor Render Targets
    /// </summary>
    inline void CleanUpEditorRenderTargets()
    {
        ReleaseCOM(m_EditorRenderTargetViewDX);
    }

#endif

    /// <summary>
    /// Cleans up the Game Depth Stencils
    /// </summary>
    inline void CleanUpGameDepthStencil()
    {
        ReleaseCOM(m_GameDepthStencilViewDX);
        ReleaseCOM(m_GameDepthStencilBufferDX);
    }

    /// <summary>
    /// Cleans up the Game Render Targets
    /// </summary>
    inline void CleanUpGameRenderTargets()
    {
        ReleaseCOM(m_GameRenderTargetViewDX);

#ifdef AE_EDITOR_MODE
        ReleaseCOM(m_GameRenderTargetSRV);
        ReleaseCOM(m_GameRenderTargetBufferDX);
#endif
    }

public:

    /// <summary>
    /// GraphicDevice Constructor
    /// </summary>
    /// <param name='hMainWnd'>Main Window of the Game Application</param>
    /// <param name='graphicOpts'>Graphic Options to use when initializing the device</param>
    GraphicDevice(HWND hMainWnd, const GraphicOptsPreferred& graphicOpts);

    /// <summary>
    /// Default GraphicDevice Destructor
    /// </summary>
    virtual ~GraphicDevice();

    //Delete copy constructor/operator
    GraphicDevice(const GraphicDevice&) = delete;
    GraphicDevice& operator=(const GraphicDevice&) = delete;

    inline GraphicsPresentationParameters& GetGraphicPP()
    {
        return m_GraphicPP;
    }

    inline const glm::vec2& GetHalfPixel() const
    { 
        return m_HalfPixel; 
    }

    inline ID3D11Device* GetDeviceDX() const
    { 
        return m_DeviceDX; 
    }

    inline ID3D11DeviceContext* GetDeviceContextDX() const
    { 
        return m_DeviceContextDX; 
    }

    inline Texture2D* GetDefaultTexture2D() const
    {
        return m_DefaultTexture2D;
    }

    void ResetDevice();
    void Resize(uint32_t width, uint32_t height);
    AEResult InitDevice();

    void Clear(bool target = true, uint32_t rt_stage_id = 0, bool depth = true, bool stencil = true, const Color& color = AEColors::WorldEngineBackground, float depthVal = 1.0f, uint8_t stencilVal = 0);
    void Present();
    AEResult CheckDevCaps(const GraphicsCheckDevCaps& devCaps);

    void SetPixelShader(PixelShader* ps);
    void SetVertexShader(VertexShader* vs);
    void SetGeometryShader(GeometryShader* gs);
    void SetHullShader(HullShader* hs);
    void SetDomainShader(DomainShader* ds);
    void SetComputeShader(ComputeShader* cs);
    void SetShader(Shader* cs);
    void SetSamplerState(ShaderType type, uint32_t index, ID3D11SamplerState* sampler);
    void SetSamplerStates(ShaderType type, uint32_t index, uint32_t numStates, ID3D11SamplerState** sampler);

    void SetConstantBuffer(ShaderType type, ConstantBuffer* cb, bool overrideBindIndex = false, uint32_t newIndex = 0);

    void SetTexture(ShaderType type, uint32_t index, Texture* texture);

    void SetTextureArray(ShaderType type, uint32_t index, TextureArray* textureArray);

    /// <summary>
    /// Sets a Sampler to the Shader stage
    /// </summary>
    /// <param name="type">Type of Shader that will use this buffer</param>
    /// <param name="sampler">Sampler to set</param>
    /// <param name="overrideBindIndex">If true, overrides the Sampler's bind index</param>
    /// <param name="newIndex">New Bind Index if overwritten was enabled</param>
    void SetSampler(ShaderType type, Sampler* sampler, bool overrideBindIndex = false, uint32_t newIndex = 0);

    /// <summary>
    /// Sets a Shader Buffer to the Shader stage
    /// </summary>
    /// <param name="type">Type of Shader that will use this buffer</param>
    /// <param name="shaderBuffer">Shader Buffer to set</param>
    /// <param name="overrideBindIndex">If true, overrides the Shader Buffer's bind index</param>
    /// <param name="newIndex">New Bind Index if overwritten was enabled</param>
    void SetShaderBuffer(ShaderType type, ShaderBuffer* shaderBuffer, bool overrideBindIndex = false, uint32_t newIndex = 0);

    /// <summary>
    /// Sets a Shader Buffer that is RW Enabled to the Compute Shader stage
    /// </summary>
    /// <param name="shaderBuffer">Shader Buffer to set</param>
    /// <param name="overrideBindIndex">If true, overrides the Shader Buffer's bind index</param>
    /// <param name="newIndex">New Bind Index if overwritten was enabled</param>
    void SetShaderRWBufferToCS(ShaderBuffer* shaderBuffer, bool overrideBindIndex = false, uint32_t newIndex = 0);

    /// <summary>
    /// Change the Render Target and keeps the current Depth Stencil
    /// </summary>
    /// <param name="stage_id">Slot where to set the Render Target</param>
    /// <param name="rt">Render Target to set</param>
    void SetRenderTarget(uint32_t stage_id, RenderTarget* rt);

    /// <summary>
    /// Change the Depth Stencil and keeps the current Render Targets
    /// </summary>
    /// <param name="depthStencilSurface">Depth Stencil to set</param>
    void SetDepthStencil(DepthStencilSurface* depthStencilSurface);

    /// <summary>
    /// Change the Render Targets and Depth Stencil
    /// </summary>
    /// <param name="numRTs">Number of Render Targets to set</param>
    /// <param name="rts">Render Target Array</param>
    /// <param name="depthStencilSurface">Depth Stencil to set</param>
    void SetRenderTargetsAndDepthStencil(uint32_t numRTs, RenderTarget* rts[], DepthStencilSurface* depthStencilSurface);

    /// <summary>
    /// Change the Render Targets
    /// </summary>
    /// <param name="numRTs">Number of Render Targets to set</param>
    /// <param name="idx">Indexes in the Render Array for the RTs</param>
    /// <param name="rts">Render Target Array</param>
    void SetRenderTargets(uint32_t numRTs, uint32_t idxs[], Texture2DArray* rtArray);

    /// <summary>
    /// Sets the Render Targets back to the default ones and keeps the current Depth Stencil
    /// </summary>
    void ResetRenderTarget();

    /// <summary>
    /// Sets the Depth Stencil back to the default one and keeps the current Render Targets
    /// </summary>
    void ResetDepthStencil();

    /// <summary>
    /// Sets the Render Targets and Depth Stencil back to the default ones
    /// </summary>
    void ResetRenderTargetAndSetDepthStencil();

    /// <summary>
    /// Sets the Viewport to the Graphic Device
    /// </summary>
    /// <param name="viewportNums">Number of Viewports to set</param>
    /// <param name="viewports">Array of Viewports</param>
    void SetViewport(uint32_t viewportNums, Viewport* viewports[]);

    /// <summary>
    /// Sets the Viewport to the Graphic Device in slot 1
    /// </summary>
    /// <param name="viewports">Viewport to set</param>
    void SetViewport(Viewport* viewport);

    /// <summary>
    /// Reset the Viewport to the default
    /// </summary>
    void ResetViewport();

    void SetIndexBuffer(IndexBuffer* ib, uint32_t offset = 0);
    void SetVertexBuffer(IVertexBuffer* vb, uint32_t stream = 0, uint32_t offset = 0);
    void SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY primitive);

    void GetBlendState(ID3D11BlendState** blendState, glm::vec4& blendFactor, uint32_t& sampleMask);
    void SetBlendState(ID3D11BlendState* blendState, const glm::vec4& blendFactor = AEMathHelpers::Vec4fOne, uint32_t sampleMask = 0xffffffff);

    void Draw(uint32_t vertexCount, uint32_t startVertex);
    void DrawIndexed(uint32_t startVertex, uint32_t startIndex, uint32_t indexCount);
    void DrawFullScreenQuad(const glm::vec4& texCoord = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    void DrawQuad2D(const RECT& size, const glm::vec4& texCoord = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

    void DispatchComputeShader(uint32_t threadGroupCountX, uint32_t threadGroupCountY, uint32_t threadGroupCountZ);

    AEResult CheckDevCaps(const std::string& file);

#if defined(AE_GRAPHIC_DEBUG_DEVICE)
    void BeginEvent(const std::string& eventName);
    void EndEvent();
    void SetEventmarker(const std::string& eventName);
#else
    inline void BeginEvent(const std::string& eventName) {}
    inline void EndEvent() {}
    inline void SetEventmarker(const std::string& eventName) {}
#endif

#ifdef AE_EDITOR_MODE

    /// <summary>
    /// Gets the SRV for the Game Render Target
    /// </summary>
    ID3D11ShaderResourceView* GetGameRenderTargetSRV()
    {
        return m_GameRenderTargetSRV;
    }

    /// <summary>
    /// Resize the Editor Targets
    /// </summary>
    /// <param name="width">Width of the new size</param>
    /// <param name="height">Height of the new size</param>
    void ResizeEditor(uint32_t width, uint32_t height);

    /// <summary>
    /// Sets the Editor Render Targets and Viewport
    /// </summary>
    void SetEditorRenderTargetAndViewPort();

#endif

};