/**
  src/audio_reader/audio_reader.h

  OpenMax audio_reader component.
  This component uses a content_pipe to read audio data
  and produces a single output stream.

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

#ifndef __AUDIO_READER_H__
#define __AUDIO_READER_H__

#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <bellagio/omxcore.h>

#include <OMX_Types.h>
#include <OMX_Component.h>
#include <OMX_Core.h>
#include <OMX_ContentPipe.h>
#include <OMX_Audio.h>

#include <bellagio/omx_base_audio_port.h>
#include <bellagio/omx_base_source.h>

/** Maximum number of component instances (0 == unlimited) */
#define MAX_COMPONENT_INSTANCES 0

/* Maximum length of a uniform resource identifier (URI) */
#define MAX_URI_SIZE 256

#define NUM_INPUT_PORTS   0
#define NUM_OUTPUT_PORTS  1
#define NUM_PORTS (NUM_INPUT_PORTS + NUM_OUTPUT_PORTS)

/** audio_reader port structure.
  */
DERIVEDCLASS(omx_audio_reader_component_PortType, omx_base_audio_PortType)
#define omx_audio_reader_component_PortType_FIELDS omx_base_audio_PortType_FIELDS \
  /** @param pAudioPcmMode Referece to OMX_AUDIO_PARAM_PCMMODETYPE structure*/  \
  OMX_AUDIO_PARAM_PCMMODETYPE pAudioPcmMode;
ENDCLASS(omx_audio_reader_component_PortType)

/** audio_reader private structure.
*/
DERIVEDCLASS(omx_audio_reader_component_PrivateType, omx_base_source_PrivateType)
#define omx_audio_reader_component_PrivateType_FIELDS omx_base_source_PrivateType_FIELDS \
  /** @param hPipe The content pipe used by the component to access content*/  \
  CP_PIPETYPE* hPipe; \
  /** @param szURI The URI that identifies the target content*/ \
  OMX_S8 szURI[MAX_URI_SIZE]; \
  /** @param eSeekMode The seek mode (fast or accurate) */ \
  OMX_TIME_SEEKMODETYPE eSeekMode;
ENDCLASS(omx_audio_reader_component_PrivateType)

/* Component private entry points declaration */

OMX_ERRORTYPE omx_audio_reader_component_Constructor(
  OMX_COMPONENTTYPE *openmaxStandComp,
  OMX_STRING cComponentName);

OMX_ERRORTYPE omx_audio_reader_component_Destructor(
  OMX_COMPONENTTYPE *openmaxStandComp);

void omx_audio_reader_component_BufferMgmtCallback(
  OMX_COMPONENTTYPE *openmaxStandComp,
  OMX_BUFFERHEADERTYPE* outputbuffer);

OMX_ERRORTYPE omx_audio_reader_component_DoStateSet(
  OMX_COMPONENTTYPE *pComponent,
  OMX_U32 destinationState);

OMX_ERRORTYPE omx_audio_reader_component_SetParameter(
  OMX_HANDLETYPE hComponent,
  OMX_INDEXTYPE nParamIndex,
  OMX_PTR pParameter);

#endif
