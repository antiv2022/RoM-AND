#pragma once

//  $Header:
//------------------------------------------------------------------------------------------------
//
//  FILE:    CvXMLLoadUtility.h
//
//  AUTHOR:  Eric MacDonald  --  8/2003
//
//  PURPOSE: Group of functions to load in the xml files for Civilization 4
//
//------------------------------------------------------------------------------------------------
//  Copyright (c) 2003 Firaxis Games, Inc. All rights reserved.
//------------------------------------------------------------------------------------------------
#ifndef XML_LOAD_UTILITY_H
#define XML_LOAD_UTILITY_H

//#include "CvStructs.h"
#include "CvInfos.h"
#include "CvGlobals.h"

class FXmlSchemaCache;
class FXml;
class CvGameText;
class CvCacheObject;
class CvImprovementBonusInfo;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  class : CvXMLLoadUtility
//
//  DESC:   Group of functions to load in the xml files for Civilization 4
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvXMLLoadUtility
{
private:
	class Impl;
//---------------------------------------PUBLIC INTERFACE---------------------------------
public:
	// default constructor
	DllExport CvXMLLoadUtility();
	// default destructor
	DllExport ~CvXMLLoadUtility(void);

	DllExport bool CreateFXml();
	DllExport void DestroyFXml();

	DllExport bool LoadPostMenuGlobals();
	DllExport bool LoadPreMenuGlobals();
	DllExport bool LoadBasicInfos();
	DllExport bool LoadPlayerOptions();
	DllExport bool LoadGraphicOptions();

	// read the global defines from a specific file
	DllExport bool ReadGlobalDefines(const TCHAR* szXMLFileName, CvCacheObject* cache);
	// loads globaldefines.xml and calls various other functions to load relevant global variables
	DllExport bool SetGlobalDefines();
	// loads globaltypes.xml and calls various other functions to load relevant global variables
	DllExport bool SetGlobalTypes();
	// loads calls various functions that load xml files that in turn load relevant global variables
	DllExport bool SetGlobals();
	// loads globaldefines.xml and calls various other functions to load relevant global variables
	DllExport bool SetPostGlobalsGlobalDefines();

	// calls various functions to release the memory associated with the global variables
	DllExport void CleanUpGlobalVariables();

	// releases global variables associated with items that can be reloaded
	DllExport void ResetLandscapeInfo();
	DllExport bool SetupGlobalLandscapeInfo();
	DllExport bool SetGlobalArtDefines();
	DllExport bool LoadGlobalText();
	DllExport bool SetHelpText();
	DllExport void ResetGlobalEffectInfo();

// for progress bars
	typedef void (*ProgressCB)(int iStepNum, int iTotalSteps, const char* szMessage);
	DllExport static int GetNumProgressSteps();
	DllExport void RegisterProgressCB(ProgressCB cbFxn) { m_pCBFxn = cbFxn; }

	// moves the current xml node from where it is now to the next non-comment node, returns false if it can't find one
	DllExport bool SkipToNextVal();

	// overloaded function that gets the child value of the tag with szName if there is only one child
	// value of that name
	DllExport void MapChildren();	// call this before GetChildXMLValByName to use fast searching
	DllExport bool GetChildXmlValByName(std::string& pszVal, const TCHAR* szName, char* pszDefault = NULL);
	DllExport bool GetChildXmlValByName(std::wstring& pszVal, const TCHAR* szName, wchar* pszDefault = NULL);
	// overloaded function that gets the child value of the tag with szName if there is only one child
	// value of that name
	DllExport bool GetChildXmlValByName(char* pszVal, const TCHAR* szName, char* pszDefault = NULL);
	DllExport bool GetChildXmlValByName(wchar* pszVal, const TCHAR* szName, wchar* pszDefault = NULL);
	// overloaded function that gets the child value of the tag with szName if there is only one child
	// value of that name
	DllExport bool GetChildXmlValByName(int* piVal, const TCHAR* szName, int iDefault = 0);
	// overloaded function that gets the child value of the tag with szName if there is only one child
	// value of that name
	DllExport bool GetChildXmlValByName(float* pfVal, const TCHAR* szName, float fDefault = 0.0f);
	// overloaded function that gets the child value of the tag with szName if there is only one child
	// value of that name
	DllExport bool GetChildXmlValByName(bool* pbVal, const TCHAR* szName, bool bDefault = false);
	// <f1rpo> (xmldefault)
	void SetInfoIDFromChildXmlVal(int& r, TCHAR const* szName);
	int GetChildTypeIndex();
	// </f1rpo>
	// loads an xml file into the FXml variable.  The szFilename parameter has
	// the m_szXmlPath member variable pre-pended to it to form the full pathname
	DllExport bool LoadCivXml(FXml* pFXml, const TCHAR* szFilename);

	// overloaded function that gets either the current xml node's or the next non-comment xml node's string value
	// depending on if the current node is a non-comment node or not
	bool GetXmlVal(std::wstring& pszVal, wchar* pszDefault = NULL);
	bool GetXmlVal(std::string& pszVal, char* pszDefault = NULL);
	// overloaded function that gets either the current xml node's or the next non-comment xml node's string value
	// depending on if the current node is a non-comment node or not
	bool GetXmlVal(wchar* pszVal, wchar* pszDefault = NULL);
	bool GetXmlVal(char* pszVal, char* pszDefault = NULL);
	// overloaded function that gets either the current xml node's or the next non-comment xml node's int value
	// depending on if the current node is a non-comment node or not
	bool GetXmlVal(int* piVal, int iDefault = 0);
	// overloaded function that gets either the current xml node's or the next non-comment xml node's float value
	// depending on if the current node is a non-comment node or not
	bool GetXmlVal(float* pfVal, float fDefault = 0.0f);
	// overloaded function that gets either the current xml node's or the next non-comment xml node's boolean value
	// depending on if the current node is a non-comment node or not
	bool GetXmlVal(bool* pbVal, bool bDefault = false);

	// overloaded function that sets the current xml node to it's next sibling and then
	//	gets the next non-comment xml node's string value
	bool GetNextXmlVal(std::string& pszVal, char* pszDefault = NULL);
	bool GetNextXmlVal(std::wstring& pszVal, wchar* pszDefault = NULL);
	// overloaded function that sets the current xml node to it's next sibling and then
	//	gets the next non-comment xml node's string value
	bool GetNextXmlVal(char* pszVal, char* pszDefault = NULL);
	bool GetNextXmlVal(wchar* pszVal, wchar* pszDefault = NULL);
	// overloaded function that sets the current xml node to it's next sibling and then
	//	gets the next non-comment xml node's int value
	bool GetNextXmlVal(int* piVal, int iDefault = 0);
	// overloaded function that sets the current xml node to it's next sibling and then
	//	gets the next non-comment xml node's float value
	bool GetNextXmlVal(float* pfVal, float fDefault = 0.0f);
	// overloaded function that sets the current xml node to it's next sibling and then
	//	gets the next non-comment xml node's boolean value
	bool GetNextXmlVal(bool* pbVal, bool bDefault = false);

	// overloaded function that sets the current xml node to it's first non-comment child node
	//	and then gets that node's string value
	bool GetChildXmlVal(std::string& pszVal, char* pszDefault = NULL);
	bool GetChildXmlVal(std::wstring& pszVal, wchar* pszDefault = NULL);
	// overloaded function that sets the current xml node to it's first non-comment child node
	//	and then gets that node's string value
	bool GetChildXmlVal(char* pszVal, char* pszDefault = NULL);
	bool GetChildXmlVal(wchar* pszVal, wchar* pszDefault = NULL);
	// overloaded function that sets the current xml node to it's first non-comment child node
	//	and then gets that node's integer value
	bool GetChildXmlVal(int* piVal, int iDefault = 0);
	// overloaded function that sets the current xml node to it's first non-comment child node
	//	and then gets that node's float value
	bool GetChildXmlVal(float* pfVal, float fDefault = 0.0f);
	// overloaded function that sets the current xml node to it's first non-comment child node
	//	and then gets that node's boolean value
	bool GetChildXmlVal(bool* pbVal, bool bDefault = false);

#ifdef _USRDLL
	FXml* GetXML() { return m_pFXml; }
#endif

	// loads the local yield from the xml file
	int SetYields(int** ppiYield);

#ifdef _USRDLL
	template <class T>
	int SetCommerce(T** ppiCommerce);
#endif
/************************************************************************************************/
/* Afforess	                  Start		 05/25/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	// allocate and set the feature struct variables for the CvBuildInfo class
	void SetFeatureStruct(int** ppiFeatureTech, int** ppiFeatureTime, int** ppiFeatureProduction, bool** ppbFeatureRemove, bool** ppbNoTechCanRemoveWithNoProductionGain);
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/

	// loads the improvement bonuses from the xml file
	void SetImprovementBonuses(CvImprovementBonusInfo** ppImprovementBonus);
    
    void setExtraFreeBuildings(std::vector<int> &m_aiExtraFreeBuildingClass, std::vector<bool> &m_abExtraFreeBuildingConnected, std::vector<bool> &m_abExtraFreeBuildingContinuous); // DarkLunaPhantom - Reads ExtraFreeBuilding entries from BuildingInfos.
	
/*************************************************************************************************/
/**	New Tag Defs	(XMLInfos)				08/09/08								Xienwolf	**/
/**																								**/
/**								Defines Function for Use in .cpp								**/
/*************************************************************************************************/
	// acquire a list of unknown length of String data from a Child setup in a String (typically for Pass 3 Use)
    void SetStringWithChildList(int* iNumEntries, std::vector<CvString>* aszXMLLoad);
	// acquire a list of unknown length of String data from a Child setup in an Array of INTs
    void SetIntWithChildList(int* iNumEntries, int** piXMLLoad);
	// acquire a list of known length of String data from a Child setup true values in an Array of BOOLs
    void SetBoolFromChildList(int iNumEntries, bool** pbXMLLoad);
/*************************************************************************************************/
/**	New Tag Defs							END													**/
/*************************************************************************************************/

	// check through the pszList parameter for the pszVal and returns the location a match
	// is found if one is found
	static int FindInInfoClass(const TCHAR* pszVal, bool hideAssert = false);

#ifdef _USRDLL
	template <class T>
	static void InitList(T **ppList, int iListLen, T val = 0);
#endif
	void InitStringList(CvString **ppszList, int iListLen, CvString szString);

	void InitImprovementBonusList(CvImprovementBonusInfo** ppImprovementBonus, int iListLen);
	// allocate and initialize the civilization's default buildings
	void InitBuildingDefaults(int **ppiDefaults);
	// allocate and initialize the civilization's default units
	void InitUnitDefaults(int **ppiDefaults);
/************************************************************************************************/
/* Afforess	                  Start		 08/26/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	// allocate and initialize a 2 dimensional array of bool pointers
	static void Init2DBoolList(bool*** pppbList, int iSizeX, int iSizeY);
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
	// allocate and initialize a 2 dimensional array of int pointers
	static void Init2DIntList(int*** pppiList, int iSizeX, int iSizeY);
	// allocate and initialize a 2 dimensional array of float pointers
	static void Init2DFloatList(float*** pppfList, int iSizeX, int iSizeY);
	// allocate and initialize a 2D array of DirectionTypes
	static void Init2DDirectionTypesList(DirectionTypes*** pppiList, int iSizeX, int iSizeY);
	// allocate an array of int pointers
	static void InitPointerIntList(int*** pppiList, int iSizeX);
	// allocate an array of float pointers
	static void InitPointerFloatList(float*** pppfList, int iSizeX);

	// allocate and initialize a list from a tag pair in the xml
	void SetVariableListTagPair(int **ppiList, const TCHAR* szRootTagName,
		int iInfoBaseSize, int iInfoBaseLength, int iDefaultListVal = 0,
		bool bCollapse = false); // f1rpo

/************************************************************************************************/
/* RevDCM  XMLloading                             05/05/10             phungus420               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	// allocate and initialize a list from a tag pair in the xml by tag name
	void SetVariableListTagPair(int **ppiList, const TCHAR* szRootTagName,
		int iInfoBaseSize, int iInfoBaseLength,
		const TCHAR* szValueTagName, int iValueInfoBaseLength, int iDefaultListVal = -1);
/************************************************************************************************/
/* RevDCM	                                 END                                                */
/************************************************************************************************/

	// allocate and initialize a list from a tag pair in the xml
	void SetVariableListTagPairInfo(CvDynamicArray<bool> *pList, const TCHAR* rootTagName,
		int iagListLength, bool defaultListVal);
	void SetVariableListTagPairInfo(CvDynamicArray<int> *pList, const TCHAR* rootTagName,
		int tagListLength, int defaultListVal);

	void SetVariableListTagPairEnum(CvDynamicArray<bool> *pList, const TCHAR* rootTagName,
		int iagListLength, bool defaultListVal);
	void SetVariableListTagPairEnum(CvDynamicArray<int> *pList, const TCHAR* rootTagName,
		int tagListLength, int defaultListVal);

	void SetVariableListTagPairForAudioScriptsInfo(CvDynamicArray<int> *pList, const TCHAR* szRootTagName,
		int tagListLength, int iDefaultListVal = -1);



	void SetVariableListTagPair(bool **ppbList, const TCHAR* szRootTagName,
		int iInfoBaseSize, int iInfoBaseLength, bool bDefaultListVal = false);


	// allocate and initialize a list from a tag pair in the xml
	void SetVariableListTagPair(float **ppfList, const TCHAR* szRootTagName,
		int iInfoBaseSize, int iInfoBaseLength, float fDefaultListVal = 0.0f);

	// allocate and initialize a list from a tag pair in the xml
	void SetVariableListTagPair(CvString **ppszList, const TCHAR* szRootTagName,
		int iInfoBaseSize, int iInfoBaseLength, CvString szDefaultListVal = "");

	// allocate and initialize a list from a tag pair in the xml
	void SetVariableListTagPair(int **ppiList, const TCHAR* szRootTagName,
		CvString* m_paszTagList, int iTagListLength, int iDefaultListVal = 0);

	// allocate and initialize a list from a tag pair in the xml for audio scripts
	void SetVariableListTagPairForAudioScripts(int **ppiList, const TCHAR* szRootTagName,
		CvString* m_paszTagList, int iTagListLength, int iDefaultListVal = -1);

	// allocate and initialize a list from a tag pair in the xml
	void SetVariableListTagPair(bool **ppbList, const TCHAR* szRootTagName,
		CvString* m_paszTagList, int iTagListLength, bool bDefaultListVal = false);

	// allocate and initialize a list from a tag pair in the xml
	void SetVariableListTagPair(CvString **ppszList, const TCHAR* szRootTagName,
		CvString* m_paszTagList, int iTagListLength, CvString szDefaultListVal = "");

	// create a hot key from a description
	CvWString CreateHotKeyFromDescription(const TCHAR* pszHotKey, bool bShift = false, bool bAlt = false, bool bCtrl = false);

	// set the variable to a default and load it from the xml if there are any children
	bool SetAndLoadVar(int** ppiVar, int iDefault=0);

	// function that sets the number of strings in a list, initializes the string to the correct length, and fills it from the
	// current xml file, it assumes that the current node is the parent node of the string list children
	bool SetStringList(CvString** ppszStringArray, int* piSize);

	// get the integer value for the keyboard mapping of the hotkey if it exists
	int GetHotKeyInt(const TCHAR* pszHotKeyVal);

/************************************************************************************************/
/* MODULAR_LOADING_CONTROL                 11/30/07                                MRGENIE      */
/*                                                                                              */
/* Savegame compatibility                                                                       */
/************************************************************************************************/
	bool doResetGlobalInfoClasses();
	bool doResetInfoClasses();
/************************************************************************************************/
/* MODULAR_LOADING_CONTROL                 END                                                  */
/************************************************************************************************/
/************************************************************************************************/
/* TGA_INDEXATION                          01/21/08                                MRGENIE      */
/*                                                                                              */
/* reorganizing the Corporations and Religions vectors                                          */
/************************************************************************************************/
	template <class T>
	void ArrangeTGA(std::vector<T*>& aInfos, const char* szInfo);
	template <class T>
	void AddTGABogus(std::vector<T*>& aInfos, const char* szInfo);
	void cleanTGA();
	template <class T>
	void RemoveTGABogusReligion(std::vector<T*>& aInfos);
	template <class T>
	void RemoveTGABogusCorporation(std::vector<T*>& aInfos);
/************************************************************************************************/
/* TGA_INDEXATION                          END                                                  */
/************************************************************************************************/
/************************************************************************************************/
/* XML_MODULAR_ART_LOADING                 10/19/07                                MRGENIE      */
/*                                                                                              */
/* Needs to be public for CvXMLLoadUtilityModTools                                              */
/************************************************************************************************/
#ifdef _DEBUG	
	void XmlArtTagVerification(char* format, ... );
#endif
/************************************************************************************************/
/* XML_MODULAR_ART_LOADING                 END                                                  */
/************************************************************************************************/
/************************************************************************************************/
/* MODULAR_LOADING_CONTROL                 10/30/07                                MRGENIE      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	void logMLF(char* format, ... );
/************************************************************************************************/
/* XML_MODULAR_ART_LOADING                 END                                                  */
/************************************************************************************************/
	//---------------------------------------PRIVATE MEMBER VARIABLES---------------------------------
private:
	FXml* m_pFXml;						// member variable pointer to the current FXml class
	FXmlSchemaCache* m_pSchemaCache;	// keep a single schema cache, instead of loading the same schemas multiple times
	int m_iCurProgressStep;
	ProgressCB m_pCBFxn;

//---------------------------------------PRIVATE INTERFACE---------------------------------
private:
	void UpdateProgressCB(const char* szMessage=NULL);

/************************************************************************************************/
/* MODULAR_LOADING_CONTROL                 10/24/07                                MRGENIE      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	// Python Modular Loading
	template <class T>
	void LoadPythonModulesInfo(std::vector<T*>& aInfos, const char* szFileRoot, const char* szXmlPath, bool bTwoPass);
	//Main control of the MLF feature
	void ModularLoadingControlXML();
	// In the next 2 methods we load the MLF classes
	template <class T>
	bool LoadModLoadControlInfo(std::vector<T*>& aInfos, const char* szFileRoot, const char* szXmlPath);
	template <class T>
	bool SetModLoadControlInfo(std::vector<T*>& aInfos, const char* szTagName, CvString szConfigString, CvString szDirDepth = "Modules\\", int iDirDepth = 0);
/************************************************************************************************/
/* MODULAR_LOADING_CONTROL                 END                                                  */
/************************************************************************************************/
/************************************************************************************************/
/* SORT_ALPHABET                           11/17/07                                MRGENIE      */
/*                                                                                              */
/* Method for alphabetically order tags                                                         */
/************************************************************************************************/
	template <class T>
	void SortAlphabetGlobalClassInfo(std::vector<T*>& aInfos);
/************************************************************************************************/
/* SORT_ALPHABET                           END                                                  */
/************************************************************************************************/
	// take a character string of hex values and return their unsigned int value
	void MakeMaskFromString(unsigned int *puiMask, char* szMask);

	// find the tag name in the xml file and set the string parameter and num val parameter based on it's value
	void SetGlobalStringArray(CvString** ppszString, char* szTagName, int* iNumVals, bool bUseEnum=false);
	void SetDiplomacyCommentTypes(CvString** ppszString, int* iNumVals);	// sets diplomacy comments

	void SetGlobalUnitScales(float* pfLargeScale, float* pfSmallScale, char* szTagName);

#ifdef _USRDLL
	template <class T>
		void SetGlobalDefine(const char* szDefineName, T*& piDefVal)
	{ GC.getDefinesVarSystem()->GetValue(szDefineName, piDefVal); }
#endif
	//
	// template which can handle all info classes
	//
	// a dynamic value for the list size
#ifdef _USRDLL
	template <class T>
	void SetGlobalClassInfo(std::vector<T*>& aInfos, const char* szTagName, bool bTwoPass, CvInfoReplacements<T>* pReplacements = NULL);
/************************************************************************************************/
/* MODULAR_LOADING_CONTROL                 05/17/08                                MRGENIE      */
/*                                                                                              */
/* This method is a replacement for the bTwoPass, if stuff that is depending on each other in   */
/* a loop, the bTwoPass would fail since it doesn't look first in the other Modules!            */
/************************************************************************************************/
	template <class T>
	void SetGlobalClassInfoTwoPassReplacement(std::vector<T*>& aInfos, const char* szTagName, CvInfoReplacements<T>* pReplacements = NULL);
/************************************************************************************************/
/* MODULAR_LOADING_CONTROL                 END                                                  */
/************************************************************************************************/
	template <class T>
	void LoadGlobalClassInfo(std::vector<T*>& aInfos, const char* szFileRoot, const char* szFileDirectory, const char* szXmlPath, bool bTwoPass, CvCacheObject* (CvDLLUtilityIFaceBase::*pArgFunction) (const TCHAR*) = NULL, CvInfoReplacements<T>* pReplacements = NULL);
/************************************************************************************************/
/* MODULAR_LOADING_CONTROL                 05/13/08                                MRGENIE      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	/*
	template <class T>
	void LoadGlobalClassInfoModular(std::vector<T*>& aInfos, const char* szFileRoot, const char* szFileDirectory, const char* szXmlPath, bool bTwoPass);*/
/************************************************************************************************/
/* MODULAR_LOADING_CONTROL                 END                                                  */
/************************************************************************************************/
#endif
	void SetDiplomacyInfo(std::vector<CvDiplomacyInfo*>& DiploInfos, const char* szTagName);
	void LoadDiplomacyInfo(std::vector<CvDiplomacyInfo*>& DiploInfos, const char* szFileRoot, const char* szFileDirectory, const char* szXmlPath, CvCacheObject* (CvDLLUtilityIFaceBase::*pArgFunction) (const TCHAR*));

	//
	// special cases of set class info which don't use the template because of extra code they have
	//
	void SetGlobalActionInfo();
	void SetGlobalAnimationPathInfo(CvAnimationPathInfo** ppAnimationPathInfo, char* szTagName, int* iNumVals);
	void SetGameText(const char* szTextGroup, const char* szTagName);

	// create a keyboard string from a KB code, Delete would be returned for KB_DELETE
	CvWString CreateKeyStringFromKBCode(const TCHAR* pszHotKey);

	void orderHotkeyInfo(int** ppiSortedIndex, int* pHotkeyIndex, int iLength);
	void logMsg(char* format, ... );
/************************************************************************************************/
/* XML_CHECK_DOUBLE_TYPE                   10/10/07                                MRGENIE      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
#ifdef _DEBUG
	void logXmlCheckDoubleTypes(char* format, ... );
#endif
/************************************************************************************************/
/* XML_CHECK_DOUBLE_TYPE                   END                                                  */
/************************************************************************************************/
/************************************************************************************************/
/* MODULAR_LOADING_CONTROL                 02/20/08                                MRGENIE      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
//#ifdef _DEBUG	
	void logXmlDependencyTypes(char* format, ... );
//#endif
/************************************************************************************************/
/* MODULAR_LOADING_CONTROL                 END                                                  */
/************************************************************************************************/
/************************************************************************************************/
/* Afforess	                  Start		 06/13/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	void logXML(char* format, ... );
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
};

#ifdef _USRDLL
//
/////////////////////////// inlines / templates
//
template <class T>
void CvXMLLoadUtility::InitList(T **ppList, int iListLen, T val)
{
	// <f1rpo> (xmldefault)
	if (*ppList != NULL) // Already initialized w/ default values
		return; // </f1rpo>
	FAssertMsg((0 <= iListLen),"list size to allocate is less than 0");
	*ppList = new T[iListLen];

	for (int i=0;i<iListLen;i++)
		(*ppList)[i] = val;
}

template <class T>
int CvXMLLoadUtility::SetCommerce(T** ppbCommerce)
{
	int i=0;			//loop counter
	int iNumSibs=0;		// the number of siblings the current xml node has
	T *pbCommerce;	// local pointer for the Commerce memory

	// Skip any comments and stop at the next value we might want
	if (SkipToNextVal())
	{
		// get the total number of children the current xml node has
		iNumSibs = GETXML->GetNumChildren(m_pFXml);
		InitList(ppbCommerce, NUM_COMMERCE_TYPES);

		pbCommerce = *ppbCommerce;
		if (0 < iNumSibs)
		{
			// if the call to the function that sets the current xml node to it's first non-comment
			// child and sets the parameter with the new node's value succeeds
			if (GetChildXmlVal(&pbCommerce[0]))
			{
				FAssertMsg((iNumSibs <= NUM_COMMERCE_TYPES) , "For loop iterator is greater than array size");
				// loop through all the siblings, we start at 1 since we already have the first value
				for (i=1;i<iNumSibs;i++)
				{
					if (!GetNextXmlVal(&pbCommerce[i]))
					{
						break;
					}
				}
				GETXML->SetToParent(m_pFXml);
			}
		}
	}

	return iNumSibs;
}
#endif

#endif	// XML_LOAD_UTILITY_H
