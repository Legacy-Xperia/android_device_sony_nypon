/**
  src/audio_decoder/audio_decoder.h

  OpenMax audio_decoder component.
  This component implements an audio_decoder stub

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

#ifndef __AUDIO_DECODER_H__
#define __AUDIO_DECODER_H__

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <bellagio/omxcore.h>

#include <OMX_Types.h>
#include <OMX_Component.h>
#include <OMX_Core.h>
#include <OMX_Audio.h>

#include <bellagio/omx_base_filter.h>
#include <bellagio/omx_base_audio_port.h>

#define NUM_INPUT_PORTS   1
#define NUM_OUTPUT_PORTS  1
#define NUM_PORTS (NUM_INPUT_PORTS + NUM_OUTPUT_PORTS)

/** Maximum number of component instances (0 == unlimited) */
#define MAX_COMPONENT_INSTANCES 0

/** Audio Decoder port structure.
  */
DERIVEDCLASS(omx_audio_decoder_component_PortType, omx_base_audio_PortType)
#define omx_audio_decoder_component_PortType_FIELDS omx_base_audio_PortType_FIELDS \
  /** @param pAudioPcmMode Referece to OMX_AUDIO_PARAM_PCMMODETYPE structure*/  \
  OMX_AUDIO_PARAM_PCMMODETYPE pAudioPcmMode;
ENDCLASS(omx_audio_decoder_component_PortType)

/** Twoport component private structure.
* see the define above
*/
DERIVEDCLASS(omx_audio_decoder_component_PrivateType, omx_base_filter_PrivateType)
#define omx_audio_decoder_component_PrivateType_FIELDS omx_base_filter_PrivateType_FIELDS
ENDCLASS(omx_audio_decoder_component_PrivateType)

/* Component private entry points declaration */

OMX_ERRORTYPE omx_audio_decoder_component_Constructor(
  OMX_COMPONENTTYPE *openmaxStandComp,
  OMX_STRING cComponentName);

OMX_ERRORTYPE omx_audio_decoder_component_Destructor(
  OMX_COMPONENTTYPE *openmaxStandComp);

void omx_audio_decoder_component_BufferMgmtCallback(
  OMX_COMPONENTTYPE *openmaxStandComp,
  OMX_BUFFERHEADERTYPE* inputbuffer,
  OMX_BUFFERHEADERTYPE* outputbuffer);

#endif
