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

// PassFail.cpp : implementation file
//

#include "stdafx.h"
#include "OperatorConsole.h"
#include "PassFail.h"
#include "afxdialogex.h"
#include <memory> // for std::auto_ptr
#include <exception> // for std::exception

///
/// Conditionally adds an entry<T> as a subitem of a CMFCPropertyGridProperty if the entry<T> is used in the pass/fail file
///
template<typename T> BOOL AddSubEntry(std::auto_ptr<CMFCPropertyGridProperty>& apGroup, entry<T>& ent)
{
	bool result = FALSE;
	if ( ent.b_isUsed)
	{
		apGroup->AddSubItem(new CMFCPropertyGridProperty( ent.name, (COleVariant) ent.value, ent.description));
		result = TRUE;
	}
	return result;
}

///
/// Conditionally adds a minMaxEntry<T> as a subitem of a CMFCPropertyGridProperty if the minMaxEntry<T> is used in the pass/fail file
///
template <typename T> BOOL AddSubMinMaxEntry(std::auto_ptr<CMFCPropertyGridProperty>& apGroup, minMaxEntry<T>& ent)
{
	bool result = FALSE;
	try
	{
		if ( ent.b_isUsed)
		{
			CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(ent.group_name,0,TRUE);
			apGroup->AddSubItem(pProp);
			CMFCPropertyGridProperty* pPropmin = new CMFCPropertyGridProperty(ent.min_name, (COleVariant) ent.min_val, ent.min_description);
			CMFCPropertyGridProperty* pPropmax = new CMFCPropertyGridProperty(ent.max_name, (COleVariant) ent.max_val, ent.max_description);
			pProp->AddSubItem(pPropmin);
			pProp->AddSubItem(pPropmax);
			result = TRUE;
		}
	}
	catch (std::exception& ex)
	{
		std::cerr << "Error in AddSubMinMaxEntry: "<<ex.what() << std::endl;
	}
	return result;
}

///
/// Conditionally adds a vecEntry<T> as a subitem of a CMFCPropertyGridProperty if the vecEntry<T> is used in the pass/fail file
///
template <typename T> BOOL AddSubVecEntry(std::auto_ptr<CMFCPropertyGridProperty>& apGroup, vecEntry<T>& ent)
{
	bool result = FALSE;

	try
	{
		if ( ent.b_isUsed)
		{
			CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(ent.group_name,0,TRUE);
			apGroup->AddSubItem(pProp);

			for (std::size_t idx = 0 ; idx < ent.value.size(); ++idx)
			{
				CString desc, name;
				if ( idx >= ent.sub_descriptions.size() || ent.sub_descriptions[idx] ==_T(""))
				{
					desc = ent.group_description;
				}
				else
				{
					desc = ent.sub_descriptions[idx];
				}

				if ( idx >= ent.sub_names.size())
				{
					name = _T("");
				}
				else
				{
					name = ent.sub_names[idx];
				}
				CMFCPropertyGridProperty* pPropsub = new CMFCPropertyGridProperty(name, (COleVariant) ent.value[idx], desc);
				pProp->AddSubItem(pPropsub);

			}
		}
	}
	catch (std::exception& ex)
	{
		std::cerr << "Error in AddSubVecEntry: "<< ex.what() << std::endl;
	}
	return result;
}

// CPassFail dialog

IMPLEMENT_DYNAMIC(CPassFail, CDialogEx)

	CPassFail::CPassFail(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPassFail::IDD, pParent)
{

}

CPassFail::CPassFail(CPassFailSettings& InPassFail, CWnd* pParent /*=NULL*/)
	: CDialogEx(CPassFail::IDD, pParent)
{
	PFSettings = InPassFail;

}

CPassFail::~CPassFail()
{
}

void CPassFail::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROPLIST_LOCATION, c_proplist_location);
	DDX_Control(pDX, IDOK, c_ok);
	DDX_Control(pDX, IDCANCEL, c_cancel);
}


BEGIN_MESSAGE_MAP(CPassFail, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPassFail::OnBnClickedOk)
END_MESSAGE_MAP()


// CPassFail message handlers


BOOL CPassFail::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rectPropList;
	c_proplist_location.GetClientRect(&rectPropList);
	c_proplist_location.MapWindowPoints(this, &rectPropList);

	m_wndPropList.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rectPropList, this, (UINT)-1);
	m_wndPropList.EnableHeaderCtrl();
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook(TRUE);
	m_wndPropList.MarkModifiedProperties(TRUE); // Changes the text of a value to bold after the user edits a value
	m_wndPropList.SetDescriptionRows(6); // set the number of rows to display by default in the description area

	///////////////////////////////////////////////////////
	// Add the SFRplus entries to the CMFCGridPropertyCtrl
	///////////////////////////////////////////////////////
	if (PFSettings.sfrplus.b_enable)
	{
		std::auto_ptr<CMFCPropertyGridProperty> apGroup1(new CMFCPropertyGridProperty(PFSettings.sfrplus.name));

		AddSubMinMaxEntry(apGroup1, PFSettings.sfrplus.Chart_mean_pixel_level_bounds);
		AddSubEntry(apGroup1, PFSettings.sfrplus.Chart_radial_pixel_shift_max);
		AddSubEntry(apGroup1, PFSettings.sfrplus.Low_pixel_saturation_fraction_max);
		AddSubEntry(apGroup1, PFSettings.sfrplus.High_pixel_saturation_fraction_max);
		AddSubEntry(apGroup1, PFSettings.sfrplus.MTF50P_ratio_min);
		AddSubEntry(apGroup1, PFSettings.sfrplus.MTF50P_CP_weighted_mean_min);
		AddSubEntry(apGroup1, PFSettings.sfrplus.Rotation_degrees_max);
		AddSubEntry(apGroup1, PFSettings.sfrplus.FOV_degrees_diagonal_min);
		AddSubEntry(apGroup1, PFSettings.sfrplus.Convergence_angle_max);
		AddSubEntry(apGroup1, PFSettings.sfrplus.Secondary_readout_1_center_mean_min);
		AddSubEntry(apGroup1, PFSettings.sfrplus.Secondary_readout_1_outer_mean_min);
		AddSubEntry(apGroup1, PFSettings.sfrplus.Secondary_readout_1_outer_min_min);
		AddSubEntry(apGroup1, PFSettings.sfrplus.Secondary_readout_1_outer_quadrant_delta_max);
		AddSubEntry(apGroup1, PFSettings.sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min);
		AddSubEntry(apGroup1, PFSettings.sfrplus.Secondary_readout_2_center_mean_min);
		AddSubEntry(apGroup1, PFSettings.sfrplus.Secondary_readout_2_outer_mean_min);
		AddSubEntry(apGroup1, PFSettings.sfrplus.Secondary_readout_2_outer_min_min);
		AddSubEntry(apGroup1, PFSettings.sfrplus.Secondary_readout_2_outer_quadrant_delta_max);
		AddSubEntry(apGroup1, PFSettings.sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min);
		AddSubEntry(apGroup1, PFSettings.sfrplus.Horizontal_bars_OK_min);
		AddSubEntry(apGroup1, PFSettings.sfrplus.All_Edge_IDs_detected);
		AddSubEntry(apGroup1, PFSettings.sfrplus.Mirrored_chart);
		AddSubEntry(apGroup1, PFSettings.sfrplus.Bayer_decode);
		AddSubEntry(apGroup1, PFSettings.sfrplus.Color_expected_detected);
		AddSubEntry(apGroup1, PFSettings.sfrplus.Stepchart_expected_detected);
		AddSubEntry(apGroup1, PFSettings.sfrplus.upside_down);
		AddSubEntry(apGroup1, PFSettings.sfrplus.passfail_ini_file_date);

		m_wndPropList.AddProperty(apGroup1.release());
	}

	///////////////////////////////////////////////////////
	// Add the Blemish entries to the CMFCGridPropertyCtrl
	///////////////////////////////////////////////////////
	if (PFSettings.blemish.b_enable)
	{
		std::auto_ptr<CMFCPropertyGridProperty> apGroup2(new CMFCPropertyGridProperty(PFSettings.blemish.name));

		AddSubEntry(apGroup2, PFSettings.blemish.Dead_pixels_max);
		AddSubEntry(apGroup2, PFSettings.blemish.Dead_pixel_clusters_max);
		AddSubEntry(apGroup2, PFSettings.blemish.Defective_pixels_max_count);
		AddSubEntry(apGroup2, PFSettings.blemish.Hot_pixel_clusters_max);
		AddSubEntry(apGroup2, PFSettings.blemish.Hot_pixels_max);
		AddSubEntry(apGroup2, PFSettings.blemish.Optical_center_offset_max);
		AddSubEntry(apGroup2, PFSettings.blemish.Optical_center_offset_X_max);
		AddSubEntry(apGroup2, PFSettings.blemish.Optical_center_offset_Y_max);
		AddSubEntry(apGroup2, PFSettings.blemish.Relative_illumination_corner_diff_pct_max);
		AddSubEntry(apGroup2, PFSettings.blemish.Relative_illumination_worst_corner_pct_min);
		AddSubEntry(apGroup2, PFSettings.blemish.Uniformity_RoverG_corners_pct_max);
		AddSubEntry(apGroup2, PFSettings.blemish.Uniformity_BoverG_corners_pct_max);
		AddSubVecEntry(apGroup2,PFSettings.blemish.Blemish_size_pixels);
		AddSubVecEntry(apGroup2,PFSettings.blemish.Blemish_maximum_count);

		m_wndPropList.AddProperty(apGroup2.release());
	}

	///////////////////////////////////////////////////////
	// Add the OIS entries to the CMFCGridPropertyCtrl
	///////////////////////////////////////////////////////
	if (PFSettings.ois.b_enable)
	{
		std::auto_ptr<CMFCPropertyGridProperty> apGroup3(new CMFCPropertyGridProperty(PFSettings.ois.name));

		AddSubEntry(apGroup3, PFSettings.ois.L_MTF50_delta2_gain_summary_all_dB_min);
		AddSubEntry(apGroup3, PFSettings.ois.R_improve_H_dB_min);
		AddSubEntry(apGroup3, PFSettings.ois.R_improve_V_dB_min);
		AddSubEntry(apGroup3, PFSettings.ois.R_improve_ALL_dB_min);
		m_wndPropList.AddProperty(apGroup3.release());
	}

	///////////////////////////////////////////////////////
	// Add the other entries to the CMFCGridPropertyCtrl
	///////////////////////////////////////////////////////
	if ( PFSettings.other.b_enable)
	{
		std::auto_ptr<CMFCPropertyGridProperty> apGroup4(new CMFCPropertyGridProperty(PFSettings.other.name));
		for ( std::size_t idx = 0; idx < PFSettings.other.ent_vec.size(); ++idx)
		{
			AddSubEntry(apGroup4, PFSettings.other.ent_vec[idx]);
		}
		m_wndPropList.AddProperty(apGroup4.release());
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

///
/// When the OK button is clicked this function updates the entire PFSettings class with the user supplied values
///
void CPassFail::OnBnClickedOk()
{

	CDialogEx::OnOK();
	std::size_t numProps = m_wndPropList.GetPropertyCount();

	for (std::size_t propidx = 0; propidx < numProps; ++propidx)
	{
		CMFCPropertyGridProperty* currentProp = m_wndPropList.GetProperty(propidx);
		CString currentName = currentProp->GetName();

		// search through and update the appropriate variables given the name of the currently selected property

		/////////////////////////////////////////////
		// Update PFSettings.blemish
		/////////////////////////////////////////////

		if ( currentName.Compare(PFSettings.blemish.name)==0) 
		{
			std::size_t subItemsCnt = currentProp->GetSubItemsCount();

			CMFCPropertyGridProperty* subItemProp;
			for ( std::size_t subIdx = 0; subIdx < subItemsCnt; ++subIdx)
			{
				subItemProp = currentProp->GetSubItem(subIdx); // Get the sub item
				CString subItemName = subItemProp->GetName();

				if (subItemName.Compare(PFSettings.blemish.Blemish_maximum_count.group_name)==0) // Blemish_maximum_count
				{
					std::size_t subSubItemCnt = subItemProp->GetSubItemsCount();
					PFSettings.blemish.Blemish_maximum_count.value.resize(subSubItemCnt);
					for (std::size_t subSubIdx = 0; subSubIdx < subSubItemCnt; ++subSubIdx)
					{
						PFSettings.blemish.Blemish_maximum_count.value[subSubIdx] = subItemProp->GetSubItem(subSubIdx)->GetValue().lVal;
					}
				}
				else if( subItemName.Compare(PFSettings.blemish.Blemish_size_pixels.group_name)==0) // Blemish_size_pixels
				{
					std::size_t subSubItemCnt = subItemProp->GetSubItemsCount();
					PFSettings.blemish.Blemish_size_pixels.value.resize(subSubItemCnt);
					for (std::size_t subSubIdx = 0; subSubIdx < subSubItemCnt; ++subSubIdx)
					{
						PFSettings.blemish.Blemish_size_pixels.value[subSubIdx] = subItemProp->GetSubItem(subSubIdx)->GetValue().lVal;
					}
				}
				else if(subItemName.Compare(PFSettings.blemish.Dead_pixels_max.name)==0) // Dead_pixels_max
				{
					PFSettings.blemish.Dead_pixels_max.value = subItemProp->GetValue().lVal;
				}
				else if(subItemName.Compare(PFSettings.blemish.Dead_pixel_clusters_max.name)==0) // Dead_pixel_clusters_max
				{
					PFSettings.blemish.Dead_pixel_clusters_max.value = subItemProp->GetValue().lVal;
				}
				else if(subItemName.Compare(PFSettings.blemish.Defective_pixels_max_count.name)==0) // Defective_pixels_max_count
				{
					PFSettings.blemish.Defective_pixels_max_count.value = subItemProp->GetValue().lVal;
				}
				else if(subItemName.Compare(PFSettings.blemish.Hot_pixel_clusters_max.name)==0) // Hot_pixel_clusters_max
				{
					PFSettings.blemish.Hot_pixel_clusters_max.value = subItemProp->GetValue().lVal;
				}
				else if(subItemName.Compare(PFSettings.blemish.Hot_pixels_max.name)==0) // Hot_pixels_max
				{
					PFSettings.blemish.Hot_pixels_max.value = subItemProp->GetValue().lVal;
				}
				else if (subItemName.Compare(PFSettings.blemish.Optical_center_offset_max.name)==0) // Optical_center_offset_max
				{
					PFSettings.blemish.Optical_center_offset_max.value = subItemProp->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.blemish.Optical_center_offset_X_max.name)==0) // Optical_center_offset_X_max
				{
					PFSettings.blemish.Optical_center_offset_X_max.value = subItemProp->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.blemish.Optical_center_offset_Y_max.name)==0) // Optical_center_offset_Y_max
				{
					PFSettings.blemish.Optical_center_offset_Y_max.value = subItemProp->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.blemish.Relative_illumination_corner_diff_pct_max.name)==0) // Relative_illumination_corner_diff_pct_max
				{
					PFSettings.blemish.Relative_illumination_corner_diff_pct_max.value = subItemProp->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.blemish.Relative_illumination_worst_corner_pct_min.name)==0) // Relative_illumination_worst_corner_pct_min
				{
					PFSettings.blemish.Relative_illumination_worst_corner_pct_min.value = subItemProp->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.blemish.Uniformity_BoverG_corners_pct_max.name)==0) // Uniformity_BoverG_corners_pct_max
				{
					PFSettings.blemish.Uniformity_BoverG_corners_pct_max.value = subItemProp->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.blemish.Uniformity_RoverG_corners_pct_max.name)==0) // Uniformity_RoverG_corners_pct_max
				{
					PFSettings.blemish.Uniformity_RoverG_corners_pct_max.value = subItemProp->GetValue().dblVal;
				}

			}

		}
		/////////////////////////////////////////////
		// Update PFSettings.sfrplus
		/////////////////////////////////////////////
		else if (currentName.Compare(PFSettings.sfrplus.name)==0)
		{
			std::size_t subItemsCnt = currentProp->GetSubItemsCount();
			CMFCPropertyGridProperty* subItemProp;
			for ( std::size_t subIdx = 0; subIdx < subItemsCnt; ++subIdx)
			{
				subItemProp = currentProp->GetSubItem(subIdx); // Get the sub item
				CString subItemName = subItemProp->GetName();

				if (subItemName.Compare(PFSettings.sfrplus.All_Edge_IDs_detected.name)==0) // All_Edge_IDs_detected
				{
					PFSettings.sfrplus.All_Edge_IDs_detected.value = subItemProp->GetValue().lVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.Bayer_decode.name)==0) // Bayer_decode
				{
					PFSettings.sfrplus.Bayer_decode.value = subItemProp->GetValue().lVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.Chart_mean_pixel_level_bounds.group_name)==0) // Chart_mean_pixel_level_bounds
				{
					std::size_t subSubItemsCnt = subItemProp->GetSubItemsCount();
					PFSettings.sfrplus.Chart_mean_pixel_level_bounds.min_val = subItemProp->GetSubItem(0)->GetValue().dblVal;
					PFSettings.sfrplus.Chart_mean_pixel_level_bounds.max_val = subItemProp->GetSubItem(1)->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.Chart_radial_pixel_shift_max.name)==0) // Chart_radial_pixel_shift_max
				{
					PFSettings.sfrplus.Chart_radial_pixel_shift_max.value = subItemProp->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.Color_expected_detected.name)==0) // Color_expected_detected
				{
					PFSettings.sfrplus.Color_expected_detected.value = subItemProp->GetValue().lVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.Convergence_angle_max.name)==0) // Convergence_angle_max
				{
					PFSettings.sfrplus.Convergence_angle_max.value = subItemProp->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.FOV_degrees_diagonal_min.name)==0) // FOV_degrees_diagonal_min
				{
					PFSettings.sfrplus.FOV_degrees_diagonal_min.value = subItemProp->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.High_pixel_saturation_fraction_max.name)==0) // High_pixel_saturation_fraction_max
				{
					PFSettings.sfrplus.High_pixel_saturation_fraction_max.value = subItemProp->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.Horizontal_bars_OK_min.name)==0) // Horizontal_bars_OK_min
				{
					PFSettings.sfrplus.Horizontal_bars_OK_min.value = subItemProp->GetValue().lVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.Low_pixel_saturation_fraction_max.name)==0) // Low_pixel_saturation_fraction_max
				{
					PFSettings.sfrplus.Low_pixel_saturation_fraction_max.value = subItemProp->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.Mirrored_chart.name)==0) // Mirrored_chart
				{
					PFSettings.sfrplus.Mirrored_chart.value = subItemProp->GetValue().lVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.MTF50P_CP_weighted_mean_min.name)==0) // MTF50P_CP_weighted_mean_min
				{
					PFSettings.sfrplus.MTF50P_CP_weighted_mean_min.value = subItemProp->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.MTF50P_ratio_min.name)==0) // MTF50P_ratio_min
				{
					PFSettings.sfrplus.MTF50P_ratio_min.value = subItemProp->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.passfail_ini_file_date.name)==0) // passfail_ini_file_date
				{
					PFSettings.sfrplus.passfail_ini_file_date.value = subItemProp->GetValue().bstrVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.Rotation_degrees_max.name)==0) // Rotation_degrees_max
				{
					PFSettings.sfrplus.Rotation_degrees_max.value = subItemProp->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.Secondary_readout_1_center_mean_min.name)==0) // Secondary_readout_1_center_mean_min
				{
					PFSettings.sfrplus.Secondary_readout_1_center_mean_min.value = subItemProp->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.Secondary_readout_1_outer_mean_min.name)==0) // Secondary_readout_1_outer_mean_min
				{
					PFSettings.sfrplus.Secondary_readout_1_outer_mean_min.value = subItemProp->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.Secondary_readout_1_outer_min_min.name)==0) // Secondary_readout_1_outer_min_min
				{
					PFSettings.sfrplus.Secondary_readout_1_outer_min_min.value = subItemProp->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.Secondary_readout_1_outer_quadrant_delta_max.name)==0) // Secondary_readout_1_outer_quadrant_delta_max
				{
					PFSettings.sfrplus.Secondary_readout_1_outer_quadrant_delta_max.value = subItemProp->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min.name)==0) // Secondary_readout_1_outer_quadrant_mean_min_min
				{
					PFSettings.sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min.value = subItemProp->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.Secondary_readout_2_center_mean_min.name)==0) // Secondary_readout_2_center_mean_min
				{
					PFSettings.sfrplus.Secondary_readout_2_center_mean_min.value = subItemProp->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.Secondary_readout_2_outer_mean_min.name)==0) // Secondary_readout_2_outer_mean_min
				{
					PFSettings.sfrplus.Secondary_readout_2_outer_mean_min.value = subItemProp->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.Secondary_readout_2_outer_min_min.name)==0) // Secondary_readout_2_outer_min_min
				{
					PFSettings.sfrplus.Secondary_readout_2_outer_min_min.value = subItemProp->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.Secondary_readout_2_outer_quadrant_delta_max.name)==0) // Secondary_readout_2_outer_quadrant_delta_max
				{
					PFSettings.sfrplus.Secondary_readout_2_outer_quadrant_delta_max.value = subItemProp->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min.name)==0) // Secondary_readout_2_outer_quadrant_mean_min_min
				{
					PFSettings.sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min.value = subItemProp->GetValue().dblVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.Stepchart_expected_detected.name)==0) // Stepchart_expected_detected
				{
					PFSettings.sfrplus.Stepchart_expected_detected.value = subItemProp->GetValue().lVal;
				}
				else if (subItemName.Compare(PFSettings.sfrplus.upside_down.name)==0) // upside_down
				{
					PFSettings.sfrplus.upside_down.value = subItemProp->GetValue().lVal;
				}
			}

		}

		/////////////////////////////////////////////
		// Update PFSettings.ois
		/////////////////////////////////////////////
	
		else if(currentName.Compare(PFSettings.ois.name)==0)
		{
			std::size_t subItemsCnt = currentProp->GetSubItemsCount();
			CMFCPropertyGridProperty* subItemProp;
			for ( std::size_t subIdx = 0; subIdx < subItemsCnt; ++subIdx)
			{
				subItemProp = currentProp->GetSubItem(subIdx); // Get the sub item
				CString subItemName = subItemProp->GetName();

				if (subItemName.Compare(PFSettings.ois.L_MTF50_delta2_gain_summary_all_dB_min.name)==0) // L_MTF50_delta2_gain_summary_all_dB_min
				{
					PFSettings.ois.L_MTF50_delta2_gain_summary_all_dB_min.value = subItemProp->GetValue().lVal;
				}
				else if (subItemName.Compare(PFSettings.ois.R_improve_ALL_dB_min.name)==0) // R_improve_ALL_dB_min
				{
					PFSettings.ois.R_improve_ALL_dB_min.value = subItemProp->GetValue().lVal;
				}
				else if (subItemName.Compare(PFSettings.ois.R_improve_H_dB_min.name)==0) // R_improve_H_dB_min
				{
					PFSettings.ois.R_improve_H_dB_min.value = subItemProp->GetValue().lVal;
				}
				else if (subItemName.Compare(PFSettings.ois.R_improve_V_dB_min.name)==0) // R_improve_V_dB_min
				{
					PFSettings.ois.R_improve_V_dB_min.value = subItemProp->GetValue().lVal;
				}
			}

		}

		/////////////////////////////////////////////
		// Update PFSettings.other
		/////////////////////////////////////////////
		else if(currentName.Compare(PFSettings.other.name)==0)
		{
			std::size_t subItemsCnt = currentProp->GetSubItemsCount();
			CMFCPropertyGridProperty* subItemProp;

			for ( std::size_t subIdx = 0; subIdx < subItemsCnt; ++subIdx)
			{
				subItemProp = currentProp->GetSubItem(subIdx); // Get the sub item
				CString subItemName = subItemProp->GetName();

				PFSettings.other.ent_vec[subIdx].value = subItemProp->GetValue().bstrVal;

			}

		}
	}

}
