// team.cpp

#include "CvGameCoreDLL.h"
#include "CvPlayerAI.h"
#include "CvDefines.h"
#include "CvGameCoreUtils.h"
#include "cvGlobals.h"
#include "CvTeamAI.h"
#include "CvPlayerAI.h"
#include "CvGameAI.h"
#include "CvMap.h"
#include "CvPlot.h"
#include "CvTeam.h"
#include "CvDLLInterfaceIFaceBase.h"
#include "CvEventReporter.h"
#include "CvDLLEngineIFaceBase.h"
#include "CvArtFileMgr.h"
#include "CvDiploParameters.h"
#include "CvInfos.h"
#include "CvPopupInfo.h"
#include "CvDLLPythonIFaceBase.h"
#include "CyArgsList.h"
#include "FProfiler.h"
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      10/02/09                                jdog5000      */
/*                                                                                              */
/* AI logging                                                                                   */
/************************************************************************************************/
#include "BetterBTSAI.h"
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/
/************************************************************************************************/
/* REVOLUTION_MOD                         10/22/08                                jdog5000      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
#include "CvInitCore.h"
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/
int AGBuilt = 0; //code added by 45� for Ascension Gate & Mastery Victory
// Public Functions...
#pragma warning( disable : 4355 )
CvTeam::CvTeam() : m_GameObject(this),
m_Properties(this)
{
	m_aiStolenVisibilityTimer = new int[MAX_TEAMS];
	m_aiWarWearinessTimes100 = new int[MAX_TEAMS];
	m_aiTechShareCount = new int[MAX_TEAMS];
	m_aiCommerceFlexibleCount = new int[NUM_COMMERCE_TYPES];
	m_aiExtraMoves = new int[NUM_DOMAIN_TYPES];

	m_aiEspionagePointsAgainstTeam = new int[MAX_TEAMS];
	m_aiCounterespionageTurnsLeftAgainstTeam = new int[MAX_TEAMS];
	m_aiCounterespionageModAgainstTeam = new int[MAX_TEAMS];

	m_abAtWar = new bool[MAX_TEAMS];
	m_abHasMet = new bool[MAX_TEAMS];
	m_abHasSeen = new bool[MAX_TEAMS]; // K-Mod
	m_abPermanentWarPeace = new bool[MAX_TEAMS];
	m_abOpenBorders = new bool[MAX_TEAMS];
	m_abDefensivePact = new bool[MAX_TEAMS];
	m_abForcePeace = new bool[MAX_TEAMS];
	m_abVassal = new bool[MAX_TEAMS];
	m_abCanLaunch = NULL;

/************************************************************************************************/
/* REVOLUTION_MOD                         01/01/08                                jdog5000      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	m_abIsRebelAgainst = new bool[MAX_TEAMS];
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/
	m_paiRouteChange = NULL;
	m_paiProjectCount = NULL;
	m_paiProjectDefaultArtTypes = NULL;
	m_pavProjectArtTypes = NULL;
	m_paiProjectMaking = NULL;
	m_paiUnitClassCount = NULL;
	m_paiBuildingClassCount = NULL;
	m_paiObsoleteBuildingCount = NULL;
	m_paiResearchProgress = NULL;
	m_paiTechCount = NULL;
	m_paiTerrainTradeCount = NULL;
	m_aiVictoryCountdown = NULL;
	m_aiForceTeamVoteEligibilityCount = NULL;

	m_pabHasTech = NULL;
	m_pabNoTradeTech = NULL;

	m_ppaaiImprovementYieldChange = NULL;
	
/************************************************************************************************/
/* Afforess	                  Start		 03/15/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	m_ppiBuildingCommerceChange = NULL;
	m_ppiBuildingYieldChange = NULL;
	m_ppiBuildingSpecialistChange = NULL;
	m_ppiBuildingCommerceModifier = NULL;
	m_ppiBuildingYieldModifier = NULL;
	m_paiTechExtraBuildingHappiness = NULL;
	m_paiTechExtraBuildingHealth = NULL;
	m_abEmbassy = new bool[MAX_TEAMS];
	m_abLimitedBorders = new bool[MAX_TEAMS];
	m_abFreeTrade = new bool[MAX_TEAMS];
	m_paiFreeSpecialistCount = NULL;
	m_paiUnitClassStrengthChange = NULL;
	m_abHasEverMet = new bool[MAX_TEAMS];
	m_aiFailedContactChecks = new int[MAX_TEAMS];
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/



	reset((TeamTypes)0, true);
}


CvTeam::~CvTeam()
{
	uninit();

	SAFE_DELETE_ARRAY(m_aiStolenVisibilityTimer);
	SAFE_DELETE_ARRAY(m_aiWarWearinessTimes100);
	SAFE_DELETE_ARRAY(m_aiTechShareCount);
	SAFE_DELETE_ARRAY(m_aiCommerceFlexibleCount);
	SAFE_DELETE_ARRAY(m_aiExtraMoves);
	SAFE_DELETE_ARRAY(m_aiEspionagePointsAgainstTeam);
	SAFE_DELETE_ARRAY(m_aiCounterespionageTurnsLeftAgainstTeam);
	SAFE_DELETE_ARRAY(m_aiCounterespionageModAgainstTeam);
	SAFE_DELETE_ARRAY(m_abAtWar);
	SAFE_DELETE_ARRAY(m_abHasMet);
	SAFE_DELETE_ARRAY(m_abHasSeen); // K-Mod
	SAFE_DELETE_ARRAY(m_abPermanentWarPeace);
	SAFE_DELETE_ARRAY(m_abOpenBorders);
	SAFE_DELETE_ARRAY(m_abDefensivePact);
	SAFE_DELETE_ARRAY(m_abForcePeace);
	SAFE_DELETE_ARRAY(m_abVassal);
/************************************************************************************************/
/* REVOLUTION_MOD                         01/01/08                                jdog5000      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	SAFE_DELETE_ARRAY(m_abIsRebelAgainst);
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/
/************************************************************************************************/
/* Afforess	                  Start		 03/15/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	SAFE_DELETE_ARRAY(m_abEmbassy);
	SAFE_DELETE_ARRAY(m_abLimitedBorders);
	SAFE_DELETE_ARRAY(m_abFreeTrade);
	SAFE_DELETE_ARRAY(m_abHasEverMet); //Rhye	
	SAFE_DELETE_ARRAY(m_aiFailedContactChecks);
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/

}

void CvTeam::init(TeamTypes eID)
{
	init(eID, true);
}

void CvTeam::init(TeamTypes eID, bool bDeclareWar)
{
	//--------------------------------
	// Init saved data
	reset(eID);

	//--------------------------------
	// Init non-saved data

	//--------------------------------
	// Init other game data
	AI_init();

/************************************************************************************************/
/* REVOLUTION_MOD                         05/30/08                                jdog5000      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	if( GC.getGame().isFinalInitialized() && bDeclareWar)
	{
		//logMsg("   Checking for declarations of war against reset team %d", (int)getID() );
	
		for (int iI = 0; iI < MAX_TEAMS; iI++)
		{
			if( iI != getID() )
			{
				if( GET_TEAM((TeamTypes)iI).isMinorCiv() )
				{
					GET_TEAM((TeamTypes)iI).declareWar(getID(), false, WARPLAN_LIMITED);
					GET_TEAM((TeamTypes)iI).setHasMet(getID(),false);
					setHasMet((TeamTypes)iI,false);
					//logMsg("   Minor team %d declared war, using war plan %d", iI, (int)GET_TEAM((TeamTypes)iI).AI_getWarPlan(getID()) );
				}
				if( GET_TEAM((TeamTypes)iI).isBarbarian() )
				{
					GET_TEAM((TeamTypes)iI).declareWar(getID(), false, WARPLAN_LIMITED);
					//logMsg("   Barb team %d declared war, using war plan %d", iI, (int)GET_TEAM((TeamTypes)iI).AI_getWarPlan(getID()) );
				}
			}
		}
	}
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/
}


void CvTeam::uninit()
{
	int iI;

	SAFE_DELETE_ARRAY(m_abCanLaunch);

	SAFE_DELETE_ARRAY(m_paiRouteChange);
	SAFE_DELETE_ARRAY(m_paiProjectCount);
	SAFE_DELETE_ARRAY(m_paiProjectDefaultArtTypes);
	SAFE_DELETE_ARRAY(m_pavProjectArtTypes);
	SAFE_DELETE_ARRAY(m_paiProjectMaking);
	SAFE_DELETE_ARRAY(m_paiUnitClassCount);
	SAFE_DELETE_ARRAY(m_paiBuildingClassCount);
	SAFE_DELETE_ARRAY(m_paiObsoleteBuildingCount);
	SAFE_DELETE_ARRAY(m_paiResearchProgress);
	SAFE_DELETE_ARRAY(m_paiTechCount);
	SAFE_DELETE_ARRAY(m_paiTerrainTradeCount);
	SAFE_DELETE_ARRAY(m_aiVictoryCountdown);
	SAFE_DELETE_ARRAY(m_aiForceTeamVoteEligibilityCount);

	SAFE_DELETE_ARRAY(m_pabHasTech);
	SAFE_DELETE_ARRAY(m_pabNoTradeTech);

	if (m_ppaaiImprovementYieldChange != NULL)
	{
		for (iI = 0; iI < GC.getNumImprovementInfos(); iI++)
		{
			SAFE_DELETE_ARRAY(m_ppaaiImprovementYieldChange[iI]);
		}
		SAFE_DELETE_ARRAY(m_ppaaiImprovementYieldChange);
	}
	
/************************************************************************************************/
/* Afforess	                  Start		 03/15/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	SAFE_DELETE_ARRAY(m_paiTechExtraBuildingHappiness);
	SAFE_DELETE_ARRAY(m_paiTechExtraBuildingHealth);
	SAFE_DELETE_ARRAY(m_paiFreeSpecialistCount);
	SAFE_DELETE_ARRAY(m_paiUnitClassStrengthChange);
	if (m_ppiBuildingCommerceChange != NULL)
	{
		for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
		{
			SAFE_DELETE_ARRAY(m_ppiBuildingCommerceChange[iI]);
		}
		SAFE_DELETE_ARRAY(m_ppiBuildingCommerceChange);
	}

	if (m_ppiBuildingYieldChange != NULL)
	{
		for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
		{
			SAFE_DELETE_ARRAY(m_ppiBuildingYieldChange[iI]);
		}
		SAFE_DELETE_ARRAY(m_ppiBuildingYieldChange);
	}

	if (m_ppiBuildingSpecialistChange != NULL)
	{
		for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
		{
			SAFE_DELETE_ARRAY(m_ppiBuildingSpecialistChange[iI]);
		}
		SAFE_DELETE_ARRAY(m_ppiBuildingSpecialistChange);
	}

	if (m_ppiBuildingCommerceModifier != NULL)
	{
		for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
		{
			SAFE_DELETE_ARRAY(m_ppiBuildingCommerceModifier[iI]);
		}
		SAFE_DELETE_ARRAY(m_ppiBuildingCommerceModifier);
	}

	if (m_ppiBuildingYieldModifier != NULL)
	{
		for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
		{
			SAFE_DELETE_ARRAY(m_ppiBuildingYieldModifier[iI]);
		}
		SAFE_DELETE_ARRAY(m_ppiBuildingYieldModifier);
	}

/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/

}


// FUNCTION: reset()
// Initializes data members that are serialized.
void CvTeam::reset(TeamTypes eID, bool bConstructorCall)
{
	int iI, iJ;

	//--------------------------------
	// Uninit class
	uninit();

	m_iNumMembers = 0;
	m_iAliveCount = 0;
	m_iEverAliveCount = 0;
	m_iNumCities = 0;
	m_iTotalPopulation = 0;
	m_iTotalLand = 0;
	m_iNukeInterception = 0;
	m_iExtraWaterSeeFromCount = 0;
	m_iMapTradingCount = 0;
	m_iTechTradingCount = 0;
	m_iGoldTradingCount = 0;
/************************************************************************************************/
/* Afforess	                  Start		 03/8/10                                                */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	m_iCanPassPeaksCount = 0;
	m_iMoveFastPeaksCount = 0;
	m_iCanFoundOnPeaksCount = 0;
	m_iRebaseAnywhereCount = 0;
// Movement Limits by 45deg - START
	m_iExtendMovementLimitsCount = 0;
	m_iRemoveMovementLimitsCount = 0;	
// Movement Limits by 45deg - END
	m_iForeignTradeModifier = 0;
	m_iTradeModifier = 0;
	m_iTradeMissionModifier = 0;
	m_iCorporationRevenueModifier = 0;
	m_iCorporationMaintenanceModifier = 0;
	m_iEmbassyTradingCount = 0;
	m_iLimitedBordersTradingCount = 0;
	m_iCanFarmDesertCount = 0;
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
	m_iOpenBordersTradingCount = 0;
	m_iDefensivePactTradingCount = 0;
	m_iPermanentAllianceTradingCount = 0;
	m_iVassalTradingCount = 0;
	m_iBridgeBuildingCount = 0;
	m_iIrrigationCount = 0;
	m_iIgnoreIrrigationCount = 0;
	m_iWaterWorkCount = 0;
	m_iVassalPower = 0;
	m_iMasterPower = 0;
	m_iEnemyWarWearinessModifier = 0;
	m_iRiverTradeCount = 0;
	m_iEspionagePointsEver = 0;

	m_bMapCentering = false;
	m_bCapitulated = false;

	m_eID = eID;

	for (iI = 0; iI < MAX_TEAMS; iI++)
	{
		m_aiStolenVisibilityTimer[iI] = 0;
		m_aiWarWearinessTimes100[iI] = 0;
		m_aiTechShareCount[iI] = 0;
		m_aiEspionagePointsAgainstTeam[iI] = 0;
		m_aiCounterespionageTurnsLeftAgainstTeam[iI] = 0;
		m_aiCounterespionageModAgainstTeam[iI] = 0;
		m_abHasMet[iI] = false;
		m_abHasSeen[iI] = false; // K-Mod
		m_abAtWar[iI] = false;
		m_abPermanentWarPeace[iI] = false;
		m_abOpenBorders[iI] = false;
/************************************************************************************************/
/* Afforess	                  Start		 03/8/10                                                */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
		m_abLimitedBorders[iI] = false;
		m_abFreeTrade[iI] = false;
		m_abEmbassy[iI] = false;
		m_abHasEverMet[iI] = false; //Rhye		
		m_aiFailedContactChecks[iI] = 0;
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
		m_abDefensivePact[iI] = false;
		m_abForcePeace[iI] = false;
		m_abVassal[iI] = false;
/************************************************************************************************/
/* REVOLUTION_MOD                         01/01/08                                jdog5000      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
		m_abIsRebelAgainst[iI] = false;
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/

		if (!bConstructorCall && getID() != NO_TEAM)
		{
			CvTeam& kLoopTeam = GET_TEAM((TeamTypes) iI);
			kLoopTeam.m_aiStolenVisibilityTimer[getID()] = 0;
			kLoopTeam.m_aiWarWearinessTimes100[getID()] = 0;
			kLoopTeam.m_aiTechShareCount[getID()] = 0;
			kLoopTeam.m_aiEspionagePointsAgainstTeam[getID()] = 0;
			kLoopTeam.m_aiCounterespionageTurnsLeftAgainstTeam[getID()] = 0;
			kLoopTeam.m_aiCounterespionageModAgainstTeam[getID()] = 0;
			kLoopTeam.m_abHasMet[getID()] = false;
			kLoopTeam.m_abHasSeen[getID()] = false; // K-Mod
			kLoopTeam.m_abAtWar[getID()] = false;
			kLoopTeam.m_abPermanentWarPeace[getID()] = false;
			kLoopTeam.m_abOpenBorders[getID()] = false;
			
/************************************************************************************************/
/* Afforess	                  Start		 03/8/10                                                */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
			kLoopTeam.m_abEmbassy[getID()] = false;
			kLoopTeam.m_abHasEverMet[getID()] = false; //Rhye		
			kLoopTeam.m_aiFailedContactChecks[getID()] = 0;
			kLoopTeam.m_abLimitedBorders[getID()] = false;
			kLoopTeam.m_abFreeTrade[getID()] = false;
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
			kLoopTeam.m_abDefensivePact[getID()] = false;
			kLoopTeam.m_abForcePeace[getID()] = false;
			kLoopTeam.m_abVassal[getID()] = false;
		}
	}

	for (iI = 0; iI < NUM_COMMERCE_TYPES; iI++)
	{
		m_aiCommerceFlexibleCount[iI] = 0;
	}

	for (iI = 0; iI < NUM_DOMAIN_TYPES; iI++)
	{
		m_aiExtraMoves[iI] = 0;
	}

	if (!bConstructorCall)
	{
		FAssertMsg(m_abCanLaunch==NULL, "about to leak memory, CvTeam::m_abCanLaunch");
		m_abCanLaunch = new bool[GC.getNumVictoryInfos()];
		for (iI = 0; iI < GC.getNumVictoryInfos(); iI++)
		{
			m_abCanLaunch[iI] = false;
		}

		FAssertMsg(m_paiRouteChange==NULL, "about to leak memory, CvTeam::m_paiRouteChange");
		m_paiRouteChange = new int[GC.getNumRouteInfos()];
		for (iI = 0; iI < GC.getNumRouteInfos(); iI++)
		{
			m_paiRouteChange[iI] = 0;
		}

		FAssertMsg(m_paiProjectCount==NULL, "about to leak memory, CvPlayer::m_paiProjectCount");
		m_paiProjectCount = new int [GC.getNumProjectInfos()];
		FAssertMsg(m_paiProjectDefaultArtTypes==NULL, "about to leak memory, CvPlayer::m_paiProjectDefaultArtTypes");
		m_paiProjectDefaultArtTypes = new int [GC.getNumProjectInfos()];
		FAssertMsg(m_pavProjectArtTypes==NULL, "about to leak memory, CvPlayer::m_pavProjectArtTypes");
		m_pavProjectArtTypes = new std::vector<int> [GC.getNumProjectInfos()];
		FAssertMsg(m_paiProjectMaking==NULL, "about to leak memory, CvPlayer::m_paiProjectMaking");
		m_paiProjectMaking = new int [GC.getNumProjectInfos()];
		for (iI = 0; iI < GC.getNumProjectInfos(); iI++)
		{
			m_paiProjectCount[iI] = 0;
			m_paiProjectDefaultArtTypes[iI] = 0;
			m_paiProjectMaking[iI] = 0;
		}

		FAssertMsg(m_paiUnitClassCount==NULL, "about to leak memory, CvTeam::m_paiUnitClassCount");
		m_paiUnitClassCount = new int [GC.getNumUnitClassInfos()];
		for (iI = 0; iI < GC.getNumUnitClassInfos(); iI++)
		{
			m_paiUnitClassCount[iI] = 0;
		}

		FAssertMsg(m_paiBuildingClassCount==NULL, "about to leak memory, CvTeam::m_paiBuildingClassCount");
		m_paiBuildingClassCount = new int [GC.getNumBuildingClassInfos()];
		for (iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
		{
			m_paiBuildingClassCount[iI] = 0;
		}

		FAssertMsg(m_paiObsoleteBuildingCount==NULL, "about to leak memory, CvTeam::m_paiObsoleteBuildingCount");
		m_paiObsoleteBuildingCount = new int[GC.getNumBuildingInfos()];
		for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
		{
			m_paiObsoleteBuildingCount[iI] = 0;
		}

		FAssertMsg(m_paiResearchProgress==NULL, "about to leak memory, CvPlayer::m_paiResearchProgress");
		m_paiResearchProgress = new int [GC.getNumTechInfos()];
		FAssertMsg(m_paiTechCount==NULL, "about to leak memory, CvPlayer::m_paiTechCount");
		m_paiTechCount = new int [GC.getNumTechInfos()];
		for (iI = 0; iI < GC.getNumTechInfos(); iI++)
		{
			m_paiResearchProgress[iI] = 0;
			m_paiTechCount[iI] = 0;
		}

		FAssertMsg(m_paiTerrainTradeCount==NULL, "about to leak memory, CvTeam::m_paiTerrainTradeCount");
		m_paiTerrainTradeCount = new int[GC.getNumTerrainInfos()];
		for (iI = 0; iI < GC.getNumTerrainInfos(); iI++)
		{
			m_paiTerrainTradeCount[iI] = 0;
		}

		FAssertMsg(m_aiVictoryCountdown==NULL, "about to leak memory, CvTeam::m_aiVictoryCountdown");
		m_aiVictoryCountdown = new int[GC.getNumVictoryInfos()];
		for (iI = 0; iI < GC.getNumVictoryInfos(); iI++)
		{
			m_aiVictoryCountdown[iI] = -1;
		}
		
		FAssertMsg(m_paiTechExtraBuildingHappiness==NULL, "about to leak memory, m_paiTechExtraBuildingHappiness"); //Afforess
		m_paiTechExtraBuildingHappiness = new int[GC.getNumBuildingInfos()];
		for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
		{
			m_paiTechExtraBuildingHappiness[iI] = 0;
		}
		
		FAssertMsg(m_paiTechExtraBuildingHealth==NULL, "about to leak memory, m_paiTechExtraBuildingHealth");
		m_paiTechExtraBuildingHealth = new int[GC.getNumBuildingInfos()];
		for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
		{
			m_paiTechExtraBuildingHealth[iI] = 0;
		}
		

		FAssertMsg(m_pabHasTech==NULL, "about to leak memory, CvTeam::m_pabHasTech");
		m_pabHasTech = new bool[GC.getNumTechInfos()];
		FAssertMsg(m_pabNoTradeTech==NULL, "about to leak memory, CvTeam::m_pabNoTradeTech");
		m_pabNoTradeTech = new bool[GC.getNumTechInfos()];
		for (iI = 0; iI < GC.getNumTechInfos(); iI++)
		{
			m_pabHasTech[iI] = false;
			m_pabNoTradeTech[iI] = false;
		}

		FAssertMsg(m_ppaaiImprovementYieldChange==NULL, "about to leak memory, CvTeam::m_ppaaiImprovementYieldChange");
		m_ppaaiImprovementYieldChange = new int*[GC.getNumImprovementInfos()];
		for (iI = 0; iI < GC.getNumImprovementInfos(); iI++)
		{
			m_ppaaiImprovementYieldChange[iI] = new int[NUM_YIELD_TYPES];
			for (iJ = 0; iJ < NUM_YIELD_TYPES; iJ++)
			{
				m_ppaaiImprovementYieldChange[iI][iJ] = 0;
			}
		}
		
		FAssertMsg(m_aiForceTeamVoteEligibilityCount==NULL, "about to leak memory, CvTeam::m_aiForceTeamVoteEligibilityCount");
		m_aiForceTeamVoteEligibilityCount = new int[GC.getNumVoteSourceInfos()];
		for (iI = 0; iI < GC.getNumVoteSourceInfos(); iI++)
		{
			m_aiForceTeamVoteEligibilityCount[iI] = 0;
		}

		m_aeRevealedBonuses.clear();

/************************************************************************************************/
/* Afforess	                  Start		 03/8/10                                                */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/

		FAssertMsg(m_ppiBuildingCommerceChange==NULL, "about to leak memory, CvTeam::m_ppiBuildingCommerceChange");
		m_ppiBuildingCommerceChange = new int*[GC.getNumBuildingInfos()];
		for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
		{
			m_ppiBuildingCommerceChange[iI] = new int[NUM_COMMERCE_TYPES];
			for (iJ = 0; iJ < NUM_COMMERCE_TYPES; iJ++)
			{
				m_ppiBuildingCommerceChange[iI][iJ] = 0;
			}
		}

		FAssertMsg(m_ppiBuildingYieldChange==NULL, "about to leak memory, CvTeam::m_ppiBuildingYieldChange");
		m_ppiBuildingYieldChange = new int*[GC.getNumBuildingInfos()];
		for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
		{
			m_ppiBuildingYieldChange[iI] = new int[NUM_YIELD_TYPES];
			for (iJ = 0; iJ < NUM_YIELD_TYPES; iJ++)
			{
				m_ppiBuildingYieldChange[iI][iJ] = 0;
			}
		}

		FAssertMsg(m_ppiBuildingSpecialistChange==NULL, "about to leak memory, CvTeam::m_ppiBuildingSpecialistChange");
		m_ppiBuildingSpecialistChange = new int*[GC.getNumBuildingInfos()];
		for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
		{
			m_ppiBuildingSpecialistChange[iI] = new int[GC.getNumSpecialistInfos()];
			for (iJ = 0; iJ < GC.getNumSpecialistInfos(); iJ++)
			{
				m_ppiBuildingSpecialistChange[iI][iJ] = 0;
			}
		}
		FAssertMsg(m_paiFreeSpecialistCount==NULL, "about to leak memory, CvTeam::m_paiFreeSpecialistCount");
		m_paiFreeSpecialistCount = new int[GC.getNumSpecialistInfos()];
		for (iJ = 0; iJ < GC.getNumSpecialistInfos(); iJ++)
		{
			m_paiFreeSpecialistCount[iJ] = 0;
		}

		FAssertMsg(m_ppiBuildingCommerceModifier==NULL, "about to leak memory, CvTeam::m_ppiBuildingCommerceModifier");
		m_ppiBuildingCommerceModifier = new int*[GC.getNumBuildingInfos()];
		for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
		{
			m_ppiBuildingCommerceModifier[iI] = new int[NUM_COMMERCE_TYPES];
			for (iJ = 0; iJ < NUM_COMMERCE_TYPES; iJ++)
			{
				m_ppiBuildingCommerceModifier[iI][iJ] = 0;
			}
		}

		FAssertMsg(m_ppiBuildingYieldModifier==NULL, "about to leak memory, CvTeam::m_ppiBuildingYieldModifier");
		m_ppiBuildingYieldModifier = new int*[GC.getNumBuildingInfos()];
		for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
		{
			m_ppiBuildingYieldModifier[iI] = new int[NUM_YIELD_TYPES];
			for (iJ = 0; iJ < NUM_YIELD_TYPES; iJ++)
			{
				m_ppiBuildingYieldModifier[iI][iJ] = 0;
			}
		}
		FAssertMsg(m_paiUnitClassStrengthChange==NULL, "about to leak memory, CvTeam::m_paiUnitClassStrengthChange");
		m_paiUnitClassStrengthChange = new int[GC.getNumUnitClassInfos()];
		for (iJ = 0; iJ < GC.getNumUnitClassInfos(); iJ++)
		{
			m_paiUnitClassStrengthChange[iJ] = 0;
		}
		
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
		AI_reset(false);
	}

	m_Properties.clear();
}

/************************************************************************************************/
/* REVOLUTION_MOD                         01/01/08                                jdog5000      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
//
// for logging
//
void CvTeam::logMsg(char* format, ... )
{
	if (GC.isXMLLogging())
	{
		static char buf[2048];
		_vsnprintf( buf, 2048-4, format, (char*)(&format+1) );
		gDLL->logMsg("sdkDbg.log", buf);
	}
}

//
// for clearing data stored in plots and cities for this team
//
void CvTeam::resetPlotAndCityData( )
{
	int iI;
	CvPlot* pLoopPlot;
	CvCity* pLoopCity;
	for (iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
	{
		pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);
		
		pLoopPlot->setRevealedOwner(getID(), NO_PLAYER);
		pLoopPlot->setRevealedImprovementType(getID(), NO_IMPROVEMENT);
		pLoopPlot->setRevealedRouteType(getID(), NO_ROUTE);
		pLoopPlot->setRevealed(getID(), false, false, getID(), true);

		pLoopCity = pLoopPlot->getPlotCity();
		if( pLoopCity != NULL )
		{
			pLoopCity->setRevealed(getID(), false);
			pLoopCity->setEspionageVisibility(getID(), false, true);
		}
	}
}

bool CvTeam::isRebel() const
{
	bool bValid;
	int iI;

	bValid = false;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID() && GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).isRebel())
			{
				bValid = true;
			}
			else
			{
				return false;
			}
		}
	}
	
	return bValid;
}

bool CvTeam::isSingleCityTeam() const
{
	int iCities = 0;
	int iI;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		CvPlayer& kPlayer = GET_PLAYER((PlayerTypes)iI);
		if( (kPlayer.getTeam() == getID()) && kPlayer.isAlive() )
		{
			iCities += GET_PLAYER((PlayerTypes)iI).getNumCities();
		}
		if(iCities > 1)
		{
			return false;
		}
	}

	return true;
}

bool CvTeam::isRebelAgainst( TeamTypes eTeam ) const
{
	if( eTeam >= 0 && eTeam < MAX_TEAMS )
		return m_abIsRebelAgainst[(int)eTeam];

	return false;
}

void CvTeam::setRebelAgainst( TeamTypes eTeam, bool bNewValue )
{
	if( eTeam >= 0 && eTeam < MAX_TEAMS )
		m_abIsRebelAgainst[(int)eTeam] = bNewValue;
}

int CvTeam::countRebelAgainst( ) const
{
	int count = 0;
	for (int iI = 0; iI < MAX_CIV_TEAMS; iI++)
	{
		if( isRebelAgainst( (TeamTypes)iI ) )
			count++;
	}
	return count;
}

int CvTeam::getNumMilitaryUnits( ) const
{
	int count = 0;
	for (int iI = 0; iI < MAX_CIV_PLAYERS; iI++)
	{
		if( GET_PLAYER((PlayerTypes)iI).getTeam() == getID() )
			count += GET_PLAYER((PlayerTypes)iI).getNumMilitaryUnits();
	}
	return count;
}
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/

void CvTeam::addTeam(TeamTypes eTeam)
{
	CLLNode<TradeData>* pNode;
	CvDeal* pLoopDeal;
	CvPlot* pLoopPlot;
	CvWString szBuffer;
	bool bValid;
	int iLoop = 0;
	int iI, iJ;

	FAssert(eTeam != NO_TEAM);
	FAssert(eTeam != getID());

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if ((GET_PLAYER((PlayerTypes)iI).getTeam() != getID()) && (GET_PLAYER((PlayerTypes)iI).getTeam() != eTeam))
			{
				if (GET_TEAM(GET_PLAYER((PlayerTypes)iI).getTeam()).isHasMet(getID()) && GET_TEAM(GET_PLAYER((PlayerTypes)iI).getTeam()).isHasMet(eTeam))
				{
					MEMORY_TRACK_EXEMPT();

					szBuffer = gDLL->getText("TXT_KEY_MISC_PLAYER_PERMANENT_ALLIANCE", getName().GetCString(), GET_TEAM(eTeam).getName().GetCString());
					AddDLLMessage(((PlayerTypes)iI), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_THEIRALLIANCE", MESSAGE_TYPE_MINOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));
				}
			}
		}
	}

	szBuffer = gDLL->getText("TXT_KEY_MISC_PLAYER_PERMANENT_ALLIANCE", getName().GetCString(), GET_TEAM(eTeam).getName().GetCString());
	GC.getGameINLINE().addReplayMessage(REPLAY_MESSAGE_MAJOR_EVENT, getLeaderID(), szBuffer, -1, -1, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));


	for (pLoopDeal = GC.getGameINLINE().firstDeal(&iLoop); pLoopDeal != NULL; pLoopDeal = GC.getGameINLINE().nextDeal(&iLoop))
	{
		if (((GET_PLAYER(pLoopDeal->getFirstPlayer()).getTeam() == getID()) && (GET_PLAYER(pLoopDeal->getSecondPlayer()).getTeam() == eTeam)) ||
			  ((GET_PLAYER(pLoopDeal->getFirstPlayer()).getTeam() == eTeam) && (GET_PLAYER(pLoopDeal->getSecondPlayer()).getTeam() == getID())))
		{
			bValid = true;

			if (pLoopDeal->getFirstTrades() != NULL)
			{
				for (pNode = pLoopDeal->getFirstTrades()->head(); pNode; pNode = pLoopDeal->getFirstTrades()->next(pNode))
				{
					if ((pNode->m_data.m_eItemType == TRADE_OPEN_BORDERS) ||
						  (pNode->m_data.m_eItemType == TRADE_DEFENSIVE_PACT) ||
						  (pNode->m_data.m_eItemType == TRADE_PEACE_TREATY) ||
						  (pNode->m_data.m_eItemType == TRADE_VASSAL) ||
/************************************************************************************************/
/* Afforess	                  Start		 06/16/10                                               */
/*                                                                                              */
/* Advanced Diplomacy                                                                           */
/************************************************************************************************/
						  (pNode->m_data.m_eItemType == TRADE_RITE_OF_PASSAGE) ||
						   (pNode->m_data.m_eItemType == TRADE_FREE_TRADE_ZONE) ||
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
						  (pNode->m_data.m_eItemType == TRADE_SURRENDER))
					{
						bValid = false;
					}
				}
			}

			if (pLoopDeal->getSecondTrades() != NULL)
			{
				for (pNode = pLoopDeal->getSecondTrades()->head(); pNode; pNode = pLoopDeal->getSecondTrades()->next(pNode))
				{
					if ((pNode->m_data.m_eItemType == TRADE_OPEN_BORDERS) ||
						  (pNode->m_data.m_eItemType == TRADE_DEFENSIVE_PACT) ||
/************************************************************************************************/
/* Afforess	                  Start		 06/16/10                                               */
/*                                                                                              */
/* Advanced Diplomacy                                                                           */
/************************************************************************************************/
						  (pNode->m_data.m_eItemType == TRADE_RITE_OF_PASSAGE) ||
						   (pNode->m_data.m_eItemType == TRADE_FREE_TRADE_ZONE) ||
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
						  (pNode->m_data.m_eItemType == TRADE_PEACE_TREATY) ||
						  (pNode->m_data.m_eItemType == TRADE_VASSAL) ||
						  (pNode->m_data.m_eItemType == TRADE_SURRENDER))
					{
						bValid = false;
					}
				}
			}

			if (!bValid)
			{
				pLoopDeal->kill();
			}
		}
	}

	shareItems(eTeam);
	GET_TEAM(eTeam).shareItems(getID());

	for (iI = 0; iI < MAX_TEAMS; iI++)
	{
		if ((iI != getID()) && (iI != eTeam))
		{
			if (GET_TEAM((TeamTypes)iI).isAlive())
			{
				if (GET_TEAM(eTeam).isHasMet((TeamTypes)iI))
				{
					meet(((TeamTypes)iI), false);
				}
				else if (isHasMet((TeamTypes)iI))
				{
					GET_TEAM(eTeam).meet(((TeamTypes)iI), false);
				}
			}
		}
	}

	for (iI = 0; iI < MAX_TEAMS; iI++)
	{
		if ((iI != getID()) && (iI != eTeam))
		{
			if (GET_TEAM((TeamTypes)iI).isAlive())
			{
				if (GET_TEAM(eTeam).isAtWar((TeamTypes)iI))
				{
					declareWar(((TeamTypes)iI), false, GET_TEAM(eTeam).AI_getWarPlan((TeamTypes)iI));
				}
				else if (isAtWar((TeamTypes)iI))
				{
					GET_TEAM(eTeam).declareWar(((TeamTypes)iI), false, AI_getWarPlan((TeamTypes)iI));
				}
			}
		}
	}

	for (iI = 0; iI < MAX_TEAMS; iI++)
	{
		if ((iI != getID()) && (iI != eTeam))
		{
			if (GET_TEAM((TeamTypes)iI).isAlive())
			{
				if (GET_TEAM(eTeam).isPermanentWarPeace((TeamTypes)iI))
				{
					setPermanentWarPeace(((TeamTypes)iI), true);
				}
				else if (isPermanentWarPeace((TeamTypes)iI))
				{
					GET_TEAM(eTeam).setPermanentWarPeace(((TeamTypes)iI), true);
				}
			}
		}
	}

	for (iI = 0; iI < MAX_TEAMS; iI++)
	{
		if ((iI != getID()) && (iI != eTeam))
		{
			if (GET_TEAM((TeamTypes)iI).isAlive())
			{
				if (GET_TEAM(eTeam).isOpenBorders((TeamTypes)iI))
				{
					setOpenBorders(((TeamTypes)iI), true);
					GET_TEAM((TeamTypes)iI).setOpenBorders(getID(), true);
				}
				else if (isOpenBorders((TeamTypes)iI))
				{
					GET_TEAM(eTeam).setOpenBorders(((TeamTypes)iI), true);
					GET_TEAM((TeamTypes)iI).setOpenBorders(eTeam, true);
				}
			}
		}
	}
/************************************************************************************************/
/* Afforess	                  Start		 03/8/10                                                */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	for (iI = 0; iI < MAX_TEAMS; iI++)
	{
		if ((iI != getID()) && (iI != eTeam))
		{
			if (GET_TEAM((TeamTypes)iI).isAlive())
			{
				if (GET_TEAM(eTeam).isLimitedBorders((TeamTypes)iI))
				{
					setLimitedBorders(((TeamTypes)iI), true);
					GET_TEAM((TeamTypes)iI).setLimitedBorders(getID(), true);
				}
				else if (isLimitedBorders((TeamTypes)iI))
				{
					GET_TEAM(eTeam).setLimitedBorders(((TeamTypes)iI), true);
					GET_TEAM((TeamTypes)iI).setLimitedBorders(eTeam, true);
				}
			}
		}
	}
	for (iI = 0; iI < MAX_TEAMS; iI++)
	{
		if ((iI != getID()) && (iI != eTeam))
		{
			if (GET_TEAM((TeamTypes)iI).isAlive())
			{
				if (GET_TEAM(eTeam).isHasEmbassy((TeamTypes)iI))
				{
					setHasEmbassy(((TeamTypes)iI), true);
					GET_TEAM((TeamTypes)iI).setHasEmbassy(getID(), true);
				}
				else if (isDefensivePact((TeamTypes)iI) || isOpenBorders((TeamTypes)iI))
				{
					GET_TEAM(eTeam).setHasEmbassy(((TeamTypes)iI), true);
					GET_TEAM((TeamTypes)iI).setHasEmbassy(eTeam, true);
				}
			}
		}
	}
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
	for (iI = 0; iI < MAX_TEAMS; iI++)
	{
		if ((iI != getID()) && (iI != eTeam))
		{
			if (GET_TEAM((TeamTypes)iI).isAlive())
			{
				if (GET_TEAM(eTeam).isDefensivePact((TeamTypes)iI))
				{
					setDefensivePact(((TeamTypes)iI), true);
					GET_TEAM((TeamTypes)iI).setDefensivePact(getID(), true);
				}
				else if (isDefensivePact((TeamTypes)iI))
				{
					GET_TEAM(eTeam).setDefensivePact(((TeamTypes)iI), true);
					GET_TEAM((TeamTypes)iI).setDefensivePact(eTeam, true);
				}
			}
		}
	}

	for (iI = 0; iI < MAX_TEAMS; iI++)
	{
		if ((iI != getID()) && (iI != eTeam))
		{
			if (GET_TEAM((TeamTypes)iI).isAlive())
			{
				if (GET_TEAM(eTeam).isForcePeace((TeamTypes)iI))
				{
					setForcePeace(((TeamTypes)iI), true);
					GET_TEAM((TeamTypes)iI).setForcePeace(getID(), true);
				}
				else if (isForcePeace((TeamTypes)iI))
				{
					GET_TEAM(eTeam).setForcePeace(((TeamTypes)iI), true);
					GET_TEAM((TeamTypes)iI).setForcePeace(eTeam, true);
				}
			}
		}
	}

	for (iI = 0; iI < MAX_TEAMS; iI++)
	{
		if ((iI != getID()) && (iI != eTeam))
		{
			if (GET_TEAM((TeamTypes)iI).isAlive())
			{
				if (GET_TEAM(eTeam).isVassal((TeamTypes)iI))
				{
					setVassal(((TeamTypes)iI), true, isCapitulated());
				}
				else if (isVassal((TeamTypes)iI))
				{
					GET_TEAM(eTeam).setVassal(((TeamTypes)iI), true, isCapitulated());
				}
			}
		}
	}

	for (iI = 0; iI < MAX_TEAMS; iI++)
	{
		if ((iI != getID()) && (iI != eTeam))
		{
			if (GET_TEAM((TeamTypes)iI).isAlive())
			{
				if (GET_TEAM((TeamTypes)iI).isVassal(eTeam))
				{
					GET_TEAM((TeamTypes)iI).setVassal(getID(), true, GET_TEAM((TeamTypes)iI).isCapitulated());
				}
				else if (GET_TEAM((TeamTypes)iI).isVassal(getID()))
				{
					GET_TEAM((TeamTypes)iI).setVassal(eTeam, true, GET_TEAM((TeamTypes)iI).isCapitulated());
				}
			}
		}
	}

	shareCounters(eTeam);
	GET_TEAM(eTeam).shareCounters(getID());

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).getTeam() == eTeam)
		{
			GET_PLAYER((PlayerTypes)iI).setTeam(getID());
		}
	}

	for (iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
	{
		pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);

		pLoopPlot->changeVisibilityCount(getID(), pLoopPlot->getVisibilityCount(eTeam), NO_INVISIBLE, false);

		for (iJ = 0; iJ < GC.getNumInvisibleInfos(); iJ++)
		{
			pLoopPlot->changeInvisibleVisibilityCount(getID(), ((InvisibleTypes)iJ), pLoopPlot->getInvisibleVisibilityCount(eTeam, ((InvisibleTypes)iJ)));
		}

		if (pLoopPlot->isRevealed(eTeam, false))
		{
			pLoopPlot->setRevealed(getID(), true, false, eTeam, false);
		}
	}

	GC.getGameINLINE().updatePlotGroups();

	for (iI = 0; iI < MAX_TEAMS; iI++)
	{
		if ((iI != getID()) && (iI != eTeam))
		{
			GET_TEAM((TeamTypes)iI).setWarWeariness(getID(), ((GET_TEAM((TeamTypes)iI).getWarWeariness(getID()) + GET_TEAM((TeamTypes)iI).getWarWeariness(eTeam)) / 2));
			GET_TEAM((TeamTypes)iI).setStolenVisibilityTimer(getID(), ((GET_TEAM((TeamTypes)iI).getStolenVisibilityTimer(getID()) + GET_TEAM((TeamTypes)iI).getStolenVisibilityTimer(eTeam)) / 2));
			GET_TEAM((TeamTypes)iI).AI_setAtWarCounter(getID(), ((GET_TEAM((TeamTypes)iI).AI_getAtWarCounter(getID()) + GET_TEAM((TeamTypes)iI).AI_getAtWarCounter(eTeam)) / 2));
			GET_TEAM((TeamTypes)iI).AI_setAtPeaceCounter(getID(), ((GET_TEAM((TeamTypes)iI).AI_getAtPeaceCounter(getID()) + GET_TEAM((TeamTypes)iI).AI_getAtPeaceCounter(eTeam)) / 2));
			GET_TEAM((TeamTypes)iI).AI_setHasMetCounter(getID(), ((GET_TEAM((TeamTypes)iI).AI_getHasMetCounter(getID()) + GET_TEAM((TeamTypes)iI).AI_getHasMetCounter(eTeam)) / 2));
			GET_TEAM((TeamTypes)iI).AI_setDefensivePactCounter(getID(), ((GET_TEAM((TeamTypes)iI).AI_getDefensivePactCounter(getID()) + GET_TEAM((TeamTypes)iI).AI_getDefensivePactCounter(eTeam)) / 2));
			GET_TEAM((TeamTypes)iI).AI_setShareWarCounter(getID(), ((GET_TEAM((TeamTypes)iI).AI_getShareWarCounter(getID()) + GET_TEAM((TeamTypes)iI).AI_getShareWarCounter(eTeam)) / 2));
			GET_TEAM((TeamTypes)iI).AI_setWarSuccess(getID(), ((GET_TEAM((TeamTypes)iI).AI_getWarSuccess(getID()) + GET_TEAM((TeamTypes)iI).AI_getWarSuccess(eTeam)) / 2));
			GET_TEAM((TeamTypes)iI).AI_setEnemyPeacetimeTradeValue(getID(), ((GET_TEAM((TeamTypes)iI).AI_getEnemyPeacetimeTradeValue(getID()) + GET_TEAM((TeamTypes)iI).AI_getEnemyPeacetimeTradeValue(eTeam)) / 2));
			GET_TEAM((TeamTypes)iI).AI_setEnemyPeacetimeGrantValue(getID(), ((GET_TEAM((TeamTypes)iI).AI_getEnemyPeacetimeGrantValue(getID()) + GET_TEAM((TeamTypes)iI).AI_getEnemyPeacetimeGrantValue(eTeam)) / 2));

/************************************************************************************************/
/* UNOFFICIAL_PATCH                       09/17/09                                jdog5000      */
/*                                                                                              */
/* Bugfix				                                                                         */
/************************************************************************************************/
			GET_TEAM((TeamTypes)iI).setEspionagePointsAgainstTeam( getID(), std::max(GET_TEAM((TeamTypes)iI).getEspionagePointsAgainstTeam(getID()), GET_TEAM((TeamTypes)iI).getEspionagePointsAgainstTeam(eTeam)));
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/

			if (GET_TEAM((TeamTypes)iI).isAlive())
			{
				GET_TEAM((TeamTypes)iI).AI_setWarPlan(getID(), NO_WARPLAN, false);
				GET_TEAM((TeamTypes)iI).AI_setWarPlan(eTeam, NO_WARPLAN, false);
		}
	}
	}

	AI_updateWorstEnemy();

	AI_updateAreaStragies();

	GC.getGameINLINE().updateScore(true);
}


void CvTeam::shareItems(TeamTypes eTeam)
{
	CvCity* pLoopCity;
	int iLoop = 0;
	int iI, iJ, iK;

	FAssert(eTeam != NO_TEAM);
	FAssert(eTeam != getID());

	for (iI = 0; iI < GC.getNumTechInfos(); iI++)
	{
		if (GET_TEAM(eTeam).isHasTech((TechTypes)iI))
		{
			setHasTech(((TechTypes)iI), true, NO_PLAYER, true, false);
		}
	}

	for (iI = 0; iI < GC.getNumBonusInfos(); ++iI)
	{
		if (GET_TEAM(eTeam).isForceRevealedBonus((BonusTypes)iI))
		{
			setForceRevealedBonus((BonusTypes)iI, true);
		}
	}

	for (int iTeam = 0; iTeam < MAX_TEAMS; ++iTeam)
	{
		setEspionagePointsAgainstTeam((TeamTypes)iTeam, std::max(GET_TEAM(eTeam).getEspionagePointsAgainstTeam((TeamTypes)iTeam), getEspionagePointsAgainstTeam((TeamTypes)iTeam)));
	}

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == eTeam)
			{
				for (pLoopCity = GET_PLAYER((PlayerTypes)iI).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER((PlayerTypes)iI).nextCity(&iLoop))
				{
					for (iJ = 0; iJ < GC.getNumBuildingInfos(); iJ++)
					{
						if (pLoopCity->getNumBuilding((BuildingTypes)iJ) > 0)
						{
							if (!isObsoleteBuilding((BuildingTypes)iJ))
							{
								if (GC.getBuildingInfo((BuildingTypes)iJ).isTeamShare())
								{
									for (iK = 0; iK < MAX_PLAYERS; iK++)
									{
										if (GET_PLAYER((PlayerTypes)iK).getTeam() == getID() && GET_PLAYER((PlayerTypes)iK).isAlive())
										{
											GET_PLAYER((PlayerTypes)iK).processBuilding(((BuildingTypes)iJ), pLoopCity->getNumBuilding((BuildingTypes)iJ), pLoopCity->area());
										}
									}
								}

								processBuilding(((BuildingTypes)iJ), pLoopCity->getNumBuilding((BuildingTypes)iJ));
							}
						}
					}
				}
			}
		}
	}

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == eTeam)
			{
				GET_PLAYER((PlayerTypes)iI).AI_updateBonusValue();
			}
		}
	}
}


void CvTeam::shareCounters(TeamTypes eTeam)
{
	int iI;

	for (iI = 0; iI < MAX_TEAMS; iI++)
	{
		if ((iI != getID()) && (iI != eTeam))
		{
			if (GET_TEAM(eTeam).getWarWeariness((TeamTypes)iI) > getWarWeariness((TeamTypes)iI))
			{
				setWarWeariness(((TeamTypes)iI), GET_TEAM(eTeam).getWarWeariness((TeamTypes)iI));
			}

			if (GET_TEAM(eTeam).getStolenVisibilityTimer((TeamTypes)iI) > getStolenVisibilityTimer((TeamTypes)iI))
			{
				setStolenVisibilityTimer(((TeamTypes)iI), GET_TEAM(eTeam).getStolenVisibilityTimer((TeamTypes)iI));
			}

			if (GET_TEAM(eTeam).AI_getAtWarCounter((TeamTypes)iI) > AI_getAtWarCounter((TeamTypes)iI))
			{
				AI_setAtWarCounter(((TeamTypes)iI), GET_TEAM(eTeam).AI_getAtWarCounter((TeamTypes)iI));
			}

			if (GET_TEAM(eTeam).AI_getAtPeaceCounter((TeamTypes)iI) > AI_getAtPeaceCounter((TeamTypes)iI))
			{
				AI_setAtPeaceCounter(((TeamTypes)iI), GET_TEAM(eTeam).AI_getAtPeaceCounter((TeamTypes)iI));
			}

			if (GET_TEAM(eTeam).AI_getHasMetCounter((TeamTypes)iI) > AI_getHasMetCounter((TeamTypes)iI))
			{
				AI_setHasMetCounter(((TeamTypes)iI), GET_TEAM(eTeam).AI_getHasMetCounter((TeamTypes)iI));
			}

			if (GET_TEAM(eTeam).AI_getOpenBordersCounter((TeamTypes)iI) > AI_getOpenBordersCounter((TeamTypes)iI))
			{
				AI_setOpenBordersCounter(((TeamTypes)iI), GET_TEAM(eTeam).AI_getOpenBordersCounter((TeamTypes)iI));
			}

			if (GET_TEAM(eTeam).AI_getDefensivePactCounter((TeamTypes)iI) > AI_getDefensivePactCounter((TeamTypes)iI))
			{
				AI_setDefensivePactCounter(((TeamTypes)iI), GET_TEAM(eTeam).AI_getDefensivePactCounter((TeamTypes)iI));
			}

			if (GET_TEAM(eTeam).AI_getShareWarCounter((TeamTypes)iI) > AI_getShareWarCounter((TeamTypes)iI))
			{
				AI_setShareWarCounter(((TeamTypes)iI), GET_TEAM(eTeam).AI_getShareWarCounter((TeamTypes)iI));
			}

			if (GET_TEAM(eTeam).AI_getWarSuccess((TeamTypes)iI) > AI_getWarSuccess((TeamTypes)iI))
			{
				AI_setWarSuccess(((TeamTypes)iI), GET_TEAM(eTeam).AI_getWarSuccess((TeamTypes)iI));
			}

			if (GET_TEAM(eTeam).AI_getEnemyPeacetimeTradeValue((TeamTypes)iI) > AI_getEnemyPeacetimeTradeValue((TeamTypes)iI))
			{
				AI_setEnemyPeacetimeTradeValue(((TeamTypes)iI), GET_TEAM(eTeam).AI_getEnemyPeacetimeTradeValue((TeamTypes)iI));
			}

			if (GET_TEAM(eTeam).AI_getEnemyPeacetimeGrantValue((TeamTypes)iI) > AI_getEnemyPeacetimeGrantValue((TeamTypes)iI))
			{
				AI_setEnemyPeacetimeGrantValue(((TeamTypes)iI), GET_TEAM(eTeam).AI_getEnemyPeacetimeGrantValue((TeamTypes)iI));
			}

			GET_TEAM(eTeam).AI_setWarPlan(((TeamTypes)iI), NO_WARPLAN, false);
		}
	}

	for (iI = 0; iI < GC.getNumProjectInfos(); iI++)
	{
		int iExtraProjects = GET_TEAM(eTeam).getProjectCount((ProjectTypes)iI) - getProjectCount((ProjectTypes)iI);
		if (iExtraProjects > 0)
		{
			changeProjectCount((ProjectTypes)iI, iExtraProjects);
			GC.getGameINLINE().incrementProjectCreatedCount((ProjectTypes)iI, -iExtraProjects);
		}

		changeProjectMaking(((ProjectTypes)iI), GET_TEAM(eTeam).getProjectMaking((ProjectTypes)iI));
	}

	for (iI = 0; iI < GC.getNumUnitClassInfos(); iI++)
	{
		changeUnitClassCount(((UnitClassTypes)iI), GET_TEAM(eTeam).getUnitClassCount((UnitClassTypes)iI));
	}

	for (iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
	{
		changeBuildingClassCount(((BuildingClassTypes)iI), GET_TEAM(eTeam).getBuildingClassCount((BuildingClassTypes)iI));
	}

	for (iI = 0; iI < GC.getNumTechInfos(); iI++)
	{
/************************************************************************************************/
/* UNOFFICIAL_PATCH                       04/29/10                                jdog5000      */
/*                                                                                              */
/* Bugfix                                                                                       */
/************************************************************************************************/
/* original bts code
		if (GET_TEAM(eTeam).getResearchProgress((TechTypes)iI) > getResearchProgress((TechTypes)iI))
		{
			setResearchProgress(((TechTypes)iI), GET_TEAM(eTeam).getResearchProgress((TechTypes)iI), getLeaderID());
		}

		if (GET_TEAM(eTeam).isNoTradeTech((TechTypes)iI))
		{
			setNoTradeTech(((TechTypes)iI), true);
		}
*/
		// Overflow from techs this team already has can cause bugged behavior
		if( !isHasTech((TechTypes)iI) )
		{
			if (GET_TEAM(eTeam).getResearchProgress((TechTypes)iI) > getResearchProgress((TechTypes)iI))
			{
				setResearchProgress(((TechTypes)iI), GET_TEAM(eTeam).getResearchProgress((TechTypes)iI), getLeaderID());
			}
		}

		// Clear no tech trade if it is false for other team
		// Fixes bug where if, with no tech brokering, team A trades a tech to team B, then later joins B in
		// a permanent alliance.  Previous code would block the AB alliance from "brokering" the tech, even
		// though A had researched it on their own.
		if ( GET_TEAM(eTeam).isHasTech((TechTypes)iI) && !(GET_TEAM(eTeam).isNoTradeTech((TechTypes)iI)) )
		{
			setNoTradeTech(((TechTypes)iI), false);
		}
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/
	}
}


void CvTeam::processBuilding(BuildingTypes eBuilding, int iChange)
{
	PROFILE_FUNC();

	for (int i = 0; i < GC.getNumVoteSourceInfos(); ++i)
	{
		if (GC.getBuildingInfo(eBuilding).getVoteSourceType() == i)
		{
			changeForceTeamVoteEligibilityCount((VoteSourceTypes)i, (GC.getBuildingInfo(eBuilding).isForceTeamVoteEligible()) ? iChange : 0);
		}
	}

	if (GC.getBuildingInfo(eBuilding).isMapCentering())
	{
		if (iChange > 0)
		{
			setMapCentering(true);
		}
	}
	changeEnemyWarWearinessModifier(GC.getBuildingInfo(eBuilding).getEnemyWarWearinessModifier() * iChange);
}


void CvTeam::doTurn()
{
	MEMORY_TRACE_FUNCTION();
	PROFILE("CvTeam::doTurn()")

	int iCount;
	int iPossibleCount;
	int iI, iJ;

	FAssertMsg(isAlive(), "isAlive is expected to be true");

	AI_doTurnPre();

	if (isBarbarian())
	{
		for (iI = 0; iI < GC.getNumTechInfos(); iI++)
		{
			if (!isHasTech((TechTypes)iI))
			{
				iCount = 0;
				iPossibleCount = 0;

				for (iJ = 0; iJ < MAX_CIV_TEAMS; iJ++)
				{
/************************************************************************************************/
/* UNOFFICIAL_PATCH                       03/01/10                     Mongoose & jdog5000      */
/*                                                                                              */
/* Bugfix                                                                                       */
/************************************************************************************************/
/* original bts code
					if (GET_TEAM((TeamTypes)iJ).isAlive())
*/
					// From Mongoose SDK, BarbarianPassiveTechFix
					if (GET_TEAM((TeamTypes)iJ).isAlive() && !GET_TEAM((TeamTypes)iJ).isBarbarian())
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/
					{
						if (GET_TEAM((TeamTypes)iJ).isHasTech((TechTypes)iI))
						{
							iCount++;
						}

						iPossibleCount++;
					}
				}

				if (iCount > 0)
				{
					FAssertMsg(iPossibleCount > 0, "iPossibleCount is expected to be greater than 0");

/************************************************************************************************/
/* UNOFFICIAL_PATCH                       03/01/10                     Mongoose & jdog5000      */
/*                                                                                              */
/* Bugfix                                                                                       */
/************************************************************************************************/
/* original bts code
					changeResearchProgress(((TechTypes)iI), ((getResearchCost((TechTypes)iI) * ((GC.getDefineINT("BARBARIAN_FREE_TECH_PERCENT") * iCount) / iPossibleCount)) / 100), getLeaderID());
*/
					// From Mongoose SDK, BarbarianPassiveTechFix
					changeResearchProgress((TechTypes)iI, std::max((getResearchCost((TechTypes)iI) * GC.getDefineINT("BARBARIAN_FREE_TECH_PERCENT") * iCount) / (100 * iPossibleCount), 1), getLeaderID());
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/
				}
			}
		}
	} else if (isMinorCiv()) {
		//Afforess: fix bug where new civilizations that spawn after a minor civ will not be war with minor civs they meet
		//Every turn for new civs this minor civ has met, and declare war
		for (iI = 0; iI < MAX_CIV_TEAMS; iI++)
		{
			if( iI != getID() && isHasMet((TeamTypes)iI) && !isAtWar((TeamTypes)iI) )
			{
				declareWar((TeamTypes)iI, false, WARPLAN_LIMITED);
			}
		}
	}

	for (iI = 0; iI < MAX_TEAMS; iI++)
	{
		if (GET_TEAM((TeamTypes)iI).isAlive())
		{
			if (getStolenVisibilityTimer((TeamTypes)iI) > 0)
			{
				changeStolenVisibilityTimer(((TeamTypes)iI), -1);
			}

			if (getCounterespionageTurnsLeftAgainstTeam((TeamTypes) iI) > 0)
			{
				changeCounterespionageTurnsLeftAgainstTeam((TeamTypes) iI, -1);
			}

			if (getCounterespionageTurnsLeftAgainstTeam((TeamTypes) iI) == 0)
			{
				setCounterespionageModAgainstTeam((TeamTypes) iI, 0);
			}
		}
	}

	if (!GC.getGameINLINE().isOption(GAMEOPTION_NO_TECH_BROKERING))
	{
		for (iI = 0; iI < GC.getNumTechInfos(); iI++)
		{
			setNoTradeTech(((TechTypes)iI), false);
		}

	}
	if (GC.getGameINLINE().isModderGameOption(MODDERGAMEOPTION_CONTACT_DECAY))
	{
		doEmbassyContactCheck();
	}
	doWarWeariness();

	testCircumnavigated();

	AI_doTurnPost();

}


void CvTeam::updateYield()
{
	int iI;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				GET_PLAYER((PlayerTypes)iI).updateYield();
			}
		}
	}
}


void CvTeam::updatePowerHealth()
{
	int iI;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				GET_PLAYER((PlayerTypes)iI).updatePowerHealth();
			}
		}
	}
}


void CvTeam::updateCommerce()
{
	int iI;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				GET_PLAYER((PlayerTypes)iI).updateCommerce();
			}
		}
	}
}


bool CvTeam::canChangeWarPeace(TeamTypes eTeam, bool bAllowVassal) const
{
	if (GC.getGameINLINE().isOption(GAMEOPTION_NO_CHANGING_WAR_PEACE))
	{
		return false;
	}

	if (eTeam == getID())
	{
		return false;
	}

	if (isPermanentWarPeace(eTeam) || GET_TEAM(eTeam).isPermanentWarPeace(getID()))
	{
		return false;
	}

/************************************************************************************************/
/* REVOLUTION_MOD                         02/08/08                                jdog5000      */
/*                                                                                              */
/* For minor civs                                                                               */
/************************************************************************************************/
	// No dealing with minor civs
	if( isMinorCiv() || GET_TEAM(eTeam).isMinorCiv() )
	{
		return false;
	}
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/
	for (int iLoopTeam = 0; iLoopTeam < MAX_CIV_TEAMS; ++iLoopTeam)
	{
		CvTeam& kLoopTeam = GET_TEAM((TeamTypes)iLoopTeam);
		if (kLoopTeam.isAlive())
		{
			if (kLoopTeam.isVassal(getID()) && kLoopTeam.isPermanentWarPeace(eTeam))
			{
				return false;
			}

			if (kLoopTeam.isVassal(eTeam) && kLoopTeam.isPermanentWarPeace(getID()))
			{
				return false;
			}
		}
	}

	if (isAVassal())
	{
		return false;
	}

	if (bAllowVassal)
	{
		if (GET_TEAM(eTeam).isVassal(getID()))
		{
			return false;
		}
	}
	else
	{
		if (GET_TEAM(eTeam).isAVassal())
		{
			return false;
		}
	}

	return true;
}


bool CvTeam::canDeclareWar(TeamTypes eTeam) const
{
	if (eTeam == getID())
	{
		return false;
	}

	if (!(isAlive()) || !(GET_TEAM(eTeam).isAlive()))
	{
		return false;
	}

	if (isAtWar(eTeam))
	{
		return false;
	}

	if (!isHasMet(eTeam))
	{
		return false;
	}

	if (isForcePeace(eTeam))
	{
		return false;
	}

	for (int i = 0; i < MAX_TEAMS; ++i)
	{
		if (i != eTeam && i != getID() && ((GET_TEAM(eTeam).isVassal((TeamTypes)i)) || (GET_TEAM((TeamTypes)i).isVassal(eTeam))))
		{
			if (isForcePeace((TeamTypes)i))
			{
				return false;
			}
		}
	}

	if (!canChangeWarPeace(eTeam, true))
	{
		return false;
	}

	if (GC.getGameINLINE().isOption(GAMEOPTION_ALWAYS_PEACE))
	{
		return false;
	}

	return true;
}

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      01/16/10                                jdog5000      */
/*                                                                                              */
/* War Strategy AI                                                                              */
/************************************************************************************************/
/// \brief Version of canDeclareWar which ignores temporary peace treaties.
///
/// This function is for AIs considering who to start war preparations against, so they're future
/// plans aren't unnecessarily affected by current conditions.
///
/// Could not change definition of canDeclareWar, some sporadic crash-inducing compatibility issue
/// with the DLL it seems.  Lost a lot of time tracking down the source of the crash, it really is 
/// just from adding bool bWhatever = false to canDeclareWar in CvTeam.h.  So, that's why there's 
/// this overlapping second function.
bool CvTeam::canEventuallyDeclareWar(TeamTypes eTeam) const
{
	if (eTeam == getID())
	{
		return false;
	}

	if (!(isAlive()) || !(GET_TEAM(eTeam).isAlive()))
	{
		return false;
	}

	if (isAtWar(eTeam))
	{
		return false;
	}

	if (!isHasMet(eTeam))
	{
		return false;
	}

	if (!canChangeWarPeace(eTeam, true))
	{
		return false;
	}

	if (GC.getGameINLINE().isOption(GAMEOPTION_ALWAYS_PEACE))
	{
		return false;
	}

	return true;
}


void CvTeam::declareWar(TeamTypes eTeam, bool bNewDiplo, WarPlanTypes eWarPlan, bool bCancelPacts)
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/
{
	PROFILE_FUNC();

	//CLLNode<TradeData>* pNode;
	CvDiploParameters* pDiplo;
	CvDeal* pLoopDeal;
	CvWString szBuffer;
	//bool bCancelDeal;
	int iLoop = 0;
	int iI, iJ;

	FAssertMsg(eTeam != NO_TEAM, "eTeam is not assigned a valid value");
	FAssertMsg(eTeam != getID(), "eTeam is not expected to be equal with getID()");

	if (!isAtWar(eTeam))
	{
		//FAssertMsg((isHuman() || isMinorCiv() || GET_TEAM(eTeam).isMinorCiv() || isBarbarian() || GET_TEAM(eTeam).isBarbarian() || AI_isSneakAttackReady(eTeam) || (GET_TEAM(eTeam).getAtWarCount(true) > 0) || !(GC.getGameINLINE().isFinalInitialized()) || gDLL->GetWorldBuilderMode() || getVassalCount() > 0  || isAVassal() || GET_TEAM(eTeam).getVassalCount() > 0  || GET_TEAM(eTeam).isAVassal() || getDefensivePactCount() > 0), "Possibly accidental AI war!!!");

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      10/02/09                                jdog5000      */
/*                                                                                              */
/* AI logging                                                                                   */
/************************************************************************************************/
		if( gTeamLogLevel >= 1 )
		{
			logBBAIForTeam(getID(), "  Team %d (%S) declares war on team %d", getID(), GET_PLAYER(getLeaderID()).getCivilizationDescription(0), eTeam);
		}
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

		for (pLoopDeal = GC.getGameINLINE().firstDeal(&iLoop); pLoopDeal != NULL; pLoopDeal = GC.getGameINLINE().nextDeal(&iLoop))
		{
			if (((GET_PLAYER(pLoopDeal->getFirstPlayer()).getTeam() == getID()) && (GET_PLAYER(pLoopDeal->getSecondPlayer()).getTeam() == eTeam)) ||
					((GET_PLAYER(pLoopDeal->getFirstPlayer()).getTeam() == eTeam) && (GET_PLAYER(pLoopDeal->getSecondPlayer()).getTeam() == getID())))
			{
				pLoopDeal->kill();
			}
		}

		for (iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (((GET_PLAYER((PlayerTypes)iI).getTeam() == getID()) || (GET_PLAYER((PlayerTypes)iI).getTeam() == eTeam)) && GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				GET_PLAYER((PlayerTypes)iI).updatePlunder(-1, false);
			}
/************************************************************************************************/
/* Afforess	                  Start		 05/23/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID() && GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				if (GET_PLAYER((PlayerTypes)iI).getPledgedSecretaryGeneralVote() == eTeam)
				{
					GET_PLAYER((PlayerTypes)iI).setPledgedSecretaryGeneralVote(NO_TEAM);
				}
			}

			if (GET_PLAYER((PlayerTypes)iI).getTeam() == eTeam && GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				if (GET_PLAYER((PlayerTypes)iI).getPledgedSecretaryGeneralVote() == getID())
				{
					GET_PLAYER((PlayerTypes)iI).setPledgedSecretaryGeneralVote(NO_TEAM);
				}
			}
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
		}

		FAssertMsg(eTeam != getID(), "eTeam is not expected to be equal with getID()");
		setAtWar(eTeam, true);
		GET_TEAM(eTeam).setAtWar(getID(), true);

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      08/21/09                                jdog5000      */
/*                                                                                              */
/* Efficiency                                                                                   */
/************************************************************************************************/
		// Plot danger cache
		GC.getMapINLINE().invalidateIsTeamBorderCache(eTeam);
		GC.getMapINLINE().invalidateIsTeamBorderCache(getID());
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

		for (iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (((GET_PLAYER((PlayerTypes)iI).getTeam() == getID()) || (GET_PLAYER((PlayerTypes)iI).getTeam() == eTeam)) && GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				GET_PLAYER((PlayerTypes)iI).updatePlunder(1, false);
			}
		}

		meet(eTeam, false);

		AI_setAtPeaceCounter(eTeam, 0);
		GET_TEAM(eTeam).AI_setAtPeaceCounter(getID(), 0);

		AI_setShareWarCounter(eTeam, 0);
		GET_TEAM(eTeam).AI_setShareWarCounter(getID(), 0);

		GET_TEAM(eTeam).AI_setWarPlan(getID(), ((isBarbarian() || isMinorCiv()) ? WARPLAN_ATTACKED : WARPLAN_ATTACKED_RECENT));

/************************************************************************************************/
/* REVOLUTION_MOD                         05/27/08                                jdog5000      */
/*                                                                                              */
/* For barbarian and minor civs                                                                 */
/************************************************************************************************/
		// Added check for barb, minor - jwd
		if (!isBarbarian() && !isMinorCiv() && !GET_TEAM(eTeam).isMinorCiv() && !GET_TEAM(eTeam).isBarbarian())
		{
			for (iI = 0; iI < MAX_TEAMS; iI++)
			{
				if (GET_TEAM((TeamTypes)iI).isAlive())
				{
					if (!GET_TEAM(eTeam).isAtWar((TeamTypes)iI) && GET_TEAM(eTeam).AI_isChosenWar((TeamTypes)iI))
					{
						GET_TEAM(eTeam).AI_setWarPlan(((TeamTypes)iI), NO_WARPLAN);
					}
				}
			}
		}
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/

		if (NO_WARPLAN != eWarPlan)
		{
			AI_setWarPlan(eTeam, eWarPlan);
		}

		FAssert(!(AI_isSneakAttackPreparing(eTeam)));
		if ((AI_getWarPlan(eTeam) == NO_WARPLAN) || AI_isSneakAttackPreparing(eTeam))
		{
			if (isHuman())
			{
				AI_setWarPlan(eTeam, WARPLAN_TOTAL);
			}
			else if (isMinorCiv() || isBarbarian() || (GET_TEAM(eTeam).getAtWarCount(true) == 1))
			{
				AI_setWarPlan(eTeam, WARPLAN_LIMITED);
			}
			else
			{
				AI_setWarPlan(eTeam, WARPLAN_DOGPILE);
			}
		}

		GC.getMapINLINE().verifyUnitValidPlot();

		for (iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID() && GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				GET_PLAYER((PlayerTypes)iI).verifyUnitStacksValid();
			}
		}

		GC.getGameINLINE().AI_makeAssignWorkDirty();

		if ((getID() == GC.getGameINLINE().getActiveTeam()) || (eTeam == GC.getGameINLINE().getActiveTeam()))
		{
			gDLL->getInterfaceIFace()->setDirty(Score_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(CityInfo_DIRTY_BIT, true);
		}

/************************************************************************************************/
/* REVOLUTION_MOD                         03/06/08                                jdog5000      */
/*                                                                                              */
/* For barbarian and minor civs                                                                 */
/************************************************************************************************/
/* original code
		for (iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				for (iJ = 0; iJ < MAX_PLAYERS; iJ++)
				{
					if (GET_PLAYER((PlayerTypes)iJ).isAlive())
					{
						if ((GET_PLAYER((PlayerTypes)iI).getTeam() == getID()) && (GET_PLAYER((PlayerTypes)iJ).getTeam() == eTeam))
						{
							GET_PLAYER((PlayerTypes)iI).AI_setFirstContact(((PlayerTypes)iJ), true);
							GET_PLAYER((PlayerTypes)iJ).AI_setFirstContact(((PlayerTypes)iI), true);
						}
					}
				}
			}
		}

		for (iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				for (iJ = 0; iJ < MAX_PLAYERS; iJ++)
				{
					if (GET_PLAYER((PlayerTypes)iJ).isAlive())
					{
						if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
						{
							if (GET_PLAYER((PlayerTypes)iJ).getTeam() == eTeam)
							{
								GET_PLAYER((PlayerTypes)iJ).AI_changeMemoryCount(((PlayerTypes)iI), MEMORY_DECLARED_WAR, 1);
							}
							else if (GET_PLAYER((PlayerTypes)iJ).getTeam() != getID())
							{
								if (GET_TEAM(GET_PLAYER((PlayerTypes)iJ).getTeam()).isHasMet(eTeam))
								{
									if ((GET_TEAM(GET_PLAYER((PlayerTypes)iJ).getTeam()).AI_getAttitude(eTeam) >= ATTITUDE_PLEASED) && !(GET_TEAM(GET_PLAYER((PlayerTypes)iJ).getTeam()).isAtWar(eTeam)))
									{
										GET_PLAYER((PlayerTypes)iJ).AI_changeMemoryCount(((PlayerTypes)iI), MEMORY_DECLARED_WAR_ON_FRIEND, 1);
									}
								}
							}
						}
					}
				}
			}
		}
*/
		// Added check for barb, minor - jwd
		if (!isBarbarian() && !(GET_TEAM(eTeam).isBarbarian()) &&
				  !isMinorCiv() && !(GET_TEAM(eTeam).isMinorCiv()))
		{
			for (iI = 0; iI < MAX_PLAYERS; iI++)
			{
				if (GET_PLAYER((PlayerTypes)iI).isAlive())
				{
					for (iJ = 0; iJ < MAX_PLAYERS; iJ++)
					{
						if (GET_PLAYER((PlayerTypes)iJ).isAlive())
						{
							if ((GET_PLAYER((PlayerTypes)iI).getTeam() == getID()) && (GET_PLAYER((PlayerTypes)iJ).getTeam() == eTeam))
							{
								GET_PLAYER((PlayerTypes)iI).AI_setFirstContact(((PlayerTypes)iJ), true);
								GET_PLAYER((PlayerTypes)iJ).AI_setFirstContact(((PlayerTypes)iI), true);
							}
						}
					}
				}
			}
/************************************************************************************************/
/* Afforess	                  Start		 05/23/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
			if (GC.getGameINLINE().isOption(GAMEOPTION_RUTHLESS_AI))
			{
				for (iI = 0; iI < MAX_PLAYERS; iI++)
				{
					if (GET_PLAYER((PlayerTypes)iI).isAlive())
					{
						//Player is on our team
						if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
						{
							//Calculate players we just backstabbed
							for (iJ = 0; iJ < MAX_PLAYERS; iJ++)
							{
								if (GET_PLAYER((PlayerTypes)iJ).isAlive())
								{
									//Player is on the team we are declaring war on
									if (GET_PLAYER((PlayerTypes)iJ).getTeam() == eTeam)
									{
										if (GET_PLAYER((PlayerTypes)iJ).AI_getAttitude((PlayerTypes)iI) >= ATTITUDE_PLEASED)
										{
											GET_PLAYER((PlayerTypes)iJ).AI_changeMemoryCount(((PlayerTypes)iI), MEMORY_BACKSTAB, 1);
										}
									}
								}
							}
							//Calculate players whose friend we just backstabbed
							for (iJ = 0; iJ < MAX_PLAYERS; iJ++)
							{
								if (GET_PLAYER((PlayerTypes)iJ).isAlive())
								{
									//Player is not on the team we declared war on, and not on our team
									if (GET_PLAYER((PlayerTypes)iJ).getTeam() != eTeam && GET_PLAYER((PlayerTypes)iJ).getTeam() != getID())
									{
										//Friends with the leader of the team we declared war on
										if (GET_PLAYER((PlayerTypes)iJ).AI_getAttitude(GET_TEAM(eTeam).getLeaderID()) >= ATTITUDE_PLEASED)
										{
											GET_PLAYER((PlayerTypes)iJ).AI_changeMemoryCount(((PlayerTypes)iI), MEMORY_BACKSTAB_FRIEND, 1);
										}
									}
								}
							}
						}
					}
				}
			}
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/

			for (iI = 0; iI < MAX_PLAYERS; iI++)
			{
				if (GET_PLAYER((PlayerTypes)iI).isAlive())
				{
					for (iJ = 0; iJ < MAX_PLAYERS; iJ++)
					{
						if (GET_PLAYER((PlayerTypes)iJ).isAlive())
						{
							if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
							{
								if (GET_PLAYER((PlayerTypes)iJ).getTeam() == eTeam)
								{
									GET_PLAYER((PlayerTypes)iJ).AI_changeMemoryCount(((PlayerTypes)iI), MEMORY_DECLARED_WAR, 1);

								}
								else if (GET_PLAYER((PlayerTypes)iJ).getTeam() != getID())
								{
									if (GET_TEAM(GET_PLAYER((PlayerTypes)iJ).getTeam()).isHasMet(eTeam))
									{
										if ((GET_TEAM(GET_PLAYER((PlayerTypes)iJ).getTeam()).AI_getAttitude(eTeam) >= ATTITUDE_PLEASED) && !(GET_TEAM(GET_PLAYER((PlayerTypes)iJ).getTeam()).isAtWar(eTeam)))
										{
											GET_PLAYER((PlayerTypes)iJ).AI_changeMemoryCount(((PlayerTypes)iI), MEMORY_DECLARED_WAR_ON_FRIEND, 1);
										}
									}
								}
							}
						}
					}
				}
			}
		}
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/

		for (iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				if ((GET_PLAYER((PlayerTypes)iI).getTeam() == getID()) || (GET_PLAYER((PlayerTypes)iI).getTeam() == eTeam))
				{
					GET_PLAYER((PlayerTypes)iI).updateWarWearinessPercentAnger();
				}
			}
		}

		for (iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				if ((GET_PLAYER((PlayerTypes)iI).getTeam() == getID()) || (GET_PLAYER((PlayerTypes)iI).getTeam() == eTeam))
				{
					GET_PLAYER((PlayerTypes)iI).updatePlotGroups();
				}
			}
		}

		for (iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				if ((GET_PLAYER((PlayerTypes)iI).getTeam() == getID()) || (GET_PLAYER((PlayerTypes)iI).getTeam() == eTeam))
				{
					GET_PLAYER((PlayerTypes)iI).updateTradeRoutes();
				}
			}
		}

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      09/03/09                       poyuzhe & jdog5000     */
/*                                                                                              */
/* Efficiency                                                                                   */
/************************************************************************************************/
		// From Sanguo Mod Performance, ie the CAR Mod
		// Attitude cache
		if (GC.getGameINLINE().isFinalInitialized())
		{
			for (int iI = 0; iI < MAX_PLAYERS; iI++)
			{
				if( GET_PLAYER((PlayerTypes)iI).isAlive() )
				{
					if( GET_PLAYER((PlayerTypes)iI).getTeam() == getID() || GET_PLAYER((PlayerTypes)iI).getTeam() == eTeam
						|| GET_TEAM(GET_PLAYER((PlayerTypes)iI).getTeam()).isAtWar(getID()) || GET_TEAM(GET_PLAYER((PlayerTypes)iI).getTeam()).isAtWar(eTeam) )
					{
						for (int iJ = 0; iJ < MAX_PLAYERS; iJ++)
						{
							if( GET_PLAYER((PlayerTypes)iJ).isAlive() && GET_PLAYER((PlayerTypes)iJ).getTeam() != GET_PLAYER((PlayerTypes)iI).getTeam() )
							{
								if( GET_PLAYER((PlayerTypes)iJ).getTeam() == getID() || GET_PLAYER((PlayerTypes)iJ).getTeam() == eTeam )
								{
									GET_PLAYER((PlayerTypes)iJ).AI_invalidateAttitudeCache((PlayerTypes)iI);
									GET_PLAYER((PlayerTypes)iI).AI_invalidateAttitudeCache((PlayerTypes)iJ);
								}
							}
						}
					}
				}
			}
		}
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

		if (GC.getGameINLINE().isFinalInitialized() && !(gDLL->GetWorldBuilderMode()))
		{
			if (bNewDiplo)
			{
				if (!isHuman())
				{
					for (iI = 0; iI < MAX_PLAYERS; iI++)
					{
						if (GET_PLAYER((PlayerTypes)iI).isAlive())
						{
							if (GET_PLAYER((PlayerTypes)iI).getTeam() == eTeam)
							{
								if (GET_PLAYER(getLeaderID()).canContact((PlayerTypes)iI))
								{
									if (GET_PLAYER((PlayerTypes)iI).isHuman())
									{
										pDiplo = new CvDiploParameters(getLeaderID());
										FAssertMsg(pDiplo != NULL, "pDiplo must be valid");
										pDiplo->setDiploComment((DiploCommentTypes)GC.getInfoTypeForString("AI_DIPLOCOMMENT_DECLARE_WAR"));
										pDiplo->setAIContact(true);
										gDLL->beginDiplomacy(pDiplo, ((PlayerTypes)iI));
									}
								}
							}
						}
					}
				}
			}

			if (!isBarbarian() && !(GET_TEAM(eTeam).isBarbarian()) &&
				  !isMinorCiv() && !(GET_TEAM(eTeam).isMinorCiv()))
			{
				for (iI = 0; iI < MAX_PLAYERS; iI++)
				{
					if (GET_PLAYER((PlayerTypes)iI).isAlive())
					{
						MEMORY_TRACK_EXEMPT();

						if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
						{
							szBuffer = gDLL->getText("TXT_KEY_MISC_YOU_DECLARED_WAR_ON", GET_TEAM(eTeam).getName().GetCString());
							AddDLLMessage(((PlayerTypes)iI), true, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_DECLAREWAR", MESSAGE_TYPE_MAJOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_WARNING_TEXT"));
						}
						else if (GET_PLAYER((PlayerTypes)iI).getTeam() == eTeam)
						{
							szBuffer = gDLL->getText("TXT_KEY_MISC_DECLARED_WAR_ON_YOU", getName().GetCString());
							AddDLLMessage(((PlayerTypes)iI), true, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_DECLAREWAR", MESSAGE_TYPE_MAJOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_WARNING_TEXT"));
						}
						else if (GET_TEAM(GET_PLAYER((PlayerTypes)iI).getTeam()).isHasMet(getID()) && GET_TEAM(GET_PLAYER((PlayerTypes)iI).getTeam()).isHasMet(eTeam))
						{
							szBuffer = gDLL->getText("TXT_KEY_MISC_SOMEONE_DECLARED_WAR", getName().GetCString(), GET_TEAM(eTeam).getName().GetCString());
							AddDLLMessage(((PlayerTypes)iI), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_THEIRDECLAREWAR", MESSAGE_TYPE_MAJOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_WARNING_TEXT"));
						}
					}
				}

				szBuffer = gDLL->getText("TXT_KEY_MISC_SOMEONE_DECLARES_WAR", getName().GetCString(), GET_TEAM(eTeam).getName().GetCString());
				GC.getGameINLINE().addReplayMessage(REPLAY_MESSAGE_MAJOR_EVENT, getLeaderID(), szBuffer, -1, -1, (ColorTypes)GC.getInfoTypeForString("COLOR_WARNING_TEXT"));
			}
		}

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      12/06/09                                jdog5000      */
/*                                                                                              */
/* Diplomacy, Customization                                                                     */
/************************************************************************************************/
		// This block is entirely redundant with the cancelDefensivePacts function, better implemented there
/*
		if (!(GET_TEAM(eTeam).isMinorCiv()))
		{
			for (iI = 0; iI < MAX_PLAYERS; iI++)
			{
				if (GET_PLAYER((PlayerTypes)iI).isAlive())
				{
					if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
					{
						for (pLoopDeal = GC.getGameINLINE().firstDeal(&iLoop); pLoopDeal != NULL; pLoopDeal = GC.getGameINLINE().nextDeal(&iLoop))
						{
							bCancelDeal = false;

							if ((pLoopDeal->getFirstPlayer() == ((PlayerTypes)iI)) || (pLoopDeal->getSecondPlayer() == ((PlayerTypes)iI)))
							{
								for (pNode = pLoopDeal->headFirstTradesNode(); (pNode != NULL); pNode = pLoopDeal->nextFirstTradesNode(pNode))
								{
									if (pNode->m_data.m_eItemType == TRADE_DEFENSIVE_PACT)
									{
										bCancelDeal = true;
									}
								}

								for (pNode = pLoopDeal->headSecondTradesNode(); (pNode != NULL); pNode = pLoopDeal->nextSecondTradesNode(pNode))
								{
									if (pNode->m_data.m_eItemType == TRADE_DEFENSIVE_PACT)
									{
										bCancelDeal = true;
									}
								}
							}

							if (bCancelDeal)
							{
								pLoopDeal->kill();
							}
						}
					}
				}
			}
		}
*/

		CvEventReporter::getInstance().changeWar(true, getID(), eTeam);

/************************************************************************************************/
/* REVOLUTION_MOD                         03/06/08                                jdog5000      */
/*                                                                                              */
/* For barbarian and minor civs                                                                 */
/************************************************************************************************/
		// Added check for barb, minor - jwd
		if (!isBarbarian() && !(GET_TEAM(eTeam).isBarbarian()) &&
				  !isMinorCiv() && !(GET_TEAM(eTeam).isMinorCiv()))
		{
			if( GC.getDefineINT("BBAI_ALLIANCE_OPTION") != 1 )
			{
				cancelDefensivePacts();
			}

			for (iI = 0; iI < MAX_TEAMS; iI++)
			{
				if (iI != getID() && iI != eTeam && GET_TEAM((TeamTypes)iI).isAlive())
				{
					if (GET_TEAM((TeamTypes)iI).isDefensivePact(eTeam))
					{
						GET_TEAM((TeamTypes)iI).declareWar(getID(), bNewDiplo, WARPLAN_DOGPILE);
					}
					else if( GET_TEAM((TeamTypes)iI).isDefensivePact(getID()))
					{
						// For alliance option.  This teams pacts are canceled above if not using alliance option.
						GET_TEAM((TeamTypes)iI).declareWar(eTeam, bNewDiplo, WARPLAN_DOGPILE);
					}
				}
			}

			if( GC.getDefineINT("BBAI_ALLIANCE_OPTION") != 1 )
			{
				GET_TEAM(eTeam).cancelDefensivePacts();
			}
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

			for (iI = 0; iI < MAX_TEAMS; iI++)
			{
				if (iI != getID() && iI != eTeam)
				{
					if (GET_TEAM((TeamTypes)iI).isAlive())
					{
						if (GET_TEAM((TeamTypes)iI).isVassal(eTeam) || GET_TEAM(eTeam).isVassal((TeamTypes)iI))
						{
							declareWar((TeamTypes)iI, bNewDiplo, AI_getWarPlan(eTeam));
						}
						else if (GET_TEAM((TeamTypes)iI).isVassal(getID()) || isVassal((TeamTypes)iI))
						{
							GET_TEAM((TeamTypes)iI).declareWar(eTeam, bNewDiplo, WARPLAN_DOGPILE);
						}
					}
				}
			}
		}
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/
	}
}

void CvTeam::makePeace(TeamTypes eTeam, bool bBumpUnits)
{
	CvWString szBuffer;
	int iI;

	FAssertMsg(eTeam != NO_TEAM, "eTeam is not assigned a valid value");
	FAssertMsg(eTeam != getID(), "eTeam is not expected to be equal with getID()");

	if (isAtWar(eTeam))
	{
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      05/21/10                                jdog5000      */
/*                                                                                              */
/* AI logging                                                                                   */
/************************************************************************************************/
		if( gTeamLogLevel >= 1 && isAlive() && GET_TEAM(eTeam).isAlive())
		{
			logBBAIForTeam(getID(), "Team %d (%S) and team %d (%S) make peace", getID(), GET_PLAYER(getLeaderID()).getCivilizationDescription(0), eTeam, GET_PLAYER(GET_TEAM(eTeam).getLeaderID()).getCivilizationDescription(0));
		}
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

		for (iI = 0; iI < MAX_PLAYERS; iI++)
		{
			//	Check isEverAlive() here rather than isAlive() since it's just an optimization (updatePlunder() will
			//	do nothing for a truely dead player as they have no units), and this method is called by REV Python
			//	in response to a plyer setAlive (false) event notification DURING the processing tof that player's
			//	death.  If we do not treat THAT player as alive here their blockades will not be lifted
			if (((GET_PLAYER((PlayerTypes)iI).getTeam() == getID()) || (GET_PLAYER((PlayerTypes)iI).getTeam() == eTeam)) && GET_PLAYER((PlayerTypes)iI).isEverAlive())
			{
				GET_PLAYER((PlayerTypes)iI).updatePlunder(-1, false);
			}
		}

		FAssertMsg(eTeam != getID(), "eTeam is not expected to be equal with getID()");
		setAtWar(eTeam, false);
		GET_TEAM(eTeam).setAtWar(getID(), false);

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      08/21/09                                jdog5000      */
/*                                                                                              */
/* Efficiency                                                                                   */
/************************************************************************************************/
		// Plot danger cache
		GC.getMapINLINE().invalidateIsTeamBorderCache(eTeam);
		GC.getMapINLINE().invalidateIsTeamBorderCache(getID());
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

		for (iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (((GET_PLAYER((PlayerTypes)iI).getTeam() == getID()) || (GET_PLAYER((PlayerTypes)iI).getTeam() == eTeam)) && GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				GET_PLAYER((PlayerTypes)iI).updatePlunder(1, false);
			}
		}

		AI_setAtWarCounter(eTeam, 0);
		GET_TEAM(eTeam).AI_setAtWarCounter(getID(), 0);

		AI_setWarSuccess(eTeam, 0);
		GET_TEAM(eTeam).AI_setWarSuccess(getID(), 0);

		AI_setWarPlan(eTeam, NO_WARPLAN);
		GET_TEAM(eTeam).AI_setWarPlan(getID(), NO_WARPLAN);

		if (bBumpUnits)
		{
			GC.getMapINLINE().verifyUnitValidPlot();
		}

		GC.getGameINLINE().AI_makeAssignWorkDirty();

		if ((getID() == GC.getGameINLINE().getActiveTeam()) || (eTeam == GC.getGameINLINE().getActiveTeam()))
		{
			gDLL->getInterfaceIFace()->setDirty(Score_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(CityInfo_DIRTY_BIT, true);
		}

		for (iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				if ((GET_PLAYER((PlayerTypes)iI).getTeam() == getID()) || (GET_PLAYER((PlayerTypes)iI).getTeam() == eTeam))
				{
					GET_PLAYER((PlayerTypes)iI).updateWarWearinessPercentAnger();
				}
			}
		}

		for (iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				if ((GET_PLAYER((PlayerTypes)iI).getTeam() == getID()) || (GET_PLAYER((PlayerTypes)iI).getTeam() == eTeam))
				{
					GET_PLAYER((PlayerTypes)iI).updatePlotGroups();
				}
			}
		}

		for (iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				if ((GET_PLAYER((PlayerTypes)iI).getTeam() == getID()) || (GET_PLAYER((PlayerTypes)iI).getTeam() == eTeam))
				{
					GET_PLAYER((PlayerTypes)iI).updateTradeRoutes();
				}
			}
		}

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      09/03/09                       poyuzhe & jdog5000     */
/*                                                                                              */
/* Efficiency                                                                                   */
/************************************************************************************************/
		// From Sanguo Mod Performance, ie the CAR Mod
		// Attitude cache
		if (GC.getGameINLINE().isFinalInitialized())
		{
			for (int iI = 0; iI < MAX_PLAYERS; iI++)
			{
				if( GET_PLAYER((PlayerTypes)iI).isAlive() )
				{
					if( GET_PLAYER((PlayerTypes)iI).getTeam() == getID() || GET_PLAYER((PlayerTypes)iI).getTeam() == eTeam
						|| GET_TEAM(GET_PLAYER((PlayerTypes)iI).getTeam()).isAtWar(getID()) || GET_TEAM(GET_PLAYER((PlayerTypes)iI).getTeam()).isAtWar(eTeam) )
					{
						for (int iJ = 0; iJ < MAX_PLAYERS; iJ++)
						{
							if( GET_PLAYER((PlayerTypes)iJ).isAlive() && GET_PLAYER((PlayerTypes)iJ).getTeam() != GET_PLAYER((PlayerTypes)iI).getTeam() )
							{
								if( GET_PLAYER((PlayerTypes)iJ).getTeam() == getID() || GET_PLAYER((PlayerTypes)iJ).getTeam() == eTeam )
								{
									GET_PLAYER((PlayerTypes)iJ).AI_invalidateAttitudeCache((PlayerTypes)iI);
									GET_PLAYER((PlayerTypes)iI).AI_invalidateAttitudeCache((PlayerTypes)iJ);
								}
							}
						}
					}
				}
			}
		}
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

/************************************************************************************************/
/* REVOLUTION_MOD                         01/01/08                                jdog5000      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
/* original code
		for (iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
				{
					szBuffer = gDLL->getText("TXT_KEY_MISC_YOU_MADE_PEACE_WITH", GET_TEAM(eTeam).getName().GetCString());
					AddDLLMessage(((PlayerTypes)iI), true, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_MAKEPEACE", MESSAGE_TYPE_MAJOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));
				}
				else if (GET_PLAYER((PlayerTypes)iI).getTeam() == eTeam)
				{
					szBuffer = gDLL->getText("TXT_KEY_MISC_YOU_MADE_PEACE_WITH", getName().GetCString());
					AddDLLMessage(((PlayerTypes)iI), true, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_MAKEPEACE", MESSAGE_TYPE_MAJOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));
				}
				else if (GET_TEAM(GET_PLAYER((PlayerTypes)iI).getTeam()).isHasMet(getID()) && GET_TEAM(GET_PLAYER((PlayerTypes)iI).getTeam()).isHasMet(eTeam))
				{
					szBuffer = gDLL->getText("TXT_KEY_MISC_SOMEONE_MADE_PEACE", getName().GetCString(), GET_TEAM(eTeam).getName().GetCString());
					AddDLLMessage(((PlayerTypes)iI), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_THEIRMAKEPEACE", MESSAGE_TYPE_MAJOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));
				}
			}
		}

		szBuffer = gDLL->getText("TXT_KEY_MISC_SOMEONE_MADE_PEACE", getName().GetCString(), GET_TEAM(eTeam).getName().GetCString());
		GC.getGameINLINE().addReplayMessage(REPLAY_MESSAGE_MAJOR_EVENT, getLeaderID(), szBuffer, -1, -1, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));
*/
		// Silence announcement when one team has no units or cities
		// Needed because civs are now set to peace with all when dieing since they keep at war state when being reborn
		bool bHasUnitsOrCities  = (getNumCities() > 0);
		bool bHasUnitsOrCities2 = (GET_TEAM(eTeam).getNumCities() > 0);

		// Could be homeless rebel, must check
		if( !bHasUnitsOrCities )
		{
			for (iI = 0; iI < MAX_PLAYERS; iI++)
			{
				if (GET_PLAYER((PlayerTypes)iI).isAlive())
				{
					if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
					{
						if( GET_PLAYER((PlayerTypes)iI).getNumUnits() > 0 )
						{
							bHasUnitsOrCities = true;
							break;
						}
					}
				}
			}
		}

		if( !bHasUnitsOrCities2 )
		{
			for (iI = 0; iI < MAX_PLAYERS; iI++)
			{
				if (GET_PLAYER((PlayerTypes)iI).isAlive())
				{
					if (GET_PLAYER((PlayerTypes)iI).getTeam() == eTeam)
					{
						if( GET_PLAYER((PlayerTypes)iI).getNumUnits() > 0 )
						{
							bHasUnitsOrCities2 = true;
							break;
						}
					}
				}
			}
		}
		

		// Anounce only if neither is minor
		if( !isMinorCiv() && !GET_TEAM(eTeam).isMinorCiv() )
		{
			// Announce if both are really alive
			if( bHasUnitsOrCities && bHasUnitsOrCities2 ) 
			{
				for (iI = 0; iI < MAX_PLAYERS; iI++)
				{
					if (GET_PLAYER((PlayerTypes)iI).isAlive())
					{
						MEMORY_TRACK_EXEMPT();

						if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
						{
							szBuffer = gDLL->getText("TXT_KEY_MISC_YOU_MADE_PEACE_WITH", GET_TEAM(eTeam).getName().GetCString());
							AddDLLMessage(((PlayerTypes)iI), true, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_MAKEPEACE", MESSAGE_TYPE_MAJOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));
						}
						else if (GET_PLAYER((PlayerTypes)iI).getTeam() == eTeam)
						{
							szBuffer = gDLL->getText("TXT_KEY_MISC_YOU_MADE_PEACE_WITH", getName().GetCString());
							AddDLLMessage(((PlayerTypes)iI), true, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_MAKEPEACE", MESSAGE_TYPE_MAJOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));
						}
						else if (GET_TEAM(GET_PLAYER((PlayerTypes)iI).getTeam()).isHasMet(getID()) && GET_TEAM(GET_PLAYER((PlayerTypes)iI).getTeam()).isHasMet(eTeam))
						{
							szBuffer = gDLL->getText("TXT_KEY_MISC_SOMEONE_MADE_PEACE", getName().GetCString(), GET_TEAM(eTeam).getName().GetCString());
							AddDLLMessage(((PlayerTypes)iI), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_THEIRMAKEPEACE", MESSAGE_TYPE_MAJOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));
						}
					}
				}

				szBuffer = gDLL->getText("TXT_KEY_MISC_SOMEONE_MADE_PEACE", getName().GetCString(), GET_TEAM(eTeam).getName().GetCString());
				GC.getGameINLINE().addReplayMessage(REPLAY_MESSAGE_MAJOR_EVENT, getLeaderID(), szBuffer, -1, -1, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));
			}
		}
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/

		CvEventReporter::getInstance().changeWar(false, getID(), eTeam);

		for (iI = 0; iI < MAX_TEAMS; iI++)
		{
			if (iI != getID() && iI != eTeam)
			{
				if (GET_TEAM((TeamTypes)iI).isAlive())
				{
					if (GET_TEAM((TeamTypes)iI).isVassal(eTeam))
					{
						GET_TEAM((TeamTypes)iI).makePeace(getID(), bBumpUnits);
					}
					else if (GET_TEAM((TeamTypes)iI).isVassal(getID()))
					{
						GET_TEAM((TeamTypes)iI).makePeace(eTeam, bBumpUnits);
					}
				}
			}
		}

	}
}


bool CvTeam::canContact(TeamTypes eTeam) const
{
	int iI, iJ;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				for (iJ = 0; iJ < MAX_PLAYERS; iJ++)
				{
					if (GET_PLAYER((PlayerTypes)iJ).isAlive())
					{
						if (GET_PLAYER((PlayerTypes)iJ).getTeam() == eTeam)
						{
							if (GET_PLAYER((PlayerTypes)iI).canContact((PlayerTypes)iJ))
							{
								return true;
							}
						}
					}
				}
			}
		}
	}

	return false;
}


void CvTeam::meet(TeamTypes eTeam, bool bNewDiplo)
{
	if (!isHasMet(eTeam))
	{
		makeHasMet(eTeam, bNewDiplo);
		GET_TEAM(eTeam).makeHasMet(getID(), bNewDiplo);

/************************************************************************************************/ 
/* BETTER_BTS_AI_MOD                      02/20/10                                jdog5000      */ 
/*                                                                                              */ 
/* AI logging                                                                                   */ 
/************************************************************************************************/ 
		if( gTeamLogLevel >= 2 )
		{
			if( GC.getGameINLINE().isFinalInitialized() )
			{
				if( eTeam != getID() && isAlive() && GET_TEAM(eTeam).isAlive() )
				{
					logBBAIForTeam(getID(), "    Team %d (%S) meets team %d (%S)", getID(), GET_PLAYER(getLeaderID()).getCivilizationDescription(0), eTeam, GET_PLAYER(GET_TEAM(eTeam).getLeaderID()).getCivilizationDescription(0) );
				}
			}
		}
/************************************************************************************************/ 
/* BETTER_BTS_AI_MOD                       END                                                  */ 
/************************************************************************************************/ 
	}
}


void CvTeam::signOpenBorders(TeamTypes eTeam)
{
	CLinkList<TradeData> ourList;
	CLinkList<TradeData> theirList;
	TradeData item;

	FAssert(eTeam != NO_TEAM);
	FAssert(eTeam != getID());

	if (!isAtWar(eTeam) && (getID() != eTeam))
	{
		setTradeItem(&item, TRADE_OPEN_BORDERS);

		if (GET_PLAYER(getLeaderID()).canTradeItem(GET_TEAM(eTeam).getLeaderID(), item) && GET_PLAYER(GET_TEAM(eTeam).getLeaderID()).canTradeItem(getLeaderID(), item))
		{
			ourList.clear();
			theirList.clear();

			ourList.insertAtEnd(item);
			theirList.insertAtEnd(item);

			GC.getGameINLINE().implementDeal(getLeaderID(), (GET_TEAM(eTeam).getLeaderID()), &ourList, &theirList);
		}
	}
}

void CvTeam::signDefensivePact(TeamTypes eTeam)
{
	CLinkList<TradeData> ourList;
	CLinkList<TradeData> theirList;
	TradeData item;

	FAssert(eTeam != NO_TEAM);
	FAssert(eTeam != getID());

	if (!isAtWar(eTeam) && (getID() != eTeam))
	{
		setTradeItem(&item, TRADE_DEFENSIVE_PACT);

		if (GET_PLAYER(getLeaderID()).canTradeItem(GET_TEAM(eTeam).getLeaderID(), item) && GET_PLAYER(GET_TEAM(eTeam).getLeaderID()).canTradeItem(getLeaderID(), item))
		{
			ourList.clear();
			theirList.clear();

			ourList.insertAtEnd(item);
			theirList.insertAtEnd(item);

			GC.getGameINLINE().implementDeal(getLeaderID(), (GET_TEAM(eTeam).getLeaderID()), &ourList, &theirList);
		}
	}
}

bool CvTeam::canSignDefensivePact(TeamTypes eTeam)
{
 /************************************************************************************************/
/* Afforess	                  Start		 03/15/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	if (GC.getGameINLINE().isOption(GAMEOPTION_ADVANCED_DIPLOMACY))
	{
	    if (!isHasEmbassy(eTeam))
	    {
	        return false;
	    }
	}
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
	for (int iTeam = 0; iTeam < MAX_CIV_TEAMS; ++iTeam)
	{
		if (iTeam != getID() && iTeam != eTeam)
		{
			CvTeam& kLoopTeam = GET_TEAM((TeamTypes)iTeam);
			if (kLoopTeam.isPermanentWarPeace(eTeam) != kLoopTeam.isPermanentWarPeace(getID()))
			{
				return false;
			}

			if (isPermanentWarPeace((TeamTypes)iTeam) != GET_TEAM(eTeam).isPermanentWarPeace((TeamTypes)iTeam))
			{
				return false;
			}
		}
	}

	return true;
}


int CvTeam::getAssets() const
{
	int iCount;
	int iI;

	iCount = 0;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				iCount += GET_PLAYER((PlayerTypes)iI).getAssets();
			}
		}
	}

	return iCount;
}


int CvTeam::getPower(bool bIncludeVassals) const
{
	int iCount;
	int iI;

	iCount = 0;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iI);
		if (kLoopPlayer.isAlive())
		{
			if (kLoopPlayer.getTeam() == getID() || (bIncludeVassals && GET_TEAM(kLoopPlayer.getTeam()).isVassal(getID())))
			{
				iCount += kLoopPlayer.getPower();

				if ( GET_TEAM(kLoopPlayer.getTeam()).isVassal(getID()) )
				{
					iCount -= kLoopPlayer.getTechPower();
				}
			}
		}
	}

	return iCount;
}

int CvTeam::getDefensivePower(TeamTypes eExcludeTeam) const
{
	int iCount;
	int iI;

	iCount = 0;

	FAssert(eExcludeTeam != getID());

	for (iI = 0; iI < MAX_CIV_TEAMS; iI++)
	{
		CvTeam& kLoopTeam = GET_TEAM((TeamTypes)iI);
		if (kLoopTeam.isAlive() && !kLoopTeam.isAVassal())
		{
			// K-Mod: added "eExcludeTeam" argument, so that defensive power can take into account the cancelation of pacts.
			if (iI != eExcludeTeam && (getID() == iI || isVassal((TeamTypes)iI) || isDefensivePact((TeamTypes)iI)))
			{
				iCount += kLoopTeam.getPower(true);
			}
		}
	}

	return iCount;
}

int CvTeam::getEnemyPower() const
{
	int iCount = 0;

	for (int iI = 0; iI < MAX_CIV_TEAMS; iI++)
	{
		CvTeam& kLoopTeam = GET_TEAM((TeamTypes)iI);
		if (kLoopTeam.isAlive())
		{
			if (getID() != iI && isAtWar((TeamTypes)iI))
			{
				iCount += kLoopTeam.getPower(false);
			}
		}
	}

	return iCount;
}


int CvTeam::getNumNukeUnits() const
{
	int iCount;
	int iI;

	iCount = 0;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iI);
		if (kLoopPlayer.isAlive())
		{
			if (kLoopPlayer.getTeam() == getID() || GET_TEAM(kLoopPlayer.getTeam()).isVassal(getID()))
			{
				iCount += kLoopPlayer.getNumNukeUnits();
			}
		}
	}

	return iCount;
}

int CvTeam::getVotes(VoteTypes eVote, VoteSourceTypes eVoteSource) const
{
	int iCount = 0;

	for (int iI = 0; iI < MAX_PLAYERS; iI++)
	{
		CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iI);
		if (kLoopPlayer.getTeam() == getID() && kLoopPlayer.isAlive())
		{
			iCount += kLoopPlayer.getVotes(eVote, eVoteSource);
		}
	}

	return iCount;
}

bool CvTeam::isVotingMember(VoteSourceTypes eVoteSource) const
{
	return (getVotes(NO_VOTE, eVoteSource) > 0);
}

bool CvTeam::isFullMember(VoteSourceTypes eVoteSource) const
{
	if (isForceTeamVoteEligible(eVoteSource))
	{
		return true;
	}

	for (int iI = 0; iI < MAX_PLAYERS; iI++)
	{
		CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iI);
		if (kLoopPlayer.getTeam() == getID() && kLoopPlayer.isAlive())
		{
			if (!kLoopPlayer.isFullMember(eVoteSource))
			{
				return false;
			}
		}
	}

	return true;
}

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      07/20/09                                jdog5000      */
/*                                                                                              */
/* General AI                                                                                   */
/************************************************************************************************/
int CvTeam::getAtWarCount(bool bIgnoreMinors, bool bIgnoreVassals) const
{
	int iCount;
	int iI;

	iCount = 0;

	for (iI = 0; iI < MAX_CIV_TEAMS; iI++)
	{
		if (GET_TEAM((TeamTypes)iI).isAlive())
		{
			if (!bIgnoreMinors || !(GET_TEAM((TeamTypes)iI).isMinorCiv()))
			{
				if( !bIgnoreVassals || !(GET_TEAM((TeamTypes)iI).isAVassal()))
				{
					if (isAtWar((TeamTypes)iI))
					{
						FAssert(iI != getID());
						FAssert(!(AI_isSneakAttackPreparing((TeamTypes)iI)));
						iCount++;
					}
				}
			}
		}
	}

	return iCount;
}
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

int CvTeam::getWarPlanCount(WarPlanTypes eWarPlan, bool bIgnoreMinors) const
{
	int iCount;
	int iI;

	iCount = 0;

	for (iI = 0; iI < MAX_CIV_TEAMS; iI++)
	{
		if (GET_TEAM((TeamTypes)iI).isAlive())
		{
			if (!bIgnoreMinors || !(GET_TEAM((TeamTypes)iI).isMinorCiv()))
			{
				if (AI_getWarPlan((TeamTypes)iI) == eWarPlan)
				{
					FAssert(iI != getID());
					iCount++;
				}
			}
		}
	}

	return iCount;
}


int CvTeam::getAnyWarPlanCount(bool bIgnoreMinors) const
{
	int iCount;
	int iI;

	iCount = 0;

	for (iI = 0; iI < MAX_CIV_TEAMS; iI++)
	{
		if (GET_TEAM((TeamTypes)iI).isAlive())
		{
			if (!bIgnoreMinors || !(GET_TEAM((TeamTypes)iI).isMinorCiv()))
			{
				if (AI_getWarPlan((TeamTypes)iI) != NO_WARPLAN)
				{
					FAssert(iI != getID());
					iCount++;
				}
			}
		}
	}

	FAssert(iCount >= getAtWarCount(bIgnoreMinors));

	return iCount;
}


int CvTeam::getChosenWarCount(bool bIgnoreMinors) const
{
	int iCount;
	int iI;

	iCount = 0;

	for (iI = 0; iI < MAX_CIV_TEAMS; iI++)
	{
		if (GET_TEAM((TeamTypes)iI).isAlive())
		{
			if (!bIgnoreMinors || !(GET_TEAM((TeamTypes)iI).isMinorCiv()))
			{
				if (AI_isChosenWar((TeamTypes)iI))
				{
					FAssert(iI != getID());
					iCount++;
				}
			}
		}
	}

	return iCount;
}


int CvTeam::getHasMetCivCount(bool bIgnoreMinors) const
{
	int iCount;
	int iI;

	iCount = 0;

	for (iI = 0; iI < MAX_CIV_TEAMS; iI++)
	{
		if (GET_TEAM((TeamTypes)iI).isAlive())
		{
			if (iI != getID())
			{
				if (!bIgnoreMinors || !(GET_TEAM((TeamTypes)iI).isMinorCiv()))
				{
					if (isHasMet((TeamTypes)iI))
					{
						FAssert(iI != getID());
						iCount++;
					}
				}
			}
		}
	}

	return iCount;
}


bool CvTeam::hasMetHuman() const
{
	int iI;

	for (iI = 0; iI < MAX_CIV_TEAMS; iI++)
	{
		if (GET_TEAM((TeamTypes)iI).isAlive())
		{
			if (iI != getID())
			{
				if (GET_TEAM((TeamTypes)iI).isHuman())
				{
					if (isHasMet((TeamTypes)iI))
					{
						FAssert(iI != getID());
						return true;
					}
				}
			}
		}
	}

	return false;
}


int CvTeam::getDefensivePactCount(TeamTypes eTeam) const
{
	int iCount;
	int iI;

	iCount = 0;

	for (iI = 0; iI < MAX_CIV_TEAMS; iI++)
	{
		if (GET_TEAM((TeamTypes)iI).isAlive())
		{
			if (iI != getID())
			{
				if (isDefensivePact((TeamTypes)iI))
				{
					if (NO_TEAM == eTeam || GET_TEAM(eTeam).isHasMet((TeamTypes)iI))
					{
						iCount++;
					}
				}
			}
		}
	}

	return iCount;
}

int CvTeam::getVassalCount(TeamTypes eTeam) const
{
	int iCount = 0;

	for (int iI = 0; iI < MAX_CIV_TEAMS; iI++)
	{
		CvTeam& kLoopTeam = GET_TEAM((TeamTypes)iI);
		if (kLoopTeam.isAlive())
		{
			if (iI != getID())
			{
				if (kLoopTeam.isVassal(getID()))
				{
					if (NO_TEAM == eTeam || GET_TEAM(eTeam).isHasMet((TeamTypes)iI))
					{
						iCount++;
					}
				}
			}
		}
	}

	return iCount;
}

bool CvTeam::isAVassal() const
{
	int iI;

	for (iI = 0; iI < MAX_CIV_TEAMS; iI++)
	{
		if (GET_TEAM((TeamTypes)iI).isAlive())
		{
			if (iI != getID())
			{
				if (isVassal((TeamTypes)iI))
				{
					FAssert(iI != getID());
					return true;
				}
			}
		}
	}

	return false;
}

bool CvTeam::canVassalRevolt(TeamTypes eMaster) const
{
	FAssert(NO_TEAM != eMaster);

	CvTeam& kMaster = GET_TEAM(eMaster);

	if (isVassal(eMaster))
	{
		if (100 * getTotalLand(false) < GC.getDefineINT("VASSAL_REVOLT_OWN_LOSSES_FACTOR") * getVassalPower())
		{
			return true;
		}

		if (100 * kMaster.getTotalLand() < GC.getDefineINT("VASSAL_REVOLT_MASTER_LOSSES_FACTOR") * getMasterPower() && (100 * GET_PLAYER(getLeaderID()).getUnitPower() > GET_PLAYER(kMaster.getLeaderID()).getUnitPower() * (10 + GC.getDefineINT("FREE_VASSAL_MILITARY_PERCENT"))))
		{
			return true;
		}
	}

	if (GC.getDefineINT("FREE_VASSAL_MILITARY_PERCENT") < 0 || 
		100 * GET_PLAYER(getLeaderID()).getUnitPower() > GET_PLAYER(kMaster.getLeaderID()).getUnitPower() * GC.getDefineINT("FREE_VASSAL_MILITARY_PERCENT"))
	{
		return true;
	}
	
	if (GC.getDefineINT("FREE_VASSAL_LAND_PERCENT") < 0 || 
		100 * getTotalLand(false) < kMaster.getTotalLand(false) * GC.getDefineINT("FREE_VASSAL_LAND_PERCENT"))
	{
		return false;
	}

	if (GC.getDefineINT("FREE_VASSAL_POPULATION_PERCENT") < 0 || 
		100 * getTotalPopulation(false) < kMaster.getTotalPopulation(false) * GC.getDefineINT("FREE_VASSAL_POPULATION_PERCENT"))
	{
		return false;
	}

	return true;
}

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      07/20/09                                jdog5000      */
/*                                                                                              */
/* General AI                                                                                   */
/************************************************************************************************/
int CvTeam::getCurrentMasterPower(bool bIncludeVassals) const
{
	if( isAVassal() )
	{
		for( int iI = 0; iI < MAX_CIV_TEAMS; iI++)
		{
			if (GET_TEAM((TeamTypes)iI).isAlive())
			{
				if (iI != getID())
				{
					if (isVassal((TeamTypes)iI))
					{
						return GET_TEAM((TeamTypes)iI).getPower(bIncludeVassals);
					}
				}
			}
		}
	}

	// Should never get here
	FAssert(false);
	return 0;
}

bool CvTeam::isMasterPlanningLandWar(CvArea* pArea)
{
	if( !isAVassal() )
	{
		return false;
	}

	if( (pArea->getAreaAIType(getID()) == AREAAI_OFFENSIVE) || (pArea->getAreaAIType(getID()) == AREAAI_DEFENSIVE) || (pArea->getAreaAIType(getID()) == AREAAI_MASSING) )
	{
		return true;
	}

	for( int iI = 0; iI < MAX_CIV_TEAMS; iI++ )
	{
		if( isVassal((TeamTypes)iI) )
		{
			if( GET_TEAM((TeamTypes)iI).getAnyWarPlanCount(true) > 0 )
			{
				if( (pArea->getAreaAIType((TeamTypes)iI) == AREAAI_OFFENSIVE) || (pArea->getAreaAIType((TeamTypes)iI) == AREAAI_DEFENSIVE) || (pArea->getAreaAIType((TeamTypes)iI) == AREAAI_MASSING) )
				{
					return true;
				}
				else if( pArea->getAreaAIType((TeamTypes)iI) == AREAAI_NEUTRAL )
				{
					// Master has no presence here
					if( (pArea->getNumCities() - countNumCitiesByArea(pArea)) > 2 )
					{
						return (GC.getGameINLINE().getSorenRandNum((isCapitulated() ? 6 : 4),"Vassal land war") == 0);
					}
				}
			}
			else if( GET_TEAM((TeamTypes)iI).isHuman() )
			{
				if( GC.getBBAI_HUMAN_VASSAL_WAR_BUILD() )
				{
					if( (pArea->getNumCities() - countNumCitiesByArea(pArea) - GET_TEAM((TeamTypes)iI).countNumCitiesByArea(pArea)) > 2 )
					{
						return (GC.getGameINLINE().getSorenRandNum(4,"Vassal land war") == 0);
					}
				}
			}

			break;
		}
	}

	return false;
}

bool CvTeam::isMasterPlanningSeaWar(CvArea* pArea)
{
	if( !isAVassal() )
	{
		return false;
	}

	if( (pArea->getAreaAIType(getID()) == AREAAI_ASSAULT) || (pArea->getAreaAIType(getID()) == AREAAI_ASSAULT_ASSIST) || (pArea->getAreaAIType(getID()) == AREAAI_ASSAULT_MASSING) )
	{
		return true;
	}

	for( int iI = 0; iI < MAX_CIV_TEAMS; iI++ )
	{
		if( isVassal((TeamTypes)iI) )
		{
			if( GET_TEAM((TeamTypes)iI).getAnyWarPlanCount(true) > 0 )
			{
				if( (pArea->getAreaAIType((TeamTypes)iI) == AREAAI_ASSAULT) || (pArea->getAreaAIType((TeamTypes)iI) == AREAAI_ASSAULT_ASSIST) || (pArea->getAreaAIType((TeamTypes)iI) == AREAAI_ASSAULT_MASSING) )
				{
					return (GC.getGameINLINE().getSorenRandNum((isCapitulated() ? 3 : 2),"Vassal sea war") == 0);
				}
				else if( pArea->getAreaAIType((TeamTypes)iI) == AREAAI_NEUTRAL )
				{
					// Master has no presence here
					return false;
				}

			}
			else if( GET_TEAM((TeamTypes)iI).isHuman() )
			{
				return false;
			}

			break;
		}
	}

	return false;
}
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

int CvTeam::getUnitClassMaking(UnitClassTypes eUnitClass) const
{
	int iCount;
	int iI;

	iCount = 0;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				iCount += GET_PLAYER((PlayerTypes)iI).getUnitClassMaking(eUnitClass);
			}
		}
	}

	return iCount;
}


int CvTeam::getUnitClassCountPlusMaking(UnitClassTypes eIndex) const
{
	return (getUnitClassCount(eIndex) + getUnitClassMaking(eIndex));
}


int CvTeam::getBuildingClassMaking(BuildingClassTypes eBuildingClass) const
{
	int iCount;
	int iI;

	iCount = 0;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				iCount += GET_PLAYER((PlayerTypes)iI).getBuildingClassMaking(eBuildingClass);
			}
		}
	}

	return iCount;
}


int CvTeam::getBuildingClassCountPlusMaking(BuildingClassTypes eIndex) const
{
	return (getBuildingClassCount(eIndex) + getBuildingClassMaking(eIndex));
}


int CvTeam::getHasReligionCount(ReligionTypes eReligion) const
{
	int iCount;
	int iI;

	iCount = 0;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				iCount += GET_PLAYER((PlayerTypes)iI).getHasReligionCount(eReligion);
			}
		}
	}

	return iCount;
}


int CvTeam::getHasCorporationCount(CorporationTypes eCorporation) const
{
	int iCount = 0;

	for (int iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				iCount += GET_PLAYER((PlayerTypes)iI).getHasCorporationCount(eCorporation);
			}
		}
	}

	return iCount;
}

int CvTeam::processedTeamCulture()
{
	int iCulture;
	if (countTotalCulture() < MAX_INT)
	{
		iCulture = (int)countTotalCulture();
	}
	else
	{
		iCulture = MAX_INT;
	}
	return iCulture;
}


unsigned long long CvTeam::countTotalCulture()
{
	unsigned long long iCount;
	int iI;

	iCount = 0;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				iCount += GET_PLAYER((PlayerTypes)iI).countTotalCulture();
			}
		}
	}

	return iCount;
}


int CvTeam::countNumUnitsByArea(CvArea* pArea) const
{
	PROFILE_FUNC();

	int iCount;
	int iI;

	iCount = 0;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				iCount += pArea->getUnitsPerPlayer((PlayerTypes)iI);
			}
		}
	}

	return iCount;
}


int CvTeam::countNumCitiesByArea(CvArea* pArea) const
{
	PROFILE_FUNC();

	int iCount;
	int iI;

	iCount = 0;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				iCount += pArea->getCitiesPerPlayer((PlayerTypes)iI);
			}
		}
	}

	return iCount;
}

// f1rpo (advc.107) from K-Mod
int CvTeam::countEnemyCitiesByArea(CvArea const* pArea) const
{
	int iCount = 0;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		CvPlayer const& kEnemy = GET_PLAYER((PlayerTypes)i);
		if (!kEnemy.isAlive())
			continue;
		if (AI_getWarPlan(kEnemy.getTeam()) != NO_WARPLAN)
			iCount += pArea->getCitiesPerPlayer(kEnemy.getID());
	}
	return iCount;
}


int CvTeam::countTotalPopulationByArea(CvArea* pArea) const
{
	int iCount;
	int iI;

	iCount = 0;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				iCount += pArea->getPopulationPerPlayer((PlayerTypes)iI);
			}
		}
	}

	return iCount;
}


int CvTeam::countPowerByArea(CvArea* pArea) const
{
	int iCount;
	int iI;

	iCount = 0;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				iCount += pArea->getPower((PlayerTypes)iI);
			}
		}
	}

	return iCount;
}


int CvTeam::countEnemyPowerByArea(CvArea* pArea) const
{
	int iCount;
	int iI;

	iCount = 0;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() != getID())
			{
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      01/11/09                                jdog5000      */
/*                                                                                              */
/* General AI                                                                                   */
/************************************************************************************************/
/* original BTS code
				if (isAtWar(GET_PLAYER((PlayerTypes)iI).getTeam()))
*/
				// Count planned wars as well
				if (AI_getWarPlan(GET_PLAYER((PlayerTypes)iI).getTeam()) != NO_WARPLAN)
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/
				{
					iCount += pArea->getPower((PlayerTypes)iI);
				}
			}
		}
	}

	return iCount;
}

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      04/01/10                                jdog5000      */
/*                                                                                              */
/* War strategy AI                                                                              */
/************************************************************************************************/
int CvTeam::countEnemyPopulationByArea(CvArea* pArea) const
{
	int iCount;
	int iI;

	iCount = 0;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() != getID())
			{
				WarPlanTypes eWarPlan = AI_getWarPlan(GET_PLAYER((PlayerTypes)iI).getTeam());
				if( eWarPlan != NO_WARPLAN )
				{
					int iContribution = pArea->getPopulationPerPlayer((PlayerTypes)iI);

					if ( eWarPlan == WARPLAN_LIMITED && (isMinorCiv() || GET_TEAM(GET_PLAYER((PlayerTypes)iI).getTeam()).isMinorCiv()) )
					{
						iContribution /= 2;
					}

					iCount += iContribution;
				}
			}
		}
	}

	return iCount;
}
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/


int CvTeam::countNumAIUnitsByArea(CvArea* pArea, UnitAITypes eUnitAI) const
{
	PROFILE_FUNC();

	int iCount;
	int iI;

	iCount = 0;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				iCount += pArea->getNumAIUnits(((PlayerTypes)iI), eUnitAI);
			}
		}
	}

	return iCount;
}

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      05/19/10                                jdog5000      */
/*                                                                                              */
/* War strategy AI                                                                              */
/************************************************************************************************/
int CvTeam::countEnemyDangerByArea(CvArea* pArea, TeamTypes eEnemyTeam ) const
{
	PROFILE_FUNC();

	CvPlot* pLoopPlot;
	int iCount;
	int iI;

	iCount = 0;

	for (iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
	{
		pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);

		if (pLoopPlot != NULL)
		{
			if (pLoopPlot->area() == pArea)
			{
				if (pLoopPlot->getTeam() == getID())
				{
					//	Koshling - don't count animals - they are not actively attacking us so they shouldn't trigger
					//	things like a defensive stance
					iCount += pLoopPlot->plotCount(PUF_canDefendEnemyNoAnimal, getLeaderID(), 0, NO_PLAYER, eEnemyTeam, PUF_isVisible, getLeaderID());
				}
			}
		}
	}

	return iCount;
}
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/


int CvTeam::getResearchCost(TechTypes eTech) const
{
	int iCost;
	int iFinalMultiplier = 1;

	FAssertMsg(eTech != NO_TECH, "Tech is not assigned a valid value");

	iCost = GC.getTechInfo(eTech).getResearchCost();

	if ( iCost > MAX_INT/10000 )
	{
		iCost /= 10000;
		iFinalMultiplier = 10000;
	}

	//	Arkatakor
	if (!isHuman() && !isBarbarian())
	{
		//	The handicap type will be based on the average handicap of all human players
		iCost *= GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIResearchPercent();
	}
	else
	{
		iCost *= GC.getHandicapInfo(getHandicapType()).getResearchPercent();
	}
	
	iCost /= 100;	

	if (!isHuman() && !isBarbarian())
	{		
	//Afforess AI Flexible Difficulty Start
		if (GC.getGameINLINE().isModderGameOption(MODDERGAMEOPTION_AI_USE_FLEXIBLE_DIFFICULTY))
		{
			//45deg: Switched off AIResearch percent; now iAIResearchPercent will be the same for every handicap level, it will only be used as an offset and to 
			//tweak balance between human and AI research rate (see use of AIResearchPercent above)
			//HandicapTypes eStandardDifficulty = (HandicapTypes)GC.getDefineINT("STANDARD_HANDICAP");
			//iCost *= std::max(50, 100 - (GC.getHandicapInfo(getHandicapType()).getAIResearchPercent() - GC.getHandicapInfo(eStandardDifficulty).getAIResearchPercent()));
			iCost *= GC.getHandicapInfo(getHandicapType()).getResearchPercent(); //45deg: now AI is using the same Research rate as human players
			iCost /= 100;
		}
	//Afforess AI Flexible Difficulty End
	}

	iCost *= GC.getWorldInfo(GC.getMapINLINE().getWorldSize()).getResearchPercent();
	iCost /= 100;

	iCost *= GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getResearchPercent();
	iCost /= 100;

	iCost *= GC.getEraInfo(GC.getGameINLINE().getStartEra()).getResearchPercent();
	iCost /= 100;

	iCost *= std::max(0, ((GC.getDefineINT("TECH_COST_EXTRA_TEAM_MEMBER_MODIFIER") * (getNumMembers() - 1)) + 100));
	iCost /= 100;
	
	if (!GC.getGameINLINE().isOption(GAMEOPTION_NO_REVOLUTION))
	{	
		iCost *= GC.getDefineINT("REVOLUTION_TECH_COST_RATIO");
		iCost /= 100;
	}
//45deg - START 	
// Attempt to make tech runaway hard to achieve and to avoid civs falling behind in techs; if any civ is far away in terms of tech from where it's supposed to be
// it gets help or it's hindered by respectively decreasing or increasing tech cost; that is, compared to the "real" timeline of our world
	if (GC.getGameINLINE().isModderGameOption(MODDERGAMEOPTION_REALISTIC_TIMESCALE))
	{
		float fEstimateEndTurn = 0;
		for (int iI = 0; iI < GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getNumTurnIncrements(); iI++)
		{
			fEstimateEndTurn += GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getGameTurnInfo(iI).iNumGameTurnsPerIncrement;
		}
		
		fEstimateEndTurn *= (100- (GC.getEraInfo(GC.getGameINLINE().getStartEra()).getStartPercent()));
		fEstimateEndTurn /= 100;		
			
		//fEstimateEndTurn -= GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getGoldenAgePercent() / 20; // leave 5 turns at the end of the game for Future Techs (blitz gamespeed), scaled for gamespeed
		EraTypes eStartEra = (EraTypes)GC.getGame().getStartEra();
		//EraTypes eEndEra = (EraTypes)((GC.getGameINLINE().isOption(GAMEOPTION_NO_FUTURE)) ? GC.getInfoTypeForString("ERA_MODERN") : GC.getNumEraInfos() - 1);
		// f1rpo (era options; cf. CvGame::doTurn)
		EraTypes eEndEra = (EraTypes)(GC.getGame().getPenultimateEra() + 1);
		int iEraCount = /* f1rpo: bugfix */ std::max(1,
				(int)eEndEra - (int)eStartEra);

		//Count only the number of techs researchable by players in the game (starting from the start era, to the end era)
		int iTechCount = 0;
		for (int iI = 0; iI < GC.getNumTechInfos(); iI++)
		{
			EraTypes eEra = (EraTypes)GC.getTechInfo((TechTypes)iI).getEra();
			if (eEra >= eStartEra && eEra <= eEndEra)
			{
				iTechCount++;
			}
		}
		//Assuming we divide turns evenly per era
		float fExpectedEra = (GC.getGameINLINE().getElapsedGameTurns() / (fEstimateEndTurn / iEraCount)) + eStartEra;
		float fPlayerEra = (float)GET_PLAYER(getLeaderID()).getCurrentEra();
		for (int iI = 0; iI < GC.getNumTechInfos(); iI++)	// 45deg: assuming we have the same number of techs per era; it's not true but it's a good approximation for our puroposes
		{
			if (!(isHasTech((TechTypes)iI)))
			{
				if (GC.getTechInfo((TechTypes)iI).getEra() < GET_PLAYER(getLeaderID()).getCurrentEra())
				{		
					fPlayerEra -= (float)(1.0/(iTechCount/iEraCount));	// 45deg: added this term to compensate techs left behind from previous eras; if I get Alphabet but I still miss half of Ancient Era techs, I'm not truly on Classical
				}
			}
			else
			{
				if (GC.getTechInfo((TechTypes)iI).getEra() >= GET_PLAYER(getLeaderID()).getCurrentEra())	//45deg: on the other hand if I have more techs of current era (say Classical), I'm a bit closer to next era
				{
					fPlayerEra += (float)(1.0/(iTechCount/iEraCount));	
				}
			}
		}

		int iModifier = (int)(100 * (pow((float)4, (fPlayerEra - fExpectedEra))));
		iModifier += 10;
		iModifier *= 10;
		iModifier /= 11;
		
		iCost *= iModifier;
		iCost /= 100;
	}
	if (isBarbarian())	//45deg: barbarians get too advanced otherwise
	{
		iCost *= 3;
		if ((GC.getGameINLINE().isOption(GAMEOPTION_RAGING_BARBARIANS)) || (GC.getGameINLINE().isOption(GAMEOPTION_BARBARIAN_WORLD)))
		{	
			iCost *= 4;
		}
	}
//45deg - END		
	return std::max(1, iCost*iFinalMultiplier);
}


int CvTeam::getResearchLeft(TechTypes eTech) const
{
	return std::max(0, (getResearchCost(eTech) - getResearchProgress(eTech)));
}


bool CvTeam::hasHolyCity(ReligionTypes eReligion) const
{
	CvCity* pHolyCity;

	FAssertMsg(eReligion != NO_RELIGION, "Religion is not assigned a valid value");

	pHolyCity = GC.getGameINLINE().getHolyCity(eReligion);

	if (pHolyCity != NULL)
	{
		return (pHolyCity->getTeam() == getID());
	}

	return false;
}


bool CvTeam::hasHeadquarters(CorporationTypes eCorporation) const
{
	FAssertMsg(eCorporation != NO_CORPORATION, "Corporation is not assigned a valid value");

	CvCity* pHeadquarters = GC.getGameINLINE().getHeadquarters(eCorporation);

	if (pHeadquarters != NULL)
	{
		return (pHeadquarters->getTeam() == getID());
	}

	return false;
}

bool CvTeam::hasBonus(BonusTypes eBonus) const
{
	for (int iI = 0; iI < MAX_PLAYERS; ++iI)
	{
		if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID() && GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).hasBonus(eBonus))
			{
				return true;
			}
		}
	}

	return false;
}

bool CvTeam::isBonusObsolete(BonusTypes eBonus) const
{
	TechTypes eObsoleteTech = (TechTypes) GC.getBonusInfo(eBonus).getTechObsolete();
	if (eObsoleteTech != NO_TECH && isHasTech(eObsoleteTech))
	{
		return true;
	}
	return false;
}


bool CvTeam::isHuman() const
{
	PROFILE_FUNC();

	int iI;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID() && GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).isHuman())
			{
				return true;
			}
		}
	}

	return false;
}


bool CvTeam::isBarbarian() const
{
	return (getID() == BARBARIAN_TEAM);
}


bool CvTeam::isMinorCiv() const
{
	bool bValid = false;

	for (int iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID() && GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (!GET_PLAYER((PlayerTypes)iI).isMinorCiv())
			{
				return false;
			}
			bValid = true;
		}
	}
	return bValid;
}

/************************************************************************************************/
/* REVOLUTION_MOD                         10/22/08                                jdog5000      */
/*                                                                                              */
/* For Minor Civs                                                                               */
/************************************************************************************************/
void CvTeam::setIsMinorCiv( bool bNewValue, bool bDoBarbCivCheck )
{
	int iI;
	if( bNewValue != isMinorCiv() )
	{
		if( isAlive() )
		{
			if( bNewValue )
			{
				logMsg( "Switching team %d to minor", getID() );
			}
			else
			{
				logMsg( "Switching minor team %d to full", getID() );
			}
		}
		else
		{
			if( bNewValue )
			{
				logMsg( "Setting non-alive team %d to minor", getID() );
			}
			else
			{
				logMsg( "Setting non-alive minor team %d to full", getID() );
			}
		}

		bool abHasMet[MAX_TEAMS];
		TeamTypes eBarbCivVictim = NO_TEAM;

		// Have to check hasMet status first because of vassal states ... dow on master causes hasMet for vassal
		for (iI = 0; iI < MAX_TEAMS; iI++)
		{
			abHasMet[iI] = isHasMet((TeamTypes)iI);
		}

		if ( isAlive() )
		{
			if( bDoBarbCivCheck && !bNewValue )
			{	
				int iMaxVal = 0;
				for (int iJ = 0; iJ < MAX_CIV_TEAMS; iJ++)
				{
					if( iJ != getID() )
					{
						int iValue = GET_TEAM(getID()).AI_getBarbarianCivWarVal((TeamTypes)iJ, 12);
						if( iValue > iMaxVal )
						{
							logMsg("    BarbCiv team %d is considering declaring war against victim Team %d", getID(), iJ);
							CvCity* pCapital = GET_PLAYER(getLeaderID()).getCapitalCity();
							if( pCapital == NULL || pCapital->plot()->isHasPathToPlayerCity(getID(),GET_TEAM((TeamTypes)iJ).getLeaderID()) )
							{
								iMaxVal = iValue;
								eBarbCivVictim = (TeamTypes)iJ;
							}
						}
					}
				}

				logMsg("    BarbCiv team %d will declare war against victim Team %d", getID(), eBarbCivVictim);
			}
		}

/*		if( !(GC.getGameINLINE().isOption(GAMEOPTION_START_AS_MINORS)) )
		{
			logMsg("    Making peace with other players");
			// Declare peace in either case, remove contact causes "Nice to meet you" diplo screens
			for (iI = 0; iI < MAX_CIV_TEAMS; iI++)
			{
				if( iI != getID() )
				{
					if( isAtWar((TeamTypes)iI) && !GET_TEAM((TeamTypes)iI).isBarbarian() && !GET_TEAM((TeamTypes)iI).isMinorCiv() )
					{
						makePeace((TeamTypes)iI, false);
					}

					setHasMet( (TeamTypes)iI, false );
					setHasMet( getID(), false );
				}
			}
		}
*/

		if( bNewValue )
		{
			// Convert all team members
			for(iI = 0; iI < MAX_CIV_PLAYERS; iI++)
			{
				if( GET_PLAYER((PlayerTypes)iI).getTeam() == getID() )
				{
					GC.getInitCore().setMinorNationCiv((PlayerTypes)iI, bNewValue);
				}
			}

			// Declare war on all outside teams
			declareWarAsMinor();
		}
		else
		{
			// Keep war againt those this team has met
			for (iI = 0; iI < MAX_CIV_TEAMS; iI++)
			{
				if( iI != getID() && GET_TEAM((TeamTypes)iI).isAlive() && !(GET_TEAM((TeamTypes)iI).isBarbarian()) && !(GET_TEAM((TeamTypes)iI).isMinorCiv()) )
				{
					if( abHasMet[iI] )
					{
						if( GC.getGameINLINE().isOption(GAMEOPTION_START_AS_MINORS) )
						{
							if( isAlive() && !isAtWar((TeamTypes)iI) )
							{
								declareWar((TeamTypes)iI, true, NO_WARPLAN);
							}
						}
						else
						{
							// Does other player want to keep war with us?
							bool bPeace = true;
							bool bPlanWar = false;

							CvPlayer& kPlayer = GET_PLAYER(GET_TEAM((TeamTypes)iI).getLeaderID());
							int iLoop = 0;
							int iCount = 0;
							for (CvUnit* pLoopUnit = kPlayer.firstUnit(&iLoop); pLoopUnit; pLoopUnit = kPlayer.nextUnit(&iLoop))
							{
								if( pLoopUnit->plot()->getTeam() == getID() )
								{
									iCount++;
								}
							}
							
							if( GET_TEAM((TeamTypes)iI).isHuman() )
							{
								if( iCount > 2 )
								{
									bPeace = false;
								}
							}
							else
							{
								if( !isAtWar((TeamTypes)iI) )
								{
									// Shouldn't happen
									bPeace = false;
								}
								else if( GET_TEAM((TeamTypes)iI).AI_minorKeepWarVal(getID()) > 0 )
								{
									if( GET_TEAM((TeamTypes)iI).AI_endWarVal(getID()) < (GET_TEAM(getID()).AI_endWarVal((TeamTypes)iI))/2 )
									{
										if( iCount > 2 || GET_TEAM((TeamTypes)iI).AI_getWarSuccess(getID()) > GC.getDefineINT("WAR_SUCCESS_CITY_CAPTURING") )
										{
											bPeace = false;
										}
										else
										{
											bPlanWar = true;
										}
									}
								}
							}

							if( bPeace )
							{
								makePeace((TeamTypes)iI, false);
						
								setHasMet( (TeamTypes)iI, false );
								GET_TEAM((TeamTypes)iI).setHasMet( getID(), false );

								if( bPlanWar )
								{
									GET_TEAM((TeamTypes)iI).AI_setWarPlan(getID(),WARPLAN_LIMITED);
									logMsg("    Team %d decides to plan war against ex-minor", iI);
								}
							}
							else
							{
								logMsg("    Team %d decides to keep war against ex-minor", iI);
							}
						}
					}
					else
					{
						if( isAtWar((TeamTypes)iI) && !GET_TEAM((TeamTypes)iI).isBarbarian() && !GET_TEAM((TeamTypes)iI).isMinorCiv() )
						{
							makePeace((TeamTypes)iI, false);
						
							setHasMet( (TeamTypes)iI, false );
							GET_TEAM((TeamTypes)iI).setHasMet( getID(), false );
						}
					}

					if( isAtWar((TeamTypes)iI) )
					{
						AI_setWarPlan((TeamTypes)iI, WARPLAN_LIMITED, true);
						GET_TEAM((TeamTypes)iI).AI_setWarPlan(getID(), WARPLAN_LIMITED, true);
					}
				}
			}

			if( eBarbCivVictim != NO_TEAM )
			{
				if( !isAtWar(eBarbCivVictim) )
				{
					if( getAnyWarPlanCount(true) == 0 )
					{
						int iLoop = 0;
						int iCount = 0;
						for (CvUnit* pLoopUnit = GET_PLAYER(getLeaderID()).firstUnit(&iLoop); pLoopUnit; pLoopUnit = GET_PLAYER(getLeaderID()).nextUnit(&iLoop))
						{
							if( pLoopUnit->plot()->getTeam() == eBarbCivVictim )
							{
								iCount++;
							}
						}

						if( iCount > 2 || GET_TEAM((TeamTypes)iI).AI_getWarSuccess(getID()) > GC.getDefineINT("WAR_SUCCESS_CITY_CAPTURING") )
						{
							logMsg("  Barb civ %d decides to keep war on victim Team %d", getID(), eBarbCivVictim);
							declareWar((TeamTypes)iI, true, WARPLAN_TOTAL); 
						}
						else
						{
							logMsg("  Barb civ %d begins preparing for war on victim Team %d", getID(), eBarbCivVictim);
							// Prepare for war with victim
							AI_setWarPlan(eBarbCivVictim,WARPLAN_TOTAL);
						}
					}
				}
				else
				{
					AI_setWarPlan(eBarbCivVictim, WARPLAN_TOTAL, true);
				}
			}

			// Convert all team members
			for(iI = 0; iI < MAX_CIV_PLAYERS; iI++)
			{
				if( GET_PLAYER((PlayerTypes)iI).getTeam() == getID() )
				{
					GC.getInitCore().setMinorNationCiv((PlayerTypes)iI, bNewValue);
				}
			}
		}

		// Update graphics to reflect changes
		gDLL->getInterfaceIFace()->setDirty(CityInfo_DIRTY_BIT, true);
		gDLL->getInterfaceIFace()->setDirty(UnitInfo_DIRTY_BIT, true);
		gDLL->getInterfaceIFace()->setDirty(InfoPane_DIRTY_BIT, true);
		gDLL->getInterfaceIFace()->setDirty(GlobeLayer_DIRTY_BIT, true);
		gDLL->getInterfaceIFace()->setDirty(Flag_DIRTY_BIT, true);
		gDLL->getInterfaceIFace()->setDirty(Score_DIRTY_BIT, true);
		gDLL->getInterfaceIFace()->setDirty(Foreign_Screen_DIRTY_BIT, true);
		gDLL->getInterfaceIFace()->setDirty(GlobeInfo_DIRTY_BIT, true);
	
	}

	return;
}

void	CvTeam::declareWarAsMinor()
{
	if ( isAlive() )
	{
		for (int iI = 0; iI < MAX_CIV_TEAMS; iI++)
		{
			// Declare war on everyone except barbs, barbs always do their own declaring
			if( iI != getID() && !isAtWar((TeamTypes)iI) )
			{
				bool bHasMet = isHasMet((TeamTypes)iI);

				declareWar((TeamTypes)iI, false, WARPLAN_LIMITED);
				if( !bHasMet )
				{
					// Declaring war creates contacts, remove those the player didn't have already
					setHasMet( (TeamTypes)iI, false );
					GET_TEAM((TeamTypes)iI).setHasMet( getID(), false );
				}
			}

			if( isAtWar((TeamTypes)iI) )
			{
				AI_setWarPlan((TeamTypes)iI, WARPLAN_LIMITED, true);
				GET_TEAM((TeamTypes)iI).AI_setWarPlan(getID(), WARPLAN_LIMITED, true);
			}
		}
	}
}

/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/

PlayerTypes CvTeam::getLeaderID() const
{
	int iI;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				return ((PlayerTypes)iI);
			}
		}
	}

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
		{
			return ((PlayerTypes)iI);
		}
	}

	return NO_PLAYER;
}


PlayerTypes CvTeam::getSecretaryID() const
{
	int iI;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).isHuman())
			{
				if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
				{
					return ((PlayerTypes)iI);
				}
			}
		}
	}

	return getLeaderID();
}


HandicapTypes CvTeam::getHandicapType() const
{
	int iGameHandicap;
	int iCount;
	int iI;

	iGameHandicap = 0;
	iCount = 0;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				iGameHandicap += GET_PLAYER((PlayerTypes)iI).getHandicapType();
				iCount++;
			}
		}
	}

	if (iCount > 0)
	{
		FAssertMsg((iGameHandicap / iCount) >= 0, "(iGameHandicap / iCount) is expected to be non-negative (invalid Index)");
		return ((HandicapTypes)(iGameHandicap / iCount));
	}
	else
	{
		return ((HandicapTypes)(GC.getDefineINT("STANDARD_HANDICAP")));
	}
}


CvWString CvTeam::getName() const
{
	CvWString szBuffer;
	bool bFirst;
	int iI;

	bFirst = true;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive() && GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
		{
			setListHelp(szBuffer, L"", GET_PLAYER((PlayerTypes)iI).getName(), L"/", bFirst);
			bFirst = false;
		}
	}

	return szBuffer;
}


int CvTeam::getNumMembers() const
{
	return m_iNumMembers;
}


void CvTeam::changeNumMembers(int iChange)
{
	m_iNumMembers += iChange;
	FAssert(getNumMembers() >= 0);
}


int CvTeam::getAliveCount() const
{
	return m_iAliveCount;
}


int CvTeam::isAlive() const
{
	return (getAliveCount() > 0);
}


void CvTeam::changeAliveCount(int iChange)
{
	m_iAliveCount += iChange;
	FAssert(getAliveCount() >= 0);

	// free vassals
	if (0 == m_iAliveCount)
	{
		for (int iTeam = 0; iTeam < MAX_TEAMS; iTeam++)
		{
			if (iTeam != getID())
			{
				CvTeam& kLoopTeam = GET_TEAM((TeamTypes)iTeam);
				if (kLoopTeam.isAlive() && kLoopTeam.isVassal(getID()))
				{
					kLoopTeam.setVassal(getID(), false, false);
				}
			}
		}
	}
}


int CvTeam::getEverAliveCount() const
{
	return m_iEverAliveCount;
}


int CvTeam::isEverAlive() const
{
	return (getEverAliveCount() > 0);
}


void CvTeam::changeEverAliveCount(int iChange)
{
	m_iEverAliveCount += iChange;
	FAssert(getEverAliveCount() >= 0);
}


int CvTeam::getNumCities() const														
{
	return m_iNumCities;
}


void CvTeam::changeNumCities(int iChange)							
{
	m_iNumCities += iChange;
	FAssert(getNumCities() >= 0);
}


int CvTeam::getTotalPopulation(bool bCheckVassals) const											
{
	int iVassalPop = 0;

	if (bCheckVassals)
	{
		if (isAVassal())
		{
			return m_iTotalPopulation / 2;
		}

		for (int iI = 0; iI < MAX_TEAMS; iI++)
		{
			if ((TeamTypes)iI != getID())
			{
				CvTeam& kLoopTeam = GET_TEAM((TeamTypes)iI);
				if (kLoopTeam.isAlive() && kLoopTeam.isVassal(getID()))
				{
					iVassalPop += kLoopTeam.getTotalPopulation(false) / 2;
				}
			}
		}
	}

	return (m_iTotalPopulation + iVassalPop);
}


void CvTeam::changeTotalPopulation(int iChange)	
{
	m_iTotalPopulation += iChange;
	FAssert(getTotalPopulation() >= 0);
}


int CvTeam::getTotalLand(bool bCheckVassals) const
{
	int iVassalLand = 0;

	if (bCheckVassals)
	{
		if (isAVassal())
		{
			return m_iTotalLand / 2;
		}

		for (int iI = 0; iI < MAX_TEAMS; iI++)
		{
			if ((TeamTypes)iI != getID())
			{
				CvTeam& kLoopTeam = GET_TEAM((TeamTypes)iI);
				if (kLoopTeam.isAlive() && kLoopTeam.isVassal(getID()))
				{
					iVassalLand += kLoopTeam.getTotalLand(false) / 2;
				}
			}
		}
	}

	return (m_iTotalLand + iVassalLand);
}


void CvTeam::changeTotalLand(int iChange)														
{
	m_iTotalLand += iChange;
	FAssert(getTotalLand() >= 0);
}


int CvTeam::getNukeInterception() const
{
	return m_iNukeInterception;
}


void CvTeam::changeNukeInterception(int iChange)
{
	m_iNukeInterception += iChange;
	FAssert(getNukeInterception() >= 0);
}


int CvTeam::getForceTeamVoteEligibilityCount(VoteSourceTypes eVoteSource) const
{
	return m_aiForceTeamVoteEligibilityCount[eVoteSource];
}


bool CvTeam::isForceTeamVoteEligible(VoteSourceTypes eVoteSource) const
{
	return ((getForceTeamVoteEligibilityCount(eVoteSource) > 0) && !isMinorCiv());
}


void CvTeam::changeForceTeamVoteEligibilityCount(VoteSourceTypes eVoteSource, int iChange)
{
	int iOldValue;

	//	Do this in a thread-safe manner
	do
	{
		iOldValue = m_aiForceTeamVoteEligibilityCount[eVoteSource];
	} while( InterlockedCompareExchange((volatile LONG*)&m_aiForceTeamVoteEligibilityCount[eVoteSource], iOldValue + iChange, iOldValue) != iOldValue );
	//m_aiForceTeamVoteEligibilityCount[eVoteSource] += iChange;
	FAssert(getForceTeamVoteEligibilityCount(eVoteSource) >= 0);
}


int CvTeam::getExtraWaterSeeFromCount() const
{
	return m_iExtraWaterSeeFromCount;
}


bool CvTeam::isExtraWaterSeeFrom() const
{
	return (getExtraWaterSeeFromCount() > 0);
}


void CvTeam::changeExtraWaterSeeFromCount(int iChange)
{
	if (iChange != 0)
	{
		GC.getMapINLINE().updateSight(false);

		m_iExtraWaterSeeFromCount = (m_iExtraWaterSeeFromCount + iChange);
		FAssert(getExtraWaterSeeFromCount() >= 0);

		GC.getMapINLINE().updateSight(true);
	}
}


int CvTeam::getMapTradingCount() const
{
	return m_iMapTradingCount;
}


bool CvTeam::isMapTrading()	const													
{
	return (getMapTradingCount() > 0);
}


void CvTeam::changeMapTradingCount(int iChange)						 
{
	m_iMapTradingCount = (m_iMapTradingCount + iChange);
	FAssert(getMapTradingCount() >= 0);
}


int CvTeam::getTechTradingCount() const
{
	return m_iTechTradingCount;
}


bool CvTeam::isTechTrading() const													 
{
	return (getTechTradingCount() > 0);
}


void CvTeam::changeTechTradingCount(int iChange)						 
{
	m_iTechTradingCount = (m_iTechTradingCount + iChange);
	FAssert(getTechTradingCount() >= 0);
}


int CvTeam::getGoldTradingCount() const
{
	return m_iGoldTradingCount;
}


bool CvTeam::isGoldTrading() const													 
{
	return (getGoldTradingCount() > 0);
}


void CvTeam::changeGoldTradingCount(int iChange)						 
{
	m_iGoldTradingCount = (m_iGoldTradingCount + iChange);
	FAssert(getGoldTradingCount() >= 0);
}


int CvTeam::getOpenBordersTradingCount() const
{
	return m_iOpenBordersTradingCount;
}


bool CvTeam::isOpenBordersTrading() const
{
	return (getOpenBordersTradingCount() > 0);
}


void CvTeam::changeOpenBordersTradingCount(int iChange)
{
	m_iOpenBordersTradingCount = (m_iOpenBordersTradingCount + iChange);
	FAssert(getOpenBordersTradingCount() >= 0);
}

int CvTeam::getDefensivePactTradingCount() const
{
	return m_iDefensivePactTradingCount;
}


bool CvTeam::isDefensivePactTrading() const
{
	return (getDefensivePactTradingCount() > 0);
}


void CvTeam::changeDefensivePactTradingCount(int iChange)
{
	m_iDefensivePactTradingCount = (m_iDefensivePactTradingCount + iChange);
	FAssert(getDefensivePactTradingCount() >= 0);
}


int CvTeam::getPermanentAllianceTradingCount() const
{
	return m_iPermanentAllianceTradingCount;
}


bool CvTeam::isPermanentAllianceTrading() const
{
	if (!(GC.getGameINLINE().isOption(GAMEOPTION_PERMANENT_ALLIANCES)))
	{
		return false;
	}

	return (getPermanentAllianceTradingCount() > 0);
}


void CvTeam::changePermanentAllianceTradingCount(int iChange)
{
	m_iPermanentAllianceTradingCount = (m_iPermanentAllianceTradingCount + iChange);
	FAssert(getPermanentAllianceTradingCount() >= 0);
}


int CvTeam::getVassalTradingCount() const
{
	return m_iVassalTradingCount;
}


bool CvTeam::isVassalStateTrading() const
{
	if (GC.getGameINLINE().isOption(GAMEOPTION_NO_VASSAL_STATES))
	{
		return false;
	}

	return (getVassalTradingCount() > 0);
}


void CvTeam::changeVassalTradingCount(int iChange)
{
	m_iVassalTradingCount += iChange;
	FAssert(getVassalTradingCount() >= 0);
}


int CvTeam::getBridgeBuildingCount() const
{
	return m_iBridgeBuildingCount;
}


bool CvTeam::isBridgeBuilding()	const											
{
	return (getBridgeBuildingCount() > 0);
}


void CvTeam::changeBridgeBuildingCount(int iChange)				 
{
	if (iChange != 0)
	{
		m_iBridgeBuildingCount = (m_iBridgeBuildingCount + iChange);
		FAssert(getBridgeBuildingCount() >= 0);

		if (GC.IsGraphicsInitialized())
		{
			gDLL->getEngineIFace()->MarkBridgesDirty();
		}
	}
}


int CvTeam::getIrrigationCount() const
{
	return m_iIrrigationCount;
}


bool CvTeam::isIrrigation() const
{
	return (getIrrigationCount() > 0);
}


void CvTeam::changeIrrigationCount(int iChange)
{
	if (iChange != 0)
	{
		m_iIrrigationCount = (m_iIrrigationCount + iChange);
		FAssert(getIrrigationCount() >= 0);

		GC.getMapINLINE().updateIrrigated();
	}
}


int CvTeam::getIgnoreIrrigationCount() const
{
	return m_iIgnoreIrrigationCount;
}


bool CvTeam::isIgnoreIrrigation() const
{
	return (getIgnoreIrrigationCount() > 0);
}


void CvTeam::changeIgnoreIrrigationCount(int iChange)
{
	m_iIgnoreIrrigationCount = (m_iIgnoreIrrigationCount + iChange);
	FAssert(getIgnoreIrrigationCount() >= 0);
}


int CvTeam::getWaterWorkCount() const
{
	return m_iWaterWorkCount;
}


bool CvTeam::isWaterWork() const
{
	return (getWaterWorkCount() > 0);
}


void CvTeam::changeWaterWorkCount(int iChange)
{
	if (iChange != 0)
	{
		m_iWaterWorkCount = (m_iWaterWorkCount + iChange);
		FAssert(getWaterWorkCount() >= 0);

		AI_makeAssignWorkDirty();
	}
}

int CvTeam::getVassalPower() const
{
	return m_iVassalPower;
}

void CvTeam::setVassalPower(int iPower)
{
	m_iVassalPower = iPower;
}

int CvTeam::getMasterPower() const
{
	return m_iMasterPower;
}

void CvTeam::setMasterPower(int iPower)
{
	m_iMasterPower = iPower;
}

int CvTeam::getEnemyWarWearinessModifier() const
{
	int iEnemyWarWearinessTotal = m_iEnemyWarWearinessModifier;
	for (int iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				iEnemyWarWearinessTotal += GET_PLAYER((PlayerTypes)iI).getNationalEnemyWarWearinessModifier();
			}
		}
	}
	//return m_iEnemyWarWearinessModifier;
	return iEnemyWarWearinessTotal;
}

void CvTeam::changeEnemyWarWearinessModifier(int iChange)
{
	int iOldValue;

	//	Do this in a thread-safe manner
	do
	{
		iOldValue = m_iEnemyWarWearinessModifier;
	} while( InterlockedCompareExchange((volatile LONG*)&m_iEnemyWarWearinessModifier, iOldValue + iChange, iOldValue) != iOldValue );
	//m_iEnemyWarWearinessModifier += iChange;
}

bool CvTeam::isMapCentering() const
{
	return m_bMapCentering;
}


void CvTeam::setMapCentering(bool bNewValue)
{
	if (isMapCentering() != bNewValue)
	{
		m_bMapCentering = bNewValue;

		if (getID() == GC.getGameINLINE().getActiveTeam())
		{ 
			gDLL->getInterfaceIFace()->setDirty(MinimapSection_DIRTY_BIT, true);
		}
	}
}


TeamTypes CvTeam::getID() const
{
	return m_eID;
}


int CvTeam::getStolenVisibilityTimer(TeamTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "iIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "iIndex is expected to be within maximum bounds (invalid Index)");

	return m_aiStolenVisibilityTimer[eIndex];
}


bool CvTeam::isStolenVisibility(TeamTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "iIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "iIndex is expected to be within maximum bounds (invalid Index)");

	return (getStolenVisibilityTimer(eIndex) > 0);
}


void CvTeam::setStolenVisibilityTimer(TeamTypes eIndex, int iNewValue)
{
	CvPlot* pLoopPlot;
	bool bOldStolenVisibility;
	int iI;

	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (getStolenVisibilityTimer(eIndex) != iNewValue)
	{
		bOldStolenVisibility = isStolenVisibility(eIndex);

		m_aiStolenVisibilityTimer[eIndex] = iNewValue;
		FAssert(getStolenVisibilityTimer(eIndex) >= 0);

		if (bOldStolenVisibility != isStolenVisibility(eIndex))
		{
			for (iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
			{
				pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);

				if (pLoopPlot->isVisible(eIndex, false))
				{
					pLoopPlot->changeStolenVisibilityCount(getID(), ((isStolenVisibility(eIndex)) ? 1 : -1));
				}
			}
		}
	}
}


void CvTeam::changeStolenVisibilityTimer(TeamTypes eIndex, int iChange)
{
	setStolenVisibilityTimer(eIndex, (getStolenVisibilityTimer(eIndex) + iChange));
}


int CvTeam::getWarWeariness(TeamTypes eIndex) const								 
{
	return getWarWearinessTimes100(eIndex)/100;
}

int CvTeam::getWarWearinessTimes100(TeamTypes eIndex) const								 
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_aiWarWearinessTimes100[eIndex];
}


void CvTeam::setWarWeariness(TeamTypes eIndex, int iNewValue)
{
	setWarWearinessTimes100(eIndex, 100*iNewValue);
}

void CvTeam::setWarWearinessTimes100(TeamTypes eIndex, int iNewValue)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");
	m_aiWarWearinessTimes100[eIndex] = std::max(0, iNewValue);
}


void CvTeam::changeWarWeariness(TeamTypes eIndex, int iChange)
{
	changeWarWearinessTimes100(eIndex, 100*iChange);
}

void CvTeam::changeWarWearinessTimes100(TeamTypes eIndex, int iChange)
{
	FAssert(eIndex >= 0 && eIndex < MAX_TEAMS);
	setWarWearinessTimes100(eIndex, getWarWearinessTimes100(eIndex) + iChange);
}

void CvTeam::changeWarWeariness(TeamTypes eOtherTeam, const CvPlot& kPlot, int iFactor)
{
	changeWarWearinessTimes100(eOtherTeam, kPlot, iFactor*100);
}

void CvTeam::changeWarWearinessTimes100(TeamTypes eOtherTeam, const CvPlot& kPlot, int iFactor)
{
	PROFILE_FUNC();

	int iOurCulture = kPlot.countFriendlyCulture(getID());
	int iTheirCulture = kPlot.countFriendlyCulture(eOtherTeam);

	int iRatio = 100;
	if (0 != iOurCulture + iTheirCulture)
	{
		iRatio = (100 * iTheirCulture) / (iOurCulture + iTheirCulture);
	}
/************************************************************************************************/
/* REVOLUTION_MOD                         05/30/08                                jdog5000      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	// Rebels don't feel as if they are fighting on foreign soil
	if( isRebelAgainst(eOtherTeam) )
	{
		iRatio = std::min(40, iRatio);
	}
	else if( isRebel() )
	{
		iRatio = std::min(60,iRatio);
	}
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/

	changeWarWearinessTimes100(eOtherTeam, iRatio * iFactor);
}


int CvTeam::getTechShareCount(int iIndex) const
{
	FAssertMsg(iIndex >= 0, "iIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(iIndex < MAX_TEAMS, "iIndex is expected to be within maximum bounds (invalid Index)");
	return m_aiTechShareCount[iIndex];
}


bool CvTeam::isTechShare(int iIndex) const
{
	return (getTechShareCount(iIndex) > 0);
}


void CvTeam::changeTechShareCount(int iIndex, int iChange)
{
	FAssertMsg(iIndex >= 0, "iIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(iIndex < MAX_TEAMS, "iIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_aiTechShareCount[iIndex] = (m_aiTechShareCount[iIndex] + iChange);
		FAssert(getTechShareCount(iIndex) >= 0);

		if (isTechShare(iIndex))
		{
			updateTechShare();
		}
	}
}


int CvTeam::getCommerceFlexibleCount(CommerceTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_COMMERCE_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_aiCommerceFlexibleCount[eIndex];
}


bool CvTeam::isCommerceFlexible(CommerceTypes eIndex) const
{
	return (getCommerceFlexibleCount(eIndex) > 0);
}


void CvTeam::changeCommerceFlexibleCount(CommerceTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_COMMERCE_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_aiCommerceFlexibleCount[eIndex] = (m_aiCommerceFlexibleCount[eIndex] + iChange);
		FAssert(getCommerceFlexibleCount(eIndex) >= 0);

		if (getID() == GC.getGameINLINE().getActiveTeam())
		{
			gDLL->getInterfaceIFace()->setDirty(PercentButtons_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}


int CvTeam::getExtraMoves(DomainTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_DOMAIN_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_aiExtraMoves[eIndex];
}


void CvTeam::changeExtraMoves(DomainTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_DOMAIN_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");
	m_aiExtraMoves[eIndex] = (m_aiExtraMoves[eIndex] + iChange);
	FAssert(getExtraMoves(eIndex) >= 0);
}


bool CvTeam::isHasMet(TeamTypes eIndex)	const														 
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");
	//FAssert((eIndex != getID()) || m_abHasMet[eIndex]);
	return m_abHasMet[eIndex];
}

// 45deg - embassies in Rhye's and fall style - start
bool CvTeam::isHasEverMet(TeamTypes eIndex)	const														 
{
	return m_abHasEverMet[eIndex];
}
// 45deg - embassies in Rhye's and fall style - end

/************************************************************************************************/
/* REVOLUTION_MOD                         02/01/08                                jdog5000      */
/*                                                                                              */
/* For barbarian and minor civs                                                                 */
/************************************************************************************************/
void CvTeam::setHasMet( TeamTypes eIndex, bool bNewValue )
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");
	if( bNewValue != isHasMet(eIndex) )
		m_abHasMet[eIndex] = bNewValue;
	return;
}
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/

void CvTeam::makeHasMet(TeamTypes eIndex, bool bNewDiplo)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (!isHasMet(eIndex))
	{
		makeHasSeen(eIndex); // K-mod
		m_abHasMet[eIndex] = true;

		if (GC.getGameINLINE().isModderGameOption(MODDERGAMEOPTION_CONTACT_DECAY))
		{
			if (m_abHasEverMet[eIndex])
			{
				bNewDiplo = false;
			}
			else
			{
				m_abHasEverMet[eIndex] = true; //Rhye
			}
		}
		updateTechShare();

		if (GET_TEAM(eIndex).isHuman())
		{
			for (int iI = 0; iI < MAX_PLAYERS; iI++)
			{
				if (GET_PLAYER((PlayerTypes)iI).isAlive())
				{
					if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
					{
						if (!(GET_PLAYER((PlayerTypes)iI).isHuman()))
						{
							GET_PLAYER((PlayerTypes)iI).clearResearchQueue();
							GET_PLAYER((PlayerTypes)iI).AI_makeProductionDirty();
						}
					}
				}
			}
		}

		for (int iI = 0; iI < MAX_CIV_TEAMS; iI++)
		{
			if (GET_TEAM((TeamTypes)iI).isAlive())
			{
				if (GET_TEAM((TeamTypes)iI).isVassal(getID()) || isVassal((TeamTypes)iI))
				{
					GET_TEAM((TeamTypes)iI).meet(eIndex, bNewDiplo);
				}
			}
		}

		if ((getID() == GC.getGameINLINE().getActiveTeam()) || (eIndex == GC.getGameINLINE().getActiveTeam()))
		{
			gDLL->getInterfaceIFace()->setDirty(Score_DIRTY_BIT, true);
			// RevolutionDCM start - redraw espionage percent buttons
			gDLL->getInterfaceIFace()->setDirty(PercentButtons_DIRTY_BIT, true);
			// RevolutionDCM end
		}

		if (GC.getGameINLINE().isOption(GAMEOPTION_ALWAYS_WAR))
		{
			if (isHuman() && getID() != eIndex)
			{
				declareWar(eIndex, false, NO_WARPLAN);
			}

/********************************************************************************/
/**		REVOLUTION_MOD							7/11/08				jdog5000	*/
/**																				*/
/**		For BarbarianCiv and Start As Minors									*/
/********************************************************************************/
			// report event to Python, along with some other key state
			CvEventReporter::getInstance().firstContact(getID(), eIndex);
/********************************************************************************/
/**		REVOLUTION_MOD							END								*/
/********************************************************************************/
		}
		else
		{
/********************************************************************************/
/**		REVOLUTION_MOD							7/11/08				jdog5000	*/
/**																				*/
/**		For BarbarianCiv and Start As Minors									*/
/********************************************************************************/
			// Move reporting to Python before diplo popup to all war declarations on first contact
			// report event to Python, along with some other key state
			CvEventReporter::getInstance().firstContact(getID(), eIndex);
/********************************************************************************/
/**		REVOLUTION_MOD							END								*/
/********************************************************************************/

			if (GC.getGameINLINE().isFinalInitialized() && !gDLL->GetWorldBuilderMode()
			&& !isHuman() && !isAtWar(eIndex))
			{
				for (int iI = 0; iI < MAX_PLAYERS; iI++)
				{
					const CvPlayer& player = GET_PLAYER((PlayerTypes)iI);
					if (player.getTeam() == eIndex && player.isAlive() && player.isHuman()
					&& GET_PLAYER(getLeaderID()).canContact((PlayerTypes)iI))
					{
						CvDiploParameters* pDiplo = new CvDiploParameters(getLeaderID());
						FAssertMsg(pDiplo != NULL, "pDiplo must be valid");
						pDiplo->setDiploComment((DiploCommentTypes)GC.getInfoTypeForString("AI_DIPLOCOMMENT_FIRST_CONTACT"));
						pDiplo->setAIContact(true);
						gDLL->beginDiplomacy(pDiplo, ((PlayerTypes)iI));
					}
				}
			}
		}
	}
}

//Rhye - start
void CvTeam::cutContact(TeamTypes eIndex)
{
	CvWString szBuffer;
	if (isHasMet(eIndex))
	{
		m_abHasMet[eIndex] = false;
		GET_TEAM(eIndex).m_abHasMet[getID()] = false;
		szBuffer = gDLL->getText("TXT_KEY_MISC_CONTACT_DECAY", GET_PLAYER(getLeaderID()).getCivilizationAdjective());
		AddDLLMessage(GET_TEAM(eIndex).getLeaderID(), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_DEAL_CANCELLED", MESSAGE_TYPE_MINOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_RED"));
	}
}
//Rhye - end

bool CvTeam::isAtWar(TeamTypes eIndex) const
{
	if ( eIndex == NO_TEAM )
	{
		return false;
	}
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_abAtWar[eIndex];
}


void CvTeam::setAtWar(TeamTypes eIndex, bool bNewValue)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");

	if ( m_abAtWar[eIndex] != bNewValue )
	{
		//	Koshling - have seen game states with team flagged as at war with
		//	themslves.  No idea hiow this arises but we can at least prevent it
		//	here as a fallback
		if ( !bNewValue || eIndex != getID() )
		{
			m_abAtWar[eIndex] = bNewValue;

			for (int iI = 0; iI < MAX_PLAYERS; iI++)
			{
				if (GET_PLAYER((PlayerTypes)iI).isAlive() &&
					GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
				{
					GET_PLAYER((PlayerTypes)iI).AI_noteWarStatusChange(eIndex, bNewValue);
				}
			}
		}
		else
		{
			FAssertMsg(false, "Team trying to mark itself as at war with itself");
		}
	}
}


bool CvTeam::isPermanentWarPeace(TeamTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_abPermanentWarPeace[eIndex];
}


void CvTeam::setPermanentWarPeace(TeamTypes eIndex, bool bNewValue)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");
	m_abPermanentWarPeace[eIndex] = bNewValue;
}

bool CvTeam::isFreeTrade(TeamTypes eIndex) const
{
	if (isAtWar(eIndex))
	{
		return false;
	}

	if (!isHasMet(eIndex))
	{
		return false;
	}
	
	if (isOpenBorders(eIndex))
	{ 
		return true;
	}
	//If the secretary general voted for open markets
	if (GC.getGameINLINE().isFreeTrade())
	{
		return true;
	}
/************************************************************************************************/
/* Afforess	                  Start		 06/27/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	if (isLimitedBorders(eIndex))
	{
		return true;
	}
	
	if (isFreeTradeAgreement(eIndex))
	{
		return true;
	}
	for (int iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive() && GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
		{
			if (GET_PLAYER((PlayerTypes)iI).getForceAllTradeRoutes() > 0)
			{
				return true;
			}
		}
	}
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/

return false;
}


bool CvTeam::isOpenBorders(TeamTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_abOpenBorders[eIndex];
}


void CvTeam::setOpenBorders(TeamTypes eIndex, bool bNewValue)
{
	bool bOldFreeTrade;
	int iI;

	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (isOpenBorders(eIndex) != bNewValue)
	{
		bOldFreeTrade = isFreeTrade(eIndex);

		m_abOpenBorders[eIndex] = bNewValue;

		AI_setOpenBordersCounter(eIndex, 0);

		GC.getMapINLINE().verifyUnitValidPlot();

		if ((getID() == GC.getGameINLINE().getActiveTeam()) || (eIndex == GC.getGameINLINE().getActiveTeam()))
		{
			gDLL->getInterfaceIFace()->setDirty(Score_DIRTY_BIT, true);
		}

		if (bOldFreeTrade != isFreeTrade(eIndex))
		{
			for (iI = 0; iI < MAX_PLAYERS; iI++)
			{
				if (GET_PLAYER((PlayerTypes)iI).isAlive())
				{
					if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
					{
						GET_PLAYER((PlayerTypes)iI).updateTradeRoutes();
					}
				}
			}
		}
	}
}

bool CvTeam::isDefensivePact(TeamTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_abDefensivePact[eIndex];
}


void CvTeam::setDefensivePact(TeamTypes eIndex, bool bNewValue)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (isDefensivePact(eIndex) != bNewValue)
	{
		m_abDefensivePact[eIndex] = bNewValue;

		if ((getID() == GC.getGameINLINE().getActiveTeam()) || (eIndex == GC.getGameINLINE().getActiveTeam()))
		{
			gDLL->getInterfaceIFace()->setDirty(Score_DIRTY_BIT, true);
		}

		if (bNewValue && !GET_TEAM(eIndex).isDefensivePact(getID()))
		{
			CvWString szBuffer = gDLL->getText("TXT_KEY_MISC_PLAYERS_SIGN_DEFENSIVE_PACT", getName().GetCString(), GET_TEAM(eIndex).getName().GetCString());

			GC.getGameINLINE().addReplayMessage(REPLAY_MESSAGE_MAJOR_EVENT, getLeaderID(), szBuffer, -1, -1, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));


			for (int iI = 0; iI < MAX_PLAYERS; iI++)
			{
				CvPlayer& kPlayer = GET_PLAYER((PlayerTypes)iI);
				if (kPlayer.isAlive())
				{
					if (isHasMet(kPlayer.getTeam()) && GET_TEAM(eIndex).isHasMet(kPlayer.getTeam()))
					{
						MEMORY_TRACK_EXEMPT();

						AddDLLMessage(((PlayerTypes)iI), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_WELOVEKING", MESSAGE_TYPE_MAJOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));
					}
				}
			}
		}

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      09/03/09                       poyuzhe & jdog5000     */
/*                                                                                              */
/* Efficiency                                                                                   */
/************************************************************************************************/
		// From Sanguo Mod Performance, ie the CAR Mod
		// Attitude cache
		for (int iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if( GET_PLAYER((PlayerTypes)iI).isAlive() )
			{
				for (int iJ = 0; iJ < MAX_PLAYERS; iJ++)
				{
					if( GET_PLAYER((PlayerTypes)iJ).isAlive() && GET_PLAYER((PlayerTypes)iJ).getTeam() != GET_PLAYER((PlayerTypes)iI).getTeam() )
					{
						if( GET_PLAYER((PlayerTypes)iJ).getTeam() == getID() || GET_PLAYER((PlayerTypes)iJ).getTeam() == eIndex )
						{
							GET_PLAYER((PlayerTypes)iJ).AI_invalidateAttitudeCache((PlayerTypes)iI);
							GET_PLAYER((PlayerTypes)iI).AI_invalidateAttitudeCache((PlayerTypes)iJ);
						}
					}
				}
			}
		}
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

	}
}


bool CvTeam::isForcePeace(TeamTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_abForcePeace[eIndex];
}


void CvTeam::setForcePeace(TeamTypes eIndex, bool bNewValue)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");
	m_abForcePeace[eIndex] = bNewValue;

	if (isForcePeace(eIndex))
	{
		if (AI_isSneakAttackPreparing(eIndex))
		{
			AI_setWarPlan(eIndex, NO_WARPLAN);
		}

		for (int iTeam = 0; iTeam < MAX_TEAMS; ++iTeam)
		{
			if (GET_TEAM((TeamTypes)iTeam).isVassal(eIndex))
			{
				if (AI_isSneakAttackPreparing((TeamTypes)iTeam))
				{
					AI_setWarPlan((TeamTypes)iTeam, NO_WARPLAN);
				}
			}
		}
	}
}

bool CvTeam::isVassal(TeamTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_abVassal[eIndex];
}

void CvTeam::setVassal(TeamTypes eIndex, bool bNewValue, bool bCapitulated)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(!bNewValue || !GET_TEAM(eIndex).isAVassal(), "can't become a vassal of a vassal");

	if (isVassal(eIndex) != bNewValue)
	{
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			if (GET_PLAYER((PlayerTypes)i).getTeam() == getID() && GET_PLAYER((PlayerTypes)i).isAlive())
			{
				GET_PLAYER((PlayerTypes)i).updateCitySight(false, false);
			}
		}

		for (int iPlayer = 0; iPlayer < MAX_PLAYERS; ++iPlayer)
		{
			CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iPlayer);

			if (kLoopPlayer.isAlive() && kLoopPlayer.getTeam() == eIndex)
			{
				int iLoop = 0;
				for (CvUnit* pLoopUnit = kLoopPlayer.firstUnit(&iLoop); NULL != pLoopUnit; pLoopUnit = kLoopPlayer.nextUnit(&iLoop))
				{
					CvPlot* pPlot = pLoopUnit->plot();
					if (pLoopUnit->getTeam() != pPlot->getTeam() && (pPlot->getTeam() == NO_TEAM || !GET_TEAM(pPlot->getTeam()).isVassal(pLoopUnit->getTeam())))
					{
						kLoopPlayer.changeNumOutsideUnits(-1);
					}
				}
			}
		}

		m_abVassal[eIndex] = bNewValue;

		for (int iPlayer = 0; iPlayer < MAX_PLAYERS; ++iPlayer)
		{
			CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iPlayer);

			if (kLoopPlayer.isAlive() && kLoopPlayer.getTeam() == eIndex)
			{
				int iLoop = 0;
				for (CvUnit* pLoopUnit = kLoopPlayer.firstUnit(&iLoop); NULL != pLoopUnit; pLoopUnit = kLoopPlayer.nextUnit(&iLoop))
				{
					CvPlot* pPlot = pLoopUnit->plot();
					if (pLoopUnit->getTeam() != pPlot->getTeam() && (pPlot->getTeam() == NO_TEAM || !GET_TEAM(pPlot->getTeam()).isVassal(pLoopUnit->getTeam())))
					{
						kLoopPlayer.changeNumOutsideUnits(1);
					}
				}
			}
		}

		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			if (GET_PLAYER((PlayerTypes)i).getTeam() == getID() && GET_PLAYER((PlayerTypes)i).isAlive())
			{
				GET_PLAYER((PlayerTypes)i).updateCitySight(true, false);
			}
		}

		for (int i = 0; i < GC.getMapINLINE().numPlotsINLINE(); ++i)
		{
			CvPlot* pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(i);
			if (pLoopPlot && (pLoopPlot->getTeam() == getID() || pLoopPlot->getTeam() == eIndex))
			{
				pLoopPlot->updateCulture(true, false);
			}
		}

		GC.getGameINLINE().updatePlotGroups();

		if (isVassal(eIndex))
		{
			m_bCapitulated = bCapitulated;

			CvDeal* pLoopDeal;
			int iLoop = 0;
			CLLNode<TradeData>* pNode;
			bool bValid;

			for (pLoopDeal = GC.getGameINLINE().firstDeal(&iLoop); pLoopDeal != NULL; pLoopDeal = GC.getGameINLINE().nextDeal(&iLoop))
			{
				if ((GET_PLAYER(pLoopDeal->getFirstPlayer()).getTeam() == getID()) || (GET_PLAYER(pLoopDeal->getSecondPlayer()).getTeam() == getID()))
				{
					bValid = true;

					if (pLoopDeal->getFirstTrades() != NULL)
					{
						for (pNode = pLoopDeal->getFirstTrades()->head(); pNode; pNode = pLoopDeal->getFirstTrades()->next(pNode))
						{
							if ((pNode->m_data.m_eItemType == TRADE_DEFENSIVE_PACT) ||
								(pNode->m_data.m_eItemType == TRADE_PEACE_TREATY))
							{
								bValid = false;
								break;
							}
						}
					}

					if (bValid && pLoopDeal->getSecondTrades() != NULL)
					{
						for (pNode = pLoopDeal->getSecondTrades()->head(); pNode; pNode = pLoopDeal->getSecondTrades()->next(pNode))
						{
							if ((pNode->m_data.m_eItemType == TRADE_DEFENSIVE_PACT) ||
								(pNode->m_data.m_eItemType == TRADE_PEACE_TREATY))
							{
								bValid = false;
								break;
							}
						}
					}

					if (!bValid)
					{
						pLoopDeal->kill();
					}
				}
			}

			setForcePeace(eIndex, false);
			GET_TEAM(eIndex).setForcePeace(getID(), false);

			// Declare war on teams you should be at war with
			for (int iI = 0; iI < MAX_TEAMS; iI++)
			{
				if ((iI != getID()) && (iI != eIndex))
				{
					if (GET_TEAM((TeamTypes)iI).isAlive())
					{
						if (GET_TEAM(eIndex).isHasMet((TeamTypes)iI))
						{
							meet((TeamTypes)iI, true);
						}

						if (isHasMet((TeamTypes)iI))
						{
							GET_TEAM(eIndex).meet((TeamTypes)iI, true);
						}

						if (GET_TEAM(eIndex).isAtWar((TeamTypes)iI))
						{
							declareWar(((TeamTypes)iI), false, WARPLAN_DOGPILE);
						}
						else if (isAtWar((TeamTypes)iI))
						{
							if (bCapitulated)
							{
								makePeace((TeamTypes)iI);
							}
							else
							{
								GET_TEAM(eIndex).declareWar((TeamTypes)iI, false, WARPLAN_DOGPILE);
							}
						}
					}
				}
			}

			for (int iI = 0; iI < MAX_TEAMS; iI++)
			{
				CvTeam& kLoopTeam = GET_TEAM((TeamTypes)iI);
				if (kLoopTeam.isAlive())
				{
					if (!kLoopTeam.isAtWar(getID()))
					{
						kLoopTeam.AI_setWarPlan(getID(), NO_WARPLAN);
						AI_setWarPlan((TeamTypes)iI, NO_WARPLAN);
					}

					if (!kLoopTeam.isAtWar(eIndex))
					{
						kLoopTeam.AI_setWarPlan(eIndex, NO_WARPLAN);
					}

				}
			}

			// All our vassals now become free
			for (int iI = 0; iI < MAX_TEAMS; iI++)
			{
				if ((iI != getID()) && (iI != eIndex))
				{
					CvTeam& kLoopTeam = GET_TEAM((TeamTypes)iI);
					if (kLoopTeam.isAlive() && kLoopTeam.isVassal(getID()))
					{
						freeVassal((TeamTypes)iI);
					}
				}
			}

			setMasterPower(GET_TEAM(eIndex).getTotalLand());
			setVassalPower(getTotalLand(false));

			if (GC.getGameINLINE().isFinalInitialized() && !(gDLL->GetWorldBuilderMode()))
			{
				CvWString szReplayMessage;
				
				if (bCapitulated)
				{
					szReplayMessage = gDLL->getText("TXT_KEY_MISC_CAPITULATE_AGREEMENT", getName().GetCString(), GET_TEAM(eIndex).getName().GetCString());
				}
				else
				{
					szReplayMessage = gDLL->getText("TXT_KEY_MISC_VASSAL_AGREEMENT", getName().GetCString(), GET_TEAM(eIndex).getName().GetCString());
				}
				GC.getGameINLINE().addReplayMessage(REPLAY_MESSAGE_MAJOR_EVENT, getLeaderID(), szReplayMessage, -1, -1, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));

				for (int iI = 0; iI < MAX_PLAYERS; iI++)
				{
					CvPlayer& kPlayer = GET_PLAYER((PlayerTypes)iI);
					if (kPlayer.isAlive())
					{
						if (isHasMet(kPlayer.getTeam()) && GET_TEAM(eIndex).isHasMet(kPlayer.getTeam()))
						{
							MEMORY_TRACK_EXEMPT();

							AddDLLMessage(((PlayerTypes)iI), false, GC.getEVENT_MESSAGE_TIME(), szReplayMessage, "AS2D_WELOVEKING", MESSAGE_TYPE_MAJOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));
						}
					}
				}
			}
		}
		else
		{
			setMasterPower(0);
			setVassalPower(0);

			if (GC.getGameINLINE().isFinalInitialized() && !(gDLL->GetWorldBuilderMode()) && isAlive() && GET_TEAM(eIndex).isAlive())
			{
				CvWString szReplayMessage;
				
				if (m_bCapitulated)
				{
					szReplayMessage = gDLL->getText("TXT_KEY_MISC_SURRENDER_REVOLT", getName().GetCString(), GET_TEAM(eIndex).getName().GetCString());
				}
				else
				{
					szReplayMessage = gDLL->getText("TXT_KEY_MISC_VASSAL_REVOLT", getName().GetCString(), GET_TEAM(eIndex).getName().GetCString());
				}

				GC.getGameINLINE().addReplayMessage(REPLAY_MESSAGE_MAJOR_EVENT, getLeaderID(), szReplayMessage, -1, -1, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));

				for (int iI = 0; iI < MAX_PLAYERS; iI++)
				{
					CvPlayer& kPlayer = GET_PLAYER((PlayerTypes)iI);
					if (kPlayer.isAlive())
					{
						CvWString szBuffer;
						if (getID() == kPlayer.getTeam() || eIndex == kPlayer.getTeam() || isHasMet(kPlayer.getTeam()) && GET_TEAM(eIndex).isHasMet(kPlayer.getTeam()))
						{
							szBuffer = szReplayMessage;
						}

						if (!szBuffer.empty())
						{
							MEMORY_TRACK_EXEMPT();

							AddDLLMessage(((PlayerTypes)iI), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_REVOLTSTART", MESSAGE_TYPE_MAJOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));
						}
					}
				}				
			}

			m_bCapitulated = false;
		}

		for (int iPlayer = 0; iPlayer < MAX_PLAYERS; iPlayer++)
		{
			CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iPlayer);
			if (kLoopPlayer.getTeam() == eIndex && kLoopPlayer.isAlive())
			{
				kLoopPlayer.setMaintenanceDirty(true);
			}
		}

		if (GC.getGameINLINE().isFinalInitialized() && !(gDLL->GetWorldBuilderMode()))
		{
			CvEventReporter::getInstance().vassalState(eIndex, getID(), bNewValue);
		}

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      09/03/09                       poyuzhe & jdog5000     */
/*                                                                                              */
/* Efficiency                                                                                   */
/************************************************************************************************/
		// From Sanguo Mod Performance, ie the CAR Mod
		// Attitude cache
		for (int iI = 0; iI < MAX_CIV_PLAYERS; iI++)
		{
			if( GET_PLAYER((PlayerTypes)iI).isAlive() )
			{
				if( GET_PLAYER((PlayerTypes)iI).getTeam() == getID() || GET_PLAYER((PlayerTypes)iI).getTeam() == eIndex )
				{
					for (int iJ = 0; iJ < MAX_CIV_PLAYERS; iJ++)
					{
						if( GET_PLAYER((PlayerTypes)iJ).isAlive() && GET_PLAYER((PlayerTypes)iJ).getTeam() != GET_PLAYER((PlayerTypes)iI).getTeam() )
						{
							if( GET_PLAYER((PlayerTypes)iJ).getTeam() == getID() || GET_PLAYER((PlayerTypes)iJ).getTeam() == eIndex )
							{
								GET_PLAYER((PlayerTypes)iJ).AI_invalidateAttitudeCache((PlayerTypes)iI);
								GET_PLAYER((PlayerTypes)iI).AI_invalidateAttitudeCache((PlayerTypes)iJ);
							}
						}
					}
				}
			}
		}
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/
	}
}

// f1rpo:
bool CvTeam::isRival(TeamTypes eOther) const
{
	CvTeam const& kOther = GET_TEAM(eOther);
	if (getID() == eOther || isVassal(eOther) || kOther.isVassal(getID()))
		return false;
	if (!isAlive() || !kOther.isAlive())
		return false;
	/*	(Checking !isAVassal first would be no help - b/c that function
		also goes through all teams.) */
	for (int i = 0; i < MAX_CIV_TEAMS; i++)
	{
		CvTeam const& kLoopTeam = GET_TEAM((TeamTypes)i);
		if (kLoopTeam.isAlive() && isVassal(kLoopTeam.getID()) &&
			kOther.isVassal(kLoopTeam.getID()))
		{
			return false;
		}
	}
	return true;
}

void CvTeam::assignVassal(TeamTypes eVassal, bool bSurrender) const
{
	CLinkList<TradeData> ourList;
	CLinkList<TradeData> theirList;
	TradeData item;

	GET_TEAM(eVassal).setVassal(getID(), true, bSurrender);

	setTradeItem(&item, bSurrender ? TRADE_SURRENDER : TRADE_VASSAL);
	item.m_iData = 1;

	for (int iPlayer = 0; iPlayer < MAX_PLAYERS; iPlayer++)
	{
		CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iPlayer);
		if (kLoopPlayer.getTeam() == eVassal && kLoopPlayer.isAlive())
		{
			ourList.clear();
			theirList.clear();

			theirList.insertAtEnd(item);

			for (int jPlayer = 0; jPlayer < MAX_PLAYERS; jPlayer++)
			{
				if (GET_PLAYER((PlayerTypes)jPlayer).getTeam() == getID() && GET_PLAYER((PlayerTypes)jPlayer).isAlive())
				{
					GC.getGameINLINE().implementDeal(((PlayerTypes)jPlayer), ((PlayerTypes)iPlayer), &ourList, &theirList, true);
				}
			}
		}
	}
}


void CvTeam::freeVassal(TeamTypes eVassal) const
{
	CvDeal* pLoopDeal;
	int iLoop = 0;
	CLLNode<TradeData>* pNode;
	bool bValid;

	for (pLoopDeal = GC.getGameINLINE().firstDeal(&iLoop); pLoopDeal != NULL; pLoopDeal = GC.getGameINLINE().nextDeal(&iLoop))
	{
		bValid = true;

		if ((GET_PLAYER(pLoopDeal->getFirstPlayer()).getTeam() == eVassal) && (GET_PLAYER(pLoopDeal->getSecondPlayer()).getTeam() == getID()))
		{

			if (pLoopDeal->getFirstTrades() != NULL)
			{
				for (pNode = pLoopDeal->getFirstTrades()->head(); pNode; pNode = pLoopDeal->getFirstTrades()->next(pNode))
				{
					if ((pNode->m_data.m_eItemType == TRADE_VASSAL) ||
						(pNode->m_data.m_eItemType == TRADE_SURRENDER))
					{
						bValid = false;
					}
				}
			}
		}

		if (bValid)
		{
			if ((GET_PLAYER(pLoopDeal->getFirstPlayer()).getTeam() == getID()) && (GET_PLAYER(pLoopDeal->getSecondPlayer()).getTeam() == eVassal))
			{
				if (pLoopDeal->getSecondTrades() != NULL)
				{
					for (pNode = pLoopDeal->getSecondTrades()->head(); pNode; pNode = pLoopDeal->getSecondTrades()->next(pNode))
					{
						if ((pNode->m_data.m_eItemType == TRADE_VASSAL) ||
							(pNode->m_data.m_eItemType == TRADE_SURRENDER))
						{
							bValid = false;
						}
					}
				}
			}
		}

		if (!bValid)
		{
			pLoopDeal->kill();
		}
	}	
}

bool CvTeam::isCapitulated() const
{
	FAssert(isAVassal());

	return m_bCapitulated;
}


int CvTeam::getRouteChange(RouteTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumRouteInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiRouteChange[eIndex];
}


void CvTeam::changeRouteChange(RouteTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumRouteInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	m_paiRouteChange[eIndex] = (m_paiRouteChange[eIndex] + iChange);
}


int CvTeam::getProjectCount(ProjectTypes eIndex) const																	 
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumProjectInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiProjectCount[eIndex];
}

int CvTeam::getProjectDefaultArtType(ProjectTypes eIndex) const																	 
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumProjectInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiProjectDefaultArtTypes[eIndex];
}

void CvTeam::setProjectDefaultArtType(ProjectTypes eIndex, int value)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumProjectInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	m_paiProjectDefaultArtTypes[eIndex] = value;
}

int CvTeam::getProjectArtType(ProjectTypes eIndex, int number) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumProjectInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(number >= 0, "number is expected to be non-negative (invalid Index)");
	FAssertMsg(number < getProjectCount(eIndex), "number is expected to be within maximum bounds (invalid Index)");
	return m_pavProjectArtTypes[eIndex][number];
}

void CvTeam::setProjectArtType(ProjectTypes eIndex, int number, int value)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumProjectInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(number >= 0, "number is expected to be non-negative (invalid Index)");
	FAssertMsg(number < getProjectCount(eIndex), "number is expected to be within maximum bounds (invalid Index)");
	m_pavProjectArtTypes[eIndex][number] = value;
}

bool CvTeam::isProjectMaxedOut(ProjectTypes eIndex, int iExtra) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumProjectInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if (!isTeamProject(eIndex))
	{
		return false;
	}

	FAssertMsg(getProjectCount(eIndex) <= GC.getProjectInfo(eIndex).getMaxTeamInstances(), "Current Project count is expected to not exceed the maximum number of instances for this project");

	return ((getProjectCount(eIndex) + iExtra) >= GC.getProjectInfo(eIndex).getMaxTeamInstances());
}

bool CvTeam::isProjectAndArtMaxedOut(ProjectTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumProjectInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if(getProjectCount(eIndex) >= GC.getProjectInfo(eIndex).getMaxTeamInstances())
	{
		int count = getProjectCount(eIndex);
		for(int i=0;i<count;i++)
		{
			if(getProjectArtType(eIndex, i) == -1) //undefined
				return false;
		}

		return true;
	}
	else
	{
		return false;
	}
}

void CvTeam::finalizeProjectArtTypes()
{
	//loop through each project and fill in default art values
	for(int i=0;i<GC.getNumProjectInfos();i++)
	{
		ProjectTypes eProject = (ProjectTypes) i;
		int projectCount = getProjectCount(eProject);
		for(int j=0;j<projectCount;j++)
		{
			int projectArtType = getProjectArtType(eProject, j);
			if(projectArtType == -1) //undefined
			{
				int defaultArtType = getProjectDefaultArtType(eProject);
				setProjectArtType(eProject, j, defaultArtType);
			}
		}
	}
}


void CvTeam::changeProjectCount(ProjectTypes eIndex, int iChange)
{
	CvWString szBuffer;
	int iOldProjectCount;

	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumProjectInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		GC.getGameINLINE().incrementProjectCreatedCount(eIndex, iChange);

		iOldProjectCount = getProjectCount(eIndex);

		int iOldValue;

		//	Do this in a thread-safe manner
		do
		{
			iOldValue = m_paiProjectCount[eIndex];
		} while( InterlockedCompareExchange((volatile LONG*)&m_paiProjectCount[eIndex], iOldValue + iChange, iOldValue) != iOldValue );
		//m_paiProjectCount[eIndex] = (m_paiProjectCount[eIndex] + iChange);
		FAssert(getProjectCount(eIndex) >= 0);
		
		//adjust default art types
		if(iChange >= 0)
		{
			int defaultType = -1;
			for(int i=0;i<iChange;i++)
				m_pavProjectArtTypes[eIndex].push_back(defaultType);
		}
		else
		{
			for(int i=0;i<-iChange;i++)
				m_pavProjectArtTypes[eIndex].pop_back();
		}
		FAssertMsg(getProjectCount(eIndex) == (int)m_pavProjectArtTypes[eIndex].size(), "[Jason] Unbalanced project art types.");

		processProjectChange(eIndex, iChange, iOldProjectCount);

		if ( iChange > 0 )
		{
			if (GC.getGameINLINE().isFinalInitialized() && !(gDLL->GetWorldBuilderMode()))
			{
				CvProjectInfo& kProject = GC.getProjectInfo(eIndex);
				szBuffer = gDLL->getText("TXT_KEY_MISC_COMPLETES_PROJECT", getName().GetCString(), kProject.getTextKeyWide());
				GC.getGameINLINE().addReplayMessage(REPLAY_MESSAGE_MAJOR_EVENT, getLeaderID(), szBuffer, -1, -1, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));

				for (int iI = 0; iI < MAX_PLAYERS; iI++)
				{
					if (GET_PLAYER((PlayerTypes)iI).isAlive())
					{
						MEMORY_TRACK_EXEMPT();

						szBuffer = gDLL->getText("TXT_KEY_MISC_SOMEONE_HAS_COMPLETED", getName().GetCString(), kProject.getTextKeyWide());
						AddDLLMessage(((PlayerTypes)iI), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_PROJECT_COMPLETED", MESSAGE_TYPE_MAJOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));
					}
				}
			}
		}
	}
}

void CvTeam::processProjectChange(ProjectTypes eIndex, int iChange, int iOldProjectCount)
{
	bool bChangeProduction;
	int iI, iJ;

	CvProjectInfo& kProject = GC.getProjectInfo(eIndex);

	changeNukeInterception(kProject.getNukeInterception() * iChange);

	if ((kProject.getTechShare() > 0) && (kProject.getTechShare() <= MAX_TEAMS))
	{
		changeTechShareCount((kProject.getTechShare() - 1), iChange);
	}

	for (int iVictory = 0; iVictory < GC.getNumVictoryInfos(); ++iVictory)
	{
		if (kProject.getVictoryThreshold(iVictory) > 0)
		{
			m_abCanLaunch[iVictory] = GC.getGameINLINE().testVictory((VictoryTypes)iVictory, getID());
		}
	}

	if (iChange > 0)
	{
		if (kProject.getEveryoneSpecialUnit() != NO_SPECIALUNIT)
		{
			GC.getGameINLINE().makeSpecialUnitValid((SpecialUnitTypes)(kProject.getEveryoneSpecialUnit()));
		}

		if (kProject.getEveryoneSpecialBuilding() != NO_SPECIALBUILDING)
		{
			GC.getGameINLINE().makeSpecialBuildingValid((SpecialBuildingTypes)(kProject.getEveryoneSpecialBuilding()));
		}
//TB Nukefix
//		if (kProject.isAllowsNukes())
//		{
//			GC.getGameINLINE().makeNukesValid(true);
//		}	

		for (iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
				{
					if (!(GET_PLAYER((PlayerTypes)iI).isHuman()))
					{
						bChangeProduction = false;

						for (iJ = 0; iJ < GC.getNumProjectInfos(); iJ++)
						{
							if ((getProjectCount(eIndex) >= GC.getProjectInfo((ProjectTypes)iJ).getProjectsNeeded(eIndex)) &&
								  (iOldProjectCount < GC.getProjectInfo((ProjectTypes)iJ).getProjectsNeeded(eIndex)))
							{
								bChangeProduction = true;
								break;
							}
						}

						if (bChangeProduction)
						{
							GET_PLAYER((PlayerTypes)iI).AI_makeProductionDirty();
						}
					}
					/************************************************************************************************/
					/* Afforess	                  Start		 01/02/10                                               */
					/*                                                                                              */
					/*                                                                                              */
					/************************************************************************************************/
					GET_PLAYER((PlayerTypes)iI).changeProjectHappiness(kProject.getGlobalHappiness());
					GET_PLAYER((PlayerTypes)iI).changeProjectHealth(kProject.getGlobalHealth());
					GET_PLAYER((PlayerTypes)iI).changeProjectInflation(kProject.getInflationModifier());
					 //DPII < Maintenance Modifiers >
					GET_PLAYER((PlayerTypes)iI).changeMaintenanceModifier(GC.getProjectInfo(eIndex).getGlobalMaintenanceModifier());
					GET_PLAYER((PlayerTypes)iI).changeDistanceMaintenanceModifier(GC.getProjectInfo(eIndex).getDistanceMaintenanceModifier());
					GET_PLAYER((PlayerTypes)iI).changeNumCitiesMaintenanceModifier(GC.getProjectInfo(eIndex).getNumCitiesMaintenanceModifier());
					GET_PLAYER((PlayerTypes)iI).changeConnectedCityMaintenanceModifier(GC.getProjectInfo(eIndex).getConnectedCityMaintenanceModifier());
					//DPII < Maintenance Modifiers >
					for (iJ = 0; iJ < NUM_COMMERCE_TYPES; iJ++)
					{
						GET_PLAYER((PlayerTypes)iI).changeCommerceRateModifier(((CommerceTypes)iJ), kProject.getCommerceModifier(iJ));
					}
				}
				GET_PLAYER((PlayerTypes)iI).changeWorldHappiness(kProject.getWorldHappiness());
				GET_PLAYER((PlayerTypes)iI).changeWorldHealth(kProject.getWorldHealth());
				GET_PLAYER((PlayerTypes)iI).changeWorldTradeRoutes(kProject.getWorldTradeRoutes());
					/************************************************************************************************/
					/* Afforess	                     END                                                            */
					/************************************************************************************************/
			}
		}
	}
}

int CvTeam::getProjectMaking(ProjectTypes eIndex) const																	 
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumProjectInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiProjectMaking[eIndex];
}


void CvTeam::changeProjectMaking(ProjectTypes eIndex, int iChange)										
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumProjectInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	
	int iOldValue;

	//	Do this in a thread-safe manner
	do
	{
		iOldValue = m_paiProjectMaking[eIndex];
	} while( InterlockedCompareExchange((volatile LONG*)&m_paiProjectMaking[eIndex], iOldValue + iChange, iOldValue) != iOldValue );
	//m_paiProjectMaking[eIndex] = (m_paiProjectMaking[eIndex] + iChange);
	FAssert(getProjectMaking(eIndex) >= 0);
}


int CvTeam::getUnitClassCount(UnitClassTypes eIndex) const											
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumUnitClassInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiUnitClassCount[eIndex];
}


bool CvTeam::isUnitClassMaxedOut(UnitClassTypes eIndex, int iExtra) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumUnitClassInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if (!isTeamUnitClass(eIndex))
	{
		return false;
	}
	
	if (GC.getUnitClassInfo(eIndex).isUnlimitedException())
	{
		return false;
	}
	
	FAssertMsg(getUnitClassCount(eIndex) <= GC.getUnitClassInfo(eIndex).getMaxTeamInstances(), "The current unit class count is expected not to exceed the maximum number of instances allowed for this team");

	return ((getUnitClassCount(eIndex) + iExtra) >= GC.getUnitClassInfo(eIndex).getMaxTeamInstances());
}


void CvTeam::changeUnitClassCount(UnitClassTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumUnitClassInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	m_paiUnitClassCount[eIndex] = (m_paiUnitClassCount[eIndex] + iChange);
	FAssert(getUnitClassCount(eIndex) >= 0);
}


int CvTeam::getBuildingClassCount(BuildingClassTypes eIndex) const											
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBuildingClassInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiBuildingClassCount[eIndex];
}


bool CvTeam::isBuildingClassMaxedOut(BuildingClassTypes eIndex, int iExtra) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBuildingClassInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if (!isTeamWonderClass(eIndex))
	{
		return false;
	}

	FAssertMsg(getBuildingClassCount(eIndex) <= GC.getBuildingClassInfo(eIndex).getMaxTeamInstances(), "The current building class count is expected not to exceed the maximum number of instances allowed for this team");

	return ((getBuildingClassCount(eIndex) + iExtra) >= GC.getBuildingClassInfo(eIndex).getMaxTeamInstances());
}


void CvTeam::changeBuildingClassCount(BuildingClassTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBuildingClassInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	//TB Debug
	int iValue = m_paiBuildingClassCount[eIndex] + iChange;
	if (iValue < 0)
	{
		iValue = 0;
	}
	m_paiBuildingClassCount[eIndex] = iValue;
	FAssert(getBuildingClassCount(eIndex) >= 0);
}


int CvTeam::getObsoleteBuildingCount(BuildingTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBuildingInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiObsoleteBuildingCount[eIndex];
}


bool CvTeam::isObsoleteBuilding(BuildingTypes eIndex) const				
{
	return (getObsoleteBuildingCount(eIndex) > 0);
}


void CvTeam::changeObsoleteBuildingCount(BuildingTypes eIndex, int iChange)
{
	CvCity* pLoopCity;
	bool bOldObsoleteBuilding;
	int iLoop = 0;
	int iI;

	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBuildingInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		bOldObsoleteBuilding = isObsoleteBuilding(eIndex);

		m_paiObsoleteBuildingCount[eIndex] = (m_paiObsoleteBuildingCount[eIndex] + iChange);
		FAssert(getObsoleteBuildingCount(eIndex) >= 0);

		if (bOldObsoleteBuilding != isObsoleteBuilding(eIndex))
		{
			for (iI = 0; iI < MAX_PLAYERS; iI++)
			{
				if (GET_PLAYER((PlayerTypes)iI).isAlive())
				{
					if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
					{
						for (pLoopCity = GET_PLAYER((PlayerTypes)iI).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER((PlayerTypes)iI).nextCity(&iLoop))
						{
							if (pLoopCity->getNumBuilding(eIndex) > 0)
							{
								pLoopCity->processBuilding(eIndex, ((isObsoleteBuilding(eIndex)) ? -pLoopCity->getNumBuilding(eIndex) : pLoopCity->getNumBuilding(eIndex)), true);
							}
						}
					}
				}
			}
		}
	}
}


int CvTeam::getResearchProgress(TechTypes eIndex) const							
{
	if (eIndex != NO_TECH)
	{
		return m_paiResearchProgress[eIndex];
	}
	else
	{
		return 0;
	}
}


void CvTeam::setResearchProgress(TechTypes eIndex, int iNewValue, PlayerTypes ePlayer)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumTechInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(ePlayer >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(ePlayer < MAX_PLAYERS, "ePlayer is expected to be within maximum bounds (invalid Index)");

	if (getResearchProgress(eIndex) != iNewValue)
	{
		m_paiResearchProgress[eIndex] = iNewValue;
		FAssert(getResearchProgress(eIndex) >= 0);

		if (getID() == GC.getGameINLINE().getActiveTeam())
		{
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(Score_DIRTY_BIT, true);
		}

		if (getResearchProgress(eIndex) >= getResearchCost(eIndex))
		{
			int iOverflow = (100 * (getResearchProgress(eIndex) - getResearchCost(eIndex))) / std::max(1, GET_PLAYER(ePlayer).calculateResearchModifier(eIndex));
/************************************************************************************************/
/* Afforess	                  Start		 12/29/09                                               */
/*                                                                                              */
/*      Multiple Research                                                                       */
/************************************************************************************************/
			setHasTech(eIndex, true, ePlayer, true, true);
			iOverflow = GET_PLAYER(ePlayer).doMultipleResearch(iOverflow);
			GET_PLAYER(ePlayer).changeOverflowResearch(iOverflow);
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
			if (!GC.getGameINLINE().isMPOption(MPOPTION_SIMULTANEOUS_TURNS) && !GC.getGameINLINE().isOption(GAMEOPTION_NO_TECH_BROKERING))
			{
				setNoTradeTech(eIndex, true);
			}
		}
	}
}


void CvTeam::changeResearchProgress(TechTypes eIndex, int iChange, PlayerTypes ePlayer)
{
	int iNewResearch = std::max(0, getResearchProgress(eIndex) + iChange);

	setResearchProgress(eIndex, iNewResearch, ePlayer);
}

int CvTeam::changeResearchProgressPercent(TechTypes eIndex, int iPercent, PlayerTypes ePlayer)
{
	int iBeakers = 0;

	if (0 != iPercent && !isHasTech(eIndex))
	{
		if (iPercent > 0)
		{
			iBeakers = std::min(getResearchLeft(eIndex), (getResearchCost(eIndex) * iPercent) / 100);
		}
		else
		{
			iBeakers = std::max(getResearchLeft(eIndex) - getResearchCost(eIndex), (getResearchCost(eIndex) * iPercent) / 100);
		}

		changeResearchProgress(eIndex, iBeakers, ePlayer);
	}

	return iBeakers;
}


int CvTeam::getTechCount(TechTypes eIndex)		 const					
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumTechInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiTechCount[eIndex];
}

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      07/27/09                                jdog5000      */
/*                                                                                              */
/* General AI                                                                                   */
/************************************************************************************************/
int CvTeam::getBestKnownTechScorePercent() const
{
	int iOurTechScore = 0;
	int iBestKnownTechScore = 0;

	for (int iI = 0; iI < MAX_CIV_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if( GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				iOurTechScore = std::max( iOurTechScore, GET_PLAYER((PlayerTypes)iI).getTechScore() );
			}
			else if (isHasMet(GET_PLAYER((PlayerTypes)iI).getTeam()))
			{
				iBestKnownTechScore = std::max( iBestKnownTechScore, GET_PLAYER((PlayerTypes)iI).getTechScore() );
			}
		}
	}

	iBestKnownTechScore = std::max( iBestKnownTechScore, iOurTechScore );

	return ((100*iOurTechScore)/std::max(iBestKnownTechScore, 1));
}
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

int CvTeam::getTerrainTradeCount(TerrainTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumTerrainInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiTerrainTradeCount[eIndex];
}


bool CvTeam::isTerrainTrade(TerrainTypes eIndex) const
{
	return (getTerrainTradeCount(eIndex) > 0);
}


void CvTeam::changeTerrainTradeCount(TerrainTypes eIndex, int iChange)
{
	int iI;

	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumTerrainInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_paiTerrainTradeCount[eIndex] = (m_paiTerrainTradeCount[eIndex] + iChange);
		FAssert(getTerrainTradeCount(eIndex) >= 0);

		for (iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
				{
					GET_PLAYER((PlayerTypes)iI).updatePlotGroups();
				}
			}
		}
	}
}


int CvTeam::getRiverTradeCount() const
{
	return m_iRiverTradeCount;
}


bool CvTeam::isRiverTrade() const
{
	return (getRiverTradeCount() > 0);
}


void CvTeam::changeRiverTradeCount(int iChange)
{
	if (iChange != 0)
	{
		m_iRiverTradeCount += iChange;
		FAssert(getRiverTradeCount() >= 0);

		for (int iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
				{
					GET_PLAYER((PlayerTypes)iI).updatePlotGroups();
				}
			}
		}
	}
}


int CvTeam::getVictoryCountdown(VictoryTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumVictoryInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_aiVictoryCountdown[eIndex];
}

void CvTeam::setVictoryCountdown(VictoryTypes eIndex, int iTurnsLeft)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumVictoryInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	m_aiVictoryCountdown[eIndex] = iTurnsLeft;
}


void CvTeam::changeVictoryCountdown(VictoryTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumVictoryInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_aiVictoryCountdown[eIndex] += iChange;
		FAssert(m_aiVictoryCountdown[eIndex] >= 0);
	}
}

int CvTeam::getVictoryDelay(VictoryTypes eVictory) const
{
	int iExtraDelayPercent = 0;
	for (int iProject = 0; iProject < GC.getNumProjectInfos(); ++iProject)
	{
		CvProjectInfo& kProject = GC.getProjectInfo((ProjectTypes)iProject);
		int iCount = getProjectCount((ProjectTypes)iProject);

		if (iCount < kProject.getVictoryMinThreshold(eVictory))
		{
			FAssert(false);
			return -1;
		}
		
		if (iCount < kProject.getVictoryThreshold(eVictory))
		{
			iExtraDelayPercent += ((kProject.getVictoryThreshold(eVictory)  - iCount) * kProject.getVictoryDelayPercent()) / kProject.getVictoryThreshold(eVictory);
		}
	}

	return (GC.getGameINLINE().victoryDelay(eVictory)  * (100 + iExtraDelayPercent)) / 100;
}

void CvTeam::setCanLaunch(VictoryTypes eVictory, bool bCan)
{
	m_abCanLaunch[eVictory] = bCan;
}

bool CvTeam::canLaunch(VictoryTypes eVictory) const
{
	bool bMasteryVictoryActive = false;

	for (int iI = 0; iI < GC.getNumVictoryInfos(); iI++)
	{
		if (GC.getGameINLINE().isVictoryValid((VictoryTypes)iI))
		{
			if (GC.getVictoryInfo((VictoryTypes)iI).isTotalVictory())
			{
				bMasteryVictoryActive = true;
			}
		}
	}

	if (bMasteryVictoryActive)
	{
		if (getLaunchSuccessRate(eVictory) < 100)
		{
			return false;
		}
	}

	return m_abCanLaunch[eVictory];
}

int CvTeam::getLaunchSuccessRate(VictoryTypes eVictory) const
{
	int iSuccessRate = 100;
	for (int iProject = 0; iProject < GC.getNumProjectInfos(); ++iProject)
	{
		CvProjectInfo& kProject = GC.getProjectInfo((ProjectTypes)iProject);
		int iCount = getProjectCount((ProjectTypes)iProject);

		if (iCount < kProject.getVictoryMinThreshold(eVictory))
		{
			return 0;
		}

		if (iCount < kProject.getVictoryThreshold(eVictory))
		{
			if (kProject.getSuccessRate() > 0)
			{
				iSuccessRate -= (kProject.getSuccessRate() * (kProject.getVictoryThreshold(eVictory) - iCount));
			}
		}
	}

	return iSuccessRate;
}

void CvTeam::resetVictoryProgress()
{
	for (int iI = 0; iI < GC.getNumVictoryInfos(); ++iI)
	{
		if (getVictoryCountdown((VictoryTypes)iI) >= 0 && GC.getGameINLINE().getGameState() == GAMESTATE_ON)
		{
			setVictoryCountdown((VictoryTypes)iI, -1);

			for (int iK = 0; iK < GC.getNumProjectInfos(); iK++)
			{
				if (GC.getProjectInfo((ProjectTypes)iK).getVictoryMinThreshold((VictoryTypes)iI) > 0)
				{
					changeProjectCount((ProjectTypes)iK, -getProjectCount((ProjectTypes)iK));
				}
			}
/************************************************************************************************/
/* Afforess                                     1/1/10                                          */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
#if defined QC_MASTERY_VICTORY
			bool bValid = false;
			for (int iJ = 0; iJ < GC.getNumVictoryInfos(); ++iJ)
			{
				if (GC.getGameINLINE().isVictoryValid((VictoryTypes)iJ))
				{
					if (GC.getVictoryInfo((VictoryTypes)iJ).isTotalVictory())
					{
						bValid = true;
						break;
					}
				}
			}			
			if (!bValid)			
#endif
			{
/************************************************************************************************/
/* Afforess	                         END                                                        */
/************************************************************************************************/	

				CvWString szBuffer = gDLL->getText("TXT_KEY_VICTORY_RESET", getName().GetCString(), GC.getVictoryInfo((VictoryTypes)iI).getTextKeyWide());

				for (int iJ = 0; iJ < MAX_PLAYERS; ++iJ)
				{
					CvPlayer& kPlayer = GET_PLAYER((PlayerTypes)iJ);
					if (kPlayer.isAlive())
					{
						MEMORY_TRACK_EXEMPT();

						AddDLLMessage(((PlayerTypes)iJ), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_MELTDOWN", MESSAGE_TYPE_MAJOR_EVENT);

						if (kPlayer.getTeam() == getID())
						{
							CvPopupInfo* pInfo = new CvPopupInfo();
							pInfo->setText(szBuffer);
							gDLL->getInterfaceIFace()->addPopup(pInfo, (PlayerTypes) iJ);
						}
					}
				}

				GC.getGameINLINE().addReplayMessage(REPLAY_MESSAGE_MAJOR_EVENT, getLeaderID(), szBuffer, -1, -1, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));
			}
		}
	}
}

bool CvTeam::isParent(TeamTypes eTeam) const
{
	FAssert(eTeam != NO_TEAM);

	if (GET_TEAM(eTeam).isVassal(getID()))
	{
		for (int i = 0; i < MAX_PLAYERS; ++i)
		{
			CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)i);
			if (kLoopPlayer.isAlive() && kLoopPlayer.getTeam() == eTeam)
			{
				if (kLoopPlayer.getParent() != NO_PLAYER)
				{
					if (GET_PLAYER(kLoopPlayer.getParent()).getTeam() == getID())
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool CvTeam::isHasTech(TechTypes eIndex) const
{
	if (eIndex == NO_TECH)
	{
		return true;
	}

	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumTechInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(m_pabHasTech != NULL, "m_pabHasTech is not expected to be equal with NULL");
	return m_pabHasTech[eIndex];
}

void CvTeam::announceTechToPlayers(TechTypes eIndex, bool bPartial)
{
	bool bSound = ((GC.getGameINLINE().isNetworkMultiPlayer() || gDLL->getInterfaceIFace()->noTechSplash()) && !bPartial);

	for (int iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				MEMORY_TRACK_EXEMPT();

				CvWString szBuffer = gDLL->getText((bPartial ? "TXT_KEY_MISC_PROGRESS_TOWARDS_TECH" : "TXT_KEY_MISC_YOU_DISCOVERED_TECH"), GC.getTechInfo(eIndex).getTextKeyWide());

				AddDLLMessage(((PlayerTypes)iI), false, (bSound ? GC.getEVENT_MESSAGE_TIME() : -1), szBuffer, (bSound ? GC.getTechInfo(eIndex).getSoundMP() : NULL), MESSAGE_TYPE_MAJOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_TECH_TEXT"));
			}
		}
	}
}

void CvTeam::setHasTech(TechTypes eIndex, bool bNewValue, PlayerTypes ePlayer, bool bFirst, bool bAnnounce)
{
	PROFILE_FUNC();

	CvCity* pCapitalCity;
	CvCity* pCity;
	CvPlot* pLoopPlot;
	CvWString szBuffer;
	CivicOptionTypes eCivicOptionType;
	CivicTypes eCivicType;
	PlayerTypes eBestPlayer;
/************************************************************************************************/
/* RevDCM	                  Start		 4/29/10                                                */
/*                                                                                              */
/* OC_LIMITED_RELIGIONS                                                                         */
/************************************************************************************************/
	ReligionTypes eReligion;
	ReligionTypes eSlotReligion;
/************************************************************************************************/
/* LIMITED_RELIGIONS               END                                                          */
/************************************************************************************************/
	BonusTypes eBonus;
	UnitTypes eFreeUnit;
	bool bReligionFounded;
	bool bFirstBonus;
	int iValue;
	int iBestValue;
	int iI, iJ, iK;

	if (eIndex == NO_TECH)
	{
		return;
	}

	if (ePlayer == NO_PLAYER)
	{
		ePlayer = getLeaderID();
	}

	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumTechInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(ePlayer >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(ePlayer < MAX_PLAYERS, "ePlayer is expected to be within maximum bounds (invalid Index)");

	if (isHasTech(eIndex) != bNewValue)
	{
		if (GC.getTechInfo(eIndex).isRepeat())
		{
			m_paiTechCount[eIndex]++;

			setResearchProgress(eIndex, 0, ePlayer);

			// report event to Python
			GC.getGameINLINE().reportEvent(TEAM_EVENT_TECHACQUIRED, ePlayer, getID() | ((bAnnounce && 1 == m_paiTechCount[eIndex]) ? 0x80000000 : 0), eIndex);
			//CvEventReporter::getInstance().techAcquired(eIndex, getID(), ePlayer, bAnnounce && 1 == m_paiTechCount[eIndex]);

			if (1 == m_paiTechCount[eIndex])
			{
				if (bAnnounce)
				{
					if (GC.getGameINLINE().isFinalInitialized() && !(gDLL->GetWorldBuilderMode()))
					{
						announceTechToPlayers(eIndex);
					}
				}
			}
		}
		else
		{
			for (iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
			{
				pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);

				if (pLoopPlot->getBonusType() != NO_BONUS)
				{
					if (pLoopPlot->getTeam() == getID())
					{
						if ((GC.getBonusInfo(pLoopPlot->getBonusType()).getTechReveal() == eIndex) ||
							  (GC.getBonusInfo(pLoopPlot->getBonusType()).getTechCityTrade() == eIndex) ||
								(GC.getBonusInfo(pLoopPlot->getBonusType()).getTechObsolete() == eIndex))
						{
							pLoopPlot->updatePlotGroupBonus(false);
						}
					}
				}
			}

			m_pabHasTech[eIndex] = bNewValue;

			for (iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
			{
				pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);

				if (pLoopPlot->getBonusType() != NO_BONUS)
				{
					if (pLoopPlot->getTeam() == getID())
					{
						if ((GC.getBonusInfo(pLoopPlot->getBonusType()).getTechReveal() == eIndex) ||
							  (GC.getBonusInfo(pLoopPlot->getBonusType()).getTechCityTrade() == eIndex) ||
							  (GC.getBonusInfo(pLoopPlot->getBonusType()).getTechObsolete() == eIndex))
						{
							pLoopPlot->updatePlotGroupBonus(true);
						}
					}
				}
			}
		}

		processTech(eIndex, ((bNewValue) ? 1 : -1), bAnnounce);

		if (isHasTech(eIndex))
		{
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      10/02/09                                jdog5000      */
/*                                                                                              */
/* AI logging                                                                                   */
/************************************************************************************************/
			if( gTeamLogLevel >= 2 )
			{
				logBBAIForTeam(getID(), "    Team %d (%S) acquires tech %S", getID(), GET_PLAYER(ePlayer).getCivilizationDescription(0), GC.getTechInfo(eIndex).getDescription() );
			}
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

			for (iI = 0; iI < MAX_PLAYERS; iI++)
			{
				if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
				{
					if (GET_PLAYER((PlayerTypes)iI).getCurrentEra() < GC.getTechInfo(eIndex).getEra())
					{
						GET_PLAYER((PlayerTypes)iI).setCurrentEra((EraTypes)(GC.getTechInfo(eIndex).getEra()));
					}

					//	Reconsider civics on acquiring tech
					GET_PLAYER((PlayerTypes)iI).AI_setCivicTimer( 0 );

					//	Recalculate bonus values on acquiring a new tech
					GET_PLAYER((PlayerTypes)iI).AI_updateBonusValue();
				}
			}

			if (GC.getTechInfo(eIndex).isMapVisible())
			{
				GC.getMapINLINE().setRevealedPlots(getID(), true, true);
			}

			// report event to Python, along with some other key state
			//CvEventReporter::getInstance().techAcquired(eIndex, getID(), ePlayer, bAnnounce);
			GC.getGameINLINE().reportEvent(TEAM_EVENT_TECHACQUIRED, ePlayer, getID() | (bAnnounce ? 0x80000000 : 0), eIndex);

			bReligionFounded = false;
			bFirstBonus = false;
			
			if (bFirst && GET_PLAYER(ePlayer).getID() != BARBARIAN_TEAM)
			{
/************************************************************************************************/
/* RevDCM	                  Start		 4/29/10                                                */
/*                                                                                              */
/* OC_LIMITED_RELIGIONS                                                                         */
/************************************************************************************************/

				if (GC.getGameINLINE().countKnownTechNumTeams(eIndex) == 1)
				{
					if (!GC.getGameINLINE().isOption(GAMEOPTION_LIMITED_RELIGIONS)
					&& GC.getGameINLINE().isTechCanFoundReligion(eIndex))
					{
						for (iI = 0; iI < GC.getNumReligionInfos(); iI++)
						{
							iBestValue = MAX_INT;
							eBestPlayer = NO_PLAYER;
							eReligion = NO_RELIGION;
							eSlotReligion = ReligionTypes(iI);

							if( (GC.getReligionInfo(eSlotReligion).getTechPrereq() == eIndex)
							&& (!GC.getGameINLINE().isReligionSlotTaken(eSlotReligion))) 
							{
								for (iJ = 0; iJ < MAX_PLAYERS; iJ++)
								{
									CvPlayer& kPlayer = GET_PLAYER((PlayerTypes)iJ);
									if( kPlayer.isAlive() && (kPlayer.getTeam() == getID()) )
									{
										if (kPlayer.canFoundReligion())
										{
											iValue = 10;
											iValue += GC.getGameINLINE().getSorenRandNum(10, "3 Found Religion (Player)");

											for (iK = 0; iK < GC.getNumReligionInfos(); iK++)
											{
												iValue += (kPlayer.getHasReligionCount((ReligionTypes)iK) * 10);
											}

											if( kPlayer.getCurrentResearch() != eIndex)
											{
												iValue *= 10;
											}

											if (iValue < iBestValue)
											{
												iBestValue = iValue;
												eBestPlayer = ((PlayerTypes)iJ);
												eReligion = ReligionTypes(iI);
											}
										}	
									}
								}
							}

							if (eBestPlayer != NO_PLAYER)
							{
								//	KOSHLING - the following line commented out because it's wrogn as 
								//	far s I can see:
								//		1) This is done inside cvPlayer::foundReligion()
								//		2) The above asserts it has not already been done (and that assertion failed)
								//		3) The code below can actualyl choozse a DIFFERENT religion anyway if
								//		   the right game options are on!
								//GC.getGameINLINE().setReligionSlotTaken((ReligionTypes)iI, true);
/************************************************************************************************/
/* LIMITED_RELIGIONS               END                                                          */
/************************************************************************************************/

								if (GC.getGameINLINE().isOption(GAMEOPTION_PICK_RELIGION))
								{
									if (GET_PLAYER(eBestPlayer).isHuman())
									{
										GET_PLAYER(eBestPlayer).m_bChoosingReligion = true;
										CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_FOUND_RELIGION, iI);
										if (NULL != pInfo)
										{
											gDLL->getInterfaceIFace()->addPopup(pInfo, eBestPlayer);
										}
									} 
									else
									{
										eReligion = GET_PLAYER(eBestPlayer).AI_chooseReligion();
										if (NO_RELIGION != eReligion)
										{
											if(GC.getGameINLINE().isTechCanFoundReligion(eIndex))
											{
												GET_PLAYER(eBestPlayer).foundReligion(eReligion, eSlotReligion, true);
												bReligionFounded = true;
											}
										}
									}
								} 
								else if (NO_RELIGION != eReligion)
								{
									if(GC.getGameINLINE().isTechCanFoundReligion(eIndex) )
									{
										GET_PLAYER(eBestPlayer).foundReligion(eReligion, eSlotReligion, true);
										bReligionFounded = true;
									}
								}
								bFirstBonus = true;
/************************************************************************************************/
/* LIMITED_RELIGIONS               END                                                          */
/************************************************************************************************/
							}
						}
					}
					if(GC.getGameINLINE().countKnownTechNumTeams(eIndex) == 1)
					{
						for (iI = 0; iI < GC.getNumCorporationInfos(); ++iI)
						{
							if (GC.getCorporationInfo((CorporationTypes)iI).getTechPrereq() == eIndex)
							{
								if (!(GC.getGameINLINE().isCorporationFounded((CorporationTypes)iI)))
								{
									iBestValue = MAX_INT;
									eBestPlayer = NO_PLAYER;

									for (iJ = 0; iJ < MAX_PLAYERS; iJ++)
									{
										if (GET_PLAYER((PlayerTypes)iJ).isAlive())
										{
											if (GET_PLAYER((PlayerTypes)iJ).getTeam() == getID())
											{
												iValue = 10;

												iValue += GC.getGameINLINE().getSorenRandNum(10, "Found Corporation (Player)");

												if (GET_PLAYER((PlayerTypes)iJ).getCurrentResearch() != eIndex)
												{
													iValue *= 10;
												}

												if (iValue < iBestValue)
												{
													iBestValue = iValue;
													eBestPlayer = ((PlayerTypes)iJ);
												}
											}
										}
									}
								}

								if (eBestPlayer != NO_PLAYER)
								{
									GET_PLAYER(eBestPlayer).foundCorporation((CorporationTypes)iI);
									bFirstBonus = true;
								}
							}
						}
					}
				}
			}
			if (GC.getGameINLINE().isTechCanFoundReligion(eIndex) && GC.getGameINLINE().isOption(GAMEOPTION_LIMITED_RELIGIONS))			
			{
				for (iI = 0; iI < GC.getNumReligionInfos(); iI++)
				{
					iBestValue = MAX_INT;
					eBestPlayer = NO_PLAYER;
					eReligion = NO_RELIGION;
					eSlotReligion = ReligionTypes(iI);
					if ((GC.getReligionInfo(eSlotReligion).getTechPrereq() == eIndex) && !GC.getGameINLINE().isReligionSlotTaken(eSlotReligion))
					{
						for (iJ = 0; iJ < MAX_PLAYERS; iJ++)
						{
							CvPlayer& kPlayer = GET_PLAYER((PlayerTypes)iJ);
							if( kPlayer.isAlive() && (kPlayer.getTeam() == getID()) )
							{
								if(kPlayer.canFoundReligion())
								{
									iValue = 10;
									iValue += GC.getGameINLINE().getSorenRandNum(10, "4 Found Religion (Player)");
									for (iK = 0; iK < GC.getNumReligionInfos(); iK++)
									{
										iValue += (kPlayer.getHasReligionCount((ReligionTypes)iK) * 10);
									}
										if (kPlayer.getCurrentResearch() != eIndex)
										{
											iValue *= 10;
										}

										if (iValue < iBestValue)
										{
											iBestValue = iValue;
											eBestPlayer = ((PlayerTypes)iJ);
											eReligion = ReligionTypes(iI);
										}
								
							

									if (eBestPlayer != NO_PLAYER)
									{
								
										if (GC.getGameINLINE().isOption(GAMEOPTION_PICK_RELIGION))
										{
											if(GET_PLAYER(eBestPlayer).isHuman())
											{
/************************************************************************************************/
/* Afforess	                  Start		 06/17/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
												GET_PLAYER(eBestPlayer).m_bChoosingReligion = true;
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
												CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_FOUND_RELIGION, iI);
												if (NULL != pInfo)
												{
													gDLL->getInterfaceIFace()->addPopup(pInfo, eBestPlayer);
												}
											}
											else
											{
												ReligionTypes eReligion = GET_PLAYER(eBestPlayer).AI_chooseReligion();
												if (NO_RELIGION != eReligion)
												{
													if(GC.getGameINLINE().isTechCanFoundReligion(eIndex))
													{
														GET_PLAYER(eBestPlayer).foundReligion(eReligion, eSlotReligion, true);
														bReligionFounded = true;
													}
												}
											}
										}
										else if (NO_RELIGION != eReligion)
										{
											if(GC.getGameINLINE().isTechCanFoundReligion(eIndex))
											{
												GET_PLAYER(eBestPlayer).foundReligion(eReligion, eSlotReligion, true);
												bReligionFounded = true;
											}
										}
									}
								}
							}
						}
					}
				}
			}
/************************************************************************************************/
/* REVDCM                                  END                                                  */
/************************************************************************************************/

			for (iI = 0; iI < MAX_PLAYERS; iI++)
			{
				if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID() && GET_PLAYER((PlayerTypes)iI).isAlive())
				{
					if (GET_PLAYER((PlayerTypes)iI).isResearchingTech(eIndex))
					{
						GET_PLAYER((PlayerTypes)iI).popResearch(eIndex);
					}
					
					// notify the player they now have the tech, if they want to make immediate changes
					GET_PLAYER((PlayerTypes)iI).AI_nowHasTech(eIndex);
					GET_PLAYER((PlayerTypes)iI).invalidateYieldRankCache();
				}
			}

			if (bFirst && GET_PLAYER(ePlayer).getID() != BARBARIAN_TEAM)
			{
				if (GC.getGameINLINE().countKnownTechNumTeams(eIndex) == 1)
				{
					eFreeUnit = GET_PLAYER(ePlayer).getTechFreeUnit(eIndex);
					if (eFreeUnit != NO_UNIT)
					{
						bFirstBonus = true;
						pCapitalCity = GET_PLAYER(ePlayer).getCapitalCity();

						if (pCapitalCity != NULL)
						{
							pCapitalCity->createGreatPeople(eFreeUnit, false, false);
						}
					}

					if (GC.getTechInfo(eIndex).getFirstFreeTechs() > 0)
					{
						bFirstBonus = true;

						if (!isHuman())
						{
							for (iI = 0; iI < GC.getTechInfo(eIndex).getFirstFreeTechs(); iI++)
							{
								GET_PLAYER(ePlayer).AI_chooseFreeTech();
							}
						}
						else
						{
							UIActivityInfo	info;

							info.eType = UIACTIVITY_CHOOSE_TECH_POPUP;
							info.ePlayer = ePlayer;
							info.szwStrData = DLL_SERIALIZE(gDLL->getText("TXT_KEY_MISC_FIRST_TECH_CHOOSE_FREE", GC.getTechInfo(eIndex).getTextKeyWide()));
							info.iData1 = GC.getTechInfo(eIndex).getFirstFreeTechs();

							GC.getGameINLINE().queueUIActivity(info);
						}

						for (iI = 0; iI < MAX_PLAYERS; iI++)
						{
							if (GET_PLAYER((PlayerTypes)iI).isAlive())
							{
								MEMORY_TRACK_EXEMPT();

								if (isHasMet(GET_PLAYER((PlayerTypes)iI).getTeam()))
								{
									szBuffer = gDLL->getText("TXT_KEY_MISC_SOMEONE_FIRST_TO_TECH", GET_PLAYER(ePlayer).getNameKey(), GC.getTechInfo(eIndex).getTextKeyWide());
								}
								else
								{
									szBuffer = gDLL->getText("TXT_KEY_MISC_UNKNOWN_FIRST_TO_TECH", GC.getTechInfo(eIndex).getTextKeyWide());
								}
								AddDLLMessage(((PlayerTypes)iI), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_FIRSTTOTECH", MESSAGE_TYPE_MAJOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));
							}
						}

						szBuffer = gDLL->getText("TXT_KEY_MISC_SOMEONE_FIRST_TO_TECH", GET_PLAYER(ePlayer).getName(), GC.getTechInfo(eIndex).getTextKeyWide());
						GC.getGameINLINE().addReplayMessage(REPLAY_MESSAGE_MAJOR_EVENT, ePlayer, szBuffer, -1, -1, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));
					}
			
					if (bFirstBonus)
					{
						for (iI = 0; iI < MAX_PLAYERS; iI++)
						{
							if (GET_PLAYER((PlayerTypes)iI).isAlive())
							{
								if (!(GET_PLAYER((PlayerTypes)iI).isHuman()))
								{
									if (GET_PLAYER((PlayerTypes)iI).isResearchingTech(eIndex))
									{
										GET_PLAYER((PlayerTypes)iI).clearResearchQueue();
									}
								}
							}
						}
					}
				}
			}

			if (bAnnounce)
			{
				if (GC.getGameINLINE().isFinalInitialized() && !(gDLL->GetWorldBuilderMode()))
				{
					announceTechToPlayers(eIndex);

					for (iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
					{
						pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);

						if (pLoopPlot->getTeam() == getID())
						{
							eBonus = pLoopPlot->getBonusType();

							if (eBonus != NO_BONUS)
							{
								if (GC.getBonusInfo(eBonus).getTechReveal() == eIndex && !isForceRevealedBonus(eBonus))
								{
									pCity = GC.getMapINLINE().findCity(pLoopPlot->getX_INLINE(), pLoopPlot->getY_INLINE(), NO_PLAYER, getID(), false);

									if (pCity != NULL)
									{
										MEMORY_TRACK_EXEMPT();

										szBuffer = gDLL->getText("TXT_KEY_MISC_YOU_DISCOVERED_BONUS", GC.getBonusInfo(eBonus).getTextKeyWide(), pCity->getNameKey());
										AddDLLMessage(pLoopPlot->getOwnerINLINE(), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_DISCOVERBONUS", MESSAGE_TYPE_INFO, GC.getBonusInfo(eBonus).getButton(), (ColorTypes)GC.getInfoTypeForString("COLOR_WHITE"), pLoopPlot->getX_INLINE(), pLoopPlot->getY_INLINE(), true, true);
									}
									/************************************************************************************************/
									/* Afforess	                  Start		 01/20/10                                               */
									/*                                                                                              */
									/*  Mine Depletion. Newly Revealed Resources Allow Depleted Mines to be Rebuilt                 */
									/************************************************************************************************/
									if (pLoopPlot->isDepletedMine())
									{
										pLoopPlot->setIsDepletedMine(false);
									}
									/************************************************************************************************/
									/* Afforess	                     END                                                            */
									/************************************************************************************************/

								}
							}
						}
					}
				}
				

				for (iI = 0; iI < MAX_PLAYERS; iI++)
				{
					if (GET_PLAYER((PlayerTypes)iI).isAlive())
					{
						if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
						{
							if (GET_PLAYER((PlayerTypes)iI).isHuman())
							{
								if (!bReligionFounded || (GET_PLAYER((PlayerTypes)iI).getLastStateReligion() != NO_RELIGION) || (iI != ePlayer))
								{
									if (GET_PLAYER((PlayerTypes)iI).canRevolution(NULL))
									{
										eCivicOptionType = NO_CIVICOPTION;
										eCivicType = NO_CIVIC;

										for (iJ = 0; iJ < GC.getNumCivicOptionInfos(); iJ++)
										{
											if (!(GET_PLAYER((PlayerTypes)iI).isHasCivicOption((CivicOptionTypes)iJ)))
											{
												for (iK = 0; iK < GC.getNumCivicInfos(); iK++)
												{
													if (GC.getCivicInfo((CivicTypes)iK).getCivicOptionType() == iJ)
													{
														if (GC.getCivicInfo((CivicTypes)iK).getTechPrereq() == eIndex)
														{
															eCivicOptionType = ((CivicOptionTypes)iJ);
															eCivicType = ((CivicTypes)iK);
														}
													}
												}
											}
										}

										if ((eCivicOptionType != NO_CIVICOPTION) && (eCivicType != NO_CIVIC))
										{
											UIActivityInfo	info;

											info.eType = UIACTIVITY_CHANGE_CIVIC_POPUP;
											info.ePlayer = (PlayerTypes)iI;
											info.iData1 = eCivicOptionType;
											info.iData2 = eCivicType;

											GC.getGameINLINE().queueUIActivity(info);
										}
									}
								}
							}
						}
					}
				}
			}
	

			for (iI = 0; iI < MAX_TEAMS; iI++)
			{
				if (GET_TEAM((TeamTypes)iI).isAlive())
				{
					if (iI != getID())
					{
						GET_TEAM((TeamTypes)iI).updateTechShare(eIndex);
					}
				}
			}
		

			if (bNewValue)
			{
				if (bAnnounce)
				{
					if (GC.getGameINLINE().isFinalInitialized() && !(gDLL->GetWorldBuilderMode()))
					{
						FAssert(ePlayer != NO_PLAYER);
						if (GET_PLAYER(ePlayer).isResearch() && (GET_PLAYER(ePlayer).getCurrentResearch() == NO_TECH))
						{
							if (ePlayer == GC.getGameINLINE().getActivePlayer())
							{
								UIActivityInfo	info;

								info.eType = UIACTIVITY_CHOOSE_TECH_POPUP;
								info.ePlayer = ePlayer;
								info.szwStrData = DLL_SERIALIZE(gDLL->getText("TXT_KEY_MISC_WHAT_TO_RESEARCH_NEXT"));
								info.iData1 = 0;

								GC.getGameINLINE().queueUIActivity(info);
							}
						}
					}
				}
			}

			if (getID() == GC.getGameINLINE().getActiveTeam())
			{
				gDLL->getInterfaceIFace()->setDirty(MiscButtons_DIRTY_BIT, true);
				gDLL->getInterfaceIFace()->setDirty(SelectionButtons_DIRTY_BIT, true);
				gDLL->getInterfaceIFace()->setDirty(ResearchButtons_DIRTY_BIT, true);
				gDLL->getInterfaceIFace()->setDirty(GlobeLayer_DIRTY_BIT, true);
			}
//45deg: rev1086, workaround for multiple resources bug - START
			for (int iJ = 0; iJ < GC.getNumTerrainInfos(); iJ++)
			{
				if ((GC.getTechInfo(eIndex).isTerrainTrade(iJ)) && (isHuman()))
				{
					GC.getGameINLINE().recalculateModifiers();
					GC.getGameINLINE().logMsg("Warning, might cause OOS in MP!");
				}
			}
//45deg: rev1086, workaround for multiple resources bug - END
		}
	}
}


bool CvTeam::isNoTradeTech(TechTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumTechInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_pabNoTradeTech[eIndex];
}


void CvTeam::setNoTradeTech(TechTypes eIndex, bool bNewValue)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumTechInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	m_pabNoTradeTech[eIndex] = bNewValue;
}


int CvTeam::getImprovementYieldChange(ImprovementTypes eIndex1, YieldTypes eIndex2) const
{
	FAssertMsg(eIndex1 >= 0, "eIndex1 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex1 < GC.getNumImprovementInfos(), "eIndex1 is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(eIndex2 >= 0, "eIndex2 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex2 < NUM_YIELD_TYPES, "eIndex2 is expected to be within maximum bounds (invalid Index)");
	return m_ppaaiImprovementYieldChange[eIndex1][eIndex2];
}


void CvTeam::changeImprovementYieldChange(ImprovementTypes eIndex1, YieldTypes eIndex2, int iChange)
{
	FAssertMsg(eIndex1 >= 0, "eIndex1 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex1 < GC.getNumImprovementInfos(), "eIndex1 is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(eIndex2 >= 0, "eIndex2 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex2 < NUM_YIELD_TYPES, "eIndex2 is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_ppaaiImprovementYieldChange[eIndex1][eIndex2] = (m_ppaaiImprovementYieldChange[eIndex1][eIndex2] + iChange);
		FAssert(eIndex1 >= 0);
		FAssert(eIndex2 >= 0);

		updateYield();
	}
}


/*	f1rpo (from K-Mod; note that I haven't endeavored to fix those "heaps of bugs"
	- to the extent that they may also exist in AND):
	In the original code, there seems to be a lot of confusion about what
	the exact conditions are for a bonus being connected. There were heaps of bugs
	where CvImprovementInfo::isImprovementBonusTrade was mistakenly used as the
	sole condition for a bonus being connected or not. I created this function
	to make the situation a bit more clear... */
bool CvTeam::doesImprovementConnectBonus(ImprovementTypes eImprovement, BonusTypes eBonus) const
{
	if (eImprovement == NO_IMPROVEMENT || eBonus == NO_BONUS)
		return false;

	CvBonusInfo const& kBonus = GC.getInfo(eBonus);
	if (!isHasTech((TechTypes)kBonus.getTechCityTrade())
		|| (kBonus.getTechObsolete() != NO_TECH &&
		isHasTech((TechTypes)kBonus.getTechObsolete())))
	{
		return false;
	}
	CvImprovementInfo const& kImprovement = GC.getInfo(eImprovement);
	return (kImprovement.isImprovementBonusTrade(eBonus)
			|| kImprovement.isActsAsCity());
}


void CvTeam::doWarWeariness()
{
	int iI;

	for (iI = 0; iI < MAX_TEAMS; iI++)
	{
		if (getWarWeariness((TeamTypes)iI) > 0)
		{
			changeWarWeariness(((TeamTypes)iI), 100 * GC.getDefineINT("WW_DECAY_RATE"));

/********************************************************************************/
/**		REVOLUTION_MOD							6/22/08				jdog5000	*/
/**																				*/
/**																				*/
/********************************************************************************/
			//if (!(GET_TEAM((TeamTypes)iI).isAlive()) || !isAtWar((TeamTypes)iI) || GC.getGameINLINE().isOption(GAMEOPTION_ALWAYS_WAR) || GC.getGameINLINE().isOption(GAMEOPTION_NO_CHANGING_WAR_PEACE))
			// War weariness decays faster when enemy can't fight, usually a rebel with just spies
			if ( ( !(GET_TEAM((TeamTypes)iI).isAlive()) || GET_TEAM((TeamTypes)iI).getNumMilitaryUnits() == 0 ) || !isAtWar((TeamTypes)iI) || GC.getGameINLINE().isOption(GAMEOPTION_ALWAYS_WAR) || GC.getGameINLINE().isOption(GAMEOPTION_NO_CHANGING_WAR_PEACE))
			{
				setWarWeariness(((TeamTypes)iI), ((getWarWeariness((TeamTypes)iI) * GC.getDefineINT("WW_DECAY_PEACE_PERCENT")) / 100));
			}
/********************************************************************************/
/**		REVOLUTION_MOD							END								*/
/********************************************************************************/
		}
	}
}

void CvTeam::updateTechShare(TechTypes eTech)
{
	int iBestShare;
	int iCount;
	int iI;

	if (isHasTech(eTech))
	{
		return;
	}

	iBestShare = MAX_INT;

	for (iI = 0; iI < MAX_TEAMS; iI++)
	{
		if (isTechShare(iI))
		{
			iBestShare = std::min(iBestShare, (iI + 1));
		}
	}

	if (iBestShare != MAX_INT)
	{
		iCount = 0;

		for (iI = 0; iI < MAX_CIV_TEAMS; iI++)
		{
			if (GET_TEAM((TeamTypes)iI).isAlive())
			{
				if (GET_TEAM((TeamTypes)iI).isHasTech(eTech))
				{
					if (isHasMet((TeamTypes)iI))
					{
						FAssertMsg(iI != getID(), "iI is not expected to be equal with getID()");
						iCount++;
					}
				}
			}
		}

		if (iCount >= iBestShare)
		{
			setHasTech(eTech, true, NO_PLAYER, true, true);
		}
	}
}


void CvTeam::updateTechShare()
{
	int iI;

	for (iI = 0; iI < GC.getNumTechInfos(); iI++)
	{
		updateTechShare((TechTypes)iI);
	}
}


void CvTeam::testCircumnavigated()
{
	CvPlot* pPlot;
	CvWString szBuffer;
	bool bFoundVisible;
	int iX, iY;

	if (isBarbarian())
	{
		return;
	}

	if (!GC.getGameINLINE().circumnavigationAvailable())
	{
		return;
	}

	if (GC.getMapINLINE().isWrapXINLINE())
	{
		for (iX = 0; iX < GC.getMapINLINE().getGridWidthINLINE(); iX++)
		{
			bFoundVisible = false;

			for (iY = 0; iY < GC.getMapINLINE().getGridHeightINLINE(); iY++)
			{
				pPlot = GC.getMapINLINE().plotSorenINLINE(iX, iY);

				if (pPlot->isRevealed(getID(), false))
				{
					bFoundVisible = true;
					break;
				}
			}

			if (!bFoundVisible)
			{
				return;
			}
		}
	}

	if (GC.getMapINLINE().isWrapYINLINE())
	{
		for (iY = 0; iY < GC.getMapINLINE().getGridHeightINLINE(); iY++)
		{
			bFoundVisible = false;

			for (iX = 0; iX < GC.getMapINLINE().getGridWidthINLINE(); iX++)
			{
				pPlot = GC.getMapINLINE().plotSorenINLINE(iX, iY);

				if (pPlot->isRevealed(getID(), false))
				{
					bFoundVisible = true;
					break;
				}
			}

			if (!bFoundVisible)
			{
				return;
			}
		}
	}

	GC.getGameINLINE().makeCircumnavigated(getID());

	if (GC.getGameINLINE().getElapsedGameTurns() > 0)
	{
		if (GC.getDefineINT("CIRCUMNAVIGATE_FREE_MOVES") != 0)
		{
			changeExtraMoves(DOMAIN_SEA, GC.getDefineINT("CIRCUMNAVIGATE_FREE_MOVES"));

			for (int iI = 0; iI < MAX_PLAYERS; iI++)
			{
				if (GET_PLAYER((PlayerTypes)iI).isAlive())
				{
					MEMORY_TRACK_EXEMPT();

					if (getID() == GET_PLAYER((PlayerTypes)iI).getTeam())
					{
						szBuffer = gDLL->getText("TXT_KEY_MISC_YOU_CIRC_GLOBE", GC.getDefineINT("CIRCUMNAVIGATE_FREE_MOVES"));
					}
					else if (isHasMet(GET_PLAYER((PlayerTypes)iI).getTeam()))
					{
						szBuffer = gDLL->getText("TXT_KEY_MISC_SOMEONE_CIRC_GLOBE", getName().GetCString());
					}
					else
					{
						szBuffer = gDLL->getText("TXT_KEY_MISC_UNKNOWN_CIRC_GLOBE");
					}
					AddDLLMessage(((PlayerTypes)iI), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_GLOBECIRCUMNAVIGATED", MESSAGE_TYPE_MAJOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));
				}
			}

			szBuffer = gDLL->getText("TXT_KEY_MISC_SOMEONE_CIRC_GLOBE", getName().GetCString());
			GC.getGameINLINE().addReplayMessage(REPLAY_MESSAGE_MAJOR_EVENT, getLeaderID(), szBuffer, -1, -1, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));
		}
	}
}

void CvTeam::processTech(TechTypes eTech, int iChange, bool bAnnounce)
{
	PROFILE_FUNC();

	CvPlot* pLoopPlot;
	BonusTypes eBonus;
	int iI, iJ;

	if (GC.getTechInfo(eTech).isExtraWaterSeeFrom())
	{
		changeExtraWaterSeeFromCount(iChange);
	}

	if (GC.getTechInfo(eTech).isMapCentering())
	{
		if (iChange > 0)
		{
			setMapCentering(true);
		}
	}

	if (GC.getTechInfo(eTech).isMapTrading())
	{
		changeMapTradingCount(iChange);
	}
/************************************************************************************************/
/* Afforess	                  Start		 03/8/10                                                */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	CvTechInfo& kTech = GC.getTechInfo(eTech);
	if (kTech.isCanPassPeaks())
	{
		changeCanPassPeaksCount(iChange);
	}

	if (kTech.isMoveFastPeaks())
	{
		changeMoveFastPeaksCount(iChange);
	}

	if (kTech.isCanFoundOnPeaks())
	{
		changeCanFoundOnPeaksCount(iChange);
		//	Koshling - makes peaks workable which chnages the yield calculation
		updateYield();
	}

	if (iChange > 0)
	{
		ObsoletePromotions(eTech);
		ObsoleteCorporations(eTech);
	}

	if (kTech.isEmbassyTrading())
	{
		changeEmbassyTradingCount(iChange);
	}

	if (kTech.isRebaseAnywhere())
	{
		changeRebaseAnywhereCount(iChange);
	}
// Movement Limits by 45deg - START
	if (kTech.isExtendMovementLimits())
	{
		changeExtendMovementLimitsCount(iChange);
	}

	if (kTech.isRemoveMovementLimits())
	{
		changeRemoveMovementLimitsCount(iChange);
	}
// Movement Limits by 45deg - END	
	if (kTech.getGlobalTradeModifier() != 0)
	{
		changeTradeModifier(kTech.getGlobalTradeModifier() * iChange);
	}

	if (kTech.getGlobalForeignTradeModifier() != 0)
	{
		changeForeignTradeModifier(kTech.getGlobalForeignTradeModifier() * iChange);
	}
	
	if (kTech.getTradeMissionModifier() != 0)
	{
		changeTradeMissionModifier(kTech.getTradeMissionModifier() * iChange);
	}
	
	if (kTech.getCorporationRevenueModifier() != 0)
	{
		changeCorporationRevenueModifier(kTech.getCorporationRevenueModifier() * iChange);
	}
	
	if (kTech.getCorporationMaintenanceModifier() != 0)
	{
		changeCorporationMaintenanceModifier(kTech.getCorporationMaintenanceModifier() * iChange);
	}
	
	if (kTech.isEnablesDesertFarming())
	{
		changeCanFarmDesertCount(iChange);
	}
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
	if (GC.getTechInfo(eTech).isTechTrading())
	{
		changeTechTradingCount(iChange);
	}

	if (GC.getTechInfo(eTech).isGoldTrading())
	{
		changeGoldTradingCount(iChange);
	}

	if (GC.getTechInfo(eTech).isOpenBordersTrading())
	{
		changeOpenBordersTradingCount(iChange);
/************************************************************************************************/
/* Afforess	                  Start		 05/22/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
		changeLimitedBordersTradingCount(iChange);
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
	}

	if (GC.getTechInfo(eTech).isDefensivePactTrading())
	{
		changeDefensivePactTradingCount(iChange);
	}

	if (GC.getTechInfo(eTech).isPermanentAllianceTrading())
	{
		changePermanentAllianceTradingCount(iChange);
	}

	if (GC.getTechInfo(eTech).isVassalStateTrading())
	{
		changeVassalTradingCount(iChange);
	}

	if (GC.getTechInfo(eTech).isBridgeBuilding())
	{
		changeBridgeBuildingCount(iChange);
	}

	if (GC.getTechInfo(eTech).isIrrigation())
	{
		changeIrrigationCount(iChange);
	}

	if (GC.getTechInfo(eTech).isIgnoreIrrigation())
	{
		changeIgnoreIrrigationCount(iChange);
	}

	if (GC.getTechInfo(eTech).isWaterWork())
	{
		changeWaterWorkCount(iChange);
	}

	for (iI = 0; iI < GC.getNumRouteInfos(); iI++)
	{
		changeRouteChange(((RouteTypes)iI), (GC.getRouteInfo((RouteTypes) iI).getTechMovementChange(eTech) * iChange));
	}

	for (iI = 0; iI < NUM_DOMAIN_TYPES; iI++)
	{
		changeExtraMoves(((DomainTypes)iI), (GC.getTechInfo(eTech).getDomainExtraMoves(iI) * iChange));
	}

	for (iI = 0; iI < NUM_COMMERCE_TYPES; iI++)
	{
		if (GC.getTechInfo(eTech).isCommerceFlexible(iI))
		{
			changeCommerceFlexibleCount(((CommerceTypes)iI), iChange);
		}
	}

	for (iI = 0; iI < GC.getNumTerrainInfos(); iI++)
	{
		if (GC.getTechInfo(eTech).isTerrainTrade(iI))
		{
			changeTerrainTradeCount(((TerrainTypes)iI), iChange);
		}
	}

	if (GC.getTechInfo(eTech).isRiverTrade())
	{
		changeRiverTradeCount(iChange);
	}

	for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
	{
		if (GC.getBuildingInfo((BuildingTypes) iI).getObsoleteTech() == eTech)
		{
			changeObsoleteBuildingCount(((BuildingTypes)iI), iChange);
		}

		if (GC.getBuildingInfo((BuildingTypes) iI).getSpecialBuildingType() != NO_SPECIALBUILDING)
		{
			if (GC.getSpecialBuildingInfo((SpecialBuildingTypes) GC.getBuildingInfo((BuildingTypes) iI).getSpecialBuildingType()).getObsoleteTech() == eTech)
			{
				changeObsoleteBuildingCount(((BuildingTypes)iI), iChange);
			}
		}
/************************************************************************************************/
/* Afforess	                  Start		 03/8/10                                                */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
		for (iJ = 0; iJ < NUM_COMMERCE_TYPES; iJ++)
		{
			changeBuildingCommerceChange(((BuildingTypes)iI), ((CommerceTypes)iJ), (GC.getBuildingInfo((BuildingTypes)iI).getTechCommerceChange(eTech, iJ) * iChange));
			changeBuildingCommerceModifier(((BuildingTypes)iI), ((CommerceTypes)iJ), (GC.getBuildingInfo((BuildingTypes)iI).getTechCommerceModifier(eTech, iJ) * iChange));
		}

		for (iJ = 0; iJ < NUM_YIELD_TYPES; iJ++)
		{
			changeBuildingYieldChange(((BuildingTypes)iI), ((YieldTypes)iJ), (GC.getBuildingInfo((BuildingTypes)iI).getTechYieldChange(eTech, iJ) * iChange));
			changeBuildingYieldModifier(((BuildingTypes)iI), ((YieldTypes)iJ), (GC.getBuildingInfo((BuildingTypes)iI).getTechYieldModifier(eTech, iJ) * iChange));
		}

		for (iJ = 0; iJ < GC.getNumSpecialistInfos(); iJ++)
		{
			changeBuildingSpecialistChange(((BuildingTypes)iI), ((SpecialistTypes)iJ), (GC.getBuildingInfo((BuildingTypes)iI).getTechSpecialistChange(eTech, iJ) * iChange));
		}
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
	}

	for (iI = 0; iI < GC.getNumImprovementInfos(); iI++)
	{
		for (iJ = 0; iJ < NUM_YIELD_TYPES; iJ++)
		{
			changeImprovementYieldChange(((ImprovementTypes)iI), ((YieldTypes)iJ), (GC.getImprovementInfo((ImprovementTypes)iI).getTechYieldChanges(eTech, iJ) * iChange));
		}
	}

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID() && GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			GET_PLAYER((PlayerTypes)iI).changeFeatureProductionModifier(GC.getTechInfo(eTech).getFeatureProductionModifier() * iChange);
			GET_PLAYER((PlayerTypes)iI).changeWorkerSpeedModifier(GC.getTechInfo(eTech).getWorkerSpeedModifier() * iChange);
			GET_PLAYER((PlayerTypes)iI).changeTradeRoutes(GC.getTechInfo(eTech).getTradeRoutes() * iChange);
			GET_PLAYER((PlayerTypes)iI).changeExtraHealth(GC.getTechInfo(eTech).getHealth() * iChange);
			GET_PLAYER((PlayerTypes)iI).changeExtraHappiness(GC.getTechInfo(eTech).getHappiness() * iChange);
			
			//DPII < Maintenance Modifiers >
			GET_PLAYER((PlayerTypes)iI).changeDistanceMaintenanceModifier(GC.getTechInfo(eTech).getDistanceMaintenanceModifier() * iChange);
			GET_PLAYER((PlayerTypes)iI).changeNumCitiesMaintenanceModifier(GC.getTechInfo(eTech).getNumCitiesMaintenanceModifier() * iChange);
			GET_PLAYER((PlayerTypes)iI).changeMaintenanceModifier(GC.getTechInfo(eTech).getMaintenanceModifier() * iChange);
			GET_PLAYER((PlayerTypes)iI).changeCoastalDistanceMaintenanceModifier(GC.getTechInfo(eTech).getCoastalDistanceMaintenanceModifier() * iChange);
			//DPII < Maintenance Modifiers >

			GET_PLAYER((PlayerTypes)iI).changeAssets(GC.getTechInfo(eTech).getAssetValue() * iChange);
			GET_PLAYER((PlayerTypes)iI).changeTechPower(GC.getTechInfo(eTech).getPowerValue() * iChange);
			GET_PLAYER((PlayerTypes)iI).changeTechScore(getTechScore(eTech) * iChange);
			
/************************************************************************************************/
/* Afforess	                  Start		 03/8/10                                                */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
			GET_PLAYER((PlayerTypes)iI).changeTechInflation(kTech.getInflationModifier() * iChange);
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
			//ls612: Tech Commerce Modifiers (repositioned for optimal processing by TB)
			for (int iJ = 0; iJ < NUM_COMMERCE_TYPES; iJ++)
			{
				GET_PLAYER((PlayerTypes)iI).changeCommerceRateModifier(((CommerceTypes)iJ), GC.getTechInfo(eTech).getCommerceModifier(iJ) * iChange);
			}
		}
	}
/************************************************************************************************/
/* Afforess	                  Start		 05/22/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	for (iI = 0; iI < GC.getNumBuildingInfos(); ++iI)
	{
		changeTechExtraBuildingHappiness((BuildingTypes)iI, GC.getBuildingInfo((BuildingTypes)iI).getTechHappinessChanges(eTech) * iChange);
		changeTechExtraBuildingHealth((BuildingTypes)iI, GC.getBuildingInfo((BuildingTypes)iI).getTechHealthChanges(eTech) * iChange);
	}
	for (iI = 0; iI < GC.getNumUnitClassInfos(); ++iI)
	{
		changeUnitClassStrengthChange((UnitClassTypes)iI, kTech.getUnitClassStrengthChange(iI) * iChange);
	}
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/	

	for (iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
	{
		pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);

		eBonus = pLoopPlot->getBonusType();

		if (eBonus != NO_BONUS)
		{
			if (GC.getBonusInfo(eBonus).getTechReveal() == eTech)
			{
				pLoopPlot->updateYield();
				pLoopPlot->setLayoutDirty(true);
			}
		}
	}

	for (iI = 0; iI < GC.getNumBuildInfos(); iI++)
	{
		if (GC.getBuildInfo((BuildTypes) iI).getTechPrereq() == eTech)
		{
			if (GC.getBuildInfo((BuildTypes) iI).getRoute() != NO_ROUTE)
			{
				for (int iJ = 0; iJ < MAX_PLAYERS; iJ++)
				{
					if (GET_PLAYER((PlayerTypes)iJ).getTeam() == getID() && GET_PLAYER((PlayerTypes)iJ).isAlive())
					{
						GET_PLAYER((PlayerTypes)iJ).processNewRoutes();
					}
				}
			}
		}
	}

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID() && GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			GET_PLAYER((PlayerTypes)iI).updateCorporation();

			//	A new tech can effect best plot build decisions so mark stale in all cities
			int iLoop = 0;
			CvCity* pLoopCity;

			for (pLoopCity = GET_PLAYER((PlayerTypes)iI).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER((PlayerTypes)iI).nextCity(&iLoop))
			{
				pLoopCity->AI_markBestBuildValuesStale();

				//	Tech changes may change what is buildable
				pLoopCity->FlushCanConstructCache();
				//Afforess: techs change unit training abilities too
				pLoopCity->clearCanTrainCache(true);
			}
		}
	}

	if ( iChange > 0 )
	{
		for (iI = 0; iI < GC.getNumSpecialBuildingInfos(); ++iI)
		{
			if (eTech == GC.getSpecialBuildingInfo((SpecialBuildingTypes)iI).getTechPrereqAnyone())
			{
				GC.getGameINLINE().makeSpecialBuildingValid((SpecialBuildingTypes)iI, bAnnounce);
			}
		}
	}
}


void CvTeam::cancelDefensivePacts()
{
	CLLNode<TradeData>* pNode;
	CvDeal* pLoopDeal;
	bool bCancelDeal;
	int iLoop = 0;

	for (pLoopDeal = GC.getGameINLINE().firstDeal(&iLoop); pLoopDeal != NULL; pLoopDeal = GC.getGameINLINE().nextDeal(&iLoop))
	{
		bCancelDeal = false;

		if ((GET_PLAYER(pLoopDeal->getFirstPlayer()).getTeam() == getID()) ||
			(GET_PLAYER(pLoopDeal->getSecondPlayer()).getTeam() == getID()))
		{
			for (pNode = pLoopDeal->headFirstTradesNode(); (pNode != NULL); pNode = pLoopDeal->nextFirstTradesNode(pNode))
			{
				if (pNode->m_data.m_eItemType == TRADE_DEFENSIVE_PACT)
				{
					bCancelDeal = true;
					break;
				}
			}

			if (!bCancelDeal)
			{
				for (pNode = pLoopDeal->headSecondTradesNode(); (pNode != NULL); pNode = pLoopDeal->nextSecondTradesNode(pNode))
				{
					if (pNode->m_data.m_eItemType == TRADE_DEFENSIVE_PACT)
					{
						bCancelDeal = true;
						break;
					}
				}
			}
		}

		if (bCancelDeal)
		{
			pLoopDeal->kill();
		}
	}
}


bool CvTeam::isFriendlyTerritory(TeamTypes eTeam) const
{
	if (eTeam == NO_TEAM)
	{
		return false;
	}

	if (eTeam == getID())
	{
		return true;
	}

	if (GET_TEAM(eTeam).isVassal(getID()))
	{
		return true;
	}

	if (isVassal(eTeam) && isOpenBorders(eTeam))
	{
		return true;
	}

	return false;
}

int CvTeam::getEspionagePointsAgainstTeam(TeamTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_aiEspionagePointsAgainstTeam[eIndex];
}

void CvTeam::setEspionagePointsAgainstTeam(TeamTypes eIndex, int iValue)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iValue != getEspionagePointsAgainstTeam(eIndex))
	{
		m_aiEspionagePointsAgainstTeam[eIndex] = iValue;

		verifySpyUnitsValidPlot();
		GET_TEAM(eIndex).verifySpyUnitsValidPlot();
	}
}

void CvTeam::changeEspionagePointsAgainstTeam(TeamTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");

	setEspionagePointsAgainstTeam(eIndex, getEspionagePointsAgainstTeam(eIndex) + iChange);
}

int CvTeam::getEspionagePointsEver() const
{
	return m_iEspionagePointsEver;
}

void CvTeam::setEspionagePointsEver(int iValue)
{
	if (iValue != getEspionagePointsEver())
	{
		m_iEspionagePointsEver = iValue;
	}
}

void CvTeam::changeEspionagePointsEver(int iChange)
{
	setEspionagePointsEver(getEspionagePointsEver() + iChange);
}

int CvTeam::getCounterespionageTurnsLeftAgainstTeam(TeamTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_aiCounterespionageTurnsLeftAgainstTeam[eIndex];
}

void CvTeam::setCounterespionageTurnsLeftAgainstTeam(TeamTypes eIndex, int iValue)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iValue != getCounterespionageTurnsLeftAgainstTeam(eIndex))
	{
		m_aiCounterespionageTurnsLeftAgainstTeam[eIndex] = iValue;

		gDLL->getInterfaceIFace()->setDirty(Espionage_Advisor_DIRTY_BIT, true);
	}
}

void CvTeam::changeCounterespionageTurnsLeftAgainstTeam(TeamTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");

	setCounterespionageTurnsLeftAgainstTeam(eIndex, getCounterespionageTurnsLeftAgainstTeam(eIndex) + iChange);
}

int CvTeam::getCounterespionageModAgainstTeam(TeamTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_aiCounterespionageModAgainstTeam[eIndex];
}

void CvTeam::setCounterespionageModAgainstTeam(TeamTypes eIndex, int iValue)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iValue != getCounterespionageModAgainstTeam(eIndex))
	{
		m_aiCounterespionageModAgainstTeam[eIndex] = iValue;

		gDLL->getInterfaceIFace()->setDirty(Espionage_Advisor_DIRTY_BIT, true);
	}
}

void CvTeam::changeCounterespionageModAgainstTeam(TeamTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");

	setCounterespionageModAgainstTeam(eIndex, getCounterespionageModAgainstTeam(eIndex) + iChange);
}

void CvTeam::verifySpyUnitsValidPlot()
{
	std::vector<CvUnit*> aUnits;

	for (int iPlayer = 0; iPlayer < MAX_PLAYERS; ++iPlayer)
	{
		CvPlayer& kPlayer = GET_PLAYER((PlayerTypes)iPlayer);

		if (kPlayer.isAlive() && kPlayer.getTeam() == getID())
		{
			int iLoop = 0;
			for (CvUnit* pUnit = kPlayer.firstUnit(&iLoop); pUnit != NULL; pUnit = kPlayer.nextUnit(&iLoop))
			{
				if ( pUnit->plot() != NULL )
				{
					PlayerTypes eOwner = pUnit->plot()->getOwnerINLINE();
					if (NO_PLAYER != eOwner)
					{
						if (pUnit->isSpy())
						{
							if (!kPlayer.canSpiesEnterBorders(eOwner))
							{
								aUnits.push_back(pUnit);
							}
						}
					}
				}
			}
		}
	}

	for (uint i = 0; i < aUnits.size(); ++i)
	{
		aUnits[i]->jumpToNearestValidPlot();
	}
}


void CvTeam::setForceRevealedBonus(BonusTypes eBonus, bool bRevealed)
{
	if (isForceRevealedBonus(eBonus) == bRevealed)
	{
		return;
	}

	for (int iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); ++iI)
	{
		CvPlot* pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);

		if (pLoopPlot->getBonusType() == eBonus)
		{
			if (pLoopPlot->getTeam() == getID())
			{
				pLoopPlot->updatePlotGroupBonus(false);
			}
		}
	}

	if (bRevealed)
	{
		m_aeRevealedBonuses.push_back(eBonus);
	}
	else
	{
		std::vector<BonusTypes>::iterator it;

		for (it = m_aeRevealedBonuses.begin(); it != m_aeRevealedBonuses.end(); ++it)
		{
			if (*it == eBonus)
			{
				m_aeRevealedBonuses.erase(it);
				break;
			}
		}
	}

	for (int iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); ++iI)
	{
		CvPlot* pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);

		if (pLoopPlot->getBonusType() == eBonus)
		{
			if (pLoopPlot->getTeam() == getID())
			{
				pLoopPlot->updatePlotGroupBonus(true);
			}
		}
	}

	for (int iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); ++iI)
	{
		CvPlot* pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);

		if (pLoopPlot->getBonusType() == eBonus)
		{
			pLoopPlot->updateYield();
			pLoopPlot->setLayoutDirty(true);
		}
	}
}

bool CvTeam::isForceRevealedBonus(BonusTypes eBonus) const
{
	std::vector<BonusTypes>::const_iterator it;

	for (it = m_aeRevealedBonuses.begin(); it != m_aeRevealedBonuses.end(); ++it)
	{
		if (*it == eBonus)
		{
			return true;
		}
	}

	return false;
}

int CvTeam::countNumHumanGameTurnActive() const
{
	int iCount = 0;

	for (int iI = 0; iI < MAX_CIV_PLAYERS; iI++)
	{
		CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iI);

		if (kLoopPlayer.isHuman() && kLoopPlayer.getTeam() == getID())
		{
			if (kLoopPlayer.isTurnActive())
			{
				++iCount;
			}
		}
	}

	return iCount;
}

void CvTeam::setTurnActive(bool bNewValue, bool bDoTurn)
{
	FAssert(GC.getGameINLINE().isSimultaneousTeamTurns());

	for (int iPlayer = 0; iPlayer < MAX_PLAYERS; ++iPlayer)
	{
		CvPlayer& kPlayer = GET_PLAYER((PlayerTypes)iPlayer);
		if (kPlayer.isAlive() && kPlayer.getTeam() == getID())
		{
			kPlayer.setTurnActive(bNewValue, bDoTurn);
		}
	}
}

bool CvTeam::isTurnActive() const
{
	FAssert(GC.getGameINLINE().isSimultaneousTeamTurns());

	for (int iPlayer = 0; iPlayer < MAX_PLAYERS; ++iPlayer)
	{
		CvPlayer& kPlayer = GET_PLAYER((PlayerTypes)iPlayer);
		if (kPlayer.getTeam() == getID() && kPlayer.isAlive())
		{
			if (kPlayer.isTurnActive())
			{
				return true;
			}
		}
	}

	return false;
}


void CvTeam::read(FDataStreamBase* pStream)
{
	CvTaggedSaveFormatWrapper&	wrapper = CvTaggedSaveFormatWrapper::getSaveFormatWrapper();

	wrapper.AttachToStream(pStream);

	WRAPPER_READ_OBJECT_START(wrapper);

	// Init data before load
	reset();

	uint uiFlag=0;
	WRAPPER_READ(wrapper, "CvTeam", &uiFlag);	// flags for expansion

	WRAPPER_READ(wrapper, "CvTeam", &m_iNumMembers);
	WRAPPER_READ(wrapper, "CvTeam", &m_iAliveCount);
	WRAPPER_READ(wrapper, "CvTeam", &m_iEverAliveCount);
	WRAPPER_READ(wrapper, "CvTeam", &m_iNumCities);
	WRAPPER_READ(wrapper, "CvTeam", &m_iTotalPopulation);
	WRAPPER_READ(wrapper, "CvTeam", &m_iTotalLand);
	WRAPPER_READ(wrapper, "CvTeam", &m_iNukeInterception);
	WRAPPER_READ(wrapper, "CvTeam", &m_iExtraWaterSeeFromCount);
	WRAPPER_READ(wrapper, "CvTeam", &m_iMapTradingCount);
	WRAPPER_READ(wrapper, "CvTeam", &m_iTechTradingCount);
	WRAPPER_READ(wrapper, "CvTeam", &m_iGoldTradingCount);
	WRAPPER_READ(wrapper, "CvTeam", &m_iOpenBordersTradingCount);
	WRAPPER_READ(wrapper, "CvTeam", &m_iDefensivePactTradingCount);
	WRAPPER_READ(wrapper, "CvTeam", &m_iPermanentAllianceTradingCount);
	WRAPPER_READ(wrapper, "CvTeam", &m_iVassalTradingCount);
	WRAPPER_READ(wrapper, "CvTeam", &m_iBridgeBuildingCount);
	WRAPPER_READ(wrapper, "CvTeam", &m_iIrrigationCount);
	WRAPPER_READ(wrapper, "CvTeam", &m_iIgnoreIrrigationCount);
	WRAPPER_READ(wrapper, "CvTeam", &m_iWaterWorkCount);
	WRAPPER_READ(wrapper, "CvTeam", &m_iVassalPower);
	WRAPPER_READ(wrapper, "CvTeam", &m_iMasterPower);
	WRAPPER_READ(wrapper, "CvTeam", &m_iEnemyWarWearinessModifier);
	WRAPPER_READ(wrapper, "CvTeam", &m_iRiverTradeCount);
	WRAPPER_READ(wrapper, "CvTeam", &m_iEspionagePointsEver);

	WRAPPER_READ(wrapper, "CvTeam", &m_bMapCentering);
	WRAPPER_READ(wrapper, "CvTeam", &m_bCapitulated);

	WRAPPER_READ(wrapper, "CvTeam", (int*)&m_eID);

	WRAPPER_READ_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_aiStolenVisibilityTimer);

	//	Format change - we now store 100 times the actual value, but no need to change the save format - just
	//	convert on load and save
	WRAPPER_READ_ARRAY_DECORATED(wrapper, "CvTeam", MAX_TEAMS, m_aiWarWearinessTimes100, "m_aiWarWeariness");

	for(int iI = 0; iI < MAX_TEAMS; iI++)
	{
		m_aiWarWearinessTimes100[iI] *= 100;
	}

	WRAPPER_READ_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_aiTechShareCount);
	WRAPPER_READ_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_aiEspionagePointsAgainstTeam);
	WRAPPER_READ_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_aiCounterespionageTurnsLeftAgainstTeam);
	WRAPPER_READ_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_aiCounterespionageModAgainstTeam);
	WRAPPER_READ_ARRAY(wrapper, "CvTeam", NUM_COMMERCE_TYPES, m_aiCommerceFlexibleCount);
	WRAPPER_READ_ARRAY(wrapper, "CvTeam", NUM_DOMAIN_TYPES, m_aiExtraMoves);
	WRAPPER_READ_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_VOTE_SOURCES, GC.getNumVoteSourceInfos(), m_aiForceTeamVoteEligibilityCount);
	WRAPPER_READ_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abHasMet);
	// K-mod
	if (uiFlag >= 1)
	{
		WRAPPER_READ_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abHasSeen);
	}		
	else
	{
		memcpy(m_abHasSeen, m_abHasMet, sizeof(*m_abHasSeen)*MAX_TEAMS);
	}		
// K-Mod end

	WRAPPER_READ_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abAtWar);
	//	Koshling - fix bugged state where the ganme thinks a team is at war with itself!
	if ( m_abAtWar[getID()] )
	{
		FAssertMsg(false,"Team flagged as at war with itself (correcting)");
		m_abAtWar[getID()] = false;
	}
	WRAPPER_READ_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abPermanentWarPeace);
	WRAPPER_READ_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abOpenBorders);
	WRAPPER_READ_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abDefensivePact);
	WRAPPER_READ_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abForcePeace);
	WRAPPER_READ_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abVassal);
	WRAPPER_READ_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_VICTORIES, GC.getNumVictoryInfos(), m_abCanLaunch);

/************************************************************************************************/
/* REVOLUTION_MOD                         01/01/08                                jdog5000      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	WRAPPER_READ_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abIsRebelAgainst);
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/
	WRAPPER_READ_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_ROUTES, GC.getNumRouteInfos(), m_paiRouteChange);
	WRAPPER_READ_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_PROJECTS, GC.getNumProjectInfos(), m_paiProjectCount);
	WRAPPER_READ_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_PROJECTS, GC.getNumProjectInfos(), m_paiProjectDefaultArtTypes);
	
	//project art types
	for(int i=0;i<wrapper.getNumClassEnumValues(REMAPPED_CLASS_TYPE_PROJECTS);i++)
	{
		int temp = 0;
		ProjectTypes eNewProjectType = (ProjectTypes)wrapper.getNewClassEnumValue(REMAPPED_CLASS_TYPE_PROJECTS, i, true);
		if ( eNewProjectType != -1 )
		{
			for(int j=0;j<m_paiProjectCount[eNewProjectType];j++)
			{
				//	Decorasted this with a rather obscure name to match the export since when I fixed this
				//	bug I didn't want the export format to chnage uneccessarily
				WRAPPER_READ_DECORATED(wrapper, "CvTeam", &temp, "m_pavProjectArtTypes[][]");
				m_pavProjectArtTypes[eNewProjectType].push_back(temp);
			}
		}
	}

	WRAPPER_READ_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_PROJECTS, GC.getNumProjectInfos(), m_paiProjectMaking);
	WRAPPER_READ_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_UNIT_CLASSES, GC.getNumUnitClassInfos(), m_paiUnitClassCount);
	WRAPPER_READ_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_BUILDING_CLASSES, GC.getNumBuildingClassInfos(), m_paiBuildingClassCount);
	WRAPPER_READ_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_BUILDINGS, GC.getNumBuildingInfos(), m_paiObsoleteBuildingCount);
	WRAPPER_READ_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_TECHS, GC.getNumTechInfos(), m_paiResearchProgress);
	WRAPPER_READ_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_TECHS, GC.getNumTechInfos(), m_paiTechCount);
	WRAPPER_READ_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_TERRAINS, GC.getNumTerrainInfos(), m_paiTerrainTradeCount);
	WRAPPER_READ_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_VICTORIES, GC.getNumVictoryInfos(), m_aiVictoryCountdown);
	WRAPPER_READ_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_TECHS, GC.getNumTechInfos(), m_pabHasTech);
	WRAPPER_READ_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_TECHS, GC.getNumTechInfos(), m_pabNoTradeTech);

	for (int i = 0; i < wrapper.getNumClassEnumValues(REMAPPED_CLASS_TYPE_IMPROVEMENTS); ++i)
	{
		int	newIndex = wrapper.getNewClassEnumValue(REMAPPED_CLASS_TYPE_IMPROVEMENTS, i, true);

		if ( newIndex != -1 )
		{
			WRAPPER_READ_ARRAY(wrapper, "CvTeam", NUM_YIELD_TYPES, m_ppaaiImprovementYieldChange[newIndex]);
		}
		else
		{
			//	Consume the values
			WRAPPER_SKIP_ELEMENT(wrapper, "CvTeam", m_ppaaiImprovementYieldChange[i], SAVE_VALUE_ANY);
		}
	}

	uint iSize = 0;
	m_aeRevealedBonuses.clear();
	WRAPPER_READ(wrapper, "CvTeam", &iSize);
	for (uint i = 0; i < iSize; ++i)
	{
		BonusTypes eBonus = NO_BONUS;
		WRAPPER_READ_CLASS_ENUM(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_BONUSES, (int*)&eBonus);
		m_aeRevealedBonuses.push_back(eBonus);
	}
/************************************************************************************************/
/* Afforess	                  Start		 03/15/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	WRAPPER_READ(wrapper, "CvTeam", &m_iCanPassPeaksCount);
	WRAPPER_READ(wrapper, "CvTeam", &m_iMoveFastPeaksCount);
	WRAPPER_READ(wrapper, "CvTeam", &m_iCanFoundOnPeaksCount);
	WRAPPER_READ(wrapper, "CvTeam", &m_iEmbassyTradingCount);
	
	WRAPPER_READ(wrapper, "CvTeam", &m_iLimitedBordersTradingCount);
	WRAPPER_READ(wrapper, "CvTeam", &m_iCanFarmDesertCount);
	
	WRAPPER_READ(wrapper, "CvTeam", &m_iRebaseAnywhereCount);
// Movement Limits by 45deg - START
	WRAPPER_READ(wrapper, "CvTeam", &m_iExtendMovementLimitsCount);
	WRAPPER_READ(wrapper, "CvTeam", &m_iRemoveMovementLimitsCount);	
// Movement Limits by 45deg - END
	WRAPPER_READ(wrapper, "CvTeam", &m_iForeignTradeModifier);
	WRAPPER_READ(wrapper, "CvTeam", &m_iTradeModifier);
	WRAPPER_READ(wrapper, "CvTeam", &m_iTradeMissionModifier);
	WRAPPER_READ(wrapper, "CvTeam", &m_iCorporationRevenueModifier);
	WRAPPER_READ(wrapper, "CvTeam", &m_iCorporationMaintenanceModifier);

    WRAPPER_READ_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abEmbassy);
	WRAPPER_READ_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abLimitedBorders);
	WRAPPER_READ_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abFreeTrade);
	WRAPPER_READ_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abHasEverMet); //Rhye, adapted by 45deg
	WRAPPER_READ_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_aiFailedContactChecks);

	WRAPPER_READ_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_BUILDINGS, GC.getNumBuildingInfos(), m_paiTechExtraBuildingHappiness);
	WRAPPER_READ_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_BUILDINGS, GC.getNumBuildingInfos(), m_paiTechExtraBuildingHealth);
	WRAPPER_READ_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_SPECIALISTS, GC.getNumSpecialistInfos(), m_paiFreeSpecialistCount);
	WRAPPER_READ_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_UNIT_CLASSES, GC.getNumUnitClassInfos(), m_paiUnitClassStrengthChange);
	for (int i = 0; i < wrapper.getNumClassEnumValues(REMAPPED_CLASS_TYPE_BUILDINGS); ++i)
	{
		int	newIndex = wrapper.getNewClassEnumValue(REMAPPED_CLASS_TYPE_BUILDINGS, i, true);

		if ( newIndex != -1 )
		{
			WRAPPER_READ_ARRAY(wrapper, "CvTeam", NUM_COMMERCE_TYPES, m_ppiBuildingCommerceChange[newIndex]);
			WRAPPER_READ_ARRAY(wrapper, "CvTeam", NUM_YIELD_TYPES, m_ppiBuildingYieldChange[newIndex]);
			WRAPPER_READ_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_SPECIALISTS, GC.getNumSpecialistInfos(), m_ppiBuildingSpecialistChange[newIndex]);
			WRAPPER_READ_ARRAY(wrapper, "CvTeam", NUM_COMMERCE_TYPES, m_ppiBuildingCommerceModifier[newIndex]);
			WRAPPER_READ_ARRAY(wrapper, "CvTeam", NUM_YIELD_TYPES, m_ppiBuildingYieldModifier[newIndex]);
		}
		else
		{
			//	Consume the values
			WRAPPER_SKIP_ELEMENT(wrapper, "CvTeam", m_ppiBuildingCommerceChange[newIndex], SAVE_VALUE_TYPE_INT_ARRAY);
			WRAPPER_SKIP_ELEMENT(wrapper, "CvTeam", m_ppiBuildingYieldChange[newIndex], SAVE_VALUE_TYPE_INT_ARRAY);
			WRAPPER_SKIP_ELEMENT(wrapper, "CvTeam", m_ppiBuildingSpecialistChange[newIndex], SAVE_VALUE_TYPE_CLASS_INT_ARRAY);
			WRAPPER_SKIP_ELEMENT(wrapper, "CvTeam", m_ppiBuildingCommerceModifier[newIndex], SAVE_VALUE_TYPE_INT_ARRAY);
			WRAPPER_SKIP_ELEMENT(wrapper, "CvTeam", m_ppiBuildingYieldModifier[newIndex], SAVE_VALUE_TYPE_INT_ARRAY);
		}
	}
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/

	m_Properties.readWrapper(pStream);

	WRAPPER_READ_OBJECT_END(wrapper);
}


void CvTeam::write(FDataStreamBase* pStream)
{
	int iI;

	CvTaggedSaveFormatWrapper&	wrapper = CvTaggedSaveFormatWrapper::getSaveFormatWrapper();

	wrapper.AttachToStream(pStream);

	WRAPPER_WRITE_OBJECT_START(wrapper);

	uint uiFlag = 1;
	WRAPPER_WRITE(wrapper, "CvTeam", uiFlag);		// flag for expansion

	WRAPPER_WRITE(wrapper, "CvTeam", m_iNumMembers);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iAliveCount);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iEverAliveCount);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iNumCities);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iTotalPopulation);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iTotalLand);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iNukeInterception);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iExtraWaterSeeFromCount);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iMapTradingCount);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iTechTradingCount);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iGoldTradingCount);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iOpenBordersTradingCount);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iDefensivePactTradingCount);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iPermanentAllianceTradingCount);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iVassalTradingCount);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iBridgeBuildingCount);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iIrrigationCount);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iIgnoreIrrigationCount);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iWaterWorkCount);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iVassalPower);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iMasterPower);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iEnemyWarWearinessModifier);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iRiverTradeCount);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iEspionagePointsEver);
	WRAPPER_WRITE(wrapper, "CvTeam", m_bMapCentering);
	WRAPPER_WRITE(wrapper, "CvTeam", m_bCapitulated);
	WRAPPER_WRITE(wrapper, "CvTeam", m_eID);
	WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_aiStolenVisibilityTimer);

	//	Format change - we now store 100 times the actual value, but no need to change the save format - just
	//	convert on load and save
	for(iI = 0; iI < MAX_TEAMS; iI++)
	{
		m_aiWarWearinessTimes100[iI] /= 100;
	}

	WRAPPER_WRITE_ARRAY_DECORATED(wrapper, "CvTeam", MAX_TEAMS, m_aiWarWearinessTimes100, "m_aiWarWeariness");
	for(iI = 0; iI < MAX_TEAMS; iI++)
	{
		m_aiWarWearinessTimes100[iI] *= 100;
	}

	WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_aiTechShareCount);
	WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_aiEspionagePointsAgainstTeam);
	WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_aiCounterespionageTurnsLeftAgainstTeam);
	WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_aiCounterespionageModAgainstTeam);
	WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", NUM_COMMERCE_TYPES, m_aiCommerceFlexibleCount);
	WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", NUM_DOMAIN_TYPES, m_aiExtraMoves);
	WRAPPER_WRITE_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_VOTE_SOURCES, GC.getNumVoteSourceInfos(), m_aiForceTeamVoteEligibilityCount);
	WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abHasMet);
	WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abHasSeen); // K-Mod. uiFlag >= 1
	WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abAtWar);
	WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abPermanentWarPeace);
	WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abOpenBorders);
	WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abDefensivePact);
	WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abForcePeace);
	WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abVassal);
	WRAPPER_WRITE_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_VICTORIES, GC.getNumVictoryInfos(), m_abCanLaunch);

/************************************************************************************************/
/* REVOLUTION_MOD                         01/01/08                                jdog5000      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abIsRebelAgainst);
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/
	WRAPPER_WRITE_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_ROUTES, GC.getNumRouteInfos(), m_paiRouteChange);
	WRAPPER_WRITE_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_PROJECTS, GC.getNumProjectInfos(), m_paiProjectCount);
	WRAPPER_WRITE_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_PROJECTS, GC.getNumProjectInfos(), m_paiProjectDefaultArtTypes);

	//project art types
	for(int i=0;i<GC.getNumProjectInfos();i++)
	{
		for(int j=0;j<m_paiProjectCount[i];j++)
			WRAPPER_WRITE(wrapper, "CvTeam", m_pavProjectArtTypes[i][j]);
	}

	WRAPPER_WRITE_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_PROJECTS, GC.getNumProjectInfos(), m_paiProjectMaking);
	WRAPPER_WRITE_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_UNIT_CLASSES, GC.getNumUnitClassInfos(), m_paiUnitClassCount);
	WRAPPER_WRITE_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_BUILDING_CLASSES, GC.getNumBuildingClassInfos(), m_paiBuildingClassCount);
	WRAPPER_WRITE_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_BUILDINGS, GC.getNumBuildingInfos(), m_paiObsoleteBuildingCount);
	WRAPPER_WRITE_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_TECHS, GC.getNumTechInfos(), m_paiResearchProgress);
	WRAPPER_WRITE_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_TECHS, GC.getNumTechInfos(), m_paiTechCount);
	WRAPPER_WRITE_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_TERRAINS, GC.getNumTerrainInfos(), m_paiTerrainTradeCount);
	WRAPPER_WRITE_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_VICTORIES, GC.getNumVictoryInfos(), m_aiVictoryCountdown);


	WRAPPER_WRITE_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_TECHS, GC.getNumTechInfos(), m_pabHasTech);
	WRAPPER_WRITE_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_TECHS, GC.getNumTechInfos(), m_pabNoTradeTech);

	for (iI=0;iI<GC.getNumImprovementInfos();iI++)
	{
		WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", NUM_YIELD_TYPES, m_ppaaiImprovementYieldChange[iI]);
	}

	WRAPPER_WRITE_DECORATED(wrapper, "CvTeam", m_aeRevealedBonuses.size(), "iSize" );
	for (std::vector<BonusTypes>::iterator it = m_aeRevealedBonuses.begin(); it != m_aeRevealedBonuses.end(); ++it)
	{
		WRAPPER_WRITE_CLASS_ENUM_DECORATED(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_BONUSES, *it, "eBonus");
	}
/************************************************************************************************/
/* Afforess	                  Start		 03/15/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	WRAPPER_WRITE(wrapper, "CvTeam", m_iCanPassPeaksCount);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iMoveFastPeaksCount);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iCanFoundOnPeaksCount);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iEmbassyTradingCount);
	
	WRAPPER_WRITE(wrapper, "CvTeam", m_iLimitedBordersTradingCount);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iCanFarmDesertCount);
	
	WRAPPER_WRITE(wrapper, "CvTeam", m_iRebaseAnywhereCount);
// Movement Limits by 45deg - START
	WRAPPER_WRITE(wrapper, "CvTeam", m_iExtendMovementLimitsCount);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iRemoveMovementLimitsCount);	
// Movement Limits by 45deg - END
	WRAPPER_WRITE(wrapper, "CvTeam", m_iForeignTradeModifier);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iTradeModifier);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iTradeMissionModifier);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iCorporationRevenueModifier);
	WRAPPER_WRITE(wrapper, "CvTeam", m_iCorporationMaintenanceModifier);
	
    WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abEmbassy);
	WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abLimitedBorders);
	WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abFreeTrade);
	WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_abHasEverMet); //Rhye, adapted by 45deg
	WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", MAX_TEAMS, m_aiFailedContactChecks);

	WRAPPER_WRITE_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_BUILDINGS, GC.getNumBuildingInfos(), m_paiTechExtraBuildingHappiness);
	WRAPPER_WRITE_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_BUILDINGS, GC.getNumBuildingInfos(), m_paiTechExtraBuildingHealth);	
	WRAPPER_WRITE_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_SPECIALISTS, GC.getNumSpecialistInfos(), m_paiFreeSpecialistCount);
	WRAPPER_WRITE_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_UNIT_CLASSES, GC.getNumUnitClassInfos(), m_paiUnitClassStrengthChange);
	for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
	{
		WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", NUM_COMMERCE_TYPES, m_ppiBuildingCommerceChange[iI]);
		WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", NUM_YIELD_TYPES, m_ppiBuildingYieldChange[iI]);
		WRAPPER_WRITE_CLASS_ARRAY(wrapper, "CvTeam", REMAPPED_CLASS_TYPE_SPECIALISTS, GC.getNumSpecialistInfos(), m_ppiBuildingSpecialistChange[iI]);
		WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", NUM_COMMERCE_TYPES, m_ppiBuildingCommerceModifier[iI]);
		WRAPPER_WRITE_ARRAY(wrapper, "CvTeam", NUM_YIELD_TYPES, m_ppiBuildingYieldModifier[iI]);
	}
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/

	m_Properties.writeWrapper(pStream);

	WRAPPER_WRITE_OBJECT_END(wrapper);
}


void CvTeam::resync(bool bWrite, ByteBuffer* pBuffer)
{
	RESYNC_INT(bWrite, pBuffer, m_iNumMembers);
	RESYNC_INT(bWrite, pBuffer, m_iAliveCount);
	RESYNC_INT(bWrite, pBuffer, m_iEverAliveCount);
	RESYNC_INT(bWrite, pBuffer, m_iNumCities);
	RESYNC_INT(bWrite, pBuffer, m_iTotalPopulation);
	RESYNC_INT(bWrite, pBuffer, m_iTotalLand);
	RESYNC_INT(bWrite, pBuffer, m_iNukeInterception);
	RESYNC_INT(bWrite, pBuffer, m_iExtraWaterSeeFromCount);
	RESYNC_INT(bWrite, pBuffer, m_iMapTradingCount);
	RESYNC_INT(bWrite, pBuffer, m_iTechTradingCount);
	RESYNC_INT(bWrite, pBuffer, m_iGoldTradingCount);
	RESYNC_INT(bWrite, pBuffer, m_iOpenBordersTradingCount);
	RESYNC_INT(bWrite, pBuffer, m_iDefensivePactTradingCount);
	RESYNC_INT(bWrite, pBuffer, m_iPermanentAllianceTradingCount);
	RESYNC_INT(bWrite, pBuffer, m_iVassalTradingCount);
	RESYNC_INT(bWrite, pBuffer, m_iBridgeBuildingCount);
	RESYNC_INT(bWrite, pBuffer, m_iIrrigationCount);
	RESYNC_INT(bWrite, pBuffer, m_iIgnoreIrrigationCount);
	RESYNC_INT(bWrite, pBuffer, m_iWaterWorkCount);
	RESYNC_INT(bWrite, pBuffer, m_iVassalPower);
	RESYNC_INT(bWrite, pBuffer, m_iMasterPower);
	RESYNC_INT(bWrite, pBuffer, m_iEnemyWarWearinessModifier);
	RESYNC_INT(bWrite, pBuffer, m_iRiverTradeCount);
	RESYNC_INT(bWrite, pBuffer, m_iEspionagePointsEver);
	RESYNC_BOOL(bWrite, pBuffer, m_bMapCentering);
	RESYNC_BOOL(bWrite, pBuffer, m_bCapitulated);
	RESYNC_INT_WITH_CAST(bWrite, pBuffer, m_eID, TeamTypes);
	RESYNC_INT_ARRAY(bWrite, pBuffer, MAX_TEAMS, m_aiStolenVisibilityTimer);
	RESYNC_INT_ARRAY(bWrite, pBuffer, MAX_TEAMS, m_aiWarWearinessTimes100);
	RESYNC_INT_ARRAY(bWrite, pBuffer, MAX_TEAMS, m_aiTechShareCount);
	RESYNC_INT_ARRAY(bWrite, pBuffer, MAX_TEAMS, m_aiEspionagePointsAgainstTeam);
	RESYNC_INT_ARRAY(bWrite, pBuffer, MAX_TEAMS, m_aiCounterespionageTurnsLeftAgainstTeam);
	RESYNC_INT_ARRAY(bWrite, pBuffer, MAX_TEAMS, m_aiCounterespionageModAgainstTeam);
	RESYNC_INT_ARRAY(bWrite, pBuffer, NUM_COMMERCE_TYPES, m_aiCommerceFlexibleCount);
	RESYNC_INT_ARRAY(bWrite, pBuffer, NUM_DOMAIN_TYPES, m_aiExtraMoves);
	RESYNC_INT_ARRAY(bWrite, pBuffer, GC.getNumVoteSourceInfos(), m_aiForceTeamVoteEligibilityCount);
	RESYNC_BOOL_ARRAY(bWrite, pBuffer, MAX_TEAMS, m_abHasMet);
	RESYNC_BOOL_ARRAY(bWrite, pBuffer, MAX_TEAMS, m_abHasSeen);
	RESYNC_BOOL_ARRAY(bWrite, pBuffer, MAX_TEAMS, m_abAtWar);
	RESYNC_BOOL_ARRAY(bWrite, pBuffer, MAX_TEAMS, m_abPermanentWarPeace);
	RESYNC_BOOL_ARRAY(bWrite, pBuffer, MAX_TEAMS, m_abOpenBorders);
	RESYNC_BOOL_ARRAY(bWrite, pBuffer, MAX_TEAMS, m_abDefensivePact);
	RESYNC_BOOL_ARRAY(bWrite, pBuffer, MAX_TEAMS, m_abForcePeace);
	RESYNC_BOOL_ARRAY(bWrite, pBuffer, MAX_TEAMS, m_abVassal);
	RESYNC_BOOL_ARRAY(bWrite, pBuffer, GC.getNumVictoryInfos(), m_abCanLaunch);
	RESYNC_BOOL_ARRAY(bWrite, pBuffer, MAX_TEAMS, m_abIsRebelAgainst);

	RESYNC_INT_ARRAY(bWrite, pBuffer, GC.getNumRouteInfos(), m_paiRouteChange);
	RESYNC_INT_ARRAY(bWrite, pBuffer, GC.getNumProjectInfos(), m_paiProjectCount);
	RESYNC_INT_ARRAY(bWrite, pBuffer, GC.getNumProjectInfos(), m_paiProjectDefaultArtTypes);

	//project art types
	for (int i = 0; i<GC.getNumProjectInfos(); i++)
	{
		RESYNC_INT_ARRAY(bWrite, pBuffer, m_paiProjectCount[i], m_pavProjectArtTypes[i]);
	}

	RESYNC_INT_ARRAY(bWrite, pBuffer, GC.getNumProjectInfos(), m_paiProjectMaking);
	RESYNC_INT_ARRAY(bWrite, pBuffer, GC.getNumUnitClassInfos(), m_paiUnitClassCount);
	RESYNC_INT_ARRAY(bWrite, pBuffer, GC.getNumBuildingClassInfos(), m_paiBuildingClassCount);
	RESYNC_INT_ARRAY(bWrite, pBuffer, GC.getNumBuildingInfos(), m_paiObsoleteBuildingCount);
	RESYNC_INT_ARRAY(bWrite, pBuffer, GC.getNumTechInfos(), m_paiResearchProgress);
	RESYNC_INT_ARRAY(bWrite, pBuffer, GC.getNumTechInfos(), m_paiTechCount);
	RESYNC_INT_ARRAY(bWrite, pBuffer, GC.getNumTerrainInfos(), m_paiTerrainTradeCount);
	RESYNC_INT_ARRAY(bWrite, pBuffer, GC.getNumVictoryInfos(), m_aiVictoryCountdown);
	RESYNC_BOOL_ARRAY(bWrite, pBuffer, GC.getNumTechInfos(), m_pabHasTech);
	RESYNC_BOOL_ARRAY(bWrite, pBuffer, GC.getNumTechInfos(), m_pabNoTradeTech);

	for (int iI = 0; iI<GC.getNumImprovementInfos(); iI++)
	{
		RESYNC_INT_ARRAY(bWrite, pBuffer, NUM_YIELD_TYPES, m_ppaaiImprovementYieldChange[iI]);
	}

	if (bWrite)
	{
		pBuffer->putInt(m_aeRevealedBonuses.size());
		for (std::vector<BonusTypes>::iterator it = m_aeRevealedBonuses.begin(); it != m_aeRevealedBonuses.end(); ++it)
		{
			pBuffer->putInt(*it);
		}
	}
	else
	{
		m_aeRevealedBonuses.clear();
		int iCount = pBuffer->getInt();
		for (int i = 0; i < iCount; i++)
		{
			m_aeRevealedBonuses.push_back((BonusTypes)pBuffer->getInt());
		}
	}

	RESYNC_INT(bWrite, pBuffer, m_iCanPassPeaksCount);
	RESYNC_INT(bWrite, pBuffer, m_iMoveFastPeaksCount);
	RESYNC_INT(bWrite, pBuffer, m_iCanFoundOnPeaksCount);
	RESYNC_INT(bWrite, pBuffer, m_iEmbassyTradingCount);
	RESYNC_INT(bWrite, pBuffer, m_iLimitedBordersTradingCount);
	RESYNC_INT(bWrite, pBuffer, m_iCanFarmDesertCount);
	RESYNC_INT(bWrite, pBuffer, m_iRebaseAnywhereCount);
// Movement Limits by 45deg - START
	RESYNC_INT(bWrite, pBuffer, m_iExtendMovementLimitsCount);
	RESYNC_INT(bWrite, pBuffer, m_iRemoveMovementLimitsCount);	
// Movement Limits by 45deg - END
	RESYNC_INT(bWrite, pBuffer, m_iForeignTradeModifier);
	RESYNC_INT(bWrite, pBuffer, m_iTradeModifier);
	RESYNC_INT(bWrite, pBuffer, m_iTradeMissionModifier);
	RESYNC_INT(bWrite, pBuffer, m_iCorporationRevenueModifier);
	RESYNC_INT(bWrite, pBuffer, m_iCorporationMaintenanceModifier);

	RESYNC_BOOL_ARRAY(bWrite, pBuffer, MAX_TEAMS, m_abEmbassy);
	RESYNC_BOOL_ARRAY(bWrite, pBuffer, MAX_TEAMS, m_abLimitedBorders);
	RESYNC_BOOL_ARRAY(bWrite, pBuffer, MAX_TEAMS, m_abFreeTrade);
	RESYNC_INT_ARRAY(bWrite, pBuffer, GC.getNumBuildingInfos(), m_paiTechExtraBuildingHappiness);
	RESYNC_INT_ARRAY(bWrite, pBuffer, GC.getNumBuildingInfos(), m_paiTechExtraBuildingHealth);
	RESYNC_INT_ARRAY(bWrite, pBuffer, GC.getNumSpecialistInfos(), m_paiFreeSpecialistCount);
	RESYNC_INT_ARRAY(bWrite, pBuffer, GC.getNumUnitClassInfos(), m_paiUnitClassStrengthChange);
	for (int iI = 0; iI < GC.getNumBuildingInfos(); iI++)
	{
		RESYNC_INT_ARRAY(bWrite, pBuffer, NUM_COMMERCE_TYPES, m_ppiBuildingCommerceChange[iI]);
		RESYNC_INT_ARRAY(bWrite, pBuffer, NUM_YIELD_TYPES, m_ppiBuildingYieldChange[iI]);
		RESYNC_INT_ARRAY(bWrite, pBuffer, GC.getNumSpecialistInfos(), m_ppiBuildingSpecialistChange[iI]);
		RESYNC_INT_ARRAY(bWrite, pBuffer, NUM_COMMERCE_TYPES, m_ppiBuildingCommerceModifier[iI]);
		RESYNC_INT_ARRAY(bWrite, pBuffer, NUM_YIELD_TYPES, m_ppiBuildingYieldModifier[iI]);
	}

}


// CACHE: cache frequently used values
///////////////////////////////////////
bool CvTeam::hasShrine(ReligionTypes eReligion)
{
	bool bHasShrine = false;
	
	if (eReligion != NO_RELIGION)
	{
		CvCity* pHolyCity = GC.getGameINLINE().getHolyCity(eReligion);
		
		// if the holy city exists, and we own it
		if (pHolyCity != NULL && GET_PLAYER(pHolyCity->getOwnerINLINE()).getTeam() == getID())
			bHasShrine = pHolyCity->hasShrine(eReligion);
	}

	return bHasShrine;
}

void CvTeam::getCompletedSpaceshipProjects(std::map<ProjectTypes, int>& mapProjects) const
{
	for (int i = 0; i < GC.getNumProjectInfos(); i++)
	{
		ProjectTypes eProject = (ProjectTypes) i;
		if (GC.getProjectInfo(eProject).isSpaceship())
		{
			mapProjects[eProject] = getProjectCount(eProject);
		}
	}
}

int CvTeam::getProjectPartNumber(ProjectTypes eProject, bool bAssert) const
{
	int iNumBuilt = getProjectCount(eProject);
	for (int i = 0; i < iNumBuilt; i++)
	{
		int artType = getProjectArtType(eProject, i);
		if (artType < 0)
		{
			return i;
		}
	}

	//didn't find empty part number
	if (bAssert)
	{
		FAssertMsg(false, "Unknown part number.");
	}

	//return the last one
	return std::min(iNumBuilt, GC.getProjectInfo(eProject).getMaxTeamInstances() - 1);
}

bool CvTeam::hasLaunched() const
{
	VictoryTypes spaceVictory = GC.getGameINLINE().getSpaceVictory();
	if (spaceVictory != NO_VICTORY)
	{
		return (getVictoryCountdown(spaceVictory) >= 0);
	}
	return false;
}

/************************************************************************************************/
/* Afforess	                  Start		 03/8/10                                                */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/

bool CvTeam::isCanPassPeaks() const											
{
	return (getCanPassPeaksCount() > 0);
}

int CvTeam::getCanPassPeaksCount() const
{
	return m_iCanPassPeaksCount;
}

void CvTeam::changeCanPassPeaksCount(int iChange)						 
{
	m_iCanPassPeaksCount = (m_iCanPassPeaksCount + iChange);
	FAssert(getCanPassPeaksCount() >= 0);
}

bool CvTeam::isMoveFastPeaks() const											
{
	return (getMoveFastPeaksCount() > 0);
}

int CvTeam::getMoveFastPeaksCount() const
{
	return m_iMoveFastPeaksCount;
}

void CvTeam::changeMoveFastPeaksCount(int iChange)						 
{
	m_iMoveFastPeaksCount = (m_iMoveFastPeaksCount + iChange);
	FAssert(getMoveFastPeaksCount() >= 0);
}

bool CvTeam::isCanFoundOnPeaks() const											
{
	return (getCanFoundOnPeaksCount() > 0);
}

int CvTeam::getCanFoundOnPeaksCount() const
{
	return m_iCanFoundOnPeaksCount;
}

void CvTeam::changeCanFoundOnPeaksCount(int iChange)						 
{
	m_iCanFoundOnPeaksCount = (m_iCanFoundOnPeaksCount + iChange);
	FAssert(getCanFoundOnPeaksCount() >= 0);
}

int CvTeam::getRebaseAnywhereCount() const
{
	return m_iRebaseAnywhereCount;
}

bool CvTeam::isRebaseAnywhere() const
{
	return m_iRebaseAnywhereCount > 0;
}

void CvTeam::changeRebaseAnywhereCount(int iChange)
{
	m_iRebaseAnywhereCount += iChange;
}
// Movement Limits by 45deg - START
bool CvTeam::isExtendMovementLimits() const											
{
	return (getExtendMovementLimitsCount() > 0);
}

int CvTeam::getExtendMovementLimitsCount() const
{
	return m_iExtendMovementLimitsCount;
}

void CvTeam::changeExtendMovementLimitsCount(int iChange)						 
{
	m_iExtendMovementLimitsCount = (m_iExtendMovementLimitsCount + iChange);
	FAssert(getExtendMovementLimitsCount() >= 0);
}

bool CvTeam::isRemoveMovementLimits() const											
{
	return (getRemoveMovementLimitsCount() > 0);
}

int CvTeam::getRemoveMovementLimitsCount() const
{
	return m_iRemoveMovementLimitsCount;
}

void CvTeam::changeRemoveMovementLimitsCount(int iChange)						 
{
	m_iRemoveMovementLimitsCount = (m_iRemoveMovementLimitsCount + iChange);
	FAssert(getRemoveMovementLimitsCount() >= 0);
}
// Movement Limits by 45deg - END
/* Returns the happiness added to a single building type by all acquired techs.
 *
 * eBuilding - the building type to look up
 */
int CvTeam::getTechExtraBuildingHappiness(BuildingTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBuildingInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiTechExtraBuildingHappiness[eIndex];
}

/* Adds iChange to the current happiness added to a single building type by all acquired techs.
 * Called from processTech() when a tech is acquired or lost.
 *
 * eIndex - the building type to change
 * iChange - the additional happiness to add to the existing value
 */
void CvTeam::changeTechExtraBuildingHappiness(BuildingTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBuildingInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		setTechExtraBuildingHappiness(eIndex, getTechExtraBuildingHappiness(eIndex) + iChange);
	}
}

int CvTeam::getTechExtraBuildingHealth(BuildingTypes eIndex) const 
{

	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBuildingInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiTechExtraBuildingHealth[eIndex];
}


void CvTeam::changeTechExtraBuildingHealth(BuildingTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBuildingInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		setTechExtraBuildingHealth(eIndex, getTechExtraBuildingHealth(eIndex) + iChange);
	}
}

void CvTeam::AI_setAssignWorkDirtyInEveryPlayerCityWithActiveBuilding(BuildingTypes eBuilding)
{
	int iI;
	int iLoopCity;
	CvCity* pLoopCity;
	
	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iI);
		if (kLoopPlayer.isAlive() && kLoopPlayer.getTeam() == getID())
		{
			for (pLoopCity = kLoopPlayer.firstCity(&iLoopCity); pLoopCity != NULL; pLoopCity = kLoopPlayer.nextCity(&iLoopCity))
			{
				if (pLoopCity->getNumActiveBuilding(eBuilding) > 0)
				{
					pLoopCity->AI_setAssignWorkDirty(true);
				}
			}
		}
	}
}
/* Sets the happiness added to a single building type by all acquired techs.
 *
 * eIndex - the building type to change
 * iNewValue - the new happiness value for the building
 */
void CvTeam::setTechExtraBuildingHappiness(BuildingTypes eIndex, int iNewValue)
{
	if (m_paiTechExtraBuildingHappiness[eIndex] != iNewValue)
	{
		int iOldValue = m_paiTechExtraBuildingHappiness[eIndex];
		m_paiTechExtraBuildingHappiness[eIndex] = iNewValue;
		
		int iI;
		int iLoopCity;
		CvCity* pLoopCity;
		
		for (iI = 0; iI < MAX_PLAYERS; iI++)
		{
			CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iI);
			if (kLoopPlayer.isAlive() && kLoopPlayer.getTeam() == getID())
			{
				for (pLoopCity = kLoopPlayer.firstCity(&iLoopCity); pLoopCity != NULL; pLoopCity = kLoopPlayer.nextCity(&iLoopCity))
				{
					int iNumBuildings = pLoopCity->getNumActiveBuilding(eIndex);
					if (iNumBuildings > 0)
					{
						// Remove the old value
						if (iOldValue > 0)
						{
							pLoopCity->changeBuildingGoodHappiness(-iOldValue * iNumBuildings);
						}
						else if (iOldValue < 0)
						{
							pLoopCity->changeBuildingBadHappiness(-iOldValue * iNumBuildings);
						}
						
						// Add the new value
						if (iNewValue > 0)
						{
							pLoopCity->changeBuildingGoodHappiness(iNewValue * iNumBuildings);
						}
						else if (iNewValue < 0)
						{
							pLoopCity->changeBuildingBadHappiness(iNewValue * iNumBuildings);
						}
					}
				}
			}
		}
	}
}

void CvTeam::setTechExtraBuildingHealth(BuildingTypes eIndex, int iNewValue)
{
	if (m_paiTechExtraBuildingHealth[eIndex] != iNewValue)
	{
		int iOldValue = m_paiTechExtraBuildingHealth[eIndex];
		m_paiTechExtraBuildingHealth[eIndex] = iNewValue;
		
		int iI;
		int iLoopCity;
		CvCity* pLoopCity;
		
		for (iI = 0; iI < MAX_PLAYERS; iI++)
		{
			CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iI);
			if (kLoopPlayer.isAlive() && kLoopPlayer.getTeam() == getID())
			{
				for (pLoopCity = kLoopPlayer.firstCity(&iLoopCity); pLoopCity != NULL; pLoopCity = kLoopPlayer.nextCity(&iLoopCity))
				{
					int iNumBuildings = pLoopCity->getNumActiveBuilding(eIndex);
					if (iNumBuildings > 0)
					{
						// Remove the old value
						if (iOldValue > 0)
						{
							pLoopCity->changeBuildingGoodHealth(-iOldValue * iNumBuildings);
						}
						else if (iOldValue < 0)
						{
							pLoopCity->changeBuildingBadHealth(-iOldValue * iNumBuildings);
						}
						
						// Add the new value
						if (iNewValue > 0)
						{
							pLoopCity->changeBuildingGoodHealth(iNewValue * iNumBuildings);
						}
						else if (iNewValue < 0)
						{
							pLoopCity->changeBuildingBadHealth(iNewValue * iNumBuildings);
						}
					}
				}
			}
		}
	}
}



int CvTeam::getCanFarmDesertCount() const
{
	return m_iCanFarmDesertCount;
}

bool CvTeam::isCanFarmDesert() const
{
	return (getCanFarmDesertCount() > 0);
}

void CvTeam::changeCanFarmDesertCount(int iChange)
{
	m_iCanFarmDesertCount = (m_iCanFarmDesertCount + iChange);
	FAssert(getCanFarmDesertCount() >= 0);
}
/************************************************************************************************/
/* Afforess	                  Start		 06/16/10                                               */
/*                                                                                              */
/* Advanced Diplomacy                                                                           */
/************************************************************************************************/
int CvTeam::getLimitedBordersTradingCount() const
{
	return m_iLimitedBordersTradingCount;
}

bool CvTeam::isLimitedBordersTrading() const
{
	return (getLimitedBordersTradingCount() > 0);
}

void CvTeam::changeLimitedBordersTradingCount(int iChange)
{
	m_iLimitedBordersTradingCount = (m_iLimitedBordersTradingCount + iChange);
	FAssert(getLimitedBordersTradingCount() >= 0);
}

void CvTeam::signLimitedBorders(TeamTypes eTeam)
{
	CLinkList<TradeData> ourList;
	CLinkList<TradeData> theirList;
	TradeData item;

	FAssert(eTeam != NO_TEAM);
	FAssert(eTeam != getID());

	if (!isAtWar(eTeam) && (getID() != eTeam) && (!isOpenBorders(eTeam)))
	{
		setTradeItem(&item, TRADE_RITE_OF_PASSAGE);

		if (GET_PLAYER(getLeaderID()).canTradeItem(GET_TEAM(eTeam).getLeaderID(), item) && GET_PLAYER(GET_TEAM(eTeam).getLeaderID()).canTradeItem(getLeaderID(), item))
		{
			ourList.clear();
			theirList.clear();

			ourList.insertAtEnd(item);
			theirList.insertAtEnd(item);

			GC.getGameINLINE().implementDeal(getLeaderID(), (GET_TEAM(eTeam).getLeaderID()), &ourList, &theirList);
		}
	}
}

bool CvTeam::canSignOpenBorders(TeamTypes eTeam)
{
	if (GC.getGameINLINE().isOption(GAMEOPTION_ADVANCED_DIPLOMACY))
	{
	    if (!isHasEmbassy(eTeam))
	    {
	        return false;
		}
    }
    return true;
}

void CvTeam::sendAmbassador(TeamTypes eTeam)
{
    CLinkList<TradeData> ourList;
    CLinkList<TradeData> theirList;
    TradeData item;

    FAssert(eTeam != NO_TEAM);
    FAssert(eTeam != getID());

    if (!isAtWar(eTeam) && (getID() != eTeam))
    {
        setTradeItem(&item, TRADE_EMBASSY);

        if (GET_PLAYER(getLeaderID()).canTradeItem(GET_TEAM(eTeam).getLeaderID(), item) && GET_PLAYER(GET_TEAM(eTeam).getLeaderID()).canTradeItem(getLeaderID(), item))
        {
            ourList.clear();
            theirList.clear();

            ourList.insertAtEnd(item);
            theirList.insertAtEnd(item);

            GC.getGameINLINE().implementDeal(getLeaderID(), (GET_TEAM(eTeam).getLeaderID()), &ourList, &theirList);
        }
    }
}


bool CvTeam::isLimitedBorders(TeamTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_abLimitedBorders[eIndex];
}



bool CvTeam::isHasEmbassy(TeamTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");
    return m_abEmbassy[eIndex];
}

void CvTeam::setHasEmbassy(TeamTypes eIndex, bool bNewValue)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");
    if (isHasEmbassy(eIndex) != bNewValue)
    {
	//Removed due to bugs
	//Fixed by damgo and reinstated by ls612
		CvCity* pCapital;
		for (int iI = 0; iI < MAX_PLAYERS; ++iI)
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				pCapital = GET_PLAYER((PlayerTypes)iI).getCapitalCity();
				if (pCapital != NULL)
				{
					pCapital->plot()->updateSight(false, true);
				}
			}
		}
		m_abEmbassy[eIndex] = bNewValue;
		for (int iI = 0; iI < MAX_PLAYERS; ++iI)
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				pCapital = GET_PLAYER((PlayerTypes)iI).getCapitalCity();
				if (pCapital != NULL)
				{
					pCapital->plot()->updateSight(true, true);
				}
			}
		}
		//m_abEmbassy[eIndex] = bNewValue;
		//End Embassy Visibility Fix
    }
}

int CvTeam::getEmbassyTradingCount() const
{
    return m_iEmbassyTradingCount;
}

bool CvTeam::isEmbassyTrading() const
{
    return (getEmbassyTradingCount() > 0);
}

void CvTeam::changeEmbassyTradingCount(int iChange)
{
    if (iChange != 0)
    {
        m_iEmbassyTradingCount = (m_iEmbassyTradingCount + iChange);
    }
}

/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/

void CvTeam::ObsoletePromotions(TechTypes eObsoleteTech)
{
	int iI;
	bool bValid = false;
	int iLoop = 0;
	
	if (eObsoleteTech != NO_TECH)
	{
		for (iI = 0; iI < GC.getNumPromotionInfos(); iI++)
		{
			if ((GC.getPromotionInfo((PromotionTypes)iI).getObsoleteTech()) == eObsoleteTech)
			{
				bValid = true;
				break;
			}
		}
		if (bValid)
		{
			for (int iPlayer = 0; iPlayer < MAX_PLAYERS; ++iPlayer)
			{
				CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iPlayer);

				if (kLoopPlayer.isAlive() && GET_TEAM(kLoopPlayer.getTeam()).getID() == getID())
				{
					for (CvUnit* pLoopUnit = kLoopPlayer.firstUnit(&iLoop); NULL != pLoopUnit; pLoopUnit = kLoopPlayer.nextUnit(&iLoop))
					{
						for (iI = 0; iI < GC.getNumPromotionInfos(); iI++)
						{
							if (pLoopUnit->isHasPromotion((PromotionTypes)iI))
							{
								if ((GC.getPromotionInfo((PromotionTypes)iI).getObsoleteTech()) == eObsoleteTech)
								{
									pLoopUnit->setHasPromotion((PromotionTypes)iI, false);
								}
							}
						}
					}
				}
			}
		}
	}
}

void CvTeam::ObsoleteCorporations(TechTypes eObsoleteTech)
{
	int iI;
	bool bValid = false;
	int iLoop = 0;
	if (eObsoleteTech != NO_TECH)
	{
		for (iI = 0; iI < GC.getNumCorporationInfos(); iI++)
		{
			if ((GC.getCorporationInfo((CorporationTypes)iI).getObsoleteTech()) == eObsoleteTech)
			{
				bValid = true;
				break;
			}
		}
		if (bValid)
		{
			for (int iPlayer = 0; iPlayer < MAX_PLAYERS; ++iPlayer)
			{
				CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iPlayer);

				if (kLoopPlayer.isAlive() && GET_TEAM(kLoopPlayer.getTeam()).getID() == getID())
				{
					for (CvCity* pLoopCity = kLoopPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kLoopPlayer.nextCity(&iLoop))
					{
						for (iI = 0; iI < GC.getNumCorporationInfos(); iI++)
						{
							if (pLoopCity->isHasCorporation((CorporationTypes)iI))
							{
								if ((GC.getCorporationInfo((CorporationTypes)iI).getObsoleteTech()) == eObsoleteTech)
								{
									pLoopCity->setHasCorporation((CorporationTypes)iI, false, false, false);
									//Remove the Corp HQ Building
									if (pLoopCity->isHeadquarters((CorporationTypes)iI))
									{
										for (int iJ = 0; iJ < GC.getNumBuildingInfos(); iJ++)
										{
											if ((CorporationTypes)GC.getBuildingInfo((BuildingTypes)iJ).getGlobalCorporationCommerce() == ((CorporationTypes)iI)
											|| GC.getBuildingInfo((BuildingTypes)iJ).getFoundsCorporation() == ((CorporationTypes)iI)
											|| GC.getBuildingInfo((BuildingTypes)iJ).getPrereqCorporation() == ((CorporationTypes)iI))
											{
												pLoopCity->setNumRealBuilding((BuildingTypes)iJ, 0);
											}
										}
										GC.getGameINLINE().setHeadquarters((CorporationTypes)iI, NULL, false);
										//Be Sure to Create a New Headquarters, ASAP
										GC.getGameINLINE().doFoundCorporation((CorporationTypes)iI, true);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

int CvTeam::getTradeModifier() const
{
	return m_iTradeModifier;
}
void CvTeam::changeTradeModifier(int iChange)
{
	if (iChange != 0)
	{
		m_iTradeModifier += iChange;
		for (int iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID() && GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				GET_PLAYER((PlayerTypes)iI).updateTradeRoutes();
			}
		}
	}
}

int CvTeam::getForeignTradeModifier() const
{
	return m_iForeignTradeModifier;
}
void CvTeam::changeForeignTradeModifier(int iChange)
{
	if (iChange != 0)
	{
		m_iForeignTradeModifier += iChange;
		for (int iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID() && GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				GET_PLAYER((PlayerTypes)iI).updateTradeRoutes();
			}
		}
	}
}

int CvTeam::getTradeMissionModifier() const
{
	return m_iTradeMissionModifier;
}
void CvTeam::changeTradeMissionModifier(int iChange)
{
	m_iTradeMissionModifier += iChange;
}

int CvTeam::getCorporationRevenueModifier() const
{
	return m_iCorporationRevenueModifier;
}
void CvTeam::changeCorporationRevenueModifier(int iChange)
{
	int iLoop = 0;
	if (iChange != 0)
	{
		m_iCorporationRevenueModifier += iChange;
		for (int iPlayer = 0; iPlayer < MAX_PLAYERS; ++iPlayer)
		{
			CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iPlayer);

			if (kLoopPlayer.isAlive() && GET_TEAM(kLoopPlayer.getTeam()).getID() == getID())
			{
				for (CvCity* pLoopCity = kLoopPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kLoopPlayer.nextCity(&iLoop))
				{
					pLoopCity->updateCorporation();
				}
			}
		}
	}
}

int CvTeam::getCorporationMaintenanceModifier() const
{
	return m_iCorporationMaintenanceModifier;
}
void CvTeam::changeCorporationMaintenanceModifier(int iChange)
{
	int iLoop = 0;
	if (iChange != 0)
	{
		m_iCorporationMaintenanceModifier += iChange;
		for (int iPlayer = 0; iPlayer < MAX_PLAYERS; ++iPlayer)
		{
			CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iPlayer);

			if (kLoopPlayer.isAlive() && GET_TEAM(kLoopPlayer.getTeam()).getID() == getID())
			{
				for (CvCity* pLoopCity = kLoopPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kLoopPlayer.nextCity(&iLoop))
				{
					pLoopCity->updateCorporation();
				}
			}
		}
	}
}

int CvTeam::getTotalVictoryScore()
{
	
	int iI, iK, iL;
	CvCity* pLoopCity;
	int iLoop = 0;
	int iTotalVictoryScore = 0;
	
	int globalCulture = 0;
	int globalThreeCityCulture = 0;
	int globalLand = 0;
	int globalPopulation = 0;
	int globalWonderScore = 0;
		
	int teamWonderScore = 0;
	int tempScore = 0;
	
	int totalTeamReligion = 0;
	int totalTeamLegendaryCities = 0;
	int tempReligion = 0;
		
	long globalPowerHistory =0;
	long teamPowerHistory =0;
	
	long tempPower =0;
	
	globalLand = GC.getMap().getLandPlots();
	
	
	for (iK = 0; iK < GC.getNumBuildingClassInfos(); iK++)
	{
		if (GC.getBuildingClassInfo((BuildingClassTypes)iK).getMaxGlobalInstances() == 1) 
		{
			globalWonderScore ++;
		}
	}

	// Get the Religion Info First
	// By definition, global religion percent is 100, so we don't need a variable for it.
	// Note: This detects whether the TEAM has the holy city.
	
	for (iK = 0; iK < GC.getNumReligionInfos(); iK++)
	{
		if (hasHolyCity((ReligionTypes)iK))
		{
			// Player has holy city
			tempReligion = GC.getGame().calculateReligionPercent((ReligionTypes)iK);
			if (tempReligion > totalTeamReligion)
			{
				totalTeamReligion = tempReligion;
			}
			
		}
	}
	
	
	
	// Get land, population, culture totals for player and globally.
	// Also get the starship launches and diplovictories achieved.

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			//Calculate global totals while looping through
			
			globalCulture += GET_PLAYER((PlayerTypes)iI).processedNationalCulture();
			globalPopulation += GET_PLAYER((PlayerTypes)iI).getTotalPopulation();
			
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				teamWonderScore += GET_PLAYER((PlayerTypes)iI).getSevoWondersScore(0);
			}
		}
	}
	
	// Get the power history sums
	
	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			//Calculate global totals while looping through
			tempPower = 0;
			for (iL = 0; iL <= GC.getGame().getGameTurn(); iL++)
			{
				tempPower += GET_PLAYER((PlayerTypes)iI).getPowerHistory(iL);
			}

			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				teamPowerHistory += tempPower;
			}
			
			globalPowerHistory += tempPower;
		}
	}

	// Get the number of legendary cities owned by this team
	
	for (iK = 0; iK < MAX_CIV_PLAYERS; iK++)
	{
		if ((GET_PLAYER((PlayerTypes)iK).getTeam() == getID()) && (GET_PLAYER((PlayerTypes)iK).isAlive()))
		{
			for (pLoopCity = GET_PLAYER((PlayerTypes)iK).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER((PlayerTypes)iK).nextCity(&iLoop))
			{
				// -2 is correct.  We need -1 to change from 'total num' to 'last index', and -1 to get the top level.
				if (pLoopCity->getCultureLevel() > GC.getNumCultureLevelInfos() - 2)
				{
					totalTeamLegendaryCities++;				
				}
			}		
		}
	}	
	

	// Add the WonderScore component
	if (globalWonderScore > 0)
	{
		iTotalVictoryScore += int(teamWonderScore * 100 / globalWonderScore);
	}	
	

	// Add the population score component
	if (globalPopulation > 0)
	{
		iTotalVictoryScore += int(getTotalPopulation() * 100 / globalPopulation);
	}

	// Add the land score component
	if (globalLand > 0) 
	{
		iTotalVictoryScore += int(getTotalLand() * 100 / globalLand);
	}
	

	// Add the culture score component
	if (globalCulture > 0)
	{
		unsigned long long iTotalCulture;
		iTotalCulture = countTotalCulture();

		if ( iTotalCulture > MAX_INT/100 )
		{
			iTotalVictoryScore += int(countTotalCulture()/(globalCulture/100));
		}
		else
		{
			iTotalVictoryScore += int(countTotalCulture() * 100 / globalCulture);
		}
	}
	

	// Add the legendary cities component
	if (totalTeamLegendaryCities > 0)
	{
		iTotalVictoryScore += (20 * totalTeamLegendaryCities);
	}
	

	// Add the Power component
	if (globalPowerHistory > 0)
	{
		iTotalVictoryScore += (teamPowerHistory * 100 / globalPowerHistory);
	}
	
	
	// Add the Religion component
	iTotalVictoryScore += totalTeamReligion;
	
	//Starship points!  Big money.
int iProjectInfos, qVictory, qNeeded;
	for (iProjectInfos = 0; iProjectInfos < GC.getNumProjectInfos(); ++iProjectInfos)
	{
		CvProjectInfo& kInfo = GC.getProjectInfo((ProjectTypes)iProjectInfos);
		for (qVictory = 0; qVictory < GC.getNumVictoryInfos(); ++qVictory) 
		{
			qNeeded = kInfo.getVictoryThreshold((VictoryTypes)qVictory);
			if ((qNeeded > 0) && (qNeeded <= getProjectCount((ProjectTypes)iProjectInfos)) && ((getLaunchSuccessRate((VictoryTypes)qVictory)) == 100))
			{
				iTotalVictoryScore +=  20;
			}
		}
	}	
	
	//Ascension Gate points (Scientific Victory)!  Big money! (Thanks to God-Emperor @CivFanatics)
int iBuildingClass, iVictory, iNeeded;
	for (iBuildingClass = 0; iBuildingClass < GC.getNumBuildingClassInfos(); ++iBuildingClass)
	{
		CvBuildingClassInfo& kInfo = GC.getBuildingClassInfo((BuildingClassTypes)iBuildingClass);
		for (iVictory = 0; iVictory < GC.getNumVictoryInfos(); ++iVictory) 
		{
			iNeeded = kInfo.getVictoryThreshold((VictoryTypes)iVictory);
			if ((iNeeded > 0) && (iNeeded <= getBuildingClassCount((BuildingClassTypes)iBuildingClass)))
			{
				iTotalVictoryScore += 100;
				AGBuilt = 1;
			}
		}
	}		
	
	return iTotalVictoryScore;
	
}



int CvTeam::getBuildingCommerceChange(BuildingTypes eIndex1, CommerceTypes eIndex2) const
{
	FAssertMsg(eIndex1 >= 0, "eIndex1 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex1 < GC.getNumBuildingInfos(), "eIndex1 is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(eIndex2 >= 0, "eIndex2 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex2 < NUM_COMMERCE_TYPES, "eIndex2 is expected to be within maximum bounds (invalid Index)");
	return m_ppiBuildingCommerceChange[eIndex1][eIndex2];
}

void CvTeam::changeBuildingCommerceChange(BuildingTypes eIndex1, CommerceTypes eIndex2, int iChange)
{
	FAssertMsg(eIndex1 >= 0, "eIndex1 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex1 < GC.getNumBuildingInfos(), "eIndex1 is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(eIndex2 >= 0, "eIndex2 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex2 < NUM_COMMERCE_TYPES, "eIndex2 is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		int iOldValue = m_ppiBuildingCommerceChange[eIndex1][eIndex2];
		int iExistingValue;
		m_ppiBuildingCommerceChange[eIndex1][eIndex2] += iChange;	
		
		for (int iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
				{
					CvCity* pLoopCity;
					int iLoop = 0;

					for (pLoopCity = GET_PLAYER((PlayerTypes)iI).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER((PlayerTypes)iI).nextCity(&iLoop))
					{
						iExistingValue = pLoopCity->getBuildingCommerceChange((BuildingClassTypes)GC.getBuildingInfo(eIndex1).getBuildingClassType(), eIndex2);
						// set the new
						pLoopCity->updateCommerceRateByBuilding(eIndex1, eIndex2, (iExistingValue - iOldValue + getBuildingCommerceChange(eIndex1, eIndex2)));
					}
				}
			}
		}
		updateCommerce();
	}
}

int CvTeam::getBuildingYieldChange(BuildingTypes eIndex1, YieldTypes eIndex2) const
{
	FAssertMsg(eIndex1 >= 0, "eIndex1 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex1 < GC.getNumBuildingInfos(), "eIndex1 is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(eIndex2 >= 0, "eIndex2 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex2 < NUM_YIELD_TYPES, "eIndex2 is expected to be within maximum bounds (invalid Index)");
	return m_ppiBuildingYieldChange[eIndex1][eIndex2];
}

void CvTeam::changeBuildingYieldChange(BuildingTypes eIndex1, YieldTypes eIndex2, int iChange)
{
	FAssertMsg(eIndex1 >= 0, "eIndex1 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex1 < GC.getNumBuildingInfos(), "eIndex1 is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(eIndex2 >= 0, "eIndex2 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex2 < NUM_YIELD_TYPES, "eIndex2 is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		int iOldValue = getBuildingYieldChange(eIndex1, eIndex2);
		int iExistingValue;
		m_ppiBuildingYieldChange[eIndex1][eIndex2] += iChange;		

		for (int iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
				{					
					CvCity* pLoopCity;
					int iLoop = 0;

					for (pLoopCity = GET_PLAYER((PlayerTypes)iI).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER((PlayerTypes)iI).nextCity(&iLoop))
					{
						iExistingValue = pLoopCity->getBuildingYieldChange((BuildingClassTypes)GC.getBuildingInfo(eIndex1).getBuildingClassType(), eIndex2);
						
						pLoopCity->updateYieldRate(eIndex1, eIndex2, (iExistingValue - iOldValue + getBuildingYieldChange(eIndex1, eIndex2)));
					}
				}
			}
		}
	}
}

int CvTeam::getBuildingSpecialistChange(BuildingTypes eIndex1, SpecialistTypes eIndex2) const
{
	FAssertMsg(eIndex1 >= 0, "eIndex1 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex1 < GC.getNumBuildingInfos(), "eIndex1 is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(eIndex2 >= 0, "eIndex2 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex2 < GC.getNumSpecialistInfos(), "eIndex2 is expected to be within maximum bounds (invalid Index)");
	return m_ppiBuildingSpecialistChange[eIndex1][eIndex2];
}

void CvTeam::changeBuildingSpecialistChange(BuildingTypes eIndex1, SpecialistTypes eIndex2, int iChange)
{
	FAssertMsg(eIndex1 >= 0, "eIndex1 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex1 < GC.getNumBuildingInfos(), "eIndex1 is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(eIndex2 >= 0, "eIndex2 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex2 < GC.getNumSpecialistInfos(), "eIndex2 is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{		
		int iOldValue = getBuildingSpecialistChange(eIndex1, eIndex2);
		m_ppiBuildingSpecialistChange[eIndex1][eIndex2] += iChange;		
		
		for (int iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
				{					
					CvCity* pLoopCity;
					int iLoop = 0;

					for (pLoopCity = GET_PLAYER((PlayerTypes)iI).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER((PlayerTypes)iI).nextCity(&iLoop))
					{
						// remove the old
						pLoopCity->updateMaxSpecialistCount(eIndex1, eIndex2, -iOldValue);
						// set the new
						pLoopCity->updateMaxSpecialistCount(eIndex1, eIndex2, getBuildingSpecialistChange(eIndex1, eIndex2));
					}
				}
			}
		}
	}
}

int CvTeam::getBuildingCommerceModifier(BuildingTypes eIndex1, CommerceTypes eIndex2) const
{
	FAssertMsg(eIndex1 >= 0, "eIndex1 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex1 < GC.getNumBuildingInfos(), "eIndex1 is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(eIndex2 >= 0, "eIndex2 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex2 < NUM_COMMERCE_TYPES, "eIndex2 is expected to be within maximum bounds (invalid Index)");
	return m_ppiBuildingCommerceModifier[eIndex1][eIndex2];
}

void CvTeam::changeBuildingCommerceModifier(BuildingTypes eIndex1, CommerceTypes eIndex2, int iChange)
{
	FAssertMsg(eIndex1 >= 0, "eIndex1 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex1 < GC.getNumBuildingInfos(), "eIndex1 is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(eIndex2 >= 0, "eIndex2 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex2 < NUM_COMMERCE_TYPES, "eIndex2 is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		int iOldValue = getBuildingCommerceModifier(eIndex1, eIndex2);
		int iExistingValue;
		m_ppiBuildingCommerceModifier[eIndex1][eIndex2] += iChange;		

		for (int iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
				{					
					CvCity* pLoopCity;
					int iLoop = 0;

					for (pLoopCity = GET_PLAYER((PlayerTypes)iI).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER((PlayerTypes)iI).nextCity(&iLoop))
					{
						iExistingValue = pLoopCity->getBuildingCommerceModifier((BuildingClassTypes)GC.getBuildingInfo(eIndex1).getBuildingClassType(), eIndex2);
						// set the new
						pLoopCity->updateCommerceModifierByBuilding(eIndex1, eIndex2, (iExistingValue - iOldValue + getBuildingCommerceModifier(eIndex1, eIndex2)));
					}
				}
			}
		}
	}
}

int CvTeam::getBuildingYieldModifier(BuildingTypes eIndex1, YieldTypes eIndex2) const
{
	FAssertMsg(eIndex1 >= 0, "eIndex1 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex1 < GC.getNumBuildingInfos(), "eIndex1 is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(eIndex2 >= 0, "eIndex2 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex2 < NUM_YIELD_TYPES, "eIndex2 is expected to be within maximum bounds (invalid Index)");
	return m_ppiBuildingYieldModifier[eIndex1][eIndex2];
}

void CvTeam::changeBuildingYieldModifier(BuildingTypes eIndex1, YieldTypes eIndex2, int iChange)
{
	FAssertMsg(eIndex1 >= 0, "eIndex1 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex1 < GC.getNumBuildingInfos(), "eIndex1 is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(eIndex2 >= 0, "eIndex2 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex2 < NUM_YIELD_TYPES, "eIndex2 is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{		
		int iOldValue = getBuildingYieldModifier(eIndex1, eIndex2);
		int iExistingValue;
		m_ppiBuildingYieldModifier[eIndex1][eIndex2] += iChange;		
		
		for (int iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
				{					
					CvCity* pLoopCity;
					int iLoop = 0;

					for (pLoopCity = GET_PLAYER((PlayerTypes)iI).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER((PlayerTypes)iI).nextCity(&iLoop))
					{
						iExistingValue = pLoopCity->getBuildingYieldModifier((BuildingClassTypes)GC.getBuildingInfo(eIndex1).getBuildingClassType(), eIndex2);
						// set the new
						pLoopCity->updateYieldModifierByBuilding(eIndex1, eIndex2, (iExistingValue - iOldValue + getBuildingYieldModifier(eIndex1, eIndex2)));
					}
				}
			}
		}
	}
}

void CvTeam::setLimitedBorders(TeamTypes eIndex, bool bNewValue)
{
	bool bOldFreeTrade;
	int iI;

	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (isLimitedBorders(eIndex) != bNewValue)
	{
		bOldFreeTrade = isFreeTrade(eIndex);

		m_abLimitedBorders[eIndex] = bNewValue;

		GC.getMapINLINE().verifyUnitValidPlot();

		if ((getID() == GC.getGameINLINE().getActiveTeam()) || (eIndex == GC.getGameINLINE().getActiveTeam()))
		{
			gDLL->getInterfaceIFace()->setDirty(Score_DIRTY_BIT, true);
		}

		if (bOldFreeTrade != isFreeTrade(eIndex))
		{
			for (iI = 0; iI < MAX_PLAYERS; iI++)
			{
				if (GET_PLAYER((PlayerTypes)iI).isAlive())
				{
					if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
					{
						GET_PLAYER((PlayerTypes)iI).updateTradeRoutes();
					}
				}
			}
		}
	}
}

int CvTeam::getFreeSpecialistCount(SpecialistTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex expected to be >= 0");
	FAssertMsg(eIndex < GC.getNumSpecialistInfos(), "eIndex expected to be < GC.getNumSpecialistInfos()");
	return m_paiFreeSpecialistCount[eIndex];
}

void CvTeam::setFreeSpecialistCount(SpecialistTypes eIndex, int iNewValue)
{
	int iOldValue;

	FAssertMsg(eIndex >= 0, "eIndex expected to be >= 0");
	FAssertMsg(eIndex < GC.getNumSpecialistInfos(), "eIndex expected to be < GC.getNumSpecialistInfos()");

	iOldValue = getFreeSpecialistCount(eIndex);

	if (iOldValue != iNewValue)
	{
		m_paiFreeSpecialistCount[eIndex] = iNewValue;
		FAssert(getFreeSpecialistCount(eIndex) >= 0);
		
		for (int iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
				{
					CvCity* pLoopCity;
					int iLoop = 0;

					for (pLoopCity = GET_PLAYER((PlayerTypes)iI).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER((PlayerTypes)iI).nextCity(&iLoop))
					{
						pLoopCity->changeFreeSpecialistCount(eIndex, 0);
					}
				}
			}
		}
	}
}

void CvTeam::changeFreeSpecialistCount(SpecialistTypes eIndex, int iChange)
{
	setFreeSpecialistCount(eIndex, (getFreeSpecialistCount(eIndex) + iChange));
}

int CvTeam::getUnitClassStrengthChange(UnitClassTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex expected to be >= 0");
	FAssertMsg(eIndex < GC.getNumUnitClassInfos(), "eIndex expected to be < GC.getNumUnitClassInfos()");
	return m_paiUnitClassStrengthChange[eIndex];
}

void CvTeam::changeUnitClassStrengthChange(UnitClassTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex expected to be >= 0");
	FAssertMsg(eIndex < GC.getNumUnitClassInfos(), "eIndex expected to be < GC.getNumUnitClassInfos()");
	m_paiUnitClassStrengthChange[eIndex] += iChange;
}

bool CvTeam::isAnyVassal() const
{
	int iI;
	for (iI = 0; iI < MAX_TEAMS; iI++)
	{
		if (iI != getID() && GET_TEAM((TeamTypes)iI).isAlive())
		{
			if (isVassal((TeamTypes)iI))
			{
				return true;
			}
		}
	}
	return false;
}

ImprovementTypes CvTeam::getImprovementUpgrade(ImprovementTypes eImprovement) const
{
	if (GC.getImprovementInfo(eImprovement).getImprovementUpgrade() == NO_IMPROVEMENT)
	{
		return NO_IMPROVEMENT;
	}
	if (GC.getImprovementInfo((ImprovementTypes)GC.getImprovementInfo(eImprovement).getImprovementUpgrade()).getPrereqTech() != NO_TECH)
	{
		if (!isHasTech((TechTypes)GC.getImprovementInfo((ImprovementTypes)GC.getImprovementInfo(eImprovement).getImprovementUpgrade()).getPrereqTech()))
		{
			return NO_IMPROVEMENT;
		}
	}
	
	return (ImprovementTypes)GC.getImprovementInfo(eImprovement).getImprovementUpgrade();
}

ImprovementTypes CvTeam::finalImprovementUpgrade(ImprovementTypes eImprovement) const
{
	if (getImprovementUpgrade(eImprovement) != NO_IMPROVEMENT)
	{
		return finalImprovementUpgrade(getImprovementUpgrade(eImprovement));
	}
	else
	{
		return eImprovement;
	}
}

bool CvTeam::isFreeTradeAgreement(TeamTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_abFreeTrade[eIndex];
}


void CvTeam::setFreeTradeAgreement(TeamTypes eIndex, bool bNewValue)
{
	bool bOldFreeTrade;
	int iI;

	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (isFreeTradeAgreement(eIndex) != bNewValue)
	{
		bOldFreeTrade = isFreeTrade(eIndex);

		m_abFreeTrade[eIndex] = bNewValue;

		if ((getID() == GC.getGameINLINE().getActiveTeam()) || (eIndex == GC.getGameINLINE().getActiveTeam()))
		{
			gDLL->getInterfaceIFace()->setDirty(Score_DIRTY_BIT, true);
		}

		if (bOldFreeTrade != isFreeTrade(eIndex))
		{
			for (iI = 0; iI < MAX_PLAYERS; iI++)
			{
				if (GET_PLAYER((PlayerTypes)iI).isAlive())
				{
					if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
					{
						GET_PLAYER((PlayerTypes)iI).updateTradeRoutes();
					}
				}
			}
		}
	}
}

void CvTeam::AI_updateBonusValue(BonusTypes eBonus)
{
	for (int iPlayer = 0; iPlayer < MAX_PLAYERS; iPlayer++)
	{
		if (GET_PLAYER((PlayerTypes)iPlayer).getTeam() == getID() && GET_PLAYER((PlayerTypes)iPlayer).isAlive())
		{
			GET_PLAYER((PlayerTypes)iPlayer).AI_updateBonusValue(eBonus);
		}
	}
}
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/

void CvTeam::addPropertiesAllCities(CvProperties *pProp)
{
	for (int iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				GET_PLAYER((PlayerTypes)iI).addPropertiesAllCities(pProp);
			}
		}
	}
}

void CvTeam::subtractPropertiesAllCities(CvProperties *pProp)
{
	for (int iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
			{
				GET_PLAYER((PlayerTypes)iI).subtractPropertiesAllCities(pProp);
			}
		}
	}
}

void CvTeam::recalculateModifiers()
{
	int iI, iJ;

	for (int iPlayer = 0; iPlayer < MAX_PLAYERS; iPlayer++)
	{
		if (GET_PLAYER((PlayerTypes)iPlayer).getTeam() == getID() && GET_PLAYER((PlayerTypes)iPlayer).isAlive())
		{
			GET_PLAYER((PlayerTypes)iPlayer).clearModifierTotals();
		}
	}

	for (iI = 0; iI < MAX_TEAMS; iI++)
	{
		m_aiTechShareCount[iI] = 0;
	}

	//	Clear team level info (from techs)
	m_iExtraWaterSeeFromCount = 0;
	m_iCanPassPeaksCount = 0;
	m_iMoveFastPeaksCount = 0;
	m_iCanFoundOnPeaksCount = 0;
	m_iRebaseAnywhereCount = 0;
// Movement Limits by 45deg - START
	m_iExtendMovementLimitsCount = 0;
	m_iRemoveMovementLimitsCount = 0;	
// Movement Limits by 45deg - END
	m_iForeignTradeModifier = 0;
	m_iTradeModifier = 0;
	m_iTradeMissionModifier = 0;
	m_iCorporationRevenueModifier = 0;
	m_iCorporationMaintenanceModifier = 0;
	m_iCanFarmDesertCount = 0;
	m_iOpenBordersTradingCount = 0;
	m_iDefensivePactTradingCount = 0;
	m_iPermanentAllianceTradingCount = 0;
	m_iVassalTradingCount = 0;
	m_iBridgeBuildingCount = 0;
	m_iIrrigationCount = 0;
	m_iIgnoreIrrigationCount = 0;
	m_iWaterWorkCount = 0;
	m_iVassalPower = 0;
	m_iMasterPower = 0;
	m_iEnemyWarWearinessModifier = 0;
	m_iRiverTradeCount = 0;
	m_iNukeInterception = 0;

	for (iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
	{
		m_paiBuildingClassCount[iI] = 0;
	}

	for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
	{
		m_paiObsoleteBuildingCount[iI] = 0;
	}

	for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
	{
		m_paiTechExtraBuildingHappiness[iI] = 0;
	}
	
	for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
	{
		m_paiTechExtraBuildingHealth[iI] = 0;
	}

	for (iI = 0; iI < GC.getNumImprovementInfos(); iI++)
	{
		for (iJ = 0; iJ < NUM_YIELD_TYPES; iJ++)
		{
			m_ppaaiImprovementYieldChange[iI][iJ] = 0;
		}
	}

	for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
	{
		for (iJ = 0; iJ < NUM_COMMERCE_TYPES; iJ++)
		{
			m_ppiBuildingCommerceChange[iI][iJ] = 0;
		}
	}

	for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
	{
		for (iJ = 0; iJ < NUM_YIELD_TYPES; iJ++)
		{
			m_ppiBuildingYieldChange[iI][iJ] = 0;
		}
	}

	for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
	{
		for (iJ = 0; iJ < GC.getNumSpecialistInfos(); iJ++)
		{
			m_ppiBuildingSpecialistChange[iI][iJ] = 0;
		}
	}

	for (iJ = 0; iJ < GC.getNumSpecialistInfos(); iJ++)
	{
		m_paiFreeSpecialistCount[iJ] = 0;
	}

	for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
	{
		for (iJ = 0; iJ < NUM_COMMERCE_TYPES; iJ++)
		{
			m_ppiBuildingCommerceModifier[iI][iJ] = 0;
		}
	}

	for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
	{
		for (iJ = 0; iJ < NUM_YIELD_TYPES; iJ++)
		{
			m_ppiBuildingYieldModifier[iI][iJ] = 0;
		}
	}

	for (iJ = 0; iJ < GC.getNumUnitClassInfos(); iJ++)
	{
		m_paiUnitClassStrengthChange[iJ] = 0;
	}

	for (iI = 0; iI < NUM_DOMAIN_TYPES; iI++)
	{
		m_aiExtraMoves[iI] = 0;
	}

	for (int iPlayer = 0; iPlayer < MAX_PLAYERS; iPlayer++)
	{
		if (GET_PLAYER((PlayerTypes)iPlayer).getTeam() == getID() && GET_PLAYER((PlayerTypes)iPlayer).isAlive())
		{
			GET_PLAYER((PlayerTypes)iPlayer).recalculateModifiers();
		}
	}

	//	Reapply techs
	for(int iI = 0; iI < GC.getNumTechInfos(); iI++)
	{
		if ( isHasTech((TechTypes)iI) )
		{
			processTech((TechTypes)iI,1);
		}
	}

	//	Reapply circumnavigation bonus
	if (GC.getGameINLINE().isCircumnavigated(getID()) && GC.getDefineINT("CIRCUMNAVIGATE_FREE_MOVES") != 0)
	{
		changeExtraMoves(DOMAIN_SEA, GC.getDefineINT("CIRCUMNAVIGATE_FREE_MOVES"));
	}

	//	Reapply projects
	for(int iI = 0; iI < GC.getNumProjectInfos(); iI++)
	{
		if ( getProjectCount((ProjectTypes)iI) > 0 )
		{
			processProjectChange((ProjectTypes)iI, getProjectCount((ProjectTypes)iI), getProjectCount((ProjectTypes)iI));
		}
	}
}

CvProperties* CvTeam::getProperties()
{
	return &m_Properties;
}

const CvProperties* CvTeam::getPropertiesConst() const
{
	return &m_Properties;
}

// K-Mod
int CvTeam::getTypicalUnitValue(UnitAITypes eUnitAI) const
{
	int iMax = 0;
	for (int iI = 0; iI < MAX_PLAYERS; ++iI)
	{
		if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
		{
			iMax = std::max(iMax, GET_PLAYER((PlayerTypes)iI).getTypicalUnitValue(eUnitAI));
		}
	}
	return iMax;
}

void CvTeam::doEmbassyContactCheck()
{
	if (isBarbarian())
	{
		return;
	}

	//45deg: Embassy code - start:
	// How to deal with espionage? I should add code for cities which are visible because of espionage: maybe contact should not be cut if I have enough espionage points to see a city, but this way
	// the other civ might see me and contact me even if they don't have any real contact with me except espionage; same for spies
	// Easier way is to drop espionage issues and hence exclude spies and great spies from units that are seeing another civ's territory
	// I still need to add a randomizing code so that contacts will be lost after some turns (depending on gamespeed)

	//Afforess: allow permenant contact tech to be overriden in global defines
	TechTypes ePermenantContactTech = (TechTypes)GC.getInfoTypeForString(GC.getDefineSTRING("TECH_ALLOWS_PERMENANT_CONTACT", "TECH_TELEGRAPH"));
	if (ePermenantContactTech != NO_TECH)
	{
		if (!isHasTech(ePermenantContactTech))
		{

			//6 x 8 = 48 turns. Tweak these parameters for longer or shorter delays between contact lost

			//How often the game should check if we still have contact with other civs (10 = once every 10 turns. 1 = every turn.)
			//Do not scale for gamespeed, only 1 paramater should be scaled
			int iContactDecaySamplingRate = GC.getDefineINT("CONTACT_DECAY_SAMPLING_RATE", 6);

			//How many times that the player can "fail" to be in contact with another civ before contact is lost
			int iContactMaxFailedChecks = GC.getDefineINT("CONTACT_DECAY_NUM_OF_FAILED_CHECKS_BEFORE_CONTACT_LOST", 8);

			iContactMaxFailedChecks *= GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getGrowthPercent();
			iContactMaxFailedChecks /= 110;

			if (GC.getGameINLINE().getElapsedGameTurns() % iContactDecaySamplingRate == 0)
			{
				for (int iI = 0; iI < MAX_TEAMS; iI++)
				{
					if (GET_TEAM((TeamTypes)iI).isAlive() && iI != getID() && iI != BARBARIAN_TEAM)
					{
						if (isHasMet((TeamTypes)iI) && !GET_TEAM((TeamTypes)iI).isHasTech(ePermenantContactTech))
						{
							if (!isHasEmbassy((TeamTypes)iI) && !isAtWar((TeamTypes)iI))
							{
								if (!isSharesBorderWith((TeamTypes)iI) && !isTerritoryVisibleFromUnits((TeamTypes)iI))
								{
									m_aiFailedContactChecks[iI]++;
									if (m_aiFailedContactChecks[iI] > iContactMaxFailedChecks)
									{
										cutContact((TeamTypes)iI);
										m_aiFailedContactChecks[iI] = 0;
									}
								}
							}
							else
							{
								m_aiFailedContactChecks[iI] = 0; //we have an embassy or ar at war, reset count
							}
						}
					}
					else
					{
						m_aiFailedContactChecks[iI] = 0; //reset to zero for dead civs, may revive and don't want to have any failed contact checks in memory
					}
				}
			}
		}
	}
	//45deg: Embassy code - end
}

bool CvTeam::isTerritoryVisibleFromUnits(TeamTypes eTeam) const
{
	for (int iI = 0; iI < MAX_CIV_PLAYERS; iI++)
	{
		CvPlayer& kPlayer = GET_PLAYER((PlayerTypes)iI);
		if (kPlayer.isAlive() && kPlayer.getTeam() == getID())
		{
			int iLoop = 0;
			for (CvUnit* pLoopUnit = kPlayer.firstUnit(&iLoop); pLoopUnit; pLoopUnit = kPlayer.nextUnit(&iLoop))
			{
				int iRange = pLoopUnit->visibilityRange(pLoopUnit->plot());
				for (int i = -iRange; i <= iRange; ++i)
				{
					for (int j = -iRange; j <= iRange; ++j)
					{
						CvPlot* pLoopPlot = ::plotXY(pLoopUnit->getX_INLINE(), pLoopUnit->getY_INLINE(), i, j);
						if (NULL != pLoopPlot)
						{
							if (pLoopPlot->getTeam() == eTeam)
							{
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}

bool CvTeam::isSharesBorderWith(TeamTypes eTeam) const
{
	for (int iPlot = 0; iPlot < GC.getMapINLINE().numPlotsINLINE(); ++iPlot)
	{
		CvPlot* pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iPlot);

		if (pLoopPlot->getTeam() == getID())
		{
			CvPlot* pAdjacentPlot;

			for (int iJ = 0; iJ < NUM_DIRECTION_TYPES; ++iJ)
			{
				pAdjacentPlot = plotDirection(pLoopPlot->getX_INLINE(), pLoopPlot->getY_INLINE(), ((DirectionTypes)iJ));

				if (pAdjacentPlot != NULL)
				{
					if (pAdjacentPlot->getTeam() == eTeam)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}
