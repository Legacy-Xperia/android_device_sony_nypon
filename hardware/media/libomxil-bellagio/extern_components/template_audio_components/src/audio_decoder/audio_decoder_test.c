/**
  src/audio_decoder/audio_decoder_test.c

  This simple test application take an input stream from a file,
  passes it to the audio_decoder component and writes the output
  stream to a file.

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

#include "audio_decoder_test.h"

OMX_BUFFERHEADERTYPE *inBuffer[BUFFER_COUNT_ACTUAL];
OMX_BUFFERHEADERTYPE *outBuffer[BUFFER_COUNT_ACTUAL];

/* Application private date: should go in the component field (segs...) */
appPrivateType* appPriv;

OMX_ERRORTYPE audiodecoderEventHandler(
  OMX_HANDLETYPE hComponent,
  OMX_PTR pAppData,
  OMX_EVENTTYPE eEvent,
  OMX_U32 Data1,
  OMX_U32 Data2,
  OMX_PTR pEventData)
{
  DEBUG(DEB_LEV_SIMPLE_SEQ, "Hi there, I am in the %s callback\n", __func__);

  if (Data1 == OMX_CommandStateSet) {
    DEBUG(DEB_LEV_SIMPLE_SEQ, "State changed in ");
    switch ((int) Data2) {
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
    tsem_up(appPriv->eofSem);
  }

  return OMX_ErrorNone;
}

OMX_ERRORTYPE audiodecoderFillBufferDone(
  OMX_HANDLETYPE hComponent,
  OMX_PTR pAppData,
  OMX_BUFFERHEADERTYPE* pBuffer)
{
  OMX_ERRORTYPE err;

  DEBUG(DEB_LEV_FULL_SEQ, "%s: Fill buffer %p\n", __func__, pBuffer);

  /* Write output data to file */
  write(appPriv->opd, pBuffer->pBuffer, pBuffer->nFilledLen);

  /* Reschedule the fill buffer request */
  pBuffer->nFilledLen = 0;
  err = OMX_FillThisBuffer(appPriv->decoderHandle, pBuffer);

  return OMX_ErrorNone;
}

OMX_ERRORTYPE audiodecoderEmptyBufferDone(
  OMX_HANDLETYPE hComponent,
  OMX_PTR pAppData,
  OMX_BUFFERHEADERTYPE* pBuffer)
{
  OMX_ERRORTYPE err;
  int count;

  DEBUG(DEB_LEV_FULL_SEQ, "%s: Fill buffer %p\n", __func__, pBuffer);

  /* Read input data from file */
  pBuffer->nOffset = 0;
  count = read(appPriv->ipd, pBuffer->pBuffer, DEFAULT_IN_BUFFER_SIZE);
  if(count != DEFAULT_IN_BUFFER_SIZE)
    pBuffer->nFlags = pBuffer->nFlags | OMX_BUFFERFLAG_EOS;
  pBuffer->nFilledLen = count;

  /* Reschedule the empty buffer request */
  err = OMX_EmptyThisBuffer(appPriv->decoderHandle, pBuffer);

  return OMX_ErrorNone;
}

OMX_CALLBACKTYPE audio_decoder_callbacks = {
  .EventHandler    = audiodecoderEventHandler,
  .EmptyBufferDone = audiodecoderEmptyBufferDone,
  .FillBufferDone  = audiodecoderFillBufferDone,
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
   * Get handle to audio_decoder
   */

  if(OMX_ErrorNone == err) {
    /** Ask the core for a handle to the audio reader component */
    err = OMX_GetHandle(&appPriv->decoderHandle, "OMX.nmp.audio_decoder", NULL , &audio_decoder_callbacks);
    if(err != OMX_ErrorNone)
      DEBUG(DEB_LEV_ERR, "audio_decoder OMX_GetHandle failed\n");
  }

  /*
   * Configure input port
   */

  setHeader(&sPortDef, sizeof(OMX_PARAM_PORTDEFINITIONTYPE));
  sPortDef.nPortIndex = 0;
  err = OMX_GetParameter(appPriv->decoderHandle, OMX_IndexParamPortDefinition, &sPortDef);
  sPortDef.nBufferCountActual = BUFFER_COUNT_ACTUAL;
  err = OMX_SetParameter(appPriv->decoderHandle, OMX_IndexParamPortDefinition, &sPortDef);
  if(err != OMX_ErrorNone){
    DEBUG(DEB_LEV_ERR, "Error in getting OMX_PORT_PARAM_TYPE parameter\n");
    exit(EXIT_FAILURE);
  }

  /*
   * Configure output port
   */

  setHeader(&sPortDef, sizeof(OMX_PARAM_PORTDEFINITIONTYPE));
  sPortDef.nPortIndex = 1;
  err = OMX_GetParameter(appPriv->decoderHandle, OMX_IndexParamPortDefinition, &sPortDef);
  sPortDef.nBufferCountActual = BUFFER_COUNT_ACTUAL;
  err = OMX_SetParameter(appPriv->decoderHandle, OMX_IndexParamPortDefinition, &sPortDef);
  if(err != OMX_ErrorNone){
    DEBUG(DEB_LEV_ERR, "Error in getting OMX_PORT_PARAM_TYPE parameter\n");
    exit(EXIT_FAILURE);
  }

  /*
   * Transition to idle state
   */

  err = OMX_SendCommand(appPriv->decoderHandle, OMX_CommandStateSet, OMX_StateIdle, NULL);

  /* Allocate buffers for input port  */
  for(j = 0; j < BUFFER_COUNT_ACTUAL; j++) {
    err = OMX_AllocateBuffer(appPriv->decoderHandle, &inBuffer[j], 0, NULL, DEFAULT_OUT_BUFFER_SIZE);
    if (err != OMX_ErrorNone) {
      DEBUG(DEB_LEV_ERR, "Error on AllocateBuffer in %i %i\n",(int) j, err);
      exit(EXIT_FAILURE);
    }
  }

  /* Allocate buffers for output port */
  for(j=0; j < BUFFER_COUNT_ACTUAL; j++) {
    err = OMX_AllocateBuffer(appPriv->decoderHandle, &outBuffer[j], 1, NULL, DEFAULT_OUT_BUFFER_SIZE);
    if (err != OMX_ErrorNone) {
      DEBUG(DEB_LEV_ERR, "Error on AllocateBuffer in %i %i\n",(int)j, err);
      exit(EXIT_FAILURE);
    }
  }
  tsem_down(appPriv->eventSem); /* Wait for transition to complete */

  /*
   * Transition to executing state
   */

  err = OMX_SendCommand(appPriv->decoderHandle, OMX_CommandStateSet, OMX_StateExecuting, NULL);
  tsem_down(appPriv->eventSem);  /* Wait for command to complete */

  /*
   * Execute
   */

  /* Send some input buffers */
  for(i = 0; i < BUFFER_COUNT_ACTUAL; i++) {
    int count = read(appPriv->ipd, inBuffer[i]->pBuffer, DEFAULT_IN_BUFFER_SIZE);
    if(count != DEFAULT_IN_BUFFER_SIZE) {
      inBuffer[i]->nFlags = inBuffer[i]->nFlags | OMX_BUFFERFLAG_EOS;
    }
    inBuffer[i]->nFilledLen = count;
    err = OMX_EmptyThisBuffer(appPriv->decoderHandle, inBuffer[i]);
  }

  /* Send some output buffers */
  for(i = 0; i < BUFFER_COUNT_ACTUAL; i++) {
    err = OMX_FillThisBuffer(appPriv->decoderHandle, outBuffer[i]);
  }

  tsem_down(appPriv->eofSem); /* Wait for EOS */

  /*
   * Transition to idle state
   */

  err = OMX_SendCommand(appPriv->decoderHandle, OMX_CommandStateSet, OMX_StateIdle, NULL);
  tsem_down(appPriv->eventSem);

  /*
   * Transition to loaded state
   */

  err = OMX_SendCommand(appPriv->decoderHandle, OMX_CommandStateSet, OMX_StateLoaded, NULL);

  /* Free input buffers */
  for(j=0;j<BUFFER_COUNT_ACTUAL;j++)
    err = OMX_FreeBuffer(appPriv->decoderHandle, 0, inBuffer[j]);

  /* Free output buffers */
  for(j=0;j<BUFFER_COUNT_ACTUAL;j++)
    err = OMX_FreeBuffer(appPriv->decoderHandle, 1, outBuffer[j]);

  tsem_down(appPriv->eventSem);   /* Wait for transition to complete */

  /*
   * De-initialize
   */

  OMX_FreeHandle(appPriv->decoderHandle);

}

static void usage(void) {
  printf("\n");
  printf("Usage: audio_decoder <infile> <outfile>\n");
  printf("\n");
  printf("       -h        : Displays this help\n");
  printf("\n");
  exit(1);
}

static void options(int argc, char** argv)
{
  int i;

  for(i = 1; i < argc; i++) {
    if(argv[i][0] == '-') {
      switch(argv[i][1]) {
      case 'h':
      default:
	usage();
	break;
      }
    } else break;
  }

  if((argc - i) == 2) {
    appPriv->ipd = open(argv[i++], O_RDONLY);
    if(-1 == appPriv->ipd) {
      perror("audio_decoder_test: ");
      exit(EXIT_FAILURE);
    }
    appPriv->opd = open(argv[i++], O_WRONLY | O_CREAT, 0);
    if(-1 == appPriv->opd) {
      perror("audio_decoder_test: ");
      exit(EXIT_FAILURE);
    }
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

