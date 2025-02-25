#pragma once

// selectionGroup.h

#ifndef CIV4_GROUP_H
#define CIV4_GROUP_H

//#include "CvStructs.h"
#include "LinkedList.h"
#include "CvPathGenerator.h"
#include "CvUnit.h"
#include "CvCity.h"
#include "ByteBuffer.h"

class CvPlot;
class CvArea;
class FAStarNode;

typedef struct
{
	int		iCost;
	int		iBestMoveCost;
	int		iWorstMoveCost;
	int		iToPlotNodeCost;
#ifdef _DEBUG
	CvPlot*	pFromPlot;
	CvPlot*	pToPlot;
#endif
} edgeCosts;

class CvSelectionGroup
{

public:

	CvSelectionGroup();
	virtual ~CvSelectionGroup();

	void* operator new(size_t size) { return GC.getGameINLINE().getAllocator().allocate(size); }
	void operator delete(void* instance) { GC.getGameINLINE().getAllocator().deAllocate(instance); }
	void* operator new[](size_t size) { return GC.getGameINLINE().getAllocator().allocate(size); }
	void operator delete[](void* instance) { GC.getGameINLINE().getAllocator().deAllocate(instance); }

	DllExport void init(int iID, PlayerTypes eOwner);
	DllExport void uninit();			
	DllExport void reset(int iID = 0, PlayerTypes eOwner = NO_PLAYER, bool bConstructorCall = false);

	void kill();

	void doTurn();

	bool showMoves() const;

	void updateTimers();
	bool doDelayedDeath();

	void playActionSound();

	bool pushMissionInternal(MissionTypes eMission, int iData1 = -1, int iData2 = -1, int iFlags = 0, bool bAppend = false, bool bManual = false, MissionAITypes eMissionAI = NO_MISSIONAI, CvPlot* pMissionAIPlot = NULL, CvUnit* pMissionAIUnit = NULL);		// Exposed to Python
	DllExport void pushMission(MissionTypes eMission, int iData1 = -1, int iData2 = -1, int iFlags = 0, bool bAppend = false, bool bManual = false, MissionAITypes eMissionAI = NO_MISSIONAI, CvPlot* pMissionAIPlot = NULL, CvUnit* pMissionAIUnit = NULL);		// Exposed to Python
	void popMission();																																										// Exposed to Python
	DllExport void autoMission();
	void updateMission();
	DllExport CvPlot* lastMissionPlot();																																					// Exposed to Python

	DllExport bool canStartMission(int iMission, int iData1, int iData2, CvPlot* pPlot = NULL, bool bTestVisible = false, bool bUseCache = false);		// Exposed to Python
	bool startMission();
	bool continueMission(int iSteps = 0);
	bool checkMoveSafety(int iX, int iY, int iFlags);

	DllExport bool canDoInterfaceMode(InterfaceModeTypes eInterfaceMode);													// Exposed to Python
	DllExport bool canDoInterfaceModeAt(InterfaceModeTypes eInterfaceMode, CvPlot* pPlot);				// Exposed to Python

	DllExport bool canDoCommand(CommandTypes eCommand, int iData1, int iData2, bool bTestVisible = false, bool bUseCache = false);		// Exposed to Python
	bool canEverDoCommand(CommandTypes eCommand, int iData1, int iData2, bool bTestVisible, bool bUseCache);
	void setupActionCache();
	
	void finishMoves();

	bool isHuman() const;																																											// Exposed to Python
	DllExport bool isBusy();
	bool isCargoBusy();
	int baseMoves();																																										// Exposed to Python 
	bool isWaiting() const;																																							// Exposed to Python
	bool isFull();																																											// Exposed to Python
	bool hasCargo();																																										// Exposed to Python
	int getCargo() const;
	DllExport bool canAllMove();																																				// Exposed to Python
	bool canAnyMove(bool bValidate = false);																																									// Exposed to Python
	bool hasMoved();																																										// Exposed to Python
	bool canEnterTerritory(TeamTypes eTeam, bool bIgnoreRightOfPassage = false) const;									// Exposed to Python
	bool canEnterArea(TeamTypes eTeam, const CvArea* pArea, bool bIgnoreRightOfPassage = false) const;									// Exposed to Python

	DllExport bool canMoveInto(CvPlot* pPlot, bool bAttack = false);																		// Exposed to Python
	bool canMoveIntoWithWar(CvPlot* pPlot, bool bAttack = false, bool bDeclareWar = false);

	DllExport bool canMoveOrAttackInto(CvPlot* pPlot, bool bDeclareWar = false);												// Exposed to Python
	bool canMoveThrough(CvPlot* pPlot, bool bDeclareWar = false);																																	// Exposed to Python
	bool canFight();																																										// Exposed to Python 
	bool canDefend();																																										// Exposed to Python

	bool hasBombardCapability(void) const;
	bool hasCollateralDamage(void) const;
	bool canPillage(const CvPlot* pPlot);
	bool canReduceCityDefense(const CvPlot* pPlot, bool bIgnoreMadeAttack = false);
	bool canVolleyAt(const CvPlot* pFromPlot, int iX, int iY);
	bool visibilityRange();
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      08/19/09                                jdog5000      */
/*                                                                                              */
/* General AI                                                                                   */
/************************************************************************************************/
	int getBombardTurns( CvCity* pCity );
	bool isHasPathToAreaPlayerCity( PlayerTypes ePlayer, int iFlags = 0 );
	bool isHasPathToAreaEnemyCity( bool bIgnoreMinors = true, int iFlags = 0 );
	bool isStranded();
	void invalidateIsStrandedCache();
	bool calculateIsStranded();
	bool canMoveAllTerrain() const;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

	void unloadAll();
	bool alwaysInvisible() const;																																							// Exposed to Python
	bool isInvisible(TeamTypes eTeam) const;																								// Exposed to Python
	int countNumUnitAIType(UnitAITypes eUnitAI);																												// Exposed to Python
	bool hasWorker();																																										// Exposed to Python
	bool IsSelected();
	DllExport void NotifyEntity(MissionTypes eMission);
	void airCircle(bool bStart);
	void setBlockading(bool bStart);

	int getX() const;
	int getY() const;
	bool at(int iX, int iY) const;																																								// Exposed to Python
	bool atPlot(const CvPlot* pPlot) const;																																				// Exposed to Python
	CvPlot* plot() const;																																								// Exposed to Python
	DllExport CvPlot* plotExternal() const;																																								// Exposed to Python
	int getArea() const;
	CvArea* area() const;																																													// Exposed to Python
	DomainTypes getDomainType() const;

	RouteTypes getBestBuildRoute(CvPlot* pPlot, BuildTypes* peBestBuild = NULL) const;	// Exposed to Python

	bool groupDeclareWar(CvPlot* pPlot, bool bForce = false);
	bool groupAttack(int iX, int iY, int iFlags, bool& bFailedAlreadyFighting);
	void groupMove(CvPlot* pPlot, bool bCombat, CvUnit* pCombatUnit = NULL, bool bEndMove = false);
	bool groupPathTo(int iX, int iY, int iFlags);
	bool groupRoadTo(int iX, int iY, int iFlags);
	bool groupBuild(BuildTypes eBuild);
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      04/18/10                                jdog5000      */
/*                                                                                              */
/* General AI                                                                                   */
/************************************************************************************************/
	void setTransportUnit(CvUnit* pTransportUnit, CvSelectionGroup** pOtherGroup = NULL);
	void setRemoteTransportUnit(CvUnit* pTransportUnit);
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

	bool isAmphibPlot(const CvPlot* pPlot) const;																																		// Exposed to Python
	bool groupAmphibMove(CvPlot* pPlot, int iFlags);

	DllExport bool readyToSelect(bool bAny = false);																										// Exposed to Python
	bool readyToMove(bool bAny = false, bool bValidate = false);																																// Exposed to Python
	bool readyToAuto();																																									// Exposed to Python 

	int getID() const;																																												// Exposed to Python
	void setID(int iID);																			

	int getMissionTimer() const;
	void setMissionTimer(int iNewValue);
	void changeMissionTimer(int iChange);
	void updateMissionTimer(int iSteps = 0);

	bool isForceUpdate();
	void setForceUpdate(bool bNewValue);

	bool isMidMove() const { return m_bIsMidMove; }

	DllExport PlayerTypes getOwner() const;																															// Exposed to Python
#ifdef _USRDLL
	inline PlayerTypes getOwnerINLINE() const
	{
		return m_eOwner;
	}
#endif
	TeamTypes getTeam() const;																																					// Exposed to Python

	ActivityTypes getActivityType() const;																															// Exposed to Python
	void setActivityType(ActivityTypes eNewValue);																											// Exposed to Python

	AutomateTypes getAutomateType() const;																																		// Exposed to Python
	bool isAutomated();																																									// Exposed to Python
	void setAutomateType(AutomateTypes eNewValue);																											// Exposed to Python

#ifdef USE_OLD_PATH_GENERATOR
	FAStarNode* getPathLastNode() const;
#endif
	CvPlot* getPathFirstPlot() const;																																		// Exposed to Python
	CvPlot* getPathEndTurnPlot() const;																														// Exposed to Python
	CvPath&	getPath() const;
	static CvPathGenerator* getPathGenerator();
	bool generatePath( const CvPlot* pFromPlot, const CvPlot* pToPlot, int iFlags = 0, bool bReuse = false, int* piPathTurns = NULL, int iMaxPathLen = MAX_INT, int iOptimizationLimit = -1) const;	// Exposed to Python
	void resetPath();	
	
	bool canPathDirectlyToInternal(CvPlot* pFromPlot, CvPlot* pToPlot, int movesRemaining);
	bool canPathDirectlyTo(CvPlot* pFromPlot, CvPlot* pToPlot);

	int movesRemainingAfterMovingTo(int iStartMoves, CvPlot* pFromPlot, CvPlot* pToPlot) const;
	int movesLeft(void) const;

	// Exposed to Python

	DllExport void clearUnits();
	DllExport bool addUnit(CvUnit* pUnit, bool bMinimalChange);
	void removeUnit(CvUnit* pUnit);
	void mergeIntoGroup(CvSelectionGroup* pSelectionGroup);
	bool containsUnit(const CvUnit* pUnit) const;
	CvSelectionGroup* splitGroup(int iSplitSize, CvUnit* pNewHeadUnit = NULL, CvSelectionGroup** ppOtherGroup = NULL);

	DllExport CLLNode<IDInfo>* deleteUnitNode(CLLNode<IDInfo>* pNode);
	DllExport CLLNode<IDInfo>* nextUnitNode(CLLNode<IDInfo>* pNode) const;
	DllExport int getNumUnits() const;																												// Exposed to Python
	DllExport int getUnitIndex(CvUnit* pUnit, int maxIndex = -1) const;
	DllExport CLLNode<IDInfo>* headUnitNode() const;
	DllExport CvUnit* getHeadUnit() const;
	DllExport CvUnit* getUnitAt(int index) const;
	UnitAITypes getHeadUnitAI() const;
	PlayerTypes getHeadOwner() const;
	TeamTypes getHeadTeam() const;

	void clearMissionQueue();																																	// Exposed to Python
	DllExport int getLengthMissionQueue() const;																											// Exposed to Python
	MissionData* getMissionFromQueue(int iIndex) const;																							// Exposed to Python
	bool insertAtEndMissionQueue(MissionData mission, bool bStart = true);
	CLLNode<MissionData>* deleteMissionQueueNode(CLLNode<MissionData>* pNode);
	DllExport CLLNode<MissionData>* nextMissionQueueNode(CLLNode<MissionData>* pNode) const;
	CLLNode<MissionData>* prevMissionQueueNode(CLLNode<MissionData>* pNode) const;
	DllExport CLLNode<MissionData>* headMissionQueueNodeExternal() const;
	CLLNode<MissionData>* headMissionQueueNode() const;
	CLLNode<MissionData>* tailMissionQueueNode() const;
	int getMissionType(int iNode) const;																														// Exposed to Python
	int getMissionData1(int iNode) const;																														// Exposed to Python
	int getMissionData2(int iNode) const;																														// Exposed to Python

// BUG - All Units Actions - start
	bool allMatch(UnitTypes eUnit) const;
// BUG - All Units Actions - end

	// for serialization
	virtual void read(FDataStreamBase* pStream);
	virtual void write(FDataStreamBase* pStream);

	//Afforess - MP Sync
	void resync(bool bWrite, ByteBuffer* pBuffer);

	virtual void AI_init() = 0;
	virtual void AI_reset() = 0;
	virtual void AI_separate() = 0;
	virtual bool AI_update() = 0;
	virtual int AI_attackOdds(const CvPlot* pPlot, bool bPotentialEnemy, bool bForce = false, bool* bWin = NULL, int iTheshold = -1) const = 0;
	virtual CvUnit* AI_getBestGroupAttacker(const CvPlot* pPlot, bool bPotentialEnemy, int& iUnitOdds, bool bForce = false, bool bNoBlitz = false, CvUnit** pDefender = NULL) const = 0;
	virtual CvUnit* AI_getBestGroupSacrifice(const CvPlot* pPlot, bool bPotentialEnemy, bool bForce = false, bool bNoBlitz = false) const = 0;
	virtual int AI_compareStacks(const CvPlot* pPlot, bool bPotentialEnemy, bool bCheckCanAttack = false, bool bCheckCanMove = false, int iRange = 0) const = 0;
	virtual int AI_sumStrength(const CvPlot* pAttackedPlot = NULL, DomainTypes eDomainType = NO_DOMAIN, bool bCheckCanAttack = false, bool bCheckCanMove = false) const = 0;
	virtual void AI_queueGroupAttack(int iX, int iY) = 0;
	virtual void AI_cancelGroupAttack() = 0;
	virtual bool AI_isGroupAttack() = 0;

	virtual bool AI_isControlled() = 0;
	virtual bool AI_isDeclareWar(const CvPlot* pPlot = NULL) = 0;
	virtual CvPlot* AI_getMissionAIPlot() = 0;
	virtual bool AI_isForceSeparate() = 0;
	virtual void AI_makeForceSeparate() = 0;
	virtual MissionAITypes AI_getMissionAIType() = 0;
	virtual void AI_setMissionAI(MissionAITypes eNewMissionAI, CvPlot* pNewPlot, CvUnit* pNewUnit) = 0;
	virtual void AI_noteSizeChange(int iChange) = 0;
	virtual CvUnit* AI_getMissionAIUnit() = 0;
	virtual CvUnit* AI_ejectBestDefender(CvPlot* pTargetPlot, bool allowAllDefenders = false) = 0;
	virtual bool AI_hasBeneficialPropertyEffectForCity(CvCity* pCity) const = 0;
	virtual CvUnit* AI_ejectBestPropertyManipulator(CvCity* pTargetCity) = 0;
	virtual void AI_separateNonAI(UnitAITypes eUnitAI) = 0;
	virtual void AI_separateAI(UnitAITypes eUnitAI) = 0;
	virtual bool AI_isCityGarrison(const CvCity* pCity) const = 0;
	virtual void AI_setAsGarrison(const CvCity* pCity) = 0;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      06/02/09                                jdog5000      */
/*                                                                                              */
/* General AI                                                                                   */
/************************************************************************************************/
	virtual void AI_separateImpassable() = 0;
	virtual void AI_separateEmptyTransports() = 0;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/
	virtual bool AI_isFull() = 0;
	virtual int AI_getGenericValueTimes100(UnitValueFlags eFlags) const = 0;
/************************************************************************************************/
/* DCM                                     04/19/09                                Johny Smith  */
/************************************************************************************************/
	// Dale - SA: Stack Attack START
	bool groupStackAttack(int iX, int iY, int iFlags, bool& bFailedAlreadyFighting);
	// Dale - SA: Stack Attack END
/************************************************************************************************/
/* DCM                                     END                                                  */
/************************************************************************************************/
	int defensiveModifierAtPlot(CvPlot* pPlot) const;
	bool meetsUnitSelectionCriteria(CvUnitSelectionCriteria* criteria) const;
	int getStrength() const;
	bool hasCommander() const;
	bool hasUnitOfAI(UnitAITypes eUnitAI) const;
	int	getWorstDamage(void) const;
	void validateLocations(bool bFixup = false) const;
	bool findNewLeader(UnitAITypes eAIType);

protected:
	// WARNING: adding to this class will cause the civ4 exe to crash

	int m_iID;
	int m_iMissionTimer;

	bool m_bForceUpdate;
	bool m_bIsMidMove;

	PlayerTypes m_eOwner;
	ActivityTypes m_eActivityType;
	//AutomateTypes m_eAutomateType;
	/*	<f1rpo> (from AdvCiv; advc.003k) Pointer to additional data members.
		Changing the size of this class is not safe b/c the EXE calls the ctor
		of the derived CvSelectionGroupAI class. The BBAI/ BUG additions seem
		to have worked out OK, but a few byte more would've caused problems
		(see Koshling's comment below). */
	class Data
	{
		friend CvSelectionGroup;
		AutomateTypes eAutomateType;
		// BETTER_BTS_AI_MOD, 08/19/09, jdog5000: General AI
		mutable bool bIsStrandedCache;
		mutable bool bIsStrandedCacheValid;
		// BETTER_BTS_AI_MOD: END
		// BUG - Safe Move - start
		bool bLastPathPlotChecked;
		bool bLastPlotVisible;
		bool bLastPlotRevealed;
		// BUG - Safe Move - end
	};
	Data* m; // dial m for members
	// </f1rpo>
	CLinkList<IDInfo> m_units;

	CLinkList<MissionData> m_missionQueue;
	std::vector<CvUnit *> m_aDifferentUnitCache;
	bool m_bIsBusyCache;

// BUG - Safe Move - start
	void checkLastPathPlot(CvPlot* pPlot);
	void clearLastPathPlot();
	bool isLastPathPlotChecked() const;
	bool isLastPathPlotVisible() const;
	bool isLastPathPlotRevealed() const;
// BUG - Safe Move - end

	bool activateHeadMission();
	void deactivateHeadMission();
	
	bool sentryAlert() const;

// BUG - Sentry Actions - start
	//#ifdef _MOD_SENTRY // f1rpo: always enable
	bool sentryAlertSameDomainType() const;
	//#endif
// BUG - Sentry Actions - end

//	KOSHLING Mod - add path validity results cache
public:
	//	These have to be static due to some assumptions the game engine seems to make about
	//	this class which prsumably relates to the comment earlier that adding to this class causes
	//	a crash in the main engine.  This is a bit untidy, but essentially fine due to the
	//	single threaded nature of the application and the fact that cache validity is only
	//	required across a single path generation call, which cannot interleave
	static CvSelectionGroup* m_pCachedMovementGroup;
	static std::map<int,edgeCosts>* m_pCachedNonEndTurnEdgeCosts;
	static std::map<int,edgeCosts>* m_pCachedEndTurnEdgeCosts;
	static CvPathGenerator*	m_generator;

	static void setGroupToCacheFor(CvSelectionGroup* group);
	bool HaveCachedPathEdgeCosts(CvPlot* pFromPlot, CvPlot* pToPlot, bool bIsEndTurnElement, int& iResult, int& iBestMoveCost, int& iWorstMoveCost, int &iToPlotNodeCost);
	void CachePathEdgeCosts(CvPlot* pFromPlot, CvPlot* pToPlot, bool bIsEndTurnElement, int iCost, int iBestMoveCost, int iWorstMoveCost, int iToPlotNodeCost);
};

/*  f1rpo: If this fails, then you've probably added a data member (directly)
	to CvSelectionGroup. */
BOOST_STATIC_ASSERT(sizeof(CvSelectionGroup) == 80);

#endif
