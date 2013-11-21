#pragma once
#include "ImageAcquisition.h"

class ImatestLibAcq :	public ImageAcquisition
{
public:
	ImatestLibAcq(void);
	~ImatestLibAcq(void);

	bool	CaptureFrame();	// capture a single frame into m_buf
	bool	Close();
	bool	Open();

protected:
	int		m_numCameras;
	int		m_cameraIndex;
	bool	m_inited;
};

