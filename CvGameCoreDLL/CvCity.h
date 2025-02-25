#pragma once

// city.h

#ifndef CIV4_CITY_H
#define CIV4_CITY_H

#include "CvDLLEntity.h"
#include "LinkedList.h"
#include "CvInfos.h"
#include "CvBuildingList.h"
#include "CvUnitList.h"
#include "CvGameObject.h"
#include "CvGame.h"
#include "CvGameAI.h"
#include "CvPipeline.h"
#include "ByteBuffer.h"

class CvPlot;
class CvPlotGroup;
class CvArea;
class CvGenericBuilding;
class CvArtInfoBuilding;
class CvCityTurnPipelineWorkItem;
class CvCityAI; // f1rpo

//	KOSHLING - Cache yield values where possible
#define YIELD_VALUE_CACHING

//	Enable canTrain results to be cached within a (caller)defined scope
#define CAN_TRAIN_CACHING

// BUG - start
void addGoodOrBad(int iValue, int& iGood, int& iBad);
void subtractGoodOrBad(int iValue, int& iGood, int& iBad);
// BUG - end

//	Unit ordering criteria
class CvUnitSelectionCriteria
{
public:
	CvUnitSelectionCriteria() : m_eUnitAI(NO_UNITAI),
								m_eIgnoreAdvisor(NO_ADVISOR),
								m_bIgnoreNotUnitAIs(false),
								m_bIgnoreGrowth(false),
								m_eProperty(NO_PROPERTY),
								m_bPropertyBeneficial(true),
								m_bIsHealer(false),
								m_bIsCommander(false)
	{
	}

	CvString	getDescription()
	{
		if ( m_eProperty != NO_PROPERTY )
		{
			return CvString::format("Property %s for %S",
									m_bPropertyBeneficial ? "improver" : "worsener",
									GC.getPropertyInfo(m_eProperty).getDescription());
		}
		else
		{
			return CvString("");
		}
	}

#define PROPERTY_MASK 0x0F
#define ADVISOR_MASK 0x07

	int getHash() const
	{
		int	iResult = (m_eUnitAI << 16);

		if ( m_eProperty != NO_PROPERTY )
		{
			iResult |= 0x8000;
			iResult |= (m_eProperty & PROPERTY_MASK) << 11;
			if ( !m_bPropertyBeneficial )
			{
				iResult |= 0x400;
			}
		}
		if ( m_bIgnoreNotUnitAIs )
		{
			iResult |= 0x200;
		}
		if ( m_bIgnoreGrowth )
		{
			iResult |= 0x100;
		}
		iResult |= (m_eIgnoreAdvisor & ADVISOR_MASK) << 5;
		if ( m_bIsHealer )
		{
			iResult |= 0x10;
		}
		if ( m_bIsCommander )
		{
			iResult |= 0x8;
		}

		return iResult;
	}

	UnitAITypes		m_eUnitAI;
	AdvisorTypes	m_eIgnoreAdvisor;
	bool			m_bIgnoreNotUnitAIs;
	bool			m_bIgnoreGrowth;
	PropertyTypes	m_eProperty;
	bool			m_bPropertyBeneficial;
	bool			m_bIsHealer;
	bool			m_bIsCommander;
};

class CvCity : public CvDLLEntity
{
friend CvCityTurnPipelineWorkItem;
public:
	CvCity();
	virtual ~CvCity();

	void* operator new(size_t size) { return GC.getGameINLINE().getAllocator().allocate(size); }
	void operator delete(void* instance) { GC.getGameINLINE().getAllocator().deAllocate(instance); }
	void* operator new[](size_t size) { return GC.getGameINLINE().getAllocator().allocate(size); }
	void operator delete[](void* instance) { GC.getGameINLINE().getAllocator().deAllocate(instance); }

	void init(int iID, PlayerTypes eOwner, int iX, int iY, bool bBumpUnits, bool bUpdatePlotGroups);
	void uninit();
	void reset(int iID = 0, PlayerTypes eOwner = NO_PLAYER, int iX = 0, int iY = 0, bool bConstructorCall = false);
	void setupGraphical();

	CvGameObjectCity* getGameObject()  {return &m_GameObject;};
	const CvGameObjectCity* getGameObjectConst() const {return (const CvGameObjectCity*)&m_GameObject;};

protected:
	CvGameObjectCity m_GameObject;

public:
/************************************************************************************************/
/* REVOLUTION_MOD                         03/29/09                                jdog5000      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	int getRevolutionIndex() const;
	void setRevolutionIndex( int iNewValue );
	void changeRevolutionIndex( int iChange );

	int getLocalRevIndex() const;
	void setLocalRevIndex( int iNewValue );
	void changeLocalRevIndex( int iChange );

	int getRevIndexAverage() const;
	void setRevIndexAverage( int iNewValue );
	void updateRevIndexAverage( );

	int getRevolutionCounter() const;
	void setRevolutionCounter( int iNewValue );
	void changeRevolutionCounter( int iChange );

	int getReinforcementCounter() const;
	void setReinforcementCounter( int iNewValue );
	void changeReinforcementCounter( int iChange );

	int getRevIndexHappinessVal();
	int getRevIndexDistanceVal();
	int getRevIndexColonyVal();
	int getRevIndexReligionVal();
	int getRevIndexNationalityVal();
	int getRevIndexHealthVal();
	int getRevIndexGarrisonVal();
	int getRevIndexDisorderVal();

	bool isRecentlyAcquired();
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/


	void kill(bool bUpdatePlotGroups, bool bUpdateCulture = true);																								// Exposed to Python
	void killTestCheap(); // For testing, do not call in a game situation

	//void doTurn();
	void doTurnBeginProcessing();
	void doTurnMiscAsync();
	void doTurnPreProduction();
	void doTurnEnactCurrentProduction(CvCityTurnPipelineWorkItem* item);
	void doTurnChooseProduction(CvCityTurnPipelineWorkItem* item);
	void doTurnEnactNewProduction(CvCityTurnPipelineWorkItem* item);
	void doPrepareToCompleteProduction();
	void doTurnCompleteProduction();
	void doTurnPostProduction();
	void completeOrderProcessing(const OrderData* pOrderData);
	void completeOrderProcessing(void);
	void doAutobuild(void);

	bool isCitySelected() const;
	DllExport bool canBeSelected() const;
	DllExport void updateSelectedCity(bool bTestProduction);

	void updateYield();

	void updateVisibility();

	void createGreatPeople(UnitTypes eGreatPersonUnit, bool bIncrementThreshold, bool bIncrementExperience);		// Exposed to Python

	void doTask(TaskTypes eTask, int iData1 = -1, int iData2 = -1, bool bOption = false, bool bAlt = false, bool bShift = false, bool bCtrl = false);		// Exposed to Python

	void chooseProduction(UnitTypes eTrainUnit = NO_UNIT, BuildingTypes eConstructBuilding = NO_BUILDING, ProjectTypes eCreateProject = NO_PROJECT, bool bFinish = false, bool bFront = false);		// Exposed to Python

	int getCityPlotIndex(const CvPlot* pPlot) const;				// Exposed to Python 
	CvPlot* getCityIndexPlot(int iIndex) const;															// Exposed to Python

	bool canWork(CvPlot* pPlot) const;																			// Exposed to Python
	void verifyWorkingPlot(int iIndex);
	void verifyWorkingPlots();
	void clearWorkingOverride(int iIndex);														// Exposed to Python
	int countNumImprovedPlots(ImprovementTypes eImprovement = NO_IMPROVEMENT, bool bPotential = false) const;																			// Exposed to Python
	int countNumWaterPlots() const;																					// Exposed to Python
	int countNumRiverPlots() const;																					// Exposed to Python

	int findPopulationRank() const;																					// Exposed to Python
	int findBaseYieldRateRank(YieldTypes eYield) const;											// Exposed to Python
	int findYieldRateRank(YieldTypes eYield) const;								// Exposed to Python					
	int findCommerceRateRank(CommerceTypes eCommerce) const;			// Exposed to Python

/************************************************************************************************/
/* REVDCM                                 05/05/10                                phungus420    */
/*                                                                                              */
/* CanTrain                                                                                     */
/************************************************************************************************/
	bool isForceObsoleteUnitClassAvailable(UnitTypes eUnit) const;						// Exposed to Python
	bool isPlotTrainable(UnitTypes eUnit, bool bContinue, bool bTestVisible) const;						// Exposed to Python
/************************************************************************************************/
/* REVDCM                                  END                                                  */
/************************************************************************************************/

	UnitTypes allUpgradesAvailable(UnitTypes eUnit, int iUpgradeCount = 0) const;						// Exposed to Python
	bool isWorldWondersMaxed() const;																							// Exposed to Python
	bool isTeamWondersMaxed() const;																							// Exposed to Python
	bool isNationalWondersMaxed() const;																					// Exposed to Python
	bool isBuildingsMaxed() const;																								// Exposed to Python
	int getMaxNumWorldWonders() const;																							// Exposed to Python
	int getMaxNumTeamWonders() const;																							// Exposed to Python
	int getMaxNumNationalWonders() const;																					// Exposed to Python

	bool canTrain(UnitTypes eUnit, bool bContinue = false, bool bTestVisible = false, bool bIgnoreCost = false, bool bIgnoreUpgrades = false) const;					// Exposed to Python 
	bool canTrainInternal(UnitTypes eUnit, bool bContinue = false, bool bTestVisible = false, bool bIgnoreCost = false, bool bIgnoreUpgrades = false) const;					// Exposed to Python 
	bool canTrain(UnitCombatTypes eUnitCombat) const;
	bool canTrainInternal(UnitCombatTypes eUnitCombat) const;
/************************************************************************************************/
/* Afforess	                  Start		 05/20/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	bool canConstruct(BuildingTypes eBuilding, bool bContinue = false, bool bTestVisible = false, bool bIgnoreCost = false, bool bIgnoreAmount = false, bool bIgnoreBuildings = false, TechTypes eIgnoreTechReq = NO_TECH, int* probabilityEverConstructable = NULL) const;	// Exposed to Python  
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
	//	KOSHLING - cache can build results
	void FlushCanConstructCache(BuildingTypes eBuilding = NO_BUILDING);
	bool canConstructInternal(BuildingTypes eBuilding, bool bContinue, bool bTestVisible, bool bIgnoreCost, bool bIgnoreAmount, BuildingClassTypes withExtraBuildingClass = NO_BUILDINGCLASS, bool bIgnoreBuildings = false, TechTypes eIgnoreTechReq = NO_TECH, int* probabilityEverConstructable = NULL) const;
	void NoteBuildingNoLongerConstructable(BuildingTypes eBuilding) const;

	bool canCreate(ProjectTypes eProject, bool bContinue = false, bool bTestVisible = false) const;		// Exposed to Python 
	bool canMaintain(ProcessTypes eProcess, bool bContinue = false) const;														// Exposed to Python  
	bool canJoin() const;																													// Exposed to Python

	int getFoodTurnsLeft() const;																				// Exposed to Python
	bool isProduction() const;																					// Exposed to Python
	bool isProductionLimited() const;																							// Exposed to Python
	bool isProductionUnit() const;																								// Exposed to Python
	bool isProductionBuilding() const;																						// Exposed to Python
	bool isProductionProject() const;																							// Exposed to Python
	bool isProductionProcess() const;																		// Exposed to Python

	bool canContinueProduction(OrderData order);														// Exposed to Python
	int getProductionExperience(UnitTypes eUnit = NO_UNIT);									// Exposed to Python
	void addProductionExperience(CvUnit* pUnit, bool bConscript = false);		// Exposed to Python

	UnitTypes getProductionUnit() const;																// Exposed to Python
	UnitAITypes getProductionUnitAI() const;																			// Exposed to Python
	BuildingTypes getProductionBuilding() const;												// Exposed to Python
	ProjectTypes getProductionProject() const;													// Exposed to Python
	ProcessTypes getProductionProcess() const;													// Exposed to Python
	const wchar* getProductionName() const;															// Exposed to Python
	const wchar* getProductionNameKey() const;													// Exposed to Python
	int getGeneralProductionTurnsLeft() const;										// Exposed to Python

	bool isFoodProduction() const;																								// Exposed to Python
	bool isFoodProduction(UnitTypes eUnit) const;																	// Exposed to Python
	int getFirstUnitOrder(UnitTypes eUnit) const;																	// Exposed to Python
	int getFirstBuildingOrder(BuildingTypes eBuilding) const;											// Exposed to Python
	int getFirstProjectOrder(ProjectTypes eProject) const;												// Exposed to Python
	int getNumTrainUnitAI(UnitAITypes eUnitAI) const;															// Exposed to Python

	int getProduction() const;																						// Exposed to Python
	int getProductionNeeded() const;																						// Exposed to Python
	int getProductionNeeded(UnitTypes eUnit) const;
	int getProductionNeeded(BuildingTypes eBuilding) const;
	int getProductionNeeded(ProjectTypes eProject) const;		
	int getQueueNodeProductionTurnsLeft(CLLNode<OrderData>* pOrderNode, int iIndex = 0) const;
	int getTotalProductionQueueTurnsLeft(void) const;
	int getProductionTurnsLeft() const;																	// Exposed to Python 
	int getProductionTurnsLeft(UnitTypes eUnit, int iNum) const;					// Exposed to Python
	int getProductionTurnsLeft(BuildingTypes eBuilding, int iNum) const;	// Exposed to Python
	int getProductionTurnsLeft(ProjectTypes eProject, int iNum) const;		// Exposed to Python
	int getProductionTurnsLeft(int iProductionNeeded, int iProduction, int iFirstProductionDifference, int iProductionDifference) const;
	void setProduction(int iNewValue);																			// Exposed to Python
	void changeProduction(int iChange);																			// Exposed to Python
	int numQueuedUnits(UnitAITypes eUnitAI, CvPlot* pDestPlot);

	int getProductionModifier() const;																						// Exposed to Python
	int getProductionModifier(UnitTypes eUnit) const;															// Exposed to Python
	int getProductionModifier(BuildingTypes eBuilding) const;											// Exposed to Python
	int getProductionModifier(ProjectTypes eProject) const;												// Exposed to Python

/************************************************************************************************/
/* Afforess	Multiple Production Mod		 08/23/09                                            */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
//	int getOverflowProductionDifference(int iProductionNeeded, int iProduction, int iProductionModifier, int iDiff, int iModifiedProduction) const;
//	int getProductionDifference(int iProductionNeeded, int iProduction, int iProductionModifier, bool bFoodProduction, bool bOverflow) const;
	int getOverflowProductionDifference() const;
	int getProductionDifference(int iProductionNeeded, int iProduction, int iProductionModifier, bool bFoodProduction, bool bOverflow, bool bYield = true) const;
/************************************************************************************************/
/* Afforess	Multiple Production Mod       END                                                */
/************************************************************************************************/
	int getCurrentProductionDifference(bool bIgnoreFood, bool bOverflow) const;				// Exposed to Python
	int getExtraProductionDifference(int iExtra) const;																					// Exposed to Python

	bool canHurry(HurryTypes eHurry, bool bTestVisible = false) const;		// Exposed to Python
	void hurry(HurryTypes eHurry);																						// Exposed to Python
// BUG - Hurry Assist - start
	bool hurryOverflow(HurryTypes eHurry, int* iProduction, int* iGold, bool bCountThisTurn = false) const;
// BUG - Hurry Assist - end

	UnitTypes getConscriptUnit() const;																// Exposed to Python
	CvUnit* initConscriptedUnit();
	int getConscriptPopulation() const;																// Exposed to Python
	int conscriptMinCityPopulation() const;																			// Exposed to Python
	int flatConscriptAngerLength() const;																				// Exposed to Python
	bool canConscript() const;																				// Exposed to Python
	void conscript();																											// Exposed to Python
/************************************************************************************************/
/* INFLUENCE_DRIVEN_WAR                   04/16/09                                johnysmith    */
/*                                                                                              */
/* Original Author Moctezuma              Start                                                 */
/************************************************************************************************/
	// ------ BEGIN InfluenceDrivenWar -------------------------------
	void emergencyConscript();
	// ------ END InfluenceDrivenWar ---------------------------------
/************************************************************************************************/
/* INFLUENCE_DRIVEN_WAR                   04/16/09                                johnysmith    */
/*                                                                                              */
/* Original Author Moctezuma              End                                                   */
/************************************************************************************************/

	int getBonusHealth(BonusTypes eBonus) const;																// Exposed to Python - getBonusHealth
	int getBonusHappiness(BonusTypes eBonus) const;															// Exposed to Python - getBonusHappiness
	int getBonusPower(BonusTypes eBonus, bool bDirty) const;										// Exposed to Python 
	int getBonusYieldRateModifier(YieldTypes eIndex, BonusTypes eBonus) const;	// Exposed to Python 
	void processBonus(BonusTypes eBonus, int iChange);

	void processBuilding(BuildingTypes eBuilding, int iChange, bool bObsolete = false, bool bReplacingNow = false);
	void processProcess(ProcessTypes eProcess, int iChange);
	void processSpecialist(SpecialistTypes eSpecialist, int iChange);

	HandicapTypes getHandicapType() const;												// Exposed to Python
	CivilizationTypes getCivilizationType() const;								// Exposed to Python
	LeaderHeadTypes getPersonalityType() const;															// Exposed to Python
	DllExport ArtStyleTypes getArtStyleType() const;														// Exposed to Python
	DllExport CitySizeTypes getCitySizeType() const;												// Exposed to Python
	DllExport const CvArtInfoBuilding* getBuildingArtInfo(BuildingTypes eBuilding) const;
	DllExport float getBuildingVisibilityPriority(BuildingTypes eBuilding) const;

	bool hasTrait(TraitTypes eTrait) const;																	// Exposed to Python
	bool isBarbarian() const;																								// Exposed to Python
	bool isHuman() const;																										// Exposed to Python
	DllExport bool isVisible(TeamTypes eTeam, bool bDebug) const;						// Exposed to Python

	bool isCapital() const;																				// Exposed to Python
	bool isCoastal(int iMinWaterSize) const;																									// Exposed to Python
	bool isDisorder() const;																			// Exposed to Python				 
	bool isHolyCity(ReligionTypes eIndex) const;									// Exposed to Python				
	bool isHolyCity() const;																			// Exposed to Python				
	bool isHeadquarters(CorporationTypes eIndex) const;									// Exposed to Python				
	bool isHeadquarters() const;																			// Exposed to Python				
	void setHeadquarters(CorporationTypes eIndex);

	int getOvercrowdingPercentAnger(int iExtra = 0) const;									// Exposed to Python
	int getNoMilitaryPercentAnger() const;																	// Exposed to Python 
	int getCulturePercentAnger() const;																			// Exposed to Python
	int getReligionPercentAnger() const;																		// Exposed to Python
	int getHurryPercentAnger(int iExtra = 0) const;																				// Exposed to Python
	int getConscriptPercentAnger(int iExtra = 0) const;																		// Exposed to Python
	int getDefyResolutionPercentAnger(int iExtra = 0) const;
	int getWarWearinessPercentAnger() const;																// Exposed to Python
/************************************************************************************************/
/* REVOLUTION_MOD                         04/19/08                                jdog5000      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	int getRevRequestPercentAnger(int iExtra = 0) const;																// Exposed to Python
	int getRevIndexPercentAnger(int iExtra = 0) const;																// Exposed to Python
	int getRevSuccessHappiness() const;																// Exposed to Python
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/
	int getLargestCityHappiness() const;																		// Exposed to Python
	int getVassalHappiness() const;																		// Exposed to Python
	int getVassalUnhappiness() const;																		// Exposed to Python
	int unhappyLevel(int iExtra = 0) const;																	// Exposed to Python 
	int happyLevel() const;																				// Exposed to Python				
	int angryPopulation(int iExtra = 0) const;										// Exposed to Python

	int visiblePopulation() const;
	int totalFreeSpecialists() const;															// Exposed to Python				 
	int extraPopulation() const;																						// Exposed to Python
	int extraSpecialists() const;																						// Exposed to Python
	int extraFreeSpecialists() const;																				// Exposed to Python

	int unhealthyPopulation(bool bNoAngry = false, int iExtra = 0) const;	// Exposed to Python
	int totalGoodBuildingHealth() const;																		// Exposed to Python
	int totalBadBuildingHealth() const;														// Exposed to Python
	int goodHealth() const;																				// Exposed to Python
	int badHealth(bool bNoAngry = false, int iExtra = 0) const;		// Exposed to Python
	int healthRate(bool bNoAngry = false, int iExtra = 0) const;	// Exposed to Python
	int foodConsumption(bool bNoAngry = false, int iExtra = 0, bool bIncludeWastage = true) const;				// Exposed to Python
	int foodDifference(bool bBottom = true, bool bIncludeWastage = true, bool bIgnoreFoodBuildOrRev = false) const;								// Exposed to Python
	float foodWastage(int surplass = -1) const;
	int growthThreshold() const;																	// Exposed to Python

	int productionLeft() const;																							// Exposed to Python
	int hurryCost(bool bExtra) const;																				// Exposed to Python
	int getHurryCostModifier(bool bIgnoreNew = false) const;
	int hurryGold(HurryTypes eHurry) const;												// Exposed to Python
	int hurryPopulation(HurryTypes eHurry) const;									// Exposed to Python
	int hurryProduction(HurryTypes eHurry) const;														// Exposed to Python
	int flatHurryAngerLength() const;																				// Exposed to Python
	int hurryAngerLength(HurryTypes eHurry) const;													// Exposed to Python
	int maxHurryPopulation() const;																					// Exposed to Python

//	int cultureDistance(int iDX, int iDY) const;														// Exposed to Python
	int cultureStrength(PlayerTypes ePlayer) const;								// Exposed to Python					 
	int cultureGarrison(PlayerTypes ePlayer) const;								// Exposed to Python					 
	
	//	Note arrival or leaving of a unit
	void noteUnitMoved(CvUnit* pUnit) const;
	int getGlobalSourcedProperty(PropertyTypes eProperty) const;
	int getTotalBuildingSourcedProperty(PropertyTypes eProperty) const;
	int getTotalUnitSourcedProperty(PropertyTypes eProperty) const;
	int getNumBuilding(BuildingTypes eIndex) const;									// Exposed to Python					
	int getNumActiveBuilding(BuildingTypes eIndex) const;						// Exposed to Python
	bool hasActiveWorldWonder() const;																			// Exposed to Python
/************************************************************************************************/
/* UNOFFICIAL_PATCH                       03/04/10                     Mongoose & jdog5000      */
/*                                                                                              */
/* Bugfix                                                                                       */
/************************************************************************************************/
	int getNumActiveWorldWonders() const;
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/

	bool processGreatWall(bool bIn, bool bForce = false, bool bSeeded = true);

	int getReligionCount() const;																						// Exposed to Python  
	int getCorporationCount() const;																						// Exposed to Python  
	int getExtraSpecialistCommerce(CommerceTypes eIndex, SpecialistTypes eSpecialist) const;
	void updateExtraSpecialistCommerce();
	int getExtraSpecialistCommerceTotal(CommerceTypes eIndex) const;

	DllExport int getID() const;																			// Exposed to Python
	DllExport int getIndex() const;
	DllExport IDInfo getIDInfo() const;
	void setID(int iID);

	DllExport int getViewportX() const;																			// Exposed to Python
	int getX() const;																			// Exposed to Python
#ifdef _USRDLL
	inline int getX_INLINE() const
	{
		return m_iX;
	}
#endif
	DllExport int getViewportY() const;																			// Exposed to Python
	int getY() const;																			// Exposed to Python
#ifdef _USRDLL
	inline int getY_INLINE() const
	{
		return m_iY;
	}
#endif
	bool isInViewport(void) const;
	bool at(int iX, int iY) const;																				// Exposed to Python
	bool at(CvPlot* pPlot) const;																					// Exposed to Python - atPlot
	CvPlot* plot() const;																	// Exposed to Python
	DllExport CvPlot* plotExternal() const;																	// Exposed to Python
	CvPlotGroup* plotGroup(PlayerTypes ePlayer) const;
	bool isConnectedTo(CvCity* pCity) const;															// Exposed to Python
	bool isConnectedToCapital(PlayerTypes ePlayer = NO_PLAYER) const;			// Exposed to Python
	int getArea() const;
	CvArea* area() const;																						// Exposed to Python
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      01/02/09                                jdog5000      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	CvArea* waterArea(bool bNoImpassable = false) const;																			// Exposed to Python
	CvArea* secondWaterArea() const;
	CvArea* sharedWaterArea(CvCity* pCity) const;
	bool isBlockaded() const;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

	CvPlot* getRallyPlot() const;																// Exposed to Python
	void setRallyPlot(CvPlot* pPlot);

	int getGameTurnFounded() const;																				// Exposed to Python
	void setGameTurnFounded(int iNewValue);

	int getGameTurnAcquired() const;																			// Exposed to Python
	void setGameTurnAcquired(int iNewValue);

	int getPopulation() const;														// Exposed to Python
	void setPopulation(int iNewValue);										// Exposed to Python
	void changePopulation(int iChange);										// Exposed to Python

	long getRealPopulation() const;																	// Exposed to Python

	int getHighestPopulation() const;																			// Exposed to Python 
	void setHighestPopulation(int iNewValue);

	int getWorkingPopulation() const;																			// Exposed to Python
	void changeWorkingPopulation(int iChange);														

	int getSpecialistPopulation() const;																	// Exposed to Python
	void changeSpecialistPopulation(int iChange);													

	int getNumGreatPeople() const;																				// Exposed to Python
	void changeNumGreatPeople(int iChange);															

	int getBaseGreatPeopleRate() const;																		// Exposed to Python
	int getGreatPeopleRate() const;																				// Exposed to Python
	int getTotalGreatPeopleRateModifier() const;													// Exposed to Python
	void changeBaseGreatPeopleRate(int iChange);										// Exposed to Python

	int getGreatPeopleRateModifier() const;																// Exposed to Python
	void changeGreatPeopleRateModifier(int iChange);

	CvProperties* getProperties();
	const CvProperties* getPropertiesConst() const;

// BUG - Building Additional Great People - start
	int getAdditionalGreatPeopleRateByBuilding(BuildingTypes eBuilding);								// Exposed to Python
	int getAdditionalBaseGreatPeopleRateByBuilding(BuildingTypes eBuilding);							// Exposed to Python
	int getAdditionalGreatPeopleRateModifierByBuilding(BuildingTypes eBuilding);						// Exposed to Python
// BUG - Building Additional Great People - end

// BUG - Specialist Additional Great People - start
	int getAdditionalGreatPeopleRateBySpecialist(SpecialistTypes eSpecialist, int iChange) const;			// Exposed to Python
	int getAdditionalBaseGreatPeopleRateBySpecialist(SpecialistTypes eSpecialist, int iChange) const;		// Exposed to Python
// BUG - Specialist Additional Great People - end

	int getGreatPeopleProgress() const;													// Exposed to Python
	void changeGreatPeopleProgress(int iChange);										// Exposed to Python

	int getNumWorldWonders() const;																				// Exposed to Python
	void changeNumWorldWonders(int iChange);

	int getNumTeamWonders() const;																				// Exposed to Python
	void changeNumTeamWonders(int iChange);

	int getNumNationalWonders() const;																		// Exposed to Python
	void changeNumNationalWonders(int iChange);

	int getNumBuildings() const;																					// Exposed to Python
	void changeNumBuildings(int iChange);

	int getGovernmentCenterCount() const;																	
	bool isGovernmentCenter() const;														// Exposed to Python
	void changeGovernmentCenterCount(int iChange);													

// BUG - Building Saved Maintenance - start
	int getSavedMaintenanceByBuilding(BuildingTypes eBuilding) const;										// Exposed to Python
	int getSavedMaintenanceTimes100ByBuilding(BuildingTypes eBuilding) const;								// Exposed to Python
// BUG - Building Saved Maintenance - end

	int getMaintenance() const;																	// Exposed to Python
	int getMaintenanceTimes100() const;																	// Exposed to Python
	int getEffectiveMaintenanceModifier() const;
	void updateMaintenance() const;
	void setMaintenanceDirty(bool bDirty) const;
	int calculateDistanceMaintenance() const;										// Exposed to Python
	int calculateNumCitiesMaintenance() const;									// Exposed to Python
	int calculateColonyMaintenance() const;									// Exposed to Python
	int calculateCorporationMaintenance() const;									// Exposed to Python
/************************************************************************************************/
/* Afforess	                  Start		 09/08/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	int calculateDistanceMaintenanceTimes100(int iExtraDistanceModifier = 0, int iExtraCoastalDistanceModifier = 0) const;										// Exposed to Python
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
	//Afforess: added iExtraModifier
	int calculateNumCitiesMaintenanceTimes100(int iExtraModifier = 0) const;					// Exposed to Python
	int calculateColonyMaintenanceTimes100() const;									// Exposed to Python
	int calculateCorporationMaintenanceTimes100(CorporationTypes eCorporation) const;									// Exposed to Python
	int calculateCorporationMaintenanceTimes100() const;									// Exposed to Python
	int calculateBaseMaintenanceTimes100() const;
	int calculateBuildingMaintenanceTimes100() const;
	int getMaintenanceModifier() const;													// Exposed to Python
	void changeMaintenanceModifier(int iChange);													

	int getWarWearinessModifier() const;																	// Exposed to Python
	void changeWarWearinessModifier(int iChange);													

	int getHurryAngerModifier() const;																	// Exposed to Python
	void changeHurryAngerModifier(int iChange);													

	int getHealRate() const;																							// Exposed to Python
	void changeHealRate(int iChange);

	int getCrime() const;
	void changeCrime(int iChange);
	int getCrimePerPop() const;
	void changeCrimePerPop(int iChange);
	int getCrimeFinal() const;
	int getCrimeRate() const;
	int getCrimePenaltyValue(const int iValue) const;

	int getEspionageHealthCounter() const;														// Exposed to Python
	void changeEspionageHealthCounter(int iChange);													// Exposed to Python

	int getEspionageHappinessCounter() const;														// Exposed to Python
	void changeEspionageHappinessCounter(int iChange);													// Exposed to Python

	int getFreshWaterGoodHealth() const;																	// Exposed to Python
	int getFreshWaterBadHealth() const;													// Exposed to Python
	void updateFreshWaterHealth();

	int getFeatureGoodHealth() const;																			// Exposed to Python
	int getFeatureBadHealth() const;														// Exposed to Python
	void updateFeatureHealth();
// BUG - Feature Health - start
	void calculateFeatureHealthPercent(int& iGood, int& iBad) const;
	void calculateFeatureHealthPercentChange(int& iGood, int& iBad, CvPlot* pIgnorePlot = NULL) const;
	int getAdditionalHealthByFeature(FeatureTypes eFeature, int iChange) const;									// Exposed to Python
	int getAdditionalHealthByFeature(FeatureTypes eFeature, int iChange, int& iGood, int& iBad) const;
	int getAdditionalHealth(int iGoodPercent, int iBadPercent, int& iGood, int& iBad) const;
// BUG - Feature Health - end

// BUG - Actual Effects - start
	int getAdditionalAngryPopuplation(int iGood, int iBad) const;
	int getAdditionalSpoiledFood(int iGood, int iBad, int iHealthAdjust = 0) const;
	int getAdditionalStarvation(int iSpoiledFood, int iFoodAdjust = 0) const;
// BUG - Actual Effects - end

	int getBuildingGoodHealth() const;																		// Exposed to Python
	int getBuildingBadHealth() const;																			// Exposed to Python
	int getBuildingHealth(BuildingTypes eBuilding) const;									// Exposed to Python
	int getBuildingGoodHealth(BuildingTypes eBuilding) const;
	int getBuildingBadHealth(BuildingTypes eBuilding) const;
	void changeBuildingGoodHealth(int iChange);
	void changeBuildingBadHealth(int iChange);

	int getPowerGoodHealth() const;																				// Exposed to Python 
	int getPowerBadHealth() const;															// Exposed to Python 
	void updatePowerHealth();

	int getBonusGoodHealth() const;																				// Exposed to Python  
	int getBonusBadHealth() const;															// Exposed to Python 
	void changeBonusGoodHealth(int iChange);
	void changeBonusBadHealth(int iChange);

	int getMilitaryHappiness() const;																			// Exposed to Python
	int getMilitaryHappinessUnits() const;																// Exposed to Python
	void changeMilitaryHappinessUnits(int iChange);

	int getBuildingGoodHappiness() const;																	// Exposed to Python 
	int getBuildingBadHappiness() const;																	// Exposed to Python 
	int getBuildingHappiness(BuildingTypes eBuilding) const;							// Exposed to Python
	void changeBuildingGoodHappiness(int iChange);
	void changeBuildingBadHappiness(int iChange);

	int getExtraBuildingGoodHappiness() const;														// Exposed to Python
	int getExtraBuildingBadHappiness() const;															// Exposed to Python
/********************************************************************************/
/* 	New Civic AI						19.08.2010				Fuyu			*/
/********************************************************************************/
//Fuyu bLimited
	void updateExtraBuildingHappiness(bool bLimited = false);

	int getAdditionalHappinessByCivic(CivicTypes eCivic, bool bDifferenceToCurrent = true, bool bCivicOptionVacuum = false, ReligionTypes eStateReligion = NO_RELIGION, int iExtraPop = 0, int iMilitaryHappinessUnits = -1) const;
	int getAdditionalHealthByCivic(CivicTypes eCivic, bool bDifferenceToCurrent = true) const;
	int getAdditionalHealthByCivic(CivicTypes eCivic, int& iGood, int& iBad, int& iBadBuilding, bool bDifferenceToCurrent = true, int iExtraPop = 0, bool bCivicOptionVacuum = false, ReligionTypes eStateReligion = NO_RELIGION, int iIgnoreNoUnhealthyPopulationCount = 0, int iIgnoreBuildingOnlyHealthyCount = 0) const;
	int getAdditionalHealthByPlayerNoUnhealthyPopulation(int iExtraPop = 0, int iIgnoreNoUnhealthyPopulationCount = 0) const;
	int getAdditionalHealthByPlayerBuildingOnlyHealthy(int iIgnoreBuildingOnlyHealthyCount = 0) const;

// BUG - Building Additional Happiness - start
	int getAdditionalHappinessByBuilding(BuildingTypes eBuilding);									// Exposed to Python
	int getAdditionalHappinessByBuilding(BuildingTypes eBuilding, int& iGood, int& iBad, int& iAngryPop);
// BUG - Building Additional Happiness - end

	int getExtraBuildingGoodHealth() const;														// Exposed to Python
	int getExtraBuildingBadHealth() const;															// Exposed to Python
//Fuyu bLimited
	void updateExtraBuildingHealth(bool bLimited = false);

// BUG - Building Additional Health - start
	int getAdditionalHealthByBuilding(BuildingTypes eBuilding);										// Exposed to Python
	int getAdditionalHealthByBuilding(BuildingTypes eBuilding, int& iGood, int& iBad, int& iSpoiledFood, int& iStarvation);
// BUG - Building Additional Health - end

	int getFeatureGoodHappiness() const;																	// Exposed to Python
	int getFeatureBadHappiness() const;																		// Exposed to Python
//Fuyu bLimited
	void updateFeatureHappiness(bool bLimited = false);

	int getBonusGoodHappiness() const;																		// Exposed to Python  
	int getBonusBadHappiness() const;																			// Exposed to Python  
	void changeBonusGoodHappiness(int iChange);
	void changeBonusBadHappiness(int iChange);

	int getReligionGoodHappiness() const;																	// Exposed to Python
	int getReligionBadHappiness() const;																	// Exposed to Python
	int getReligionHappiness(ReligionTypes eReligion) const;							// Exposed to Python
	int getReligionHealth(ReligionTypes eReligion) const;							// Exposed to Python	
//Fuyu bLimited
	void updateReligionHappiness(bool bLimited = false);
	void updateReligionHealth(bool bLimited = false);
	int getReligionGoodHealth() const;																	// Exposed to Python
	int getReligionBadHealth() const;																	// Exposed to Python	
/********************************************************************************/
/* 	New Civic AI												END 			*/
/********************************************************************************/

	int getExtraHappiness() const;																				// Exposed to Python
	void changeExtraHappiness(int iChange);													// Exposed to Python

	int getExtraHealth() const;																				// Exposed to Python
	void changeExtraHealth(int iChange);													// Exposed to Python

	int getHurryAngerTimer() const;																				// Exposed to Python
	void changeHurryAngerTimer(int iChange);												// Exposed to Python

/************************************************************************************************/
/* REVOLUTION_MOD                         04/28/08                                jdog5000      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	int getRevRequestAngerTimer() const;															// Exposed to Python
	void changeRevRequestAngerTimer(int iChange);												// Exposed to Python
	
	int getRevSuccessTimer() const;															// Exposed to Python
	void changeRevSuccessTimer(int iChange);												// Exposed to Python
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/
	int getConscriptAngerTimer() const;																		// Exposed to Python
	void changeConscriptAngerTimer(int iChange);										// Exposed to Python

	int getDefyResolutionAngerTimer() const;																		// Exposed to Python
	void changeDefyResolutionAngerTimer(int iChange);										// Exposed to Python
	int flatDefyResolutionAngerLength() const;																				// Exposed to Python

	int getHappinessTimer() const;																				// Exposed to Python
	void changeHappinessTimer(int iChange);												// Exposed to Python

	int getNoUnhappinessCount() const;
	bool isNoUnhappiness() const;																					// Exposed to Python
	void changeNoUnhappinessCount(int iChange);

	int getNoUnhealthyPopulationCount() const;
	bool isNoUnhealthyPopulation() const;																	// Exposed to Python
	void changeNoUnhealthyPopulationCount(int iChange);

	int getBuildingOnlyHealthyCount() const;
	bool isBuildingOnlyHealthy() const;																		// Exposed to Python
	void changeBuildingOnlyHealthyCount(int iChange);

	int getFood() const;																				// Exposed to Python
	void setFood(int iNewValue);																		// Exposed to Python
	void changeFood(int iChange);																		// Exposed to Python

	int getFoodKept() const;																							// Exposed to Python
	void setFoodKept(int iNewValue);
	void changeFoodKept(int iChange);

	int getMaxFoodKeptPercent() const;																		// Exposed to Python
	void changeMaxFoodKeptPercent(int iChange, bool bAdd);

	int getOverflowProduction() const;																		// Exposed to Python
	void setOverflowProduction(int iNewValue);											// Exposed to Python
	void changeOverflowProduction(int iChange, int iProductionModifier);

	int getFeatureProduction()const;																		// Exposed to Python
	void setFeatureProduction(int iNewValue);											// Exposed to Python
	void changeFeatureProduction(int iChange);

	int getMilitaryProductionModifier() const;														// Exposed to Python
	void changeMilitaryProductionModifier(int iChange);												

	int getSpaceProductionModifier() const;																// Exposed to Python
	void changeSpaceProductionModifier(int iChange);

	int getExtraTradeRoutes() const;																			// Exposed to Python
	void changeExtraTradeRoutes(int iChange);															// Exposed to Python

	int getTradeRouteModifier() const;																		// Exposed to Python
	void changeTradeRouteModifier(int iChange);

	int getForeignTradeRouteModifier() const;																		// Exposed to Python
	void changeForeignTradeRouteModifier(int iChange);

	int getBuildingDefense() const;																				// Exposed to Python
	void changeBuildingDefense(int iChange);

	int getBuildingBombardDefense() const;																				// Exposed to Python
	void changeBuildingBombardDefense(int iChange);
// BUG - Building Additional Bombard Defense - start
	int getAdditionalBombardDefenseByBuilding(BuildingTypes eBuilding) const;									// Exposed to Python
// BUG - Building Additional Bombard Defense - start

	int getFreeExperience() const;																				// Exposed to Python
	void changeFreeExperience(int iChange);															

	int getCurrAirlift() const;																						// Exposed to Python
	void setCurrAirlift(int iNewValue);
	void changeCurrAirlift(int iChange);

	int getMaxAirlift() const;																						// Exposed to Python
	void changeMaxAirlift(int iChange);

	int getAirModifier() const;																						// Exposed to Python
	void changeAirModifier(int iChange);

	int getAirUnitCapacity(TeamTypes eTeam) const;										// Exposed to Python
	void changeAirUnitCapacity(int iChange);										// Exposed to Python

	int getNukeModifier() const;																					// Exposed to Python
	void changeNukeModifier(int iChange);

	int getFreeSpecialist() const;																				// Exposed to Python  
	void changeFreeSpecialist(int iChange);

	int getPowerCount() const;
	int getNoPowerCount() const;	
	bool isPower() const;																									// Exposed to Python
	bool isNoPower() const;																						// Exposed to Python
	bool isAreaCleanPower() const;																				// Exposed to Python
	// xUPT: variables (dbkblk, 2015-02)
public:
	bool isAreaSaturatedOfLandMilitaryUnits();
	bool isAreaSaturatedOfCivilianUnits();
	int getCurrentUnitsInArea(bool isCombatUnit);
	int getMaxUnitsInArea(bool isCombatUnit);
	void distributeUnitsOverFreeTiles();
	bool m_bAreaSaturatedOfLandMilitaryUnits;
	bool m_bAreaSaturatedOfAirMilitaryUnits;
	bool m_bAreaSaturatedOfCivilianUnits;
	// xUPT: end.

public:
	int getDirtyPowerCount() const;
	bool isDirtyPower() const;																						// Exposed to Python
	void changePowerCount(int iChange, bool bDirty);
	void changeNoPowerCount(int iChange);
	
	bool isAreaBorderObstacle() const;																				// Exposed to Python

	int getDefenseDamage() const;																					// Exposed to Python
	void changeDefenseDamage(int iChange);													// Exposed to Python
	void changeDefenseModifier(int iChange);													// Exposed to Python

	int getLastDefenseDamage() const;																			// Exposed to Python
	void setLastDefenseDamage(int iNewValue);

	bool isBombardable(const CvUnit* pUnit) const;														// Exposed to Python
	int getNaturalDefense() const;																				// Exposed to Python
	int getTotalDefense(bool bIgnoreBuilding) const;											// Exposed to Python
	int getDefenseModifier(bool bIgnoreBuilding) const;										// Exposed to Python

	int getOccupationTimer() const;															// Exposed to Python
	bool isOccupation() const;																	// Exposed to Python 
	void setOccupationTimer(int iNewValue);													// Exposed to Python
	void changeOccupationTimer(int iChange);												// Exposed to Python

	int getCultureUpdateTimer() const;															// Exposed to Python
	void setCultureUpdateTimer(int iNewValue);
	void changeCultureUpdateTimer(int iChange);												// Exposed to Python

	// < M.A.D. Nukes Start >
	int getMADIncoming();											// Exposed to Python
	void setMADIncoming(int iValue);								// Exposed to Python
	void changeMADIncoming(int iValue);							// Exposed to Python
	// < M.A.D. Nukes End   >

	bool isNeverLost() const;																							// Exposed to Python
	void setNeverLost(bool bNewValue);															// Exposed to Python

	bool isBombarded() const;																							// Exposed to Python
	void setBombarded(bool bNewValue);															// Exposed to Python

	bool isDrafted() const;																								// Exposed to Python
	void setDrafted(bool bNewValue);																// Exposed to Python

	bool isAirliftTargeted() const;																				// Exposed to Python
	void setAirliftTargeted(bool bNewValue);												// Exposed to Python

	bool isWeLoveTheKingDay() const;														// Exposed to Python 
	void setWeLoveTheKingDay(bool bNewValue);

	bool isCitizensAutomated() const;														// Exposed to Python 
	void setCitizensAutomated(bool bNewValue);											// Exposed to Python 

	bool isProductionAutomated() const;													// Exposed to Python
	void setProductionAutomated(bool bNewValue, bool bClear);										// Exposed to Python 

	/* allows you to programatically specify a cities walls rather than having them be generated automagically */
	DllExport bool isWallOverride() const; 
	void setWallOverride(bool bOverride);

	DllExport bool isInfoDirty() const;
	DllExport void setInfoDirty(bool bNewValue);

	DllExport bool isLayoutDirty() const;
	DllExport void setLayoutDirty(bool bNewValue);

	bool isPlundered() const;																		// Exposed to Python
	void setPlundered(bool bNewValue);																// Exposed to Python

	DllExport PlayerTypes getOwner() const;																// Exposed to Python
#ifdef _USRDLL
	inline PlayerTypes getOwnerINLINE() const
	{
		return m_eOwner;
	}
#endif
	DllExport TeamTypes getTeam() const;																	// Exposed to Python

	PlayerTypes getPreviousOwner() const;																	// Exposed to Python
	void setPreviousOwner(PlayerTypes eNewValue);

	PlayerTypes getOriginalOwner() const;																	// Exposed to Python
	void setOriginalOwner(PlayerTypes eNewValue);

	inline CultureLevelTypes getCultureLevel() const														// Exposed to Python
	{
		return m_eCultureLevel; // f1rpo.opt: inline
	}
	int getCultureThreshold() const;																	// Exposed to Python
	int getCultureThreshold(CultureLevelTypes eLevel) const;
	void setCultureLevel(CultureLevelTypes eNewValue, bool bUpdatePlotGroups);
	void updateCultureLevel(bool bUpdatePlotGroups);

	int getSeaPlotYield(YieldTypes eIndex) const;																// Exposed to Python
	void changeSeaPlotYield(YieldTypes eIndex, int iChange);

	int getRiverPlotYield(YieldTypes eIndex) const;																// Exposed to Python
	void changeRiverPlotYield(YieldTypes eIndex, int iChange);

// BUG - Building Additional Yield - start
	int getAdditionalYieldByBuilding(YieldTypes eIndex, BuildingTypes eBuilding);						// Exposed to Python
	int getAdditionalBaseYieldRateByBuilding(YieldTypes eIndex, BuildingTypes eBuilding);			// Exposed to Python
	int getAdditionalYieldRateModifierByBuilding(YieldTypes eIndex, BuildingTypes eBuilding) const;			// Exposed to Python
// BUG - Building Additional Yield - end

// BUG - Specialist Additional Yield - start
	int getAdditionalYieldBySpecialist(YieldTypes eIndex, SpecialistTypes eSpecialist, int iChange) const;				// Exposed to Python
	int getAdditionalBaseYieldRateBySpecialist(YieldTypes eIndex, SpecialistTypes eSpecialist, int iChange) const;		// Exposed to Python
// BUG - Specialist Additional Yield - end

	int getBaseYieldRate(YieldTypes eIndex) const;															// Exposed to Python
	int getBaseYieldRateModifier(YieldTypes eIndex, int iExtra = 0) const;			// Exposed to Python
	int getYieldRate(YieldTypes eIndex) const; // Exposed to Python
	int getYieldRate100(YieldTypes eIndex, const bool bCrime = true) const;
	void setBaseYieldRate(YieldTypes eIndex, int iNewValue);												// Exposed to Python
	void changeBaseYieldRate(YieldTypes eIndex, int iChange);												// Exposed to Python

	int getYieldRateModifier(YieldTypes eIndex) const;													// Exposed to Python
	void changeYieldRateModifier(YieldTypes eIndex, int iChange);

	int getPowerYieldRateModifier(YieldTypes eIndex) const;											// Exposed to Python 
	void changePowerYieldRateModifier(YieldTypes eIndex, int iChange);

	int getBonusYieldRateModifier(YieldTypes eIndex) const;											// Exposed to Python 
	void changeBonusYieldRateModifier(YieldTypes eIndex, int iChange);

	int getTradeYield(YieldTypes eIndex) const;																	// Exposed to Python
	int totalTradeModifier(CvCity* pOtherCity = NULL) const;																							// Exposed to Python
	int getPopulationTradeModifier() const;
	int getPeaceTradeModifier(TeamTypes eTeam) const;
	int getBaseTradeProfit(CvCity* pCity) const;
// BUG - Fractional Trade Routes - start
#ifdef _MOD_FRACTRADE
	int calculateTradeProfitTimes100(CvCity* pCity) const;															// Exposed to Python
#endif
// BUG - Fractional Trade Routes - end
	int calculateTradeProfit(CvCity* pCity) const;															// Exposed to Python
	int calculateTradeYield(YieldTypes eIndex, int iTradeProfit) const;					// Exposed to Python
// BUG - Trade Totals - start
	void calculateTradeTotals(YieldTypes eIndex, int& iDomesticYield, int& iDomesticRoutes, int& iForeignYield, int& iForeignRoutes, PlayerTypes eWithPlayer = NO_PLAYER, bool bRound = false, bool bBase = false) const;
	int calculateTotalTradeYield(YieldTypes eIndex, PlayerTypes eWithPlayer = NO_PLAYER, bool bRound = false, bool bBase = false) const;
// BUG - Trade Totals - end
	void setTradeYield(YieldTypes eIndex, int iNewValue);

	int getExtraSpecialistYield(YieldTypes eIndex) const;																				// Exposed to Python
	int getExtraSpecialistYield(YieldTypes eIndex, SpecialistTypes eSpecialist) const;					// Exposed to Python
	void updateExtraSpecialistYield(YieldTypes eYield);
	void updateExtraSpecialistYield();

	int getCommerceRate(CommerceTypes eIndex) const;									// Exposed to Python
	int getCommerceRateTimes100(CommerceTypes eIndex) const; // Exposed to Python
	int getCommerceFromPercent(CommerceTypes eIndex, int iYieldRate) const;			// Exposed to Python
	int getBaseCommerceRate(CommerceTypes eIndex) const;												// Exposed to Python
	int getBaseCommerceRateTimes100(CommerceTypes eIndex) const;												// Exposed to Python
	int getTotalCommerceRateModifier(CommerceTypes eIndex) const;								// Exposed to Python
	void setCommerceModifierDirty(CommerceTypes eCommerce);
	void setCommerceDirty(CommerceTypes eCommerce = NO_COMMERCE);
	void updateCommerce(CommerceTypes eIndex = NO_COMMERCE, bool bForce = true) const;

	int getProductionToCommerceModifier(CommerceTypes eIndex) const;						// Exposed to Python
	void changeProductionToCommerceModifier(CommerceTypes eIndex, int iChange);

	int getBuildingCommerce(CommerceTypes eIndex) const;																				// Exposed to Python
	int getBuildingCommerceByBuilding(CommerceTypes eIndex, BuildingTypes eBuilding) const;			// Exposed to Python
// BUG - Building Additional Commerce - start
	int getAdditionalCommerceByBuilding(CommerceTypes eIndex, BuildingTypes eBuilding);						// Exposed to Python
	int getAdditionalCommerceTimes100ByBuilding(CommerceTypes eIndex, BuildingTypes eBuilding);				// Exposed to Python
	int getAdditionalBaseCommerceRateByBuilding(CommerceTypes eIndex, BuildingTypes eBuilding);				// Exposed to Python
	int getAdditionalBaseCommerceRateByBuildingImpl(CommerceTypes eIndex, BuildingTypes eBuilding);
	int getAdditionalCommerceRateModifierByBuilding(CommerceTypes eIndex, BuildingTypes eBuilding);			// Exposed to Python
	int getAdditionalCommerceRateModifierByBuildingImpl(CommerceTypes eIndex, BuildingTypes eBuilding);
// BUG - Building Additional Commerce - end
	void updateBuildingCommerce();

	int getSpecialistCommerce(CommerceTypes eIndex) const;											// Exposed to Python
	void changeSpecialistCommerceTimes100(CommerceTypes eIndex, int iChange);
// BUG - Specialist Additional Commerce - start
	int getAdditionalCommerceBySpecialist(CommerceTypes eIndex, SpecialistTypes eSpecialist, int iChange) const;				// Exposed to Python
	int getAdditionalCommerceTimes100BySpecialist(CommerceTypes eIndex, SpecialistTypes eSpecialist, int iChange) const;		// Exposed to Python
	int getAdditionalBaseCommerceRateBySpecialist(CommerceTypes eIndex, SpecialistTypes eSpecialist, int iChange) const;		// Exposed to Python
	int getAdditionalBaseCommerceRateBySpecialistImpl(CommerceTypes eIndex, SpecialistTypes eSpecialist, int iChange) const;
// BUG - Specialist Additional Commerce - end


	int getReligionCommerce(CommerceTypes eIndex) const;																				// Exposed to Python
	int getReligionCommerceByReligion(CommerceTypes eIndex, ReligionTypes eReligion) const;			// Exposed to Python
	void updateReligionCommerce(CommerceTypes eIndex);
	void updateReligionCommerce();

	void setCorporationYield(YieldTypes eIndex, int iNewValue);
	int getCorporationCommerce(CommerceTypes eIndex) const;																				// Exposed to Python
	int getCorporationCommerceByCorporation(CommerceTypes eIndex, CorporationTypes eCorporation) const;			// Exposed to Python
	int getCorporationYield(YieldTypes eIndex) const;																				// Exposed to Python
	int getCorporationYieldByCorporation(YieldTypes eIndex, CorporationTypes eCorporation) const;			// Exposed to Python
	void updateCorporation();
	void updateCorporationCommerce(CommerceTypes eIndex);
	void updateCorporationYield(YieldTypes eIndex);
	void updateCorporationBonus();

	int getCommerceRateModifier(CommerceTypes eIndex) const;										// Exposed to Python
	void changeCommerceRateModifier(CommerceTypes eIndex, int iChange);

	int getCommerceHappinessPer(CommerceTypes eIndex) const;										// Exposed to Python
	int getCommerceHappinessByType(CommerceTypes eIndex) const;									// Exposed to Python
	int getCommerceHappiness() const;																						// Exposed to Python
	void changeCommerceHappinessPer(CommerceTypes eIndex, int iChange);

	int getDomainFreeExperience(DomainTypes eIndex) const;											// Exposed to Python
	void changeDomainFreeExperience(DomainTypes eIndex, int iChange);

	int getDomainProductionModifier(DomainTypes eIndex) const;									// Exposed to Python
	void changeDomainProductionModifier(DomainTypes eIndex, int iChange);

	int getCulture(PlayerTypes eIndex) const;													// Exposed to Python
	int getCultureTimes100(PlayerTypes eIndex) const;													// Exposed to Python
	int countTotalCultureTimes100() const;																							// Exposed to Python
	PlayerTypes findHighestCulture() const;																			// Exposed to Python
	int calculateCulturePercent(PlayerTypes eIndex) const;											// Exposed to Python
	int calculateTeamCulturePercent(TeamTypes eIndex) const;										// Exposed to Python
	void setCulture(PlayerTypes eIndex, int iNewValue, bool bPlots, bool bUpdatePlotGroups);			// Exposed to Python
	void setCultureTimes100(PlayerTypes eIndex, int iNewValue, bool bPlots, bool bUpdatePlotGroups);			// Exposed to Python
	void changeCulture(PlayerTypes eIndex, int iChange, bool bPlots, bool bUpdatePlotGroups);		// Exposed to Python
	void changeCultureTimes100(PlayerTypes eIndex, int iChange, bool bPlots, bool bUpdatePlotGroups);		// Exposed to Python

	int getNumRevolts(PlayerTypes eIndex) const;
	void changeNumRevolts(PlayerTypes eIndex, int iChange);
	int getRevoltTestProbability() const;

	bool isTradeRoute(PlayerTypes eIndex) const;																	// Exposed to Python
	void setTradeRoute(PlayerTypes eIndex, bool bNewValue);

	bool isEverOwned(PlayerTypes eIndex) const;																		// Exposed to Python
	void setEverOwned(PlayerTypes eIndex, bool bNewValue);

	DllExport bool isRevealed(TeamTypes eIndex, bool bDebug) const;								// Exposed to Python
	void setRevealed(TeamTypes eIndex, bool bNewValue);											// Exposed to Python

	bool getEspionageVisibility(TeamTypes eTeam) const;								// Exposed to Python
	void setEspionageVisibility(TeamTypes eTeam, bool bVisible, bool bUpdatePlotGroups);
	void updateEspionageVisibility(bool bUpdatePlotGroups);

	DllExport const CvWString getName(uint uiForm = 0) const;								// Exposed to Python
	DllExport const wchar* getNameKey() const;															// Exposed to Python
	void setName(const wchar* szNewValue, bool bFound = false);		// Exposed to Python
	void doFoundMessage();

	// Script data needs to be a narrow string for pickling in Python
	std::string getScriptData() const;																						// Exposed to Python
	void setScriptData(std::string szNewValue);															// Exposed to Python

	int getFreeBonus(BonusTypes eIndex) const;																		// Exposed to Python
	void changeFreeBonus(BonusTypes eIndex, int iChange);																		// Exposed to Python

	void processNumBonusChange(BonusTypes eIndex, int iOldValue, int iNewValue);
	void endDeferredBonusProcessing(void);
	void startDeferredBonusProcessing(void);
	int getNumBonusesFromBase(BonusTypes eIndex, int iBaseNum) const;

	int getNumBonuses(BonusTypes eIndex) const;																		// Exposed to Python
	bool hasBonus(BonusTypes eIndex) const;															// Exposed to Python
	void changeNumBonuses(BonusTypes eIndex, int iChange);

	int getNumCorpProducedBonuses(BonusTypes eIndex) const;
	bool isCorporationBonus(BonusTypes eBonus) const;
	bool isActiveCorporation(CorporationTypes eCorporation) const;

	int getBuildingProduction(BuildingTypes eIndex) const;							// Exposed to Python
	void setBuildingProduction(BuildingTypes eIndex, int iNewValue);				// Exposed to Python
	void changeBuildingProduction(BuildingTypes eIndex, int iChange);				// Exposed to Python

	int getBuildingProductionTime(BuildingTypes eIndex) const;										// Exposed to Python
	void setBuildingProductionTime(BuildingTypes eIndex, int iNewValue);		// Exposed to Python
	void changeBuildingProductionTime(BuildingTypes eIndex, int iChange);		// Exposed to Python
// BUG - Production Decay - start
	bool isBuildingProductionDecay(BuildingTypes eIndex) const;														// Exposed to Python
	int getBuildingProductionDecay(BuildingTypes eIndex) const;														// Exposed to Python
	int getBuildingProductionDecayTurns(BuildingTypes eIndex) const;												// Exposed to Python
// BUG - Production Decay - end

	int getProjectProduction(ProjectTypes eIndex) const;								// Exposed to Python
	void setProjectProduction(ProjectTypes eIndex, int iNewValue);					// Exposed to Python
	void changeProjectProduction(ProjectTypes eIndex, int iChange);					// Exposed to Python

	int getBuildingOriginalOwner(BuildingTypes eIndex) const;											// Exposed to Python
	int getBuildingOriginalTime(BuildingTypes eIndex) const;											// Exposed to Python

	int getUnitProduction(UnitTypes eIndex) const;											// Exposed to Python
	void setUnitProduction(UnitTypes eIndex, int iNewValue);								// Exposed to Python
	void changeUnitProduction(UnitTypes eIndex, int iChange);								// Exposed to Python

	int getUnitProductionTime(UnitTypes eIndex) const;														// Exposed to Python
	void setUnitProductionTime(UnitTypes eIndex, int iNewValue);						// Exposed to Python
	void changeUnitProductionTime(UnitTypes eIndex, int iChange);						// Exposed to Python
// BUG - Production Decay - start
	bool isUnitProductionDecay(UnitTypes eIndex) const;														// Exposed to Python
	int getUnitProductionDecay(UnitTypes eIndex) const;														// Exposed to Python
	int getUnitProductionDecayTurns(UnitTypes eIndex) const;												// Exposed to Python
// BUG - Production Decay - end

	int getGreatPeopleUnitRate(UnitTypes eIndex) const;														// Exposed to Python
	void setGreatPeopleUnitRate(UnitTypes eIndex, int iNewValue);
	void changeGreatPeopleUnitRate(UnitTypes eIndex, int iChange);

	int getGreatPeopleUnitProgress(UnitTypes eIndex) const;							// Exposed to Python
	void setGreatPeopleUnitProgress(UnitTypes eIndex, int iNewValue);				// Exposed to Python
	void changeGreatPeopleUnitProgress(UnitTypes eIndex, int iChange);			// Exposed to Python

	int getSpecialistCount(SpecialistTypes eIndex) const;								// Exposed to Python
	void setSpecialistCount(SpecialistTypes eIndex, int iNewValue);
	void changeSpecialistCount(SpecialistTypes eIndex, int iChange);
	void alterSpecialistCount(SpecialistTypes eIndex, int iChange);					// Exposed to Python

	int getMaxSpecialistCount(SpecialistTypes eIndex) const;						// Exposed to Python
	bool isSpecialistValid(SpecialistTypes eIndex, int iExtra = 0) const;					// Exposed to Python
	void changeMaxSpecialistCount(SpecialistTypes eIndex, int iChange);

	int getForceSpecialistCount(SpecialistTypes eIndex) const;					// Exposed to Python
	bool isSpecialistForced() const;																							// Exposed to Python
	void setForceSpecialistCount(SpecialistTypes eIndex, int iNewValue);		// Exposed to Python
	void changeForceSpecialistCount(SpecialistTypes eIndex, int iChange);		// Exposed to Python

	int getFreeSpecialistCount(SpecialistTypes eIndex) const;					// Exposed to Python
	void setFreeSpecialistCount(SpecialistTypes eIndex, int iNewValue);			// Exposed to Python
	void changeFreeSpecialistCount(SpecialistTypes eIndex, int iChange, bool bUnattributed = false);		// Exposed to Python
	int getAddedFreeSpecialistCount(SpecialistTypes eIndex) const;		// Exposed to Python

	int getImprovementFreeSpecialists(ImprovementTypes eIndex) const;			// Exposed to Python
	void changeImprovementFreeSpecialists(ImprovementTypes eIndex, int iChange);		// Exposed to Python

	int getReligionInfluence(ReligionTypes eIndex) const;													// Exposed to Python
	void changeReligionInfluence(ReligionTypes eIndex, int iChange);				// Exposed to Python

	int getCurrentStateReligionHappiness() const;																	// Exposed to Python
	int getStateReligionHappiness(ReligionTypes eIndex) const;										// Exposed to Python
	void changeStateReligionHappiness(ReligionTypes eIndex, int iChange);		// Exposed to Python
	
	int getCurrentStateReligionHealth() const;																	// Exposed to Python
	int getStateReligionHealth(ReligionTypes eIndex) const;										// Exposed to Python
	void changeStateReligionHealth(ReligionTypes eIndex, int iChange);		// Exposed to Python	

	int getUnitCombatFreeExperience(UnitCombatTypes eIndex) const;								// Exposed to Python
	void changeUnitCombatFreeExperience(UnitCombatTypes eIndex, int iChange);

	int getFreePromotionCount(PromotionTypes eIndex) const;												// Exposed to Python
	bool isFreePromotion(PromotionTypes eIndex) const;														// Exposed to Python
	void changeFreePromotionCount(PromotionTypes eIndex, int iChange);

	int getSpecialistFreeExperience() const;								// Exposed to Python
	void changeSpecialistFreeExperience(int iChange);

	int getEspionageDefenseModifier() const;										// Exposed to Python
	void changeEspionageDefenseModifier(int iChange);

	bool isWorkingPlot(int iIndex) const;													// Exposed to Python
	bool isWorkingPlot(const CvPlot* pPlot) const;													// Exposed to Python
	void setWorkingPlot(int iIndex, bool bNewValue);
	void setWorkingPlot(CvPlot* pPlot, bool bNewValue);
	void alterWorkingPlot(int iIndex);																			// Exposed to Python
	void processWorkingPlot(int iPlot, int iChange, bool yieldsOnly = false);

	int getNumRealBuilding(BuildingTypes eIndex) const;														// Exposed to Python
	void setNumRealBuilding(BuildingTypes eIndex, int iNewValue);		// Exposed to Python
	void setNumRealBuildingTimed(BuildingTypes eIndex, int iNewValue, bool bFirst, PlayerTypes eOriginalOwner, int iOriginalTime);

	bool isValidBuildingLocation(BuildingTypes eIndex) const;

	int getNumFreeBuilding(BuildingTypes eIndex) const;															// Exposed to Python
    int getNumContConnFreeBuilding(BuildingTypes eIndex) const; // DarkLunaPhantom - Continuous and Connected Extra Free Building entries (cf. BuildingSchema xml).
	int getNumFreeAreaBuilding(BuildingTypes eIndex) const;															// Exposed to Python
	int getNumFreeTradeRegionBuilding(BuildingTypes eIndex) const;
	void setNumFreeBuilding(BuildingTypes eIndex, int iNewValue);
    void setNumContConnFreeBuilding(BuildingTypes eIndex, int iNewValue); // DarkLunaPhantom - Continuous and Connected Extra Free Building entries (cf. BuildingSchema xml).
	void setNumFreeAreaBuilding(BuildingTypes eIndex, int iNewValue);
	void setNumFreeTradeRegionBuilding(BuildingTypes eIndex, int iNewValue);
	void changeNumFreeTradeRegionBuilding(BuildingTypes eIndex, int iChange);

	bool isHasReligion(ReligionTypes eIndex) const;
	void setHasReligion(ReligionTypes eIndex, bool bNewValue, bool bAnnounce, bool bArrows = true);
	void applyReligionModifiers(ReligionTypes eIndex, bool bValue);

	bool isHasCorporation(CorporationTypes eIndex) const;
	void setHasCorporation(CorporationTypes eIndex, bool bNewValue, bool bAnnounce, bool bArrows = true);
	void applyCorporationModifiers(CorporationTypes eIndex, bool bValue);

	CvCity* getTradeCity(int iIndex) const;																				// Exposed to Python
	int getTradeRoutes() const;																										// Exposed to Python
	void clearTradeRoutes();
	void updateTradeRoutes();

	void clearOrderQueue();																														// Exposed to Python
	bool pushOrder(OrderTypes eOrder, int iData1, int iData2, bool bSave, bool bPop, bool bAppend, bool bForce = false, CvPlot* deliveryDestination = NULL, UnitAITypes contractedAIType = NO_UNITAI, byte contractFlags = 0);		// Exposed to Python
	void setProposedOrder(OrderTypes eOrder, int iData1, int iData2 = -1);
	void popOrder(int iNum, bool bFinish = false, bool bChoose = false, bool bResolveList = true, bool bPipelineCompletion = false);		// Exposed to Python
	void startHeadOrder();
	void stopHeadOrder();
	int getOrderQueueLength();																		// Exposed to Python
	OrderData* getOrderFromQueue(int iIndex);											// Exposed to Python
	CLLNode<OrderData>* nextOrderQueueNode(CLLNode<OrderData>* pNode) const;
	CLLNode<OrderData>* headOrderQueueNode() const;
	DllExport int getNumOrdersQueued() const;
	DllExport OrderData getOrderData(int iIndex) const;
	OrderData getOrderDataInternal(int iIndex, bool externalView = true) const;
	bool pushFirstValidBuildListOrder(int iListID);

	// fill the kVisible array with buildings that you want shown in city, as well as the number of generics
	// This function is called whenever CvCity::setLayoutDirty() is called
	DllExport void getVisibleBuildings(std::list<BuildingTypes>& kVisible, int& iNumGenerics);
	
	// Fill the kEffectNames array with references to effects in the CIV4EffectInfos.xml to have a
	// city play a given set of effects. This is called whenever the interface updates the city billboard
	// or when the zoom level changes
	DllExport void getVisibleEffects(ZoomLevelTypes eCurrentZoom, std::vector<const TCHAR*>& kEffectNames);


	// Billboard appearance controls
	DllExport void getCityBillboardSizeIconColors(NiColorA& kDotColor, NiColorA& kTextColor) const;
	DllExport const TCHAR* getCityBillboardProductionIcon() const;
	DllExport bool getFoodBarPercentages(std::vector<float>& afPercentages) const;
	DllExport bool getProductionBarPercentages(std::vector<float>& afPercentages) const;
	DllExport NiColorA getBarBackgroundColor() const;
	DllExport bool isStarCity() const;
	
	// Exposed to Python
	DllExport void setWallOverridePoints(const std::vector< std::pair<float, float> >& kPoints); /* points are given in world space ... i.e. PlotXToPointX, etc */
	DllExport const std::vector< std::pair<float, float> >& getWallOverridePoints() const;

	bool isEventTriggerPossible(EventTriggerTypes eTrigger) const;
	int getTriggerValue(EventTriggerTypes eTrigger) const;
	bool canApplyEvent(EventTypes eEvent, const EventTriggeredData& kTriggeredData) const;
	void applyEvent(EventTypes eEvent, const EventTriggeredData* pTriggeredData);
	bool isEventOccured(EventTypes eEvent) const;
	void setEventOccured(EventTypes eEvent, bool bOccured);

	int getBuildingYieldChange(BuildingClassTypes eBuildingClass, YieldTypes eYield) const;           // Exposed to Python
	void setBuildingYieldChange(BuildingClassTypes eBuildingClass, YieldTypes eYield, int iChange);          // Exposed to Python
	void changeBuildingYieldChange(BuildingClassTypes eBuildingClass, YieldTypes eYield, int iChange);
	int getBuildingCommerceChange(BuildingClassTypes eBuildingClass, CommerceTypes eCommerce) const;           // Exposed to Python
	void setBuildingCommerceChange(BuildingClassTypes eBuildingClass, CommerceTypes eCommerce, int iChange);          // Exposed to Python
	void changeBuildingCommerceChange(BuildingClassTypes eBuildingClass, CommerceTypes eCommerce, int iChange);
	int getBuildingHappyChange(BuildingClassTypes eBuildingClass) const;           // Exposed to Python
	void setBuildingHappyChange(BuildingClassTypes eBuildingClass, int iChange);          // Exposed to Python
	int getBuildingHealthChange(BuildingClassTypes eBuildingClass) const;           // Exposed to Python
	void setBuildingHealthChange(BuildingClassTypes eBuildingClass, int iChange);          // Exposed to Python

	PlayerTypes getLiberationPlayer(bool bConquest) const;   // Exposed to Python
	void liberate(bool bConquest);    // Exposed to Python

	void changeNoBonusCount(BonusTypes eBonus, int iChange);   // Exposed to Python
	int getNoBonusCount(BonusTypes eBonus) const;
	bool isNoBonus(BonusTypes eBonus) const;   // Exposed to Python

	bool isAutoRaze() const;

	DllExport int getMusicScriptId() const;
	DllExport int getSoundscapeScriptId() const;
	DllExport void cheat(bool bCtrl, bool bAlt, bool bShift);

	DllExport void getBuildQueue(std::vector<std::string>& astrQueue) const;

/************************************************************************************************/
/* Afforess  New Functions                             12/7/09                                 */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	int getCivicHappiness() const;
	int getAdditionalDefenseByBuilding(BuildingTypes eBuilding) const;
	int getNumCityPlots() const;
	int getPopulationgrowthratepercentage() const;
	void changePopulationgrowthratepercentage(int iChange, bool bAdd);
	bool isValidTerrainForBuildings(BuildingTypes eIndex) const;
	
	void changeFreshWater(int iChange);
	bool hasFreshWater() const;
	
	bool canUpgradeUnit(UnitTypes eUnit) const;
	int getUnitClassProductionModifier(UnitClassTypes eIndex) const;				 //Exposed to Python
	void changeUnitClassProductionModifier(UnitClassTypes eIndex, int iChange);
	int getBuildingClassProductionModifier(BuildingClassTypes eIndex) const;
	void changeBuildingClassProductionModifier(BuildingClassTypes eIndex, int iChange);
	bool hadVicinityBonus(BonusTypes eIndex) const;
	int getBonusDefenseChanges(BonusTypes eIndex) const;
	void changeBonusDefenseChanges(BonusTypes eIndex, int iChange);
	int getBonusCommerceRateModifier(CommerceTypes eIndex) const;
	void changeBonusCommerceRateModifier(CommerceTypes eIndex, int iChange);
	bool isBuiltFoodProducedUnit() const;
	void setBuiltFoodProducedUnit(bool bNewValue);
	int getSpecialistGoodHealth() const;																			// Exposed to Python
	int getSpecialistBadHealth() const;														// Exposed to Python
	int getSpecialistHappiness() const;																			// Exposed to Python
	int getSpecialistUnhappiness() const;														// Exposed to Python
	void changeSpecialistGoodHealth(int iChange);
	void changeSpecialistBadHealth(int iChange);
	void changeSpecialistHappiness(int iChange);
	void changeSpecialistUnhappiness(int iChange);
	int getImprovementGoodHealth() const;																			// Exposed to Python
	int getImprovementBadHealth() const;														// Exposed to Python
	void updateImprovementHealth();
	int getBonusCommerceRateModifier(CommerceTypes eIndex, BonusTypes eBonus) const;	// Exposed to Python 
	void clearLostProduction();
	bool isProductionWonder() const;
	void updateYieldRate(BuildingTypes eBuilding, YieldTypes eYield, int iChange);
	void updateMaxSpecialistCount(BuildingTypes eBuilding, SpecialistTypes eSpecialist, int iChange);
	void updateCommerceModifierByBuilding(BuildingTypes eBuilding, CommerceTypes eCommerce, int iChange);
	void updateCommerceRateByBuilding(BuildingTypes eBuilding, CommerceTypes eCommerce, int iChange);
	void updateYieldModifierByBuilding(BuildingTypes eBuilding, YieldTypes eYield, int iChange);
//	int getImprovementYieldChange(ImprovementTypes eIndex1, YieldTypes eIndex2) const;
//	void changeImprovementYieldChange(ImprovementTypes eIndex1, YieldTypes eIndex2, int iChange);
	int calculateBonusDefense() const;
	
	void setCivilizationType(int iCiv);
	
	int getBuildingCommerceModifier(BuildingClassTypes eBuildingClass, CommerceTypes eCommerce) const;
	int getBuildingYieldModifier(BuildingClassTypes eBuildingClass, YieldTypes eYield) const;
	
	int calculateBuildingCommerceModifier(CommerceTypes eCommerce) const;
	int calculateBuildingYieldModifier(YieldTypes eYield) const;
	int getRevTrend();
	bool isInquisitionConditions();
	int calculateCorporationHealth() const;
	int calculateCorporationHappiness() const;
	virtual bool AI_isEmphasizeAvoidAngryCitizens() = 0;
	virtual bool AI_isEmphasizeAvoidUnhealthyCitizens() = 0;
	int getExtraYieldTurns() const;
	void changeExtraYieldTurns (int iChange);
	void setExtraYieldTurns(int iNewVal);
	BuildTypes findChopBuild(FeatureTypes eFeature);
	CultureLevelTypes getOccupationCultureLevel() const;
	void setOccupationCultureLevel(CultureLevelTypes eNewValue);
	CultureLevelTypes getMaxCultureLevelAmongPlayers() const;
	CultureLevelTypes getCultureLevel(PlayerTypes eIndex) const;
	CultureLevelTypes getCultureLevelForCulture(int iCulture) const;
	int getLineOfSight() const;
	void changeLineOfSight(int iChange);
	int calculateBonusCommerceRateModifier(CommerceTypes eIndex) const;
	int getLandmarkAngerTimer() const;
	void changeLandmarkAngerTimer(int iChange);
	int getLandmarkAnger() const;
	void clearVicinityBonusCache(BonusTypes eBonus);
	bool hasVicinityBonus(BonusTypes eBonus) const;
	void checkBuildings(bool bBonus = true, bool bCivics = true, bool bWar = true, bool bPower = true, bool bPopulation = true, bool bAlertOwner = true);
	void doVicinityBonus();
	bool isDevelopingCity() const;
	
	int getMintedCommerceTimes100() const;

	int getUnitCombatExtraStrength(UnitCombatTypes eIndex) const;
	void changeUnitCombatExtraStrength(UnitCombatTypes eIndex, int iChange);
	
	int getInvasionChance() const;
	void changeInvasionChance(int iChange);
	
	int getInvasionTimer() const;
	void changeInvasionTimer(int iChange);
	bool isInvaded() const;
	
	void doInvasion();

	void setDisabledBuilding(BuildingTypes eIndex, bool bNewValue);
	bool isDisabledBuilding(BuildingTypes eIndex) const;

	bool isZoneOfControl() const;
	
	int getAdjacentDamagePercent() const;
	void changeAdjacentDamagePercent(int iChange);
	
	inline int getWorkableRadiusOverride() const
	{
		return m_iWorkableRadiusOverride; // f1rpo.opt: inline
	}
	void setWorkableRadiusOverride(int iNewVal);
	
	int getProtectedCultureCount() const;
	bool isProtectedCulture() const;
	void changeProtectedCultureCount(int iChange);
	
	int getNumUnitFullHeal() const;
	void changeNumUnitFullHeal(int iChange);
	
	int getCommerceAttacks(CommerceTypes eIndex) const;
	void changeCommerceAttacks(CommerceTypes eIndex, int iChange);
	int getMaxCommerceAttacks(CommerceTypes eIndex) const;
	void changeMaxCommerceAttacks(CommerceTypes eIndex, int iChange);
	
	void doAttack();
	void doHeal();
	void decayCommerce();
	
	void doCorporation();
	int getCorporationInfluence(CorporationTypes eCorporation) const;
	int calculateCorporateTaxes() const;
	
	int getDisabledPowerTimer() const;
	void changeDisabledPowerTimer(int iChange);
	void doDisabledPower();
	
	int getWarWearinessTimer() const;
	void changeWarWearinessTimer(int iChange);
	void doWarWeariness();
	
	int getEventAnger() const;
	void changeEventAnger(int iChange);
	
	int getNonHolyReligionCount() const;
	
	void calculateExtraTradeRouteProfit(int iExtra, int* &aiTradeYields) const;
	
	int getMinimumDefenseLevel() const;
	void setMinimumDefenseLevel(int iNewValue);
	
	SpecialistTypes getBestSpecialist(int iExtra);
	
	int getNumPopulationEmployed() const;
	void setNumPopulationEmployed(int iNewValue);
	void changeNumPopulationEmployed(int iNewValue);
	
	void removeWorstCitizenActualEffects(int iNumCitizens, int& iGreatPeopleRate, int& iHappiness, int& iHealthiness, int*& aiYields, int*& aiCommerces);
	virtual int AI_plotValue(CvPlot* pPlot, bool bAvoidGrowth, bool bRemove, bool bIgnoreFood = false, bool bIgnoreGrowth = false, bool bIgnoreStarvation = false) = 0;
	
	void changeHealthPercentPerPopulation(int iChange);
	int calculatePopulationHealth() const;
	void changeHappinessPercentPerPopulation(int iChange);
	int calculatePopulationHappiness() const;

	int getAssignedSpecialistCount() const;
	
	int getBonusCommercePercentChanges(CommerceTypes eIndex) const;
	int getBonusCommercePercentChanges(CommerceTypes eIndex, BonusTypes eBonus) const;
	int getBonusCommercePercentChanges(CommerceTypes eIndex, BuildingTypes eBuilding) const;
	void changeBonusCommercePercentChanges(CommerceTypes eIndex, int iChange);
	
	int getAdditionalBaseCommerceRateByBuildingTimes100(CommerceTypes eIndex, BuildingTypes eBuilding);

	bool isAutomatedCanBuild(BuildTypes eBuild) const; //Exposed to Python
	void setAutomatedCanBuild(BuildTypes eBuild, bool bNewValue); //Exposed to Python
	
	virtual int AI_getMilitaryProductionRateRank() const = 0;

	virtual bool AI_isMilitaryProductionCity() const = 0;
	virtual void AI_setMilitaryProductionCity(bool bNewVal) = 0;
	
	int getConnectedDomesticCities() const;
	int getConnectedForeignCities() const;
	int getAvgTargetCities() const;

	void doConnectednessCalculations();
	int getConnectednessYield(BuildingTypes eBuilding, YieldTypes eYield, bool bIncludeExtraCapitalCommerce = true); // Exposed to Python

	void changeWonderCapacityIncrement(int iChange);
	int getWonderCapacityIncrement() const;

	int calculateExtraCapitalCommerce() const;
	void changeExtraCapitalCommerce(int iChange);
	int calculateExtraForeignCapitalCommerce() const;
	void changeExtraForeignCapitalCommerce(int iChange);
	int getDomesticConnectednessCommerce() const;
	int getForeignConnectednessCommerce() const;
	int getForeignConnectednessNeeded() const;

	void updateSeizedForeignConnectedness();
	int getSeizedForeignConnectednessTimes100(PlayerTypes ePlayer) const;
	int getTotalSeizedForeignConnectednessTimes100() const;

	int getDistanceMaintenanceSavedTimes100ByCivic(CivicTypes eCivic) const;
	int getNumCitiesMaintenanceSavedTimes100ByCivic(CivicTypes eCivic) const;
	int getHomeAreaMaintenanceSavedTimes100ByCivic(CivicTypes eCivic) const;
	int getOtherAreaMaintenanceSavedTimes100ByCivic(CivicTypes eCivic) const;

	void recalculateZoomLevel(CultureLevelTypes eCultureLevel);

	void resync(bool bWrite, ByteBuffer* pBuffer);

	int getForcedRevolutionCounter() const;
	void changeForcedRevolutionCounter(int iChange);
	void doForcedRevolutionUpdate();
/************************************************************************************************/
/* Afforess	                         END                                                        */
/************************************************************************************************/	

	void read(FDataStreamBase* pStream);
	void write(FDataStreamBase* pStream);


	// WATIGGI adapted by 45deg
	void setInConqueredMode(bool bInConqueredMode);
	bool isInConqueredMode();

	void setSuspendOccupationTimer(int iSuspendOccupationTimer);
	int getSuspendOccupationTimer();
	void changeSuspendOccupationTimer(int iChange);

	void setResistance(int iResistance);
	int getResistance();
	void setSpecialistRelocationProb(int iSpecialistProb);
	int getSpecialistRelocationProb();
	// end WATIGGI adapted by 45deg

	virtual void AI_init() = 0;
	virtual void AI_reset() = 0;
	virtual void AI_doTurn() = 0;
	virtual void AI_assignWorkingPlots() = 0;
	virtual void AI_updateAssignWork() = 0;
	virtual bool AI_avoidGrowth() = 0;											// Exposed to Python
	virtual int AI_specialistValue(SpecialistTypes eSpecialist, bool bAvoidGrowth, bool bRemove) = 0;
	virtual bool AI_changeProduction() = 0;
	virtual void AI_prepareForTurnProcessing() = 0;
	virtual void AI_chooseProduction() = 0;
	//	KOSHLING - initialisation called on every city prior to performing unit mission allocation logic
	//	This allows caches that will remian valid for the procesign of teh current turn's units to be cleared
	virtual void AI_preUnitTurn(void) = 0;
	virtual void AI_noteUnitEscortNeeded(void) = 0;
	virtual void AI_trained(UnitTypes eUnitType, UnitAITypes eUnitAIType) = 0;
	virtual UnitTypes AI_bestUnit(int& iBestValue, int iNumSelectableTypes = -1, UnitAITypes* pSelectableTypes = NULL, bool bAsync = false, UnitAITypes* peBestUnitAI = NULL, bool bNoRand = false, bool bNoWeighting = false, CvUnitSelectionCriteria* criteria = NULL) = 0;
/********************************************************************************/
/* 	City Defenders						24.07.2010				Fuyu			*/
/********************************************************************************/
//Fuyu bIgnoreNotUnitAIs
	virtual UnitTypes AI_bestUnitAI(UnitAITypes eUnitAI, int& iBestValue, bool bAsync = false, bool bNoRand = false, CvUnitSelectionCriteria* criteria = NULL) = 0;
/********************************************************************************/
/* 	City Defenders												END 			*/
/********************************************************************************/
	virtual void AI_FlushBuildingValueCache(bool bRetainValues = false) = 0;
	virtual BuildingTypes AI_bestBuilding(int iFocusFlags = 0, int iMaxTurns = MAX_INT, bool bAsync = false, AdvisorTypes eIgnoreAdvisor = NO_ADVISOR, bool bMaximizeFlaggedValue = false) = 0;
	virtual int AI_buildingValue(BuildingTypes eBuilding, int iFocusFlags = 0, bool bForTech = false) = 0;
	virtual int AI_projectValue(ProjectTypes eProject) = 0;
	virtual int AI_neededSeaWorkers() = 0;
	virtual bool AI_isDefended(int iExtra = 0, bool bAllowAnyDefenders = true) = 0;
/********************************************************************************/
/**		BETTER_BTS_AI_MOD							9/19/08		jdog5000		*/
/**																				*/
/**		Air AI																	*/
/********************************************************************************/
/* original BTS code
	virtual bool AI_isAirDefended(int iExtra = 0) = 0;
*/
	virtual bool AI_isAirDefended(bool bCountLand = 0, int iExtra = 0) = 0;
	virtual bool AI_isAdequateHappinessMilitary(int iExtra = 0) = 0;
/********************************************************************************/
/**		BETTER_BTS_AI_MOD						END								*/
/********************************************************************************/
	virtual bool AI_isDanger() = 0;
	virtual int evaluateDanger() = 0;
	virtual int AI_neededDefenders() = 0;
	virtual int AI_neededAirDefenders() = 0;
	virtual int AI_minDefenders() = 0;
	virtual bool AI_isEmphasizeAvoidGrowth() = 0;
	virtual bool AI_isAssignWorkDirty() = 0;
	virtual CvCity* AI_getRouteToCity() const = 0;
	virtual void AI_setAssignWorkDirty(bool bNewValue) = 0;
	virtual bool AI_isChooseProductionDirty() = 0;
	virtual void AI_setChooseProductionDirty(bool bNewValue) = 0;
	virtual bool AI_isEmphasize(EmphasizeTypes eIndex) = 0;											// Exposed to Python
	virtual void AI_setEmphasize(EmphasizeTypes eIndex, bool bNewValue) = 0;
	virtual bool AI_isEmphasizeSpecialist(SpecialistTypes eIndex) = 0;
	virtual void AI_setEmphasizeSpecialist(SpecialistTypes eIndex, bool bNewValue) = 0;
	virtual int AI_getBestBuildValue(int iIndex) = 0;
	virtual void AI_markBestBuildValuesStale(void) = 0;

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      06/25/09                                jdog5000      */
/*                                                                                              */
/* Debug                                                                                        */
/************************************************************************************************/
	virtual int AI_getTargetSize() = 0;
	virtual int AI_getGoodTileCount() = 0;
	virtual int AI_getImprovementValue( CvPlot* pPlot, ImprovementTypes eImprovement, int iFoodPriority, int iProductionPriority, int iCommercePriority, int iFoodChange, bool bOriginal = false ) = 0;
	virtual void AI_getYieldMultipliers( int &iFoodMultiplier, int &iProductionMultiplier, int &iCommerceMultiplier, int &iDesiredFoodChange ) = 0;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/
	virtual int AI_totalBestBuildValue(CvArea* pArea) = 0;
	virtual int AI_countBestBuilds(CvArea* pArea) = 0;													// Exposed to Python
	virtual BuildTypes AI_getBestBuild(int iIndex) = 0;
	virtual void AI_updateBestBuild() = 0;
	virtual int AI_cityValue() const = 0;
	virtual int AI_clearFeatureValue(int iIndex) = 0;

	virtual int AI_calculateCulturePressure(bool bGreatWork = false) = 0;
	virtual int AI_calculateWaterWorldPercent() = 0;
	virtual int AI_countNumBonuses(BonusTypes eBonus, bool bIncludeOurs, bool bIncludeNeutral, int iOtherCultureThreshold, bool bLand = true, bool bWater = true) = 0;
	virtual int AI_yieldMultiplier(YieldTypes eYield) = 0;
	virtual int AI_playerCloseness(PlayerTypes eIndex, int iMaxDistance = 7) = 0;
	virtual int AI_cityThreat(TeamTypes eTargetTeam = NO_TEAM, int* piThreatModifier = NULL) = 0;
	virtual BuildingTypes AI_bestAdvancedStartBuilding(int iPass) = 0;
	
	virtual int AI_getWorkersHave() = 0;
	virtual int AI_getWorkersNeeded() = 0;
	virtual void AI_changeWorkersHave(int iChange) = 0;
/********************************************************************************/
/* 	Worker Counting						03.08.2010				Fuyu			*/
/********************************************************************************/
	virtual int AI_workingCityPlotTargetMissionAIs(PlayerTypes ePlayer, MissionAITypes eMissionAI, UnitAITypes eUnitAI = NO_UNITAI, bool bSameAreaOnly = false) const = 0;
/********************************************************************************/
/* 	Worker Counting 											END 			*/
/********************************************************************************/
	virtual int AI_getBuildPriority(void) const = 0;

	bool hasShrine(ReligionTypes eReligion);
	bool hasOrbitalInfrastructure() const;
	void processVoteSourceBonus(VoteSourceTypes eVoteSource, bool bActive);

	void invalidatePopulationRankCache();
	void invalidateYieldRankCache(YieldTypes eYield = NO_YIELD);
	void invalidateCommerceRankCache(CommerceTypes eCommerce = NO_COMMERCE);

	int getBestYieldAvailable(YieldTypes eYield) const;

/************************************************************************************************/
/* phunny_pharmer                Start		 05/03/10                                           */
/*   note: recalculateCultureDistance must be const as it is called from cultureDistance, a     */
/*     const function; this means that the actual cached structure must be mutable in order to  */
/*     be modified in the const method                                                          */
/************************************************************************************************/
	void recalculateCultureDistances(int iMaxDistance) const;
	int calculateCultureDistance(int iDX, int iDY, int iMaxDistance) const;
	void clearCultureDistanceCache();
	int cultureDistance(int iDX, int iDY, bool bForce = false) const;														// Exposed to Python
/************************************************************************************************/
/* phunny_pharmer                End		 05/03/10                                           */
/************************************************************************************************/
	void clearModifierTotals(void);
	void recalculateModifiers(void);

	void setBuildingListInvalid();
	bool getBuildingListFilterActive(BuildingFilterTypes eFilter);
	void setBuildingListFilterActive(BuildingFilterTypes eFilter, bool bActive);
	BuildingGroupingTypes getBuildingListGrouping();
	void setBuildingListGrouping(BuildingGroupingTypes eGrouping);
	BuildingSortTypes getBuildingListSorting();
	void setBuildingListSorting(BuildingSortTypes eSorting);
	int getBuildingListGroupNum();
	int getBuildingListNumInGroup(int iGroup);
	BuildingTypes getBuildingListType(int iGroup, int iPos);
	int getBuildingListSelectedBuildingRow();
	int getBuildingListSelectedWonderRow();
	void setBuildingListSelectedBuilding(BuildingTypes eBuilding);
	void setBuildingListSelectedWonder(BuildingTypes eWonder);
	BuildingTypes getBuildingListSelectedBuilding();
	BuildingTypes getBuildingListSelectedWonder();

	void setUnitListInvalid();
	bool getUnitListFilterActive(UnitFilterTypes eFilter);
	void setUnitListFilterActive(UnitFilterTypes eFilter, bool bActive);
	UnitGroupingTypes getUnitListGrouping();
	void setUnitListGrouping(UnitGroupingTypes eGrouping);
	UnitSortTypes getUnitListSorting();
	void setUnitListSorting(UnitSortTypes eSorting);
	int getUnitListGroupNum();
	int getUnitListNumInGroup(int iGroup);
	UnitTypes getUnitListType(int iGroup, int iPos);
	int getUnitListSelectedRow();
	void setUnitListSelected(UnitTypes eUnit);
	UnitTypes getUnitListSelected();

	bool isDirectAttackable() const;

	void changeZoCCount(short iChange);
	/*	<f1rpo> Downcasting to CvCityAI is fine
		(but can't do a static_cast w/o including the CityAI header) */
	__forceinline CvCityAI& AI() { return *reinterpret_cast<CvCityAI*>(this); }
	__forceinline CvCityAI const& AI() const { return *reinterpret_cast<CvCityAI const*>(this); }
	// </f1rpo>

protected:

	int m_iID;
	int m_iX;
	int m_iY;
	int m_iRallyX;
	int m_iRallyY;
	int m_iGameTurnFounded;
	int m_iGameTurnAcquired;
	int m_iPopulation;
	int m_iHighestPopulation;
	int m_iWorkingPopulation;
	int m_iSpecialistPopulation;
	int m_iNumGreatPeople;
	int m_iBaseGreatPeopleRate;
	int m_iGreatPeopleRateModifier;
	int m_iGreatPeopleProgress;
	int m_iNumWorldWonders;
	int m_iNumTeamWonders;
	int m_iNumNationalWonders;
	int m_iNumBuildings;
	int m_iGovernmentCenterCount;
	mutable int m_iMaintenance;
	int m_iMaintenanceModifier;
	int m_iWarWearinessModifier;
	int m_iHurryAngerModifier;
	int m_iHealRate;
	int m_iCrime;
	int m_iCrimePerPop;
	int m_iEspionageHealthCounter;
	int m_iEspionageHappinessCounter;
	int m_iFreshWaterGoodHealth;
	int m_iFreshWaterBadHealth;
	int m_iFeatureGoodHealth;
	int m_iFeatureBadHealth;
	int m_iBuildingGoodHealth;
	int m_iBuildingBadHealth;
	int m_iPowerGoodHealth;
	int m_iPowerBadHealth;
	int m_iBonusGoodHealth;
	int m_iBonusBadHealth;
	int m_iHurryAngerTimer;
/************************************************************************************************/
/* REVOLUTION_MOD                         04/28/08                                jdog5000      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	int m_iRevRequestAngerTimer;
	int m_iRevSuccessTimer;
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/
	int m_iConscriptAngerTimer;
	int m_iDefyResolutionAngerTimer;
	int m_iHappinessTimer;
	int m_iMilitaryHappinessUnits;
	int m_iBuildingGoodHappiness;
	int m_iBuildingBadHappiness;
	int m_iExtraBuildingGoodHappiness;
	int m_iExtraBuildingBadHappiness;
	int m_iExtraBuildingGoodHealth;
	int m_iExtraBuildingBadHealth;
	int m_iFeatureGoodHappiness;
	int m_iFeatureBadHappiness;
	int m_iBonusGoodHappiness;
	int m_iBonusBadHappiness;
	int m_iReligionGoodHappiness;
	int m_iReligionBadHappiness;
	int m_iReligionGoodHealth;
	int m_iReligionBadHealth;	
	int m_iExtraHappiness;
	int m_iExtraHealth;
	int m_iNoUnhappinessCount;
	int m_iNoUnhealthyPopulationCount;
	int m_iBuildingOnlyHealthyCount;
	int m_iFood;
	int m_iFoodKept;
	float m_fMaxFoodKeptMultiplierLog;
#define INVALID_STORED_FOOD_PERCENT_LOG (-1000000)	//	Used as a reserved value to trigger calculation on upgrade of save format
	int m_iOverflowProduction;
	int m_iFeatureProduction;
/************************************************************************************************/
/* Afforess	New Variables	    	 12/7/09                                                    */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	int m_iLostProductionBase;
	int m_iSpecialistGoodHealth;
	int m_iSpecialistBadHealth;
	int m_iSpecialistHappiness;
	int m_iSpecialistUnhappiness;
	int m_iImprovementGoodHealth;
	int m_iImprovementBadHealth;
	int m_iLostProductionModified;
	int m_iGoldFromLostProduction;
	int m_iCiv;
	float m_fPopulationgrowthratepercentageLog;
#define INVALID_GROWTH_PERCENT_LOG ((float)-10000.0)	//	Used to detect old format saves when loading

	bool m_bBuiltFoodProducedUnit;
	bool m_bResetTechs;
	int m_iExtraYieldTurns;
	int m_iLineOfSight;
	int m_iLandmarkAngerTimer;
	int m_iInvasionChance;
	int m_iInvasionTimer;
	int m_iAdjacentDamagePercent;
	int m_iWorkableRadiusOverride;
	int m_iProtectedCultureCount;
	int m_iNumUnitFullHeal;
	int m_iDisabledPowerTimer;
	int m_iWarWearinessTimer;
	int m_iMinimumDefenseLevel;
	int m_iNumPopulationEmployed;
	int m_iHappinessPercentPerPopulation;
	int m_iHealthPercentPerPopulation;
	int m_iExtraCapitalCommerce;
	int m_iExtraForeignCapitalCommerce;

	int m_iPreviousExtraCommerce;
	int m_iPreviousConnectedCommerce;
	int m_iPreviousForeignConnectedCommerce;
	
	int m_iLostProduction;
	int m_iEventAnger;
	
	int m_iFreshWater;
	short m_iZoCCount;

	int* m_aiBonusCommerceRateModifier;
	int* m_aiBonusCommercePercentChanges;
	int* m_paiUnitClassProductionModifier;
	int* m_paiBuildingClassProductionModifier;
	int* m_paiBonusDefenseChanges;
	bool* m_pabHadVicinityBonus;
	mutable bool* m_pabHasVicinityBonusCached;
	mutable bool* m_pabHasVicinityBonus;
	bool* m_pabDisabledBuilding;
	int* m_paiUnitCombatExtraStrength;
	int* m_aiCommerceAttacks;
	int* m_aiMaxCommerceAttacks;
	bool* m_pabAutomatedCanBuild;

	int m_iForeignConnectednessNeeded;

	int* m_aiSeizedForeignConnectedness;
	void clearSeizedForeignConnectedness();

	std::vector<BuildingCommerceModifier> m_aBuildingCommerceModifier;
	std::vector<BuildingYieldModifier> m_aBuildingYieldModifier;
	
	CultureLevelTypes m_eOccupationCultureLevel;

	int m_iForcedRevolutionCounter;
/************************************************************************************************/
/* Afforess	                          END                                                       */
/************************************************************************************************/
	int m_iMilitaryProductionModifier;
	int m_iSpaceProductionModifier;
	int m_iExtraTradeRoutes;
	int m_iTradeRouteModifier;
	int m_iForeignTradeRouteModifier;
	int m_iBuildingDefense;
	int m_iBuildingBombardDefense;
	int m_iFreeExperience;
	int m_iCurrAirlift;
	int m_iMaxAirlift;
	int m_iAirModifier;
	int m_iAirUnitCapacity;
	int m_iWonderCapacityIncrement;
	int m_iNukeModifier;
	int m_iFreeSpecialist;
	int m_iPowerCount;
	int m_iNoPowerCount;
	int m_iDirtyPowerCount;
	int m_iDefenseDamage;
	int m_iLastDefenseDamage;
	int m_iOccupationTimer;
	int m_iCultureUpdateTimer;
	int m_iSpecialistFreeExperience;
	int m_iEspionageDefenseModifier;
	// < M.A.D. Nukes Start >
	int m_iMADIncoming;
	// < M.A.D. Nukes Start >

	bool m_bNeverLost;
	bool m_bBombarded;
	bool m_bDrafted;
	bool m_bAirliftTargeted;
	bool m_bWeLoveTheKingDay;
	bool m_bCitizensAutomated;
	bool m_bProductionAutomated;
	bool m_bWallOverride;
	bool m_bInfoDirty;
	bool m_bLayoutDirty;
	bool m_bPlundered;
/************************************************************************************************/
/* UNOFFICIAL_PATCH                       12/07/09                         denev & jdog5000     */
/*                                                                                              */
/* Bugfix                                                                                       */
/************************************************************************************************/
	bool m_bPopProductionProcess;
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/

	PlayerTypes m_eOwner;
	PlayerTypes m_ePreviousOwner;
	PlayerTypes m_eOriginalOwner;
	CultureLevelTypes m_eCultureLevel;

/************************************************************************************************/
/* REVOLUTION_MOD                         06/10/08                                jdog5000      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	int m_iRevolutionIndex;
	int m_iLocalRevIndex;
	int m_iRevIndexAverage;
	int m_iRevolutionCounter;
	int m_iReinforcementCounter;
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/
	int m_iExtraBuildingDefenseRecoverySpeedModifier;
	int m_iModifiedBuildingDefenseRecoverySpeedCap;
	int m_iExtraCityDefenseRecoverySpeedModifier;
	int* m_aiSeaPlotYield;
	int* m_aiRiverPlotYield;
	int* m_aiBaseYieldRate;
	int* m_aiYieldRateModifier;
	int* m_aiPowerYieldRateModifier;
	int* m_aiBonusYieldRateModifier;
	int* m_aiTradeYield;
	int* m_aiCorporationYield;
	int* m_aiExtraSpecialistYield;
	mutable int* m_aiCommerceRate;
	mutable bool* m_abCommerceRateDirty;
	int* m_aiProductionToCommerceModifier;
	int* m_aiBuildingCommerce;
	int* m_aiSpecialistCommerce;
	int* m_aiReligionCommerce;
	int* m_aiCorporationCommerce;
	int* m_aiCommerceRateModifier;
	int* m_aiCommerceHappinessPer;
	int* m_aiDomainFreeExperience;
	int* m_aiDomainProductionModifier;
	int* m_aiCulture;
	int* m_aiNumRevolts;

	bool* m_abEverOwned;
	bool* m_abTradeRoute;
	bool* m_abRevealed;
	bool* m_abEspionageVisibility;

	CvWString m_szName;
	CvString m_szScriptData;

	int* m_paiNoBonus;
	int* m_paiFreeBonus;
	int* m_paiNumBonuses;
	int* m_paiNumCorpProducedBonuses;
	int* m_paiProjectProduction;
	int* m_paiBuildingProduction;
	int* m_paiBuildingProductionTime;
	int* m_paiBuildingOriginalOwner;
	int* m_paiBuildingOriginalTime;
	int* m_paiUnitProduction;
	int* m_paiUnitProductionTime;
	int* m_paiGreatPeopleUnitRate;
	int* m_paiGreatPeopleUnitProgress;
	int* m_paiSpecialistCount;
	int* m_paiMaxSpecialistCount;
	int* m_paiForceSpecialistCount;
	int* m_paiFreeSpecialistCount;
	int* m_paiFreeSpecialistCountUnattributed;
	int* m_paiImprovementFreeSpecialists;
	int* m_paiReligionInfluence;
	int* m_paiStateReligionHappiness;
	int* m_paiStateReligionHealth;	
	int* m_paiUnitCombatFreeExperience;
	int* m_paiFreePromotionCount;
	int* m_paiNumRealBuilding;
	int* m_paiNumFreeBuilding;
    int* m_paiNumContConnFreeBuilding; // DarkLunaPhantom - Continuous and Connected Extra Free Building entries (cf. BuildingSchema xml).
	int* m_paiNumFreeAreaBuilding;
	int* m_paiNumFreeTradeRegionBuilding;
	mutable int* m_paiBuildingReplaced;
	mutable bool m_bHasCalculatedBuildingReplacement;

	bool* m_pabWorkingPlot;
	bool* m_pabHasReligion;
	bool* m_pabHasCorporation;

	int		m_deferringBonusProcessingCount;
	int*	m_paiStartDeferredSectionNumBonuses;

	CvProperties m_Properties;
	CvBuildingList m_BuildingList;
	CvUnitList m_UnitList;

	IDInfo* m_paTradeCities;

	mutable CLinkList<OrderData> m_orderQueue;

	std::vector< std::pair < float, float> > m_kWallOverridePoints;

	std::vector<EventTypes> m_aEventsOccured;
	std::vector<BuildingYieldChange> m_aBuildingYieldChange;
	std::vector<BuildingCommerceChange> m_aBuildingCommerceChange;
	BuildingChangeArray m_aBuildingHappyChange;
	BuildingChangeArray m_aBuildingHealthChange;

	// CACHE: cache frequently used values
	mutable int	m_iPopulationRank;
	mutable bool m_bPopulationRankValid;
	int*	m_aiBaseYieldRank;
	bool*	m_abBaseYieldRankValid;
	int*	m_aiYieldRank;
	bool*	m_abYieldRankValid;
	int*	m_aiCommerceRank;
	bool*	m_abCommerceRankValid;

	CvCityTurnPipelineWorkItem* m_workItem;

/************************************************************************************************/
/* phunny_pharmer                Start		 04/21/10                                           */
/************************************************************************************************/
	mutable std::map<int,int> m_aCultureDistances;
/************************************************************************************************/
/* phunny_pharmer                End		 04/21/10                                           */
/************************************************************************************************/

	void doGrowth();
	void doCulture();
	void doPlotCulture(bool bUpdate, PlayerTypes ePlayer, int iCultureRate);
	void doProduction(CvCityTurnPipelineWorkItem* workItem, bool bRequeueOnProduce);
	void doDecay();
	void doReligion();
	void doGreatPeople();
	void doMeltdown();
	bool doCheckProduction();
/************************************************************************************************/
/* Afforess	Meteorology Modmod Start		 09/25/09                                           */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	void doPromotion();
/************************************************************************************************/
/* Afforess	                               END                                                  */
/************************************************************************************************/	

	int getExtraProductionDifference(int iExtra, UnitTypes eUnit) const;
	int getExtraProductionDifference(int iExtra, BuildingTypes eBuilding) const;
	int getExtraProductionDifference(int iExtra, ProjectTypes eProject) const;
	int getExtraProductionDifference(int iExtra, int iModifier) const;
	int getHurryCostModifier(UnitTypes eUnit, bool bIgnoreNew) const;
	int getHurryCostModifier(BuildingTypes eBuilding, bool bIgnoreNew) const;
	int getHurryCostModifier(int iBaseModifier, int iProduction, bool bIgnoreNew) const;
	int getHurryCost(bool bExtra, UnitTypes eUnit, bool bIgnoreNew) const;
	int getHurryCost(bool bExtra, BuildingTypes eBuilding, bool bIgnoreNew) const;
	int getHurryCost(bool bExtra, int iProductionLeft, int iHurryModifier, int iModifier) const;
	int getHurryPopulation(HurryTypes eHurry, int iHurryCost) const;
	int getHurryGold(HurryTypes eHurry, int iHurryCost) const;
	bool canHurryUnit(HurryTypes eHurry, UnitTypes eUnit, bool bIgnoreNew) const;
	bool canHurryBuilding(HurryTypes eHurry, BuildingTypes eBuilding, bool bIgnoreNew) const;
	void recalculateMaxFoodKeptPercent(void);
	void recalculatePopulationgrowthratepercentage(void);
	virtual bool AI_addBestCitizen(bool bWorkers, bool bSpecialists, int* piBestPlot = NULL, SpecialistTypes* peBestSpecialist = NULL) = 0;
	virtual bool AI_removeWorstCitizen(SpecialistTypes eIgnoreSpecialist = NO_SPECIALIST) = 0;

	// WATIGGI adapted by 45deg 
	bool m_bInConqueredMode;
	int m_iSuspendOccupationTimer;

	int m_cc_iResistance;
	int m_cc_iSpecialistProb;
	// end WATIGGI adapted by 45deg
	
	void calculateBuildingReplacements(void) const;
	void changeBuildingReplacementCount(BuildingTypes eBuilding, bool bAdd);

	int getCitySorenRandNum(int iNum, TCHAR* pszLog);
	int getRandNum100();
#ifdef YIELD_VALUE_CACHING
	virtual void AI_NoteWorkerChange(void) = 0;
	virtual void AI_NoteSpecialistChange(void) = 0;
public:
	virtual void ClearYieldValueCache(void) = 0;
#ifdef _DEBUG
	virtual void CheckYieldValueCache(char* label) = 0;
#define CHECK_YIELD_VALUE_CACHE(label) CheckYieldValueCache(label);
#else
#define CHECK_YIELD_VALUE_CACHE(label) ;
#endif
#endif

	//	Koshling - add cache of trainability of units which will be
	//	populated prior to calculating the city's build choices and
	//	then invalidated so it is only used within that scope
#ifdef CAN_TRAIN_CACHING
	int getExtraBuildingDefenseRecoverySpeedModifier() const;
	void setExtraBuildingDefenseRecoverySpeedModifier(int iValue);
	void changeExtraBuildingDefenseRecoverySpeedModifier(int iChange);
	
	int getModifiedBuildingDefenseRecoverySpeedCap() const;
	void setModifiedBuildingDefenseRecoverySpeedCap(int iValue);
	void changeModifiedBuildingDefenseRecoverySpeedCap(int iChange);
	
	int getExtraCityDefenseRecoverySpeedModifier() const;
	void setExtraCityDefenseRecoverySpeedModifier(int iValue);
	void changeExtraCityDefenseRecoverySpeedModifier(int iChange);

	int cityDefenseRecoveryRate() const;

private:
	mutable stdext::hash_map<UnitCombatTypes,bool> m_canTrainCache;
	mutable stdext::hash_map<UnitTypes,bool> m_canTrainCacheUnits;
	mutable stdext::hash_map<UnitTypes,UnitTypes> m_eCachedAllUpgradesResults;
	mutable stdext::hash_map<UnitTypes,UnitTypes> m_eCachedAllUpgradesResultsRoot;
	mutable bool m_canTrainCachePopulated;
	mutable bool m_canTrainCacheDirty;
	mutable bool m_canTrainCacheGloballyValid;
	mutable int m_cachedBuildingYieldModifers[NUM_YIELD_TYPES];
	int m_recalcBuilding;
	bool m_bPlotWorkingMasked;
	mutable int m_totalCommerceRateModifier[NUM_COMMERCE_TYPES];

	mutable std::map<int,int> m_buildingSourcedPropertyCache;
	mutable std::map<int,int> m_unitSourcedPropertyCache;

	bool m_bIsGreatWallSeed;
	mutable bool m_bMaintenanceDirty;

	std::vector<OrderData>		m_inProcessOrders;
	std::vector<OrderData>		m_completedOrders;
	mutable std::map<int,bool>*	m_bCanConstruct;
	mutable CRITICAL_SECTION	m_cCanConstructCacheSection;
	mutable CRITICAL_SECTION	m_cCanTrainCacheSection;
	mutable CRITICAL_SECTION	m_cUpgradeCacheSection;
	mutable CRITICAL_SECTION	m_cBuildingCommerceChangeSection;

	void beginDeferredFreeTradeRegionBuildingProcessing();
	void endDeferredFreeTradeRegionBuildingProcessing();
	volatile int*				m_paiDeferredNumFreeTradeRegionBuilding;
	bool						m_bFreeTradeBuildingProcessingDeferred;
public:
	void populateCanTrainCache(bool bUnconditional = true) const;
	void clearCanTrainCache(bool fullClear) const;
protected:
	void invalidateCachedCanTrainForUnit(UnitCombatTypes eUnitCombat) const;
#endif
};

//	Helper class for doTurn pipeline
class CvCityTurnPipelineWorkItem : public CvPipelineWorkItem
{
public:
	CvCityTurnPipelineWorkItem(CvCity* pCity) : m_proposedOrderType( NO_ORDER ),
		// f1rpo: Better initialize these
		m_proposedOrderInstanceId(-1), m_proposedData2(-1)
	{
		SetRandSeed(GC.getGameINLINE().getSorenRand().getSeed() + pCity->getID());
		m_pCity = pCity;

		pCity->m_workItem = this;
		m_bAllowNoProduction = false;
		m_bHasIncludedTurnProduction = false;
	}
	virtual ~CvCityTurnPipelineWorkItem()
	{
		m_pCity->m_workItem = NULL;
	}
	virtual int	GetPriority(void) const
	{
		return -m_pCity->getID();
	}
	virtual const CvWString	GetName(void) const
	{
		return m_pCity->getName();
	}

	CvCity*	getCity(void) const
	{
		return m_pCity;
	}

	void	getProposedProduction(OrderTypes& orderType, int& instanceId, int& iData2) const
	{
		orderType = m_proposedOrderType;
		instanceId = m_proposedOrderInstanceId;
		iData2 = m_proposedData2;
	}

	void	setProposedProduction(OrderTypes orderType, int instanceId, int iData2)
	{
		m_proposedOrderType = orderType;
		m_proposedOrderInstanceId = instanceId;
		m_proposedData2 = iData2;
	}

public:
	bool		m_bAllowNoProduction;
	bool		m_bHasIncludedTurnProduction;
//	static bool	m_bHasProcessedDeferalOfVoteSources;
//	static bool	m_bHasProcessedUndeferalOfVoteSources;
private:
	CvCity*		m_pCity;
	OrderTypes	m_proposedOrderType;
	int			m_proposedOrderInstanceId;
	int			m_proposedData2;
};

#endif
