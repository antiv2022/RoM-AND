#pragma once

// cityAI.h

#ifndef CIV4_CITY_AI_H
#define CIV4_CITY_AI_H

#include "CvCity.h"
#include "ByteBuffer.h"

//	Possible focus flags to use when evaluating buildings
#define BUILDINGFOCUS_FOOD					(1 << 1)
#define BUILDINGFOCUS_PRODUCTION			(1 << 2)
#define BUILDINGFOCUS_GOLD					(1 << 3)
#define BUILDINGFOCUS_RESEARCH				(1 << 4)
#define BUILDINGFOCUS_CULTURE				(1 << 5)
#define BUILDINGFOCUS_DEFENSE				(1 << 6)
#define BUILDINGFOCUS_HAPPY					(1 << 7)
#define BUILDINGFOCUS_HEALTHY				(1 << 8)
#define BUILDINGFOCUS_EXPERIENCE			(1 << 9)
#define BUILDINGFOCUS_MAINTENANCE			(1 << 10)
#define BUILDINGFOCUS_SPECIALIST			(1 << 11)
#define BUILDINGFOCUS_ESPIONAGE				(1 << 12)
#define BUILDINGFOCUS_BIGCULTURE			(1 << 13)
#define BUILDINGFOCUS_WORLDWONDER			(1 << 14)
#define BUILDINGFOCUS_DOMAINSEA				(1 << 15)
#define BUILDINGFOCUS_WONDEROK				(1 << 16)
#define BUILDINGFOCUS_CAPITAL				(1 << 17)

#define BUILDINGFOCUS_CONSIDER_ANY			(1 << 18)

//	KOSHLING Mod - pre-calculate and cache building values for all focuses
class	BuildingValueCache;

//	Koshling - add caching to yield calculations
#ifdef YIELD_VALUE_CACHING
struct yieldValueCacheEntry
{
	yieldValueCacheEntry() { iLastUseCount = 0; }

	short aiYields[NUM_YIELD_TYPES];
	short aiCommerceYields[NUM_COMMERCE_TYPES];
	bool bAvoidGrowth;
	bool bRemove;
	bool bIgnoreFood;
	bool bIgnoreGrowth;
	bool bIgnoreStarvation;
	bool bWorkerOptimization;
	int iResult;
	int iLastUseCount;
};

#define YIELD_VALUE_CACHE_SIZE 16

class yieldCache
{
public:
	yieldCache()
	{
		currentUseCounter = 0;
	}

	struct yieldValueCacheEntry entries[YIELD_VALUE_CACHE_SIZE];
	int currentUseCounter;
};
#endif

typedef std::vector<std::pair<UnitAITypes, int> > UnitTypeWeightArray;

typedef struct
{
	UnitTypes	eUnit;
	int			iValue;
} UnitValueInfo;

class CvCityAI : public CvCity
{

public:

	CvCityAI();
	virtual ~CvCityAI();

	void AI_init();
	void AI_uninit();
	void AI_reset();

	void AI_doTurn();

	void AI_assignWorkingPlots();
	void AI_updateAssignWork();
	void AI_doContractFloatingDefenders();

	bool AI_avoidGrowth();
	bool AI_ignoreGrowth();
	int AI_specialistValue(SpecialistTypes eSpecialist, bool bAvoidGrowth, bool bRemove);

	//	KOSHLING - initialisation called on every city prior to performing unit mission allocation logic
	//	This allows caches that will remain valid for the processing of the current turn's units to be cleared
	virtual void AI_preUnitTurn(void);

	virtual void AI_noteUnitEscortNeeded(void);
	virtual void AI_trained(UnitTypes eUnitType, UnitAITypes eUnitAIType);

	bool AI_changeProduction();
	void AI_prepareForTurnProcessing();
	void AI_chooseProduction();
	void AI_chooseProductionExperimental(); // Experimental test (dbkblk, 2015-03)

	UnitTypes AI_bestUnit(int& iBestValue, int iNumSelectableTypes = -1, UnitAITypes* pSelectableTypes = NULL, bool bAsync = false, UnitAITypes* peBestUnitAI = NULL, bool bNoRand = false, bool bNoWeighting = false, CvUnitSelectionCriteria* criteria = NULL);
/********************************************************************************/
/* 	City Defenders						24.07.2010				Fuyu			*/
/********************************************************************************/
//Fuyu bIgnoreNotUnitAIs
	UnitTypes AI_bestUnitAI(UnitAITypes eUnitAI, int& iBestValue, bool bAsync = false, bool bNoRand = false, CvUnitSelectionCriteria* criteria = NULL);
/********************************************************************************/
/* 	City Defenders												END 			*/
/********************************************************************************/

	BuildingTypes AI_bestBuilding(int iFocusFlags = 0, int iMaxTurns = 0, bool bAsync = false, AdvisorTypes eIgnoreAdvisor = NO_ADVISOR, bool bMaximizeFlaggedValue = false);
	BuildingTypes AI_bestBuildingThreshold(int iFocusFlags = 0, int iMaxTurns = 0, int iMinThreshold = 0, bool bAsync = false, AdvisorTypes eIgnoreAdvisor = NO_ADVISOR, bool bMaximizeFlaggedValue = false);
	
	int AI_buildingValue(BuildingTypes eBuilding, int iFocusFlags = 0, bool bForTech = false);
	int AI_buildingValueThreshold(BuildingTypes eBuilding, int iFocusFlags = 0, int iThreshold = 0, bool bMaximizeFlaggedValue = false, bool bIgnoreCanConstruct = false);
	int AI_buildingValueThresholdOriginal(BuildingTypes eBuilding, int iFocusFlags = 0, int iThreshold = 0, bool bMaximizeFlaggedValue = false, bool bIgnoreCanBuildReplacement = false, bool bForTech = false);
	int AI_buildingValueThresholdOriginalUncached(BuildingTypes eBuilding, int iFocusFlags = 0, int iThreshold = 0, bool bMaximizeFlaggedValue = false, bool bIgnoreCanBuildReplacement = false, bool bForTech = false);
	int getBuildingCommerceValue(BuildingTypes eBuilding, int iI, int* aiFreeSpecialistYield, int* aiFreeSpecialistCommerce, int* aiBaseCommerceRate, int* aiPlayerCommerceRate);
	int AI_buildingYieldValue(YieldTypes eYield, BuildingTypes eBuilding, CvBuildingInfo& kBuilding, bool bForeignTrade, int iFoodDifference, int iFreeSpecialistYield);

	ProjectTypes AI_bestProject();
	int AI_projectValue(ProjectTypes eProject);

	ProcessTypes AI_bestProcess(CommerceTypes eCommerceType = NO_COMMERCE, int* commerceWeights = NULL);
	int AI_processValue(ProcessTypes eProcess, CommerceTypes eCommerceType = NO_COMMERCE, int* commerceWeights = NULL);

	int AI_neededSeaWorkers();

	bool AI_isDefended(int iExtra = 0, bool bAllowAnyDefenders = true);
/********************************************************************************/
/* 	BETTER_BTS_AI_MOD							9/19/08		jdog5000		    */
/* 																			    */
/* 	Air AI																	    */
/********************************************************************************/
/* original BTS code
	bool AI_isAirDefended(int iExtra = 0);
*/
	bool AI_isAirDefended(bool bCountLand = false, int iExtra = 0);
/********************************************************************************/
/* 	BETTER_BTS_AI_MOD						END								    */
/********************************************************************************/
	bool AI_isDanger();
	int evaluateDanger();

	bool AI_isAdequateHappinessMilitary(int iExtra);
	int AI_neededHappinessDefenders();
	int AI_neededDefenders();
	int AI_neededAirDefenders();
	int AI_minDefenders();
	int AI_neededFloatingDefenders();

	int getGarrisonStrength(bool bAllowAnyDefenders = true) const;
	int AI_neededDefenseStrength();
	int AI_minDefenseStrength();

	int AI_getEmphasizeAvoidGrowthCount();
	bool AI_isEmphasizeAvoidGrowth();

	int AI_getEmphasizeGreatPeopleCount();
	bool AI_isEmphasizeGreatPeople();

	bool AI_isAssignWorkDirty();
	void AI_setAssignWorkDirty(bool bNewValue);

	bool AI_isChooseProductionDirty();
	void AI_setChooseProductionDirty(bool bNewValue);

	CvCity* AI_getRouteToCity() const;
	void AI_updateRouteToCity() const;

	int AI_getEmphasizeYieldCount(YieldTypes eIndex);
	bool AI_isEmphasizeYield(YieldTypes eIndex);

	int AI_getEmphasizeCommerceCount(CommerceTypes eIndex);
	bool AI_isEmphasizeCommerce(CommerceTypes eIndex);

	bool AI_isEmphasize(EmphasizeTypes eIndex);
	void AI_setEmphasize(EmphasizeTypes eIndex, bool bNewValue);
	bool AI_isEmphasizeSpecialist(SpecialistTypes eIndex);
	void AI_setEmphasizeSpecialist(SpecialistTypes eIndex, bool bNewValue);
	void AI_forceEmphasizeCulture(bool bNewValue);

	void AI_markBestBuildValuesStale(void);
	int AI_getBestBuildValue(int iIndex);
	int AI_totalBestBuildValue(CvArea* pArea);

	int AI_clearFeatureValue(int iIndex);
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      07/25/09                                jdog5000      */
/*                                                                                              */
/* Debug                                                                                        */
/************************************************************************************************/
	int AI_getGoodTileCount();
	int AI_countWorkedPoorTiles();
	int AI_getTargetSize();
	void AI_getYieldMultipliers();
	int AI_getImprovementValue();
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/
	BuildTypes AI_getBestBuild(int iIndex);
	int AI_countBestBuilds(CvArea* pArea);
	void AI_updateBestBuild();

	virtual int AI_cityValue() const;
    
    int AI_calculateWaterWorldPercent();
    
    int AI_getCityImportance(bool bEconomy, bool bMilitary);
    
    int AI_yieldMultiplier(YieldTypes eYield);
    void AI_updateSpecialYieldMultiplier();
    int AI_specialYieldMultiplier(YieldTypes eYield);
    
    int AI_countNumBonuses(BonusTypes eBonus, bool bIncludeOurs, bool bIncludeNeutral, int iOtherCultureThreshold, bool bLand = true, bool bWater = true);
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      11/14/09                                jdog5000      */
/*                                                                                              */
/* City AI                                                                                      */
/************************************************************************************************/
	int AI_countNumImprovableBonuses( bool bIncludeNeutral, TechTypes eExtraTech = NO_TECH, bool bLand = true, bool bWater = false );
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

	int AI_playerCloseness(PlayerTypes eIndex, int iMaxDistance);
	int AI_cityThreat(TeamTypes eTargetTeam = NO_TEAM, int* piThreatModifier = NULL);
	
	int AI_getWorkersHave();
	int AI_getWorkersNeeded();
	void AI_changeWorkersHave(int iChange);
/********************************************************************************/
/* 	Worker Counting						03.08.2010				Fuyu			*/
/********************************************************************************/
	int AI_workingCityPlotTargetMissionAIs(PlayerTypes ePlayer, MissionAITypes eMissionAI, UnitAITypes eUnitAI = NO_UNITAI, bool bSameAreaOnly = false) const;
/********************************************************************************/
/* 	Worker Counting 											END 			*/
/********************************************************************************/
	BuildingTypes AI_bestAdvancedStartBuilding(int iPass);

	void read(FDataStreamBase* pStream);
	void write(FDataStreamBase* pStream);

	//Afforess - MP Resync
	void resync(bool bWrite, ByteBuffer* pBuffer);

/************************************************************************************************/
/* Afforess	                  Start		 01/26/10                                               */
/*                                                                                              */
/* Inquisitions                                                                                 */
/************************************************************************************************/
	bool AI_trainInquisitor();
/************************************************************************************************/
/* Inquisitions	                 END                                                            */
/************************************************************************************************/
/************************************************************************************************/
/* Afforess	                  Start		 10/31/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	int AI_getEmphasizeAvoidAngryCitizensCount();
	bool AI_isEmphasizeAvoidAngryCitizens();
	int AI_getEmphasizeAvoidUnhealthyCitizensCount();
	bool AI_isEmphasizeAvoidUnhealthyCitizens();
	bool AI_buildCaravan();
	int AI_getPromotionValue(PromotionTypes ePromotion) const;
	int AI_calculateActualImprovementHealth(ImprovementTypes eImprovement) const;
	int AI_getMilitaryProductionRateRank() const;
	int AI_getMilitaryProductionRate() const; // f1rpo
	bool AI_isMilitaryProductionCity() const;
	void AI_setMilitaryProductionCity(bool bNewVal);

	int AI_getDefensiveSpyValue() const;
	int AI_maxTheoriticalPopulation() const;
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
	int AI_getBuildPriority(void) const;

protected:
/************************************************************************************************/
/* Afforess	                  Start		 10/31/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	int m_iEmphasizeAvoidAngryCitizensCount;
	int m_iEmphasizeAvoidUnhealthyCitizensCount;
	bool m_bMilitaryProductionCity;
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/

	int m_iEmphasizeAvoidGrowthCount;
	int m_iEmphasizeGreatPeopleCount;

	bool m_bAssignWorkDirty;
	bool m_bChooseProductionDirty;

	mutable IDInfo m_routeToCity;
	mutable bool m_routeToCityUpdated;

	int* m_aiEmphasizeYieldCount;
	int* m_aiEmphasizeCommerceCount;
	bool m_bForceEmphasizeCulture;

	bool m_bestBuildValuesStale;
	int m_aiBestBuildValue[NUM_CITY_PLOTS];

	BuildTypes m_aeBestBuild[NUM_CITY_PLOTS];

	bool* m_pbEmphasize;
	bool* m_pbEmphasizeSpecialist;

	int* m_aiSpecialYieldMultiplier;

	int m_iCachePlayerClosenessTurn;
	int m_iCachePlayerClosenessDistance;
	int* m_aiPlayerCloseness;

	int m_iWorkersNeeded;
	int m_iWorkersHave;
	
	int	m_iBuildPriority;
	int	m_iTempBuildPriority;
	bool m_bRequestedUnit;
	bool m_bRequestedBuilding;

	void AI_doDraft(bool bForce = false);
	void AI_doHurry(bool bForce = false);
	void AI_doEmphasize();
	int AI_getHappyFromHurry(HurryTypes eHurry);
	int AI_getHappyFromHurry(HurryTypes eHurry, UnitTypes eUnit, bool bIgnoreNew);
	int AI_getHappyFromHurry(HurryTypes eHurry, BuildingTypes eBuilding, bool bIgnoreNew);
	int AI_getHappyFromHurry(int iHurryPopulation);
	bool AI_doPanic();
	int AI_calculateCulturePressure(bool bGreatWork = false);

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      01/09/10                                jdog5000      */
/*                                                                                              */
/* City AI                                                                                      */
/************************************************************************************************/
/********************************************************************************/
/* 	City Defenders						24.07.2010				Fuyu			*/
/********************************************************************************/
//Fuyu bIgnoreNotUnitAIs
	bool AI_chooseUnit(const char* reason, UnitAITypes eUnitAI = NO_UNITAI, int iOdds = -1, int iUnitStrength = -1, int iPriorityOverride = -1, CvUnitSelectionCriteria* criteria = NULL);
	bool AI_chooseUnitImmediate(const char* reason, UnitAITypes eUnitAI, CvUnitSelectionCriteria* criteria = NULL);
/********************************************************************************/
/* 	City Defenders												END 			*/
/********************************************************************************/
	bool AI_chooseUnit(UnitTypes eUnit, UnitAITypes eUnitAI);
	bool AI_chooseDefender(const char* reason);
	bool AI_chooseLeastRepresentedUnit(const char* reason, UnitTypeWeightArray &allowedTypes, int iOdds = -1);
	bool AI_chooseBuilding(int iFocusFlags = 0, int iMaxTurns = MAX_INT, int iMinThreshold = 0, int iOdds = -1, bool bMaximizePerTurnValue = false);
	bool AI_choosePropertyControlUnit(int iTriggerPercentOfPropertyOpRange);
	bool AI_chooseProject();
	bool AI_chooseProcess(CommerceTypes eCommerceType = NO_COMMERCE, int* commerceWeights = NULL);
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/
	int	getPlayerDangerPercentage(PlayerTypes ePlayer, int& iModifier) const;

	bool AI_bestSpreadUnit(bool bMissionary, bool bExecutive, int iBaseChance, UnitTypes* eBestSpreadUnit, int* iBestSpreadUnitValue);
	bool AI_addBestCitizen(bool bWorkers, bool bSpecialists, int* piBestPlot = NULL, SpecialistTypes* peBestSpecialist = NULL);
	bool AI_removeWorstCitizen(SpecialistTypes eIgnoreSpecialist = NO_SPECIALIST);
	void AI_juggleCitizens();

	bool AI_potentialPlot(short* piYields);
	bool AI_foodAvailable(int iExtra = 0);

public:
	int AI_yieldValue(short* piYields, short* piCommerceYields, bool bAvoidGrowth, bool bRemove, bool bIgnoreFood = false, bool bIgnoreGrowth = false, bool bIgnoreStarvation = false, bool bWorkerOptimization = false);
protected:
#ifdef YIELD_VALUE_CACHING
	virtual void AI_NoteWorkerChange(void);
	virtual void AI_NoteSpecialistChange(void);

	int AI_yieldValueWithCache(short* piYields, short* piCommerceYields, bool bAvoidGrowth, bool bRemove, bool bIgnoreFood, bool bIgnoreGrowth, bool bIgnoreStarvation, bool bWorkerOptimization, bool bSpecialist);
#endif
	int AI_yieldValueInternal(short* piYields, short* piCommerceYields, bool bAvoidGrowth, bool bRemove, bool bIgnoreFood = false, bool bIgnoreGrowth = false, bool bIgnoreStarvation = false, bool bWorkerOptimization = false);
	int AI_plotValue(CvPlot* pPlot, bool bAvoidGrowth, bool bRemove, bool bIgnoreFood = false, bool bIgnoreGrowth = false, bool bIgnoreStarvation = false);

	int AI_experienceWeight();
	int AI_buildUnitProb(/* f1rpo: */ bool bDraft = false);

	void AI_bestPlotBuild(CvPlot* pPlot, int* piBestValue, BuildTypes* peBestBuild, int iFoodPriority, int iProductionPriority, int iCommercePriority, bool bChop, int iHappyAdjust, int iHealthAdjust, int iFoodChange);
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      06/25/09                                jdog5000      */
/*                                                                                              */
/* Debug                                                                                        */
/************************************************************************************************/
	int AI_getImprovementValue( CvPlot* pPlot, ImprovementTypes eImprovement, int iFoodPriority, int iProductionPriority, int iCommercePriority, int iFoodChange, bool bOriginal = false );
	void AI_getYieldMultipliers( int &iFoodMultiplier, int &iProductionMultiplier, int &iCommerceMultiplier, int &iDesiredFoodChange );
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/
	int tradeRouteValue(CvBuildingInfo& kBuilding, YieldTypes eYield, bool bForeignTrade) const;


	void AI_buildGovernorChooseProduction();
	
	int AI_getYieldMagicValue(const int* piYieldsTimes100, bool bHealthy);
	int AI_getPlotMagicValue(CvPlot* pPlot, bool bHealthy, bool bWorkerOptimization = false);
	int AI_countGoodTiles(bool bHealthy, bool bUnworkedOnly, int iThreshold = 50, bool bWorkerOptimization = false);
	int AI_countGoodSpecialists(bool bHealthy);
	int AI_calculateTargetCulturePerTurn();
	
	void AI_stealPlots();
	
	int AI_buildingSpecialYieldChangeValue(BuildingTypes kBuilding, YieldTypes eYield);
	
	void AI_cachePlayerCloseness(int iMaxDistance);
	void AI_updateWorkersNeededHere();

	// added so under cheat mode we can call protected functions for testing
	friend class CvGameTextMgr;

public:
	//	KOSHLING MOD - pre-calculate building values for all buildings/focuses
	void AI_FlushBuildingValueCache(bool bRetainValues = false);
	int getPropertySourceValue(PropertyTypes eProperty, int iSourceValue) const;
	int getPropertyDecay(PropertyTypes eProperty) const;
	int getPropertyNonBuildingSource(PropertyTypes eProperty) const;
private:
	int	GetBuildingValue(BuildingTypes eType, int iFocusFlags, int threshold, bool bMaximizeFlaggedValue, bool bIgnoreCanConstruct = false);
	bool buildingMayHaveAnyValue(BuildingTypes eBuilding, int iFocusFlags);
	void CalculateAllBuildingValues(int iFocusFlags);
	int happynessValue(int iAddedHappyness, int iBaseHappinessLevel, int iBaseHealthLevel);
	int healthValue(int iAddedHealth, int iUseHappinessLevel, int iBaseHealthLevel, int iBaseFoodDifference);
	int worstWorkedPlotValue();
	//	Evaluate a building we are considering building here in terms of its
	//	effect on properties
	int buildingPropertiesValue(CvBuildingInfo& kBuilding) const;

	//	The cache itself
	BuildingValueCache* cachedBuildingValues;

#ifdef YIELD_VALUE_CACHING
private:
	virtual void ClearYieldValueCache(void);
	virtual void CheckYieldValueCache(char* label);

	yieldCache yieldValueCache;
	static int yieldValueCacheHits;
	static int yieldValueCacheReads;
#endif

	int m_iNumImprovableBonuses;	//	Cached befoer concurrent processing begins
	int m_neededDefenders;	//	Cached at the start of unit mission calculations each turn
	int m_neededDefendersVicinity;
	int m_requestedEscorts;

	int m_iCityThreat;
	int m_iCityThreatModifier;

	mutable CRITICAL_SECTION m_cBestUnitCacheSection;
	std::map<int, UnitValueInfo> m_bestUnits;

	std::map<int, int> m_buildValueCache;
};

#endif
