#pragma once
#include "com/com_guid.hpp"
#include "d3d11.h"

struct MTL_TEMPORAL_UPSCALE_D3D11_DESC {
  UINT InputContentWidth; // can be 0, which means full width
  UINT InputContentHeight; // can be 0, which means full height
  BOOL AutoExposure;
  BOOL InReset;
  BOOL DepthReversed;
  BOOL MotionVectorInDisplayRes;
  ID3D11Resource *Color;
  ID3D11Resource *Depth;
  ID3D11Resource *MotionVector;
  ID3D11Resource *Output;
  FLOAT MotionVectorScaleX;
  FLOAT MotionVectorScaleY;
  FLOAT PreExposure;
  ID3D11Resource *ExposureTexture;
  FLOAT JitterOffsetX;
  FLOAT JitterOffsetY;
};

DEFINE_COM_INTERFACE("43ace3ce-1956-448b-a4eb-aee68bdeb283",
                     IMTLD3D11ContextExt)
    : public IUnknown{
   virtual void STDMETHODCALLTYPE TemporalUpscale(const MTL_TEMPORAL_UPSCALE_D3D11_DESC *pDesc) = 0;
   virtual void STDMETHODCALLTYPE BeginUAVOverlap() = 0;
   virtual void STDMETHODCALLTYPE EndUAVOverlap() = 0;
};

typedef enum MTL_FEATURE {
  MTL_FEATURE_METALFX_TEMPORAL_SCALER = 0,
} MTL_FEATURE;

DEFINE_COM_INTERFACE("19a8e35a-38be-418f-94e3-9f7323936870", IMTLD3D11ContextExt1) : public IMTLD3D11ContextExt {
  virtual HRESULT STDMETHODCALLTYPE CheckFeatureSupport(
      MTL_FEATURE Feature, void *pFeatureSupportData, UINT FeatureSupportDataSize
  ) = 0;
};

DEFINE_COM_INTERFACE("efc77ae6-2179-4c0a-b844-7661ca0dcde7", IMTLD3D11DeviceExt)
    : public IUnknown {
  virtual void STDMETHODCALLTYPE SetShaderExtensionSlot(UINT Slot) = 0;
};

/* Export of a texture's Metal object to an out-of-process consumer (VR compositor). The handles are winemetal
   object handles (obj_handle_t) as seen by the unix side, i.e. MTLTexture / MTLSharedEvent ids that the
   winemetal unix implementation (or a remoting executor) can resolve. Not a D3D11 API. */
DEFINE_COM_INTERFACE("ad6245d1-3a11-4dd6-87b7-7a58048b55d3", IMTLD3D11ExportableTexture)
    : public IUnknown {
  /* Flushes the immediate context. On success *pTexture is the MTLTexture of the resource's current
     allocation, and all work submitted so far (including every write to this texture) has completed once
     the MTLSharedEvent *pSharedEvent reaches *pSignalValue. The handles stay valid while the resource lives
     and is not renamed (render targets are never renamed). */
  virtual HRESULT STDMETHODCALLTYPE GetMetalTexture(UINT64 *pTexture, UINT64 *pSharedEvent, UINT64 *pSignalValue) = 0;
};
