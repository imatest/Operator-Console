/* Extremely Simple Capture API */

struct SimpleCapParams
{
	/* Target buffer. 
	 * Must be at least mWidth * mHeight * sizeof(int) of size! 
	 */
	int * mTargetBuf;
	/* Buffer width */
	int mWidth;
	/* Buffer height */
	int mHeight;
};


/* Sets up the ESCAPI DLL and the function pointers below. Call this first! */
/* Returns number of capture devices found (same as countCaptureDevices, below) */
extern int setupESCAPI();

/* return the number of capture devices found */
typedef int (*countCaptureDevicesProc)();
extern countCaptureDevicesProc countCaptureDevices;

/* initCapture tries to open the video capture device. 
 * Returns 0 on failure, 1 on success. 
 * Note: Capture parameter values must not change while capture device
 *       is in use (i.e. between initCapture and deinitCapture).
 *       Do *not* free the target buffer, or change its pointer!
 */
typedef int (*initCaptureProc)(unsigned int deviceno, struct SimpleCapParams *aParams);
extern initCaptureProc initCapture;

/* deinitCapture closes the video capture device. */
typedef void (*deinitCaptureProc)(unsigned int deviceno);
extern deinitCaptureProc deinitCapture;

/* doCapture requests video frame to be captured. */
typedef void (*doCaptureProc)(unsigned int deviceno);
extern doCaptureProc doCapture;

/* isCaptureDone returns 1 when the requested frame has been captured.*/
typedef int (*isCaptureDoneProc)(unsigned int deviceno);
extern isCaptureDoneProc isCaptureDone;

/* Get the user-friendly name of a capture device. */
typedef void (*getCaptureDeviceNameProc)(unsigned int deviceno, char *namebuffer, int bufferlength);
extern getCaptureDeviceNameProc getCaptureDeviceName;

/* Returns the ESCAPI DLL version. 0x200 for 2.0 */
typedef int (*ESCAPIDLLVersionProc)();
extern ESCAPIDLLVersionProc ESCAPIDLLVersion;

