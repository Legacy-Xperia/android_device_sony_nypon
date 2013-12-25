/**
  src/audio_capturer/audio_capturer.c

  OpenMax audio capturer component.
  This component implements an audio_capturer that accepts a
  single audio PCM stream.

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

#include "audio_capturer.h"

/** Maximum Number of Audio Capturer Component Instance*/
static OMX_U32 nComponentInstances = 0;

/** This function is used to consume one input buffer
  */
void omx_audio_capturer_component_BufferMgmtCallback(
  OMX_COMPONENTTYPE *pComponent,
  OMX_BUFFERHEADERTYPE* pOutBuffer)
{
  int length = pOutBuffer->nAllocLen - pOutBuffer->nFilledLen - pOutBuffer->nOffset;
  memset(pOutBuffer->pBuffer - pOutBuffer->nOffset, 'A', length);
  pOutBuffer->nFilledLen += length;
}

OMX_ERRORTYPE omx_audio_capturer_SetParameter(
  OMX_HANDLETYPE hComponent,
  OMX_INDEXTYPE nParamIndex,
  OMX_PTR ComponentParameterStructure)
{
  int omxErr = OMX_ErrorNone;
  OMX_AUDIO_PARAM_PORTFORMATTYPE *pAudioPortFormat;
  OMX_U32 portIndex;

  if (ComponentParameterStructure == NULL) {
    return OMX_ErrorBadParameter;
  }

  DEBUG(DEB_LEV_SIMPLE_SEQ, "   Setting parameter %i\n", nParamIndex);

  switch(nParamIndex) {

  case OMX_IndexParamAudioPortFormat:
    pAudioPortFormat = (OMX_AUDIO_PARAM_PORTFORMATTYPE*)ComponentParameterStructure;
    portIndex = pAudioPortFormat->nPortIndex;
    /*Check Structure Header and verify component state*/
    omxErr = omx_base_component_ParameterSanityCheck(hComponent, portIndex, pAudioPortFormat, sizeof(OMX_AUDIO_PARAM_PORTFORMATTYPE));
    if(omxErr!=OMX_ErrorNone) {
      DEBUG(DEB_LEV_ERR, "In %s Parameter Check Error=%x\n", __func__, omxErr);
      break;
    }
    if (portIndex != 0) {
      return OMX_ErrorBadPortIndex;
    }
    break;

  default: /*Call the base component function*/
    return omx_base_component_SetParameter(hComponent, nParamIndex, ComponentParameterStructure);
  }

  return omxErr;
}

OMX_ERRORTYPE omx_audio_capturer_GetParameter(
  OMX_HANDLETYPE hComponent,
  OMX_INDEXTYPE nParamIndex,
  OMX_PTR ComponentParameterStructure)
{
  OMX_AUDIO_PARAM_PORTFORMATTYPE *pAudioPortFormat;
  OMX_ERRORTYPE err = OMX_ErrorNone;
  OMX_COMPONENTTYPE *pComponent = (OMX_COMPONENTTYPE*)hComponent;
  omx_audio_capturer_component_PrivateType* pPrivate = (omx_audio_capturer_component_PrivateType*) pComponent->pComponentPrivate;
  omx_base_audio_PortType *pPort = (omx_base_audio_PortType *) pPrivate->ports[OMX_BASE_SOURCE_OUTPUTPORT_INDEX];

  if (ComponentParameterStructure == NULL) {
    return OMX_ErrorBadParameter;
  }
  DEBUG(DEB_LEV_SIMPLE_SEQ, "   Getting parameter %i\n", nParamIndex);
  /* Check which structure we are being fed and fill its header */
  switch(nParamIndex) {
  case OMX_IndexParamAudioInit:
    if ((err = checkHeader(ComponentParameterStructure, sizeof(OMX_PORT_PARAM_TYPE))) != OMX_ErrorNone) {
      break;
    }
    memcpy(ComponentParameterStructure, &pPrivate->sPortTypesParam[OMX_PortDomainAudio], sizeof(OMX_PORT_PARAM_TYPE));
    break;
  case OMX_IndexParamAudioPortFormat:
    pAudioPortFormat = (OMX_AUDIO_PARAM_PORTFORMATTYPE*)ComponentParameterStructure;
    if ((err = checkHeader(ComponentParameterStructure, sizeof(OMX_AUDIO_PARAM_PORTFORMATTYPE))) != OMX_ErrorNone) {
      break;
    }
    if (pAudioPortFormat->nPortIndex < 1) {
      memcpy(pAudioPortFormat, &pPort->sAudioParam, sizeof(OMX_AUDIO_PARAM_PORTFORMATTYPE));
    } else {
      return OMX_ErrorBadPortIndex;
    }
    break;
  case OMX_IndexParamAudioPcm:
    if(((OMX_AUDIO_PARAM_PCMMODETYPE*)ComponentParameterStructure)->nPortIndex != 0) {
      return OMX_ErrorBadParameter;
    }
    if ((err = checkHeader(ComponentParameterStructure, sizeof(OMX_AUDIO_PARAM_PCMMODETYPE))) != OMX_ErrorNone) {
      break;
    }
    omx_audio_capturer_component_PortType* pPort;
    pPort = (omx_audio_capturer_component_PortType *) pPrivate->ports[0];
    memcpy(ComponentParameterStructure, &pPort->sPCMModeParam, sizeof(OMX_AUDIO_PARAM_PCMMODETYPE));
    break;
  default: /*Call the base component function*/
  return omx_base_component_GetParameter(hComponent, nParamIndex, ComponentParameterStructure);
  }
  return err;
}

/** The destructor
  */
OMX_ERRORTYPE omx_audio_capturer_component_Destructor(
  OMX_COMPONENTTYPE *pComponent)
{
  omx_audio_capturer_component_PrivateType* pPrivate = pComponent->pComponentPrivate;
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

  pPrivate->BufferMgmtCallback = omx_audio_capturer_component_BufferMgmtCallback;
  omx_base_source_Destructor(pComponent);

  nComponentInstances--;

  return OMX_ErrorNone;
}

OMX_ERRORTYPE omx_audio_capturer_component_Constructor(
  OMX_COMPONENTTYPE *pComponent,
  OMX_STRING cComponentName)
{
  OMX_ERRORTYPE err = OMX_ErrorNone;
  omx_audio_capturer_component_PrivateType* pPrivate;
  omx_audio_capturer_component_PortType* pPort;

  DEBUG(DEB_LEV_FUNCTION_NAME, "%s\n", __func__);

  if(OMX_ErrorNone == err) {
    if(NULL == pComponent->pComponentPrivate) {
      pComponent->pComponentPrivate = calloc(1, sizeof(omx_audio_capturer_component_PrivateType));
      if(pComponent->pComponentPrivate == NULL) {
	return OMX_ErrorInsufficientResources;
      }
    } else {
      DEBUG(DEB_LEV_FUNCTION_NAME, "In %s, Error Component %x Already Allocated\n", __func__, (int) pComponent->pComponentPrivate);
    }
  }

  /** Calling base source constructor */
  if(OMX_ErrorNone == err)
    err = omx_base_source_Constructor(pComponent, cComponentName);

  if(OMX_ErrorNone == err) {

    pPrivate = pComponent->pComponentPrivate;
    pPrivate->sPortTypesParam[OMX_PortDomainAudio].nPorts = NUM_PORTS;

    /** Allocate Ports and call port constructor. */
    pPrivate->ports = calloc(pPrivate->sPortTypesParam[OMX_PortDomainAudio].nPorts, sizeof(omx_base_PortType *));
    if (NULL == pPrivate->ports)
      return OMX_ErrorInsufficientResources;

    pPrivate->ports[0] = calloc(1, sizeof(omx_audio_capturer_component_PortType));
    if (NULL == pPrivate->ports[0])
      return OMX_ErrorInsufficientResources;

    /* construct output port */
    base_audio_port_Constructor(pComponent, &pPrivate->ports[0], 0, OMX_FALSE);

    /** Domain specific section for the ports. */
    pPort = (omx_audio_capturer_component_PortType *) pPrivate->ports[0];
    pPort->sPortParam.nBufferSize = DEFAULT_OUT_BUFFER_SIZE;

    /* OMX_AUDIO_PARAM_PCMMODETYPE */
    setHeader(&pPort->sPCMModeParam, sizeof(OMX_AUDIO_PARAM_PCMMODETYPE));
    pPort->sPCMModeParam.nPortIndex = 0;
    pPort->sPCMModeParam.nChannels = 2;
    pPort->sPCMModeParam.eNumData = OMX_NumericalDataSigned;
    pPort->sPCMModeParam.eEndian = OMX_EndianLittle;
    pPort->sPCMModeParam.bInterleaved = OMX_TRUE;
    pPort->sPCMModeParam.nBitPerSample = 16;
    pPort->sPCMModeParam.nSamplingRate = 8000;
    pPort->sPCMModeParam.ePCMMode = OMX_AUDIO_PCMModeLinear;
    pPort->sPCMModeParam.eChannelMapping[0] = OMX_AUDIO_ChannelNone;

    pComponent->GetParameter = omx_audio_capturer_GetParameter;
    pComponent->SetParameter = omx_audio_capturer_SetParameter;
    pPrivate->BufferMgmtCallback = omx_audio_capturer_component_BufferMgmtCallback;
    pPrivate->destructor = omx_audio_capturer_component_Destructor;


  }

  if((MAX_COMPONENT_INSTANCES > 0) &&
     (nComponentInstances >= MAX_COMPONENT_INSTANCES)) {
    return OMX_ErrorInsufficientResources;
  } else nComponentInstances++;

  return err;
}

