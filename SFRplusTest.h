/****************************************************************************	
*	Operator Console - an extensible user interface for the Imatest IT 		*
*	library																	*
*	Copyright (C) 2013 Imatest LLC.											*
*																			*
*	This program is free software: you can redistribute it and/or modify	*
*	it under the terms of the GNU General Public License as published by	*
*	the Free Software Foundation, either version 3 of the License, or		*
*	(at your option) any later version.										*
*																			*
*	This program is distributed in the hope that it will be useful,			*
*	but WITHOUT ANY WARRANTY; without even the implied warranty of			*
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the			*
*	GNU General Public License for more details.							*
*																			*
*	You should have received a copy of the GNU General Public License		*
*	along with this program.  If not, see <http://www.gnu.org/licenses/>. 	*
****************************************************************************/

#pragma once

#include <string>
#include "ImageTest.h"
#include "ThreadControl.h"
#include "qso.h"

///
/// contains the details of an sfrPlus test to be performed including all relevant fields for determining pass or fail
///
class SFRplusTest : public ImageTest
{
public:
	SFRplusTest(void);
	virtual ~SFRplusTest(void);

	const bool *GetQSO() {return m_qso;}
	void ClearQSO(void);
#if 0
	void		Run();															//!< DEPRECATED: The Run method has been moved up to the parent class (ImageTest).
	void		Run(void *raw_pixels, int width, int height, Config &config);	//!< DEPRECATED: The Run method has been moved up to the parent class (ImageTest).
#endif

protected:
	void		GetResults(JSONNode &passfail);
	void		ParseResults(std::string &results);
	void		ParseOverlay(JSONNode &data, JSONNode &passfail);

protected:
	bool		m_qso[eNumQuadrants];	//!< pass/fail for each quadrant
	
};

