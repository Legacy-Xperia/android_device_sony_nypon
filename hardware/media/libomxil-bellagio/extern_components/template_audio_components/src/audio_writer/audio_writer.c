/**
  src/audio_writer/audio_writer.c

  OpenMax audio_writer component.
  This component uses a content_pipe to read audio data
  and produces a single output stream.

  Copyright (C) 2007-2009 STMicroelectronics
  Copyright (C) 2007-2009  Nokia Corporation and/or its subsidiary(-ies).

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

#include "audio_writer.h"

static OMX_U32 nComponentInstances = 0;

/** This function is used to write one output buffer
  */
void omx_audio_writer_component_BufferMgmtCallback(
  OMX_COMPONENTTYPE *pComponent,
  OMX_BUFFERHEADERTYPE* pInBuffer)
{
  omx_audio_writer_component_PrivateType *pPrivate = pComponent->pComponentPrivate;
  CPresult res;

  /* Use the pipe, luke */
  res = pPrivate->hPipe->Write((CPhandle*) pPrivate->hPipe,
			       (CPbyte*) (pInBuffer->pBuffer + pInBuffer->nOffset),
			       pInBuffer->nFilledLen);
  pInBuffer->nFilledLen = 0;
}

OMX_ERRORTYPE omx_audio_writer_component_SetParameter(
  OMX_HANDLETYPE hComponent,
  OMX_INDEXTYPE nParamIndex,
  OMX_PTR pParameter)
{
  OMX_ERRORTYPE err = OMX_ErrorNone;
  OMX_COMPONENTTYPE *pComponent = (OMX_COMPONENTTYPE*) hComponent;
  omx_audio_writer_component_PrivateType *pPrivate = pComponent->pComponentPrivate;

  DEBUG(DEB_LEV_SIMPLE_SEQ, "Setting parameter %i\n", nParamIndex);

  if (pParameter == NULL)
    return OMX_ErrorBadParameter;

  switch(nParamIndex) {
    case OMX_IndexParamCustomContentPipe:
      DEBUG(DEB_LEV_FUNCTION_NAME, "%s: OMX_IndexParamCustomContentPipe", __func__);
      OMX_PARAM_CONTENTPIPETYPE* pContentPipe = (OMX_PARAM_CONTENTPIPETYPE*) pParameter;
      pPrivate->hPipe = (CP_PIPETYPE*) pContentPipe->hPipe;
      break;
    case OMX_IndexParamContentURI:
      DEBUG(DEB_LEV_FUNCTION_NAME, "%s: OMX_IndexParamContentURI", __func__);
      OMX_PARAM_CONTENTURITYPE* pContentType = (OMX_PARAM_CONTENTURITYPE*) pParameter;
      memcpy(pPrivate->szURI, pContentType->contentURI, MAX_URI_SIZE);
      break;
    default:
      err = omx_base_component_SetParameter(hComponent, nParamIndex, pParameter);
      break;
  }

  return err;
}


/**
 * A component that uses content pipes shall, by default, attempt to acquire any content
 * pipes it needs when transitioned into executing unless the IL client itself has
 * specified the content pipe
 *
 * @param pComponent the openmax component which state is to be changed
 * @param destinationState the requested target state
 *
 * @return OMX_ErrorNotImplemented if the state change is noty handled in this base class, but needs
 * a specific handling
 */
OMX_ERRORTYPE omx_audio_writer_component_DoStateSet(
  OMX_COMPONENTTYPE *pComponent,
  OMX_U32 destinationState)
{
  omx_audio_writer_component_PrivateType *pPrivate = pComponent->pComponentPrivate;
  OMX_ERRORTYPE err = OMX_ErrorNone;
  CPresult res;

  /*
   * On initial transition to Executing, acquire and open the pipe
   */

  if((destinationState == OMX_StateExecuting) && (pPrivate->state != OMX_StatePause)) {
    /* Pipe not specified by client, try to acquire */
    if(NULL == pPrivate->hPipe) {
      err = OMX_GetContentPipe((OMX_HANDLETYPE*) &pPrivate->hPipe, (CPstring) pPrivate->szURI);
      if(OMX_ErrorNone != err) {
	DEBUG(DEB_LEV_ERR, "OMX_GetContentPipe failed");
	err = OMX_ErrorContentPipeCreationFailed;
      }
    }
    /* Have pipe, try to open */
    if(OMX_ErrorNone == err) {
      res = pPrivate->hPipe->Create((CPhandle*) pPrivate->hPipe, (CPstring) pPrivate->szURI);
      if(res != 0x00000000) {
	DEBUG(DEB_LEV_ERR, "ContentPipe open failed");
	err = OMX_ErrorContentPipeOpenFailed;
      }
    }
  }

  /*
   * On transition back to Idle, close the pipe
   */

  else if((destinationState == OMX_StateIdle) && (pPrivate->state != OMX_StateLoaded)) {
    res = pPrivate->hPipe->Close((CPhandle*) pPrivate->hPipe);
    if(res != 0x00000000) {
      DEBUG(DEB_LEV_ERR, "ContentPipe close failed");
      /* ignore error, complete transition to idle? */
    }
  }

  /*
   * On transition to Loaded, release the pipe
   */

  else if(destinationState == OMX_StateLoaded) {
    if(NULL != pPrivate->hPipe) {
      free(pPrivate->hPipe);
      pPrivate->hPipe = NULL;
    }
  }

  if(OMX_ErrorNone == err)
    err = omx_base_component_DoStateSet(pComponent, destinationState);

  return err;
}

/** The destructor
  */
OMX_ERRORTYPE omx_audio_writer_component_Destructor(
  OMX_COMPONENTTYPE *pComponent)
{
  omx_audio_writer_component_PrivateType* pPrivate = pComponent->pComponentPrivate;
  OMX_U32 i;

  /* frees port/s */
  if (pPrivate->ports) {
    for (i=0; i < pPrivate->sPortTypesParam[OMX_PortDomainAudio].nPorts; i++) {
      if(pPrivate->ports[i])
        pPrivate->ports[i]->PortDestructor(pPrivate->ports[i]);
    }
    free(pPrivate->ports);
    pPrivate->ports=NULL;
  }

  DEBUG(DEB_LEV_FUNCTION_NAME, "Destructor of audiodecoder component is called\n");
  omx_base_sink_Destructor(pComponent);

  nComponentInstances--;

  return OMX_ErrorNone;
}

/** The constructor
  */
OMX_ERRORTYPE omx_audio_writer_component_Constructor(
  OMX_COMPONENTTYPE *pComponent,
  OMX_STRING cComponentName)
{
  OMX_ERRORTYPE err = OMX_ErrorNone;
  omx_audio_writer_component_PrivateType* pPrivate;
  omx_audio_writer_component_PortType* pPort;
  OMX_U32 i, idx = 0;

  DEBUG(DEB_LEV_FUNCTION_NAME, "In %s, allocating component\n", __func__);

  if((MAX_COMPONENT_INSTANCES > 0) &&
     (nComponentInstances > MAX_COMPONENT_INSTANCES))
    err = OMX_ErrorInsufficientResources;

  if(OMX_ErrorNone == err) {
    if(NULL == pComponent->pComponentPrivate) {
      pComponent->pComponentPrivate = calloc(1, sizeof(omx_audio_writer_component_PrivateType));
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
    pPrivate->ports[i] = calloc(1, sizeof(omx_audio_writer_component_PortType));
    if (NULL == pPrivate->ports[i])
      return OMX_ErrorInsufficientResources;

    }

    /* construct all input ports */
    for(i = 0; i < NUM_INPUT_PORTS; i++) {
      base_audio_port_Constructor(pComponent, &pPrivate->ports[idx], idx, OMX_TRUE);
      idx++;
    }

    /* construct all output ports */
    for(i = 0; i < NUM_OUTPUT_PORTS; i++) {
      base_audio_port_Constructor(pComponent, &pPrivate->ports[idx], idx, OMX_FALSE);
      idx++;
    }

    /** Domain specific section for the ports. */
    for(i = 0; i < pPrivate->sPortTypesParam[OMX_PortDomainAudio].nPorts; i++) {
      pPort = (omx_audio_writer_component_PortType *) pPrivate->ports[i];

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

    pComponent->SetParameter = omx_audio_writer_component_SetParameter;
    pPrivate->BufferMgmtCallback = omx_audio_writer_component_BufferMgmtCallback;
    pPrivate->DoStateSet = omx_audio_writer_component_DoStateSet;
    pPrivate->destructor = omx_audio_writer_component_Destructor;
    memset(pPrivate->szURI, 0x00, MAX_URI_SIZE);
    pPrivate->hPipe = NULL;

    nComponentInstances++;

  }

  return err;
}

