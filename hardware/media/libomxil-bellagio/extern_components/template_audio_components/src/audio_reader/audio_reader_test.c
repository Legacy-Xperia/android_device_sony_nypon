/**
  src/audio_reader/audio_reader_test.c

  This simple test application uses an audio_reader and
  content_pipe to read audio data from a file URI

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

#include "audio_reader_test.h"

OMX_BUFFERHEADERTYPE *inBuffer[BUFFER_COUNT_ACTUAL];
OMX_BUFFERHEADERTYPE *outBuffer[BUFFER_COUNT_ACTUAL];

/* Application private date: should go in the component field (segs...) */
appPrivateType* appPriv;

OMX_ERRORTYPE audioreaderEventHandler(
  OMX_HANDLETYPE hComponent,
  OMX_PTR pAppData,
  OMX_EVENTTYPE eEvent,
  OMX_U32 Data1,
  OMX_U32 Data2,
  OMX_PTR pEventData)
{
  DEBUG(DEB_LEV_SIMPLE_SEQ, "Hi there, I am in the %s callback\n", __func__);

  switch(eEvent) {
  case OMX_EventCmdComplete:
    if (Data1 == OMX_CommandStateSet) {
      DEBUG(DEB_LEV_SIMPLE_SEQ, "State changed in ");
      switch ((int)Data2) {
      case OMX_StateInvalid:
	DEBUG(DEB_LEV_SIMPLE_SEQ, "OMX_StateInvalid\n");
	break;
      case OMX_StateLoaded:
	DEBUG(DEB_LEV_SIMPLE_SEQ, "OMX_StateLoaded\n");
	break;
      case OMX_StateIdle:
	DEBUG(DEB_LEV_SIMPLE_SEQ, "OMX_StateIdle\n");
	break;
      case OMX_StateExecuting:
	DEBUG(DEB_LEV_SIMPLE_SEQ, "OMX_StateExecuting\n");
	break;
      case OMX_StatePause:
	DEBUG(DEB_LEV_SIMPLE_SEQ, "OMX_StatePause\n");
	break;
      case OMX_StateWaitForResources:
	DEBUG(DEB_LEV_SIMPLE_SEQ, "OMX_StateWaitForResources\n");
	break;
      }
      tsem_up(appPriv->eventSem);
    } else if (Data1 == OMX_CommandPortEnable){
      DEBUG(DEB_LEV_SIMPLE_SEQ, "In %s Received Port Enable  Event\n",__func__);
      tsem_up(appPriv->eventSem);
    } else if (Data1 == OMX_CommandPortDisable){
      DEBUG(DEB_LEV_SIMPLE_SEQ, "In %s Received Port Disable Event\n",__func__);
      tsem_up(appPriv->eventSem);
    } else {
      DEBUG(DEB_LEV_SIMPLE_SEQ, "Param1 is %i\n", (int) Data1);
      DEBUG(DEB_LEV_SIMPLE_SEQ, "Param2 is %i\n", (int) Data2);

    }
    break;
  case OMX_EventError:
    break;
  case OMX_EventMark:
    break;
  case OMX_EventPortFormatDetected:
    DEBUG(DEB_LEV_SIMPLE_SEQ, "OMX_EventPortFormatDetected\n");
    break;
  case OMX_EventPortSettingsChanged:
    DEBUG(DEB_LEV_SIMPLE_SEQ, "OMX_EventPortSettingsChanged\n");
    break;
  case OMX_EventBufferFlag:
    DEBUG(DEB_LEV_SIMPLE_SEQ, "OMX_EventBufferFlag\n");
    tsem_up(appPriv->eofSem);
    break;
  case OMX_EventResourcesAcquired:
    break;
  case OMX_EventComponentResumed:
    break;
  case OMX_EventDynamicResourcesAvailable:
    break;
  default:
    break;
  }

  return OMX_ErrorNone;
}

OMX_ERRORTYPE audioreaderFillBufferDone(
  OMX_HANDLETYPE hComponent,
  OMX_PTR pAppData,
  OMX_BUFFERHEADERTYPE* pBuffer)
{
  OMX_ERRORTYPE err;

  DEBUG(DEB_LEV_FULL_SEQ, "Hi there, I am in the %s callback.\n", __func__);

  DEBUG(DEB_LEV_PARAMS, "Fill buffer %x\n", (int) pBuffer);

  if(-1 != appPriv->opd)
    write(appPriv->opd,
	  pBuffer->pBuffer + pBuffer->nOffset,
	  pBuffer->nFilledLen);

  if((pBuffer->nFlags & OMX_BUFFERFLAG_EOS) != OMX_BUFFERFLAG_EOS) {
    /* Reschedule the fill buffer request */
    pBuffer->nFilledLen = 0;
    err = OMX_FillThisBuffer(appPriv->handle, pBuffer);
  } else {
    DEBUG(DEB_LEV_FULL_SEQ,"In %s EOS reached\n",__func__);
    tsem_up(appPriv->eofSem);
  }

  return OMX_ErrorNone;
}

OMX_CALLBACKTYPE callbacks = {
  .EventHandler    = audioreaderEventHandler,
  .EmptyBufferDone = NULL,
  .FillBufferDone  = audioreaderFillBufferDone,
};

static void test_component(void)
{
  OMX_PARAM_PORTDEFINITIONTYPE sPortDef;
  OMX_ERRORTYPE err;
  OMX_U32 i,j;

  /*
   * Initialize
   */

  err = OMX_Init();
  if(err != OMX_ErrorNone)
    DEBUG(DEB_LEV_ERR, "OMX_Init() failed\n");

  /*
   * Get handle to audio_reader
   */

  if(OMX_ErrorNone == err) {
    /** Ask the core for a handle to the audio reader component */
    err = OMX_GetHandle(&appPriv->handle, "OMX.nmp.audio_reader", NULL , &callbacks);
    if(err != OMX_ErrorNone)
      DEBUG(DEB_LEV_ERR, "audio_reader OMX_GetHandle failed\n");
  }

  /*
   * Specify content by URI
   */

  if(OMX_ErrorNone == err) {
    OMX_PARAM_CONTENTURITYPE* pContentURI;
    OMX_U32 nSize = (OMX_U32) (sizeof(OMX_PARAM_CONTENTURITYPE) + strlen((char*) appPriv->szURI));
    pContentURI = (OMX_PARAM_CONTENTURITYPE*) calloc(1, nSize);
    if(NULL != pContentURI) {
      setHeader(pContentURI, nSize);
      strcpy((char*)pContentURI->contentURI, (char*) appPriv->szURI);
      err = OMX_SetParameter(appPriv->handle, OMX_IndexParamContentURI, pContentURI);
      free(pContentURI);
    }

  }


  /*
   * Configure audio_reader output port
   */

  setHeader(&sPortDef, sizeof(OMX_PARAM_PORTDEFINITIONTYPE));
  sPortDef.nPortIndex = 0;
  err = OMX_GetParameter(appPriv->handle, OMX_IndexParamPortDefinition, &sPortDef);
  sPortDef.nBufferCountActual = BUFFER_COUNT_ACTUAL;
  err = OMX_SetParameter(appPriv->handle, OMX_IndexParamPortDefinition, &sPortDef);
  if(err != OMX_ErrorNone){
    DEBUG(DEB_LEV_ERR, "Error in getting OMX_PORT_PARAM_TYPE parameter\n");
    exit(1);
  }

  /*
   * Transition to idle state
   */

  /* Allocate buffers for output port */
  err = OMX_SendCommand(appPriv->handle, OMX_CommandStateSet, OMX_StateIdle, NULL);
  for(j=0; j < BUFFER_COUNT_ACTUAL; j++) {
    err = OMX_AllocateBuffer(appPriv->handle, &outBuffer[j], 0, NULL, DEFAULT_OUT_BUFFER_SIZE);
    if (err != OMX_ErrorNone) {
      DEBUG(DEB_LEV_ERR, "Error on AllocateBuffer in %i %i\n",(int)j, err);
      exit(1);
    }
  }

  tsem_down(appPriv->eventSem); /* Wait for command to complete */

  /*
   * Transition to executing state
   */

  err = OMX_SendCommand(appPriv->handle, OMX_CommandStateSet, OMX_StateExecuting, NULL);
  tsem_down(appPriv->eventSem);  /* Wait for command to complete */

  /*
   * Execute
   */

  for(i = 0; i < BUFFER_COUNT_ACTUAL; i++)
    err = OMX_FillThisBuffer(appPriv->handle, outBuffer[i]);
  tsem_down(appPriv->eofSem); /* Wait for EOS */

  /*
   * Transition to idle state
   */

  err = OMX_SendCommand(appPriv->handle, OMX_CommandStateSet, OMX_StateIdle, NULL);
  tsem_down(appPriv->eventSem);  /* Wait for command to complete */

  /*
   * Transition to loaded state
   */

  err = OMX_SendCommand(appPriv->handle, OMX_CommandStateSet, OMX_StateLoaded, NULL);
  for(j=0;j<BUFFER_COUNT_ACTUAL;j++)
    err = OMX_FreeBuffer(appPriv->handle, 0, inBuffer[j]);
  tsem_down(appPriv->eventSem);   /* Wait for commands to complete */

  /*
   * De-initialize
   */

  OMX_FreeHandle(appPriv->handle);

}

static void usage(void) {
  printf("\n");
  printf("Usage: audio_reader [options] <URI>\n");
  printf("\nOptions:\n");
  printf("       -o outfile: If this option is specified, the output stream is written to outfile\n");
  printf("       -h        : Displays this help\n");
  printf("\n");
  exit(1);
}

static void options(int argc, char** argv)
{
  char* szOutFile = NULL;
  int i;

  appPriv->opd = -1;
  appPriv->szURI = (OMX_S8*) "";

  for(i = 1; i < argc; i++) {
    if(argv[i][0] == '-') {
      switch(argv[i][1]) {
      case 'o':
	szOutFile = argv[++i];
	break;
      case 'h':
      default:
	usage();
	break;
      }
    } else break;
  }

  if(NULL != szOutFile) {
    appPriv->opd = open(szOutFile, O_WRONLY | O_CREAT, 0);
    if(-1 == appPriv->opd) {
      perror("audio_reader_test: ");
      exit(EXIT_FAILURE);
    }
  }

  if((argc - i) == 1) {
    appPriv->szURI = (OMX_S8*) argv[i];
  } else usage();

}

int main(int argc, char** argv)
{
  /* Initialize application private data */
  appPriv = malloc(sizeof(appPrivateType));
  appPriv->eventSem = malloc(sizeof(tsem_t));
  tsem_init(appPriv->eventSem, 0);
  appPriv->eofSem = malloc(sizeof(tsem_t));
  tsem_init(appPriv->eofSem, 0);

  /* process command line options */
  options(argc, argv);

  /* test the component */
  test_component();

  /* tidyup */
  free(appPriv->eventSem);
  free(appPriv->eofSem);
  free(appPriv);

  return EXIT_SUCCESS;
}

