#pragma once

// playerAI.h

#ifndef CIV4_PLAYER_AI_H
#define CIV4_PLAYER_AI_H

#include <boost/shared_ptr.hpp>

#include "CvPlayer.h"
#include "AI_defines.h"
#include "ByteBuffer.h"

class CvEventTriggerInfo;

typedef struct
{
	int	iCount;		//	Num units targeting the mission target
	int iClosest;	//	How close is the closest
} MissionTargetInfo;

//	Koshling - add caching to plot danger calculations
#define PLOT_DANGER_CACHING
#ifdef PLOT_DANGER_CACHING
struct plotDangerCacheEntry
{
	plotDangerCacheEntry() { iLastUseCount = 0; }

	int plotX;
	int plotY;
	int iRange;
	bool bTestMoves;
	int iResult;
	int iLastUseCount;
};

#define PLOT_DANGER_CACHE_SIZE 24

class plotDangerCache
{
public:
	plotDangerCache()
	{
		currentUseCounter = -1;
		clear();
	}

	void clear(void)
	{
		if ( currentUseCounter != 0 )
		{
			currentUseCounter = 0;

			for(int i = 0; i < PLOT_DANGER_CACHE_SIZE; i++)
			{
				entries[i].iLastUseCount = 0;
			}
		}
	}

	struct plotDangerCacheEntry entries[PLOT_DANGER_CACHE_SIZE];
	int currentUseCounter;
};
#endif

class CvPlayerAI : public CvPlayer
{

public:

	CvPlayerAI();
	virtual ~CvPlayerAI();

  // inlined for performance reasons
#ifdef _USRDLL
  static CvPlayerAI& getPlayer(PlayerTypes ePlayer) 
  {
	  FAssertMsg(ePlayer != NO_PLAYER, "Player is not assigned a valid value");
	  FAssertMsg(ePlayer < MAX_PLAYERS, "Player is not assigned a valid value");
	  return m_aPlayers[ePlayer]; 
  }
#endif
	DllExport static CvPlayerAI& getPlayerNonInl(PlayerTypes ePlayer);

	static void initStatics();
	static void freeStatics();
	DllExport static bool areStaticsInitialized();

	void AI_init();
	void AI_uninit();
	void AI_reset(bool bConstructor);

	int AI_getFlavorValue(FlavorTypes eFlavor) const;

	void AI_doTurnPre();
	void AI_doTurnPost();
	void AI_doTurnUnitsPre();
	void AI_doTurnUnitsPost();

	void AI_doPeace();

	void AI_updateFoundValues(bool bClear = false, CvArea* area = NULL) const;
	void AI_updateAreaTargets();

	int AI_movementPriority(CvSelectionGroup* pGroup) const;
	void AI_unitUpdate();

	void AI_makeAssignWorkDirty();
	void AI_assignWorkingPlots();
	void AI_updateAssignWork();

	void AI_makeProductionDirty();

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      05/08/09                                jdog5000      */
/*                                                                                              */
/* City AI                                                                                      */
/************************************************************************************************/
	void AI_doCentralizedProduction();
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

	void AI_conquerCity(CvCity* pCity);

	bool AI_acceptUnit(CvUnit* pUnit) const;

	DomainTypes AI_unitAIDomainType(UnitAITypes eUnitAI) const;

	int AI_yieldWeight(YieldTypes eYield) const;
	int AI_commerceWeight(CommerceTypes eCommerce, CvCity* pCity = NULL) const;

	int AI_foundValue(int iX, int iY, int iMinRivalRange = -1, bool bStartingLoc = false) const;

	bool AI_isAreaAlone(CvArea* pArea) const;
	bool AI_isCapitalAreaAlone() const;
	bool AI_isPrimaryArea(CvArea* pArea) const;
	// <f1rpo>
	bool AI_feelsSafe() const;
	bool AI_isFocusWar(CvArea const* pArea = NULL) const;
	bool AI_isLandWar(CvArea const* kArea) const; // </f1rpo>
	int AI_militaryWeight(CvArea* pArea) const;

	int AI_targetCityValue(CvCity* pCity, bool bRandomize, bool bIgnoreAttackers = false) const;
	CvCity* AI_findTargetCity(CvArea* pArea) const;

	bool AI_isCommercePlot(CvPlot* pPlot) const;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      08/20/09                                jdog5000      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	bool AI_getAnyPlotDanger(CvPlot const* pPlot, int iRange = -1, bool bTestMoves = true) const;
	bool AI_getVisiblePlotDanger(CvPlot* pPlot, int iRange, bool bAnimalOnly, CvSelectionGroup* group = NULL, int acceptableOdds = -1) const;
	int AI_getPlotDanger(const CvPlot* pPlot, int iRange = -1, bool bTestMoves = true) const;
	int AI_getPlotDangerInternal(const CvPlot* pPlot, int iRange, bool bTestMoves) const;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/
	int AI_getWaterDanger(CvPlot* pPlot, int iRange, bool bTestMoves = true) const;
	int AI_countNumLocalNavy(CvPlot* pPlot, int iRange) const;

	bool AI_avoidScience() const;
	bool AI_isFinancialTrouble() const;
	int AI_costAsPercentIncome(int iExtraCost = 0,
		int iExpenseMod = 0, // Toffer
		int* piNetCommerce = NULL // f1rpo
	) const;
	int AI_safeCostAsPercentIncome() const;
	int AI_goldTarget() const;
	int AI_goldValueAssessmentModifier() const;

	TechTypes AI_bestTech(int iMaxPathLength = 1, bool bIgnoreCost = false, bool bAsync = false, TechTypes eIgnoreTech = NO_TECH, AdvisorTypes eIgnoreAdvisor = NO_ADVISOR) const;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      03/18/10                                jdog5000      */
/*                                                                                              */
/* Tech AI                                                                                      */
/************************************************************************************************/
	int AI_techValue( TechTypes eTech, int iPathLength, bool bIgnoreCost, bool bAsync, int* paiBonusClassRevealed, int* paiBonusClassUnrevealed, int* paiBonusClassHave) const;
	int AI_techBuildingValue( TechTypes eTech, int iPathLength, bool &bEnablesWonder ) const;
	int AI_techUnitValue( TechTypes eTech, int iPathLength, bool &bEnablesUnitWonder ) const;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/
	int  AI_TechValueCached(TechTypes eTech, bool bAsync, int* paiBonusClassRevealed, int* paiBonusClassUnrevealed, int* paiBonusClassHave, bool considerFollowOns = false) const;
	int AI_averageCurrentTechValue(TechTypes eRelativeTo, bool bAsync, int* paiBonusClassRevealed, int* paiBonusClassUnrevealed, int* paiBonusClassHave) const;
	void AI_chooseFreeTech();
	void AI_chooseResearch();
	void AI_startGoldenAge();

	DllExport DiploCommentTypes AI_getGreeting(PlayerTypes ePlayer) const;
	bool AI_isWillingToTalk(PlayerTypes ePlayer) const;
	bool AI_demandRebukedSneak(PlayerTypes ePlayer) const;
	bool AI_demandRebukedWar(PlayerTypes ePlayer) const;
	bool AI_hasTradedWithTeam(TeamTypes eTeam) const;

	AttitudeTypes AI_getAttitude(PlayerTypes ePlayer, bool bForced = true) const;
	int AI_getAttitudeVal(PlayerTypes ePlayer, bool bForced = true) const;
	static AttitudeTypes AI_getAttitudeFromValue(int iAttitudeVal);

	int AI_calculateStolenCityRadiusPlots(PlayerTypes ePlayer) const;
	int AI_getCloseBordersAttitude(PlayerTypes ePlayer) const;

	int AI_getWarAttitude(PlayerTypes ePlayer) const;
	int AI_getPeaceAttitude(PlayerTypes ePlayer) const;
	int AI_getSameReligionAttitude(PlayerTypes ePlayer) const;
	int AI_getDifferentReligionAttitude(PlayerTypes ePlayer) const;
	int AI_getBonusTradeAttitude(PlayerTypes ePlayer) const;
	int AI_getOpenBordersAttitude(PlayerTypes ePlayer) const;
	int AI_getDefensivePactAttitude(PlayerTypes ePlayer) const;
	int AI_getRivalDefensivePactAttitude(PlayerTypes ePlayer) const;
	int AI_getRivalVassalAttitude(PlayerTypes ePlayer) const;
	int AI_getShareWarAttitude(PlayerTypes ePlayer) const;
	int AI_getFavoriteCivicAttitude(PlayerTypes ePlayer) const;
	int AI_getTradeAttitude(PlayerTypes ePlayer) const;
	int AI_getRivalTradeAttitude(PlayerTypes ePlayer) const;
	int AI_getMemoryAttitude(PlayerTypes ePlayer, MemoryTypes eMemory) const;
	int AI_getColonyAttitude(PlayerTypes ePlayer) const;

    // BEGIN: Show Hidden Attitude Mod 01/22/2010
	int AI_getFirstImpressionAttitude(PlayerTypes ePlayer, /* f1rpo: */ bool bAsync) const;
	int AI_getPrejudiceAttitude(PlayerTypes ePlayer, bool bAsync) const; // f1rpo (Sexism, Racism)
	int AI_getTeamSizeAttitude(PlayerTypes ePlayer) const;
	int AI_getBetterRankDifferenceAttitude(PlayerTypes ePlayer) const;
	int AI_getWorseRankDifferenceAttitude(PlayerTypes ePlayer) const;
	int AI_getLowRankAttitude(PlayerTypes ePlayer) const;
	int AI_getLostWarAttitude(PlayerTypes ePlayer) const;
    int AI_getKnownPlayerRank(PlayerTypes ePlayer) const;
    // END: Show Hidden Attitude Mod

	PlayerVoteTypes AI_diploVote(const VoteSelectionSubData& kVoteData, VoteSourceTypes eVoteSource, bool bPropose);

	int AI_dealVal(PlayerTypes ePlayer, const CLinkList<TradeData>* pList, bool bIgnoreAnnual = false, int iExtra = 1) const;
	bool AI_goldDeal(const CLinkList<TradeData>* pList) const;
	bool AI_considerOffer(PlayerTypes ePlayer, const CLinkList<TradeData>* pTheirList, const CLinkList<TradeData>* pOurList, int iChange = 1) const;
	bool AI_counterPropose(PlayerTypes ePlayer, const CLinkList<TradeData>* pTheirList, const CLinkList<TradeData>* pOurList, CLinkList<TradeData>* pTheirInventory, CLinkList<TradeData>* pOurInventory, CLinkList<TradeData>* pTheirCounter, CLinkList<TradeData>* pOurCounter) const;

	DllExport int AI_maxGoldTrade(PlayerTypes ePlayer) const;

	DllExport int AI_maxGoldPerTurnTrade(PlayerTypes ePlayer) const;
	int AI_goldPerTurnTradeVal(int iGoldPerTurn) const;

	int AI_bonusVal(BonusTypes eBonus, int iChange = 1, bool bForTrade = false) const;
	int AI_baseBonusVal(BonusTypes eBonus, bool bForTrade = false) const;
	int AI_bonusTradeVal(BonusTypes eBonus, PlayerTypes ePlayer, int iChange) const;
	DenialTypes AI_bonusTrade(BonusTypes eBonus, PlayerTypes ePlayer) const;
	int AI_corporationBonusVal(BonusTypes eBonus) const;

	int AI_cityTradeVal(CvCity* pCity) const;
	DenialTypes AI_cityTrade(CvCity* pCity, PlayerTypes ePlayer) const;

	int AI_stopTradingTradeVal(TeamTypes eTradeTeam, PlayerTypes ePlayer) const;
	DenialTypes AI_stopTradingTrade(TeamTypes eTradeTeam, PlayerTypes ePlayer) const;

	int AI_civicTradeVal(CivicTypes eCivic, PlayerTypes ePlayer) const;
	DenialTypes AI_civicTrade(CivicTypes eCivic, PlayerTypes ePlayer) const;

	int AI_religionTradeVal(ReligionTypes eReligion, PlayerTypes ePlayer) const;
	DenialTypes AI_religionTrade(ReligionTypes eReligion, PlayerTypes ePlayer) const;

	int AI_unitImpassableCount(UnitTypes eUnit) const;
/********************************************************************************/
/* 	City Defenders						24.07.2010				Fuyu			*/
/********************************************************************************/
//Fuyu bIgnoreNotUnitAIs
	int AI_unitValue(UnitTypes eUnit, UnitAITypes eUnitAI, CvArea* pArea, CvUnitSelectionCriteria* criteria = NULL) const;
/********************************************************************************/
/* 	City Defenders												END 			*/
/********************************************************************************/
	int AI_unitPropertyValue(UnitTypes eUnit, PropertyTypes eProperty = NO_PROPERTY) const;
	int AI_totalUnitAIs(UnitAITypes eUnitAI) const;
	int AI_totalAreaUnitAIs(CvArea* pArea, UnitAITypes eUnitAI) const;
	int AI_totalWaterAreaUnitAIs(CvArea* pArea, UnitAITypes eUnitAI) const;
	int AI_countCargoSpace(UnitAITypes eUnitAI) const;

	int AI_neededExplorers(CvArea* pArea, bool bIdeal = false) const;
	int AI_neededHunters(CvArea* pArea, bool bIdeal = false) const;
	int AI_neededWorkers(CvArea* pArea) const;
	int AI_neededMissionaries(CvArea* pArea, ReligionTypes eReligion) const;
	int AI_neededExecutives(CvArea* pArea, CorporationTypes eCorporation) const;
	
	int AI_missionaryValue(CvArea* pArea, ReligionTypes eReligion, PlayerTypes* peBestPlayer = NULL) const;
	int AI_executiveValue(CvArea* pArea, CorporationTypes eCorporation, PlayerTypes* peBestPlayer = NULL) const;
	
	int AI_corporationValue(CorporationTypes eCorporation, CvCity* pCity = NULL) const;
	
	int AI_adjacentPotentialAttackers(CvPlot* pPlot, bool bTestCanMove = false) const;
	int AI_totalMissionAIs(MissionAITypes eMissionAI, CvSelectionGroup* pSkipSelectionGroup = NULL) const;
	int AI_areaMissionAIs(CvArea* pArea, MissionAITypes eMissionAI, CvSelectionGroup* pSkipSelectionGroup = NULL) const;
	int AI_plotTargetMissionAIsInternal(CvPlot* pPlot, MissionAITypes eMissionAI, int iRange = 0, int* piClosest = NULL, CvSelectionGroup* pSkipSelectionGroup = NULL) const; //Afforess: added CvSelectionGroup* pSkipSelectionGroup = NULL
	int AI_plotTargetMissionAIs(CvPlot* pPlot, MissionAITypes eMissionAI, CvSelectionGroup* pSkipSelectionGroup = NULL, int iRange = 0, int* piClosest = NULL) const;
	void AI_noteMissionAITargetCountChange(MissionAITypes eMissionAI, CvPlot* pPlot, int iChange, CvPlot* pUnitPlot);
	int AI_unitTargetMissionAIs(CvUnit* pUnit, MissionAITypes eMissionAI, CvSelectionGroup* pSkipSelectionGroup = NULL) const;
	int AI_unitTargetMissionAIs(CvUnit* pUnit, MissionAITypes* aeMissionAI, int iMissionAICount, CvSelectionGroup* pSkipSelectionGroup = NULL) const;
	int AI_enemyTargetMissionAIs(MissionAITypes eMissionAI, CvSelectionGroup* pSkipSelectionGroup = NULL) const;
	int AI_enemyTargetMissionAIs(MissionAITypes* aeMissionAI, int iMissionAICount, CvSelectionGroup* pSkipSelectionGroup = NULL) const;
	int AI_wakePlotTargetMissionAIs(CvPlot* pPlot, MissionAITypes eMissionAI, CvSelectionGroup* pSkipSelectionGroup = NULL) const;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      10/26/09                                jdog5000      */
/*                                                                                              */
/* General AI                                                                                   */
/************************************************************************************************/
	int AI_cityTargetUnitsByPath(CvCity* pCity, CvSelectionGroup* pSkipSelectionGroup, int iMaxPathTurns) const;
	int AI_enemyTargetMissions(TeamTypes eTargetTeam, CvSelectionGroup* pSkipSelectionGroup = NULL) const;
	int AI_enemyTargetMissionAIs(TeamTypes eTargetTeam, MissionAITypes* aeMissionAI, int iMissionAICount, CvSelectionGroup* pSkipSelectionGroup = NULL) const;
	int AI_unitTargetMissionAIs(CvUnit* pUnit, MissionAITypes* aeMissionAI, int iMissionAICount, CvSelectionGroup* pSkipSelectionGroup, int iMaxPathTurns) const;
	CivicTypes AI_bestCivic(CivicOptionTypes eCivicOption, int* iBestValue) const;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/
	CivicTypes AI_bestCivic(CivicOptionTypes eCivicOption, int* iBestValue, bool bCivicOptionVacuum = false, CivicTypes* paeSelectedCivics = NULL) const;
	CivicTypes AI_bestCivic(CivicOptionTypes eCivicOption) const;
	int AI_civicValue(CivicTypes eCivic, bool bCivicOptionVacuum = false, CivicTypes* paeSelectedCivics = NULL) const;
	//	Provide a measure of overall happyness (weighted appropriately by city)
	int AI_getOverallHappyness(int iExtraUnhappy = 0) const;

/************************************************************************************************/
/* REVOLUTION_MOD                         05/22/08                                jdog5000      */
/*                                                                                              */
/* Revolution AI                                                                                */
/************************************************************************************************/
	int AI_RevCalcCivicRelEffect(CivicTypes eCivic) const;
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/

	ReligionTypes AI_bestReligion() const;
	int AI_religionValue(ReligionTypes eReligion) const;
/************************************************************************************************/
/* REVOLUTION_MOD                         05/22/08                                jdog5000      */
/*                                                                                              */
/* Revolution AI                                                                                */
/************************************************************************************************/
	ReligionTypes AI_findHighestHasReligion();
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/

	EspionageMissionTypes AI_bestPlotEspionage(CvPlot* pSpyPlot, PlayerTypes& eTargetPlayer, CvPlot*& pPlot, int& iData) const;
	int AI_espionageVal(PlayerTypes eTargetPlayer, EspionageMissionTypes eMission, CvPlot* pPlot, int iData) const;

	int AI_getPeaceWeight() const;
	void AI_setPeaceWeight(int iNewValue);

	int AI_getEspionageWeight() const;
	void AI_setEspionageWeight(int iNewValue);

	int AI_getAttackOddsChange() const;
	void AI_setAttackOddsChange(int iNewValue);

	int AI_getCivicTimer() const;
	void AI_setCivicTimer(int iNewValue);
	void AI_changeCivicTimer(int iChange);

	int AI_getReligionTimer() const;
	void AI_setReligionTimer(int iNewValue);
	void AI_changeReligionTimer(int iChange);

	int AI_getExtraGoldTarget() const;
	void AI_setExtraGoldTarget(int iNewValue);

	int AI_getNumTrainAIUnits(UnitAITypes eIndex) const;
	void AI_changeNumTrainAIUnits(UnitAITypes eIndex, int iChange);

	int AI_getNumAIUnits(UnitAITypes eIndex) const;
	void AI_changeNumAIUnits(UnitAITypes eIndex, int iChange);

	int AI_getSameReligionCounter(PlayerTypes eIndex) const;
	void AI_changeSameReligionCounter(PlayerTypes eIndex, int iChange);

	int AI_getDifferentReligionCounter(PlayerTypes eIndex) const;
	void AI_changeDifferentReligionCounter(PlayerTypes eIndex, int iChange);

	int AI_getFavoriteCivicCounter(PlayerTypes eIndex) const;
	void AI_changeFavoriteCivicCounter(PlayerTypes eIndex, int iChange);

	int AI_getBonusTradeCounter(PlayerTypes eIndex) const;
	void AI_changeBonusTradeCounter(PlayerTypes eIndex, int iChange);

	int AI_getPeacetimeTradeValue(PlayerTypes eIndex) const;
	void AI_changePeacetimeTradeValue(PlayerTypes eIndex, int iChange);

	int AI_getPeacetimeGrantValue(PlayerTypes eIndex) const;
	void AI_changePeacetimeGrantValue(PlayerTypes eIndex, int iChange);

	int AI_getGoldTradedTo(PlayerTypes eIndex) const;
	void AI_changeGoldTradedTo(PlayerTypes eIndex, int iChange);

	int AI_getAttitudeExtra(PlayerTypes eIndex) const;
	void AI_setAttitudeExtra(PlayerTypes eIndex, int iNewValue);
	void AI_changeAttitudeExtra(PlayerTypes eIndex, int iChange);

	bool AI_isFirstContact(PlayerTypes eIndex) const;
	void AI_setFirstContact(PlayerTypes eIndex, bool bNewValue);

	int AI_getContactTimer(PlayerTypes eIndex1, ContactTypes eIndex2) const;
	void AI_changeContactTimer(PlayerTypes eIndex1, ContactTypes eIndex2, int iChange);

	int AI_getMemoryCount(PlayerTypes eIndex1, MemoryTypes eIndex2) const;
	void AI_changeMemoryCount(PlayerTypes eIndex1, MemoryTypes eIndex2, int iChange);

	int AI_calculateGoldenAgeValue() const;

	void AI_doCommerce();

	EventTypes AI_chooseEvent(int iTriggeredId, int* pValue = NULL) const;
	virtual void AI_launch(VictoryTypes eVictory);

	int AI_getCultureVictoryStage() const;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      03/17/10                                jdog5000      */
/*                                                                                              */
/* Victory Strategy AI                                                                          */
/************************************************************************************************/
	int AI_getSpaceVictoryStage() const;
	int AI_getConquestVictoryStage() const;
	int AI_getDominationVictoryStage() const;
	int AI_getDiplomacyVictoryStage() const;
	bool AI_isDoVictoryStrategy(int iVictoryStrategy) const;
	bool AI_isDoVictoryStrategyLevel4() const;
	bool AI_isDoVictoryStrategyLevel3() const;
	void AI_forceUpdateVictoryStrategies();
	int AI_getVictoryStrategyHash() const;
	int AI_getStrategyRand(int iShift) const;
	void AI_calculateStrategyRand();
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/		
	
	int AI_cultureVictoryTechValue(TechTypes eTech) const;
	
	bool AI_isDoStrategy(int iStrategy) const;
	void AI_forceUpdateStrategies();

	void AI_nowHasTech(TechTypes eTech);
	
    int AI_countDeadlockedBonuses(CvPlot* pPlot) const;
    
    int AI_getOurPlotStrength(CvPlot const* pPlot, int iRange, bool bDefensiveBonuses, bool bTestMoves,
			int* piUnitCount = NULL) const; // f1rpo
    int AI_getEnemyPlotStrength(CvPlot const* pPlot, int iRange, bool bDefensiveBonuses, bool bTestMoves) const;

	int AI_goldToUpgradeAllUnits(int iExpThreshold = 0) const;

	int AI_goldTradeValuePercent() const;
	
	int AI_averageYieldMultiplier(YieldTypes eYield) const;
	int AI_averageCommerceMultiplier(CommerceTypes eCommerce) const;
	int AI_averageGreatPeopleMultiplier() const;
	int AI_averageCommerceExchange(CommerceTypes eCommerce) const;
	
	int AI_playerCloseness(PlayerTypes eIndex, int iMaxDistance) const;
	
	int AI_getTotalCityThreat() const;
	int AI_getTotalFloatingDefenseNeeded() const;
	
	
	int AI_getTotalAreaCityThreat(CvArea* pArea, int* piLargestThreat = NULL) const;
	int AI_countNumAreaHostileUnits(CvArea* pArea, bool bPlayer, bool bTeam, bool bNeutral, bool bHostile, CvPlot* pPlot, int iMaxDistance) const;
	int AI_getTotalFloatingDefendersNeeded(CvArea* pArea) const;
	int AI_getTotalFloatingDefenders(CvArea* pArea) const;

	RouteTypes AI_bestAdvancedStartRoute(CvPlot* pPlot, int* piYieldValue = NULL) const;
	UnitTypes AI_bestAdvancedStartUnitAI(CvPlot* pPlot, UnitAITypes eUnitAI) const;
	CvPlot* AI_advancedStartFindCapitalPlot() const;
	
	bool AI_advancedStartPlaceExploreUnits(bool bLand);
	void AI_advancedStartRevealRadius(CvPlot* pPlot, int iRadius);
	bool AI_advancedStartPlaceCity(CvPlot* pPlot);
	bool AI_advancedStartDoRoute(CvPlot* pFromPlot, CvPlot* pToPlot);
	void AI_advancedStartRouteTerritory();
	void AI_doAdvancedStart(bool bNoExit = false);
	
	int AI_getMinFoundValue() const;
	
	void AI_recalculateFoundValues(int iX, int iY, int iInnerRadius, int iOuterRadius) const;
	
	int AI_bestCityValue(CvPlot* pPlot, int iFoundValue) const;
	void AI_updateCitySites(int iMinFoundValueThreshold, int iMaxSites) const;
	void calculateCitySites(void) const;

	bool AI_isPlotCitySite(CvPlot* pPlot) const;
	int AI_getNumAreaCitySites(int iAreaID, int& iBestValue) const;
	int AI_getNumAdjacentAreaCitySites(int iWaterAreaID, int iExcludeArea, int& iBestValue) const;
	
	int AI_getNumCitySites() const;
	CvPlot* AI_getCitySite(int iIndex) const;
	
	int AI_bestAreaUnitAIValue(UnitAITypes eUnitAI, CvArea* pArea, UnitTypes* peBestUnitType = NULL) const;
	int AI_bestCityUnitAIValue(UnitAITypes eUnitAI, CvCity* pCity, UnitTypes* peBestUnitType = NULL) const;
	
	int AI_calculateTotalBombard(DomainTypes eDomain) const;
	
	int AI_getUnitClassWeight(UnitClassTypes eUnitClass) const;
	int AI_getUnitCombatWeight(UnitCombatTypes eUnitCombat) const;
	int AI_calculateUnitAIViability(UnitAITypes eUnitAI, DomainTypes eDomain) const;
	
	void AI_updateBonusValue();
	void AI_updateBonusValue(BonusTypes eBonus);
	
	int AI_getAttitudeWeight(PlayerTypes ePlayer) const;

	ReligionTypes AI_chooseReligion();
	
	int AI_getPlotAirbaseValue(CvPlot* pPlot) const;
	int AI_getPlotCanalValue(CvPlot* pPlot) const;

	bool AI_isPlotThreatened(CvPlot* pPlot, int iRange = -1, bool bTestMoves = true) const;

	bool AI_isFirstTech(TechTypes eTech) const;

/************************************************************************************************/
/* RevDCM	                  Start		 12/9/09                                                */
/*                                                                                              */
/* Inquisitions                                                                                 */
/************************************************************************************************/
	CvCity* getInquisitionRevoltCity(CvUnit *pUnit, bool bNoUnit, int iRevIndexThreshold, int iTrendThreshold);
	CvCity* getTeamInquisitionRevoltCity(CvUnit *pUnit, bool bNoUnit, int iRevIndexThreshold, int iTrendThreshold);
	CvCity* getReligiousVictoryTarget(CvUnit *pUnit, bool bNoUnit);

	bool isPushReligiousVictory() const;
	void AI_setPushReligiousVictory();

	bool isConsiderReligiousVictory() const;
	void AI_setConsiderReligiousVictory();

	bool hasInquisitionTarget() const;
	void AI_setHasInquisitionTarget();

	int countCityReligionRevolts() const;
/************************************************************************************************/
/* RevDCM	                     END                                                            */
/************************************************************************************************/

/************************************************************************************************/
/* Afforess	                  Start		 12/9/09                                                */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	int AI_getWeakestRelationsAttitudeChange(PlayerTypes ePlayer) const;
	int AI_getPowerfulRelationsAttitudeChange(PlayerTypes ePlayer) const;

	int AI_getLessPowerfulRelationsAttitudeChange(PlayerTypes ePlayer) const;
	int AI_getCivicRelationsAttitudeChange() const;

    int AI_workerTradeVal(CvUnit* pUnit) const;
	int AI_militaryUnitTradeVal(CvUnit* pUnit) const;
	int AI_corporationTradeVal(CorporationTypes eCorporation, PlayerTypes ePlayer) const;
    int AI_pledgeVoteTradeVal(VoteTriggeredData* kData, PlayerVoteTypes ePlayerVote, PlayerTypes ePlayer) const;
	int AI_secretaryGeneralTradeVal(VoteSourceTypes eVoteSource, PlayerTypes ePlayer) const;
	
	int AI_getEmbassyAttitude(PlayerTypes ePlayer) const;
	TeamTypes AI_bestJoinWarTeam(PlayerTypes ePlayer);
	TeamTypes AI_bestMakePeaceTeam(PlayerTypes ePlayer);
	TeamTypes AI_bestStopTradeTeam(PlayerTypes ePlayer);
	int AI_militaryBonusVal(BonusTypes eBonus);
    int AI_getCivicShareAttitude(PlayerTypes ePlayer) const;
	int AI_getCivicAttitudeChange(PlayerTypes ePlayer) const;
	bool AI_avoidIncreasingTaxes() const;
	int AI_promotionValue(PromotionTypes ePromotion, UnitTypes eUnit, const CvUnit* pUnit = NULL, UnitAITypes eUnitAI = NO_UNITAI) const;
	TechTypes AI_bestReligiousTech(int iMaxPathLength, TechTypes eIgnoreTech, AdvisorTypes eIgnoreAdvisor) const;
	int AI_religiousTechValue(TechTypes eTech) const;
	int AI_ourCityValue(CvCity* pCity) const;
	// <f1rpo>
	int AI_nukePlotValue(CvPlot const& kPlot, int iCivilianTargetWeight) const;
	int AI_nukeBaseDestructionWeight() const;
	int AI_nukeExtraDestructionWeight(PlayerTypes eTarget,
			int iTheirNukes, bool bLimited) const;
	int AI_estimateNukeCount(PlayerTypes eOwner) const;
	scaled AI_nukeChanceToKillUnit(int iHP, int iNukeModifier = 0) const;
	// </f1rpo>

	void AI_noteWarStatusChange(TeamTypes eTeam, bool bAtWar);
	
	void AI_doMilitaryProductionCity();
	int AI_getMilitaryProductionCityCount() const;

	CvCity* findBestCoastalCity(void) const;
	UnitTypes bestBuildableUnitForAIType(DomainTypes eDomain, UnitAITypes eUnitAIType, CvUnitSelectionCriteria* criteria = NULL) const;
	int strengthOfBestUnitAI(DomainTypes eDomain, UnitAITypes eUnitAIType) const;

	int AI_tradeWarReparationsVal(PlayerTypes ePlayer) const;
	int AI_tradeCeaseRelationsVal(PlayerTypes ePlayer, PlayerTypes eRequestFrom) const;
	
	mutable int m_iMilitaryProductionCityCount;
	mutable int* m_aiCivicValueCache;

	int AI_getNumNeededSpies(CvArea* pArea, bool bDefensiveOnly) const;
	void AI_updateEspionagePlan();
	EspionagePlans AI_getEspionagePlan();

	void AI_invalidateClaimedTerritory();
	int AI_getClaimedTerritoryCount() const;
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      09/03/09                       poyuzhe & jdog5000     */
/*                                                                                              */
/* Efficiency                                                                                   */
/************************************************************************************************/
	// From Sanguo Mod Performance, ie the CAR Mod
	// Attitude cache
	void AI_invalidateAttitudeCache(PlayerTypes ePlayer);
	void AI_invalidateAttitudeCache();
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/
	int	AI_getNumBuildingsNeeded(BuildingTypes eBuilding, bool bCoastal) const;
	void AI_changeNumBuildingsNeeded(BuildingTypes eBuilding, int iChange);

	// for serialization
	virtual void read(FDataStreamBase* pStream);
	virtual void write(FDataStreamBase* pStream);

	//Afforess - MP Resync
	void resync(bool bWrite, ByteBuffer* pBuffer);

	void AI_noteUnitRecalcNeeded(void);
	void AI_recalculateUnitCounts(void);
	int AI_calculateAverageLocalInstability(void) const;
	int AI_calculateAverageCityDistance(void) const;

protected:

	static CvPlayerAI* m_aPlayers;

	int m_iPeaceWeight;
	int m_iEspionageWeight;
	int m_iAttackOddsChange;
	int m_iCivicTimer;
	int m_iReligionTimer;
	int m_iExtraGoldTarget;

	//Afforess: Espionage AI
	EspionagePlans m_eEspionagePlan;

	mutable int m_iClaimedTerritory;
	
	mutable int m_iStrategyHash;
	mutable int m_iStrategyHashCacheTurn;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      03/18/10                                jdog5000      */
/*                                                                                              */
/* Victory Strategy AI                                                                          */
/************************************************************************************************/
	mutable int m_iStrategyRand;
	mutable int m_iVictoryStrategyHash;
	mutable int m_iVictoryStrategyHashCacheTurn;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/		
/************************************************************************************************/
/* RevDCM	                  Start		 12/9/09                                                */
/*                                                                                              */
/* Inquisitions                                                                                 */
/************************************************************************************************/
	bool m_bPushReligiousVictory;
	bool m_bConsiderReligiousVictory;
	bool m_bHasInquisitionTarget;
/************************************************************************************************/
/* RevDCM	                     END                                                            */
/************************************************************************************************/
	mutable TechTypes m_eBestResearchTarget;	//	Koshling - retain beeling terget to minmimize need for recalculation
	
	mutable volatile int m_iAveragesCacheTurn;
	
	mutable int m_iAverageGreatPeopleMultiplier;
	
	mutable int *m_aiAverageYieldMultiplier;
	mutable int *m_aiAverageCommerceMultiplier;
	mutable int *m_aiAverageCommerceExchange;
	
	mutable int m_iUpgradeUnitsCacheTurn;
	mutable int m_iUpgradeUnitsCachedExpThreshold;
	mutable int m_iUpgradeUnitsCachedGold;

	int *m_aiNumTrainAIUnits;
	int *m_aiNumAIUnits;
	int* m_aiSameReligionCounter;
	int* m_aiDifferentReligionCounter;
	int* m_aiFavoriteCivicCounter;
	int* m_aiBonusTradeCounter;
	int* m_aiPeacetimeTradeValue;
	int* m_aiPeacetimeGrantValue;
	int* m_aiGoldTradedTo;
	int* m_aiAttitudeExtra;
	int* m_aiBonusValue;
	int* m_aiTradeBonusValue;
	bool* m_abNonTradeBonusCalculated;
	int* m_aiUnitClassWeights;
	int* m_aiUnitCombatWeights;
	
	mutable CRITICAL_SECTION m_csBonusValSection;

	mutable int* m_aiCloseBordersAttitudeCache;

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      09/03/09                       poyuzhe & jdog5000     */
/*                                                                                              */
/* Efficiency                                                                                   */
/************************************************************************************************/
	// From Sanguo Mod Performance, ie the CAR Mod
	// Attitude cache
	mutable int* m_aiAttitudeCache;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

	bool* m_abFirstContact;

	int** m_aaiContactTimer;
	int** m_aaiMemoryCount;
	
	mutable std::vector<int> m_aiAICitySites;
	mutable bool m_bCitySitesNotCalculated;
	
	bool m_bWasFinancialTrouble;
	int m_iTurnLastProductionDirty;

	void AI_doCounter();
	void AI_doMilitary();
	void AI_doResearch();
public:
	void AI_doCivics();
protected:
	void AI_doReligion();
	void AI_doDiplo();
	// f1rpo:
	int AI_chooseReparations(PlayerTypes eEnemy, int iTargetValue, TechTypes& eBestTech, int& iBestGold, CvCity*& pBestCity);
/************************************************************************************************/
/* REVOLUTIONDCM_MOD                         02/04/08                            Glider1        */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	// RevolutionDCM - new diplomacy option
	void AI_beginDiplomacy(CvDiploParameters* pDiploParams, PlayerTypes ePlayer, DiplomacyRequestType eRequest = NO_REQUEST);
	// RevolutionDCM - end
/************************************************************************************************/
/* REVOLUTIONDCM_MOD                         02/04/08                            Glider1        */
/************************************************************************************************/
	
	void AI_doSplit();
	void AI_doCheckFinancialTrouble();
	
	bool AI_disbandUnit(int iExpThreshold, bool bObsolete);
	
	int AI_getStrategyHash() const;
	void AI_calculateAverages() const;

	bool AI_isThreatFromMinorCiv() const; // f1rpo
/********************************************************************************/
/* 	New Civic AI						02.08.2010				Fuyu			*/
/********************************************************************************/
	bool AI_isCivicCanChangeOtherValues(CivicTypes eCivicSelected, ReligionTypes eAssumedReligion = NO_RELIGION);
	bool AI_isCivicValueRecalculationRequired(CivicTypes eCivic, CivicTypes eCivicSelected, ReligionTypes eAssumedReligion = NO_RELIGION);
/********************************************************************************/
/* 	New Civic AI												END 			*/
/********************************************************************************/

	int AI_getHappinessWeight(int iHappy, int iExtraPop) const;
	int AI_getHealthWeight(int iHealth, int iExtraPop) const;
	
	void AI_convertUnitAITypesForCrush();
	int AI_eventValue(EventTypes eEvent, const EventTriggeredData& kTriggeredData) const;
		
	void AI_doEnemyUnitData();
	void AI_invalidateCloseBordersAttitudeCache();
	
	friend class CvGameTextMgr;

#ifdef PLOT_DANGER_CACHING
public:
	static void ClearPlotDangerCache(void)
	{
		plotDangerCache.clear();
	}
private:
	static plotDangerCache plotDangerCache;
	static int plotDangerCacheHits;
	static int plotDangerCacheReads;
#endif

	techPath*	findBestPath(TechTypes eTech, int& valuePerUnitCost, bool bIgnoreCost, bool bAsync, int* paiBonusClassRevealed, int* paiBonusClassUnrevealed, int* paiBonusClassHave) const;
	int	 techPathValuePerUnitCost(techPath* path, TechTypes eTech, bool bIgnoreCost, bool bAsync, int* paiBonusClassRevealed, int* paiBonusClassUnrevealed, int* paiBonusClassHave) const;
	TechTypes findStartTech(techPath* path) const;

	mutable std::map<TechTypes,int>	m_cachedTechValues;
	mutable std::map<BuildingTypes, int>	m_numBuildingsNeeded;

	int m_iCityGrowthValueBase;
	int m_turnsSinceLastRevolution;
	int m_iCivicSwitchMinDeltaThreshold;
	bool bUnitRecalcNeeded;

	mutable std::map<MissionAITypes,boost::shared_ptr<std::map<CvPlot*,MissionTargetInfo> > > m_missionTargetCache;
};

// helper for accessing static functions
#ifdef _USRDLL
#define GET_PLAYER CvPlayerAI::getPlayer
#else
#define GET_PLAYER CvPlayerAI::getPlayerNonInl
#endif

#endif
