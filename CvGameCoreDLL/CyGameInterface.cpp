#include "CvGameCoreDLL.h"
#include "CyGame.h"
#include "CvRandom.h"
#include "CyCity.h"
#include "CyDeal.h"
#include "CyReplayInfo.h"
#include "CyPlot.h"
//
// published python interface for CyGame
// 

void CyGamePythonInterface()
{
	OutputDebugString("Python Extension Module - CyGamePythonInterface\n");

	python::class_<CyGame>("CyGame")
		.def("isNone", &CyGame::isNone, "CyGame* () - is the instance valid?")
/************************************************************************************************/
/* REVOLUTION_MOD                                                                 lemmy101      */
/*                                                                                jdog5000      */
/*                                                                                              */
/************************************************************************************************/
		.def("isMultiplayer", &CyGame::isMultiplayer, "CyGame* () - is the instance valid?")
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/

		.def("updateScore", &CyGame::updateScore, "void (bool bForce)")
		.def("cycleCities", &CyGame::cycleCities, "void (bool bForward, bool bAdd)")
		.def("cycleSelectionGroups", &CyGame::cycleSelectionGroups, "void (bool bClear, bool bForward, bool bWorkers)")
		.def("cyclePlotUnits", &CyGame::cyclePlotUnits, "bool (CyPlot* pPlot, bool bForward, bool bAuto, int iCount)")

		.def("selectionListMove", &CyGame::selectionListMove, "void (CyPlot* pPlot, bool bAlt, bool bShift, bool bCtrl)")
		.def("selectionListGameNetMessage", &CyGame::selectionListGameNetMessage, "void (int eMessage, int iData2, int iData3, int iData4, int iFlags, bool bAlt, bool bShift)")
		.def("selectedCitiesGameNetMessage", &CyGame::selectedCitiesGameNetMessage, "void (int eMessage, int iData2, int iData3, int iData4, bool bOption, bool bAlt, bool bShift, bool bCtrl)")
		.def("cityPushOrder", &CyGame::cityPushOrder, "void (CyCity* pCity, OrderTypes eOrder, int iData, bool bAlt, bool bShift, bool bCtrl)")

		.def("getSymbolID", &CyGame::getSymbolID, "int (int iSymbol)")

		.def("getProductionPerPopulation", &CyGame::getProductionPerPopulation, "int (int /*HurryTypes*/ eHurry)")

		.def("getAdjustedPopulationPercent", &CyGame::getAdjustedPopulationPercent, "int (int eVictory)")
		.def("getAdjustedLandPercent", &CyGame::getAdjustedLandPercent, "int (int eVictory)")

		.def("isTeamVote", &CyGame::isTeamVote, "bool (int eVote)")
		.def("isChooseElection", &CyGame::isChooseElection, "bool (int eVote)")
		.def("isTeamVoteEligible", &CyGame::isTeamVoteEligible, "bool (int eTeam, int eVoteSource)")
		.def("countPossibleVote", &CyGame::countPossibleVote, "int (int eVote, int eVoteSource)")
		.def("getVoteRequired", &CyGame::getVoteRequired, "int (int eVote, int eVoteSource)")
		.def("getSecretaryGeneral", &CyGame::getSecretaryGeneral, "int (int eVoteSource)")
		.def("canHaveSecretaryGeneral", &CyGame::canHaveSecretaryGeneral, "bool (int eVoteSource)")
		.def("getVoteSourceReligion", &CyGame::getVoteSourceReligion, "int (int /*VoteSourceTypes*/ eVoteSource)")
		.def("setVoteSourceReligion", &CyGame::setVoteSourceReligion, "void (int /*VoteSourceTypes*/ eVoteSource, int /*ReligionTypes*/ eReligion, bool bAnnounce)")

		.def("countCivPlayersAlive", &CyGame::countCivPlayersAlive, "int ()")
		.def("countCivPlayersEverAlive", &CyGame::countCivPlayersEverAlive, "int ()")
		.def("countCivTeamsAlive", &CyGame::countCivTeamsAlive, "int ()")
		.def("countCivTeamsEverAlive", &CyGame::countCivTeamsEverAlive, "int ()")
		.def("countHumanPlayersAlive", &CyGame::countHumanPlayersAlive, "int ()")

		.def("countTotalCivPower", &CyGame::countTotalCivPower, "int ()")
		.def("countTotalNukeUnits", &CyGame::countTotalNukeUnits, "int ()")
		.def("countKnownTechNumTeams", &CyGame::countKnownTechNumTeams, "int (int eTech)")
		.def("getNumFreeBonuses", &CyGame::getNumFreeBonuses, "int (int eBonus)")

		.def("countReligionLevels", &CyGame::countReligionLevels, "int (int eReligion)")
		.def("calculateReligionPercent", &CyGame::calculateReligionPercent, "int (int eReligion)")
		.def("countCorporationLevels", &CyGame::countCorporationLevels, "int (int eCorporation)")

		.def("goldenAgeLength", &CyGame::goldenAgeLength, "int ()")
		.def("victoryDelay", &CyGame::victoryDelay, "int (int /*VictoryTypes*/ eVictory)")
		.def("getImprovementUpgradeTime", &CyGame::getImprovementUpgradeTime, "int (int /*ImprovementTypes*/ eImprovement)")
		.def("canTrainNukes", &CyGame::canTrainNukes, "bool ()")

/************************************************************************************************/
/* RevDCM	                  Start		 11/04/10                                phungus420     */
/*                                                                                              */
/* New World Logic                                                                              */
/************************************************************************************************/
		.def("getHighestEra", &CyGame::getHighestEra, "int /*EratTypes*/ ()")
/************************************************************************************************/
/* New World Logic                 END                                                          */
/************************************************************************************************/
		.def("getCurrentEra", &CyGame::getCurrentEra, "int /*EratTypes*/ ()")

		.def("getActiveTeam", &CyGame::getActiveTeam, "int () - returns ID for the group")
		.def("getActiveCivilizationType", &CyGame::getActiveCivilizationType, "int () - returns CivilizationID" )
		.def("isNetworkMultiPlayer", &CyGame::isNetworkMultiPlayer, "bool () - NetworkMultiplayer()? ")
		.def("isGameMultiPlayer", &CyGame::isGameMultiPlayer, "bool () - GameMultiplayer()? ")
		.def("isTeamGame", &CyGame::isTeamGame, "bool ()")
		.def("getNumHumanPlayers", &CyGame::getNumHumanPlayers, "int () - # of human players in-game")

		.def("isModem", &CyGame::isModem, "bool () - Using a modem? ")
		.def("setModem", &CyGame::setModem, "void (bool bModem) - Use a modem! (or don't)")

/************************************************************************************************/
/* REVOLUTION_MOD                                                                  lemmy101     */
/*                                                                                 jdog5000     */
/*                                                                                              */
/************************************************************************************************/
		.def("reviveActivePlayer", &CyGame::reviveActivePlayer, "void ()")
		.def("revivePlayer", &CyGame::revivePlayer, "void ()")
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/

		.def("getGameTurn", &CyGame::getGameTurn, "int () - current game turn")
		.def("setGameTurn", &CyGame::setGameTurn, "void (iNewValue) - set current game turn")
		.def("getTurnYear", &CyGame::getTurnYear, "int (iGameTurn) - turn Time")
		.def("getGameTurnYear", &CyGame::getGameTurnYear)
		.def("getElapsedGameTurns", &CyGame::getElapsedGameTurns, "int () - Elapsed turns thus far")
		.def("getMaxTurns", &CyGame::getMaxTurns)
		.def("setMaxTurns", &CyGame::setMaxTurns)
		.def("changeMaxTurns", &CyGame::changeMaxTurns)
		.def("getMaxCityElimination", &CyGame::getMaxCityElimination)
		.def("setMaxCityElimination", &CyGame::setMaxCityElimination)
		.def("getNumAdvancedStartPoints", &CyGame::getNumAdvancedStartPoints)
		.def("setNumAdvancedStartPoints", &CyGame::setNumAdvancedStartPoints)
		.def("getStartTurn", &CyGame::getStartTurn, "int () - Returns the starting Turn (0 unless a scenario or advanced era start)")
		.def("getStartYear", &CyGame::getStartYear, "int () - Returns the starting year (e.g. -4000)")
		.def("setStartYear", &CyGame::setStartYear, "void () - Sets the starting year (e.g. -4000)")
		.def("getEstimateEndTurn", &CyGame::getEstimateEndTurn)
		.def("setEstimateEndTurn", &CyGame::setEstimateEndTurn)
		.def("getTurnSlice", &CyGame::getTurnSlice)
//PB Mod - START
		.def("incrementTurnTimer", &CyGame::incrementTurnTimer,"void (int iNumTurnSlices) - Change turn timer slices")
//PB Mod - END		
		.def("getMinutesPlayed", &CyGame::getMinutesPlayed, "Returns the number of minutes since the game began")
		.def("getTargetScore", &CyGame::getTargetScore)
		.def("setTargetScore", &CyGame::setTargetScore)

		.def("getNumGameTurnActive", &CyGame::getNumGameTurnActive)
		.def("countNumHumanGameTurnActive", &CyGame::countNumHumanGameTurnActive)
		.def("getNumCities", &CyGame::getNumCities, "int () - total cities in Game")
		.def("getNumCivCities", &CyGame::getNumCivCities, "int () - total non-barbarian cities in Game")
		.def("getTotalPopulation", &CyGame::getTotalPopulation, "int () - total game population")

		.def("getTradeRoutes", &CyGame::getTradeRoutes)
		.def("changeTradeRoutes", &CyGame::changeTradeRoutes)
		.def("getFreeTradeCount", &CyGame::getFreeTradeCount)
		.def("isFreeTrade", &CyGame::isFreeTrade)
		.def("changeFreeTradeCount", &CyGame::changeFreeTradeCount)
		.def("getNoNukesCount", &CyGame::getNoNukesCount)
		.def("isNoNukes", &CyGame::isNoNukes)
		.def("changeNoNukesCount", &CyGame::changeNoNukesCount)
		.def("getSecretaryGeneralTimer", &CyGame::getSecretaryGeneralTimer)
		.def("getVoteTimer", &CyGame::getVoteTimer)
		.def("getNukesExploded", &CyGame::getNukesExploded)
		.def("changeNukesExploded", &CyGame::changeNukesExploded)

		.def("getMaxPopulation", &CyGame::getMaxPopulation)
		.def("getMaxLand", &CyGame::getMaxLand)
		.def("getMaxTech", &CyGame::getMaxTech)
		.def("getMaxWonders", &CyGame::getMaxWonders)
		.def("getInitPopulation", &CyGame::getInitPopulation)
		.def("getInitLand", &CyGame::getInitLand)
		.def("getInitTech", &CyGame::getInitTech)
		.def("getInitWonders", &CyGame::getInitWonders)

/************************************************************************************************/
/* REVOLUTION_MOD                                                                 lemmy101      */
/*                                                                                jdog5000      */
/*                                                                                              */
/************************************************************************************************/
		.def("getAIAutoPlay", &CyGame::getAIAutoPlay, "int (int iPlayer)")
		.def("setAIAutoPlay", &CyGame::setAIAutoPlay, "void (int iPlayer, int iValue)")
		.def("isForcedAIAutoPlay", &CyGame::isForcedAIAutoPlay, "bool (int iPlayer)")
		.def("getForcedAIAutoPlay", &CyGame::getForcedAIAutoPlay, "int (int iPlayer)")
		.def("setForcedAIAutoPlay", &CyGame::setForcedAIAutoPlay, "void (int iPlayer, int iValue, bool bForced)")
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/

		// < Water Animals Start >
		.def("getWaterAnimalSpawnChance", &CyGame::getWaterAnimalSpawnChance, "int ()")
		.def("setWaterAnimalSpawnChance", &CyGame::setWaterAnimalSpawnChance, "void (int)")
		.def("changeWaterAnimalSpawnChance", &CyGame::changeWaterAnimalSpawnChance, "void (int)")
		
		.def("getXResolution", &CyGame::getXResolution, "int ()")
		.def("setXResolution", &CyGame::setXResolution, "void (int)")
		.def("changeXResolution", &CyGame::changeXResolution, "void (int)")
		
		.def("getYResolution", &CyGame::getYResolution, "int ()")
		.def("setYResolution", &CyGame::setYResolution, "void (int)")
		.def("changeYResolution", &CyGame::changeYResolution, "void (int)")
		// < Water Animals End   >

		.def("isScoreDirty", &CyGame::isScoreDirty, "bool ()")
		.def("setScoreDirty", &CyGame::setScoreDirty)
		.def("isCircumnavigated", &CyGame::isCircumnavigated, "bool () - is the globe circumnavigated?")
		.def("isDiploVote", &CyGame::isDiploVote, "bool (VoteSourceTypes)")
		.def("changeDiploVote", &CyGame::changeDiploVote, "void (VoteSourceTypes, int)")
		.def("isDebugMode", &CyGame::isDebugMode, "bool () - is the game in Debug Mode?")
		.def("toggleDebugMode", &CyGame::toggleDebugMode)
/************************************************************************************************/
/* REVOLUTION_MOD                         03/18/09                                jdog5000      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
		.def("getChtLvl", &CyGame::getChtLvl, "int getChtLvl()")
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/


		.def("getPitbossTurnTime", &CyGame::getPitbossTurnTime, "int ()")
		.def("setPitbossTurnTime", &CyGame::setPitbossTurnTime, "void (int)")
		.def("isHotSeat", &CyGame::isHotSeat, "bool ()")
		.def("isPbem", &CyGame::isPbem, "bool ()")
		.def("isPitboss", &CyGame::isPitboss, "bool ()")
//PB Mod - START
		.def("isPitbossShortNames", &CyGame::isPitbossShortNames, "bool ()")
		.def("setPitbossShortNames", &CyGame::setPitbossShortNames, "void (bool bShort, int maxLenName, int maxLenDesc )")
//PB Mod - END		
		.def("isSimultaneousTeamTurns", &CyGame::isSimultaneousTeamTurns, "bool ()")

		.def("isFinalInitialized", &CyGame::isFinalInitialized, "bool () - Returns whether or not the game initialization process has ended (game has started)")

		.def("getActivePlayer", &CyGame::getActivePlayer, "returns index of the active player")
		.def("setActivePlayer", &CyGame::setActivePlayer, "void (int /*PlayerTypes*/ eNewValue, bool bForceHotSeat)")
		.def("getPausePlayer", &CyGame::getPausePlayer, "int () - will get who paused us")
//PB Mod - START		
		.def("setPausePlayer", &CyGame::setPausePlayer, "void (int /*PlayerTypes*/ eNewValue)")		
//PB Mod - END		
		.def("isPaused", &CyGame::isPaused, "bool () - will say if the game is paused")
		.def("getBestLandUnit", &CyGame::getBestLandUnit, "returns index of the best unit")
		.def("getBestLandUnitCombat", &CyGame::getBestLandUnitCombat, "int ()")
		.def("getWinner", &CyGame::getWinner)
		.def("getVictory", &CyGame::getVictory)
		.def("setWinner", &CyGame::setWinner)
		.def("getGameState", &CyGame::getGameState)
		.def("getHandicapType", &CyGame::getHandicapType, "HandicapType () - difficulty level settings")
		.def("getCalendar", &CyGame::getCalendar, "CalendarType ()")
		.def("getStartEra", &CyGame::getStartEra)
		.def("getGameSpeedType", &CyGame::getGameSpeedType)
		.def("getRankPlayer", &CyGame::getRankPlayer)
		.def("getPlayerRank", &CyGame::getPlayerRank)
		.def("getPlayerScore", &CyGame::getPlayerScore)
		.def("getRankTeam", &CyGame::getRankTeam)
		.def("getTeamRank", &CyGame::getTeamRank)
		.def("getTeamScore", &CyGame::getTeamScore)
		.def("isOption", &CyGame::isOption, "bool (eIndex) - returns whether Game Option is valid")
		.def("setOption", &CyGame::setOption, "void (GameOptionIndex, bEnabled) - sets a Game Option")
		.def("isMPOption", &CyGame::isMPOption, "bool (eIndex) - returns whether MP Option is valid")
		.def("isForcedControl", &CyGame::isForcedControl, "bool (eIndex) - returns whether Control should be forced")
		.def("getUnitCreatedCount", &CyGame::getUnitCreatedCount, "int (eIndex) - returns number of this unit type created (?)")
		.def("getUnitClassCreatedCount", &CyGame::getUnitClassCreatedCount, "int (eIndex) - returns number of this unit class type created (?)")
		.def("isUnitClassMaxedOut", &CyGame::isUnitClassMaxedOut, "bool (eIndex, iExtra) - returns whether or not this unit class is maxed out (e.g. spies)")
		.def("getBuildingClassCreatedCount", &CyGame::getBuildingClassCreatedCount, "int (BuildingClassType) - building Class count")
		.def("isBuildingClassMaxedOut", &CyGame::isBuildingClassMaxedOut, "bool (BuildingClassType) - max # reached?")

		.def("getProjectCreatedCount", &CyGame::getProjectCreatedCount, "int (ProjectTypes eIndex)")
		.def("isProjectMaxedOut", &CyGame::isProjectMaxedOut, "bool (ProjectTypes eIndex)")

		.def("getForceCivicCount", &CyGame::getForceCivicCount, "int (CivicTypes eIndex)")
		.def("isForceCivic", &CyGame::isForceCivic, "bool (CivicTypes eIndex)")
		.def("isForceCivicOption", &CyGame::isForceCivicOption, "bool (CivicOptionTypes eCivicOption)")

		.def("getVoteOutcome", &CyGame::getVoteOutcome, "int (VoteTypes eIndex)")

		.def("getReligionGameTurnFounded", &CyGame::getReligionGameTurnFounded)
		.def("isReligionFounded", &CyGame::isReligionFounded, "bool (ReligionID) - is religion founded?")
		.def("isReligionSlotTaken", &CyGame::isReligionSlotTaken, "bool (ReligionID) - is religion in that tech slot founded?")
/************************************************************************************************/
/* RevDCM	                  Start		 4/29/10                                                */
/*                                                                                              */
/* OC_LIMITED_RELIGIONS                                                                         */
/************************************************************************************************/
		.def("isGameStart", &CyGame::isGameStart, "bool ()")
		.def("countNumReligionsFounded", &CyGame::countNumReligionsFounded, "int ()")
		.def("countNumReligionTechsDiscovered", &CyGame::countNumReligionTechsDiscovered, "int ()")
		.def("isTechCanFoundReligion", &CyGame::isTechCanFoundReligion, "bool (TechID) - can this tech found a religion?")
/************************************************************************************************/
/* LIMITED_RELIGIONS               END                                                          */
/************************************************************************************************/
		.def("getCorporationGameTurnFounded", &CyGame::getCorporationGameTurnFounded)
		.def("isCorporationFounded", &CyGame::isCorporationFounded, "bool (CorporationID) - is corporation founded?")
		.def("isVictoryValid", &CyGame::isVictoryValid)
		.def("isVotePassed", &CyGame::isVotePassed)
		.def("isSpecialUnitValid", &CyGame::isSpecialUnitValid)
		.def("makeSpecialUnitValid", &CyGame::makeSpecialUnitValid)

		.def("isSpecialBuildingValid", &CyGame::isSpecialBuildingValid)
		.def("makeSpecialBuildingValid", &CyGame::makeSpecialBuildingValid)

		//TB Nukefix (Reversal) The following two lines should be commented out
		//.def("isNukesValid", &CyGame::isNukesValid, "bool")
		//.def("makeNukesValid", &CyGame::makeNukesValid, " void (bool bValid)")

		.def("isInAdvancedStart", &CyGame::isInAdvancedStart, "bool")

		.def("getHolyCity", &CyGame::getHolyCity, python::return_value_policy<python::manage_new_object>(), "CyCity getHolyCity()")
		.def("setHolyCity", &CyGame::setHolyCity, "void (int eIndex, CyCity *pNewValue, bAnnounce) - Sets holy city for religion eIndex to pNewValue")
		.def("clearHolyCity", &CyGame::clearHolyCity, "void (int eIndex) - clears the holy city for religion eIndex")

		.def("getHeadquarters", &CyGame::getHeadquarters, python::return_value_policy<python::manage_new_object>(), "CyCity getHeadquarters()")
		.def("setHeadquarters", &CyGame::setHeadquarters, "void (int eIndex, CyCity *pNewValue, bAnnounce) - Sets headquarters for corporation eIndex to pNewValue")
		.def("clearHeadquarters", &CyGame::clearHeadquarters, "void (int eIndex) - clears the headquarters for corporation eIndex")

		.def("getPlayerVote", &CyGame::getPlayerVote)

		.def("getScriptData", &CyGame::getScriptData, "str () - Returns ScriptData member (used to store custom data)")
		.def("setScriptData", &CyGame::setScriptData, "void (str) - Sets ScriptData member (used to store custom data)")

		.def("setName", &CyGame::setName)
		.def("getName", &CyGame::getName)
		.def("getIndexAfterLastDeal", &CyGame::getIndexAfterLastDeal)
		.def("getNumDeals", &CyGame::getNumDeals)
		.def("getDeal", &CyGame::getDeal, python::return_value_policy<python::manage_new_object>())
		.def("addDeal", &CyGame::addDeal, python::return_value_policy<python::manage_new_object>())
		.def("getMapRand", &CyGame::getMapRand, python::return_value_policy<python::reference_existing_object>())
		.def("getMapRandNum", &CyGame::getMapRandNum)
		.def("getSorenRand", &CyGame::getSorenRand, python::return_value_policy<python::reference_existing_object>())
		.def("getSorenRandNum", &CyGame::getSorenRandNum)
		.def("calculateSyncChecksum", &CyGame::calculateSyncChecksum)
		.def("calculateOptionsChecksum", &CyGame::calculateOptionsChecksum)

		.def("GetWorldBuilderMode", &CyGame::GetWorldBuilderMode)
		.def("isPitbossHost", &CyGame::isPitbossHost)
		.def("getCurrentLanguage", &CyGame::getCurrentLanguage)
		.def("setCurrentLanguage", &CyGame::setCurrentLanguage)

		.def("getReplayMessageTurn", &CyGame::getReplayMessageTurn)
		.def("getReplayMessageType", &CyGame::getReplayMessageType)
		.def("getReplayMessagePlotX", &CyGame::getReplayMessagePlotX)
		.def("getReplayMessagePlotY", &CyGame::getReplayMessagePlotY)
		.def("getReplayMessagePlayer", &CyGame::getReplayMessagePlayer)
		.def("getReplayMessageColor", &CyGame::getReplayMessageColor)
		.def("getReplayMessageText", &CyGame::getReplayMessageText)
		.def("getNumReplayMessages", &CyGame::getNumReplayMessages)
		.def("getReplayInfo", &CyGame::getReplayInfo, python::return_value_policy<python::manage_new_object>())
		.def("hasSkippedSaveChecksum", &CyGame::hasSkippedSaveChecksum)
		.def("saveReplay", &CyGame::saveReplay)
/************************************************************************************************/
/* REVOLUTION_MOD                                                                 lemmy101      */
/*                                                                                jdog5000      */
/*                                                                                              */
/************************************************************************************************/
		.def("addPlayer", &CyGame::addPlayer, "void (int eNewPlayer, int eLeader, int eCiv)")
		//.def("addPlayer", &CyGame::addPlayer, "void (int eNewPlayer, int eLeader, int eCiv, [bool bSetAlive = true]) - if bSetAlive = false new player isn't set to be alive and won't die if not given units or cities")
		.def("changeHumanPlayer", &CyGame::changeHumanPlayer, "void ( int /*PlayerTypes*/ eOldHuman, int /*PlayerTypes*/ eNewHuman )" )
		.def("addReplayMessage", &CyGame::addReplayMessage, "void (int /*ReplayMessageTypes*/ eType, int /*PlayerTypes*/ ePlayer, std::wstring pszText, int iPlotX, int iPlotY, int /*ColorTypes*/ eColor)" )
		.def("log", &CyGame::log, "void log(str)")
		.def("logw", &CyGame::logw, "void log(wstring str)")
/************************************************************************************************/
/* REVOLUTION_MOD                          END                                                  */
/************************************************************************************************/

/************************************************************************************************/
/* RevolutionDCM	                  Start		 05/31/10                        Afforess       */
/*                                                                                              */
/* Battle Effects                                                                               */
/************************************************************************************************/
		.def("drawBattleEffects", &CyGame::drawBattleEffects, "void ()")
/************************************************************************************************/
/* RevolutionDCM	             Battle Effects END                                             */
/************************************************************************************************/

		.def("getCultureThreshold", &CyGame::getCultureThreshold, "int getCultureThreshold(CultureLevelTypes eLevel)")

		.def("setPlotExtraYield", &CyGame::setPlotExtraYield, "void (int iX, int iY, int /*YieldTypes*/ eYield, int iExtraYield)")
		.def("changePlotExtraCost", &CyGame::changePlotExtraCost, "void (int iX, int iY, int iCost)")

		.def("isCivEverActive", &CyGame::isCivEverActive, "bool (int /*CivilizationTypes*/ eCivilization)")
		.def("isLeaderEverActive", &CyGame::isLeaderEverActive, "bool (int /*LeaderHeadTypes*/ eLeader)")
		.def("isUnitEverActive", &CyGame::isUnitEverActive, "bool (int /*UnitTypes*/ eUnit)")
		.def("isBuildingEverActive", &CyGame::isBuildingEverActive, "bool (int /*BuildingTypes*/ eBuilding)")

		.def("isEventActive", &CyGame::isEventActive, "bool (int /*EventTriggerTypes*/ eTrigger)")
		.def("doControl", &CyGame::doControl, "void (int /*ControlTypes*/ iControl)")
//PB Mod - START
		.def("setCivPassword", &CyGame::setCivPassword, "void (int /*PlayerId*/, string, string) - Allows change of passwords over webinterface")
//45deg: Pitboss Mod START
		.def("isDiploScreenUp", &CyGame::isDiploScreenUp, "bool ()")		
//45deg: Pitboss Mod END
//PB Mod - END		
// BUG - MapFinder - start
		.def("canRegenerateMap", &CyGame::canRegenerateMap, "bool ()")
		.def("regenerateMap", &CyGame::regenerateMap, "void ()")
		
		.def("saveGame", &CyGame::saveGame, "void saveGame(string filename)")
		.def("takeJPEGScreenShot", &CyGame::takeJPEGScreenShot, "bool takeJPEGScreenShot(string filename)")
// BUG - MapFinder - end

// BUG - EXE/DLL Paths - start
		.def("getDLLPath", &CyGame::getDLLPath, "string getDLLPath()")
		.def("getExePath", &CyGame::getExePath, "string getExePath()")
// BUG - EXE/DLL Paths - end
/************************************************************************************************/
/* Afforess	                  Start		 07/22/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
		.def("getStarshipLaunched", &CyGame::getStarshipLaunched, "bool ()")
		.def("getDiplomaticVictoryAchieved", &CyGame::getDiplomaticVictoryAchieved, "bool ()")
		.def("getAscensionGateBuilt", &CyGame::getAscensionGateBuilt, "bool ()") // code added for Ascension Gate & Mastery Victory (by 45�)
		.def("getCutLosersCounter", &CyGame::getCutLosersCounter)
		.def("getHighToLowCounter", &CyGame::getHighToLowCounter)
		.def("cheatCodesEnabled", &CyGame::cheatCodesEnabled)
		.def("setVictoryValid", &CyGame::setVictoryValid, "(int iVictoryType, bool bNewVal)")
		.def("isModderGameOption", &CyGame::isModderGameOption, "bool ()")
		.def("getModderGameOption", &CyGame::getModderGameOption, "bool ()")
		.def("setModderGameOption", &CyGame::setModderGameOption, "void ()")
		.def("setFutureEras", &CyGame::setFutureEras, "void ()")
		.def("canEverConstruct", &CyGame::canEverConstruct, "bool (int iBuilding)")
		.def("canEverTrain", &CyGame::canEverTrain, "bool (int iUnit)")
		.def("canEverSpread", &CyGame::canEverSpread, "bool (int iCorporation)")

		.def("isDarkAge", &CyGame::isDarkAge, "bool ()")
		.def("isDarkAgePending", &CyGame::isDarkAgePending, "bool ()")
		.def("triggerDarkAge", &CyGame::triggerDarkAge, "void (int iDelayLen, iLenTurns, std::wstring szStartTxtKey, std::wstring szEndTxtKey)")
		.def("getNumDarkAgesEverOccurred", &CyGame::getNumDarkAgesEverOccurred, "int ()")
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
		;

	python::class_<CyDeal>("CyDeal")
		.def("isNone", &CyDeal::isNone)
		.def("getID", &CyDeal::getID)
		.def("getInitialGameTurn", &CyDeal::getInitialGameTurn)
		.def("getFirstPlayer", &CyDeal::getFirstPlayer)
		.def("getSecondPlayer", &CyDeal::getSecondPlayer)
		.def("getLengthFirstTrades", &CyDeal::getLengthFirstTrades)
		.def("getLengthSecondTrades", &CyDeal::getLengthSecondTrades)
		.def("getFirstTrade", &CyDeal::getFirstTrade, python::return_value_policy<python::reference_existing_object>())
		.def("getSecondTrade", &CyDeal::getSecondTrade, python::return_value_policy<python::reference_existing_object>())
		.def("kill", &CyDeal::kill)

// BUG - Expose Deal Cancelability - start
		.def("isCancelable", &CyDeal::isCancelable, "bool isCancelable(int /*PlayerTypes*/ eByPlayer, bool bIgnoreWaitingPeriod)")
		.def("getCannotCancelReason", &CyDeal::getCannotCancelReason, "string getCannotCancelReason(int /*PlayerTypes*/ eByPlayer)")
		.def("turnsToCancel", &CyDeal::turnsToCancel, "int turnsToCancel(int /*PlayerTypes*/ eByPlayer)")
// BUG - Expose Deal Cancelability - end
		;
}
