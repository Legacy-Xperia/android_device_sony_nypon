/**
  test/components/audio/omxaudiocapnplay.h

  This test application is supposed to exchange voice over the network
  using some client server mechanism.

  Copyright (C) 2007-2009 STMicroelectronics
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

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <OMX_Core.h>
#include <OMX_Component.h>
#include <OMX_Types.h>
#include <bellagio/tsemaphore.h>
#include <user_debug_levels.h>

#define BUFFER_OUT_SIZE 16384

/** Specification version*/
#define VERSIONMAJOR    1
#define VERSIONMINOR    1
#define VERSIONREVISION 0
#define VERSIONSTEP     0

/* Application's private data */
typedef struct appPrivateType{
  tsem_t* audiosrcEventSem;
  tsem_t* audioencEventSem;
  tsem_t* audiodecEventSem;
  tsem_t* audiosinkEventSem;
  tsem_t* eofSem;
  OMX_HANDLETYPE audiosrcHandle;
  OMX_HANDLETYPE audioencHandle;
  OMX_HANDLETYPE audiodecHandle;
  OMX_HANDLETYPE audiosinkHandle;
}appPrivateType;

/* Audio Packet Structure exchanged between the client and structure. */
typedef struct AudioPacket{
  OMX_U32 nSize;
  OMX_S8 *data;
}AudioPacket;

/* Callback prototypes for audio source, encoder, decoder and sink */
OMX_ERRORTYPE audiosrcEventHandler(
  OMX_HANDLETYPE hComponent,
  OMX_PTR pAppData,
  OMX_EVENTTYPE eEvent,
  OMX_U32 Data1,
  OMX_U32 Data2,
  OMX_PTR pEventData);

OMX_ERRORTYPE audioencEventHandler(
  OMX_HANDLETYPE hComponent,
  OMX_PTR pAppData,
  OMX_EVENTTYPE eEvent,
  OMX_U32 Data1,
  OMX_U32 Data2,
  OMX_PTR pEventData);

OMX_ERRORTYPE audioencFillBufferDone(
  OMX_HANDLETYPE hComponent,
  OMX_PTR pAppData,
  OMX_BUFFERHEADERTYPE* pBuffer);

OMX_ERRORTYPE audiodecEventHandler(
  OMX_HANDLETYPE hComponent,
  OMX_PTR pAppData,
  OMX_EVENTTYPE eEvent,
  OMX_U32 Data1,
  OMX_U32 Data2,
  OMX_PTR pEventData);

OMX_ERRORTYPE audiodecEmptyBufferDone(
  OMX_HANDLETYPE hComponent,
  OMX_PTR pAppData,
  OMX_BUFFERHEADERTYPE* pBuffer);

OMX_ERRORTYPE audiosinkEventHandler(
  OMX_HANDLETYPE hComponent,
  OMX_PTR pAppData,
  OMX_EVENTTYPE eEvent,
  OMX_U32 Data1,
  OMX_U32 Data2,
  OMX_PTR pEventData);

/** display general help  */
void display_help();

/** this function sets the color converter and audio sink port characteristics
  * based on the audio decoder output port settings
  */
int setPortParameters();


