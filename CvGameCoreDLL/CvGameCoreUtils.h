#pragma once

// utils.h

#ifndef CIV4_GAMECORE_UTILS_H
#define CIV4_GAMECORE_UTILS_H


//#include "CvStructs.h"
#include "CvGlobals.h"
#include "CvMap.h"

#ifndef _USRDLL
// use non inline functions when not in the dll
#define getMapINLINE	getMap
#define getGridHeightINLINE	getGridHeight
#define getGridWidthINLINE	getGridWidth
#define isWrapYINLINE	isWrapY
#define isWrapXINLINE	isWrapX
#define plotINLINE	plot
#define getX_INLINE	getX
#define getY_INLINE	getY

#endif

class CvPlot;
class CvCity;
class CvUnit;
class CvString;
class CvRandom;
class FAStarNode;
class FAStar;
class CvInfoBase;


#ifndef SQR
#define SQR(x) ( (x)*(x))
#endif

#undef max
#undef min

//sign function taken from FirePlace - JW
template<class T> __forceinline T getSign( T x ) { return (( x < 0 ) ? T(-1) : x > 0 ? T(1) : T(0)); };

/*	f1rpo (from AdvCiv):
	Can also use ScaledNum instead . Maybe these functions read a bit better
	in code that doesn't use fractions much. */
namespace intdiv
{
	inline int round(int iDividend, int iDivisor)
	{
		int iSign = ((iDividend ^ iDivisor) >= 0 ? 1 : -1);
		return (iDividend + iSign * iDivisor / 2) / iDivisor;
	}

	// The "u" functions are only for nonnegative numbers ...
	inline int uround(int iDividend, int iDivisor)
	{
		FAssert((iDividend ^ iDivisor) >= 0); // Both negative is OK
		return (iDividend + iDivisor / 2) / iDivisor;
	}

	inline int uceil(int iDividend, int iDivisor)
	{
		FAssert(iDividend >= 0 && iDivisor > 0);
		return 1 + (iDividend - 1) / iDivisor;
	}
}

/*  f1rpo: Hash based on kInputs. Plot index of capital factored in for
	increased range if ePlayer given. (ePlayer is ignored if it has no capital.) */
int intHash(std::vector<int> const& kInputs, PlayerTypes ePlayer);

// <f1rpo> from AdvCiv
inline int longLongToInt(long long x)
{
	FAssert(x <= MAX_INT && x >= MIN_INT);
	return static_cast<int>(std::max<long long>(std::min<long long>(MAX_INT, x), MIN_INT));
}

template<typename T>
inline short toShort(T x)
{
	BOOST_STATIC_ASSERT(sizeof(T) > sizeof(short));
	FAssert(x <= MAX_SHORT && x >= MIN_SHORT);
	return static_cast<short>(std::max<T>(std::min<T>(MAX_SHORT, x), MIN_SHORT));
}

template<typename T>
inline char toChar(T x)
{
	BOOST_STATIC_ASSERT(sizeof(T) > sizeof(char));
	FAssert(x <= MAX_CHAR && x >= MIN_CHAR);
	return static_cast<char>(std::max<T>(std::min<T>(MAX_CHAR, x), MIN_CHAR));
}

template<typename T>
inline wchar toWChar(T x)
{
	BOOST_STATIC_ASSERT(sizeof(T) > sizeof(wchar));
	FAssert(x <= WCHAR_MAX && x >= WCHAR_MIN);
	return static_cast<wchar>(std::max<T>(std::min<T>(WCHAR_MAX, x), WCHAR_MIN));
} // </f1rpo>


inline int range(int iNum, int iLow, int iHigh)
{
	FAssertMsg(iHigh >= iLow, "High should be higher than low");

	if (iNum < iLow)
	{
		return iLow;
	}
	else if (iNum > iHigh)
	{
		return iHigh;
	}
	else
	{
		return iNum;
	}
}

inline float range(float fNum, float fLow, float fHigh)
{
	FAssertMsg(fHigh >= fLow, "High should be higher than low");

	if (fNum < fLow)
	{
		return fLow;
	}
	else if (fNum > fHigh)
	{
		return fHigh;
	}
	else
	{
		return fNum;
	}
}

inline int coordDistance(int iFrom, int iTo, int iRange, bool bWrap)
{
	if (bWrap && (abs(iFrom - iTo) > (iRange / 2)))
	{
		return (iRange - abs(iFrom - iTo));
	}

	return abs(iFrom - iTo);
}

inline int wrapCoordDifference(int iDiff, int iRange, bool bWrap)
{
	if (bWrap)
	{
		if (iDiff > (iRange / 2))
		{
			return (iDiff - iRange);
		}
		else if (iDiff < -(iRange / 2))
		{
			return (iDiff + iRange);
		}
	}

	return iDiff;
}

inline int xDistance(int iFromX, int iToX)
{
	return coordDistance(iFromX, iToX, GC.getMapINLINE().getGridWidthINLINE(), GC.getMapINLINE().isWrapXINLINE());
}

inline int yDistance(int iFromY, int iToY)
{
	return coordDistance(iFromY, iToY, GC.getMapINLINE().getGridHeightINLINE(), GC.getMapINLINE().isWrapYINLINE());
}

inline int dxWrap(int iDX)																													// Exposed to Python
{
	return wrapCoordDifference(iDX, GC.getMapINLINE().getGridWidthINLINE(), GC.getMapINLINE().isWrapXINLINE());
}

inline int dyWrap(int iDY)																													// Exposed to Python
{
	return wrapCoordDifference(iDY, GC.getMapINLINE().getGridHeightINLINE(), GC.getMapINLINE().isWrapYINLINE());
}

// 4 | 4 | 3 | 3 | 3 | 4 | 4
// -------------------------
// 4 | 3 | 2 | 2 | 2 | 3 | 4
// -------------------------
// 3 | 2 | 1 | 1 | 1 | 2 | 3
// -------------------------
// 3 | 2 | 1 | 0 | 1 | 2 | 3
// -------------------------
// 3 | 2 | 1 | 1 | 1 | 2 | 3
// -------------------------
// 4 | 3 | 2 | 2 | 2 | 3 | 4
// -------------------------
// 4 | 4 | 3 | 3 | 3 | 4 | 4
//
// Returns the distance between plots according to the pattern above...
inline int plotDistance(int iX1, int iY1, int iX2, int iY2)													// Exposed to Python
{
	int iDX;
	int iDY;

	iDX = xDistance(iX1, iX2);
	iDY = yDistance(iY1, iY2);

	return (std::max(iDX, iDY) + (std::min(iDX, iDY) / 2));
}

// f1rpo (from K-Mod):
inline int plotDistance(CvPlot const* pFirst, CvPlot const* pSecond)
{
	return plotDistance(pFirst->getX_INLINE(), pFirst->getY_INLINE(),
			pSecond->getX_INLINE(), pSecond->getY_INLINE());
}

// 3 | 3 | 3 | 3 | 3 | 3 | 3
// -------------------------
// 3 | 2 | 2 | 2 | 2 | 2 | 3
// -------------------------
// 3 | 2 | 1 | 1 | 1 | 2 | 3
// -------------------------
// 3 | 2 | 1 | 0 | 1 | 2 | 3
// -------------------------
// 3 | 2 | 1 | 1 | 1 | 2 | 3
// -------------------------
// 3 | 2 | 2 | 2 | 2 | 2 | 3
// -------------------------
// 3 | 3 | 3 | 3 | 3 | 3 | 3
//
// Returns the distance between plots according to the pattern above...
inline int stepDistance(int iX1, int iY1, int iX2, int iY2)													// Exposed to Python
{
	return std::max(xDistance(iX1, iX2), yDistance(iY1, iY2));
}

// f1rpo (from K-Mod):
inline int stepDistance(CvPlot const* pFirst, CvPlot const* pSecond)
{
	return stepDistance(pFirst->getX_INLINE(), pFirst->getY_INLINE(),
			pSecond->getX_INLINE(), pSecond->getY_INLINE());
}

inline CvPlot* plotDirection(int iX, int iY, DirectionTypes eDirection)							// Exposed to Python
{
	if(eDirection == NO_DIRECTION)
	{
		return GC.getMapINLINE().plotINLINE(iX, iY);
	}
	else
	{
		return GC.getMapINLINE().plotINLINE((iX + GC.getPlotDirectionX()[eDirection]), (iY + GC.getPlotDirectionY()[eDirection]));
	}
}

inline CvPlot* plotDirection(CvPlot* pPlot, DirectionTypes eDirection)
{
	return plotDirection(pPlot->getX_INLINE(), pPlot->getY_INLINE(), eDirection);
}

inline CvPlot* plotCardinalDirection(int iX, int iY, CardinalDirectionTypes eCardinalDirection)	// Exposed to Python
{
	return GC.getMapINLINE().plotINLINE((iX + GC.getPlotCardinalDirectionX()[eCardinalDirection]), (iY + GC.getPlotCardinalDirectionY()[eCardinalDirection]));
}

inline CvPlot* plotXY(int iX, int iY, int iDX, int iDY)																// Exposed to Python
{
	return GC.getMapINLINE().plotINLINE((iX + iDX), (iY + iDY));
}

inline DirectionTypes directionXY(int iDX, int iDY)																		// Exposed to Python
{
	if ((abs(iDX) > DIRECTION_RADIUS) || (abs(iDY) > DIRECTION_RADIUS))
	{
		return NO_DIRECTION;
	}
	else
	{
		return GC.getXYDirection((iDX + DIRECTION_RADIUS), (iDY + DIRECTION_RADIUS));
	}
}

inline DirectionTypes reverseDirection(DirectionTypes iDirection)																		// Exposed to Python
{
	return (DirectionTypes)((iDirection + 4)%NUM_DIRECTION_TYPES);
}

inline DirectionTypes directionXY(const CvPlot* pFromPlot, const CvPlot* pToPlot)			// Exposed to Python
{
	return directionXY(dxWrap(pToPlot->getX_INLINE() - pFromPlot->getX_INLINE()), dyWrap(pToPlot->getY_INLINE() - pFromPlot->getY_INLINE()));
}

inline DirectionTypes getAdjacentDirection(DirectionTypes eDirection, bool bClockwise)
{
	int iDir = (int)eDirection;
	 iDir += bClockwise ? 1 : -1;
	return (DirectionTypes)((iDir + NUM_DIRECTION_TYPES) % NUM_DIRECTION_TYPES);
}

CvPlot* plotCity(int iX, int iY, int iIndex);																			// Exposed to Python
int plotCityXY(int iDX, int iDY);																									// Exposed to Python
int plotCityXY(const CvCity* pCity, const CvPlot* pPlot);													// Exposed to Python

CardinalDirectionTypes getOppositeCardinalDirection(CardinalDirectionTypes eDir);	// Exposed to Python 
DirectionTypes cardinalDirectionToDirection(CardinalDirectionTypes eCard);				// Exposed to Python
DllExport bool isCardinalDirection(DirectionTypes eDirection);															// Exposed to Python
DllExport DirectionTypes estimateDirection(int iDX, int iDY);																// Exposed to Python
DllExport DirectionTypes estimateDirection(const CvPlot* pFromPlot, const CvPlot* pToPlot);
DllExport float directionAngle(DirectionTypes eDirection);

DllExport bool atWar(TeamTypes eTeamA, TeamTypes eTeamB);												// Exposed to Python
bool isPotentialEnemy(TeamTypes eOurTeam, TeamTypes eTheirTeam);			// Exposed to Python

DllExport CvCity* getCity(IDInfo city);	// Exposed to Python
DllExport CvUnit* getUnit(IDInfo unit);	// Exposed to Python

bool isBeforeUnitCycle(const CvUnit* pFirstUnit, const CvUnit* pSecondUnit);
bool isPromotionValid(PromotionTypes ePromotion, UnitTypes eUnit, bool bLeader);	// Exposed to Python

int getPopulationAsset(int iPopulation);								// Exposed to Python
int getLandPlotsAsset(int iLandPlots);									// Exposed to Python
int getPopulationPower(int iPopulation);								// Exposed to Python
int getPopulationScore(int iPopulation);								// Exposed to Python
int getLandPlotsScore(int iLandPlots);									// Exposed to Python
int getTechScore(TechTypes eTech);											// Exposed to Python
int getWonderScore(BuildingClassTypes eWonderClass);		// Exposed to Python

ImprovementTypes finalImprovementUpgrade(ImprovementTypes eImprovement, int iCount = 0);		// Exposed to Python

int getWorldSizeMaxConscript(CivicTypes eCivic);								// Exposed to Python

bool isReligionTech(TechTypes eTech);														// Exposed to Python

bool isTechRequiredForUnit(TechTypes eTech, UnitTypes eUnit);							// Exposed to Python
bool isTechRequiredForBuilding(TechTypes eTech, BuildingTypes eBuilding);	// Exposed to Python
bool isTechRequiredForProject(TechTypes eTech, ProjectTypes eProject);		// Exposed to Python

bool isWorldUnitClass(UnitClassTypes eUnitClass);											// Exposed to Python
bool isTeamUnitClass(UnitClassTypes eUnitClass);											// Exposed to Python
bool isNationalUnitClass(UnitClassTypes eUnitClass);									// Exposed to Python
bool isLimitedUnitClass(UnitClassTypes eUnitClass);										// Exposed to Python

bool isWorldWonderClass(BuildingClassTypes eBuildingClass);						// Exposed to Python
bool isTeamWonderClass(BuildingClassTypes eBuildingClass);						// Exposed to Python
bool isNationalWonderClass(BuildingClassTypes eBuildingClass);				// Exposed to Python
bool isLimitedWonderClass(BuildingClassTypes eBuildingClass);					// Exposed to Python
int limitedWonderClassLimit(BuildingClassTypes eBuildingClass);

bool isWorldProject(ProjectTypes eProject);														// Exposed to Python
bool isTeamProject(ProjectTypes eProject);														// Exposed to Python
bool isLimitedProject(ProjectTypes eProject);													// Exposed to Python

__int64 getBinomialCoefficient(int iN, int iK);
int getCombatOdds(CvUnit* pAttacker, CvUnit* pDefender);							// Exposed to Python
/////////////////////////////////////////////////////////////////
// ADVANCED COMABT ODDS                         PieceOfMind    //
// BEGIN                                                       //
/////////////////////////////////////////////////////////////////
float getCombatOddsSpecific(CvUnit* pAttacker, CvUnit* pDefender, int n_A, int n_D);
/////////////////////////////////////////////////////////////////
// ADVANCED COMABT ODDS                         PieceOfMind    //
// END                                                         //
/////////////////////////////////////////////////////////////////

int getEspionageModifier(TeamTypes eOurTeam, TeamTypes eTargetTeam);							// Exposed to Python

DllExport void setTradeItem(TradeData* pItem, TradeableItems eItemType = TRADE_ITEM_NONE, int iData = 0);

bool isPlotEventTrigger(EventTriggerTypes eTrigger);

TechTypes getDiscoveryTech(UnitTypes eUnit, PlayerTypes ePlayer);

void setListHelp(wchar* szBuffer, const wchar* szStart, const wchar* szItem, const wchar* szSeparator, bool bFirst);
void setListHelp(CvWString& szBuffer, const wchar* szStart, const wchar* szItem, const wchar* szSeparator, bool bFirst);
void setListHelp(CvWStringBuffer& szBuffer, const wchar* szStart, const wchar* szItem, const wchar* szSeparator, bool bFirst);

// PlotUnitFunc's...
bool PUF_isGroupHead( const CvUnit* pUnit, int iData1 = -1, int iData2 = -1);
bool PUF_isPlayer( const CvUnit* pUnit, int iData1, int iData2 = -1);
bool PUF_isTeam( const CvUnit* pUnit, int iData1, int iData2 = -1);
bool PUF_isCombatTeam(const CvUnit* pUnit, int iData1, int iData2);
bool PUF_isOtherPlayer( const CvUnit* pUnit, int iData1, int iData2 = -1);
bool PUF_isOtherTeam( const CvUnit* pUnit, int iData1, int iData2 = -1);
bool PUF_isEnemy( const CvUnit* pUnit, int iData1, int iData2 = -1);
bool PUF_isParadrop(const CvUnit* pUnit, int iData1, int iData2 = -1);
bool PUF_isNonAlly(const CvUnit* pUnit, int iData1, int iData2 = -1);
bool PUF_isVisible( const CvUnit* pUnit, int iData1, int iData2 = -1);
bool PUF_isVisibleDebug( const CvUnit* pUnit, int iData1, int iData2 = -1);
bool PUF_canSiege( const CvUnit* pUnit, int iData1, int iData2 = -1);
bool PUF_isPotentialEnemy( const CvUnit* pUnit, int iData1, int iData2 = -1);
bool PUF_canDeclareWar( const CvUnit* pUnit, int iData1 = -1, int iData2 = -1);
bool PUF_canDefend( const CvUnit* pUnit, int iData1 = -1, int iData2 = -1);
bool PUF_isCityGarrison(const CvUnit* pUnit, int iData1 = -1, int iData2 = -1);
bool PUF_isMilitaryLandUnit(const CvUnit* pUnit, int iData1 = -1, int iData2 = -1);
bool PUF_isCivilianLandUnit(const CvUnit* pUnit, int iData1 = -1, int iData2 = -1);
bool PUF_isMilitaryNavalUnit(const CvUnit* pUnit, int iData1 = -1, int iData2 = -1);
bool PUF_isCivilianNavalUnit(const CvUnit* pUnit, int iData1 = -1, int iData2 = -1);
bool PUF_isMilitaryAirUnit(const CvUnit* pUnit, int iData1 = -1, int iData2 = -1);
bool PUF_isCivilianAirUnit(const CvUnit* pUnit, int iData1 = -1, int iData2 = -1);
bool PUF_cannotDefend( const CvUnit* pUnit, int iData1 = -1, int iData2 = -1);
bool PUF_canDefendGroupHead( const CvUnit* pUnit, int iData1 = -1, int iData2 = -1);
bool PUF_canDefendEnemy( const CvUnit* pUnit, int iData1, int iData2 = -1);
bool PUF_canDefendEnemyNoAnimal( const CvUnit* pUnit, int iData1, int iData2 = -1);
bool PUF_canDefendPotentialEnemy( const CvUnit* pUnit, int iData1, int iData2 = -1);
bool PUF_canAirAttack( const CvUnit* pUnit, int iData1 = -1, int iData2 = -1);
bool PUF_canAirDefend( const CvUnit* pUnit, int iData1 = -1, int iData2 = -1);
bool PUF_isFighting( const CvUnit* pUnit, int iData1, int iData2 = -1);
bool PUF_isAnimal( const CvUnit* pUnit, int iData1 = -1, int iData2 = -1);
bool PUF_isMilitaryHappiness( const CvUnit* pUnit, int iData1 = -1, int iData2 = -1);
bool PUF_isInvestigate( const CvUnit* pUnit, int iData1 = -1, int iData2 = -1);
bool PUF_isCounterSpy( const CvUnit* pUnit, int iData1 = -1, int iData2 = -1);
bool PUF_isSpy( const CvUnit* pUnit, int iData1 = -1, int iData2 = -1);
bool PUF_isUnitType( const CvUnit* pUnit, int iData1, int iData2 = -1);
bool PUF_isDomainType( const CvUnit* pUnit, int iData1, int iData2 = -1);
bool PUF_isUnitAIType( const CvUnit* pUnit, int iData1, int iData2 = -1);
bool PUF_isCityAIType( const CvUnit* pUnit, int iData1, int iData2 = -1);
bool PUF_isNotCityAIType( const CvUnit* pUnit, int iData1, int iData2 = -1);
bool PUF_isSelected( const CvUnit* pUnit, int iData1 = -1, int iData2 = -1);
bool PUF_makeInfoBarDirty(CvUnit* pUnit, int iData1 = -1, int iData2 = -1);
bool PUF_isNoMission(const CvUnit* pUnit, int iData1 = -1, int iData2 = -1);
bool PUF_isMissionAI(const CvUnit* pUnit, int iData1 = -1, int iData2 = -1);

/********************************************************************************/
/* 	Worker Counting						03.08.2010				Fuyu			*/
/********************************************************************************/
bool PUF_isNoMissionAI(const CvUnit* pUnit, int iData1 = -1, int iData2 = -1);
/********************************************************************************/
/* 	Worker Counting												END 			*/
/********************************************************************************/
bool PUF_isFiniteRange(const CvUnit* pUnit, int iData1 = -1, int iData2 = -1);
bool PUF_isAvailableUnitAITypeGroupie(const CvUnit* pUnit, int iData1, int iData2);
bool PUF_isUnitAITypeGroupie(const CvUnit* pUnit, int iData1, int iData2);
bool PUF_isFiniteRangeAndNotJustProduced(const CvUnit* pUnit, int iData1, int iData2);
bool PUF_isMissionary(const CvUnit* pUnit, int /*ReligionTypes*/ iData1, int /*PlayerTypes*/ iData2);
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/

// FAStarFunc...
int potentialIrrigation(FAStarNode* parent, FAStarNode* node, int data, const void* pointer, FAStar* finder);
int checkFreshWater(FAStarNode* parent, FAStarNode* node, int data, const void* pointer, FAStar* finder);
int changeIrrigated(FAStarNode* parent, FAStarNode* node, int data, const void* pointer, FAStar* finder);
int pathDestValid(int iToX, int iToY, const void* pointer, FAStar* finder);
int pathHeuristic(int iFromX, int iFromY, int iToX, int iToY);
int pathCost(FAStarNode* parent, FAStarNode* node, int data, const void* pointer, FAStar* finder);
int pathValid(FAStarNode* parent, FAStarNode* node, int data, const void* pointer, FAStar* finder);
int pathAdd(FAStarNode* parent, FAStarNode* node, int data, const void* pointer, FAStar* finder);
int stepDestValid(int iToX, int iToY, const void* pointer, FAStar* finder);
int stepHeuristic(int iFromX, int iFromY, int iToX, int iToY);
int stepValid(FAStarNode* parent, FAStarNode* node, int data, const void* pointer, FAStar* finder);
int stepCost(FAStarNode* parent, FAStarNode* node, int data, const void* pointer, FAStar* finder);
int stepAdd(FAStarNode* parent, FAStarNode* node, int data, const void* pointer, FAStar* finder);
/********************************************************************************/
/* 	BETTER_BTS_AI_MOD					11/30/08				jdog5000	*/
/* 																			*/
/* 																			*/
/********************************************************************************/
int teamStepValid(FAStarNode* parent, FAStarNode* node, int data, const void* pointer, FAStar* finder);
/********************************************************************************/
/* 	BETTER_BTS_AI_MOD						END								*/
/********************************************************************************/
int routeValid(FAStarNode* parent, FAStarNode* node, int data, const void* pointer, FAStar* finder);
int borderValid(FAStarNode* parent, FAStarNode* node, int data, const void* pointer, FAStar* finder);
int areaValid(FAStarNode* parent, FAStarNode* node, int data, const void* pointer, FAStar* finder);
int joinArea(FAStarNode* parent, FAStarNode* node, int data, const void* pointer, FAStar* finder);
int plotGroupValid(FAStarNode* parent, FAStarNode* node, int data, const void* pointer, FAStar* finder);
int countPlotGroup(FAStarNode* parent, FAStarNode* node, int data, const void* pointer, FAStar* finder);
int countRegion(FAStarNode* parent, FAStarNode* node, int data, const void* pointer, FAStar* finder);

bool moveToValid(CvSelectionGroup* pSelectionGroup, CvPlot* pToPlot, int iFlags);

//	Koashling - new pathing generator callback functions
int	NewPathHeuristicFunc(CvSelectionGroup* pGroup, int iFromX, int iFromY, int iToX, int iToY, int& iLimitCost);
int	NewPathCostFunc(CvPathGeneratorBase* generator, CvSelectionGroup* pGroup, int iFromX, int iFromY, int iToX, int iToY, int iFlags, int& iMovementRemaining, int iPathTurns, int& iToNodeCost, bool bIsTerminalNode);
bool ContextFreeNewPathValidFunc(CvSelectionGroup* pGroup, int iFromX, int iFromY, int iToX, int iToY, int iFlags, bool isTerminus, bool bMoveTerminationChecksOnly, int iPathTurns, bool* pbToNodeInvalidity, bool* pbValidAsTerminus);
bool NewPathValidFunc(CvSelectionGroup* pGroup, int iFromX, int iFromY, int iToX, int iToY, int iFlags, bool isTerminus, bool bMoveTerminationChecksOnly, int iPathTurns, bool& bToNodeInvalidity);
bool NewPathDestValid(CvSelectionGroup* pSelectionGroup, int iToX, int iToY, int iFlags, bool& bRequiresWar);
bool NewPathTurnEndValidityCheckRequired(CvSelectionGroup* pGroup, int iFlags);

int baseYieldToSymbol(int iNumYieldTypes, int iYieldStack);

bool isPickableName(const TCHAR* szName);

DllExport int* shuffle(int iNum, CvRandom& rand);
void shuffleArray(int* piShuffle, int iNum, CvRandom& rand);

int getTurnMonthForGame(int iGameTurn, int iStartYear, CalendarTypes eCalendar, GameSpeedTypes eSpeed);
int getTurnYearForGame(int iGameTurn, int iStartYear, CalendarTypes eCalendar, GameSpeedTypes eSpeed);

void getDirectionTypeString(CvWString& szString, DirectionTypes eDirectionType);
void getCardinalDirectionTypeString(CvWString& szString, CardinalDirectionTypes eDirectionType);
void getActivityTypeString(CvWString& szString, ActivityTypes eActivityType);
void getMissionTypeString(CvWString& szString, MissionTypes eMissionType);
void getMissionAIString(CvWString& szString, MissionAITypes eMissionAI);
void getUnitAIString(CvWString& szString, UnitAITypes eUnitAI);
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      02/21/10                                jdog5000      */
/*                                                                                              */
/* Lead From Behind                                                                             */
/************************************************************************************************/
// From Lead From Behind by UncutDragon
int LFBgetCombatOdds(int iAttackerLowFS,	int iAttackerHighFS, int iDefenderLowFS, int iDefenderHighFS, int iNeededRoundsAttacker, int iNeededRoundsDefender, int iAttackerOdds);
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

// BUG - Unit Experience - start
/*
 * Calculates the experience needed to reach the next level after the given level.
 */
int calculateExperience(int iLevel, PlayerTypes ePlayer);								// Exposed to Python
int calculateLevel(int iExperience, PlayerTypes ePlayer);								// Exposed to Python
// BUG - Unit Experience - end
/************************************************************************************************/
/* Afforess	                  Start		 06/15/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
DirectionTypes getOppositeDirection(DirectionTypes eDirection);
bool isAdjacentDirection(DirectionTypes eFacingDirection, DirectionTypes eOtherDirection);
/************************************************************************************************/
/* Afforess	                         END                                                        */
/************************************************************************************************/	

//	Koshling - abstract treaty length from the define int to allow scaling
int getTreatyLength(void);

void BeginDLLSerialization();
void EndDLLSerialization();

inline CvWString ReturnArgAndLeaveCriticalSection(CvWString arg)
{
	EndDLLSerialization();

	return arg;
}

#define DLL_SERIALIZE(x)	(BeginDLLSerialization(), ReturnArgAndLeaveCriticalSection(x))

void AddDLLMessage(PlayerTypes ePlayer, bool bForce, int iLength, CvWString szString, LPCTSTR pszSound = NULL,
		InterfaceMessageTypes eType = MESSAGE_TYPE_INFO, LPCSTR pszIcon = NULL, ColorTypes eFlashColor = NO_COLOR,
		int iFlashX = -1, int iFlashY = -1, bool bShowOffScreenArrows = false, bool bShowOnScreenArrows = false);

void AddMessage(PlayerTypes ePlayer, bool bForce, int iLength, CvWString szString, LPCTSTR pszSound = NULL,
	InterfaceMessageTypes eType = MESSAGE_TYPE_INFO, LPCSTR pszIcon = NULL, ColorTypes eFlashColor = NO_COLOR,
	int iFlashX = -1, int iFlashY = -1, bool bShowOffScreenArrows = false, bool bShowOnScreenArrows = false);

//	Koshling - better checksum algorithm that can be used when reasonably high quality
//	hashes are needed
class CvChecksum {
public:
    CvChecksum() { clear(); }
    void clear() { sum = 0; r = 55665; c1 = 52845; c2 = 22719;}
    void add(int i);
    void add(byte b);
    int get() { return (int)sum; }
protected:
    WORD r;
    WORD c1;
    WORD c2;
    DWORD sum;
}; 

#endif
