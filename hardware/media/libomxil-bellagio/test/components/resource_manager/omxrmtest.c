/**
  test/components/resource_manager/omxrmtest.c

  This simple test application tests the functionalities of the simple resource
  manager provided with Bellagio components that implements the basic support defined in OpenMAX
  for resource management.

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

#include "omxrmtest.h"
#include <bellagio/extension_struct.h>

#define MAX_COMPONENTS 5
/* Application private date: should go in the component field (segs...) */
//#define PROPRITY_TEST
#define WAIT_TEST
//#define ALTERNATE_WAIT_TEST

OMX_ERRORTYPE err;

OMX_HANDLETYPE handle[MAX_COMPONENTS];

OMX_CALLBACKTYPE callbacks = { .EventHandler = rmEventHandler,
                               .EmptyBufferDone = rmEmptyBufferDone,
                               .FillBufferDone = rmFillBufferDone,
};

#ifdef PROPRITY_TEST
static void setHeader(OMX_PTR header, OMX_U32 size) {
  OMX_VERSIONTYPE* ver = (OMX_VERSIONTYPE*)(header + sizeof(OMX_U32));
  *((OMX_U32*)header) = size;

  ver->s.nVersionMajor = VERSIONMAJOR;
  ver->s.nVersionMinor = VERSIONMINOR;
  ver->s.nRevision = VERSIONREVISION;
  ver->s.nStep = VERSIONSTEP;
}
#endif

void display_help() {
  printf("\n");
  printf("Usage: omxrmtest\n");
  printf("\n");
  exit(1);
}

int main(int argc, char** argv) {

	int i;
	int num_of_components;
#ifdef PROPRITY_TEST
    OMX_PRIORITYMGMTTYPE oPriority;
#else
#endif
    OMX_STATETYPE state;
	/* Obtain file descriptor */
	eventSem = malloc(sizeof(tsem_t));
	tsem_init(eventSem, 0);
	bResourceErrorReceived = OMX_FALSE;

	err = OMX_Init();
	if(err != OMX_ErrorNone) {
		DEBUG(DEB_LEV_ERR, "OMX_Init() failed\n");
		exit(1);
	}
	DEBUG(DEFAULT_MESSAGES, "OMX_Init()\n");

	for (i = 0; i<MAX_COMPONENTS; i++) {
		err = OMX_GetHandle(&handle[i], "OMX.st.volume.component", NULL, &callbacks);
		if(err != OMX_ErrorNone) {
			DEBUG(DEB_LEV_ERR, "OMX_GetHandle [%i] failed\n", i);
			exit(1);
		}
		DEBUG(DEFAULT_MESSAGES, "OMX_GetHandle() %i\n", i);
	}
#ifdef ALTERNATE_WAIT_TEST
	DEBUG(DEFAULT_MESSAGES, "OMX_StateWaitForResources 0\n");
    err = OMX_SendCommand(handle[0], OMX_CommandPortDisable, 0, 0);
    err = OMX_SendCommand(handle[0], OMX_CommandPortDisable, 1, 0);
    err = OMX_SendCommand(handle[0], OMX_CommandStateSet, OMX_StateWaitForResources, NULL);
    tsem_down(eventSem);
    tsem_down(eventSem);
	DEBUG(DEFAULT_MESSAGES, "OMX_StateLoaded 0\n");
    err = OMX_SendCommand(handle[0], OMX_CommandStateSet, OMX_StateLoaded, NULL);
    tsem_down(eventSem);
#endif

	for (i = 0; i<MAX_COMPONENTS; i++) {
		num_of_components = i;
        err = OMX_SendCommand(handle[i], OMX_CommandPortDisable, 0, 0);
        err = OMX_SendCommand(handle[i], OMX_CommandPortDisable, 1, 0);
        err = OMX_SendCommand(handle[i], OMX_CommandStateSet, OMX_StateIdle, NULL);
		if(err != OMX_ErrorNone) {
			DEBUG(DEB_LEV_ERR, "The component can't go to Idle\n");
			break;
		}
        tsem_down(eventSem);
        if (bResourceErrorReceived) {
        	DEBUG(DEFAULT_MESSAGES, "The resources are exhausted\n");
#ifdef PROPRITY_TEST
			setHeader(&oPriority, sizeof(OMX_PRIORITYMGMTTYPE));
		    oPriority.nGroupPriority = 1;
		    err = OMX_SetParameter(handle[i], OMX_IndexParamPriorityMgmt, &oPriority);
	        err = OMX_SendCommand(handle[i], OMX_CommandStateSet, OMX_StateIdle, NULL);
	        tsem_down(eventSem);
#endif
#ifdef WAIT_TEST
	        err = OMX_SendCommand(handle[i], OMX_CommandStateSet, OMX_StateWaitForResources, NULL);
	        tsem_down(eventSem);
	        err = OMX_SendCommand(handle[i-1], OMX_CommandStateSet, OMX_StateLoaded, NULL);
	        tsem_down(eventSem);
	        tsem_down(eventSem);
#endif
        	break;
        }
	}
	DEBUG(DEFAULT_MESSAGES, "The resource manager has operated!\n");

	DEBUG(DEFAULT_MESSAGES, "Dispose the system\n");
	for (i = 0; i<num_of_components; i++) {
		err = OMX_GetState(handle[i], &state);
		if (state == OMX_StateIdle) {
			err = OMX_SendCommand(handle[i], OMX_CommandStateSet, OMX_StateLoaded, NULL);
	        tsem_down(eventSem);
			DEBUG(DEFAULT_MESSAGES, "Component %i sent to Loaded\n", i);
		} else if (state == OMX_StateLoaded) {
			DEBUG(DEFAULT_MESSAGES, "Component %i already loaded\n", i);
		} else {
			DEBUG(DEFAULT_MESSAGES, "Component %i in the wrong state!\n", i);
		}
	}

	for (i = 0; i<MAX_COMPONENTS; i++) {
		err = OMX_FreeHandle(handle[i]);
		if(err != OMX_ErrorNone) {
			DEBUG(DEB_LEV_ERR, "OMX_FreeHandle [%i] failed\n", i);
			exit(1);
		}
		DEBUG(DEFAULT_MESSAGES, "OMX_FreeHandle() %i\n", i);
	}

	err = OMX_Deinit();
	if(err != OMX_ErrorNone) {
		DEBUG(DEB_LEV_ERR, "OMX_Deinit() failed\n");
		exit(1);
	}
	free(eventSem);
	DEBUG(DEFAULT_MESSAGES, "OMX_Deinit()\n");
	return 0;
}

/* Callbacks implementation */
OMX_ERRORTYPE rmEventHandler(
  OMX_HANDLETYPE hComponent,
  OMX_PTR pAppData,
  OMX_EVENTTYPE eEvent,
  OMX_U32 Data1,
  OMX_U32 Data2,
  OMX_PTR pEventData) {

  DEBUG(DEB_LEV_ERR, "Hi there, I am in the %s callback for %x\n", __func__, (int)hComponent);
  if(eEvent == OMX_EventCmdComplete) {
    if (Data1 == OMX_CommandStateSet) {
      DEBUG(DEFAULT_MESSAGES, "Volume Component %x State changed in ", (int)hComponent);
      switch ((int)Data2) {
      case OMX_StateInvalid:
        DEBUG(DEFAULT_MESSAGES, "OMX_StateInvalid\n");
        break;
      case OMX_StateLoaded:
        DEBUG(DEFAULT_MESSAGES, "OMX_StateLoaded\n");
        break;
      case OMX_StateIdle:
        DEBUG(DEFAULT_MESSAGES, "OMX_StateIdle\n");
        break;
      case OMX_StateExecuting:
        DEBUG(DEFAULT_MESSAGES, "OMX_StateExecuting\n");
        break;
      case OMX_StatePause:
        DEBUG(DEFAULT_MESSAGES, "OMX_StatePause\n");
        break;
      case OMX_StateWaitForResources:
        DEBUG(DEFAULT_MESSAGES, "OMX_StateWaitForResources\n");
        break;
      }
      tsem_up(eventSem);
    } else if (Data1 == OMX_CommandPortDisable) {
    	DEBUG(DEFAULT_MESSAGES, "Disabled port %i\n", (int)Data2);
    }
  } else if (eEvent == OMX_EventError) {
  	if (Data1 == OMX_ErrorInsufficientResources) {
  	  	DEBUG(DEFAULT_MESSAGES, "Received error OMX_ErrorInsufficientResources\n");
  	  	bResourceErrorReceived = OMX_TRUE;
        tsem_up(eventSem);
  	} else if (Data1 == OMX_ErrorResourcesLost) {
  	  	DEBUG(DEFAULT_MESSAGES, "Received error OMX_ErrorResourcesLost\n");
  	} else if (Data1 == OMX_ErrorResourcesPreempted) {
  	  	DEBUG(DEFAULT_MESSAGES, "Received error OMX_ErrorResourcesPreempted\n");
  	} else {
  	  	DEBUG(DEFAULT_MESSAGES, "Received error %i\n", (int)Data1);
  	}
  } else if(eEvent == OMX_EventResourcesAcquired) {
	  	DEBUG(DEFAULT_MESSAGES, "Received message OMX_EventResourcesAcquired\n");
  } else {
    DEBUG(DEFAULT_MESSAGES, "Param1 is %i\n", (int)Data1);
    DEBUG(DEFAULT_MESSAGES, "Param2 is %i\n", (int)Data2);
  }
  return OMX_ErrorNone;
}

OMX_ERRORTYPE rmEmptyBufferDone(
		OMX_HANDLETYPE hComponent,
		OMX_PTR pAppData,
		OMX_BUFFERHEADERTYPE* pBuffer) {

	return OMX_ErrorNone;
}

OMX_ERRORTYPE rmFillBufferDone(
		OMX_HANDLETYPE hComponent,
		OMX_PTR pAppData,
		OMX_BUFFERHEADERTYPE* pBuffer) {

	return OMX_ErrorNone;
}
