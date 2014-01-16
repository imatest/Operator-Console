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

#include "stdafx.h"
#include <map>
#include <vector>
#include <string>

///
/// A templated struct that contains all information for a single, scalar pass/fail variable.
///
template <typename T> struct entry{
	CString name;				//!< the pass/fail variable (or key) name as given in the Imatest documentation
	CString description;		//!< The description of this variable to be displayed.
	T value;					//!< the value of the pass/fail variable
	std::string data_type;		//!< an indicator for the data_type to be used by inifile()
								/**< This variable is used by inifile() for determining what data-type should be returned. Set equal to "i" for int, "d" for double, or "" (or "s") for string.*/
	bool b_isUsed;				//!< Indicates that this particular variable was found in the pass/fail file and thus should be displayed and kept up to date in the pass/fail file.
	void assign_value(const T& input, const T& badVal); //!< A member function for conditionally assigning the variable 'value' to 'input'
														/**< if 'input' is != the garbage value returned by inifile() when a variable is not found, then b_isUsed = true and value = input */
	entry(void): name(),description(),value(),data_type(),b_isUsed(0) {} //!< a default constructor
};

template <typename T> void entry<T>::assign_value( const T& input, const T& badVal)
{
	if (input != badVal)
	{
		this->b_isUsed = true;
		this->value = input;
	}
}
///
/// A templated struct that contains all information for a single min-max variable pair.
///
template <typename T> struct minMaxEntry{
	CString group_name;			//!< The overall name for the pair
	CString min_name;			//!< The name displayed for the min entry
								/**< Note that an MFC property grid will not display a description for the group*/
	CString max_name;			//!< The name displayed for the max entry
								/**< Note that an MFC property grid will not display a description for the group*/								
	CString min_description;	//!< The description displayed for the min variable
	CString max_description;	//!< The description displayed for the max variable
	T min_val;					//!< The value for the min variable
	T max_val;					//!< The value for the max variable
	std::string data_type;		//!< an indicator for the data_type to be used by inifile()
								/**< This variable is used by inifile() for determining what data-type should be returned. Set equal to "i" for int, "d" for double, or "" (or "s") for string.*/
	bool b_isUsed;				//!< Indicates that this particular variable was found in the pass/fail file and thus should be displayed and kept up to date in the pass/fail file.
	void assign_value(const std::vector<T>& input, const T& badVal);	//!< A member function for conditionally assigning the variable 'value' to 'input'
																		/**< if 'input' is != the garbage value returned by inifile() when a variable is not found, then b_isUsed = true and value = input */
	minMaxEntry(void): group_name(), min_name(), max_name(), min_description(), max_description(), min_val(0), max_val(0), data_type(), b_isUsed(0) {} //!< default constructor
};
template <typename T> void minMaxEntry<T>::assign_value( const std::vector<T>& input, const T& badVal)
{
	if (input[0] != badVal)
	{
		this->b_isUsed = true;
		this->min_val = input[0];
		this->max_val = input[1];
	}
}

///
/// A templated struct that contains all the information for a single vector pass/fail variable
///
template <typename T> struct vecEntry{
	CString group_name;						//!< The name to be displayed for the collection
	CString group_description;				//!< The description for the collection
											/**< If you want all values to have the same description, you can leave sub_descriptions empty and place the description in group_description*/
	std::vector<CString> sub_names;			//!< Names to be displayed for each element of the variable
	std::vector<CString> sub_descriptions;	//!< Descriptions to be displayed for each element
											/**< If you want all values to have the same description, you can leave sub_descriptions empty and place the description in group_description*/
	std::vector<T> value;					//!< A vector containing all the values of the variable
	std::string data_type;					//!< an indicator for the data_type to be used by inifile()
											/**< This variable is used by inifile() for determining what data-type should be returned. Set equal to "i" for int, "d" for double, or "" (or "s") for string.*/
	bool b_isUsed;							//!< Indicates that this particular variable was found in the pass/fail file and thus should be displayed and kept up to date in the pass/fail file.
	void assign_value(const std::vector<T>& input, const T& badVal);	//!< A member function for conditionally assigning the variable 'value' to 'input'
																		/**< if 'input' is != the garbage value returned by inifile() when a variable is not found, then b_isUsed = true and value = input */
	vecEntry(void): group_name(), group_description(), sub_names(), sub_descriptions(), value(), data_type(), b_isUsed(0) {}	//!< default constructor
};

template <typename T> void vecEntry<T>::assign_value( const std::vector<T>& input, const T& badVal)
{
	if (input[0] != badVal)
	{
		this->value.resize(input.size());
		this->b_isUsed = true;
		for (std::size_t idx=0; idx < input.size(); ++idx;) 
		{
			this->value[idx] = input[idx];
		}
	}
}
///
/// A struct that contains an entry<T>,minMaxEntry<T>, etc. for every pass/fail SFRPlus variable
///
typedef struct SFRplusPassFail{
	// the list of keys
	entry<long>			All_Edge_IDs_detected;
	entry<long>			Bayer_decode;
	minMaxEntry<double> Chart_mean_pixel_level_bounds;
	entry<double>		Chart_radial_pixel_shift_max;
	entry<long>			Color_expected_detected;
	entry<double>		DeltaE_00_mean_max;
	entry<double>		Convergence_angle_max;
	entry<double>		FOV_degrees_diagonal_min;
	entry<double>		High_pixel_saturation_fraction_max;
	entry<long>			Horizontal_bars_OK_min;
	entry<double>		Low_pixel_saturation_fraction_max;
	entry<long>			Mirrored_chart;
	entry<double>		MTF50P_ratio_min;
	entry<double>		MTF50P_CP_weighted_mean_min;
	entry<CString>		passfail_ini_file_date;
	entry<double>		Rotation_degrees_max;
	entry<double>		Secondary_readout_1_center_mean_min;
	entry<double>		Secondary_readout_1_outer_mean_min;
	entry<double>		Secondary_readout_1_outer_min_min;
	entry<double>		Secondary_readout_1_outer_quadrant_delta_max;
	entry<double>		Secondary_readout_1_outer_quadrant_mean_min_min;
	entry<double>		Secondary_readout_2_center_mean_min;
	entry<double>		Secondary_readout_2_outer_mean_min;
	entry<double>		Secondary_readout_2_outer_min_min;
	entry<double>		Secondary_readout_2_outer_quadrant_delta_max;
	entry<double>		Secondary_readout_2_outer_quadrant_mean_min_min;
	entry<long>			Stepchart_expected_detected;
	entry<long>			upside_down;

	CString name;			//!< The name to be displayed for this assembly of variables.
	std::size_t numEntries; //!< The number of variables in this struct. This MUST be updated if new keys are added.
	bool b_enable;			//!< Indicates that the SFRPlus field was found in the pass/fail file
}SFRplus;

///
/// A struct that contains an entry<T>,minMaxEntry<T>, or vecEntry<T> for every pass/fail Blemish variable
///
typedef struct BlemishPassFail{
	// the list of keys
	vecEntry<long>	Blemish_maximum_count;
	vecEntry<long>	Blemish_size_pixels;
	entry<long>		Dead_pixel_clusters_max;
	entry<long>		Dead_pixels_max;
	entry<long>		Defective_pixels_max_count;
	entry<long>		Hot_pixel_clusters_max;
	entry<long>		Hot_pixels_max;
	entry<double>	Optical_center_offset_max;
	entry<double>	Optical_center_offset_X_max;
	entry<double>	Optical_center_offset_Y_max;
	entry<double>	Relative_illumination_corner_diff_pct_max;
	entry<double>	Relative_illumination_worst_corner_pct_min;
	entry<double>	Uniformity_BoverG_corners_pct_max;
	entry<double>	Uniformity_RoverG_corners_pct_max;

	CString name;			//!< The name to be displayed for this assembly of variables.
	std::size_t numEntries;	//!< The number of variables in this struct. This MUST be updated if new keys are added.
	bool b_enable;			//!< Indicates that the Blemish field was found in the pass/fail file
}Blemish;

///
/// A struct that contains an entry<T>,minMaxEntry<T>, or vecEntry<T> for every pass/fail OIS variable
///
typedef struct OISPassFail{
	// the list of keys
	entry<long> L_MTF50_delta2_gain_summary_all_dB_min;
	entry<long> R_improve_H_dB_min;
	entry<long> R_improve_V_dB_min;
	entry<long> R_improve_ALL_dB_min;

	CString name;			//!< The name to be displayed for this assembly of variables.	
	std::size_t numEntries;	//!< The number of variables in this struct. This MUST be updated if new keys are added.
	bool b_enable;			//!< Indicates that the OIS field was found in the pass/fail file
}OIS;

///
/// A struct that can be used to catch other (that is, not Blemish, OIS, or SFRplus) pass/fail categories into a vector of entry<CString>'s
///
typedef struct OtherPassFail{
	std::vector<entry<CString> > ent_vec;

	CString name;	//!< The name to be displayed for this assembly of variables.
	bool b_enable;	//!< The number of variables in this struct. This MUST be updated if new keys are added.
}other;

///
/// A class to contain the pass/fail criteria including the descriptions of each pass/fail variable. All the display data
/// (e.g. the entry<T> name and description fields) is initialized in the constructor for the CPassFailSettings class. Ultimately,
/// the data contained in the various structs within this class will be displayed by class CPassFail in a
/// MFC property grid control; a given variable is only displayed if it is found in the pass/fail file.
///
class CPassFailSettings
{
public:
	CPassFailSettings(void);				//!< The constructor, which contains all the initializations for every pass/fail variable
	~CPassFailSettings(void);
	SFRplusPassFail sfrplus;
	Blemish blemish;
	OISPassFail ois;
	OtherPassFail other;
	bool b_isReadOnly;						//!< for checking if the Pass/Fail file is read-only
	CString m_ini_file;						//!< fully-qualified filename for the Imatest ini file
	CString m_pass_fail_file;				//!< fully-qualified filename for the Pass/Fail file
};

