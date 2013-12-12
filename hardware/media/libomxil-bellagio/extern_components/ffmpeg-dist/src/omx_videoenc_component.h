/**
  src/omx_videoenc_component.h

  This component implements an MPEG-4 video encoder.
  The MPEG-4 Video encoder is based on the FFmpeg software library.

  Copyright (C) 2007-2009 STMicroelectronics
  Copyright (C) 2007-2009 Nokia Corporation and/or its subsidiary(-ies)

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

#ifndef _OMX_VIDEOENC_COMPONENT_H_
#define _OMX_VIDEOENC_COMPONENT_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <OMX_Types.h>
#include <OMX_Component.h>
#include <OMX_Core.h>
#include <string.h>
#include <bellagio/omx_base_filter.h>

/* Specific include files */
#if FFMPEG_LIBNAME_HEADERS
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
#else
#include <ffmpeg/avcodec.h>
#include <ffmpeg/avformat.h>
#include <ffmpeg/avutil.h>
#include <ffmpeg/swscale.h>
#endif

#define VIDEO_ENC_BASE_NAME "OMX.st.video_encoder"
#define VIDEO_ENC_MPEG4_NAME "OMX.st.video_encoder.mpeg4"
#define VIDEO_ENC_MPEG4_ROLE "video_encoder.mpeg4"

/** Video Encoder component private structure.
  */
DERIVEDCLASS(omx_videoenc_component_PrivateType, omx_base_filter_PrivateType)
#define omx_videoenc_component_PrivateType_FIELDS omx_base_filter_PrivateType_FIELDS \
  /** @param avCodec pointer to the FFmpeg video encoder */ \
  AVCodec *avCodec;  \
  /** @param avCodecContext pointer to FFmpeg encoder context  */ \
  AVCodecContext *avCodecContext;  \
  /** @param picture pointer to FFmpeg AVFrame  */ \
  AVFrame *picture; \
  /** @param pVideoMpeg4 Reference to OMX_VIDEO_PARAM_MPEG4TYPE structure*/  \
  OMX_VIDEO_PARAM_MPEG4TYPE pVideoMpeg4;  \
  OMX_BOOL avcodecReady;  \
  /** @param minBufferLength Field that stores the minimum allowed size for FFmpeg encoder */ \
  OMX_U16 minBufferLength; \
  /** @param isFirstBuffer Field that the buffer is the first buffer */ \
  OMX_S32 isFirstBuffer;\
  /** @param isNewBuffer Field that indicate a new buffer has arrived*/ \
  OMX_S32 isNewBuffer;  \
  /** @param video_encoding_type Field that indicate the supported video format of video encoder */ \
  OMX_U32 video_encoding_type;   \
  /** @param eOutFramePixFmt Field that indicate output frame pixel format */ \
  enum PixelFormat eOutFramePixFmt;
ENDCLASS(omx_videoenc_component_PrivateType)

/* Component private entry points enclaration */
OMX_ERRORTYPE omx_videoenc_component_Constructor(OMX_COMPONENTTYPE *openmaxStandComp,OMX_STRING cComponentName);
OMX_ERRORTYPE omx_videoenc_component_Destructor(OMX_COMPONENTTYPE *openmaxStandComp);
OMX_ERRORTYPE omx_videoenc_component_Init(OMX_COMPONENTTYPE *openmaxStandComp);
OMX_ERRORTYPE omx_videoenc_component_Deinit(OMX_COMPONENTTYPE *openmaxStandComp);
OMX_ERRORTYPE omx_videoenc_component_MessageHandler(OMX_COMPONENTTYPE*,internalRequestMessageType*);

void omx_videoenc_component_BufferMgmtCallback(
  OMX_COMPONENTTYPE *openmaxStandComp,
  OMX_BUFFERHEADERTYPE* inputbuffer,
  OMX_BUFFERHEADERTYPE* outputbuffer);

OMX_ERRORTYPE omx_videoenc_component_GetParameter(
  OMX_HANDLETYPE hComponent,
  OMX_INDEXTYPE nParamIndex,
  OMX_PTR ComponentParameterStructure);

OMX_ERRORTYPE omx_videoenc_component_SetParameter(
  OMX_HANDLETYPE hComponent,
  OMX_INDEXTYPE nParamIndex,
  OMX_PTR ComponentParameterStructure);

OMX_ERRORTYPE omx_videoenc_component_ComponentRoleEnum(
  OMX_HANDLETYPE hComponent,
  OMX_U8 *cRole,
  OMX_U32 nIndex);

void SetInternalVideoEncParameters(OMX_COMPONENTTYPE *openmaxStandComp);


#endif
