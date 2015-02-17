#ifndef GUARD_IMATESTSOURCEIDS_H
#define GUARD_IMATESTSOURCEIDS_H

typedef enum sourceIDs {
	SOURCE_File				 = 0,  // read a file
	SOURCE_Epiphan              = 1,  // Epiphan
	SOURCE_Omnivision           = 2,  // Omnivision OVTAPathmerM
	SOURCE_Toshiba              = 3,  // Toshiba
	SOURCE_STM                  = 4,  // STM Conduit
	SOURCE_Graphin              = 5,  // Graphin EasyLab    
	SOURCE_Aptina               = 6,  // Aptina Devware
	SOURCE_Android              = 7,  // Android Camera Interface
	SOURCE_OpConsoleDirectShow  = 8,  // DirectShow for the C++ OperatorConsole

	// Below is the first entry for Image Acquisition toolbox devices. All
	// values above ImageAcq_first are reserved for Image Acquisition
	// toolbox devices.
	SOURCE_ImageAcq_first   = 128,
} AcqSource;

#endif