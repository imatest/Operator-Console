#include <windows.h>
#include "escapi.h"

countCaptureDevicesProc countCaptureDevices;
initCaptureProc initCapture;
deinitCaptureProc deinitCapture;
doCaptureProc doCapture;
isCaptureDoneProc isCaptureDone;
getCaptureDeviceNameProc getCaptureDeviceName;
ESCAPIDLLVersionProc ESCAPIDLLVersion;

/* Internal: initialize COM */
typedef void (*initCOMProc)();
initCOMProc initCOM;

int setupESCAPI()
{
  /* Load DLL dynamically */
  HMODULE capdll = LoadLibrary("escapi.dll");
  if (capdll == NULL)
    return 0;

  /* Fetch function entry points */
  countCaptureDevices = (countCaptureDevicesProc)GetProcAddress(capdll, "countCaptureDevices");
  initCapture = (initCaptureProc)GetProcAddress(capdll, "initCapture");
  deinitCapture = (deinitCaptureProc)GetProcAddress(capdll, "deinitCapture");
  doCapture = (doCaptureProc)GetProcAddress(capdll, "doCapture");
  isCaptureDone = (isCaptureDoneProc)GetProcAddress(capdll, "isCaptureDone");
  initCOM = (initCOMProc)GetProcAddress(capdll, "initCOM");
  getCaptureDeviceName = (getCaptureDeviceNameProc)GetProcAddress(capdll, "getCaptureDeviceName");
  ESCAPIDLLVersion = (ESCAPIDLLVersionProc)GetProcAddress(capdll, "ESCAPIDLLVersion");

  /* Check that we got all the entry points */
  if (initCOM == NULL ||
      ESCAPIDLLVersion == NULL ||
      getCaptureDeviceName == NULL ||
      countCaptureDevices == NULL ||
      initCapture == NULL ||
      deinitCapture == NULL ||
      doCapture == NULL ||
      isCaptureDone == NULL)
      return 0;

  /* Verify DLL version */
  if (ESCAPIDLLVersion() != 0x200)
    return 0;

  /* Initialize COM.. */
  initCOM();

  /* and return the number of capture devices found. */
  return countCaptureDevices();
}

