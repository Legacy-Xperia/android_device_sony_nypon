/**
  src/audio_renderer/audio_renderer.c

  OpenMax audio renderer component.
  This component implements an audio_renderer that accepts
  a single audio PCM stream

  Copyright (C) 2007-2009  STMicroelectronics
  Copyright (C) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).

  This library is free software; you can redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by the Free
  Software Foundation; either version 2.1 of the License, or (at your option)
  any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
  details.

  You should have received a copy of the GNU Lesser General Public License
  along with this library; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St, Fifth Floor, Boston, MA
  02110-1301  USA

*/

#include "audio_renderer.h"

/** Maximum Number of Audio Renderer Component Instance*/
static OMX_U32 nComponentInstances = 0;


OMX_ERRORTYPE omx_audio_renderer_component_Constructor(
  OMX_COMPONENTTYPE *pComponent,
  OMX_STRING cComponentName)
{
  OMX_ERRORTYPE err = OMX_ErrorNone;
  omx_audio_renderer_component_PrivateType* pPrivate;
  omx_audio_renderer_component_PortType* pPort;
  OMX_U32 i, idx = 0;

  DEBUG(DEB_LEV_FUNCTION_NAME, "%s\n", __func__);

  if((MAX_COMPONENT_INSTANCES > 0) &&
     (nComponentInstances > MAX_COMPONENT_INSTANCES))
    err = OMX_ErrorInsufficientResources;

  if(OMX_ErrorNone == err) {
    if(NULL == pComponent->pComponentPrivate) {
      pComponent->pComponentPrivate = calloc(1, sizeof(omx_audio_renderer_component_PrivateType));
      if(pComponent->pComponentPrivate == NULL) {
	return OMX_ErrorInsufficientResources;
      }
    } else {
      DEBUG(DEB_LEV_FUNCTION_NAME, "In %s, Error Component %x Already Allocated\n", __func__, (int) pComponent->pComponentPrivate);
    }
  }

  /** Calling base sink constructor */
  if(OMX_ErrorNone == err)
    err = omx_base_sink_Constructor(pComponent, cComponentName);

  if(OMX_ErrorNone == err) {

    pPrivate = pComponent->pComponentPrivate;
    pPrivate->sPortTypesParam[OMX_PortDomainAudio].nPorts = NUM_PORTS;

    /** Allocate Ports and call port constructor. */
    pPrivate->ports = calloc(pPrivate->sPortTypesParam[OMX_PortDomainAudio].nPorts, sizeof(omx_base_PortType *));
    if (NULL == pPrivate->ports)
      return OMX_ErrorInsufficientResources;

    for (i=0; i < pPrivate->sPortTypesParam[OMX_PortDomainAudio].nPorts; i++) {
    pPrivate->ports[i] = calloc(1, sizeof(omx_audio_renderer_component_PortType));
    if (NULL == pPrivate->ports[i])
      return OMX_ErrorInsufficientResources;

    }

    /* construct all output ports */
    for(i = 0; i < NUM_INPUT_PORTS; i++) {
      base_audio_port_Constructor(pComponent, &pPrivate->ports[idx], idx, OMX_TRUE);
      idx++;
    }

    /** Domain specific section for the ports. */
    for(i = 0; i < pPrivate->sPortTypesParam[OMX_PortDomainAudio].nPorts; i++) {
      pPort = (omx_audio_renderer_component_PortType *) pPrivate->ports[i];

      pPort->sPortParam.nBufferSize = DEFAULT_OUT_BUFFER_SIZE;

      setHeader(&pPort->pAudioPcmMode,sizeof(OMX_AUDIO_PARAM_PCMMODETYPE));
      pPort->pAudioPcmMode.nPortIndex = i;
      pPort->pAudioPcmMode.nChannels = 2;
      pPort->pAudioPcmMode.eNumData = OMX_NumericalDataSigned;
      pPort->pAudioPcmMode.eEndian = OMX_EndianBig;
      pPort->pAudioPcmMode.bInterleaved = OMX_TRUE;
      pPort->pAudioPcmMode.nBitPerSample = 16;
      pPort->pAudioPcmMode.nSamplingRate = 48000;
      pPort->pAudioPcmMode.ePCMMode = OMX_AUDIO_PCMModeLinear;
    }

    pPrivate->BufferMgmtCallback = omx_audio_renderer_component_BufferMgmtCallback;
    pPrivate->destructor = omx_audio_renderer_component_Destructor;

    nComponentInstances++;

  }

  return err;
}


/** The destructor
  */
OMX_ERRORTYPE omx_audio_renderer_component_Destructor(
  OMX_COMPONENTTYPE *pComponent)
{
  omx_audio_renderer_component_PrivateType* pPrivate = pComponent->pComponentPrivate;
  OMX_U32 i;

  DEBUG(DEB_LEV_FUNCTION_NAME, "%s\n", __func__);

  /* Free port(s) */
  if (pPrivate->ports) {
    for (i=0; i < pPrivate->sPortTypesParam[OMX_PortDomainAudio].nPorts; i++) {
      if(pPrivate->ports[i])
        pPrivate->ports[i]->PortDestructor(pPrivate->ports[i]);
    }
    free(pPrivate->ports);
    pPrivate->ports=NULL;
  }

  pPrivate->BufferMgmtCallback = omx_audio_renderer_component_BufferMgmtCallback;
  omx_base_sink_Destructor(pComponent);

  nComponentInstances--;

  return OMX_ErrorNone;
}

/** This function is used to consume one input buffer
  */
void omx_audio_renderer_component_BufferMgmtCallback(
  OMX_COMPONENTTYPE *pComponent,
  OMX_BUFFERHEADERTYPE* pOutBuffer)
{
  /* Throw away the contents */
  pOutBuffer->nFilledLen = 0;
}
