# Sid Meier's Civilization 4
# Copyright Firaxis Games 2005
#
# CvRandomEventInterface.py
#
# These functions are App Entry Points from C++
# WARNING: These function names should not be changed
# WARNING: These functions can not be placed into a class
#
# No other modules should import this
#
import CvUtil
from CvPythonExtensions import *
from operator import itemgetter

gc = CyGlobalContext()
localText = CyTranslator()


######## BLESSED SEA ###########

def getHelpBlessedSea1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	map = gc.getMap()
	iOurMinLandmass = gc.getWorldInfo(map.getWorldSize()).getDefaultPlayers()
	
	szHelp = localText.getText("TXT_KEY_EVENT_BLESSED_SEA_HELP", (iOurMinLandmass, ))

	return szHelp

def canTriggerBlessedSea(argsList):
	kTriggeredData = argsList[0]
	map = gc.getMap()

	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_ONE_CITY_CHALLENGE) and gc.getPlayer(kTriggeredData.ePlayer).isHuman():
		return False

	iMapMinLandmass = 2 * gc.getWorldInfo(map.getWorldSize()).getDefaultPlayers()
	iOurMaxLandmass = gc.getWorldInfo(map.getWorldSize()).getDefaultPlayers() / 2

	if (map.getNumLandAreas() < iMapMinLandmass):
		return False

	iOurLandmasses = 0
	for i in range(map.getIndexAfterLastArea()):
		area = map.getArea(i)
		if not area.isNone() and not area.isWater() and area.getCitiesPerPlayer(kTriggeredData.ePlayer) > 0:
			iOurLandmasses += 1

	if (iOurLandmasses > iOurMaxLandmass):
		return False

	player = gc.getPlayer(kTriggeredData.ePlayer)
	if player.getUnitClassCount(gc.getInfoTypeForString("UNITCLASS_GALLEY")) == 0:
		if player.getUnitClassCount(gc.getInfoTypeForString("UNITCLASS_CARAVEL")) == 0:
			if player.getUnitClassCount(gc.getInfoTypeForString("UNITCLASS_GALLEON")) == 0:
				if player.getUnitClassCount(gc.getInfoTypeForString("UNITCLASS_WARGALLEY")) == 0:
					if player.getUnitClassCount(gc.getInfoTypeForString("UNITCLASS_COG")) == 0:
						if player.getUnitClassCount(gc.getInfoTypeForString("UNITCLASS_FLUYT")) == 0:
							if player.getUnitClassCount(gc.getInfoTypeForString("UNITCLASS_BRIGANTINE")) == 0:
								return False

	return True

def canTriggerBlessedSea2(argsList):

	kTriggeredData = argsList[0]
	map = gc.getMap()
	iOurMinLandmass = gc.getWorldInfo(map.getWorldSize()).getDefaultPlayers()
	
	iOurLandmasses = 0
	for i in range(map.getIndexAfterLastArea()):
		area = map.getArea(i)
		if not area.isNone() and not area.isWater() and area.getCitiesPerPlayer(kTriggeredData.ePlayer) > 0:
			iOurLandmasses += 1

	if (iOurLandmasses < iOurMinLandmass):
		return False

	return True

def applyBlessedSea2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	iBuilding = -1

	if (-1 != kTriggeredData.eReligion):
		for i in range(gc.getNumBuildingInfos()):
			if (gc.getBuildingInfo(i).getSpecialBuildingType() == gc.getInfoTypeForString("SPECIALBUILDING_TEMPLE")):
				if (gc.getBuildingInfo(i).getReligionType() == kTriggeredData.eReligion):
					iBuilding = i
					break


	if (iBuilding == -1):
		return

	player = gc.getPlayer(kTriggeredData.ePlayer)
	(loopCity, iter) = player.firstCity(False)

	while(loopCity):

		if (loopCity.getPopulation() >= 5):
			if (loopCity.canConstruct(iBuilding, False, False, True)):
				loopCity.setNumRealBuilding(iBuilding, 1)

		(loopCity, iter) = player.nextCity(iter, False)


def canApplyBlessedSea2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	iBuilding = -1

	if (-1 != kTriggeredData.eReligion):
		for i in range(gc.getNumBuildingInfos()):
			if (gc.getBuildingInfo(i).getSpecialBuildingType() == gc.getInfoTypeForString("SPECIALBUILDING_TEMPLE")):
				if (gc.getBuildingInfo(i).getReligionType() == kTriggeredData.eReligion):
					iBuilding = i
					break


	if (iBuilding == -1):
		return False

	player = gc.getPlayer(kTriggeredData.ePlayer)
	(loopCity, iter) = player.firstCity(False)
	bFound = False

	while(loopCity):

		if (loopCity.getPopulation() >= 5):
			if (loopCity.canConstruct(iBuilding, False, False, True)):
				bFound = True
				break

		(loopCity, iter) = player.nextCity(iter, False)

	return bFound


######## HOLY MOUNTAIN ###########

def getHelpHolyMountain1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	map = gc.getMap()
	iMinPoints = 2 * gc.getWorldInfo(map.getWorldSize()).getDefaultPlayers()

	iBuilding = -1
	iReligion = gc.getPlayer(kTriggeredData.ePlayer).getStateReligion()

	if (-1 != iReligion):
		for i in range(gc.getNumBuildingInfos()):
			#if (gc.getBuildingInfo(i).getSpecialBuildingType() == gc.getInfoTypeForString("SPECIALBUILDING_CATHEDRAL")):
			# Rise of Mankind start
			if (gc.getBuildingInfo(i).getSpecialBuildingType() == gc.getInfoTypeForString("SPECIALBUILDING_CATHEDRAL") or gc.getBuildingInfo(i).getSpecialBuildingType() == gc.getInfoTypeForString("SPECIALBUILDING_PANTHEON")):

				if (gc.getBuildingInfo(i).getReligionType() == iReligion):
					iBuilding = i
					break
			# Rise of Mankind end


		szHelp = localText.getText("TXT_KEY_EVENT_HOLY_MOUNTAIN_HELP", ( gc.getBuildingInfo(iBuilding).getTextKey(), gc.getBuildingInfo(iBuilding).getTextKey(), iMinPoints))

	return szHelp

def canTriggerHolyMountain(argsList):
	kTriggeredData = argsList[0]
	map = gc.getMap()

	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_ONE_CITY_CHALLENGE) and gc.getPlayer(kTriggeredData.ePlayer).isHuman():
		return False

	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)
	if (plot.getOwner() == -1):
		return True

	return False

def expireHolyMountain1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)
	if (plot == None):
		return True

	if (plot.getOwner() != kTriggeredData.ePlayer and plot.getOwner() != -1 and plot.getOwner() != gc.getBARBARIAN_PLAYER()):
		return True

	return False

def canTriggerHolyMountainDone(argsList):

	kTriggeredData = argsList[0]
	trigger = gc.getEventTriggerInfo(kTriggeredData.eTrigger)
	player = gc.getPlayer(kTriggeredData.ePlayer)

	kOrigTriggeredData = player.getEventOccured(trigger.getPrereqEvent(0))

	if (kOrigTriggeredData == None):
		return False

	plot = gc.getMap().plot(kOrigTriggeredData.iPlotX, kOrigTriggeredData.iPlotY)
	if (plot == None):
		return False

	if (plot.getOwner() != kTriggeredData.ePlayer):
		return False

	return True

def canTriggerHolyMountainRevealed(argsList):

	kTriggeredData = argsList[0]
	trigger = gc.getEventTriggerInfo(kTriggeredData.eTrigger)
	player = gc.getPlayer(kTriggeredData.ePlayer)

	kOrigTriggeredData = player.getEventOccured(trigger.getPrereqEvent(0))

	if (kOrigTriggeredData == None):
		return False

	iNumPoints = 0

	for i in range(gc.getNumBuildingInfos()):
		if (gc.getBuildingInfo(i).getReligionType() == kOrigTriggeredData.eReligion):
			if (gc.getBuildingInfo(i).getSpecialBuildingType() == gc.getInfoTypeForString("SPECIALBUILDING_CATHEDRAL")):
				iNumPoints += 4 * player.countNumBuildings(i)
			# Rise of mankind start
			elif (gc.getBuildingInfo(i).getSpecialBuildingType() == gc.getInfoTypeForString("SPECIALBUILDING_PANTHEON")):
				iNumPoints += 4 * player.countNumBuildings(i)
			# Rise of Mankind end
			elif (gc.getBuildingInfo(i).getSpecialBuildingType() == gc.getInfoTypeForString("SPECIALBUILDING_TEMPLE")):
				iNumPoints += player.countNumBuildings(i)
			elif (gc.getBuildingInfo(i).getSpecialBuildingType() == gc.getInfoTypeForString("SPECIALBUILDING_MONASTERY")):
				iNumPoints += player.countNumBuildings(i)

	if (iNumPoints < 2 * gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers()):
		return False

	plot = gc.getMap().plot(kOrigTriggeredData.iPlotX, kOrigTriggeredData.iPlotY)
	if (plot == None):
		return False

	plot.setRevealed(player.getTeam(), True, True, -1)

	kActualTriggeredDataObject = player.getEventTriggered(kTriggeredData.iId)
	kActualTriggeredDataObject.iPlotX = kOrigTriggeredData.iPlotX
	kActualTriggeredDataObject.iPlotY = kOrigTriggeredData.iPlotY

	return True

def doHolyMountainRevealed(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	if (kTriggeredData.ePlayer == gc.getGame().getActivePlayer()):
		CyCamera().JustLookAtPlot( CyMap().plot( kTriggeredData.iPlotX, kTriggeredData.iPlotY ) )

	return 1

######## MARATHON ###########

def canTriggerMarathon(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	otherPlayer = gc.getPlayer(kTriggeredData.eOtherPlayer)
	team = gc.getTeam(player.getTeam())

	if (team.AI_getAtWarCounter(otherPlayer.getTeam()) == 1):
		(loopUnit, iter) = otherPlayer.firstUnit(False)
		while( loopUnit ):
			plot = loopUnit.plot()
			if (not plot.isNone()):
				if (plot.getOwner() == kTriggeredData.ePlayer):
					return True
			(loopUnit, iter) = otherPlayer.nextUnit(iter, False)

	return False

######## WEDDING FEUD ###########

def doWeddingFeud2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	player = gc.getPlayer(kTriggeredData.ePlayer)
	(loopCity, iter) = player.firstCity(False)

	while(loopCity):
		if loopCity.isHasReligion(kTriggeredData.eReligion):
			loopCity.changeHappinessTimer(30)
		(loopCity, iter) = player.nextCity(iter, False)

	return 1

def getHelpWeddingFeud2(argsList):
	iEvent = argsList[0]
	event = gc.getEventInfo(iEvent)
	kTriggeredData = argsList[1]
	religion = gc.getReligionInfo(kTriggeredData.eReligion)

	szHelp = localText.getText("TXT_KEY_EVENT_WEDDING_FEUD_2_HELP", (gc.getDefineINT("TEMP_HAPPY"), 30, religion.getChar()))

	return szHelp

def canDoWeddingFeud3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	player = gc.getPlayer(kTriggeredData.ePlayer)

	if player.getGold() - 10 * player.getNumCities() < 0:
		return False

	return True


def doWeddingFeud3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	destPlayer = gc.getPlayer(kTriggeredData.eOtherPlayer)
	player = gc.getPlayer(kTriggeredData.ePlayer)

	for iLoopPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iLoopPlayer)
		if loopPlayer.isAlive() and loopPlayer.getStateReligion() == player.getStateReligion():
			loopPlayer.AI_changeAttitudeExtra(kTriggeredData.ePlayer, 1)
			player.AI_changeAttitudeExtra(iLoopPlayer, 1)

	if gc.getTeam(destPlayer.getTeam()).canDeclareWar(player.getTeam()):
		if destPlayer.isHuman():
			# this works only because it's a single-player only event
			popupInfo = CyPopupInfo()
			popupInfo.setButtonPopupType(ButtonPopupTypes.BUTTONPOPUP_PYTHON)
			popupInfo.setText(localText.getText("TXT_KEY_EVENT_WEDDING_FEUD_OTHER_3", (gc.getReligionInfo(kTriggeredData.eReligion).getAdjectiveKey(), player.getCivilizationShortDescriptionKey())))
			popupInfo.setData1(kTriggeredData.eOtherPlayer)
			popupInfo.setData2(kTriggeredData.ePlayer)
			popupInfo.setPythonModule("CvRandomEventInterface")
			popupInfo.setOnClickedPythonCallback("weddingFeud3Callback")
			popupInfo.addPythonButton(localText.getText("TXT_KEY_POPUP_YES", ()), "")
			popupInfo.addPythonButton(localText.getText("TXT_KEY_POPUP_NO", ()), "")
			popupInfo.addPopup(kTriggeredData.eOtherPlayer)
		else:
			gc.getTeam(destPlayer.getTeam()).declareWar(player.getTeam(), False, WarPlanTypes.WARPLAN_LIMITED)

	return 1


def weddingFeud3Callback(argsList):
	iButton = argsList[0]
	iData1 = argsList[1]
	iData2 = argsList[2]
	iData3 = argsList[3]
	szText = argsList[4]
	bOption1 = argsList[5]
	bOption2 = argsList[6]

	if iButton == 0:
		destPlayer = gc.getPlayer(iData1)
		player = gc.getPlayer(iData2)
		gc.getTeam(destPlayer.getTeam()).declareWar(player.getTeam(), False, WarPlanTypes.WARPLAN_LIMITED)

	return 0

def getHelpWeddingFeud3(argsList):
	iEvent = argsList[0]
	event = gc.getEventInfo(iEvent)
	kTriggeredData = argsList[1]
	religion = gc.getReligionInfo(kTriggeredData.eReligion)

	szHelp = localText.getText("TXT_KEY_EVENT_WEDDING_FEUD_3_HELP", (1, religion.getChar()))

	return szHelp

######## SPICY ###########

def canTriggerSpicy(argsList):

	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iSpice = gc.getInfoTypeForString("BONUS_SPICES")
	iHappyBonuses = 0
	bSpices = False
	for i in range(gc.getNumBonusInfos()):
		bonus = gc.getBonusInfo(i)
		iNum = player.getNumAvailableBonuses(i)
		if iNum > 0 :
			if bonus.getHappiness() > 0:
				iHappyBonuses += 1
				if iHappyBonuses > 4:
					return False
			if i == iSpice:
				return False

	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)
	if not plot.canHaveBonus(iSpice, False):
		return False

	return True

def doSpicy2(argsList):
#	need this because plantations are notmally not allowed unless there are already spices
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)

	if not plot.isNone():
		plot.setImprovementType(gc.getInfoTypeForString("IMPROVEMENT_PLANTATION"))

	return 1

def getHelpSpicy2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	iPlantation = gc.getInfoTypeForString("IMPROVEMENT_PLANTATION")
	szHelp = localText.getText("TXT_KEY_EVENT_IMPROVEMENT_GROWTH", ( gc.getImprovementInfo(iPlantation).getTextKey(), ))

	return szHelp

######## BABY BOOM ###########

def canTriggerBabyBoom(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	team = gc.getTeam(player.getTeam())

	if team.getAtWarCount(True) > 0:
		return False

	for iLoopTeam in range(gc.getMAX_CIV_TEAMS()):
		if iLoopTeam != player.getTeam():
			if team.AI_getAtPeaceCounter(iLoopTeam) == 1:
				return True

	return False

######## BARD TALE ###########

def applyBardTale3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	player = gc.getPlayer(kTriggeredData.ePlayer)

	player.changeGold(-10 * player.getNumCities())

def canApplyBardTale3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	player = gc.getPlayer(kTriggeredData.ePlayer)

	if player.getGold() - 10 * player.getNumCities() < 0:
		return False

	return True


def getHelpBardTale3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	player = gc.getPlayer(kTriggeredData.ePlayer)

	szHelp = localText.getText("TXT_KEY_EVENT_GOLD_LOST", (10 * player.getNumCities(), ))

	return szHelp

######## LOOTERS ###########

def getHelpLooters3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	otherPlayer = gc.getPlayer(kTriggeredData.eOtherPlayer)
	city = otherPlayer.getCity(kTriggeredData.iOtherPlayerCityId)

	szHelp = localText.getText("TXT_KEY_EVENT_LOOTERS_3_HELP", (1, 2, city.getNameKey()))

	return szHelp

def canApplyLooters3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	otherPlayer = gc.getPlayer(kTriggeredData.eOtherPlayer)
	city = otherPlayer.getCity(kTriggeredData.iOtherPlayerCityId)

	iNumBuildings = 0
	for iBuilding in range(gc.getNumBuildingInfos()):
		if (city.getNumRealBuilding(iBuilding) > 0 and gc.getBuildingInfo(iBuilding).getProductionCost() <= 100 and gc.getBuildingInfo(iBuilding).getProductionCost() > 0  and not isLimitedWonderClass(gc.getBuildingInfo(iBuilding).getBuildingClassType())):
			iNumBuildings += 1

	return (iNumBuildings > 0)


def applyLooters3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	otherPlayer = gc.getPlayer(kTriggeredData.eOtherPlayer)
	city = otherPlayer.getCity(kTriggeredData.iOtherPlayerCityId)

	iNumBuildings = gc.getGame().getSorenRandNum(2, "Looters event number of buildings destroyed")
	iNumBuildingsDestroyed = 0

	listBuildings = []
	for iBuilding in range(gc.getNumBuildingInfos()):
		if (city.getNumRealBuilding(iBuilding) > 0 and gc.getBuildingInfo(iBuilding).getProductionCost() <= 100 and gc.getBuildingInfo(iBuilding).getProductionCost() > 0  and not isLimitedWonderClass(gc.getBuildingInfo(iBuilding).getBuildingClassType())):
			listBuildings.append(iBuilding)

	for i in range(iNumBuildings+1):
		if len(listBuildings) > 0:
			iBuilding = listBuildings[gc.getGame().getSorenRandNum(len(listBuildings), "Looters event building destroyed")]
			szBuffer = localText.getText("TXT_KEY_EVENT_CITY_IMPROVEMENT_DESTROYED", (gc.getBuildingInfo(iBuilding).getTextKey(), ))
			CyInterface().addMessage(kTriggeredData.eOtherPlayer, False, gc.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_BOMBARDED", InterfaceMessageTypes.MESSAGE_TYPE_INFO, gc.getBuildingInfo(iBuilding).getButton(), gc.getInfoTypeForString("COLOR_RED"), city.getX(), city.getY(), True, True)
			city.setNumRealBuilding(iBuilding, 0)
			iNumBuildingsDestroyed += 1
			listBuildings.remove(iBuilding)

	if iNumBuildingsDestroyed > 0:
		szBuffer = localText.getText("TXT_KEY_EVENT_NUM_BUILDINGS_DESTROYED", (iNumBuildingsDestroyed, gc.getPlayer(kTriggeredData.eOtherPlayer).getCivilizationAdjectiveKey(), city.getNameKey()))
		CyInterface().addMessage(kTriggeredData.ePlayer, False, gc.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_BOMBARDED", InterfaceMessageTypes.MESSAGE_TYPE_INFO, None, gc.getInfoTypeForString("COLOR_WHITE"), -1, -1, True, True)

######## BROTHERS IN NEED ###########

def canTriggerBrothersInNeed(argsList):
	kTriggeredData = argsList[0]
	trigger = gc.getEventTriggerInfo(kTriggeredData.eTrigger)
	player = gc.getPlayer(kTriggeredData.ePlayer)
	otherPlayer = gc.getPlayer(kTriggeredData.eOtherPlayer)

	if not player.canTradeNetworkWith(kTriggeredData.eOtherPlayer):
		return False

	listResources = []
	listResources.append(gc.getInfoTypeForString("BONUS_COPPER"))
	listResources.append(gc.getInfoTypeForString("BONUS_IRON"))
	listResources.append(gc.getInfoTypeForString("BONUS_HORSE"))
	listResources.append(gc.getInfoTypeForString("BONUS_IVORY"))
	listResources.append(gc.getInfoTypeForString("BONUS_OIL"))
	listResources.append(gc.getInfoTypeForString("BONUS_URANIUM"))

	bFound = False
	for iResource in listResources:
		if (player.getNumTradeableBonuses(iResource) > 1 and otherPlayer.getNumAvailableBonuses(iResource) <= 0):
			bFound = True
			break

	if not bFound:
		return False

	for iTeam in range(gc.getMAX_CIV_TEAMS()):
		if iTeam != player.getTeam() and iTeam != otherPlayer.getTeam() and gc.getTeam(iTeam).isAlive():
			if gc.getTeam(iTeam).isAtWar(otherPlayer.getTeam()) and not gc.getTeam(iTeam).isAtWar(player.getTeam()):
				return True

	return False

def canDoBrothersInNeed1(argsList):
	kTriggeredData = argsList[1]
	newArgs = (kTriggeredData, )

	return canTriggerBrothersInNeed(newArgs)

######## HURRICANE ###########

def canTriggerHurricaneCity(argsList):
	eTrigger = argsList[0]
	ePlayer = argsList[1]
	iCity = argsList[2]

	player = gc.getPlayer(ePlayer)
	city = player.getCity(iCity)

	if city.isNone():
		return False

	if not city.isCoastal(gc.getMIN_WATER_SIZE_FOR_OCEAN()):
		return False

	if city.plot().getLatitude() <= 0:
		return False

	if city.getPopulation() < 2:
		return False

	return True

def applyHurricane1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	player = gc.getPlayer(kTriggeredData.ePlayer)
	city = player.getCity(kTriggeredData.iCityId)

	listCheapBuildings = []
	listExpensiveBuildings = []
	for iBuilding in range(gc.getNumBuildingInfos()):
		if (city.getNumRealBuilding(iBuilding) > 0 and gc.getBuildingInfo(iBuilding).getProductionCost() <= 100 and gc.getBuildingInfo(iBuilding).getProductionCost() > 0 and not isLimitedWonderClass(gc.getBuildingInfo(iBuilding).getBuildingClassType())):
			listCheapBuildings.append(iBuilding)
		if (city.getNumRealBuilding(iBuilding) > 0 and gc.getBuildingInfo(iBuilding).getProductionCost() > 100 and not isLimitedWonderClass(gc.getBuildingInfo(iBuilding).getBuildingClassType())):
			listExpensiveBuildings.append(iBuilding)

	if len(listCheapBuildings) > 0:
		iBuilding = listCheapBuildings[gc.getGame().getSorenRandNum(len(listCheapBuildings), "Hurricane event cheap building destroyed")]
		szBuffer = localText.getText("TXT_KEY_EVENT_CITY_IMPROVEMENT_DESTROYED", (gc.getBuildingInfo(iBuilding).getTextKey(), ))
		CyInterface().addMessage(kTriggeredData.ePlayer, False, gc.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_BOMBARDED", InterfaceMessageTypes.MESSAGE_TYPE_INFO, gc.getBuildingInfo(iBuilding).getButton(), gc.getInfoTypeForString("COLOR_RED"), city.getX(), city.getY(), True, True)
		city.setNumRealBuilding(iBuilding, 0)

	if len(listExpensiveBuildings) > 0:
		iBuilding = listExpensiveBuildings[gc.getGame().getSorenRandNum(len(listExpensiveBuildings), "Hurricane event expensive building destroyed")]
		szBuffer = localText.getText("TXT_KEY_EVENT_CITY_IMPROVEMENT_DESTROYED", (gc.getBuildingInfo(iBuilding).getTextKey(), ))
		CyInterface().addMessage(kTriggeredData.ePlayer, False, gc.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_BOMBARDED", InterfaceMessageTypes.MESSAGE_TYPE_INFO, gc.getBuildingInfo(iBuilding).getButton(), gc.getInfoTypeForString("COLOR_RED"), city.getX(), city.getY(), True, True)
		city.setNumRealBuilding(iBuilding, 0)

	if len(listCheapBuildings) == 0 and len(listExpensiveBuildings) == 0:
		city.changePopulation(-1)

def getHelpHurricane(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	player = gc.getPlayer(kTriggeredData.ePlayer)
	city = player.getCity(kTriggeredData.iCityId)
	szHelp = localText.getText("TXT_KEY_EVENT_HURRICANE_2_HELP", ())

	for iBuilding in range(gc.getNumBuildingInfos()):
		if (city.getNumRealBuilding(iBuilding) > 0 and gc.getBuildingInfo(iBuilding).getProductionCost() > 0 and not isLimitedWonderClass(gc.getBuildingInfo(iBuilding).getBuildingClassType())):
			szHelp = localText.getText("TXT_KEY_EVENT_HURRICANE_1_HELP", ())
				
	return szHelp


######## CYCLONE ###########

def canTriggerCycloneCity(argsList):
	eTrigger = argsList[0]
	ePlayer = argsList[1]
	iCity = argsList[2]

	player = gc.getPlayer(ePlayer)
	city = player.getCity(iCity)

	if city.isNone():
		return False

	if not city.isCoastal(gc.getMIN_WATER_SIZE_FOR_OCEAN()):
		return False

	if city.plot().getLatitude() >= 0:
		return False

	return True

######## TSUNAMI ###########

def canTriggerTsunamiCity(argsList):
	eTrigger = argsList[0]
	ePlayer = argsList[1]
	iCity = argsList[2]

	player = gc.getPlayer(ePlayer)
	city = player.getCity(iCity)

	if city.isNone():
		return False

	if not city.isCoastal(gc.getMIN_WATER_SIZE_FOR_OCEAN()):
		return False

	return True

def canApplyTsunami1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	player = gc.getPlayer(kTriggeredData.ePlayer)
	city = player.getCity(kTriggeredData.iCityId)

	return (city.getPopulation() < 6)

def canApplyTsunami2(argsList):
	return (not canApplyTsunami1(argsList))


def applyTsunami1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	player = gc.getPlayer(kTriggeredData.ePlayer)
	city = player.getCity(kTriggeredData.iCityId)

	city.kill()

def applyTsunami2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	player = gc.getPlayer(kTriggeredData.ePlayer)
	city = player.getCity(kTriggeredData.iCityId)

	listBuildings = []
	for iBuilding in range(gc.getNumBuildingInfos()):
		if (city.getNumRealBuilding(iBuilding) > 0 and gc.getBuildingInfo(iBuilding).getProductionCost() > 0  and not isLimitedWonderClass(gc.getBuildingInfo(iBuilding).getBuildingClassType())):
			listBuildings.append(iBuilding)

	for i in range(5):
		if len(listBuildings) > 0:
			iBuilding = listBuildings[gc.getGame().getSorenRandNum(len(listBuildings), "Tsunami event building destroyed")]
			szBuffer = localText.getText("TXT_KEY_EVENT_CITY_IMPROVEMENT_DESTROYED", (gc.getBuildingInfo(iBuilding).getTextKey(), ))
			CyInterface().addMessage(kTriggeredData.ePlayer, False, gc.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_BOMBARDED", InterfaceMessageTypes.MESSAGE_TYPE_INFO, gc.getBuildingInfo(iBuilding).getButton(), gc.getInfoTypeForString("COLOR_RED"), city.getX(), city.getY(), True, True)
			city.setNumRealBuilding(iBuilding, 0)
			listBuildings.remove(iBuilding)


def getHelpTsunami2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	city = player.getCity(kTriggeredData.iCityId)

	szHelp = localText.getText("TXT_KEY_EVENT_TSUNAMI_2_HELP", (5, city.getNameKey()))

	return szHelp


######## MONSOON ###########

def canTriggerMonsoonCity(argsList):
	eTrigger = argsList[0]
	ePlayer = argsList[1]
	iCity = argsList[2]

	player = gc.getPlayer(ePlayer)
	city = player.getCity(iCity)

	if city.isNone():
		return False

	if city.isCoastal(gc.getMIN_WATER_SIZE_FOR_OCEAN()):
		return False

	iJungleType = gc.getInfoTypeForString("FEATURE_JUNGLE")

	for iDX in range(-3, 4):
		for iDY in range(-3, 4):
			pLoopPlot = plotXY(city.getX(), city.getY(), iDX, iDY)
			if not pLoopPlot.isNone() and pLoopPlot.getFeatureType() == iJungleType:
				return True

	return False

######## VOLCANO ###########

def getHelpVolcano1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_VOLCANO_1_HELP", ())

	return szHelp

def canApplyVolcano1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	iNumImprovements = 0
	for iDX in range(-1, 2):
		for iDY in range(-1, 2):
			loopPlot = plotXY(kTriggeredData.iPlotX, kTriggeredData.iPlotY, iDX, iDY)
			if not loopPlot.isNone():
				if (iDX != 0 or iDY != 0):
					if loopPlot.getImprovementType() != -1:
						iNumImprovements += 1

	return (iNumImprovements > 0)

def applyVolcano1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	listPlots = []
	for iDX in range(-1, 2):
		for iDY in range(-1, 2):
			loopPlot = plotXY(kTriggeredData.iPlotX, kTriggeredData.iPlotY, iDX, iDY)
			if not loopPlot.isNone():
				if (iDX != 0 or iDY != 0):
					if loopPlot.getImprovementType() != -1:
						listPlots.append(loopPlot)

	listRuins = []
	listRuins.append(gc.getInfoTypeForString("IMPROVEMENT_COTTAGE"))
	listRuins.append(gc.getInfoTypeForString("IMPROVEMENT_HAMLET"))
	listRuins.append(gc.getInfoTypeForString("IMPROVEMENT_VILLAGE"))
	listRuins.append(gc.getInfoTypeForString("IMPROVEMENT_TOWN"))

	iRuins = gc.getInfoTypeForString("IMPROVEMENT_CITY_RUINS")

	for i in range(3):
		if len(listPlots) > 0:
			plot = listPlots[gc.getGame().getSorenRandNum(len(listPlots), "Volcano event improvement destroyed")]
			iImprovement = plot.getImprovementType()
			szBuffer = localText.getText("TXT_KEY_EVENT_CITY_IMPROVEMENT_DESTROYED", (gc.getImprovementInfo(iImprovement).getTextKey(), ))
			CyInterface().addMessage(kTriggeredData.ePlayer, False, gc.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_BOMBARDED", InterfaceMessageTypes.MESSAGE_TYPE_INFO, gc.getImprovementInfo(iImprovement).getButton(), gc.getInfoTypeForString("COLOR_RED"), plot.getX(), plot.getY(), True, True)
			if iImprovement in listRuins:
				plot.setImprovementType(iRuins)
			else:
				plot.setImprovementType(-1)
			listPlots.remove(plot)

			if i == 1 and gc.getGame().getSorenRandNum(100, "Volcano event num improvements destroyed") < 50:
				break
				
	import BugUtil
	
	iClassical = gc.getInfoTypeForString("ERA_CLASSICAL")
	
	if (not gc.getGame().isNetworkMultiPlayer() and gc.getGame().getHighestEra() > iClassical and canTriggerNewEvent(argsList)):

		iBaseOdds = 200
		iBaseOdds /= (1 + gc.getGame().getNumDarkAgesEverOccurred())

		if (gc.getGame().getSorenRandNum(10000, "Volcano Dark Age Chance") < iBaseOdds):
			delay = 4 + gc.getGame().getSorenRandNum(8, "Volcano Strike Dark Age Delay")
			lenTurns = 12 + gc.getGame().getSorenRandNum(15, "Volcano Strike Dark Age Delay")

			BugUtil.debug("unscaled delay: %d, unscaled lenTurns: %d", delay, lenTurns)

			delay *= gc.getGameSpeedInfo(gc.getGame().getGameSpeedType()).getGrowthPercent()
			delay /= 100

			lenTurns *= gc.getGameSpeedInfo(gc.getGame().getGameSpeedType()).getGrowthPercent()
			lenTurns /= 100

			if (lenTurns > 60): lenTurns = 60
			if (delay > 20): delay = 20

			BugUtil.debug("scaled delay: %d, scaled lenTurns: %d", delay, lenTurns)

			gc.getGame().triggerDarkAge(delay, lenTurns, "TXT_KEY_EVENT_DARK_AGE_VOLCANO_BEGINS", "TXT_KEY_EVENT_DARK_AGE_VOLCANO_ENDS")


######## DUSTBOWL ###########

def canTriggerDustbowlCont(argsList):
	kTriggeredData = argsList[0]

	trigger = gc.getEventTriggerInfo(kTriggeredData.eTrigger)
	player = gc.getPlayer(kTriggeredData.ePlayer)

	kOrigTriggeredData = player.getEventOccured(trigger.getPrereqEvent(0))

	if (kOrigTriggeredData == None):
		return False

	iFarmType = gc.getInfoTypeForString("IMPROVEMENT_FARM")
	iPlainsType = gc.getInfoTypeForString("TERRAIN_PLAINS")

	map = gc.getMap()
	iBestValue = map.getGridWidth() + map.getGridHeight()
	bestPlot = None
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == kTriggeredData.ePlayer and plot.getImprovementType() == iFarmType and plot.getTerrainType() == iPlainsType):
			iValue = plotDistance(kOrigTriggeredData.iPlotX, kOrigTriggeredData.iPlotY, plot.getX(), plot.getY())
			if iValue < iBestValue:
				iBestValue = iValue
				bestPlot = plot

	if bestPlot != None:
		kActualTriggeredDataObject = player.getEventTriggered(kTriggeredData.iId)
		kActualTriggeredDataObject.iPlotX = bestPlot.getX()
		kActualTriggeredDataObject.iPlotY = bestPlot.getY()
	else:
		player.resetEventOccured(trigger.getPrereqEvent(0))
		return False

	return True

def getHelpDustBowl2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_DUSTBOWL_2_HELP", ())

	return szHelp

######## SALTPETER ###########

def getSaltpeterNumExtraPlots():
	map = gc.getMap()
	if map.getWorldSize() <= 1:
		return 1
	elif map.getWorldSize() <= 3:
		return 2
	elif map.getWorldSize() <= 4:
		return 3
	else:
		return 4

def getHelpSaltpeter(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_SALTPETER_HELP", (getSaltpeterNumExtraPlots(), ))

	return szHelp

def canApplySaltpeter(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	map = gc.getMap()

	player = gc.getPlayer(kTriggeredData.ePlayer)

	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)
	if (plot == None):
		return False

	iForest = gc.getInfoTypeForString("FEATURE_FOREST")

	iNumPlots = 0
	for i in range(map.numPlots()):
		loopPlot = map.plotByIndex(i)
		if (loopPlot.getOwner() == kTriggeredData.ePlayer and loopPlot.getFeatureType() == iForest and loopPlot.isHills()):
			iDistance = plotDistance(kTriggeredData.iPlotX, kTriggeredData.iPlotY, loopPlot.getX(), loopPlot.getY())
			if iDistance > 0:
				iNumPlots += 1

	return (iNumPlots >= getSaltpeterNumExtraPlots())

def applySaltpeter(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	map = gc.getMap()

	player = gc.getPlayer(kTriggeredData.ePlayer)

	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)
	if (plot == None):
		return

	iForest = gc.getInfoTypeForString("FEATURE_FOREST")

	listPlots = []
	for i in range(map.numPlots()):
		loopPlot = map.plotByIndex(i)
		if (loopPlot.getOwner() == kTriggeredData.ePlayer and loopPlot.getFeatureType() == iForest and loopPlot.isHills()):
			iDistance = plotDistance(kTriggeredData.iPlotX, kTriggeredData.iPlotY, loopPlot.getX(), loopPlot.getY())
			if iDistance > 0:
				listPlots.append((iDistance, loopPlot))

	listPlots.sort(key=itemgetter(0))

	iCount = getSaltpeterNumExtraPlots()
	for loopPlot in listPlots:
		if iCount == 0:
			break
		iCount -= 1
		gc.getGame().setPlotExtraYield(loopPlot[1].getX(), loopPlot[1].getY(), YieldTypes.YIELD_COMMERCE, 1)
		CyInterface().addMessage(kTriggeredData.ePlayer, False, gc.getEVENT_MESSAGE_TIME(), localText.getText("TXT_KEY_EVENT_SALTPETER_DISCOVERED", ()), "", InterfaceMessageTypes.MESSAGE_TYPE_INFO, None, gc.getInfoTypeForString("COLOR_WHITE"), loopPlot[1].getX(), loopPlot[1].getY(), True, True)

######## GREAT DEPRESSION ###########

def applyGreatDepression(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	player = gc.getPlayer(kTriggeredData.ePlayer)
	corporation = gc.getCorporationInfo(kTriggeredData.eCorporation)

	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			loopPlayer.changeGold(-loopPlayer.getGold()/4)

			if iPlayer != kTriggeredData.ePlayer:
				szText = localText.getText("TXT_KEY_EVENTTRIGGER_GREAT_DEPRESSION", (player.getCivilizationAdjectiveKey(), u"", u"", u"", u"", corporation.getTextKey()))
				szText += u"\n\n" + localText.getText("TXT_KEY_EVENT_GREAT_DEPRESSION_HELP", (25, ))
				popupInfo = CyPopupInfo()
				popupInfo.setText(szText)
				popupInfo.addPopup(iPlayer)


def getHelpGreatDepression(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_GREAT_DEPRESSION_HELP", (25, ))

	return szHelp

######## CHAMPION ###########

def canTriggerChampion(argsList):
	kTriggeredData = argsList[0]

	player = gc.getPlayer(kTriggeredData.ePlayer)
	team = gc.getTeam(player.getTeam())

	if team.getAtWarCount(True) > 0:
		return False

	return True

def canTriggerChampionUnit(argsList):
	eTrigger = argsList[0]
	ePlayer = argsList[1]
	iUnit = argsList[2]

	player = gc.getPlayer(ePlayer)
	unit = player.getUnit(iUnit)

	if unit.isNone():
		return False

	if unit.getDamage() > 0:
		return False

	if unit.getExperience() < 3:
		return False

	iLeadership = gc.getInfoTypeForString("PROMOTION_LEADERSHIP")
	if unit.isHasPromotion(iLeadership):
		return False

	return True

def applyChampion(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	player = gc.getPlayer(kTriggeredData.ePlayer)
	unit = player.getUnit(kTriggeredData.iUnitId)

	iLeadership = gc.getInfoTypeForString("PROMOTION_LEADERSHIP")

	unit.setHasPromotion(iLeadership, True)

def getHelpChampion(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	player = gc.getPlayer(kTriggeredData.ePlayer)
	unit = player.getUnit(kTriggeredData.iUnitId)

	iLeadership = gc.getInfoTypeForString("PROMOTION_LEADERSHIP")

	szHelp = localText.getText("TXT_KEY_EVENT_CHAMPION_HELP", (unit.getNameKey(), gc.getPromotionInfo(iLeadership).getTextKey()))

	return szHelp

######## ELECTRIC COMPANY ###########

def canTriggerElectricCompany(argsList):
	kTriggeredData = argsList[0]

	player = gc.getPlayer(kTriggeredData.ePlayer)

	player = gc.getPlayer(kTriggeredData.ePlayer)

	(loopCity, iter) = player.firstCity(False)

	while(loopCity):

		if (loopCity.angryPopulation(0) > 0):
			return False

		(loopCity, iter) = player.nextCity(iter, False)

	return True

######## GOLD RUSH ###########

def canTriggerGoldRush(argsList):
	kTriggeredData = argsList[0]

	player = gc.getPlayer(kTriggeredData.ePlayer)

	iIndustrial = gc.getInfoTypeForString("ERA_INDUSTRIAL")

	if player.getCurrentEra() != iIndustrial:
		return False


	return True

######## INFLUENZA ###########

def canTriggerInfluenza(argsList):
	kTriggeredData = argsList[0]

	player = gc.getPlayer(kTriggeredData.ePlayer)
	team = gc.getTeam(player.getTeam())

	iIndustrial = gc.getInfoTypeForString("ERA_INDUSTRIAL")

	if player.getCurrentEra() <= iIndustrial:
		return False

	iMedicine = gc.getInfoTypeForString("TECH_MEDICINE")

	if team.isHasTech(iMedicine):
		return False

	return True

def applyInfluenza2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	player = gc.getPlayer(kTriggeredData.ePlayer)
	eventCity = player.getCity(kTriggeredData.iCityId)

	iNumCities = 2 + gc.getGame().getSorenRandNum(3, "Influenza event number of cities")

	listCities = []
	(loopCity, iter) = player.firstCity(False)
	while(loopCity):
		if loopCity.getPopulation() > 2:
			iDistance = plotDistance(eventCity.getX(), eventCity.getY(), loopCity.getX(), loopCity.getY())
			if iDistance > 0:
				listCities.append((iDistance, loopCity))
		(loopCity, iter) = player.nextCity(iter, False)

	listCities.sort(key=itemgetter(0))

	if iNumCities > len(listCities):
		iNumCities = len(listCities)

	for i in range(iNumCities):
		(iDist, loopCity) = listCities[i]
		loopCity.changePopulation(-2)
		szBuffer = localText.getText("TXT_KEY_EVENT_INFLUENZA_HIT_CITY", (loopCity.getNameKey(), ))
		CyInterface().addMessage(kTriggeredData.ePlayer, False, gc.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_PILLAGE", InterfaceMessageTypes.MESSAGE_TYPE_INFO, None, gc.getInfoTypeForString("COLOR_RED"), loopCity.getX(), loopCity.getY(), True, True)


def getHelpInfluenza2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_INFLUENZA_HELP_2", (2, ))

	return szHelp

######## SOLO FLIGHT ###########


def canTriggerSoloFlight(argsList):
	kTriggeredData = argsList[0]

	map = gc.getMap()
	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		iMinLandmass  = 3
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		iMinLandmass  = 4
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		iMinLandmass  = 6
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		iMinLandmass  = 8
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		iMinLandmass  = 10
	else:
		iMinLandmass  = 12

	if (map.getNumLandAreas() < iMinLandmass):
		return False

	if gc.getGame().isGameMultiPlayer():
		return False

	return True

def getHelpSoloFlight(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_SOLO_FLIGHT_HELP_1", (1, ))

	return szHelp

def applySoloFlight(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive() and iPlayer != kTriggeredData.ePlayer:
			loopTeam = gc.getTeam(loopPlayer.getTeam())
			if loopTeam.isHasMet(gc.getPlayer(kTriggeredData.ePlayer).getTeam()):
				loopPlayer.AI_changeAttitudeExtra(kTriggeredData.ePlayer, 1)


######## ANTELOPE ###########

def canTriggerAntelope(argsList):

	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iDeer = gc.getInfoTypeForString("BONUS_DEER")
	iHappyBonuses = 0
	bDeer = False
	for i in range(gc.getNumBonusInfos()):
		bonus = gc.getBonusInfo(i)
		iNum = player.getNumAvailableBonuses(i)
		if iNum > 0 :
			if bonus.getHappiness() > 0:
				iHappyBonuses += 1
				if iHappyBonuses > 5:
					return False
			if i == iDeer:
				return False

	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)
	if not plot.canHaveBonus(iDeer, False):
		return False

	return True

def doAntelope2(argsList):
#	Need this because camps are not normally allowed unless there is already deer.
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)

	if not plot.isNone():
		plot.setImprovementType(gc.getInfoTypeForString("IMPROVEMENT_CAMP"))

	return 1

def getHelpAntelope2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	iCamp = gc.getInfoTypeForString("IMPROVEMENT_CAMP")
	szHelp = localText.getText("TXT_KEY_EVENT_IMPROVEMENT_GROWTH", ( gc.getImprovementInfo(iCamp).getTextKey(), ))

	return szHelp

######## WHALEOFATHING ###########

def canTriggerWhaleOfAThing(argsList):

	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iWhale = gc.getInfoTypeForString("BONUS_WHALE")
	iHappyBonuses = 0
	bWhale = False
	for i in range(gc.getNumBonusInfos()):
		bonus = gc.getBonusInfo(i)
		iNum = player.getNumAvailableBonuses(i)
		if iNum > 0 :
			if bonus.getHappiness() > 0:
				iHappyBonuses += 1
				if iHappyBonuses > 5:
					return False
			if i == iWhale:
				return False

	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)
	if not plot.canHaveBonus(iWhale, False):
		return False

	return True


######## HIYOSILVER ###########

def canTriggerHiyoSilver(argsList):

	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iSilver = gc.getInfoTypeForString("BONUS_SILVER")
	iHappyBonuses = 0
	bSilver = False
	for i in range(gc.getNumBonusInfos()):
		bonus = gc.getBonusInfo(i)
		iNum = player.getNumAvailableBonuses(i)
		if iNum > 0 :
			if bonus.getHappiness() > 0:
				iHappyBonuses += 1
				if iHappyBonuses > 5:
					return False
			if i == iSilver:
				return False

	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)
	if not plot.canHaveBonus(iSilver, False):
		return False

	return True

######## WININGMONKS ###########

def canTriggerWiningMonks(argsList):

	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	if player.getNumAvailableBonuses(gc.getInfoTypeForString("BONUS_WINE")) > 0:
		return False

	return True


def doWiningMonks2(argsList):
#	Need this because wineries are not normally allowed unless there is already wine.
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)

	if not plot.isNone():
		plot.setImprovementType(gc.getInfoTypeForString("IMPROVEMENT_WINERY"))

	return 1

def getHelpWiningMonks2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	iImp = gc.getInfoTypeForString("IMPROVEMENT_WINERY")
	szHelp = localText.getText("TXT_KEY_EVENT_IMPROVEMENT_GROWTH", ( gc.getImprovementInfo(iImp).getTextKey(), ))

	return szHelp


######## INDEPENDENTFILMS ###########

def canTriggerIndependentFilms(argsList):

	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	for i in range(gc.getNumBuildingInfos()):
		if gc.getBuildingInfo(i).getFreeBonus() == gc.getInfoTypeForString("BONUS_MOVIES"):
			if player.countNumBuildings(i) > 0:
				return False

	return True

def doIndependentFilms(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	player = gc.getPlayer(kTriggeredData.ePlayer)
	city = player.getCity(kTriggeredData.iCityId)

	iBonus = gc.getInfoTypeForString("BONUS_MOVIES")

	city.changeFreeBonus(iBonus, 1)

	return 1

def getHelpIndependentFilms(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	player = gc.getPlayer(kTriggeredData.ePlayer)
	city = player.getCity(kTriggeredData.iCityId)

	iBonus = gc.getInfoTypeForString("BONUS_MOVIES")

	szHelp = localText.getText("TXT_KEY_EVENT_INDEPENDENTFILMS_HELP_1", ( 1, gc.getBonusInfo(iBonus).getChar(), city.getNameKey()))

	return szHelp

######## ANCIENT OLYMPICS ###########

def canTriggerAncientOlympics(argsList):

	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	stateReligion = player.getStateReligion()

	if stateReligion == gc.getInfoTypeForString("RELIGION_JUDAISM"):
		return False

	if stateReligion == gc.getInfoTypeForString("RELIGION_CHRISTIANITY"):
		return False

	if stateReligion == gc.getInfoTypeForString("RELIGION_ISLAM"):
		return False

	return True

def doAncientOlympics2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	map = gc.getMap()

	for j in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(j)
		if j != kTriggeredData.ePlayer and loopPlayer.isAlive() and not loopPlayer.isMinorCiv():

			for i in range(map.numPlots()):
				plot = map.plotByIndex(i)
				if not plot.isWater() and plot.getOwner() == kTriggeredData.ePlayer and plot.isAdjacentPlayer(j, True):
					loopPlayer.AI_changeMemoryCount(kTriggeredData.ePlayer, MemoryTypes.MEMORY_EVENT_GOOD_TO_US, 1)
					break

	return 1

def getHelpAncientOlympics2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_ANCIENTOLYMPICS_HELP_2", ( 1, ))

	return szHelp


######## MODERN OLYMPICS ###########

def canTriggerModernOlympics(argsList):

	kTriggeredData = argsList[0]
	trigger = gc.getEventTriggerInfo(kTriggeredData.eTrigger)
	player = gc.getPlayer(kTriggeredData.ePlayer)

	kOrigTriggeredData = player.getEventOccured(trigger.getPrereqEvent(0))

	if (kOrigTriggeredData == None):
		return False

	kActualTriggeredDataObject = player.getEventTriggered(kTriggeredData.iId)
	kActualTriggeredDataObject.iCityId = kOrigTriggeredData.iCityId
	kActualTriggeredDataObject.iPlotX = kOrigTriggeredData.iPlotX
	kActualTriggeredDataObject.iPlotY = kOrigTriggeredData.iPlotY

	return True

def getHelpModernOlympics(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_SOLO_FLIGHT_HELP_1", (1, ))

	return szHelp

def applyModernOlympics(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive() and iPlayer != kTriggeredData.ePlayer:
			loopTeam = gc.getTeam(loopPlayer.getTeam())
			if loopTeam.isHasMet(gc.getPlayer(kTriggeredData.ePlayer).getTeam()):
				loopPlayer.AI_changeAttitudeExtra(kTriggeredData.ePlayer, 1)


######## INTERSTATE ###########

def canTriggerInterstate(argsList):

	kTriggeredData = argsList[0]
	trigger = gc.getEventTriggerInfo(kTriggeredData.eTrigger)
	player = gc.getPlayer(kTriggeredData.ePlayer)

	if not player.isCivic(gc.getInfoTypeForString("CIVIC_LIBERAL")):
		return False

	return True

def getHelpInterstate(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_UNIT_MOVEMENT", (1, gc.getRouteInfo(gc.getInfoTypeForString("ROUTE_MODERN_ROAD")).getTextKey()))

	return szHelp

def applyInterstate(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	team = gc.getTeam(player.getTeam())

	iRoad = gc.getInfoTypeForString("ROUTE_MODERN_ROAD")

	team.changeRouteChange(iRoad, -5)

######## EARTH DAY ###########

def getHelpEarthDay2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_EARTHDAY_HELP_2", ())

	return szHelp

def canApplyEarthDay2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	player = gc.getPlayer(kTriggeredData.ePlayer)

	iCivic = gc.getInfoTypeForString("CIVIC_ENVIRONMENTALISM")

	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive() and iPlayer != kTriggeredData.ePlayer and not loopPlayer.isHuman():
			loopTeam = gc.getTeam(loopPlayer.getTeam())
			if loopTeam.isHasMet(gc.getPlayer(kTriggeredData.ePlayer).getTeam()):
				tradeData = TradeData()
				tradeData.ItemType = TradeableItems.TRADE_CIVIC
				tradeData.iData = iCivic
				if loopPlayer.canTradeItem(kTriggeredData.ePlayer, tradeData, False):
					if (loopPlayer.getTradeDenial(kTriggeredData.ePlayer, tradeData) == DenialTypes.NO_DENIAL):
						return True
	return False


def applyEarthDay2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iCivic = gc.getInfoTypeForString("CIVIC_ENVIRONMENTALISM")
	iCivicOption = gc.getInfoTypeForString("CIVICOPTION_ECONOMY")

	listPlayers = []
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive() and iPlayer != kTriggeredData.ePlayer and not loopPlayer.isHuman():
			loopTeam = gc.getTeam(loopPlayer.getTeam())
			if loopTeam.isHasMet(gc.getPlayer(kTriggeredData.ePlayer).getTeam()):
				tradeData = TradeData()
				tradeData.ItemType = TradeableItems.TRADE_CIVIC
				tradeData.iData = iCivic
				if loopPlayer.canTradeItem(kTriggeredData.ePlayer, tradeData, False):
					if (loopPlayer.getTradeDenial(kTriggeredData.ePlayer, tradeData) == DenialTypes.NO_DENIAL):
						listPlayers.append((-loopPlayer.AI_civicValue(iCivic), iPlayer))

	listPlayers.sort()

	if len(listPlayers) > 3:
		listPlayers = listPlayers[:2]

	for (iValue, iPlayer) in listPlayers:
		gc.getPlayer(iPlayer).setCivics(iCivicOption, iCivic)

######## FREEDOM CONCERT ###########

def getHelpFreedomConcert2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_FREEDOMCONCERT_HELP_2", ())

	return szHelp

def canApplyFreedomConcert2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	player = gc.getPlayer(kTriggeredData.ePlayer)
	eventCity = player.getCity(kTriggeredData.iCityId)

	for iReligion in range(gc.getNumReligionInfos()):
		if eventCity.isHasReligion(iReligion):
			(loopCity, iter) = player.firstCity(False)
			while(loopCity):
				if not loopCity.isHasReligion(iReligion):
					for jReligion in range(gc.getNumReligionInfos()):
						if loopCity.isHasReligion(jReligion):
							return True
				(loopCity, iter) = player.nextCity(iter, False)

	return False

def applyFreedomConcert2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	eventCity = player.getCity(kTriggeredData.iCityId)

	for iReligion in range(gc.getNumReligionInfos()):
		if eventCity.isHasReligion(iReligion):

			bestCity = None
			iBestDistance = 0
			(loopCity, iter) = player.firstCity(False)
			while(loopCity):
				if not loopCity.isHasReligion(iReligion):
					bValid = False
					for jReligion in range(gc.getNumReligionInfos()):
						if loopCity.isHasReligion(jReligion):
							bValid = True
							break

					if bValid:
						iDistance = plotDistance(eventCity.getX(), eventCity.getY(), loopCity.getX(), loopCity.getY())

						if iDistance < iBestDistance or bestCity == None:
							bestCity = loopCity
							iBestDistance = iDistance

				(loopCity, iter) = player.nextCity(iter, False)


			if bestCity != None:
				bestCity.setHasReligion(iReligion, True, True, True)

######## HEROIC_GESTURE ###########

def canTriggerHeroicGesture(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	destPlayer = gc.getPlayer(kTriggeredData.eOtherPlayer)

	if not gc.getTeam(destPlayer.getTeam()).canChangeWarPeace(player.getTeam()):
		return False

	if gc.getTeam(destPlayer.getTeam()).AI_getWarSuccess(player.getTeam()) <= 0:
		return False

	if gc.getTeam(player.getTeam()).AI_getWarSuccess(destPlayer.getTeam()) <= 0:
		return False

	return True

def doHeroicGesture2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	destPlayer = gc.getPlayer(kTriggeredData.eOtherPlayer)
	player = gc.getPlayer(kTriggeredData.ePlayer)

	if destPlayer.isHuman():
		# this works only because it's a single-player only event
		popupInfo = CyPopupInfo()
		popupInfo.setButtonPopupType(ButtonPopupTypes.BUTTONPOPUP_PYTHON)
		popupInfo.setText(localText.getText("TXT_KEY_EVENT_HEROIC_GESTURE_OTHER_3", (player.getCivilizationAdjectiveKey(), )))
		popupInfo.setData1(kTriggeredData.eOtherPlayer)
		popupInfo.setData2(kTriggeredData.ePlayer)
		popupInfo.setPythonModule("CvRandomEventInterface")
		popupInfo.setOnClickedPythonCallback("heroicGesture2Callback")
		popupInfo.addPythonButton(localText.getText("TXT_KEY_POPUP_YES", ()), "")
		popupInfo.addPythonButton(localText.getText("TXT_KEY_POPUP_NO", ()), "")
		popupInfo.addPopup(kTriggeredData.eOtherPlayer)
	else:
		destPlayer.forcePeace(kTriggeredData.ePlayer)
		destPlayer.AI_changeAttitudeExtra(kTriggeredData.ePlayer, 1)
		player.AI_changeAttitudeExtra(kTriggeredData.eOtherPlayer, 1)

	return

def heroicGesture2Callback(argsList):
	iButton = argsList[0]
	iData1 = argsList[1]
	iData2 = argsList[2]
	iData3 = argsList[3]
	szText = argsList[4]
	bOption1 = argsList[5]
	bOption2 = argsList[6]

	if iButton == 0:
		destPlayer = gc.getPlayer(iData1)
		player = gc.getPlayer(iData2)
		destPlayer.forcePeace(iData2)
		destPlayer.AI_changeAttitudeExtra(iData2, 1)
		player.AI_changeAttitudeExtra(iData1, 1)

	return 0

def getHelpHeroicGesture2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	destPlayer = gc.getPlayer(kTriggeredData.eOtherPlayer)

	# Get help text
	szHelp = localText.getText("TXT_KEY_EVENT_ATTITUDE_GOOD", (1, destPlayer.getNameKey()));

	return szHelp

######## GREAT_MEDIATOR ###########

def canTriggerGreatMediator(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	destPlayer = gc.getPlayer(kTriggeredData.eOtherPlayer)

	if not gc.getTeam(player.getTeam()).canChangeWarPeace(destPlayer.getTeam()):
		return False

	if gc.getTeam(player.getTeam()).AI_getAtWarCounter(destPlayer.getTeam()) < 10:
		return False

	return True

def doGreatMediator2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	destPlayer = gc.getPlayer(kTriggeredData.eOtherPlayer)
	player = gc.getPlayer(kTriggeredData.ePlayer)

	if destPlayer.isHuman():
		# this works only because it's a single-player only event
		popupInfo = CyPopupInfo()
		popupInfo.setButtonPopupType(ButtonPopupTypes.BUTTONPOPUP_PYTHON)
		popupInfo.setText(localText.getText("TXT_KEY_EVENT_GREAT_MEDIATOR_OTHER_3", (player.getCivilizationAdjectiveKey(), )))
		popupInfo.setData1(kTriggeredData.eOtherPlayer)
		popupInfo.setData2(kTriggeredData.ePlayer)
		popupInfo.setPythonModule("CvRandomEventInterface")
		popupInfo.setOnClickedPythonCallback("greatMediator2Callback")
		popupInfo.addPythonButton(localText.getText("TXT_KEY_POPUP_YES", ()), "")
		popupInfo.addPythonButton(localText.getText("TXT_KEY_POPUP_NO", ()), "")
		popupInfo.addPopup(kTriggeredData.eOtherPlayer)
	else:
		gc.getTeam(player.getTeam()).makePeace(destPlayer.getTeam())
		destPlayer.AI_changeAttitudeExtra(kTriggeredData.ePlayer, 1)
		player.AI_changeAttitudeExtra(kTriggeredData.eOtherPlayer, 1)

	return

def greatMediator2Callback(argsList):
	iButton = argsList[0]
	iData1 = argsList[1]
	iData2 = argsList[2]
	iData3 = argsList[3]
	szText = argsList[4]
	bOption1 = argsList[5]
	bOption2 = argsList[6]

	if iButton == 0:
		destPlayer = gc.getPlayer(iData1)
		player = gc.getPlayer(iData2)
		gc.getTeam(destPlayer.getTeam()).makePeace(player.getTeam())
		destPlayer.AI_changeAttitudeExtra(iData2, 1)
		player.AI_changeAttitudeExtra(iData1, 1)

	return 0

def getHelpGreatMediator2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	destPlayer = gc.getPlayer(kTriggeredData.eOtherPlayer)

	# Get help text
	szHelp = localText.getText("TXT_KEY_EVENT_ATTITUDE_GOOD", (1, destPlayer.getNameKey()));

	return szHelp

######## ANCIENT_TEXTS ###########

def doAncientTexts2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive() and iPlayer != kTriggeredData.ePlayer:
			loopTeam = gc.getTeam(loopPlayer.getTeam())
			if loopTeam.isHasMet(gc.getPlayer(kTriggeredData.ePlayer).getTeam()):
				loopPlayer.AI_changeAttitudeExtra(kTriggeredData.ePlayer, 1)

	return

def getHelpAncientTexts2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_SOLO_FLIGHT_HELP_1", (1, ))

	return szHelp


######## IMPACT_CRATER ###########

def canTriggerImpactCrater(argsList):

	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iUranium = gc.getInfoTypeForString("BONUS_URANIUM")
	if player.getNumAvailableBonuses(iUranium) > 0:
		return False

	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)
	if not plot.canHaveBonus(iUranium, False):
		return False

	return True

def doImpactCrater2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)

	if not plot.isNone():
		plot.setImprovementType(gc.getInfoTypeForString("IMPROVEMENT_MINE"))

	return 1

def getHelpImpactCrater2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	iMine = gc.getInfoTypeForString("IMPROVEMENT_MINE")
	szHelp = localText.getText("TXT_KEY_EVENT_IMPROVEMENT_GROWTH", ( gc.getImprovementInfo(iMine).getTextKey(), ))

	return szHelp


######## THE_HUNS ###########

def canTriggerTheHuns(argsList):

	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

#   If Barbarians are disabled in this game, this event will not occur.
	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_BARBARIANS):
		return False

#   At least one civ on the board must know Mounted Archery. V: Changed to Horse Breeding
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_HORSE_BREEDING")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

#   At least one civ on the board must know Iron Working.
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_IRON_WORKING")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

	# Can we build the counter unit?
	iCounterUnitClass = gc.getInfoTypeForString("UNITCLASS_SPEARMAN")
	iCounterUnit = gc.getCivilizationInfo(player.getCivilizationType()).getCivilizationUnits(iCounterUnitClass)
	if iCounterUnit == -1:
		return False

	(loopCity, iter) = player.firstCity(False)
	bFound = False
	while(loopCity):
		if (loopCity.canTrain(iCounterUnit, False, False, False, False)):
			bFound = True
			break

		(loopCity, iter) = player.nextCity(iter, False)

	if not bFound:
		return False

#	Find an eligible plot
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			return True

	return False


def getHelpTheHuns1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_THE_HUNS_HELP_1", ())

	return szHelp


def applyTheHuns1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	listPlots = []
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			listPlots.append(i)

	if 0 == len(listPlots):
		return

	plot = map.plotByIndex(listPlots[gc.getGame().getSorenRandNum(len(listPlots), "Hun event location")])

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		iNumUnits  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		iNumUnits  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		iNumUnits  = 3
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		iNumUnits  = 4
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		iNumUnits  = 5
	else:
		iNumUnits  = 6

	iUnitType = gc.getInfoTypeForString("UNIT_HORSE_ARCHER")

	barbPlayer = gc.getPlayer(gc.getBARBARIAN_PLAYER())
	for i in range(iNumUnits):
		barbPlayer.initUnit(iUnitType, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK_CITY_LEMMING, DirectionTypes.DIRECTION_SOUTH)


######## THE_VANDALS ###########

def canTriggerTheVandals(argsList):

	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

#   If Barbarians are disabled in this game, this event will not occur.
	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_BARBARIANS):
		return False
			
#   At least one civ on the board must know Metal Casting. V: changed to Music.
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_MUSIC")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):			
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

#   At least one civ on the board must know Smithing.
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_SMITHING")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

	# Can we build the counter unit?
	iCounterUnitClass = gc.getInfoTypeForString("UNITCLASS_AXEMAN")
	iCounterUnit = gc.getCivilizationInfo(player.getCivilizationType()).getCivilizationUnits(iCounterUnitClass)
	if iCounterUnit == -1:
		return False

	(loopCity, iter) = player.firstCity(False)
	bFound = False
	while(loopCity):
		if (loopCity.canTrain(iCounterUnit, False, False, False, False)):
			bFound = True
			break

		(loopCity, iter) = player.nextCity(iter, False)

	if not bFound:
		return False

#	Find an eligible plot
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			return True

	return False


def getHelpTheVandals1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_THE_VANDALS_HELP_1", ())

	return szHelp


def applyTheVandals1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	listPlots = []
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			listPlots.append(i)

	if 0 == len(listPlots):
		return

	plot = map.plotByIndex(listPlots[gc.getGame().getSorenRandNum(len(listPlots), "Vandal event location")])

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		iNumUnits  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		iNumUnits  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		iNumUnits  = 3
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		iNumUnits  = 4
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		iNumUnits  = 5
	else:
		iNumUnits  = 6

	iUnitType = gc.getInfoTypeForString("UNIT_SWORDSMAN")

	barbPlayer = gc.getPlayer(gc.getBARBARIAN_PLAYER())
	for i in range(iNumUnits):
		barbPlayer.initUnit(iUnitType, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK_CITY_LEMMING, DirectionTypes.DIRECTION_SOUTH)


######## THE_GOTHS ###########

def canTriggerTheGoths(argsList):

	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

#   If Barbarians are disabled in this game, this event will not occur.
	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_BARBARIANS):
		return False

#   At least one civ on the board must know Mathematics.
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_MATHEMATICS")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

#   At least one civ on the board must know Iron Working.
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_IRON_WORKING")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

	# Can we build the counter unit?
	iCounterUnitClass = gc.getInfoTypeForString("UNITCLASS_CHARIOT")
	iCounterUnit = gc.getCivilizationInfo(player.getCivilizationType()).getCivilizationUnits(iCounterUnitClass)
	if iCounterUnit == -1:
		return False

	(loopCity, iter) = player.firstCity(False)
	bFound = False
	while(loopCity):
		if (loopCity.canTrain(iCounterUnit, False, False, False, False)):
			bFound = True
			break

		(loopCity, iter) = player.nextCity(iter, False)

	if not bFound:
		return False

#	Find an eligible plot
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			return True

	return False


def getHelpThGoths1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_THE_GOTHS_HELP_1", ())

	return szHelp


def applyTheGoths1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	listPlots = []
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			listPlots.append(i)

	if 0 == len(listPlots):
		return

	plot = map.plotByIndex(listPlots[gc.getGame().getSorenRandNum(len(listPlots), "Goth event location")])

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		iNumUnits  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		iNumUnits  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		iNumUnits  = 3
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		iNumUnits  = 4
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		iNumUnits  = 5
	else:
		iNumUnits  = 6

	iUnitType = gc.getInfoTypeForString("UNIT_AXEMAN")

	barbPlayer = gc.getPlayer(gc.getBARBARIAN_PLAYER())
	for i in range(iNumUnits):
		barbPlayer.initUnit(iUnitType, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK_CITY_LEMMING, DirectionTypes.DIRECTION_SOUTH)


######## THE_PHILISTINES ###########

def canTriggerThePhilistines(argsList):

	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

#   If Barbarians are disabled in this game, this event will not occur.
	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_BARBARIANS):
		return False

#   At least one civ on the board must know Monotheism. V: changed to Scriptures.
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_SCRIPTURES")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

#   At least one civ on the board must know Bronze Working.
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_BRONZE_WORKING")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

	# Can we build the counter unit?
	iCounterUnitClass = gc.getInfoTypeForString("UNITCLASS_AXEMAN")
	iCounterUnit = gc.getCivilizationInfo(player.getCivilizationType()).getCivilizationUnits(iCounterUnitClass)
	if iCounterUnit == -1:
		return False

	(loopCity, iter) = player.firstCity(False)
	bFound = False
	while(loopCity):
		if (loopCity.canTrain(iCounterUnit, False, False, False, False)):
			bFound = True
			break

		(loopCity, iter) = player.nextCity(iter, False)

	if not bFound:
		return False

#	Find an eligible plot
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			return True

	return False


def getHelpThePhilistines1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_THE_PHILISTINES_HELP_1", ())

	return szHelp


def applyThePhilistines1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	listPlots = []
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			listPlots.append(i)

	if 0 == len(listPlots):
		return

	plot = map.plotByIndex(listPlots[gc.getGame().getSorenRandNum(len(listPlots), "Philistine event location")])

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		iNumUnits  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		iNumUnits  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		iNumUnits  = 3
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		iNumUnits  = 4
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		iNumUnits  = 5
	else:
		iNumUnits  = 6

	iUnitType = gc.getInfoTypeForString("UNIT_SPEARMAN")

	barbPlayer = gc.getPlayer(gc.getBARBARIAN_PLAYER())
	for i in range(iNumUnits):
		barbPlayer.initUnit(iUnitType, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK_CITY_LEMMING, DirectionTypes.DIRECTION_SOUTH)


######## THE_VEDIC_ARYANS ###########

def canTriggerTheVedicAryans(argsList):

	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

#   If Barbarians are disabled in this game, this event will not occur.
	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_BARBARIANS):
		return False

#   At least one civ on the board must know Polytheism.
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_POLYTHEISM")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

#   At least one civ on the board must know Archery.
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_ARCHERY")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

	# Can we build the counter unit?
	iCounterUnitClass = gc.getInfoTypeForString("UNITCLASS_ARCHER")
	iCounterUnit = gc.getCivilizationInfo(player.getCivilizationType()).getCivilizationUnits(iCounterUnitClass)
	if iCounterUnit == -1:
		return False

	(loopCity, iter) = player.firstCity(False)
	bFound = False
	while(loopCity):
		if (loopCity.canTrain(iCounterUnit, False, False, False, False)):
			bFound = True
			break

		(loopCity, iter) = player.nextCity(iter, False)

	if not bFound:
		return False

#	Find an eligible plot
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			return True

	return False


def getHelpTheVedicAryans1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_THE_VEDIC_ARYANS_HELP_1", ())

	return szHelp


def applyTheVedicAryans1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	listPlots = []
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			listPlots.append(i)

	if 0 == len(listPlots):
		return

	plot = map.plotByIndex(listPlots[gc.getGame().getSorenRandNum(len(listPlots), "Vedic Aryan event location")])

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		iNumUnits  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		iNumUnits  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		iNumUnits  = 3
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		iNumUnits  = 4
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		iNumUnits  = 5
	else:
		iNumUnits  = 6

	iUnitType = gc.getInfoTypeForString("UNIT_ARCHER")

	barbPlayer = gc.getPlayer(gc.getBARBARIAN_PLAYER())
	for i in range(iNumUnits):
		barbPlayer.initUnit(iUnitType, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK_CITY_LEMMING, DirectionTypes.DIRECTION_SOUTH)

######## SECURITY_TAX ###########

def canTriggerSecurityTax(argsList):

	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iWalls = gc.getInfoTypeForString("BUILDINGCLASS_WALLS")
	if player.getNumCities() > player.getBuildingClassCountWithUpgrades(iWalls):
		return False

	return True


######## LITERACY ###########

def canTriggerLiteracy(argsList):

	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iLibrary = gc.getInfoTypeForString("BUILDINGCLASS_LIBRARY")
	if player.getNumCities() > player.getBuildingClassCountWithUpgrades(iLibrary):
		return False

	return True

######## TEA ###########

def canTriggerTea(argsList):

	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	if player.isCivic(gc.getInfoTypeForString("CIVIC_MERCANTILISM")):
		return False

	bCanTrade = False
	for iLoopPlayer in range(gc.getMAX_CIV_PLAYERS()):
		if player.canHaveTradeRoutesWith(iLoopPlayer):
			bCanTrade = True
			break

	if not bCanTrade:
		return False

	return True

######## HORSE WHISPERING ###########

def canTriggerHorseWhispering(argsList):
	kTriggeredData = argsList[0]

	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_ONE_CITY_CHALLENGE) and gc.getPlayer(kTriggeredData.ePlayer).isHuman():
		return False

	return True

def getHelpHorseWhispering1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	map = gc.getMap()

	iNumStables = int((gc.getWorldInfo(map.getWorldSize()).getDefaultPlayers() + 1)/2)
	iNumHorses = 2 + int(gc.getWorldInfo(map.getWorldSize()).getDefaultPlayers()/9)
	szHelp = localText.getText("TXT_KEY_EVENT_HORSE_WHISPERING_HELP", (iNumStables, iNumHorses))

	return szHelp

def canTriggerHorseWhisperingDone(argsList):
	kTriggeredData = argsList[0]
	trigger = gc.getEventTriggerInfo(kTriggeredData.eTrigger)
	player = gc.getPlayer(kTriggeredData.ePlayer)
	map = gc.getMap()

	iStable = gc.getInfoTypeForString("BUILDINGCLASS_STABLE")
	if int((gc.getWorldInfo(map.getWorldSize()).getDefaultPlayers() + 1)/2) > player.getBuildingClassCountWithUpgrades(iStable):
		return False

	iHorse = gc.getInfoTypeForString("BONUS_HORSE")
	if (2+int(gc.getWorldInfo(map.getWorldSize()).getDefaultPlayers()/9)) > player.countOwnedBonuses(iHorse):
		return False

	return True

def getHelpHorseWhisperingDone1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	map = gc.getMap()

	iNumUnits = gc.getWorldInfo(map.getWorldSize()).getDefaultPlayers()
	szHelp = localText.getText("TXT_KEY_EVENT_HORSE_WHISPERING_DONE_HELP_1", (iNumUnits, ))

	return szHelp

def applyHorseWhisperingDone1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	map = gc.getMap()
	plot = map.plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)
	iNumUnits = gc.getWorldInfo(map.getWorldSize()).getDefaultPlayers()
	iUnitClassType = gc.getInfoTypeForString("UNITCLASS_HORSE_ARCHER")
	iUnitType = gc.getCivilizationInfo(player.getCivilizationType()).getCivilizationUnits(iUnitClassType)

	if iUnitType != -1:
		for i in range(iNumUnits):
			player.initUnit(iUnitType, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK, DirectionTypes.DIRECTION_SOUTH)

######## HARBORMASTER ###########

def getHelpHarbormaster1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	iCaravelsRequired = gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers() + 1
	iHarborsRequired = (iCaravelsRequired - 1) / 2

	szHelp = localText.getText("TXT_KEY_EVENT_HARBORMASTER_HELP", (iHarborsRequired, iCaravelsRequired))

	return szHelp


def canTriggerHarbormaster(argsList):
	kTriggeredData = argsList[0]
	trigger = gc.getEventTriggerInfo(kTriggeredData.eTrigger)
	player = gc.getPlayer(kTriggeredData.ePlayer)

	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_ONE_CITY_CHALLENGE) and gc.getPlayer(kTriggeredData.ePlayer).isHuman():
		return False

	map = gc.getMap()

	iNumWater = 0

	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)

		if plot.isWater():
			iNumWater += 1

		if 100 * iNumWater >= 40 * map.numPlots():
			return True

	return False

def canTriggerHarbormasterDone(argsList):
	kTriggeredData = argsList[0]
	trigger = gc.getEventTriggerInfo(kTriggeredData.eTrigger)
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iCaravelsRequired = gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers() + 1
	iHarborsRequired = (iCaravelsRequired - 1) / 2

	iHarbor = gc.getInfoTypeForString("BUILDINGCLASS_COMMERCIAL_PORT")
	if iHarborsRequired > player.getBuildingClassCountWithUpgrades(iHarbor):
		return False

	iCaravel = gc.getInfoTypeForString("UNITCLASS_CARAVEL")
	iBrigantine = gc.getInfoTypeForString("UNITCLASS_BRIGANTINE")
	if iCaravelsRequired > (player.getUnitClassCount(iCaravel) + player.getUnitClassCount(iBrigantine)):
		return False

	return True

######## CLASSIC LITERATURE ###########

def canTriggerClassicLiterature(argsList):
	kTriggeredData = argsList[0]

	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_ONE_CITY_CHALLENGE) and gc.getPlayer(kTriggeredData.ePlayer).isHuman():
		return False

	return True

def getHelpClassicLiterature1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	iLibrariesRequired = int(gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers()/2 + 0.5)

	szHelp = localText.getText("TXT_KEY_EVENT_CLASSIC_LITERATURE_HELP_1", (iLibrariesRequired, iLibrariesRequired))

	return szHelp


def canTriggerClassicLiteratureDone(argsList):
	kTriggeredData = argsList[0]
	trigger = gc.getEventTriggerInfo(kTriggeredData.eTrigger)
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iLibrary = gc.getInfoTypeForString("BUILDINGCLASS_LIBRARY")
	iTheatre = gc.getInfoTypeForString("BUILDINGCLASS_THEATRE")
	iBuildingsRequired = int(gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers()/2 + 0.5)
	if iBuildingsRequired > player.getBuildingClassCountWithUpgrades(iLibrary):
		return False
	if iBuildingsRequired > player.getBuildingClassCountWithUpgrades(iTheatre):
		return False

	return True

def getHelpClassicLiteratureDone2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_CLASSIC_LITERATURE_DONE_HELP_2", ( ))

	return szHelp

def canApplyClassicLiteratureDone2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iEraAncient = gc.getInfoTypeForString("ERA_ANCIENT")
	iEraClassical = gc.getInfoTypeForString("ERA_CLASSICAL")

	for iTech in range(gc.getNumTechInfos()):
		if (gc.getTechInfo(iTech).getEra() == iEraAncient or gc.getTechInfo(iTech).getEra() == iEraClassical) and player.canResearch(iTech, False):
			return True

	return False

def applyClassicLiteratureDone2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iEraAncient = gc.getInfoTypeForString("ERA_ANCIENT")
	iEraClassical = gc.getInfoTypeForString("ERA_CLASSICAL")

	listTechs = []
	for iTech in range(gc.getNumTechInfos()):
		if (gc.getTechInfo(iTech).getEra() == iEraAncient or gc.getTechInfo(iTech).getEra() == iEraClassical) and player.canResearch(iTech, False):
			listTechs.append(iTech)

	if len(listTechs) > 0:
		iTech = listTechs[gc.getGame().getSorenRandNum(len(listTechs), "Classic Literature Event Tech selection")]
		gc.getTeam(player.getTeam()).setHasTech(iTech, True, kTriggeredData.ePlayer, True, True)

def getHelpClassicLiteratureDone3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iSpecialist = gc.getInfoTypeForString("SPECIALIST_SCIENTIST")
	iGreatLibrary = gc.getInfoTypeForString("BUILDING_GREAT_LIBRARY")

	szCityName = u""
	(loopCity, iter) = player.firstCity(False)
	while(loopCity):
		if (loopCity.isHasBuilding(iGreatLibrary)):
			szCityName = loopCity.getNameKey()
			break

		(loopCity, iter) = player.nextCity(iter, False)

	szHelp = localText.getText("TXT_KEY_EVENT_FREE_SPECIALIST", (1, gc.getSpecialistInfo(iSpecialist).getTextKey(), szCityName))

	return szHelp

def canApplyClassicLiteratureDone3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iGreatLibrary = gc.getInfoTypeForString("BUILDING_GREAT_LIBRARY")

	(loopCity, iter) = player.firstCity(False)
	while(loopCity):
		if (loopCity.isHasBuilding(iGreatLibrary)):
			return True

		(loopCity, iter) = player.nextCity(iter, False)

	return False

def applyClassicLiteratureDone3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iSpecialist = gc.getInfoTypeForString("SPECIALIST_SCIENTIST")
	iGreatLibrary = gc.getInfoTypeForString("BUILDING_GREAT_LIBRARY")

	(loopCity, iter) = player.firstCity(False)
	while(loopCity):
		if (loopCity.isHasBuilding(iGreatLibrary)):
			loopCity.changeFreeSpecialistCount(iSpecialist, 1)
			return

		(loopCity, iter) = player.nextCity(iter, False)

######## MASTER BLACKSMITH ###########

def canTriggerMasterBlacksmith(argsList):
	kTriggeredData = argsList[0]

	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_ONE_CITY_CHALLENGE) and gc.getPlayer(kTriggeredData.ePlayer).isHuman():
		return False

	return True

def getHelpMasterBlacksmith1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	map = gc.getMap()

	iRequired = int((gc.getWorldInfo(map.getWorldSize()).getDefaultPlayers() + 1)/2)
	iNumResources = max(3, 2 + int(gc.getWorldInfo(map.getWorldSize()).getDefaultPlayers()/4))

	szHelp = localText.getText("TXT_KEY_EVENT_MASTER_BLACKSMITH_HELP_1", (iRequired, iNumResources, player.getCity(kTriggeredData.iCityId).getNameKey()))

	return szHelp

def expireMasterBlacksmith1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	city = player.getCity(kTriggeredData.iCityId)
	if city == None or city.getOwner() != kTriggeredData.ePlayer:
		return True

	return False

def canTriggerMasterBlacksmithDone(argsList):
	kTriggeredData = argsList[0]
	trigger = gc.getEventTriggerInfo(kTriggeredData.eTrigger)
	player = gc.getPlayer(kTriggeredData.ePlayer)
	map = gc.getMap()

	iForge = gc.getInfoTypeForString("BUILDINGCLASS_FORGE");
	iBuildingsRequired = int((gc.getWorldInfo(map.getWorldSize()).getDefaultPlayers() + 1)/2)
	iNumResources = max(3, 2 + int(gc.getWorldInfo(map.getWorldSize()).getDefaultPlayers()/4))
	pCopper = gc.getInfoTypeForString("BONUS_COPPER")
	pGold = gc.getInfoTypeForString("BONUS_GOLD")
	pIron = gc.getInfoTypeForString("BONUS_IRON")
	pLead = gc.getInfoTypeForString("BONUS_LEAD")
	pSilver = gc.getInfoTypeForString("BONUS_SILVER")
	iMetal = player.countOwnedBonuses(pCopper) + player.countOwnedBonuses(pGold) + player.countOwnedBonuses(pIron) + player.countOwnedBonuses(pLead) + player.countOwnedBonuses(pSilver)

	if iBuildingsRequired > player.getBuildingClassCountWithUpgrades(iForge):
		return False

	if iNumResources > iMetal:
		return False

	kOrigTriggeredData = player.getEventOccured(trigger.getPrereqEvent(0))

	city = player.getCity(kOrigTriggeredData.iCityId)
	if city == None or city.getOwner() != kTriggeredData.ePlayer:
		return False

	kActualTriggeredDataObject = player.getEventTriggered(kTriggeredData.iId)
	kActualTriggeredDataObject.iCityId = kOrigTriggeredData.iCityId

	return True

def canApplyMasterBlacksmithDone1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iBonus = gc.getInfoTypeForString("BONUS_COPPER")
	city = player.getCity(kTriggeredData.iCityId)

	if city == None:
		return False

	map = gc.getMap()
	iBestValue = map.getGridWidth() + map.getGridHeight()
	bestPlot = None
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == kTriggeredData.ePlayer and plot.canHaveBonus(iBonus, False)):
			iValue = plotDistance(city.getX(), city.getY(), plot.getX(), plot.getY())
			if iValue < iBestValue:
				iBestValue = iValue
				bestPlot = plot

	if bestPlot == None:
		return False

	kActualTriggeredDataObject = player.getEventTriggered(kTriggeredData.iId)
	kActualTriggeredDataObject.iPlotX = bestPlot.getX()
	kActualTriggeredDataObject.iPlotY = bestPlot.getY()

	return True

def applyMasterBlacksmithDone1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)
	city = player.getCity(kTriggeredData.iCityId)

	iBonus = gc.getInfoTypeForString("BONUS_COPPER")
	plot.setBonusType(iBonus)

	szBuffer = localText.getText("TXT_KEY_MISC_DISCOVERED_NEW_RESOURCE", (gc.getBonusInfo(iBonus).getTextKey(), city.getNameKey()))
	CyInterface().addMessage(kTriggeredData.ePlayer, False, gc.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_DISCOVERBONUS", InterfaceMessageTypes.MESSAGE_TYPE_MINOR_EVENT, gc.getBonusInfo(iBonus).getButton(), gc.getInfoTypeForString("COLOR_WHITE"), plot.getX(), plot.getY(), True, True)

def canApplyMasterBlacksmithDone3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	if player.getStateReligion() == -1:
		return False

	return True

######## THE BEST DEFENSE ###########

def canTriggerBestDefense(argsList):
	kTriggeredData = argsList[0]

	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_ONE_CITY_CHALLENGE) and gc.getPlayer(kTriggeredData.ePlayer).isHuman():
		return False

	return True

def getHelpBestDefense1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	map = gc.getMap()

	iRequired = int((gc.getWorldInfo(map.getWorldSize()).getDefaultPlayers() + 1)/2)

	szHelp = localText.getText("TXT_KEY_EVENT_BEST_DEFENSE_HELP_1", (iRequired, iRequired, ))

	return szHelp

def canTriggerBestDefenseDone(argsList):
	kTriggeredData = argsList[0]
	trigger = gc.getEventTriggerInfo(kTriggeredData.eTrigger)
	player = gc.getPlayer(kTriggeredData.ePlayer)
	map = gc.getMap()


	iCastle = gc.getInfoTypeForString("BUILDINGCLASS_CASTLE")
	iFort = gc.getInfoTypeForString("IMPROVEMENT_FORT")
	iBuildingsRequired = int((gc.getWorldInfo(map.getWorldSize()).getDefaultPlayers() + 1)/2)
	if iBuildingsRequired > player.getBuildingClassCountWithUpgrades(iCastle):
		return False
	if iBuildingsRequired > player.getImprovementCount(iFort):
		return False

	return True

def getHelpBestDefenseDone2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_BEST_DEFENSE_DONE_HELP_2", (3, ))

	return szHelp

def applyBestDefenseDone2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive() and iPlayer != kTriggeredData.ePlayer:
			loopTeam = gc.getTeam(loopPlayer.getTeam())
			if loopTeam.isHasMet(gc.getPlayer(kTriggeredData.ePlayer).getTeam()):
				loopPlayer.AI_changeAttitudeExtra(kTriggeredData.ePlayer, 3)


def canApplyBestDefenseDone3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iGreatWall = gc.getInfoTypeForString("BUILDING_GREAT_WALL")

	(loopCity, iter) = player.firstCity(False)
	while(loopCity):
		if (loopCity.isHasBuilding(iGreatWall)):
			return True

		(loopCity, iter) = player.nextCity(iter, False)

	return False

######## NATIONAL SPORTS LEAGUE ###########

def canTriggerSportsLeague(argsList):
	kTriggeredData = argsList[0]

	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_ONE_CITY_CHALLENGE) and gc.getPlayer(kTriggeredData.ePlayer).isHuman():
		return False

	return True
def getHelpSportsLeague1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	map = gc.getMap()

	iRequired = int((gc.getWorldInfo(map.getWorldSize()).getDefaultPlayers() + 1)/2)
	iBuilding = gc.getInfoTypeForString("BUILDING_CIRCUS_MAXIMUS")

	szHelp = localText.getText("TXT_KEY_EVENT_SPORTS_LEAGUE_HELP_1", (iRequired, gc.getBuildingInfo(iBuilding).getTextKey()))

	return szHelp

def canTriggerSportsLeagueDone(argsList):
	kTriggeredData = argsList[0]
	trigger = gc.getEventTriggerInfo(kTriggeredData.eTrigger)
	player = gc.getPlayer(kTriggeredData.ePlayer)
	map = gc.getMap()

	iColosseum = gc.getInfoTypeForString("BUILDINGCLASS_ARENA")
	iStatue = gc.getInfoTypeForString("BUILDINGCLASS_STATUE_OF_CHAMPION")
	iBuildingsRequired = int((gc.getWorldInfo(map.getWorldSize()).getDefaultPlayers() + 1)/2)
	if iBuildingsRequired > player.getBuildingClassCountWithUpgrades(iColosseum):
		return False
	if 1 > player.getBuildingClassCountWithUpgrades(iStatue):
		return False

	return True

def canApplySportsLeagueDone3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iZeus = gc.getInfoTypeForString("BUILDING_CIRCUS_MAXIMUS")

	(loopCity, iter) = player.firstCity(False)
	while(loopCity):
		if (loopCity.isHasBuilding(iZeus)):
			return True

		(loopCity, iter) = player.nextCity(iter, False)

	return False

######## CRUSADE ###########

def canTriggerCrusade(argsList):
	kTriggeredData = argsList[0]
	trigger = gc.getEventTriggerInfo(kTriggeredData.eTrigger)
	player = gc.getPlayer(kTriggeredData.ePlayer)
	otherPlayer = gc.getPlayer(kTriggeredData.eOtherPlayer)
	holyCity = gc.getGame().getHolyCity(kTriggeredData.eReligion)

	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_ONE_CITY_CHALLENGE) and gc.getPlayer(kTriggeredData.ePlayer).isHuman():
		return False

	if holyCity.getOwner() != kTriggeredData.eOtherPlayer:
		return False

	kActualTriggeredDataObject = player.getEventTriggered(kTriggeredData.iId)
	kActualTriggeredDataObject.iOtherPlayerCityId = holyCity.getID()

	return True

def getHelpCrusade1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	holyCity = gc.getGame().getHolyCity(kTriggeredData.eReligion)

	szHelp = localText.getText("TXT_KEY_EVENT_CRUSADE_HELP_1", (holyCity.getNameKey(), ))

	return szHelp

def expireCrusade1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	otherPlayer = gc.getPlayer(kTriggeredData.eOtherPlayer)
	holyCity = gc.getGame().getHolyCity(kTriggeredData.eReligion)

	if holyCity.getOwner() == kTriggeredData.ePlayer:
		return False

	if player.getStateReligion() != kTriggeredData.eReligion:
		return True

	if holyCity.getOwner() != kTriggeredData.eOtherPlayer:
		return True

	if not gc.getTeam(player.getTeam()).isAtWar(otherPlayer.getTeam()):
		return True

	return False

def canTriggerCrusadeDone(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	trigger = gc.getEventTriggerInfo(kTriggeredData.eTrigger)

	kOrigTriggeredData = player.getEventOccured(trigger.getPrereqEvent(0))
	holyCity = gc.getGame().getHolyCity(kOrigTriggeredData.eReligion)

	if holyCity.getOwner() != kTriggeredData.ePlayer:
		return False

	kActualTriggeredDataObject = player.getEventTriggered(kTriggeredData.iId)
	kActualTriggeredDataObject.iCityId = holyCity.getID()
	kActualTriggeredDataObject.eOtherPlayer = kOrigTriggeredData.eOtherPlayer
	kActualTriggeredDataObject.eReligion = kOrigTriggeredData.eReligion

	for iBuilding in range(gc.getNumBuildingInfos()):
		if gc.getBuildingInfo(iBuilding).getHolyCity() == kOrigTriggeredData.eReligion:
			kActualTriggeredDataObject.eBuilding = BuildingTypes(iBuilding)
			break

	return True

def getHelpCrusadeDone1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	holyCity = gc.getGame().getHolyCity(kTriggeredData.eReligion)
	szUnit = gc.getUnitInfo(holyCity.getConscriptUnit()).getTextKey()
	iNumUnits = gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers() / 2 + 1
	szHelp = localText.getText("TXT_KEY_EVENT_CRUSADE_DONE_HELP_1", (iNumUnits, szUnit, holyCity.getNameKey()))

	return szHelp

def canApplyCrusadeDone1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	holyCity = gc.getGame().getHolyCity(kTriggeredData.eReligion)
	if -1 == holyCity.getConscriptUnit():
		return False

	return True

def applyCrusadeDone1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	holyCity = gc.getGame().getHolyCity(kTriggeredData.eReligion)
	iUnitType = holyCity.getConscriptUnit()
	iNumUnits = gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers() / 2 + 1

	if iUnitType != -1:
		for i in range(iNumUnits):
			player.initUnit(iUnitType, holyCity.getX(), holyCity.getY(), UnitAITypes.UNITAI_CITY_DEFENSE, DirectionTypes.DIRECTION_SOUTH)

def getHelpCrusadeDone2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	holyCity = gc.getGame().getHolyCity(kTriggeredData.eReligion)

	szHelp = localText.getText("TXT_KEY_EVENT_CRUSADE_DONE_HELP_2", (gc.getBuildingInfo(kTriggeredData.eBuilding).getTextKey(), holyCity.getNameKey()))

	return szHelp

def canApplyCrusadeDone2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	holyCity = gc.getGame().getHolyCity(kTriggeredData.eReligion)

	if -1 == kTriggeredData.eBuilding or holyCity.isHasBuilding(kTriggeredData.eBuilding):
		return False

	return True

def applyCrusadeDone2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	holyCity = gc.getGame().getHolyCity(kTriggeredData.eReligion)
	holyCity.setNumRealBuilding(kTriggeredData.eBuilding, 1)

	if (not gc.getGame().isNetworkMultiPlayer() and kTriggeredData.ePlayer == gc.getGame().getActivePlayer()):
		popupInfo = CyPopupInfo()
		popupInfo.setButtonPopupType(ButtonPopupTypes.BUTTONPOPUP_PYTHON_SCREEN)
		popupInfo.setData1(kTriggeredData.eBuilding)
		popupInfo.setData2(holyCity.getID())
		popupInfo.setData3(0)
		popupInfo.setText(u"showWonderMovie")
		popupInfo.addPopup(kTriggeredData.ePlayer)

def getHelpCrusadeDone3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	iNumCities = gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers()
	holyCity = gc.getGame().getHolyCity(kTriggeredData.eReligion)

	szHelp = localText.getText("TXT_KEY_EVENT_CRUSADE_DONE_HELP_3", (gc.getReligionInfo(kTriggeredData.eReligion).getTextKey(), iNumCities))

	return szHelp

def canApplyCrusadeDone3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	iNumCities = gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers()

	if gc.getGame().getNumCities() == gc.getGame().countReligionLevels(kTriggeredData.eReligion):
		return False

	return True

def applyCrusadeDone3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	holyCity = gc.getGame().getHolyCity(kTriggeredData.eReligion)

	listCities = []
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			(loopCity, iter) = loopPlayer.firstCity(False)

			while(loopCity):
				if (not loopCity.isHasReligion(kTriggeredData.eReligion)):
					iDistance = plotDistance(holyCity.getX(), holyCity.getY(), loopCity.getX(), loopCity.getY())
					listCities.append((iDistance, loopCity))

				(loopCity, iter) = loopPlayer.nextCity(iter, False)

	listCities.sort(key=itemgetter(0))

	iNumCities = min(gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers(), len(listCities))

	for i in range(iNumCities):
		iDistance, loopCity = listCities[i]
		loopCity.setHasReligion(kTriggeredData.eReligion, True, True, True)

######## ESTEEMEED_PLAYWRIGHT ###########

def canTriggerEsteemedPlaywright(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	# If source civ is operating this Civic, disallow the event to trigger.
	if player.isCivic(gc.getInfoTypeForString("CIVIC_SLAVERY")):
		return False

	return True


######## SECRET_KNOWLEDGE ###########

def getHelpSecretKnowledge2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	szHelp = localText.getText("TXT_KEY_EVENT_YIELD_CHANGE_BUILDING", (gc.getBuildingInfo(kTriggeredData.eBuilding).getTextKey(), u"+4[ICON_CULTURE]"))

	return szHelp

def applySecretKnowledge2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	city = player.getCity(kTriggeredData.iCityId)
	city.setBuildingCommerceChange(gc.getBuildingInfo(kTriggeredData.eBuilding).getBuildingClassType(), CommerceTypes.COMMERCE_CULTURE, 4)

######## HIGH_WARLORD ###########

def canTriggerHighWarlord(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	# If source civ is operating this Civic, disallow the event to trigger.
	if player.isCivic(gc.getInfoTypeForString("CIVIC_LIBERAL")):
		return False

	return True


######## EXPERIENCED_CAPTAIN ###########

def canTriggerExperiencedCaptain(argsList):
	kTriggeredData = argsList[0]

	player = gc.getPlayer(kTriggeredData.ePlayer)
	unit = player.getUnit(kTriggeredData.iUnitId)

	if unit.isNone():
		return False

	if unit.getExperience() < 7:
		return False

	return True

######## PARTISANS ###########

def getNumPartisanUnits(plot, iPlayer):
	for i in range(gc.getNumCultureLevelInfos()):
		iI = gc.getNumCultureLevelInfos() - i - 1
		if plot.getCulture(iPlayer) >= gc.getCultureLevelInfo(iI).getSpeedThreshold(gc.getGame().getGameSpeedType()):
			return iI
	return 0

def getHelpPartisans1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	capital = player.getCapitalCity()
	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)

	if None != capital and not capital.isNone():
		iNumUnits = getNumPartisanUnits(plot, kTriggeredData.ePlayer)
		szUnit = gc.getUnitInfo(capital.getConscriptUnit()).getTextKey()

		szHelp = localText.getText("TXT_KEY_EVENT_PARTISANS_HELP_1", (iNumUnits, szUnit))

	return szHelp

def canApplyPartisans1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)

	if getNumPartisanUnits(plot, kTriggeredData.ePlayer) <= 0:
		return False

	for i in range(3):
		for j in range(3):
			loopPlot = gc.getMap().plot(kTriggeredData.iPlotX + i - 1, kTriggeredData.iPlotY + j - 1)
			if None != loopPlot and not loopPlot.isNone():
				if not (loopPlot.isVisibleEnemyUnit(kTriggeredData.ePlayer) or loopPlot.isWater() or loopPlot.isImpassable() or loopPlot.isCity()):
					return True
	return False


def applyPartisans1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	capital = player.getCapitalCity()
	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)

	if None != capital and not capital.isNone():
		iNumUnits = getNumPartisanUnits(plot, kTriggeredData.ePlayer)

		listPlots = []
		for i in range(3):
			for j in range(3):
				loopPlot = gc.getMap().plot(kTriggeredData.iPlotX + i - 1, kTriggeredData.iPlotY + j - 1)
				if None != loopPlot and not loopPlot.isNone() and (i != 1 or j != 1):
					if not (loopPlot.isVisibleEnemyUnit(kTriggeredData.ePlayer) or loopPlot.isWater() or loopPlot.isImpassable()):
						listPlots.append(loopPlot)

		if len(listPlots) > 0:
			for i in range(iNumUnits):
				iPlot = gc.getGame().getSorenRandNum(len(listPlots), "Partisan event placement")
				player.initUnit(capital.getConscriptUnit(), listPlots[iPlot].getX(), listPlots[iPlot].getY(), UnitAITypes.UNITAI_ATTACK, DirectionTypes.DIRECTION_SOUTH)

def getHelpPartisans2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	capital = player.getCapitalCity()
	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)

	if None != capital and not capital.isNone():
		iNumUnits = max(1, getNumPartisanUnits(plot, kTriggeredData.ePlayer) / 2)
		szUnit = gc.getUnitInfo(capital.getConscriptUnit()).getTextKey()

		szHelp = localText.getText("TXT_KEY_EVENT_PARTISANS_HELP_2", (iNumUnits, szUnit, capital.getNameKey()))

	return szHelp

def canApplyPartisans2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)

	return (max(1, getNumPartisanUnits(plot, kTriggeredData.ePlayer) / 2) > 0)

def applyPartisans2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	capital = player.getCapitalCity()
	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)

	if None != capital and not capital.isNone():
		iNumUnits = max(1, getNumPartisanUnits(plot, kTriggeredData.ePlayer) / 2)
		for i in range(iNumUnits):
			player.initUnit(capital.getConscriptUnit(), capital.getX(), capital.getY(), UnitAITypes.UNITAI_ATTACK, DirectionTypes.DIRECTION_SOUTH)

######## GREED ###########

def canTriggerGreed(argsList):
	kTriggeredData = argsList[0]
	trigger = gc.getEventTriggerInfo(kTriggeredData.eTrigger)
	player = gc.getPlayer(kTriggeredData.ePlayer)
	otherPlayer = gc.getPlayer(kTriggeredData.eOtherPlayer)

	if not gc.getTeam(player.getTeam()).canChangeWarPeace(otherPlayer.getTeam()):
		return False

	listBonuses = []
	iOil = gc.getInfoTypeForString("BONUS_OIL")
	if 0 == player.getNumAvailableBonuses(iOil):
		listBonuses.append(iOil)
	iIron = gc.getInfoTypeForString("BONUS_IRON")
	if 0 == player.getNumAvailableBonuses(iIron):
		listBonuses.append(iIron)
	iHorse = gc.getInfoTypeForString("BONUS_HORSE")
	if 0 == player.getNumAvailableBonuses(iHorse):
		listBonuses.append(iHorse)
	iCopper = gc.getInfoTypeForString("BONUS_COPPER")
	if 0 == player.getNumAvailableBonuses(iCopper):
		listBonuses.append(iCopper)

	map = gc.getMap()
	bFound = False
	listPlots = []
	for iBonus in listBonuses:
		for i in range(map.numPlots()):
			loopPlot = map.plotByIndex(i)
			if loopPlot.getOwner() == kTriggeredData.eOtherPlayer and loopPlot.getBonusType(player.getTeam()) == iBonus and loopPlot.isRevealed(player.getTeam(), False) and not loopPlot.isWater():
				listPlots.append(loopPlot)
				bFound = True
		if bFound:
			break

	if not bFound:
		return False

	plot = listPlots[gc.getGame().getSorenRandNum(len(listPlots), "Greed event plot selection")]

	if -1 == getGreedUnit(player, plot):
		return False

	kActualTriggeredDataObject = player.getEventTriggered(kTriggeredData.iId)
	kActualTriggeredDataObject.iPlotX = plot.getX()
	kActualTriggeredDataObject.iPlotY = plot.getY()

	return True

def getHelpGreed1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	otherPlayer = gc.getPlayer(kTriggeredData.eOtherPlayer)
	iBonus = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY).getBonusType(player.getTeam())

	iTurns = gc.getGameSpeedInfo(gc.getGame().getGameSpeedType()).getGrowthPercent()

	szHelp = localText.getText("TXT_KEY_EVENT_GREED_HELP_1", (otherPlayer.getCivilizationShortDescriptionKey(), gc.getBonusInfo(iBonus).getTextKey(), iTurns))

	return szHelp

def expireGreed1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	otherPlayer = gc.getPlayer(kTriggeredData.eOtherPlayer)
	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)

	if plot.getOwner() == kTriggeredData.ePlayer or plot.getOwner() == -1:
		return False

	if gc.getGame().getGameTurn() >= kTriggeredData.iTurn + gc.getGameSpeedInfo(gc.getGame().getGameSpeedType()).getGrowthPercent():
		return True

	if plot.getOwner() != kTriggeredData.eOtherPlayer:
		return True

	return False

def canTriggerGreedDone(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	trigger = gc.getEventTriggerInfo(kTriggeredData.eTrigger)
	kOrigTriggeredData = player.getEventOccured(trigger.getPrereqEvent(0))
	plot = gc.getMap().plot(kOrigTriggeredData.iPlotX, kOrigTriggeredData.iPlotY)

	if plot.getOwner() != kOrigTriggeredData.ePlayer:
		return False

	if -1 == getGreedUnit(player, plot):
		return False

	kActualTriggeredDataObject = player.getEventTriggered(kTriggeredData.iId)
	kActualTriggeredDataObject.iPlotX = kOrigTriggeredData.iPlotX
	kActualTriggeredDataObject.iPlotY = kOrigTriggeredData.iPlotY
	kActualTriggeredDataObject.eOtherPlayer = kOrigTriggeredData.eOtherPlayer

	return True

def getGreedUnit(player, plot):
	iBonus = plot.getBonusType(player.getTeam())
	iBestValue = 0
	iBestUnit = -1
	for iUnitClass in range(gc.getNumUnitClassInfos()):
		iUnit = gc.getCivilizationInfo(player.getCivilizationType()).getCivilizationUnits(iUnitClass)
		if -1 != iUnit and player.canTrain(iUnit, False, False) and (gc.getUnitInfo(iUnit).getDomainType() == DomainTypes.DOMAIN_LAND):
			iValue = 0
			if gc.getUnitInfo(iUnit).getPrereqAndBonus() == iBonus:
				iValue = player.AI_unitValue(iUnit, UnitAITypes.UNITAI_ATTACK, plot.area())
			else:
				for j in range(gc.getNUM_UNIT_PREREQ_OR_BONUSES()):
					if (gc.getUnitInfo(iUnit).getPrereqOrBonuses(j) == iBonus):
						iValue = player.AI_unitValue(iUnit, UnitAITypes.UNITAI_ATTACK, plot.area())
						break
			if iValue > iBestValue:
				iBestValue = iValue
				iBestUnit = iUnit

	return iBestUnit


def getHelpGreedDone1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)

	iNumUnits = gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers() / 2 + 1
	iUnitType = getGreedUnit(player, plot)

	if iUnitType != -1:
		szHelp = localText.getText("TXT_KEY_EVENT_GREED_DONE_HELP_1", (iNumUnits, gc.getUnitInfo(iUnitType).getTextKey()))

	return szHelp

def applyGreedDone1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)

	iUnitType = getGreedUnit(player, plot)
	iNumUnits = gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers() / 2 + 1

	if iUnitType != -1:
		for i in range(iNumUnits):
			player.initUnit(iUnitType, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK, DirectionTypes.DIRECTION_SOUTH)


######## WAR CHARIOTS ###########

def canTriggerWarChariots(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	kActualTriggeredDataObject = player.getEventTriggered(kTriggeredData.iId)
	kActualTriggeredDataObject.eReligion = ReligionTypes(player.getStateReligion())

	return True

def getHelpWarChariots1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	iNumUnits = gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers() + 1
	szHelp = localText.getText("TXT_KEY_EVENT_WAR_CHARIOTS_HELP_1", (iNumUnits, ))

	return szHelp

def canTriggerWarChariotsDone(argsList):
	kTriggeredData = argsList[0]
	trigger = gc.getEventTriggerInfo(kTriggeredData.eTrigger)
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iNumUnits = gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers() + 1
	iUnitClassType = gc.getInfoTypeForString("UNITCLASS_CHARIOT")
	if player.getUnitClassCount(iUnitClassType) < iNumUnits:
		return False

	kOrigTriggeredData = player.getEventOccured(trigger.getPrereqEvent(0))
	kActualTriggeredDataObject = player.getEventTriggered(kTriggeredData.iId)
	kActualTriggeredDataObject.eReligion = kOrigTriggeredData.eReligion

	return True

######## ELITE SWORDSMEN ###########

def getHelpEliteSwords1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	iNumUnits = gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers() + 1
	szHelp = localText.getText("TXT_KEY_EVENT_ELITE_SWORDS_HELP_1", (iNumUnits, ))

	return szHelp

def canTriggerEliteSwordsDone(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iNumUnits = gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers() + 1
	iUnitClassType = gc.getInfoTypeForString("UNITCLASS_LIGHT_SWORDSMAN")
	iUnitClassType2 = gc.getInfoTypeForString("UNITCLASS_SWORDSMAN")
	iUnitClassType3 = gc.getInfoTypeForString("UNITCLASS_HEAVY_SWORDSMAN")
	if (player.getUnitClassCount(iUnitClassType) + player.getUnitClassCount(iUnitClassType2) + player.getUnitClassCount(iUnitClassType3)) < iNumUnits:
		return False

	return True


def canApplyEliteSwordsDone2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iCivic = gc.getInfoTypeForString("CIVIC_HEREDITARY_RULE")

	if not player.isCivic(iCivic):
		return False

	return True

######## WARSHIPS ###########

def canTriggerWarships(argsList):
	kTriggeredData = argsList[0]

	map = gc.getMap()
	iNumWater = 0

	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)

		if plot.isWater():
			iNumWater += 1

		if 100 * iNumWater >= 55 * map.numPlots():
			return True

	return False

def getHelpWarships1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	iNumUnits = gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers()

	iBuilding = gc.getInfoTypeForString("BUILDING_GREAT_LIGHTHOUSE")
	szHelp = localText.getText("TXT_KEY_EVENT_WARSHIPS_HELP_1", (iNumUnits, gc.getBuildingInfo(iBuilding).getTextKey()))

	return szHelp

def canTriggerWarshipsDone(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iNumUnits = gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers()
	iUnitClassType = gc.getInfoTypeForString("UNITCLASS_TRIREME")

	if player.getUnitClassCount(iUnitClassType) < iNumUnits:
		return False

	return True


def canApplyWarshipsDone2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iBuilding = gc.getInfoTypeForString("BUILDING_GREAT_LIGHTHOUSE")
	if player.getBuildingClassCountWithUpgrades(gc.getBuildingInfo(iBuilding).getBuildingClassType()) == 0:
		return False

	return True

######## GUNS NOT BUTTER ###########

def getHelpGunsButter1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	iNumUnits = gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers() + 1
	iBuilding = gc.getInfoTypeForString("BUILDING_TAJ_MAHAL")

	szHelp = localText.getText("TXT_KEY_EVENT_GUNS_BUTTER_HELP_1", (iNumUnits, gc.getBuildingInfo(iBuilding).getTextKey()))

	return szHelp

def canTriggerGunsButterDone(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iNumUnits = gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers() + 1
	iUnitClassType = gc.getInfoTypeForString("UNITCLASS_MUSKETMAN")
	iUnitClassTypeTwo = gc.getInfoTypeForString("UNITCLASS_RIFLEMAN")

	if (player.getUnitClassCount(iUnitClassType) + player.getUnitClassCount(iUnitClassTypeTwo)) < iNumUnits:
		return False

	return True


def canApplyGunsButterDone2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iCivic = gc.getInfoTypeForString("CIVIC_ARISTOCRACY")

	if not player.isCivic(iCivic):
		return False

	return True

def canApplyGunsButterDone3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iBuilding = gc.getInfoTypeForString("BUILDING_TAJ_MAHAL")
	if player.getBuildingClassCountWithUpgrades(gc.getBuildingInfo(iBuilding).getBuildingClassType()) == 0:
		return False

	return True

######## NOBLE KNIGHTS ###########

def canTriggerNobleKnights(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	kActualTriggeredDataObject = player.getEventTriggered(kTriggeredData.iId)
	kActualTriggeredDataObject.eReligion = ReligionTypes(player.getStateReligion())

	return True

def getHelpNobleKnights1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	iNumUnits = gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers() + 1
	iBuilding = gc.getInfoTypeForString("BUILDING_ORACLE")

	szHelp = localText.getText("TXT_KEY_EVENT_NOBLE_KNIGHTS_HELP_1", (iNumUnits, gc.getBuildingInfo(iBuilding).getTextKey()))

	return szHelp

def canTriggerNobleKnightsDone(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iNumUnits = gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers() + 1
	iUnitClassTypeRdr = gc.getInfoTypeForString("UNITCLASS_HEAVY_CAVALRY")
	iUnitClassTypeKn = gc.getInfoTypeForString("UNITCLASS_KNIGHT")
	iUnitClassTypeNKn = gc.getInfoTypeForString("UNITCLASS_CHIVALRY")

	if (player.getUnitClassCount(iUnitClassTypeRdr) + player.getUnitClassCount(iUnitClassTypeKn) + player.getUnitClassCount(iUnitClassTypeNKn)) < iNumUnits:
		return False

	trigger = gc.getEventTriggerInfo(kTriggeredData.eTrigger)
	kOrigTriggeredData = player.getEventOccured(trigger.getPrereqEvent(0))
	kActualTriggeredDataObject = player.getEventTriggered(kTriggeredData.iId)
	kActualTriggeredDataObject.eReligion = kOrigTriggeredData.eReligion

	iBuilding = gc.getInfoTypeForString("BUILDING_ORACLE")

	(loopCity, iter) = player.firstCity(False)
	while(loopCity):
		if (loopCity.isHasBuilding(iBuilding)):
			kActualTriggeredDataObject.iPlotX = loopCity.getX()
			kActualTriggeredDataObject.iPlotY = loopCity.getY()
			kActualTriggeredDataObject.iCityId = loopCity.getID()
			break

		(loopCity, iter) = player.nextCity(iter, False)

	return True

def canApplyNobleKnightsDone2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iCivic = gc.getInfoTypeForString("CIVIC_STATE_CHURCH")

	if not player.isCivic(iCivic):
		return False

	return True

######## OVERWHELM DOCTRINE ###########

def canTriggerOverwhelm(argsList):
	kTriggeredData = argsList[0]

	map = gc.getMap()
	iNumWater = 0

	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if plot.isWater():
			iNumWater += 1
		if 100 * iNumWater >= 55 * map.numPlots():
			return True
	return False

def getHelpOverwhelm1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	iDestroyer = gc.getInfoTypeForString("UNIT_DESTROYER")
	iDestroyerA = gc.getInfoTypeForString("UNIT_WW1_DESTROYER")
	iNumDestroyers = 4
	iBattleship = gc.getInfoTypeForString("UNIT_BATTLESHIP")
	iBattleshipA = gc.getInfoTypeForString("UNIT_WW2_BATTLESHIP")
	iNumBattleships = 2
	iCarrier = gc.getInfoTypeForString("UNIT_CARRIER")
	iCarrierB = gc.getInfoTypeForString("UNIT_NUCLEAR_CARRIER")
	iNumCarriers = 3
	iFighter = gc.getInfoTypeForString("SPECIALUNIT_FIGHTER")
	iNumFighters = 9
	iProject = gc.getInfoTypeForString("PROJECT_MANHATTAN_PROJECT")
	iBuilding = gc.getInfoTypeForString("BUILDING_FIRST_NUCLEAR_TEST")

	szHelp = localText.getText("TXT_KEY_EVENT_OVERWHELM_HELP_1", (iNumDestroyers, gc.getUnitInfo(iDestroyerA).getTextKey(), gc.getUnitInfo(iDestroyer).getTextKey(), iNumBattleships, gc.getUnitInfo(iBattleshipA).getTextKey(), gc.getUnitInfo(iBattleship).getTextKey(), iNumCarriers, gc.getUnitInfo(iCarrier).getTextKey(), gc.getUnitInfo(iCarrierB).getTextKey(), iNumFighters, gc.getSpecialUnitInfo(iFighter).getTextKey(), gc.getProjectInfo(iProject).getTextKey()))
#	szHelp = localText.getText("TXT_KEY_EVENT_OVERWHELM_HELP_1", (iNumDestroyers, gc.getUnitInfo(iDestroyer).getTextKey(), iNumBattleships, gc.getUnitInfo(iBattleship).getTextKey(), iNumCarriers, gc.getUnitInfo(iCarrier).getTextKey(), iNumFighters, gc.getSpecialUnitInfo(iFighter).getTextKey(), gc.getBuildingInfo(iBuilding).getTextKey()))

	return szHelp

def canTriggerOverwhelmDone(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	
	iDestroyer = gc.getInfoTypeForString("UNITCLASS_DESTROYER")
	iDestroyerA = gc.getInfoTypeForString("UNITCLASS_WW1_DESTROYER")
	iNumDestroyers = 4
	if (player.getUnitClassCount(iDestroyer) + player.getUnitClassCount(iDestroyerA)) < iNumDestroyers:
		return False

	iBattleship = gc.getInfoTypeForString("UNITCLASS_BATTLESHIP")
	iBattleshipA = gc.getInfoTypeForString("UNITCLASS_WW2_BATTLESHIP")
	iNumBattleships = 2
	if (player.getUnitClassCount(iBattleship) + player.getUnitClassCount(iBattleshipA)) < iNumBattleships:
		return False

	iCarrier = gc.getInfoTypeForString("UNITCLASS_CARRIER")
	iCarrierB = gc.getInfoTypeForString("UNITCLASS_NUCLEAR_CARRIER")
	iNumCarriers = 3
	if (player.getUnitClassCount(iCarrier) + player.getUnitClassCount(iCarrierB)) < iNumCarriers:
		return False

	iFighter = gc.getInfoTypeForString("SPECIALUNIT_FIGHTER")
	iNumFighters = 9
	iNumPlayerFighters = 0
	(loopUnit, iter) = player.firstUnit(False)
	while (loopUnit):
		for i in range(0, loopUnit.getNumSpecialUnitTypes()):
			if loopUnit.getSpecialUnitType(i) == iFighter:
				iNumPlayerFighters += 1
		(loopUnit, iter) = player.nextUnit(iter, False)

	if iNumPlayerFighters < iNumFighters:
		return False

	return True

def getHelpOverwhelmDone3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_OVERWHELM_DONE_HELP_3", ())

	return szHelp

def canApplyOverwhelmDone3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iProject = gc.getInfoTypeForString("PROJECT_MANHATTAN_PROJECT")

	if gc.getTeam(player.getTeam()).getProjectCount(iProject) == 0:
		return False

	return True

def applyOverwhelmDone3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	gc.getGame().changeNoNukesCount(1)

######## CORPORATE EXPANSION ###########

def canTriggerCorporateExpansion(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	city = gc.getGame().getHeadquarters(kTriggeredData.eCorporation)
	if None == city or city.isNone():
		return False

	# Hack to remember the number of cities you already have with the Corporation
	kActualTriggeredDataObject = player.getEventTriggered(kTriggeredData.iId)
	kActualTriggeredDataObject.iOtherPlayerCityId = gc.getGame().countCorporationLevels(kTriggeredData.eCorporation)
	kActualTriggeredDataObject.iCityId = city.getID()
	kActualTriggeredDataObject.iPlotX = city.getX()
	kActualTriggeredDataObject.iPlotY = city.getY()

	bFound = False
	for iBuilding in range(gc.getNumBuildingInfos()):
		if gc.getBuildingInfo(iBuilding).getFoundsCorporation() == kTriggeredData.eCorporation:
			kActualTriggeredDataObject.eBuilding = BuildingTypes(iBuilding)
			bFound = True
			break

	if not bFound:
		return False

	return True

def expireCorporateExpansion1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	city = player.getCity(kTriggeredData.iCityId)
	if None == city or city.isNone():
		return True

	return False

def getHelpCorporateExpansion1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	iNumCities = gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers() + 1

	szHelp = localText.getText("TXT_KEY_EVENT_CORPORATE_EXPANSION_HELP_1", (gc.getCorporationInfo(kTriggeredData.eCorporation).getTextKey(), iNumCities))

	return szHelp

def canTriggerCorporateExpansionDone(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	trigger = gc.getEventTriggerInfo(kTriggeredData.eTrigger)
	kOrigTriggeredData = player.getEventOccured(trigger.getPrereqEvent(0))

	iNumCitiesRequired = gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers() + 1 + kOrigTriggeredData.iOtherPlayerCityId

	if iNumCitiesRequired > gc.getGame().countCorporationLevels(kOrigTriggeredData.eCorporation):
		return False


	kActualTriggeredDataObject = player.getEventTriggered(kTriggeredData.iId)
	kActualTriggeredDataObject.eCorporation = kOrigTriggeredData.eCorporation
	kActualTriggeredDataObject.eBuilding = kOrigTriggeredData.eBuilding
	kActualTriggeredDataObject.iCityId = kOrigTriggeredData.iCityId
	kActualTriggeredDataObject.iPlotX = kOrigTriggeredData.iPlotX
	kActualTriggeredDataObject.iPlotY = kOrigTriggeredData.iPlotY

	return True

def getHelpCorporateExpansionDone1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_YIELD_CHANGE_BUILDING", (gc.getBuildingInfo(kTriggeredData.eBuilding).getTextKey(), u"+10[ICON_GOLD]"))

	return szHelp

def applyCorporateExpansionDone1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	city = player.getCity(kTriggeredData.iCityId)
	if None != city and not city.isNone():
		city.setBuildingCommerceChange(gc.getBuildingInfo(kTriggeredData.eBuilding).getBuildingClassType(), CommerceTypes.COMMERCE_GOLD, 10)

######## HOSTILE TAKEOVER ###########

def canTriggerHostileTakeover(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_ONE_CITY_CHALLENGE) and gc.getPlayer(kTriggeredData.ePlayer).isHuman():
		return False

	city = gc.getGame().getHeadquarters(kTriggeredData.eCorporation)
	if None == city or city.isNone():
		return False

	# Hack to remember the number of cities you already have with the Corporation
	kActualTriggeredDataObject = player.getEventTriggered(kTriggeredData.iId)
	kActualTriggeredDataObject.iCityId = city.getID()
	kActualTriggeredDataObject.iPlotX = city.getX()
	kActualTriggeredDataObject.iPlotY = city.getY()

	bFound = False
	for iBuilding in range(gc.getNumBuildingInfos()):
		if gc.getBuildingInfo(iBuilding).getFoundsCorporation() == kTriggeredData.eCorporation:
			kActualTriggeredDataObject.eBuilding = BuildingTypes(iBuilding)
			bFound = True
			break

	if not bFound:
		return False

	listResources = getHostileTakeoverListResources(gc.getCorporationInfo(kTriggeredData.eCorporation), player)
	if len(listResources) == 0:
		return False

	return True

def expireHostileTakeover1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	city = player.getCity(kTriggeredData.iCityId)
	if None == city or city.isNone():
		return True

	return False

def getHostileTakeoverListResources(corporation, player):
	map = gc.getMap()
	listHave = []
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if plot.getOwner() == player.getID():
			iBonus = plot.getBonusType(player.getTeam())
			if iBonus != -1:
				if not iBonus in listHave:
					listHave.append(iBonus)
	listNeed = []
	for i in range(gc.getNUM_CORPORATION_PREREQ_BONUSES()):
		iBonus = corporation.getPrereqBonus(i)
		if iBonus != -1:
			if not iBonus in listHave:
				if (gc.getBonusInfo(iBonus).getBonusClassType() != gc.getInfoTypeForString("BONUSCLASS_MANUFACTURED") and gc.getBonusInfo(iBonus).getBonusClassType() != gc.getInfoTypeForString("BONUSCLASS_FUTURE")):
					listNeed.append(iBonus)
	return listNeed

def getHelpHostileTakeover1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	listResources = getHostileTakeoverListResources(gc.getCorporationInfo(kTriggeredData.eCorporation), player)
	szList = u""
	bFirst = True
	for iBonus in listResources:
		if not bFirst:
			szList += u", "
		else:
			bFirst = False
		szList += u"[COLOR_HIGHLIGHT_TEXT]" + gc.getBonusInfo(iBonus).getDescription() + u"[COLOR_REVERT]"

	szHelp = localText.getText("TXT_KEY_EVENT_HOSTILE_TAKEOVER_HELP_1", (len(listResources), szList))

	return szHelp

def canTriggerHostileTakeoverDone(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	trigger = gc.getEventTriggerInfo(kTriggeredData.eTrigger)
	kOrigTriggeredData = player.getEventOccured(trigger.getPrereqEvent(0))

	listResources = getHostileTakeoverListResources(gc.getCorporationInfo(kOrigTriggeredData.eCorporation), player)

	if len(listResources) > 0:
		return False

	kActualTriggeredDataObject = player.getEventTriggered(kTriggeredData.iId)
	kActualTriggeredDataObject.eCorporation = kOrigTriggeredData.eCorporation
	kActualTriggeredDataObject.eBuilding = kOrigTriggeredData.eBuilding
	kActualTriggeredDataObject.iCityId = kOrigTriggeredData.iCityId
	kActualTriggeredDataObject.iPlotX = kOrigTriggeredData.iPlotX
	kActualTriggeredDataObject.iPlotY = kOrigTriggeredData.iPlotY

	return True

def getHelpHostileTakeoverDone1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_YIELD_CHANGE_BUILDING", (gc.getBuildingInfo(kTriggeredData.eBuilding).getTextKey(), u"+20[ICON_GOLD]"))

	return szHelp

def applyHostileTakeoverDone1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	city = player.getCity(kTriggeredData.iCityId)
	if None != city and not city.isNone():
		city.setBuildingCommerceChange(gc.getBuildingInfo(kTriggeredData.eBuilding).getBuildingClassType(), CommerceTypes.COMMERCE_GOLD, 20)


######## Great Beast ########

def doGreatBeast3(argsList):
	kTriggeredData = argsList[1]

	player = gc.getPlayer(kTriggeredData.ePlayer)
	(loopCity, iter) = player.firstCity(False)

	while(loopCity):
		if loopCity.isHasReligion(kTriggeredData.eReligion):
			loopCity.changeHappinessTimer(40)
		(loopCity, iter) = player.nextCity(iter, False)

def getHelpGreatBeast3(argsList):
	kTriggeredData = argsList[1]
	religion = gc.getReligionInfo(kTriggeredData.eReligion)

	szHelp = localText.getText("TXT_KEY_EVENT_GREAT_BEAST_3_HELP", (gc.getDefineINT("TEMP_HAPPY"), 40, religion.getChar()))

	return szHelp

####### Comet Fragment ########

def canDoCometFragment(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	if (player.getSpaceProductionModifier()) > 10:
		return False

	return True

####### Immigrants ########

def canTriggerImmigrantCity(argsList):
	ePlayer = argsList[1]
	iCity = argsList[2]

	player = gc.getPlayer(ePlayer)
	city = player.getCity(iCity)

	if city.isNone():
		return False

	if ((city.happyLevel() - city.unhappyLevel(0) < 1) or (city.goodHealth() - city.badHealth(True) < 1)):
		return False

	if (city.getCommerceRateTimes100(CommerceTypes.COMMERCE_CULTURE) < 5500):
		return False

####### Controversial Philosopher ######

def canTriggerControversialPhilosopherCity(argsList):
	ePlayer = argsList[1]
	iCity = argsList[2]

	player = gc.getPlayer(ePlayer)
	city = player.getCity(iCity)

	if city.isNone():
		return False
	if (not city.isCapital()):
		return False
	if (city.getCommerceRateTimes100(CommerceTypes.COMMERCE_RESEARCH) < 3500):
		return False

	return True

####### Spy Discovered #######


def canDoSpyDiscovered3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	player = gc.getPlayer(kTriggeredData.ePlayer)

	if player.getCapitalCity().isNone():
		return False

	return True

def doSpyDiscovered3(argsList):
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	team = gc.getTeam(player.getTeam())

	plot = player.getCapitalCity().plot()
	iNumUnits = player.getNumCities() / 4
	if iNumUnits < 1:
		iNumUnits = 1
	
	iMechWar = gc.getInfoTypeForString("TECH_MECHANIZED_WARFARE")
	iArmVeh = gc.getInfoTypeForString("TECH_MOTORIZED_TRANSPORTATION")
	iAutoWeap = gc.getInfoTypeForString("TECH_AUTOMATIC_WEAPONS")
	iSteel = gc.getInfoTypeForString("BONUS_STEEL")
	iDiesel = gc.getInfoTypeForString("BONUS_DIESEL")

	iUnitClassType = gc.getInfoTypeForString("UNITCLASS_RIFLEMAN")
	if (iAutoWeap > 0):
		if (team.isHasTech(iAutoWeap)):
			iUnitClassType = gc.getInfoTypeForString("UNITCLASS_INFANTRY")
	if player.hasBonus(iSteel) and player.hasBonus(iDiesel):
		if (iArmVeh > 0):
			if (team.isHasTech(iArmVeh)):
				iUnitClassType = gc.getInfoTypeForString("UNITCLASS_EARLY_TANK")
		if (iMechWar > 0):
			if (team.isHasTech(iMechWar)):
				iUnitClassType = gc.getInfoTypeForString("UNITCLASS_TANK")

	iUnitType = gc.getCivilizationInfo(player.getCivilizationType()).getCivilizationUnits(iUnitClassType)

	if iUnitType != -1:
		for i in range(iNumUnits):
			player.initUnit(iUnitType, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK, DirectionTypes.DIRECTION_SOUTH)

def getHelpSpyDiscovered3(argsList):
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	team = gc.getTeam(player.getTeam())
	iNumUnits = player.getNumCities() / 4
	if iNumUnits < 1:
		iNumUnits = 1

	iMechWar = gc.getInfoTypeForString("TECH_MECHANIZED_WARFARE")
	iArmVeh = gc.getInfoTypeForString("TECH_MOTORIZED_TRANSPORTATION")
	iAutoWeap = gc.getInfoTypeForString("TECH_AUTOMATIC_WEAPONS")
	iSteel = gc.getInfoTypeForString("BONUS_STEEL")
	iDiesel = gc.getInfoTypeForString("BONUS_DIESEL")

	iUnitClassType = gc.getInfoTypeForString("UNITCLASS_RIFLEMAN")
	if (iAutoWeap > 0):
		if (team.isHasTech(iAutoWeap)):
			iUnitClassType = gc.getInfoTypeForString("UNITCLASS_INFANTRY")
	if player.hasBonus(iSteel) and player.hasBonus(iDiesel):
		if (iArmVeh > 0):
			if (team.isHasTech(iArmVeh)):
				iUnitClassType = gc.getInfoTypeForString("UNITCLASS_EARLY_TANK")
		if (iMechWar > 0):
			if (team.isHasTech(iMechWar)):
				iUnitClassType = gc.getInfoTypeForString("UNITCLASS_TANK")

	iUnitType = gc.getCivilizationInfo(player.getCivilizationType()).getCivilizationUnits(iUnitClassType)
	szHelp = localText.getText("TXT_KEY_EVENT_BONUS_UNIT", (iNumUnits, gc.getUnitInfo(iUnitType).getTextKey()))

	return szHelp

####### Nuclear Protest #######

def canTriggerNuclearProtest(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iICBMClass = gc.getInfoTypeForString("UNITCLASS_ICBM")
	iTacNukeClass = gc.getInfoTypeForString("UNITCLASS_TACTICAL_NUKE")
	if player.getNumNukeUnits() < 10:
		return False

	return True

def doNuclearProtest1(argsList):
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	(loopUnit, iter) = player.firstUnit(False)
	while (loopUnit):
		if (loopUnit.nukeRange() > -1 or loopUnit.getUnitAIType() == UnitAITypes.UNITAI_ICBM and not loopUnit.isNone()):
			loopUnit.kill(False, -1)
		(loopUnit, iter) = player.nextUnit(iter, False)

def getHelpNuclearProtest1(argsList):
	szHelp = localText.getText("TXT_KEY_EVENT_NUCLEAR_PROTEST_1_HELP", ())
	return szHelp


######## Preaching Researcher #######

def canTriggerPreachingResearcherCity(argsList):
	iCity = argsList[2]

	player = gc.getPlayer(argsList[1])
	city = player.getCity(iCity)

	if city.isHasBuilding(gc.getInfoTypeForString("BUILDING_UNIVERSITY")):
		return True
	return False

######## Toxcatl (Aztec event) #########

def canTriggerToxcatl(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	if (player.getCivilizationType() == gc.getInfoTypeForString("CIVILIZATION_AZTEC")):
		return True
	return False

######## Dissident Priest (Egyptian event) #######

def canTriggerDissidentPriest(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	if (player.getCivilizationType() == gc.getInfoTypeForString("CIVILIZATION_EGYPT")):
		return True
	return False

def canTriggerDissidentPriestCity(argsList):
	iCity = argsList[2]

	player = gc.getPlayer(argsList[1])
	city = player.getCity(iCity)

	if city.isGovernmentCenter():
		return False
	if (city.getCommerceRateTimes100(CommerceTypes.COMMERCE_CULTURE) < 3000):
		return False

	return True

######## Rogue Station  (Russian event) ###########

def canTriggerRogueStation(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	if (player.getCivilizationType() == gc.getInfoTypeForString("CIVILIZATION_RUSSIA")):
		return True
	return False

######## Antimonarchists (French event) #########

def canTriggerAntiMonarchists(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	if (player.getCivilizationType() == gc.getInfoTypeForString("CIVILIZATION_FRANCE")):
		return True
	return False

######## Impeachment (American event) ########

def canTriggerImpeachment(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	if (player.getCivilizationType() == gc.getInfoTypeForString("CIVILIZATION_AMERICA")):
		return True
	return False

def canTriggerImpeachmentCity(argsList):
	iCity = argsList[2]

	player = gc.getPlayer(argsList[1])
	city = player.getCity(iCity)

	if city.isCapital():
		return True
	return False

######## FUTBOL_WAR #######
#Need to stop vassals from being selected as "other player" b/c can't DoW them.  Also, b/c this event railroads other player into war, don't
#want that other player to be a Human in multiplayer game.  (Assuming this event wouldn't OOS in MP anyway.)
#Don't know how the AI will handle the event, but we'll see.  otherPlayer.isHuman() may be redundant as bSinglePlayer = 1 selected in XML.


def canTriggerFutbol_War(argslist):

	kTriggeredData = argslist[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	otherPlayer = gc.getPlayer(kTriggeredData.eOtherPlayer)

	if not gc.getTeam(otherPlayer.getTeam()).canChangeWarPeace(pPlayer.getTeam()):
		return False
	if gc.getTeam(otherPlayer.getTeam()).isAVassal():
		return False
	if gc.getTeam(pPlayer.getTeam()).isAVassal():
		return False
	if otherPlayer.isHuman():
		return False

	return True

######## NAPALM ########
#Pacifist civs won't be inventing napalm bombs

def canTriggerNapalm(argsList):

	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)

# Rise of Mankind 2.5 start - pacifism no longer exist in mod so this check is removed
	#if pPlayer.isCivic(gc.getInfoTypeForString("CIVIC_PACIFISM")):
	#	return False
# Rise of Mankind 2.5 end
	return True

def getHelpNapalm1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_NAPALM_HELP_1", ())

	return szHelp

def applyNapalm1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive() and iPlayer != kTriggeredData.ePlayer:
			loopTeam = gc.getTeam(loopPlayer.getTeam())
			if loopTeam.isHasMet(gc.getPlayer(kTriggeredData.ePlayer).getTeam()):
				loopPlayer.AI_changeAttitudeExtra(kTriggeredData.ePlayer, -1)

def getHelpNapalm2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_NAPALM_HELP_2", ())

	return szHelp

######## BILLIONS_AND_BILLIONS ########
#Carl Sagan doesn't like theocracies -- he only likes free speech

def canTriggerBillionsandBillions(argsList):

	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)

	if pPlayer.isCivic(gc.getInfoTypeForString("CIVIC_DIVINE_RULE")) or pPlayer.isCivic(gc.getInfoTypeForString("CIVIC_INTOLERANT")):
		return False
	if ( pPlayer.isCivic(gc.getInfoTypeForString("CIVIC_DEMOCRACY")) or pPlayer.isCivic(gc.getInfoTypeForString("CIVIC_FEDERAL"))) and pPlayer.isCivic(gc.getInfoTypeForString("CIVIC_LIBERAL")):
		return True

	return False

def getHelpBillionsandBillions2 (argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_HELP_BILLIONS_AND_BILLIONS_2", ())

	return szHelp

def canApplyBillionsandBillions2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iBuilding = gc.getInfoTypeForString("BUILDING_HOLLYWOOD")
	if player.getBuildingClassCountWithUpgrades(gc.getBuildingInfo(iBuilding).getBuildingClassType()) == 0:
		return False

	return True


################## FREE_ENTERPRISE (Free Market only) ###################

def canTriggerFreeEnterprise(argsList):

	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)

	if pPlayer.isCivic(gc.getInfoTypeForString("CIVIC_LAISSEZ_FAIRE")):
		return True

	return False

def getHelpFreeEnterprise2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_HELP_FREE_ENTERPRISE_2", ())

	return szHelp


#################  FIAT_MONEY ##########################
# Must be poor, can't be running Decentralization

def canTriggerFiatMoney(argsList):

	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)

	if pPlayer.getGold() > 100:
		return False
	if pPlayer.isCivic(gc.getInfoTypeForString("CIVIC_BARTER")):
		return False

	return True


############## CROSSING_THE_RUBICON (Greece/Carthage/Rome only) ##################
# Only want Representation and Universal Suffrage, must be one of the classical republican civilizations of the
# Mediterranean, and must take place in the Ancient/classical eras (Pyramid->early Representation folks, this one's for you)
# Must select a unit that is experienced (Now 10XP as of 2/26/08,) and doesn't have any of
# the Morale promotion.  (CAN be an MGL unit, doesn't have to be.)  Must select city, and it must be the capital.
# Civ can't be in anarchy; civ must be eligible to undergo revolution.
# pPlayer can change civics immediately after applyRubicon3; no way around it I could tell b/c per the API, there
# is no way to force RevolutionTimer to a specific setting.  (pls advise if you know a way; I would like to change it)


def canTriggerRubicon(argsList):

	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	R = pPlayer.getRevolutionTimer()
	A = pPlayer.isAnarchy()
	HerdRule = gc.getInfoTypeForString("CIVIC_HEREDITARY_RULE")
	Nob = gc.getInfoTypeForString("CIVIC_ARISTOCRACY")

	if R>0:
		return False
	if A>0:
		return False
	if pPlayer.canDoCivics(HerdRule)==0 and pPlayer.canDoCivics(Nob)==0:
		return False
	if pPlayer.isCivic(gc.getInfoTypeForString("CIVIC_REPUBLIC")) or pPlayer.isCivic(gc.getInfoTypeForString("CIVIC_DEMOCRACY")) or pPlayer.isCivic(gc.getInfoTypeForString("CIVIC_FEDERAL")) or pPlayer.isCivic(gc.getInfoTypeForString("CIVIC_SENATE")):
		return True

	return False

def canTriggerRubiconCity(argsList):
	iCity = argsList[2]

	pPlayer = gc.getPlayer(argsList[1])
	city = pPlayer.getCity(iCity)

	if city.isCapital():
		return True

	return False

def canTriggerRubiconUnit(argsList):
	eTrigger = argsList[0]
	ePlayer = argsList[1]
	iUnit = argsList[2]

	pPlayer = gc.getPlayer(ePlayer)
	unit = pPlayer.getUnit(iUnit)

	if unit.isNone():
		return False

	if unit.getExperience() < 10:
		return False

	iMorale = gc.getInfoTypeForString("PROMOTION_MORALE")
	if unit.isHasPromotion(iMorale):
		return False

	return True

def getHelpRubicon2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	unit = pPlayer.getUnit(kTriggeredData.iUnitId)

	iMorale = gc.getInfoTypeForString("PROMOTION_MORALE")

	szHelp = localText.getText("TXT_KEY_EVENT_CROSSING_THE_RUBICON_2_HELP", (unit.getNameKey(), gc.getPromotionInfo(iMorale).getTextKey()))

	return szHelp

def applyRubicon2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	pPlayer2 = gc.getPlayer(kTriggeredData.ePlayer)
	unit2 = pPlayer2.getUnit(kTriggeredData.iUnitId)

	iMorale = gc.getInfoTypeForString("PROMOTION_MORALE")

	unit2.setHasPromotion(iMorale, True)

def getHelpRubicon3(argsList):
	import BugUtil
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	pPlayer3 = gc.getPlayer(kTriggeredData.ePlayer)

	szHelp = localText.getText("TXT_KEY_EVENT_CROSSING_THE_RUBICON_3_HELP", (1, ))
	if not pPlayer3.isCivic(gc.getInfoTypeForString("CIVIC_HEREDITARY_RULE")):
		if pPlayer3.canDoCivics(gc.getInfoTypeForString("CIVIC_HEREDITARY_RULE"))==1:
			szHelp += u"\n"
			szHelp += localText.getText("TXT_KEY_EVENT_CROSSING_THE_RUBICON_4_HELP", (gc.getCivicInfo(gc.getInfoTypeForString("CIVIC_HEREDITARY_RULE")).getTextKey(), ))
	if pPlayer3.isCivic(gc.getInfoTypeForString("CIVIC_SENATE")):
		if pPlayer3.canDoCivics(gc.getInfoTypeForString("CIVIC_ARISTOCRACY"))==1:
			szHelp += u"\n"
			szHelp += localText.getText("TXT_KEY_EVENT_CROSSING_THE_RUBICON_5_HELP", (gc.getCivicInfo(gc.getInfoTypeForString("CIVIC_ARISTOCRACY")).getTextKey(), ))

	return szHelp

def applyRubicon3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	pPlayer3 = gc.getPlayer(kTriggeredData.ePlayer)

	iCivicType = gc.getInfoTypeForString("CIVIC_HEREDITARY_RULE")
	iCivicOptionType = gc.getInfoTypeForString("CIVICOPTION_GOVERNMENT")
	iCivicType2 = gc.getInfoTypeForString("CIVIC_ARISTOCRACY")
	iCivicOptionType2 = gc.getInfoTypeForString("CIVICOPTION_POWER")

	if not pPlayer3.isCivic(iCivicType):
		if pPlayer3.canDoCivics(iCivicType)==1:
			pPlayer3.setCivics(iCivicOptionType, iCivicType)
	if pPlayer3.isCivic(gc.getInfoTypeForString("CIVIC_SENATE")):
		if pPlayer3.canDoCivics(iCivicType2)==1:
			pPlayer3.setCivics(iCivicOptionType2, iCivicType2)
	pPlayer3.changeAnarchyTurns(2)


####### THE_GENERALS_PUTSCH (France only) #####
#Must be French, must be running Rep or Univ. Suffrage, must be at war more than 10 turns.
#Can't be in situation where Police State is impossible b/c gov't civic forced (e.g., by UN Resolution)

def canTriggerGeneralsPutsch(argsList):

	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	otherPlayer = gc.getPlayer(kTriggeredData.eOtherPlayer)
	R = pPlayer.getRevolutionTimer()
	A = pPlayer.isAnarchy()
	PolState = gc.getInfoTypeForString("CIVIC_DESPOTISM")
	Despot = gc.getInfoTypeForString("CIVIC_DESPOTISM")
	HerdRule = gc.getInfoTypeForString("CIVIC_HEREDITARY_RULE")


	if R>0:
		return False
	if A>0:
		return False
	if gc.getTeam(pPlayer.getTeam()).AI_getAtWarCounter(otherPlayer.getTeam()) < 10:
		return False
	if pPlayer.isCivic(Despot):
		return False
	if pPlayer.isCivic(HerdRule):
		return False
	if pPlayer.isCivic(PolState):
		return False
	if pPlayer.canDoCivics(PolState)==0:
		return False

	return True

def canTriggerGeneralsPutschCity(argsList):
	iCity = argsList[2]
	kTriggeredData = argsList[0]

	pPlayer = gc.getPlayer(argsList[1])
	pCity = pPlayer.getCity(iCity)


	if pCity.isGovernmentCenter():
		return True


	return False

def getHelpGeneralsPutsch1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_THE_GENERALS_PUTSCH_1_HELP", ())

	return szHelp

def canDoGeneralsPutsch1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	iEiffel = gc.getInfoTypeForString("BUILDING_EIFFEL_TOWER")
	iBroadcast = gc.getInfoTypeForString("BUILDING_BROADCAST_TOWER")
	EifCnt = pPlayer.getBuildingClassCountWithUpgrades(gc.getBuildingInfo(iEiffel).getBuildingClassType())
	BrodCnt = pPlayer.getBuildingClassCountWithUpgrades(gc.getBuildingInfo(iBroadcast).getBuildingClassType())
	bAggregate = EifCnt + BrodCnt

	if bAggregate < 1:
		return False

	return True

def getHelpGeneralsPutsch3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_THE_GENERALS_PUTSCH_3_HELP", ())

	return szHelp

def applyGeneralsPutsch3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	iCivicType = gc.getInfoTypeForString("CIVIC_DESPOTISM")
	iCivicOptionType = gc.getInfoTypeForString("CIVICOPTION_GOVERNMENT")

	pPlayer.setCivics(iCivicOptionType, iCivicType)
	pPlayer.changeAnarchyTurns(1)

######  CURE_FOR_CANCER ########

def canTriggerCureforCancer(argsList):

	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	iOxford = gc.getInfoTypeForString("BUILDING_OXFORD_UNIVERSITY")

	if pPlayer.isCivic(gc.getInfoTypeForString("CIVIC_DIVINE_RULE")):
		return False
	if pPlayer.getBuildingClassCountWithUpgrades(gc.getBuildingInfo(iOxford).getBuildingClassType()) == 0:
		return False

	return True

def getHelpCureforCancer1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_CURE_FOR_CANCER_HELP_1", (1, ))

	return szHelp

def applyCureforCancer1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive() and iPlayer != kTriggeredData.ePlayer:
			loopTeam = gc.getTeam(loopPlayer.getTeam())
			if loopTeam.isHasMet(gc.getPlayer(kTriggeredData.ePlayer).getTeam()):
				loopPlayer.AI_changeAttitudeExtra(kTriggeredData.ePlayer, 1)



####### CARNATION_REVOLUTION ########


def canTriggerCarnation(argsList):
	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	otherPlayer = gc.getPlayer(kTriggeredData.eOtherPlayer)
	R = pPlayer.getRevolutionTimer()
	A = pPlayer.isAnarchy()
	Rep = gc.getInfoTypeForString("CIVIC_DEMOCRACY")
	StatProp = gc.getInfoTypeForString("CIVIC_PLANNED")


	if R>0:
		return False
	if A>0:
		return False
	if gc.getTeam(pPlayer.getTeam()).AI_getAtWarCounter(otherPlayer.getTeam()) < 10:
		return False
	if not gc.getTeam(otherPlayer.getTeam()).canChangeWarPeace(pPlayer.getTeam()):
		return False
	if pPlayer.isCivic(gc.getInfoTypeForString("CIVIC_DEMOCRACY")):
		return False
	if pPlayer.isCivic(gc.getInfoTypeForString("CIVIC_FEDERAL")):
		return False
	if pPlayer.isCivic(gc.getInfoTypeForString("CIVIC_PLANNED")):
		return False
	if pPlayer.canDoCivics(Rep)==0:
		return False
	if pPlayer.canDoCivics(StatProp)==0:
		return False

	return True

def canTriggerCarnationUnit(argsList):
	eTrigger = argsList[0]
	ePlayer = argsList[1]
	iUnit = argsList[2]

	player = gc.getPlayer(ePlayer)
	unit = player.getUnit(iUnit)

	if unit.isNone():
		return False

	if unit.getExperience() < 10:
		return False

	return True



def doCarnation2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	destPlayer = gc.getPlayer(kTriggeredData.eOtherPlayer)
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	iCivicType1 = gc.getInfoTypeForString("CIVIC_DEMOCRACY")
	iCivicOptionType1 = gc.getInfoTypeForString("CIVICOPTION_GOVERNMENT")
	iCivicType3 = gc.getInfoTypeForString("CIVIC_PLANNED")
	iCivicOptionType3 = gc.getInfoTypeForString("CIVICOPTION_ECONOMY")


	if destPlayer.isHuman():
		# this works only because it's a single-player only event
		popupInfo = CyPopupInfo()
		popupInfo.setButtonPopupType(ButtonPopupTypes.BUTTONPOPUP_PYTHON)
		popupInfo.setText(localText.getText("TXT_KEY_EVENT_CARNATION_REVOLUTION_OTHER_2", (pPlayer.getCivilizationAdjectiveKey(), )))
		popupInfo.setData1(kTriggeredData.eOtherPlayer)
		popupInfo.setData2(kTriggeredData.ePlayer)
		popupInfo.setPythonModule("CvRandomEventInterface")
		popupInfo.setOnClickedPythonCallback("Carnation2Callback")
		popupInfo.addPythonButton(localText.getText("TXT_KEY_POPUP_YES", ()), "")
		popupInfo.addPythonButton(localText.getText("TXT_KEY_POPUP_NO", ()), "")
		popupInfo.addPopup(kTriggeredData.eOtherPlayer)
	else:
		destPlayer.forcePeace(kTriggeredData.ePlayer)
		pPlayer.setCivics(iCivicOptionType1, iCivicType1)
		pPlayer.setCivics(iCivicOptionType3, iCivicType3)
		pPlayer.changeAnarchyTurns(2)

	return

def Carnation2Callback(argsList):
	iButton = argsList[0]
	iData1 = argsList[1]
	iData2 = argsList[2]
	iData3 = argsList[3]
	szText = argsList[4]
	bOption1 = argsList[5]
	bOption2 = argsList[6]

	if iButton == 0:
		destPlayer = gc.getPlayer(iData1)
		player = gc.getPlayer(iData2)
		destPlayer.forcePeace(iData2)
		destPlayer.AI_changeAttitudeExtra(iData2, 1)
		player.AI_changeAttitudeExtra(iData1, 1)

	return 0

def getHelpCarnation2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	destPlayer = gc.getPlayer(kTriggeredData.eOtherPlayer)

	# Get help text
	szHelp = localText.getText("TXT_KEY_EVENT_CARNATION_REVOLUTION_HELP_2", (1, destPlayer.getNameKey()));

	return szHelp


#####  SYNTHETIC_FUELS  #####

def canTriggerSyntheticFuels(argsList):
	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	iEthanol = gc.getInfoTypeForString("BUILDING_CORPORATION_3")
	bOil = gc.getInfoTypeForString("BONUS_OIL")
	bCoal = gc.getInfoTypeForString("BONUS_COAL")

	if pPlayer.getBuildingClassCountWithUpgrades(gc.getBuildingInfo(iEthanol).getBuildingClassType()) > 0:
		return False
	if pPlayer.hasBonus(bOil) > 0:
		return False
	if pPlayer.hasBonus(bCoal) < 1:
		return False
	for i in range(gc.getNumBuildingInfos()):
		if gc.getBuildingInfo(i).getFreeBonus() == bOil:
			if pPlayer.countNumBuildings(i) > 0:
				return False
	return True

def canTriggerCitySyntheticFuels(argsList):
	iCity = argsList[2]

	pPlayer = gc.getPlayer(argsList[1])
	pCity = pPlayer.getCity(iCity)

	if pCity.isGovernmentCenter():
		return False

	return True


def getHelpSyntheticFuels1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	pCity = pPlayer.getCity(kTriggeredData.iCityId)

	oBonus = gc.getInfoTypeForString("BONUS_OIL")

	szHelp = localText.getText("TXT_KEY_EVENT_SYNTHETIC_FUELS_HELP_1", ( 1, gc.getBonusInfo(oBonus).getChar(), pCity.getNameKey()))

	return szHelp

def getHelpSyntheticFuels2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	pCity = pPlayer.getCity(kTriggeredData.iCityId)

	oBonus = gc.getInfoTypeForString("BONUS_OIL")

	szHelp = localText.getText("TXT_KEY_EVENT_SYNTHETIC_FUELS_HELP_2", ( 1, gc.getBonusInfo(oBonus).getChar(), pCity.getNameKey()))

	return szHelp

def getHelpSyntheticFuels3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_SYNTHETIC_FUELS_HELP_3", (1, ))

	return szHelp

def getHelpSyntheticFuels4(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	pCity = pPlayer.getCity(kTriggeredData.iCityId)

	oBonus = gc.getInfoTypeForString("BONUS_DIESEL")

	szHelp = localText.getText("TXT_KEY_EVENT_SYNTHETIC_FUELS_HELP_4", (1, gc.getBonusInfo(oBonus).getChar(), pCity.getNameKey()))

	return szHelp

def doSyntheticFuels4(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	pCity = pPlayer.getCity(kTriggeredData.iCityId)

	iBonus = gc.getInfoTypeForString("BONUS_DIESEL")

	pCity.changeFreeBonus(iBonus, 1)

	return 1


####### ALTERNATIVE_ENERGY ######

def canTriggerAlternativeEnergy(argsList):
	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	i3Gorges = gc.getInfoTypeForString("BUILDING_GREAT_DAM")
	iCoalPlant = gc.getInfoTypeForString("BUILDINGCLASS_COAL_PLANT")
	iOilPlant = gc.getInfoTypeForString("BUILDINGCLASS_OIL_PLANT")

	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_ONE_CITY_CHALLENGE) and pPlayer.isHuman():
		return False
	if pPlayer.getBuildingClassCountWithUpgrades(gc.getBuildingInfo(i3Gorges).getBuildingClassType()) > 0:
		return False
	if pPlayer.getBuildingClassCount(iCoalPlant) + pPlayer.getBuildingClassCount(iOilPlant) == 0:
		return False

	return True


def getHelpAlternativeEnergy1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)

	iRequired = gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers()

	szHelp = localText.getText("TXT_KEY_EVENT_ALTERNATIVE_ENERGY_HELP_1", (iRequired, ))

	return szHelp


def expireAlternativeEnergy1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	i3Gorges = gc.getInfoTypeForString("BUILDING_GREAT_DAM")

	if pPlayer.getBuildingClassCountWithUpgrades(gc.getBuildingInfo(i3Gorges).getBuildingClassType()) > 0:
		return True

	return False

def canTriggerAlternativeEnergyDone(argsList):
	kTriggeredData = argsList[0]
	trigger = gc.getEventTriggerInfo(kTriggeredData.eTrigger)
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	i3Gorges = gc.getInfoTypeForString("BUILDING_GREAT_DAM")

	iNuke = gc.getInfoTypeForString("BUILDINGCLASS_NUCLEAR_PLANT")
	iBuildingsRequired = gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers()

	if iBuildingsRequired > pPlayer.getBuildingClassCountWithUpgrades(iNuke):
		return False

	return True

def canDoAlternativeEnergy4(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)

	if pPlayer.isCivic(gc.getInfoTypeForString("CIVIC_ENVIRONMENTALISM")):
		return True

	return False


def getHelpAlternativeEnergy4(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_ALTERNATIVE_ENERGY_4_HELP", ())

	return szHelp


#################  MORE_FIAT_MONEY ##########################
# Must be poor, can't be running Decentralization; same as Fiat_Money

def canTriggerMoreFiatMoney(argsList):

	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)

	if pPlayer.getGold() > 100:
		return False
	if pPlayer.isCivic(gc.getInfoTypeForString("CIVIC_BARTER")):
		return False

	return True



##### INDUSTRIAL_ACTION #####

def canTriggerIndustrialAction(argsList):

	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)

	if pPlayer.isCivic(gc.getInfoTypeForString("CIVIC_SLAVERY")):
		return False

	return True

def canDoTriggerCityIndustrialAction(argsList):

	iCity = argsList[2]
	pPlayer = gc.getPlayer(argsList[1])
	pCity = pPlayer.getCity(iCity)

	if pCity.isGovernmentCenter():
		return False

	return True

def canDoIndustrialAction2(argsList):

	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)

	if pPlayer.isCivic(gc.getInfoTypeForString("CIVIC_PLANNED")):
		return False

	return True

def getHelpIndustrialAction2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_INDUSTRIAL_ACTION_2_HELP", ())

	return szHelp

def getHelpIndustrialAction3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_INDUSTRIAL_ACTION_3_HELP", ())

	return szHelp


#####  DARWINS_VOYAGE  #####


def canTriggerDarwinsVoyage(argsList):

	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	map = gc.getMap()

	iDestroyer = gc.getInfoTypeForString("UNITCLASS_DESTROYER")
	iBattleship = gc.getInfoTypeForString("UNITCLASS_BATTLESHIP")
	iCarrier = gc.getInfoTypeForString("UNITCLASS_CARRIER")
	iFrigate = gc.getInfoTypeForString("UNITCLASS_FRIGATE")
	iSoL = gc.getInfoTypeForString("UNITCLASS_SHIP_OF_THE_LINE")
	iIronclad = gc.getInfoTypeForString("UNITCLASS_IRONCLAD")
	iStealth = gc.getInfoTypeForString("UNITCLASS_STEALTH_DESTROYER")
	iMissile = gc.getInfoTypeForString("UNITCLASS_MISSILE_CRUISER")
	iBoomer = gc.getInfoTypeForString("UNITCLASS_SUBMARINE")
	iSeawolf = gc.getInfoTypeForString("UNITCLASS_ATTACK_SUBMARINE")
	iPrivateer = gc.getInfoTypeForString("UNITCLASS_PRIVATEER")
	iTransport = gc.getInfoTypeForString("UNITCLASS_TRANSPORT")
	iGalleon = gc.getInfoTypeForString("UNITCLASS_GALLEON")
	iCaravel = gc.getInfoTypeForString("UNITCLASS_CARAVEL")

	pPlayerDD = pPlayer.getUnitClassCount(iDestroyer)
	pPlayerBB = pPlayer.getUnitClassCount(iBattleship)
	pPlayerCV = pPlayer.getUnitClassCount(iCarrier)
	pPlayerFF = pPlayer.getUnitClassCount(iFrigate)
	pPlayerSoL = pPlayer.getUnitClassCount(iSoL)
	pPlayerIC = pPlayer.getUnitClassCount(iIronclad)
	pPlayerSDD = pPlayer.getUnitClassCount(iStealth)
	pPlayerMC = pPlayer.getUnitClassCount(iMissile)
	pPlayerSSN = pPlayer.getUnitClassCount(iSeawolf)
	pPlayerSSBN = pPlayer.getUnitClassCount(iBoomer)
	pPlayerPT = pPlayer.getUnitClassCount(iPrivateer)
	pPlayerTT = pPlayer.getUnitClassCount(iTransport)
	pPlayerGL = pPlayer.getUnitClassCount(iGalleon)
	pPlayerCY = pPlayer.getUnitClassCount(iCaravel)
	pAggregate = pPlayerPT + pPlayerDD + pPlayerBB + pPlayerCV + pPlayerFF + pPlayerSoL + pPlayerIC + pPlayerSDD + pPlayerMC + pPlayerSSN + pPlayerSSBN
	pTransports = pPlayerTT + pPlayerGL + pPlayerCY



	if not pPlayer.isCivic(gc.getInfoTypeForString("CIVIC_SECULAR")):
		return False
	if pTransports < 1:
		return False
	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		if pAggregate < 4:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		if pAggregate < 5:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		if pAggregate < 6:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		if pAggregate < 7:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		if pAggregate < 8:
			return False
	else:
		if pAggregate < 9:
			return False

	return True



######## THE_BUCCANEERS ###########

def canTriggerTheBuccaneers(argsList):

	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)

#   If Barbarians are disabled in this game, this event will not occur.
	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_BARBARIANS):
		return False

#   At least one civ on the board must know Gunpowder.
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_GUNPOWDER")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

#   At least one civ on the board must know Astronomy.
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_ASTRONOMY")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

#	Find an eligible plot
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			return True

	return False


def getHelpTheBuccaneers1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = ("TXT_KEY_EVENT_THE_BUCCANEERS_HELP_1", ())

	return szHelp


def applyTheBuccaneers1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)

	listPlots = []
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			listPlots.append(i)

	if 0 == len(listPlots):
		return

	plot = map.plotByIndex(listPlots[gc.getGame().getSorenRandNum(len(listPlots), "Buccaneers event location")])

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		iNumUnits  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		iNumUnits  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		iNumUnits  = 3
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		iNumUnits  = 4
	elif map.getWorldSize() == gc.getInfoTypeForString("ORLDSIZE_LARGE"):
		iNumUnits  = 5
	else:
		iNumUnits  = 6

	iUnitType = gc.getInfoTypeForString("UNIT_FRENCH_MUSKETEER")

	barbPlayer = gc.getPlayer(gc.getBARBARIAN_PLAYER())
	for i in range(iNumUnits):
		barbPlayer.initUnit(iUnitType, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK_CITY_LEMMING, DirectionTypes.DIRECTION_SOUTH)


######## BLACKBEARD ###########

def canTriggerBlackbeard(argsList):

	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	map = gc.getMap()

#   If Barbarians are disabled in this game, this event will not occur.
	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_BARBARIANS):
		return False

###     kTriggeredData.ePlayer must have less than a variable number of combat ships based on map size.
###     Triremes, Galleys, Caravels/Carracks, Galleons/East Indiamen, Transports don't count.
###     I've included the modern ships just to prevent anomalous triggering.

	iPrivateer = gc.getInfoTypeForString("UNITCLASS_PRIVATEER")
	iDestroyer = gc.getInfoTypeForString("UNITCLASS_DESTROYER")
	iWDestroyer = gc.getInfoTypeForString("UNITCLASS_WW1_DESTROYER")
	iIBattleship = gc.getInfoTypeForString("UNITCLASS_PRE_DREADNOUGHT")
	iDreadnought = gc.getInfoTypeForString("UNITCLASS_DREADNOUGHT_SHIP")
	iBattlecruiser = gc.getInfoTypeForString("UNITCLASS_BATTLECRUISER")
	iBattleship = gc.getInfoTypeForString("UNITCLASS_BATTLESHIP")
	iWWBattleship = gc.getInfoTypeForString("UNITCLASS_WW2_BATTLESHIP")
	iCarrier = gc.getInfoTypeForString("UNITCLASS_CARRIER")
	iCruiser = gc.getInfoTypeForString("UNITCLASS_CRUISER")
	iFrigate = gc.getInfoTypeForString("UNITCLASS_FRIGATE")
	iMoW = gc.getInfoTypeForString("UNITCLASS_MANOWAR")
	iSoL = gc.getInfoTypeForString("UNITCLASS_SHIP_OF_THE_LINE")
	iIronclad = gc.getInfoTypeForString("UNITCLASS_IRONCLAD")
	iIronFrig = gc.getInfoTypeForString("UNITCLASS_IRON_FRIGATE")
	iStealth = gc.getInfoTypeForString("UNITCLASS_STEALTH_DESTROYER")
	iMissile = gc.getInfoTypeForString("UNITCLASS_MISSILE_CRUISER")
	iBoomer = gc.getInfoTypeForString("UNITCLASS_SUBMARINE")
	iSeawolf = gc.getInfoTypeForString("UNITCLASS_ATTACK_SUBMARINE")

	pPlayerPT = pPlayer.getUnitClassCount(iPrivateer)
	pPlayerDD = pPlayer.getUnitClassCount(iDestroyer)
	pPlayerWD = pPlayer.getUnitClassCount(iWDestroyer)
	pPlayerIB = pPlayer.getUnitClassCount(iIBattleship)
	pPlayerDN = pPlayer.getUnitClassCount(iDreadnought)
	pPlayerBC = pPlayer.getUnitClassCount(iBattlecruiser)
	pPlayerBB = pPlayer.getUnitClassCount(iBattleship)
	pPlayerWB = pPlayer.getUnitClassCount(iWWBattleship)
	pPlayerCV = pPlayer.getUnitClassCount(iCarrier)
	pPlayerCL = pPlayer.getUnitClassCount(iCruiser)
	pPlayerFF = pPlayer.getUnitClassCount(iFrigate)
	pPlayerMoW = pPlayer.getUnitClassCount(iMoW)
	pPlayerSoL = pPlayer.getUnitClassCount(iSoL)
	pPlayerIC = pPlayer.getUnitClassCount(iIronclad)
	pPlayerIFr = pPlayer.getUnitClassCount(iIronFrig)
	pPlayerSDD = pPlayer.getUnitClassCount(iStealth)
	pPlayerMC = pPlayer.getUnitClassCount(iMissile)
	pPlayerSSN = pPlayer.getUnitClassCount(iSeawolf)
	pPlayerSSBN = pPlayer.getUnitClassCount(iBoomer)
	pAggregate = pPlayerPT + pPlayerDD + pPlayerWD + pPlayerIB + pPlayerBC + pPlayerBB + pPlayerWB + pPlayerDN + pPlayerCV + pPlayerCL + pPlayerFF + pPlayerMoW + pPlayerSoL + pPlayerIC + pPlayerIFr + pPlayerSDD + pPlayerMC + pPlayerSSN + pPlayerSSBN

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		if pAggregate > 4:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		if pAggregate > 5:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		if pAggregate > 6:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		if pAggregate > 7:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		if pAggregate > 8:
			return False
	else:
		if pAggregate > 9:
			return False


#   At least one civ on the board must know Chemistry. V:Changed to Naval Cannon V:Changed to Naval Tactics
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_NAVAL_TACTICS")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

#   At least one civ on the board must know Astronomy. V:Changed to Navigation
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_NAVIGATION")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

#	Find an eligible plot
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and plot.isWater() and not plot.isImpassable() and not plot.getNumUnits() > 0 and not plot.isLake() and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			return True

	return False


def getHelpBlackbeard1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_BLACKBEARD_HELP_1", ())

	return szHelp

def applyBlackbeard1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)

	listPlots = []
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and plot.isWater() and not plot.isImpassable() and not plot.getNumUnits() > 0 and not plot.isLake() and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			listPlots.append(i)

	if 0 == len(listPlots):
		return

	plot = map.plotByIndex(listPlots[gc.getGame().getSorenRandNum(len(listPlots), "Blackbeard event location")])

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		iNumUnit1  = 1
		iNumUnit2  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		iNumUnit1  = 2
		iNumUnit2  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		iNumUnit1  = 3
		iNumUnit2  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		iNumUnit1  = 4
		iNumUnit2  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		iNumUnit1  = 5
		iNumUnit2  = 2
	else:
		iNumUnit1  = 6
		iNumUnit2  = 2

	iUnitType1 = gc.getInfoTypeForString("UNIT_PRIVATEER")
	iUnitType2 = gc.getInfoTypeForString("UNIT_FRIGATE")

	barbPlayer = gc.getPlayer(gc.getBARBARIAN_PLAYER())
	for i in range(iNumUnit1):
		barbPlayer.initUnit(iUnitType1, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK_SEA, DirectionTypes.DIRECTION_SOUTH)
	for i in range(iNumUnit2):
		barbPlayer.initUnit(iUnitType2, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK_SEA, DirectionTypes.DIRECTION_SOUTH)



######## PIRATES_OF_THE_NEUTRAL_ZONES ###########

def canTriggerPiratesoftheNeutralZones(argsList):

	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	map = gc.getMap()

#   If Barbarians are disabled in this game, this event will not occur.
	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_BARBARIANS):
		return False

###     kTriggeredData.ePlayer must have less than a variable number of combat ships based on map size
###     Galleys & Triremes & Galleons/East Indiamen & Caravels/Carracks & Transports & privateers
###     & frigates & ships of the line & Ironclads don't count.


	iDestroyer = gc.getInfoTypeForString("UNITCLASS_DESTROYER")
	iWDestroyer = gc.getInfoTypeForString("UNITCLASS_WW1_DESTROYER")
	iBattleship = gc.getInfoTypeForString("UNITCLASS_BATTLESHIP")
	iWWBattleship = gc.getInfoTypeForString("UNITCLASS_WW2_BATTLESHIP")
	iCarrier = gc.getInfoTypeForString("UNITCLASS_CARRIER")
	iSCarrier = gc.getInfoTypeForString("UNITCLASS_NUCLEAR_CARRIER")
	iCruiser = gc.getInfoTypeForString("UNITCLASS_CRUISER")
	iStealth = gc.getInfoTypeForString("UNITCLASS_STEALTH_DESTROYER")
	iMissile = gc.getInfoTypeForString("UNITCLASS_MISSILE_CRUISER")
	iBoomer = gc.getInfoTypeForString("UNITCLASS_SUBMARINE")
	iSeawolf = gc.getInfoTypeForString("UNITCLASS_ATTACK_SUBMARINE")
	iSteaSub = gc.getInfoTypeForString("UNITCLASS_STEALTH_SUBMARINE")
	iFDestroyer = gc.getInfoTypeForString("UNITCLASS_FF_DESTROYER_I")
	iFCruiser = gc.getInfoTypeForString("UNITCLASS_FUSION_CRUISER")
	iFBattleship = gc.getInfoTypeForString("UNITCLASS_FUSION_BATTLESHIP")
	iFCarrier = gc.getInfoTypeForString("UNITCLASS_FUSION_CARRIER")
	iFSubmarine = gc.getInfoTypeForString("UNITCLASS_FUSION_SUBMARINE")

	pPlayerDD = pPlayer.getUnitClassCount(iDestroyer)
	pPlayerWD = pPlayer.getUnitClassCount(iWDestroyer)
	pPlayerBB = pPlayer.getUnitClassCount(iBattleship)
	pPlayerWB = pPlayer.getUnitClassCount(iWWBattleship)
	pPlayerCV = pPlayer.getUnitClassCount(iCarrier)
	pPlayerCVN = pPlayer.getUnitClassCount(iSCarrier)
	pPlayerCL = pPlayer.getUnitClassCount(iCruiser)
	pPlayerSDD = pPlayer.getUnitClassCount(iStealth)
	pPlayerMC = pPlayer.getUnitClassCount(iMissile)
	pPlayerSSN = pPlayer.getUnitClassCount(iSeawolf)
	pPlayerSSBN = pPlayer.getUnitClassCount(iBoomer)
	pPlayerSSS = pPlayer.getUnitClassCount(iSteaSub)
	pPlayerFDD = pPlayer.getUnitClassCount(iFDestroyer)
	pPlayerFCA = pPlayer.getUnitClassCount(iFCruiser)
	pPlayerFBB = pPlayer.getUnitClassCount(iFBattleship)
	pPlayerFCV = pPlayer.getUnitClassCount(iFCarrier)
	pPlayerFSS = pPlayer.getUnitClassCount(iFSubmarine)
	pAggregate = pPlayerDD + pPlayerWD + pPlayerBB + pPlayerWB + pPlayerCV + pPlayerCVN + pPlayerCL + pPlayerSDD + pPlayerMC + pPlayerSSN + pPlayerSSBN + pPlayerSSS + pPlayerFDD + pPlayerFCA + pPlayerFBB + pPlayerFCV + pPlayerFSS

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		if pAggregate > 5:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		if pAggregate > 6:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		if pAggregate > 7:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		if pAggregate > 8:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		if pAggregate > 9:
			return False
	else:
		if pAggregate > 10:
			return False

#   At least one civ on the board must know Stealth
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_STEALTH")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False


#   At least one civ on the board must know Robotics
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_ROBOTICS")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

#	Find an eligible plot
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and plot.isWater() and not plot.isImpassable() and not plot.getNumUnits() > 0 and not plot.isLake() and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			return True

	return False


def getHelpPiratesoftheNeutralZones1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_PIRATES_OF_THE_NEUTRAL_ZONES_HELP_1", ())

	return szHelp

def applyPiratesoftheNeutralZones1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)

	listPlots = []
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and plot.isWater() and not plot.isImpassable() and not plot.getNumUnits() > 0 and not plot.isLake() and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			listPlots.append(i)

	if 0 == len(listPlots):
		return

	plot = map.plotByIndex(listPlots[gc.getGame().getSorenRandNum(len(listPlots), "PiratesoftheNeutralZones event location")])

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		iNumUnit1  = 4
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		iNumUnit1  = 5
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		iNumUnit1  = 6
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		iNumUnit1  = 7
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		iNumUnit1  = 8
	else:
		iNumUnit1  = 9

	iUnitType1 = gc.getInfoTypeForString("UNIT_STILETTO_BOAT")
	barbPlayer = gc.getPlayer(gc.getBARBARIAN_PLAYER())
	iNav1 = gc.getInfoTypeForString("PROMOTION_NAVIGATION1")
	unit = i in range(iNumUnit1)
	for i in range(iNumUnit1):
		barbPlayer.initUnit(iUnitType1, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK_SEA, DirectionTypes.DIRECTION_SOUTH)

	(loopUnit, iter) = barbPlayer.firstUnit(False)
	while (loopUnit):
		if loopUnit.getUnitType() == iUnitType1:
			loopUnit.setHasPromotion(iNav1, True)
			loopUnit.setName("Pirate Corvette")
		(loopUnit, iter) = barbPlayer.nextUnit(iter, False)


######## MALACCAN_PIRATES ###########

def canTriggerMalaccanPirates(argsList):

	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	map = gc.getMap()

#   If Barbarians are disabled in this game, this event will not occur.
	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_BARBARIANS):
		return False

###     kTriggeredData.ePlayer must have less than a variable number of combat ships based on map size
###     Galleys & Triremes & Galleons/East Indiamen & Caravels/Carracks & Transports & privateers & frigates & ships of the line don't count.
###     I've included the modern ships just to prevent anomalous triggering.

	iDestroyer = gc.getInfoTypeForString("UNITCLASS_DESTROYER")
	iWDestroyer = gc.getInfoTypeForString("UNITCLASS_WW1_DESTROYER")
	iIBattleship = gc.getInfoTypeForString("UNITCLASS_PRE_DREADNOUGHT")
	iDreadnought = gc.getInfoTypeForString("UNITCLASS_DREADNOUGHT_SHIP")
	iBattlecruiser = gc.getInfoTypeForString("UNITCLASS_BATTLECRUISER")
	iBattleship = gc.getInfoTypeForString("UNITCLASS_BATTLESHIP")
	iWWBattleship = gc.getInfoTypeForString("UNITCLASS_WW2_BATTLESHIP")
	iCarrier = gc.getInfoTypeForString("UNITCLASS_CARRIER")
	iCruiser = gc.getInfoTypeForString("UNITCLASS_CRUISER")
	iFrigate = gc.getInfoTypeForString("UNITCLASS_FRIGATE")
	iStealth = gc.getInfoTypeForString("UNITCLASS_STEALTH_DESTROYER")
	iMissile = gc.getInfoTypeForString("UNITCLASS_MISSILE_CRUISER")
	iBoomer = gc.getInfoTypeForString("UNITCLASS_SUBMARINE")
	iSeawolf = gc.getInfoTypeForString("UNITCLASS_ATTACK_SUBMARINE")

	pPlayerDD = pPlayer.getUnitClassCount(iDestroyer)
	pPlayerWD = pPlayer.getUnitClassCount(iWDestroyer)
	pPlayerIB = pPlayer.getUnitClassCount(iIBattleship)
	pPlayerDN = pPlayer.getUnitClassCount(iDreadnought)
	pPlayerBC = pPlayer.getUnitClassCount(iBattlecruiser)
	pPlayerBB = pPlayer.getUnitClassCount(iBattleship)
	pPlayerWB = pPlayer.getUnitClassCount(iWWBattleship)
	pPlayerCV = pPlayer.getUnitClassCount(iCarrier)
	pPlayerCL = pPlayer.getUnitClassCount(iCruiser)
	pPlayerFF = pPlayer.getUnitClassCount(iFrigate)
	pPlayerSDD = pPlayer.getUnitClassCount(iStealth)
	pPlayerMC = pPlayer.getUnitClassCount(iMissile)
	pPlayerSSN = pPlayer.getUnitClassCount(iSeawolf)
	pPlayerSSBN = pPlayer.getUnitClassCount(iBoomer)
	pAggregate = pPlayerDD + pPlayerWD + pPlayerIB + pPlayerBC + pPlayerBB + pPlayerWB + pPlayerDN + pPlayerCV + pPlayerCL + pPlayerFF + pPlayerSDD + pPlayerMC + pPlayerSSN + pPlayerSSBN

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		if pAggregate > 2:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		if pAggregate > 3:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		if pAggregate > 4:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		if pAggregate > 5:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		if pAggregate > 6:
			return False
	else:
		if pAggregate > 7:
			return False


#   At least one civ on the board must know Automatic weapons
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_AUTOMATIC_WEAPONS")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False


#   At least one civ on the board must know Radio
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_RADIO")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

#	Find an eligible plot
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and plot.isWater() and not plot.isImpassable() and not plot.getNumUnits() > 0 and not plot.isLake() and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			return True

	return False

def getHelpMalaccanPirates1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_MALACCAN_PIRATES_HELP_1", ())

	return szHelp

def applyMalaccanPirates1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)

	listPlots = []
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and plot.isWater() and not plot.isImpassable() and not plot.getNumUnits() > 0 and not plot.isLake() and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			listPlots.append(i)

	if 0 == len(listPlots):
		return

	plot = map.plotByIndex(listPlots[gc.getGame().getSorenRandNum(len(listPlots), "MalaccanPirates event location")])

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		iNumUnit1  = 5
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		iNumUnit1  = 6
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		iNumUnit1  = 7
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		iNumUnit1  = 8
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		iNumUnit1  = 9
	else:
		iNumUnit1  = 10

	iUnitType1 = gc.getInfoTypeForString("UNIT_WW1_DESTROYER")

	barbPlayer = gc.getPlayer(gc.getBARBARIAN_PLAYER())
	iNav1 = gc.getInfoTypeForString("PROMOTION_NAVIGATION1")
	iCbt4 = gc.getInfoTypeForString("PROMOTION_COMBAT4")
	iCoAs1 = gc.getInfoTypeForString("PROMOTION_COASTAL_ASSAULT1")
	unit = i in range(iNumUnit1)
	for i in range(iNumUnit1):
		barbPlayer.initUnit(iUnitType1, plot.getX(), plot.getY(), UnitAITypes.UNITAI_PIRATE_SEA, DirectionTypes.DIRECTION_SOUTH)

	(loopUnit, iter) = barbPlayer.firstUnit(False)
	while (loopUnit):
		if loopUnit.getUnitType() == iUnitType1:
			loopUnit.setHasPromotion(iNav1, True)
			loopUnit.setHasPromotion(iCbt4, True)
			loopUnit.setHasPromotion(iCoAs1, True)
			loopUnit.setName("Malaccan Gunboat")
		(loopUnit, iter) = barbPlayer.nextUnit(iter, False)


######## HENRY_MORGAN ###########

def canTriggerHenryMorgan(argsList):

	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	map = gc.getMap()

#   If Barbarians are disabled in this game, this event will not occur.
	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_BARBARIANS):
		return False

###     kTriggeredData.ePlayer must have less than a variable number of combat ships based on map size
###     Galleys & Triremes & Galleons/East Indiamen & Caravels/Carracks & Transports don't count.
###     I've included the modern ships just to prevent anomalous triggering.

	iPrivateer = gc.getInfoTypeForString("UNITCLASS_PRIVATEER")
	iDestroyer = gc.getInfoTypeForString("UNITCLASS_DESTROYER")
	iWDestroyer = gc.getInfoTypeForString("UNITCLASS_WW1_DESTROYER")
	iIBattleship = gc.getInfoTypeForString("UNITCLASS_PRE_DREADNOUGHT")
	iDreadnought = gc.getInfoTypeForString("UNITCLASS_DREADNOUGHT_SHIP")
	iBattlecruiser = gc.getInfoTypeForString("UNITCLASS_BATTLECRUISER")
	iBattleship = gc.getInfoTypeForString("UNITCLASS_BATTLESHIP")
	iWWBattleship = gc.getInfoTypeForString("UNITCLASS_WW2_BATTLESHIP")
	iCarrier = gc.getInfoTypeForString("UNITCLASS_CARRIER")
	iCruiser = gc.getInfoTypeForString("UNITCLASS_CRUISER")
	iFrigate = gc.getInfoTypeForString("UNITCLASS_FRIGATE")
	iMoW = gc.getInfoTypeForString("UNITCLASS_MANOWAR")
	iSoL = gc.getInfoTypeForString("UNITCLASS_SHIP_OF_THE_LINE")
	iIronclad = gc.getInfoTypeForString("UNITCLASS_IRONCLAD")
	iIronFrig = gc.getInfoTypeForString("UNITCLASS_IRON_FRIGATE")
	iStealth = gc.getInfoTypeForString("UNITCLASS_STEALTH_DESTROYER")
	iMissile = gc.getInfoTypeForString("UNITCLASS_MISSILE_CRUISER")
	iBoomer = gc.getInfoTypeForString("UNITCLASS_SUBMARINE")
	iSeawolf = gc.getInfoTypeForString("UNITCLASS_ATTACK_SUBMARINE")

	pPlayerPT = pPlayer.getUnitClassCount(iPrivateer)
	pPlayerDD = pPlayer.getUnitClassCount(iDestroyer)
	pPlayerWD = pPlayer.getUnitClassCount(iWDestroyer)
	pPlayerIB = pPlayer.getUnitClassCount(iIBattleship)
	pPlayerDN = pPlayer.getUnitClassCount(iDreadnought)
	pPlayerBC = pPlayer.getUnitClassCount(iBattlecruiser)
	pPlayerBB = pPlayer.getUnitClassCount(iBattleship)
	pPlayerWB = pPlayer.getUnitClassCount(iWWBattleship)
	pPlayerCV = pPlayer.getUnitClassCount(iCarrier)
	pPlayerCL = pPlayer.getUnitClassCount(iCruiser)
	pPlayerFF = pPlayer.getUnitClassCount(iFrigate)
	pPlayerMoW = pPlayer.getUnitClassCount(iMoW)
	pPlayerSoL = pPlayer.getUnitClassCount(iSoL)
	pPlayerIC = pPlayer.getUnitClassCount(iIronclad)
	pPlayerIFr = pPlayer.getUnitClassCount(iIronFrig)
	pPlayerSDD = pPlayer.getUnitClassCount(iStealth)
	pPlayerMC = pPlayer.getUnitClassCount(iMissile)
	pPlayerSSN = pPlayer.getUnitClassCount(iSeawolf)
	pPlayerSSBN = pPlayer.getUnitClassCount(iBoomer)
	pAggregate = pPlayerPT + pPlayerDD + pPlayerWD + pPlayerIB + pPlayerBC + pPlayerBB + pPlayerWB + pPlayerDN + pPlayerCV + pPlayerCL + pPlayerFF + pPlayerMoW + pPlayerSoL + pPlayerIC + pPlayerIFr + pPlayerSDD + pPlayerMC + pPlayerSSN + pPlayerSSBN

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		if pAggregate > 3:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		if pAggregate > 4:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		if pAggregate > 5:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		if pAggregate > 6:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		if pAggregate > 7:
			return False
	else:
		if pAggregate > 8:
			return False

#   At least one civ on the board must know Astronomy V: Removed
#	bFoundValid = False
#	iTech = gc.getInfoTypeForString("TECH_ASTRONOMY")
#	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
#		loopPlayer = gc.getPlayer(iPlayer)
#		if loopPlayer.isAlive():
#			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
#				bFoundValid = True
#				break

#	if not bFoundValid:
#		return False


#   At least one civ on the board must know Military Science V: Grand War
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_GRAND_WAR")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

#	Find an eligible plot
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and plot.isWater() and not plot.isImpassable() and not plot.getNumUnits() > 0 and not plot.isLake() and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			return True

	return False

def getHelpHenryMorgan1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_HENRY_MORGAN_HELP_1", ())

	return szHelp

def applyHenryMorgan1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)

	listPlots = []
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and plot.isWater() and not plot.isImpassable() and not plot.getNumUnits() > 0 and not plot.isLake() and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			listPlots.append(i)

	if 0 == len(listPlots):
		return

	plot = map.plotByIndex(listPlots[gc.getGame().getSorenRandNum(len(listPlots), "HenryMorgan event location")])

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		iNumUnit1  = 1
		iNumUnit2  = 1
		iNumUnit3  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		iNumUnit1  = 1
		iNumUnit2  = 1
		iNumUnit3  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		iNumUnit1  = 1
		iNumUnit2  = 2
		iNumUnit3  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		iNumUnit1  = 1
		iNumUnit2  = 2
		iNumUnit3  = 3
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		iNumUnit1  = 1
		iNumUnit2  = 2
		iNumUnit3  = 4
	else:
		iNumUnit1  = 1
		iNumUnit2  = 3
		iNumUnit3  = 4

	iUnitType1 = gc.getInfoTypeForString("UNIT_SHIP_OF_THE_LINE")
	iUnitType2 = gc.getInfoTypeForString("UNIT_FRIGATE")
	iUnitType3 = gc.getInfoTypeForString("UNIT_PRIVATEER")


	barbPlayer = gc.getPlayer(gc.getBARBARIAN_PLAYER())
	iCbt4 = gc.getInfoTypeForString("PROMOTION_COMBAT4")
	for i in range(iNumUnit1):
		barbPlayer.initUnit(iUnitType1, plot.getX(), plot.getY(), UnitAITypes.UNITAI_PIRATE_SEA, DirectionTypes.DIRECTION_SOUTH)
	for i in range(iNumUnit2):
		barbPlayer.initUnit(iUnitType2, plot.getX(), plot.getY(), UnitAITypes.UNITAI_PIRATE_SEA, DirectionTypes.DIRECTION_SOUTH)
	for i in range(iNumUnit3):
		barbPlayer.initUnit(iUnitType3, plot.getX(), plot.getY(), UnitAITypes.UNITAI_PIRATE_SEA, DirectionTypes.DIRECTION_SOUTH)

	(loopUnit, iter) = barbPlayer.firstUnit(False)
	while (loopUnit):
		if loopUnit.getUnitType() == iUnitType1:
			loopUnit.setHasPromotion(iCbt4, True)
		(loopUnit, iter) = barbPlayer.nextUnit(iter, False)

######## STEDE_BONNET ###########

def canTriggerStedeBonnet(argsList):

	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	map = gc.getMap()

#   If Barbarians are disabled in this game, this event will not occur.
	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_BARBARIANS):
		return False

###     kTriggeredData.ePlayer must have less than a variable number of combat ships based on map size
###     Galleys & Triremes don't count.
###     I've included the modern ships just to prevent anomalous triggering.


	iCaravel = gc.getInfoTypeForString("UNITCLASS_CARAVEL")
	iFluyt = gc.getInfoTypeForString("UNITCLASS_FLUYT")
	iGalleon = gc.getInfoTypeForString("UNITCLASS_GALLEON")
	iSteamer = gc.getInfoTypeForString("UNITCLASS_STEAMER")
	iTransport = gc.getInfoTypeForString("UNITCLASS_TRANSPORT")
	iPrivateer = gc.getInfoTypeForString("UNITCLASS_PRIVATEER")
	iDestroyer = gc.getInfoTypeForString("UNITCLASS_DESTROYER")
	iWDestroyer = gc.getInfoTypeForString("UNITCLASS_WW1_DESTROYER")
	iIBattleship = gc.getInfoTypeForString("UNITCLASS_PRE_DREADNOUGHT")
	iDreadnought = gc.getInfoTypeForString("UNITCLASS_DREADNOUGHT_SHIP")
	iBattlecruiser = gc.getInfoTypeForString("UNITCLASS_BATTLECRUISER")
	iBattleship = gc.getInfoTypeForString("UNITCLASS_BATTLESHIP")
	iWWBattleship = gc.getInfoTypeForString("UNITCLASS_WW2_BATTLESHIP")
	iCarrier = gc.getInfoTypeForString("UNITCLASS_CARRIER")
	iCruiser = gc.getInfoTypeForString("UNITCLASS_CRUISER")
	iFrigate = gc.getInfoTypeForString("UNITCLASS_FRIGATE")
	iMoW = gc.getInfoTypeForString("UNITCLASS_MANOWAR")
	iSoL = gc.getInfoTypeForString("UNITCLASS_SHIP_OF_THE_LINE")
	iIronclad = gc.getInfoTypeForString("UNITCLASS_IRONCLAD")
	iIronFrig = gc.getInfoTypeForString("UNITCLASS_IRON_FRIGATE")
	iStealth = gc.getInfoTypeForString("UNITCLASS_STEALTH_DESTROYER")
	iMissile = gc.getInfoTypeForString("UNITCLASS_MISSILE_CRUISER")
	iBoomer = gc.getInfoTypeForString("UNITCLASS_SUBMARINE")
	iSeawolf = gc.getInfoTypeForString("UNITCLASS_ATTACK_SUBMARINE")

	pPlayerCR = pPlayer.getUnitClassCount(iCaravel)
	pPlayerFY = pPlayer.getUnitClassCount(iFluyt)
	pPlayerGN = pPlayer.getUnitClassCount(iGalleon)
	pPlayerST = pPlayer.getUnitClassCount(iSteamer)
	pPlayerTP = pPlayer.getUnitClassCount(iTransport)
	pPlayerPT = pPlayer.getUnitClassCount(iPrivateer)
	pPlayerDD = pPlayer.getUnitClassCount(iDestroyer)
	pPlayerWD = pPlayer.getUnitClassCount(iWDestroyer)
	pPlayerIB = pPlayer.getUnitClassCount(iIBattleship)
	pPlayerDN = pPlayer.getUnitClassCount(iDreadnought)
	pPlayerBC = pPlayer.getUnitClassCount(iBattlecruiser)
	pPlayerBB = pPlayer.getUnitClassCount(iBattleship)
	pPlayerWB = pPlayer.getUnitClassCount(iWWBattleship)
	pPlayerCV = pPlayer.getUnitClassCount(iCarrier)
	pPlayerCL = pPlayer.getUnitClassCount(iCruiser)
	pPlayerFF = pPlayer.getUnitClassCount(iFrigate)
	pPlayerMoW = pPlayer.getUnitClassCount(iMoW)
	pPlayerSoL = pPlayer.getUnitClassCount(iSoL)
	pPlayerIC = pPlayer.getUnitClassCount(iIronclad)
	pPlayerIFr = pPlayer.getUnitClassCount(iIronFrig)
	pPlayerSDD = pPlayer.getUnitClassCount(iStealth)
	pPlayerMC = pPlayer.getUnitClassCount(iMissile)
	pPlayerSSN = pPlayer.getUnitClassCount(iSeawolf)
	pPlayerSSBN = pPlayer.getUnitClassCount(iBoomer)
	pAggregate = pPlayerCR + pPlayerFY + pPlayerGN + pPlayerST + pPlayerTP + pPlayerPT + pPlayerDD + pPlayerWD + pPlayerIB + pPlayerBC + pPlayerBB + pPlayerWB + pPlayerDN + pPlayerCV + pPlayerCL + pPlayerFF + pPlayerMoW + pPlayerSoL + pPlayerIC + pPlayerIFr + pPlayerSDD + pPlayerMC + pPlayerSSN + pPlayerSSBN

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		if pAggregate > 2:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		if pAggregate > 3:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		if pAggregate > 4:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		if pAggregate > 5:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		if pAggregate > 6:
			return False
	else:
		if pAggregate > 7:
			return False

#   At least one civ on the board must know Astronomy
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_ASTRONOMY")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False


#   At least one civ on the board must know Gunpowder
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_GUNPOWDER")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

#	Find an eligible plot
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and plot.isWater() and not plot.isImpassable() and not plot.getNumUnits() > 0 and not plot.isLake() and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			return True

	return False

def getHelpStedeBonnet1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_STEDE_BONNET_HELP_1", ())

	return szHelp

def applyStedeBonnet1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)

	listPlots = []
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and plot.isWater() and not plot.isImpassable() and not plot.getNumUnits() > 0 and not plot.isLake() and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			listPlots.append(i)

	if 0 == len(listPlots):
		return

	plot = map.plotByIndex(listPlots[gc.getGame().getSorenRandNum(len(listPlots), "StedeBonnet event location")])

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		iNumUnit1  = 1
		iNumUnit2  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		iNumUnit1  = 2
		iNumUnit2  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		iNumUnit1  = 2
		iNumUnit2  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		iNumUnit1  = 3
		iNumUnit2  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		iNumUnit1  = 3
		iNumUnit2  = 3
	else:
		iNumUnit1  = 4
		iNumUnit2  = 3

	iUnitType1 = gc.getInfoTypeForString("UNIT_PORTUGAL_CARRACK")
	iUnitType2 = gc.getInfoTypeForString("UNIT_NETHERLANDS_OOSTINDIEVAARDER")
	iNav1 = gc.getInfoTypeForString("PROMOTION_NAVIGATION1")

	barbPlayer = gc.getPlayer(gc.getBARBARIAN_PLAYER())
	for i in range(iNumUnit1):
		barbPlayer.initUnit(iUnitType1, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK_SEA, DirectionTypes.DIRECTION_SOUTH)
	for i in range(iNumUnit2):
		barbPlayer.initUnit(iUnitType2, plot.getX(), plot.getY(), UnitAITypes.UNITAI_PIRATE_SEA, DirectionTypes.DIRECTION_SOUTH)

	(loopUnit, iter) = barbPlayer.firstUnit(False)
	while (loopUnit):
		if loopUnit.getUnitType() == iUnitType1:
			loopUnit.setName("Barque")
		if loopUnit.getUnitType() == iUnitType2:
			loopUnit.setName("Fast Galleon")
			loopUnit.setHasPromotion(iNav1, True)
		(loopUnit, iter) = barbPlayer.nextUnit(iter, False)

######## THE_CORSAIRS ###########

def canTriggerTheCorsairs(argsList):

	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	map = gc.getMap()

#   If Barbarians are disabled in this game, this event will not occur.
	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_BARBARIANS):
		return False

###     kTriggeredData.ePlayer must have less than a variable number of combat ships based on map size
###     Galleys & Triremes don't count.
###     I've included the modern ships just to prevent anomalous triggering.

	iCaravel = gc.getInfoTypeForString("UNITCLASS_CARAVEL")
	iFluyt = gc.getInfoTypeForString("UNITCLASS_FLUYT")
	iGalleon = gc.getInfoTypeForString("UNITCLASS_GALLEON")
	iSteamer = gc.getInfoTypeForString("UNITCLASS_STEAMER")
	iTransport = gc.getInfoTypeForString("UNITCLASS_TRANSPORT")
	iPrivateer = gc.getInfoTypeForString("UNITCLASS_PRIVATEER")
	iDestroyer = gc.getInfoTypeForString("UNITCLASS_DESTROYER")
	iWDestroyer = gc.getInfoTypeForString("UNITCLASS_WW1_DESTROYER")
	iIBattleship = gc.getInfoTypeForString("UNITCLASS_PRE_DREADNOUGHT")
	iDreadnought = gc.getInfoTypeForString("UNITCLASS_DREADNOUGHT_SHIP")
	iBattlecruiser = gc.getInfoTypeForString("UNITCLASS_BATTLECRUISER")
	iBattleship = gc.getInfoTypeForString("UNITCLASS_BATTLESHIP")
	iWWBattleship = gc.getInfoTypeForString("UNITCLASS_WW2_BATTLESHIP")
	iCarrier = gc.getInfoTypeForString("UNITCLASS_CARRIER")
	iCruiser = gc.getInfoTypeForString("UNITCLASS_CRUISER")
	iFrigate = gc.getInfoTypeForString("UNITCLASS_FRIGATE")
	iMoW = gc.getInfoTypeForString("UNITCLASS_MANOWAR")
	iSoL = gc.getInfoTypeForString("UNITCLASS_SHIP_OF_THE_LINE")
	iIronclad = gc.getInfoTypeForString("UNITCLASS_IRONCLAD")
	iIronFrig = gc.getInfoTypeForString("UNITCLASS_IRON_FRIGATE")
	iStealth = gc.getInfoTypeForString("UNITCLASS_STEALTH_DESTROYER")
	iMissile = gc.getInfoTypeForString("UNITCLASS_MISSILE_CRUISER")
	iBoomer = gc.getInfoTypeForString("UNITCLASS_SUBMARINE")
	iSeawolf = gc.getInfoTypeForString("UNITCLASS_ATTACK_SUBMARINE")

	pPlayerCR = pPlayer.getUnitClassCount(iCaravel)
	pPlayerFY = pPlayer.getUnitClassCount(iFluyt)
	pPlayerGN = pPlayer.getUnitClassCount(iGalleon)
	pPlayerST = pPlayer.getUnitClassCount(iSteamer)
	pPlayerTP = pPlayer.getUnitClassCount(iTransport)
	pPlayerPT = pPlayer.getUnitClassCount(iPrivateer)
	pPlayerDD = pPlayer.getUnitClassCount(iDestroyer)
	pPlayerWD = pPlayer.getUnitClassCount(iWDestroyer)
	pPlayerIB = pPlayer.getUnitClassCount(iIBattleship)
	pPlayerDN = pPlayer.getUnitClassCount(iDreadnought)
	pPlayerBC = pPlayer.getUnitClassCount(iBattlecruiser)
	pPlayerBB = pPlayer.getUnitClassCount(iBattleship)
	pPlayerWB = pPlayer.getUnitClassCount(iWWBattleship)
	pPlayerCV = pPlayer.getUnitClassCount(iCarrier)
	pPlayerCL = pPlayer.getUnitClassCount(iCruiser)
	pPlayerFF = pPlayer.getUnitClassCount(iFrigate)
	pPlayerMoW = pPlayer.getUnitClassCount(iMoW)
	pPlayerSoL = pPlayer.getUnitClassCount(iSoL)
	pPlayerIC = pPlayer.getUnitClassCount(iIronclad)
	pPlayerIFr = pPlayer.getUnitClassCount(iIronFrig)
	pPlayerSDD = pPlayer.getUnitClassCount(iStealth)
	pPlayerMC = pPlayer.getUnitClassCount(iMissile)
	pPlayerSSN = pPlayer.getUnitClassCount(iSeawolf)
	pPlayerSSBN = pPlayer.getUnitClassCount(iBoomer)
	pAggregate = pPlayerCR + pPlayerFY + pPlayerGN + pPlayerST + pPlayerTP + pPlayerPT + pPlayerDD + pPlayerWD + pPlayerIB + pPlayerBC + pPlayerBB + pPlayerWB + pPlayerDN + pPlayerCV + pPlayerCL + pPlayerFF + pPlayerMoW + pPlayerSoL + pPlayerIC + pPlayerIFr + pPlayerSDD + pPlayerMC + pPlayerSSN + pPlayerSSBN

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		if pAggregate > 2:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		if pAggregate > 2:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		if pAggregate > 3:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		if pAggregate > 3:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		if pAggregate > 4:
			return False
	else:
		if pAggregate > 4:
			return False


#   At least one civ on the board must know Optics
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_OPTICS")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False


#	Find an eligible plot
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and plot.isWater() and not plot.isImpassable() and not plot.getNumUnits() > 0 and not plot.isLake() and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			return True

	return False

def getHelpTheCorsairs1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_THE_CORSAIRS_HELP_1", ())

	return szHelp

def applyTheCorsairs1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)

	listPlots = []
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and plot.isWater() and not plot.isImpassable() and not plot.getNumUnits() > 0 and not plot.isLake() and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			listPlots.append(i)

	if 0 == len(listPlots):
		return

	plot = map.plotByIndex(listPlots[gc.getGame().getSorenRandNum(len(listPlots), "Corsairs event location")])

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		iNumUnit1  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		iNumUnit1  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		iNumUnit1  = 3
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		iNumUnit1  = 4
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		iNumUnit1  = 5
	else:
		iNumUnit1  = 6

	iUnitType1 = gc.getInfoTypeForString("UNIT_PORTUGAL_CARRACK")


	barbPlayer = gc.getPlayer(gc.getBARBARIAN_PLAYER())
	for i in range(iNumUnit1):
		barbPlayer.initUnit(iUnitType1, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK_SEA, DirectionTypes.DIRECTION_SOUTH)

	(loopUnit, iter) = barbPlayer.firstUnit(False)
	while (loopUnit):
		if loopUnit.getUnitType() == iUnitType1:
			loopUnit.setName("Corsair Raider")
		(loopUnit, iter) = barbPlayer.nextUnit(iter, False)


######## ILLYRIAN_PIRATES ###########

def canTriggerIllyrianPirates(argsList):

	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	map = gc.getMap()

#   If Barbarians are disabled in this game, this event will not occur.
	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_BARBARIANS):
		return False

###     kTriggeredData.ePlayer must have less than a variable number of combat ships based on map size
###     Galleys don't count.
###     I've included the modern ships just to prevent anomalous triggering.

	iWarGalley = gc.getInfoTypeForString("UNITCLASS_WARGALLEY")
	iTrireme = gc.getInfoTypeForString("UNITCLASS_TRIREME")
	iCaravel = gc.getInfoTypeForString("UNITCLASS_CARAVEL")
	iFluyt = gc.getInfoTypeForString("UNITCLASS_FLUYT")
	iGalleon = gc.getInfoTypeForString("UNITCLASS_GALLEON")
	iSteamer = gc.getInfoTypeForString("UNITCLASS_STEAMER")
	iTransport = gc.getInfoTypeForString("UNITCLASS_TRANSPORT")
	iPrivateer = gc.getInfoTypeForString("UNITCLASS_PRIVATEER")
	iDestroyer = gc.getInfoTypeForString("UNITCLASS_DESTROYER")
	iWDestroyer = gc.getInfoTypeForString("UNITCLASS_WW1_DESTROYER")
	iIBattleship = gc.getInfoTypeForString("UNITCLASS_PRE_DREADNOUGHT")
	iDreadnought = gc.getInfoTypeForString("UNITCLASS_DREADNOUGHT_SHIP")
	iBattlecruiser = gc.getInfoTypeForString("UNITCLASS_BATTLECRUISER")
	iBattleship = gc.getInfoTypeForString("UNITCLASS_BATTLESHIP")
	iWWBattleship = gc.getInfoTypeForString("UNITCLASS_WW2_BATTLESHIP")
	iCarrier = gc.getInfoTypeForString("UNITCLASS_CARRIER")
	iCruiser = gc.getInfoTypeForString("UNITCLASS_CRUISER")
	iFrigate = gc.getInfoTypeForString("UNITCLASS_FRIGATE")
	iMoW = gc.getInfoTypeForString("UNITCLASS_MANOWAR")
	iSoL = gc.getInfoTypeForString("UNITCLASS_SHIP_OF_THE_LINE")
	iIronclad = gc.getInfoTypeForString("UNITCLASS_IRONCLAD")
	iIronFrig = gc.getInfoTypeForString("UNITCLASS_IRON_FRIGATE")
	iStealth = gc.getInfoTypeForString("UNITCLASS_STEALTH_DESTROYER")
	iMissile = gc.getInfoTypeForString("UNITCLASS_MISSILE_CRUISER")
	iBoomer = gc.getInfoTypeForString("UNITCLASS_SUBMARINE")
	iSeawolf = gc.getInfoTypeForString("UNITCLASS_ATTACK_SUBMARINE")

	pPlayerWG = pPlayer.getUnitClassCount(iWarGalley)
	pPlayerTE = pPlayer.getUnitClassCount(iTrireme)
	pPlayerCR = pPlayer.getUnitClassCount(iCaravel)
	pPlayerFY = pPlayer.getUnitClassCount(iFluyt)
	pPlayerGN = pPlayer.getUnitClassCount(iGalleon)
	pPlayerST = pPlayer.getUnitClassCount(iSteamer)
	pPlayerTP = pPlayer.getUnitClassCount(iTransport)
	pPlayerPT = pPlayer.getUnitClassCount(iPrivateer)
	pPlayerDD = pPlayer.getUnitClassCount(iDestroyer)
	pPlayerWD = pPlayer.getUnitClassCount(iWDestroyer)
	pPlayerIB = pPlayer.getUnitClassCount(iIBattleship)
	pPlayerDN = pPlayer.getUnitClassCount(iDreadnought)
	pPlayerBC = pPlayer.getUnitClassCount(iBattlecruiser)
	pPlayerBB = pPlayer.getUnitClassCount(iBattleship)
	pPlayerWB = pPlayer.getUnitClassCount(iWWBattleship)
	pPlayerCV = pPlayer.getUnitClassCount(iCarrier)
	pPlayerCL = pPlayer.getUnitClassCount(iCruiser)
	pPlayerFF = pPlayer.getUnitClassCount(iFrigate)
	pPlayerMoW = pPlayer.getUnitClassCount(iMoW)
	pPlayerSoL = pPlayer.getUnitClassCount(iSoL)
	pPlayerIC = pPlayer.getUnitClassCount(iIronclad)
	pPlayerIFr = pPlayer.getUnitClassCount(iIronFrig)
	pPlayerSDD = pPlayer.getUnitClassCount(iStealth)
	pPlayerMC = pPlayer.getUnitClassCount(iMissile)
	pPlayerSSN = pPlayer.getUnitClassCount(iSeawolf)
	pPlayerSSBN = pPlayer.getUnitClassCount(iBoomer)
	pAggregate = pPlayerWG + pPlayerTE + pPlayerCR + pPlayerFY + pPlayerGN + pPlayerST + pPlayerTP + pPlayerPT + pPlayerDD + pPlayerWD + pPlayerIB + pPlayerBC + pPlayerBB + pPlayerWB + pPlayerDN + pPlayerCV + pPlayerCL + pPlayerFF + pPlayerMoW + pPlayerSoL + pPlayerIC + pPlayerIFr + pPlayerSDD + pPlayerMC + pPlayerSSN + pPlayerSSBN

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		if pAggregate > 2:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		if pAggregate > 2:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		if pAggregate > 3:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		if pAggregate > 3:
			return False
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		if pAggregate > 4:
			return False
	else:
		if pAggregate > 4:
			return False

#   At least one civ on the board must know Metal Casting.
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_METAL_CASTING")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

#   At least one civ on the board must know Sailing.
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_SAILING")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

#	Find an eligible plot
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and plot.isWater() and not plot.isImpassable() and not plot.getNumUnits() > 0 and not plot.isLake() and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			return True

	return False

def getHelpIllyrianPirates1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_ILLYRIAN_PIRATES_HELP_1", ())

	return szHelp

def applyIllyrianPirates1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)

	listPlots = []
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and plot.isWater() and not plot.isImpassable() and not plot.getNumUnits() > 0 and not plot.isLake() and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			listPlots.append(i)

	if 0 == len(listPlots):
		return

	plot = map.plotByIndex(listPlots[gc.getGame().getSorenRandNum(len(listPlots), "IllyrianPirates event location")])

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		iNumUnit1  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		iNumUnit1  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		iNumUnit1  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		iNumUnit1  = 3
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		iNumUnit1  = 4
	else:
		iNumUnit1  = 5

	iUnitType1 = gc.getInfoTypeForString("UNIT_WARGALLEY")
	barbPlayer = gc.getPlayer(gc.getBARBARIAN_PLAYER())
	for i in range(iNumUnit1):
		barbPlayer.initUnit(iUnitType1, plot.getX(), plot.getY(), UnitAITypes.UNITAI_PIRATE_SEA, DirectionTypes.DIRECTION_SOUTH)

	(loopUnit, iter) = barbPlayer.firstUnit(False)
	while (loopUnit):
		if loopUnit.getUnitType() == iUnitType1:
			loopUnit.setName("Illyrian Raider")
		(loopUnit, iter) = barbPlayer.nextUnit(iter, False)


######## MAHDI_ARMY ###########

def canTriggerMahdiArmy(argsList):

	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)

#   If Barbarians are disabled in this game, this event will not occur.
	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_BARBARIANS):
		return False

#   At least one civ on the board must know Flintlock.
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_FLINTLOCK")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

#   At least one civ on the board must know Liberalism.
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_LIBERALISM")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

#	Find an eligible plot
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			return True

	return False

def getHelpMahdiArmy1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_MAHDI_ARMY_HELP_1", ())

	return szHelp


def applyMahdiArmy1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	listPlots = []
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			listPlots.append(i)

	if 0 == len(listPlots):
		return

	plot = map.plotByIndex(listPlots[gc.getGame().getSorenRandNum(len(listPlots), "Mahdi Army event location")])

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		iNumUnits  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		iNumUnits  = 4
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		iNumUnits  = 6
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		iNumUnits  = 7
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		iNumUnits  = 8
	else:
		iNumUnits  = 9

	iUnitType = gc.getInfoTypeForString("UNIT_MACEMAN")

	barbPlayer = gc.getPlayer(gc.getBARBARIAN_PLAYER())
	for i in range(iNumUnits):
		barbPlayer.initUnit(iUnitType, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK_CITY_LEMMING, DirectionTypes.DIRECTION_SOUTH)


######## THE_TALIBAN ###########

def canTriggerTheTaliban(argsList):

	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)

#   If Barbarians are disabled in this game, this event will not occur.
	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_BARBARIANS):
		return False

#   At least one civ on the board must know Satellites.
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_SATELLITES")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

#   At least one civ on the board must know Robotics.
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_ROBOTICS")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

#	Find an eligible plot
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			return True

	return False

def getHelpTheTaliban1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_THE_TALIBAN_HELP_1", ())

	return szHelp


def applyTheTaliban1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	listPlots = []
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			listPlots.append(i)

	if 0 == len(listPlots):
		return

	plot = map.plotByIndex(listPlots[gc.getGame().getSorenRandNum(len(listPlots), "The Taliban event location")])

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		iNumUnits1 = 2
		iNumUnits2 = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		iNumUnits1 = 3
		iNumUnits2 = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		iNumUnits1 = 4
		iNumUnits2 = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		iNumUnits1 = 5
		iNumUnits2 = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		iNumUnits1 = 6
		iNumUnits2 = 1
	else:
		iNumUnits1 = 7
		iNumUnits2 = 2

	iUnitType1 = gc.getInfoTypeForString("UNIT_MODERN_INFANTRY")
	iUnitType2 = gc.getInfoTypeForString("UNIT_AT_INFANTRY")

	barbPlayer = gc.getPlayer(gc.getBARBARIAN_PLAYER())
	for i in range(iNumUnits1):
		barbPlayer.initUnit(iUnitType1, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK_CITY_LEMMING, DirectionTypes.DIRECTION_SOUTH)
	for i in range(iNumUnits2):
		barbPlayer.initUnit(iUnitType2, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK_CITY_LEMMING, DirectionTypes.DIRECTION_SOUTH)



######## V_GER ############

def canTriggerV_Ger(argsList):

	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)
	iObsrvty = gc.getInfoTypeForString("BUILDINGCLASS_OBSERVATORY")

	if pPlayer.getBuildingClassCountWithUpgrades(iObsrvty) < 1:
		return False

	if gc.getGame().isVictoryValid(gc.getInfoTypeForString("VICTORY_SPACE_RACE")):
		return True

	return False

######## Sailing founded ############

def canTriggerSailingFounded(argsList):
	eTrigger = argsList[0]
	ePlayer = argsList[1]
	iCity = argsList[2]

	player = gc.getPlayer(ePlayer)
	city = player.getCity(iCity)

	if city.isNone():
		return False

	if not city.isCoastal(gc.getMIN_WATER_SIZE_FOR_OCEAN()):
		return False

	if city.plot().getLatitude() <= 0:
		return False

	return True

######## Chariotry founded ############

def canTriggerChariotryFounded(argsList):
	eTrigger = argsList[0]
	ePlayer = argsList[1]
	iCity = argsList[2]

	player = gc.getPlayer(ePlayer)
	city = player.getCity(iCity)

	iHorse = gc.getInfoTypeForString("BONUS_HORSE")

	if city.isNone():
		return False

	if (city.plot().getLatitude() <= 0):
		return False

	if (city.getNumBonuses(iHorse) < 1):
		return False

	return True

######## MERCENARIES ANCIENT ###########

def canTriggerMercenariesAncient(argsList):

	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

#   If Barbarians are disabled in this game, this event will not occur.
	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_BARBARIANS):
		return False

#   At least one civ on the board must know Archery.
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_ARCHERY")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

#   At least one civ on the board must know Mining.
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_MINING")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

	# Can we build the counter unit?
	iCounterUnitClass = gc.getInfoTypeForString("UNITCLASS_ARCHER")
	iCounterUnit = gc.getCivilizationInfo(player.getCivilizationType()).getCivilizationUnits(iCounterUnitClass)
	if iCounterUnit == -1:
		return False

	(loopCity, iter) = player.firstCity(False)
	bFound = False
	while(loopCity):
		if (loopCity.canTrain(iCounterUnit, False, False, False, False)):
			bFound = True
			break

		(loopCity, iter) = player.nextCity(iter, False)

	if not bFound:
		return False

#	Find an eligible plot
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			return True

	return False


def getHelpMercenariesAncient1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_MERCENARIES_ANCIENT_HELP_1", ())

	return szHelp


def applyMercenariesAncient1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	listPlots = []
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			listPlots.append(i)

	if 0 == len(listPlots):
		return

	plot = map.plotByIndex(listPlots[gc.getGame().getSorenRandNum(len(listPlots), "Mercenary event location")])

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		iNumUnits  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		iNumUnits  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		iNumUnits  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		iNumUnits  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		iNumUnits  = 3
	else:
		iNumUnits  = 3

	iUnitType = gc.getInfoTypeForString("UNIT_WARRIOR")

	barbPlayer = gc.getPlayer(gc.getBARBARIAN_PLAYER())
	for i in range(iNumUnits):
		barbPlayer.initUnit(iUnitType, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK_CITY_LEMMING, DirectionTypes.DIRECTION_SOUTH)

	# (loopUnit, iter) = barbPlayer.firstUnit(False)
	# while (loopUnit):
	#	if loopUnit.getUnitType() == iUnitType:
	#		loopUnit.setName("Mercenary Warrior")
	#	(loopUnit, iter) = barbPlayer.nextUnit(iter, False)

def applyMercenariesAncient2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	listPlots = []
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			listPlots.append(i)

	if 0 == len(listPlots):
		return

	plot = map.plotByIndex(listPlots[gc.getGame().getSorenRandNum(len(listPlots), "Mercenary event location")])

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		iNumUnits  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		iNumUnits  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		iNumUnits  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		iNumUnits  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		iNumUnits  = 3
	else:
		iNumUnits  = 3

	iUnitType = gc.getInfoTypeForString("UNIT_WARRIOR")

	for i in range(iNumUnits):
		player.initUnit(iUnitType, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK_CITY, DirectionTypes.DIRECTION_SOUTH)

	# (loopUnit, iter) = player.firstUnit(False)
	# while (loopUnit):
	#	if loopUnit.getUnitType() == iUnitType:
	#		loopUnit.setName("Mercenary Warrior")
	#	(loopUnit, iter) = player.nextUnit(iter, False)

######## MERCENARIES CLASSICAL ###########

def canTriggerMercenariesClassical(argsList):

	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

#   If Barbarians are disabled in this game, this event will not occur.
	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_BARBARIANS):
		return False

#   At least one civ on the board must know Archery.
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_IRON_WORKING")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

#   At least one civ on the board must know Mining.
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_WRITING")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

	# Can we build the counter unit?
	iCounterUnitClass = gc.getInfoTypeForString("UNITCLASS_AXEMAN")
	iCounterUnit = gc.getCivilizationInfo(player.getCivilizationType()).getCivilizationUnits(iCounterUnitClass)
	if iCounterUnit == -1:
		return False

	(loopCity, iter) = player.firstCity(False)
	bFound = False
	while(loopCity):
		if (loopCity.canTrain(iCounterUnit, False, False, False, False)):
			bFound = True
			break

		(loopCity, iter) = player.nextCity(iter, False)

	if not bFound:
		return False

#	Find an eligible plot
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			return True

	return False


def getHelpMercenariesClassical1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_MERCENARIES_CLASSICAL_HELP_1", ())

	return szHelp


def applyMercenariesClassical1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	listPlots = []
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			listPlots.append(i)

	if 0 == len(listPlots):
		return

	plot = map.plotByIndex(listPlots[gc.getGame().getSorenRandNum(len(listPlots), "Mercenary event location")])

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		iNumUnits  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		iNumUnits  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		iNumUnits  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		iNumUnits  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		iNumUnits  = 3
	else:
		iNumUnits  = 3

	iUnitType = gc.getInfoTypeForString("UNIT_AXEMAN")

	barbPlayer = gc.getPlayer(gc.getBARBARIAN_PLAYER())
	for i in range(iNumUnits):
		barbPlayer.initUnit(iUnitType, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK_CITY_LEMMING, DirectionTypes.DIRECTION_SOUTH)

	# (loopUnit, iter) = barbPlayer.firstUnit(False)
	# while (loopUnit):
	#	if loopUnit.getUnitType() == iUnitType:
	#		loopUnit.setName("Mercenary Warrior")
	#	(loopUnit, iter) = barbPlayer.nextUnit(iter, False)

def applyMercenariesClassical2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	listPlots = []
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			listPlots.append(i)

	if 0 == len(listPlots):
		return

	plot = map.plotByIndex(listPlots[gc.getGame().getSorenRandNum(len(listPlots), "Mercenary event location")])

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		iNumUnits  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		iNumUnits  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		iNumUnits  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		iNumUnits  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		iNumUnits  = 3
	else:
		iNumUnits  = 3

	iUnitType = gc.getInfoTypeForString("UNIT_AXEMAN")

	for i in range(iNumUnits):
		player.initUnit(iUnitType, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK_CITY, DirectionTypes.DIRECTION_SOUTH)

	# (loopUnit, iter) = player.firstUnit(False)
	# while (loopUnit):
	#	if loopUnit.getUnitType() == iUnitType:
	#		loopUnit.setName("Mercenary Warrior")
	#	(loopUnit, iter) = player.nextUnit(iter, False)

######## MERCENARIES MEDIEVAL ###########

def canTriggerMercenariesMedieval(argsList):

	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

#   If Barbarians are disabled in this game, this event will not occur.
	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_BARBARIANS):
		return False

#   At least one civ on the board must know Feudalism.
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_FEUDALISM")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

#   At least one civ on the board must know Machinery.
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_MACHINERY")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False

	# Can we build the counter unit?
	iCounterUnitClass = gc.getInfoTypeForString("UNITCLASS_LONGBOWMAN")
	iCounterUnit = gc.getCivilizationInfo(player.getCivilizationType()).getCivilizationUnits(iCounterUnitClass)
	if iCounterUnit == -1:
		return False

	(loopCity, iter) = player.firstCity(False)
	bFound = False
	while(loopCity):
		if (loopCity.canTrain(iCounterUnit, False, False, False, False)):
			bFound = True
			break

		(loopCity, iter) = player.nextCity(iter, False)

	if not bFound:
		return False

#	Find an eligible plot
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			return True

	return False


def getHelpMercenariesMedieval1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_MERCENARIES_MEDIEVAL_HELP_1", ())

	return szHelp


def applyMercenariesMedieval1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	listPlots = []
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			listPlots.append(i)

	if 0 == len(listPlots):
		return

	plot = map.plotByIndex(listPlots[gc.getGame().getSorenRandNum(len(listPlots), "Mercenary event location")])

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		iNumUnits  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		iNumUnits  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		iNumUnits  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		iNumUnits  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		iNumUnits  = 3
	else:
		iNumUnits  = 3

	iUnitType = gc.getInfoTypeForString("UNIT_SWORDSMAN")

	# PLAYER DIDN'T GIVE GOLD TO MERCENARIES SO THEY ATTACK
	barbPlayer = gc.getPlayer(gc.getBARBARIAN_PLAYER())
	for i in range(iNumUnits):
		barbPlayer.initUnit(iUnitType, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK_CITY_LEMMING, DirectionTypes.DIRECTION_SOUTH)

	# (loopUnit, iter) = barbPlayer.firstUnit(False)
	# while (loopUnit):
	#	if loopUnit.getUnitType() == iUnitType:
	#		loopUnit.setName("Mercenary Warrior")
	#	(loopUnit, iter) = barbPlayer.nextUnit(iter, False)

def applyMercenariesMedieval2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	listPlots = []
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			listPlots.append(i)

	if 0 == len(listPlots):
		return

	plot = map.plotByIndex(listPlots[gc.getGame().getSorenRandNum(len(listPlots), "Mercenary event location")])

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		iNumUnits  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		iNumUnits  = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		iNumUnits  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		iNumUnits  = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		iNumUnits  = 3
	else:
		iNumUnits  = 3

	iUnitType = gc.getInfoTypeForString("UNIT_SWORDSMAN")

	# PLAYER BRIBED THE BARBARIANS
	for i in range(iNumUnits):
		player.initUnit(iUnitType, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK_CITY, DirectionTypes.DIRECTION_SOUTH)

	# (loopUnit, iter) = player.firstUnit(False)
	# while (loopUnit):
	#	if loopUnit.getUnitType() == iUnitType:
	#		loopUnit.setName("Mercenary Warrior")
	#	(loopUnit, iter) = player.nextUnit(iter, False)

######## EARTHQUAKE ###########

def getHelpEarthquake1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_EARTHQUAKE_1_HELP", ())

	return szHelp

def canApplyEarthquake1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	iNumImprovements = 0
	for iDX in range(-1, 2):
		for iDY in range(-1, 2):
			loopPlot = plotXY(kTriggeredData.iPlotX, kTriggeredData.iPlotY, iDX, iDY)
			if not loopPlot.isNone():
				if (iDX != 0 or iDY != 0):
					if loopPlot.getImprovementType() != -1:
						iNumImprovements += 1

	return (iNumImprovements > 0)

def applyEarthquake1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	listPlots = []
	for iDX in range(-1, 2):
		for iDY in range(-1, 2):
			loopPlot = plotXY(kTriggeredData.iPlotX, kTriggeredData.iPlotY, iDX, iDY)
			if not loopPlot.isNone():
				if (iDX != 0 or iDY != 0):
					if loopPlot.getImprovementType() != -1:
						listPlots.append(loopPlot)

	listRuins = []
	listRuins.append(gc.getInfoTypeForString("IMPROVEMENT_COTTAGE"))
	listRuins.append(gc.getInfoTypeForString("IMPROVEMENT_HAMLET"))
	listRuins.append(gc.getInfoTypeForString("IMPROVEMENT_VILLAGE"))
	listRuins.append(gc.getInfoTypeForString("IMPROVEMENT_TOWN"))

	iRuins = gc.getInfoTypeForString("IMPROVEMENT_CITY_RUINS")

	for i in range(3):
		if len(listPlots) > 0:
			plot = listPlots[gc.getGame().getSorenRandNum(len(listPlots), "Earthquake event improvement destroyed")]
			iImprovement = plot.getImprovementType()
			szBuffer = localText.getText("TXT_KEY_EVENT_CITY_IMPROVEMENT_DESTROYED", (gc.getImprovementInfo(iImprovement).getTextKey(), ))
			CyInterface().addMessage(kTriggeredData.ePlayer, False, gc.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_BOMBARDED", InterfaceMessageTypes.MESSAGE_TYPE_INFO, gc.getImprovementInfo(iImprovement).getButton(), gc.getInfoTypeForString("COLOR_RED"), plot.getX(), plot.getY(), True, True)
			if iImprovement in listRuins:
				plot.setImprovementType(iRuins)
			else:
				plot.setImprovementType(-1)
			listPlots.remove(plot)

			if i == 1 and gc.getGame().getSorenRandNum(100, "Earthquake event num improvements destroyed") < 50:
				break


####### Assassin Discovered #######


def canDoAssassinDiscovered3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	player = gc.getPlayer(kTriggeredData.ePlayer)

	if player.getCapitalCity().isNone():
		return False

	return True

def doAssassinDiscovered3(argsList):
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	team = gc.getTeam(player.getTeam())

	plot = player.getCapitalCity().plot()
	iNumUnits = player.getNumCities() / 4
	if iNumUnits < 1:
		iNumUnits = 1

	iCivServ = gc.getInfoTypeForString("TECH_CIVIL_SERVICE")
	iMach = gc.getInfoTypeForString("TECH_MACHINERY")
	iMatch = gc.getInfoTypeForString("TECH_MATCHLOCK")
	iRifle = gc.getInfoTypeForString("TECH_RIFLING")
	iCopper = gc.getInfoTypeForString("BONUS_COPPER")
	iIron = gc.getInfoTypeForString("BONUS_IRON")

	iUnitClassType = gc.getInfoTypeForString("UNITCLASS_AXEMAN")
	if (iMach > 0):
		if (iCivServ > 0):
			if team.isHasTech(iMach) and team.isHasTech(iCivServ) and (player.hasBonus(iCopper) or player.hasBonus(iIron)):
				iUnitClassType = gc.getInfoTypeForString("UNITCLASS_MACEMAN")
	if (iMatch > 0):
		if team.isHasTech(iMatch):
			iUnitClassType = gc.getInfoTypeForString("UNITCLASS_MUSKETMAN")
	if (iRifle > 0):
		if team.isHasTech(iRifle):
			iUnitClassType = gc.getInfoTypeForString("UNITCLASS_RIFLEMAN")

	iUnitType = gc.getCivilizationInfo(player.getCivilizationType()).getCivilizationUnits(iUnitClassType)

	if iUnitType != -1:
		for i in range(iNumUnits):
			player.initUnit(iUnitType, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK, DirectionTypes.DIRECTION_SOUTH)

def getHelpAssassinDiscovered3(argsList):
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	team = gc.getTeam(player.getTeam())
	iNumUnits = player.getNumCities() / 4
	if iNumUnits < 1:
		iNumUnits = 1

	iCivServ = gc.getInfoTypeForString("TECH_CIVIL_SERVICE")
	iMach = gc.getInfoTypeForString("TECH_MACHINERY")
	iMatch = gc.getInfoTypeForString("TECH_MATCHLOCK")
	iRifle = gc.getInfoTypeForString("TECH_RIFLING")
	iCopper = gc.getInfoTypeForString("BONUS_COPPER")
	iIron = gc.getInfoTypeForString("BONUS_IRON")

	iUnitClassType = gc.getInfoTypeForString("UNITCLASS_AXEMAN")
	if (iMach > 0):
		if (iCivServ > 0):
			if team.isHasTech(iMach) and team.isHasTech(iCivServ) and (player.hasBonus(iCopper) or player.hasBonus(iIron)):
				iUnitClassType = gc.getInfoTypeForString("UNITCLASS_MACEMAN")
	if (iMatch > 0):
		if team.isHasTech(iMatch):
			iUnitClassType = gc.getInfoTypeForString("UNITCLASS_MUSKETMAN")
	if (iRifle > 0):
		if team.isHasTech(iRifle):
			iUnitClassType = gc.getInfoTypeForString("UNITCLASS_RIFLEMAN")

	iUnitType = gc.getCivilizationInfo(player.getCivilizationType()).getCivilizationUnits(iUnitClassType)

	szHelp = localText.getText("TXT_KEY_EVENT_BONUS_UNIT", (iNumUnits, gc.getUnitInfo(iUnitType).getTextKey()))

	return szHelp

######## BLACK DEATH PLAGUE ###########

def canTriggerBlackDeath(argsList):
	kTriggeredData = argsList[0]

	player = gc.getPlayer(kTriggeredData.ePlayer)
	team = gc.getTeam(player.getTeam())

	iClassical = gc.getInfoTypeForString("ERA_CLASSICAL")

	if player.getCurrentEra() <= iClassical:
		return False

	iMedicine = gc.getInfoTypeForString("TECH_MEDICINE")

	if team.isHasTech(iMedicine):
		return False

	return True

def applyBlackDeath2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	player = gc.getPlayer(kTriggeredData.ePlayer)
	eventCity = player.getCity(kTriggeredData.iCityId)

	iNumCities = 2 + gc.getGame().getSorenRandNum(3, "Black Death event number of cities")

	listCities = []
	(loopCity, iter) = player.firstCity(False)
	while(loopCity):
		if loopCity.getPopulation() > 2:
			iDistance = plotDistance(eventCity.getX(), eventCity.getY(), loopCity.getX(), loopCity.getY())
			if iDistance > 0:
				listCities.append((iDistance, loopCity))
		(loopCity, iter) = player.nextCity(iter, False)

	listCities.sort(key=itemgetter(0))

	if iNumCities > len(listCities):
		iNumCities = len(listCities)

	for i in range(iNumCities):
		(iDist, loopCity) = listCities[i]
		loopCity.changePopulation(-2)
		szBuffer = localText.getText("TXT_KEY_EVENT_BLACK_DEATH_HIT_CITY", (loopCity.getNameKey(), ))
		CyInterface().addMessage(kTriggeredData.ePlayer, False, gc.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_PILLAGE", InterfaceMessageTypes.MESSAGE_TYPE_INFO, None, gc.getInfoTypeForString("COLOR_RED"), loopCity.getX(), loopCity.getY(), True, True)


def getHelpBlackDeath2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_BLACK_DEATH_HELP_2", (2, ))

	return szHelp

######## SMALLPOX EPIDEMIC ###########

def canTriggerSmallpox(argsList):
	kTriggeredData = argsList[0]

	player = gc.getPlayer(kTriggeredData.ePlayer)
	team = gc.getTeam(player.getTeam())

	iClassical = gc.getInfoTypeForString("ERA_CLASSICAL")

	if player.getCurrentEra() <= iClassical:
		return False

	iMedicine = gc.getInfoTypeForString("TECH_MEDICINE")

	if team.isHasTech(iMedicine):
		return False

	return True

def applySmallpox2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	player = gc.getPlayer(kTriggeredData.ePlayer)
	eventCity = player.getCity(kTriggeredData.iCityId)

	iNumCities = 1 + gc.getGame().getSorenRandNum(3, "Smallpox event number of cities")

	listCities = []
	(loopCity, iter) = player.firstCity(False)
	while(loopCity):
		if loopCity.getPopulation() > 2:
			iDistance = plotDistance(eventCity.getX(), eventCity.getY(), loopCity.getX(), loopCity.getY())
			if iDistance > 0:
				listCities.append((iDistance, loopCity))
		(loopCity, iter) = player.nextCity(iter, False)

	listCities.sort(key=itemgetter(0))

	if iNumCities > len(listCities):
		iNumCities = len(listCities)

	for i in range(iNumCities):
		(iDist, loopCity) = listCities[i]
		loopCity.changePopulation(-1)
		szBuffer = localText.getText("TXT_KEY_EVENT_SMALLPOX_HIT_CITY", (loopCity.getNameKey(), ))
		CyInterface().addMessage(kTriggeredData.ePlayer, False, gc.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_PILLAGE", InterfaceMessageTypes.MESSAGE_TYPE_INFO, None, gc.getInfoTypeForString("COLOR_RED"), loopCity.getX(), loopCity.getY(), True, True)


def getHelpSmallpox2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_SMALLPOX_HELP_2", (2, ))

	return szHelp


####### Measles ########

def canTriggerMeasles(argsList):
	ePlayer = argsList[1]
	iCity = argsList[2]

	player = gc.getPlayer(ePlayer)
	city = player.getCity(iCity)

	if city.isNone():
		return False

	# city health is positive, no epidemic
	if ((city.goodHealth() - city.badHealth(True)) > 1):
		return False

	return True



	######## THE SILVER RAIN ###########

def canTriggerSilverRain(argsList):

	kTriggeredData = argsList[0]
	pPlayer = gc.getPlayer(kTriggeredData.ePlayer)

#   If Barbarians are disabled in this game, this event will not occur.
	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_BARBARIANS):
		return False

#   At least one civ on the board must know Weather control
	bFoundValid = False
	iTech = gc.getInfoTypeForString("TECH_WEATHER_CONTROL")
	for iPlayer in range(gc.getMAX_CIV_PLAYERS()):
		loopPlayer = gc.getPlayer(iPlayer)
		if loopPlayer.isAlive():
			if gc.getTeam(loopPlayer.getTeam()).isHasTech(iTech):
				bFoundValid = True
				break

	if not bFoundValid:
		return False


#	Find an eligible plot
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			return True

	return False

def getHelpSilverRain1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_SILVER_RAIN_HELP_1", ())

	return szHelp


def applySilverRain1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	listPlots = []
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			listPlots.append(i)

	if 0 == len(listPlots):
		return

	plot = map.plotByIndex(listPlots[gc.getGame().getSorenRandNum(len(listPlots), "The Silver Rain event location")])

	if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
		iNumUnits1 = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
		iNumUnits1 = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
		iNumUnits1 = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
		iNumUnits1 = 1
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
		iNumUnits1 = 2
	elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_HUGE"):
		iNumUnits1 = 2
	else:
		iNumUnits1 = 3


	iUnitType1 = gc.getInfoTypeForString("UNIT_NANITE_CLOUD")

	barbPlayer = gc.getPlayer(gc.getBARBARIAN_PLAYER())
	for i in range(iNumUnits1):
		barbPlayer.initUnit(iUnitType1, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK_CITY_LEMMING, DirectionTypes.DIRECTION_SOUTH)

def getHelpSilverRain3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_SILVER_RAIN_HELP_3", ())

	return szHelp

def applySilverRain3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	listPlots = []
	map = gc.getMap()
	for i in range(map.numPlots()):
		plot = map.plotByIndex(i)
		if (plot.getOwner() == -1 and not plot.isWater() and not plot.isImpassable() and plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
			listPlots.append(i)

	if 0 == len(listPlots):
		return

	plot = map.plotByIndex(listPlots[gc.getGame().getSorenRandNum(len(listPlots), "Silver Rain event location")])


	if (gc.getDefineINT("CIV4_VERSION") >= 317):
		# Can we build the counter unit?
		iCounterUnitClass1 = gc.getInfoTypeForString("UNITCLASS_TACTICAL_NUKE")
		iCounterUnit1 = gc.getCivilizationInfo(player.getCivilizationType()).getCivilizationUnits(iCounterUnitClass1)

		iCounterUnitClass2 = gc.getInfoTypeForString("UNITCLASS_ICBM")
		iCounterUnit2 = gc.getCivilizationInfo(player.getCivilizationType()).getCivilizationUnits(iCounterUnitClass2)
		(loopCity, iter) = player.firstCity(False)
		bFound = False
		while(loopCity):
			if (loopCity.canTrain(iCounterUnit1, False, False, False, False) or loopCity.canTrain(iCounterUnit2, False, False, False, False)):
				bFound = True
				break

	if ( bFound == True):
		iUnitType = gc.getInfoTypeForString("UNIT_NANITE_CLOUD")
		barbPlayer = gc.getPlayer(gc.getBARBARIAN_PLAYER())
		iNukeUnit = barbPlayer.initUnit(iUnitType, plot.getX(), plot.getY(), UnitAITypes.NO_UNITAI, DirectionTypes.DIRECTION_SOUTH)
		plot.nukeExplosion(1, iNukeUnit)
		iNukeUnit.kill(False, -1)
	else:
		if map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_DUEL"):
			iNumUnits1 = 1
		elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_TINY"):
			iNumUnits1 = 1
		elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_SMALL"):
			iNumUnits1 = 1
		elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_STANDARD"):
			iNumUnits1 = 1
		elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_LARGE"):
			iNumUnits1 = 2
		elif map.getWorldSize() == gc.getInfoTypeForString("WORLDSIZE_HUGE"):
			iNumUnits1 = 2
		else:
			iNumUnits1 = 3

		iUnitType1 = gc.getInfoTypeForString("UNIT_NANITE_CLOUD")

		barbPlayer = gc.getPlayer(gc.getBARBARIAN_PLAYER())
		for i in range(iNumUnits1):
			barbPlayer.initUnit(iUnitType1, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ATTACK_CITY_LEMMING, DirectionTypes.DIRECTION_SOUTH)


# Sea Storm 3 event

def canTriggerSeaStorm3(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)
	if (plot == None):
		return False

	if ( plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
		return False

	iNatWeather = gc.getInfoTypeForString("BUILDINGCLASS_NWS")
	if player.getBuildingClassCountWithUpgrades(iNatWeather) > 0:
		return False

	if (plot.getOwner() != kTriggeredData.ePlayer and plot.isWater() and not plot.isImpassable()):
		if gc.getGame().getSorenRandNum(100, "Ship sinking chance 85 percent") < 85:
			return True

	return False


# Sea Storm 4 event

def canTriggerSeaStorm4(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)
	if (plot == None):
		return False

	if ( plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
		return False

	iNatWeather = gc.getInfoTypeForString("BUILDINGCLASS_NWS")
	if player.getBuildingClassCountWithUpgrades(iNatWeather) > 0:
		return False

	if (plot.getOwner() != kTriggeredData.ePlayer and plot.isWater() and not plot.isImpassable()):
		if gc.getGame().getSorenRandNum(100, "Ship sinking chance 75 percent") < 75:
			return True

	return False

# Sea Storm 5 event

def canTriggerSeaStorm5(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)
	if (plot == None):
		return False

	if ( plot.area().getCitiesPerPlayer(kTriggeredData.ePlayer) > 0 and plot.isAdjacentPlayer(kTriggeredData.ePlayer, True)):
		return False

	iNatWeather = gc.getInfoTypeForString("BUILDINGCLASS_NWS")
	if player.getBuildingClassCountWithUpgrades(iNatWeather) > 0:
		return False

	if (plot.getOwner() != kTriggeredData.ePlayer and plot.isWater() and not plot.isImpassable()):
		if gc.getGame().getSorenRandNum(100, "Ship sinking chance 75 percent") < 75:
			return True

	return False

# ########### City ruins treasure event ###################

def canTriggerCityRuinsTreasure1(argsList):

	kTriggeredData = argsList[0]

	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)

	# check for modmods if they add sea cities, do not allow treasure unit to spawn on water plots if city ruins are in water
	if (plot.isWater() or plot.isImpassable()):
			return False

	return True

def getHelpCityRuinsTreasure1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	iUnitType = gc.getInfoTypeForString("UNIT_TREASURE")
	szHelp = localText.getText("TXT_KEY_EVENT_BONUS_UNIT", (1, gc.getUnitInfo(iUnitType).getTextKey()))

	return szHelp

def applyCityRuinsTreasure1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)

	iUnitType = gc.getInfoTypeForString("UNIT_TREASURE")
	iNumUnits = 1

	for i in range(iNumUnits):
		player.initUnit(iUnitType, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ENGINEER, DirectionTypes.DIRECTION_SOUTH)

# ########### City ruins relic events ###################

def canTriggerCityRuinsRelics1(argsList):

	kTriggeredData = argsList[0]

	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)

	# check for modmods if they add sea cities, do not allow treasure unit to spawn on water plots if city ruins are in water
	if (plot.isWater() or plot.isImpassable()):
			return False

	return True

def getHelpCityRuinsRelics1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	iUnitType = gc.getInfoTypeForString("UNIT_TREASURE")
	szHelp = localText.getText("TXT_KEY_EVENT_BONUS_UNIT", (1, gc.getUnitInfo(iUnitType).getTextKey()))

	return szHelp

def doCityRuinsRelics1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)

	iUnitType = gc.getInfoTypeForString("UNIT_TREASURE")
	iNumUnits = 1

	for i in range(iNumUnits):
		player.initUnit(iUnitType, plot.getX(), plot.getY(), UnitAITypes.UNITAI_ENGINEER, DirectionTypes.DIRECTION_SOUTH)

def doRelics2(argsList):
#	need this because relics normally only spawn on marsh, desert, or jungle
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)

	if not plot.isNone():
		plot.setBonusType(gc.getInfoTypeForString("BONUS_ANCIENTTEMPLE"))

	return 1

def getHelpRelics2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_TRIGGER_RELICS_HELP_2", ())

	return szHelp

def canTriggerRelics3(argsList):
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	eventCity = player.getCity(kTriggeredData.iCityId)
	team = gc.getTeam(player.getTeam())

	iScientific = gc.getInfoTypeForString("TECH_SCIENTIFIC_METHOD")
	if (iScientific > 0):
		if (not team.isHasTech(iScientific)):
			return False
	return True

def doRelics3(argsList):
#	need this because relics normally spawn on marsh, desert, or jungle and excavations are notmally not allowed unless there are already relics
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)

	if not plot.isNone():
		plot.setBonusType(gc.getInfoTypeForString("BONUS_ANCIENTTEMPLE"))
		plot.setImprovementType(gc.getInfoTypeForString("IMPROVEMENT_EXPLORETEMPLE"))

	return 1

def getHelpRelics3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	team = gc.getTeam(player.getTeam())
	iScientific = gc.getInfoTypeForString("TECH_SCIENTIFIC_METHOD")

	szHelp = localText.getText("TXT_KEY_EVENT_TRIGGER_RELICS_HELP_2", ())
	szHelp += u"\n"
	iExploreTemple = gc.getInfoTypeForString("IMPROVEMENT_EXPLORETEMPLE")
	szHelp += localText.getText("TXT_KEY_EVENT_IMPROVEMENT_GROWTH", ( gc.getImprovementInfo(iExploreTemple).getTextKey(), ))
	if (not team.isHasTech(iScientific)):
		szHelp += u"\n"
		szHelp += localText.getText("TXT_KEY_EVENT_REQUIRES_SCIENTIFIC", ())

	return szHelp

# ###########Compulsory Democracy########### #

def canTriggerCompulsoryDem(argsList):
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	eCivic = player.getCivics(gc.getInfoTypeForString("CIVICOPTION_GOVERNMENT"))
	iFederal = gc.getInfoTypeForString("CIVIC_FEDERAL")
	iDemocracy = gc.getInfoTypeForString("CIVIC_DEMOCRACY")
	iRepublic = gc.getInfoTypeForString("CIVIC_REPUBLIC")
	if eCivic == iFederal or eCivic == iDemocracy or eCivic == iRepublic:
		return True
	else:
		return False

def canEventTriggerCompulsoryDem(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	if (not canTriggerNewEvent(argsList)):
		return False

	if (player.isAnarchy() or player.getRevolutionTimer() > 0):
		return False

	eCivic = player.getCivics(gc.getInfoTypeForString("CIVICOPTION_GOVERNMENT"))
	iFederal = gc.getInfoTypeForString("CIVIC_FEDERAL")
	iDemocracy = gc.getInfoTypeForString("CIVIC_DEMOCRACY")
	iRepublic = gc.getInfoTypeForString("CIVIC_REPUBLIC")
	if eCivic == iFederal or eCivic == iDemocracy or eCivic == iRepublic:
		bFederal = player.canDoCivics(gc.getInfoTypeForString("CIVIC_FEDERAL"))
		bDemocracy = player.canDoCivics(gc.getInfoTypeForString("CIVIC_DEMOCRACY"))
		bRepublic = player.canDoCivics(gc.getInfoTypeForString("CIVIC_REPUBLIC"))
		bMonarchy = player.canDoCivics(gc.getInfoTypeForString("CIVIC_HEREDITARY_RULE"))
		bDespotism = player.canDoCivics(gc.getInfoTypeForString("CIVIC_DESPOTISM"))
		if (bFederal or bDemocracy or bRepublic):
			if (bMonarchy or bDespotism):
				return True

	return False

def TriggerCompulsoryDem2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	eCivic = player.getCivics(gc.getInfoTypeForString("CIVICOPTION_GOVERNMENT"))
	iFederal = gc.getInfoTypeForString("CIVIC_FEDERAL")

	bFederal = player.canDoCivics(gc.getInfoTypeForString("CIVIC_FEDERAL"))
	bDemocracy = player.canDoCivics(gc.getInfoTypeForString("CIVIC_DEMOCRACY"))
	bRepublic = player.canDoCivics(gc.getInfoTypeForString("CIVIC_REPUBLIC"))

	curCivics = []

	for i in range(gc.getNumCivicOptionInfos()):
		curCivics.append(player.getCivics(i))

	if bFederal and eCivic != iFederal:
		curCivics[gc.getInfoTypeForString("CIVICOPTION_GOVERNMENT")] = iFederal
	elif eCivic != gc.getInfoTypeForString("CIVIC_DEMOCRACY") and bDemocracy:
		curCivics[gc.getInfoTypeForString("CIVICOPTION_GOVERNMENT")] = gc.getInfoTypeForString("CIVIC_DEMOCRACY")
	elif eCivic != gc.getInfoTypeForString("CIVIC_REPUBLIC") and bRepublic:
		curCivics[gc.getInfoTypeForString("CIVICOPTION_GOVERNMENT")] = gc.getInfoTypeForString("CIVIC_REPUBLIC")

	player.doRevolution(curCivics, False)

def TriggerCompulsoryDem3(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	eCivic = player.getCivics(gc.getInfoTypeForString("CIVICOPTION_GOVERNMENT"))

	# bFederal = player.canDoCivics(gc.getInfoTypeForString("CIVIC_FEDERAL"))
	# bDemocracy = player.canDoCivics(gc.getInfoTypeForString("CIVIC_DEMOCRACY"))
	# bRepublic = player.canDoCivics(gc.getInfoTypeForString("CIVIC_REPUBLIC"))
	bMonarchy = player.canDoCivics(gc.getInfoTypeForString("CIVIC_HEREDITARY_RULE"))
	bDespotism = player.canDoCivics(gc.getInfoTypeForString("CIVIC_DESPOTISM"))

	curCivics = []

	for i in range(gc.getNumCivicOptionInfos()):
		curCivics.append(player.getCivics(i))

	if bMonarchy:
		curCivics[gc.getInfoTypeForString("CIVICOPTION_GOVERNMENT")] = gc.getInfoTypeForString("CIVIC_HEREDITARY_RULE")
	elif bDespotism:
		curCivics[gc.getInfoTypeForString("CIVICOPTION_GOVERNMENT")] = gc.getInfoTypeForString("CIVIC_DESPOTISM")
	player.doRevolution(curCivics, False)

def getHelpCompulsoryDem1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	szHelp = localText.getText("TXT_KEY_EVENT_TRIGGER_COMPULSORY_DEM_HELP_1", ())

	return szHelp

def getHelpCompulsoryDem2(argsList):
	import BugUtil
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	eCivic = player.getCivics(gc.getInfoTypeForString("CIVICOPTION_GOVERNMENT"))
	iFederal = gc.getInfoTypeForString("CIVIC_FEDERAL")

	bFederal = player.canDoCivics(gc.getInfoTypeForString("CIVIC_FEDERAL"))
	if bFederal:
		BugUtil.debug("Can Switch to Federal")
	bDemocracy = player.canDoCivics(gc.getInfoTypeForString("CIVIC_DEMOCRACY"))
	if bDemocracy:
		BugUtil.debug("Can Switch to Democracy")
	bRepublic = player.canDoCivics(gc.getInfoTypeForString("CIVIC_REPUBLIC"))
	if bRepublic:
		BugUtil.debug("Can Switch to Republic")
	bMonarchy = player.canDoCivics(gc.getInfoTypeForString("CIVIC_HEREDITARY_RULE"))
	if bMonarchy:
		BugUtil.debug("Can Switch to Monarchy")
	bDespotism = player.canDoCivics(gc.getInfoTypeForString("CIVIC_DESPOTISM"))
	if bDespotism:
		BugUtil.debug("Can Switch to Despotism")

	szType = ""

	if (iEvent == gc.getInfoTypeForString("EVENT_COMPULSORY_DEM_2")):
		BugUtil.debug("Event #2")
		if bFederal and eCivic != iFederal:
			szType = gc.getCivicInfo(gc.getInfoTypeForString("CIVIC_FEDERAL")).getTextKey()
			BugUtil.debug("Can Switch to Federal")
		elif eCivic != gc.getInfoTypeForString("CIVIC_DEMOCRACY") and bDemocracy:
			szType = gc.getCivicInfo(gc.getInfoTypeForString("CIVIC_DEMOCRACY")).getTextKey()
			BugUtil.debug("Can Switch to Democracy")
		elif eCivic != gc.getInfoTypeForString("CIVIC_REPUBLIC") and bRepublic:
			szType = gc.getCivicInfo(gc.getInfoTypeForString("CIVIC_REPUBLIC")).getTextKey()
			BugUtil.debug("Can Switch to Republic")
	else:
		BugUtil.debug("Event #3")
		if bMonarchy:
			szType = gc.getCivicInfo(gc.getInfoTypeForString("CIVIC_HEREDITARY_RULE")).getTextKey()
			BugUtil.debug("Can Switch to Monarchy")
		elif bDespotism:
			szType = gc.getCivicInfo(gc.getInfoTypeForString("CIVIC_DESPOTISM")).getTextKey()
			BugUtil.debug("Can Switch to Despotism")

	szHelp = BugUtil.getText("TXT_KEY_EVENT_CHANGE_CIVICS", szType)

	return szHelp

def TriggerHarryPotter2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	(loopCity, iter) = player.firstCity(False)
	while(loopCity):
		iLibrary = gc.getInfoTypeForString("BUILDINGCLASS_LIBRARY")
		iLibrary = gc.getCivilizationInfo(loopCity.getCivilizationType()).getCivilizationBuildings(iLibrary)
		if (loopCity.canConstruct(iLibrary, False, False, True)):
			loopCity.setNumRealBuilding(iLibrary, 1)

		if (loopCity.isHasReligion(player.getStateReligion())):
			loopCity.changeHurryAngerTimer(loopCity.flatHurryAngerLength())

		(loopCity, iter) = player.nextCity(iter, False)

	for ePlayer in range(gc.getMAX_PLAYERS()):
		kPlayer = gc.getPlayer(ePlayer)
		if (kPlayer.isAlive() and gc.getTeam(kPlayer.getTeam()).isHasMet(player.getTeam())):
			if (kPlayer.getStateReligion() == player.getStateReligion()):
				kPlayer.AI_changeMemoryCount(player.getID(), MemoryTypes.MEMORY_EVENT_BAD_TO_US, 1)

def getHelpHarryPotter2(argsList):
	import BugUtil
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iLibrary = gc.getInfoTypeForString("BUILDINGCLASS_LIBRARY")
	iLibrary = gc.getCivilizationInfo(player.getCivilizationType()).getCivilizationBuildings(iLibrary)

	szHelp = BugUtil.getText("TXT_KEY_EVENT_FREE_BUILDING_GLOBAL", gc.getBuildingInfo(iLibrary).getTextKey())
	szHelp += "\n"
	szHelp += BugUtil.getText("TXT_KEY_EVENT_CAUSES_HOLY_ANGER")
	szHelp += "\n"
	szHelp += BugUtil.getText("TXT_KEY_EVENT_CAUSES_FOREIGN_HOLY_ANGER")

	return szHelp

def canEventTriggerLessDictator(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	if (not canTriggerNewEvent(argsList)):
		return False

	if (player.isAnarchy() or player.getRevolutionTimer() > 0):
		return False

	eCivic = player.getCivics(gc.getInfoTypeForString("CIVICOPTION_GOVERNMENT"))
	iMonarch = gc.getInfoTypeForString("CIVIC_HEREDITARY_RULE")
	iFascist = gc.getInfoTypeForString("CIVIC_FASCIST")
	iDespot = gc.getInfoTypeForString("CIVIC_DESPOTISM")
	if eCivic == iMonarch or eCivic == iFascist or eCivic == iDespot:
		return True

	return False

def TriggerLessDictator1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	(loopCity, iter) = player.firstCity(False)
	while(loopCity):
		loopCity.setWeLoveTheKingDay(True)
		(loopCity, iter) = player.nextCity(iter, False)

def getHelpLessDictator1(argsList):
	import BugUtil
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	szHelp = BugUtil.getText("TXT_KEY_EVENT_NATIONAL_HOLIDAY")
	return szHelp

def TriggerLessDictator2(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	eCivic = player.getCivics(gc.getInfoTypeForString("CIVICOPTION_GOVERNMENT"))
	iFederal = gc.getInfoTypeForString("CIVIC_FEDERAL")
	iRepublic = gc.getInfoTypeForString("CIVIC_REPUBLIC")
	iDemocracy = gc.getInfoTypeForString("CIVIC_DEMOCRACY")

	curCivics = []
	for i in range(gc.getNumCivicOptionInfos()):
		curCivics.append(player.getCivics(i))

	bFederal = player.canDoCivics(iFederal)
	bDemocracy = player.canDoCivics(iDemocracy)
	bRepublic = player.canDoCivics(iRepublic)

	if bFederal:
		curCivics[gc.getInfoTypeForString("CIVICOPTION_GOVERNMENT")] = iFederal
	elif bDemocracy:
		curCivics[gc.getInfoTypeForString("CIVICOPTION_GOVERNMENT")] = iDemocracy
	elif bRepublic:
		curCivics[gc.getInfoTypeForString("CIVICOPTION_GOVERNMENT")] = iRepublic

	player.doRevolution(curCivics, False)

def getHelpLessDictator2(argsList):
	import BugUtil
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iFederal = gc.getInfoTypeForString("CIVIC_FEDERAL")
	iRepublic = gc.getInfoTypeForString("CIVIC_REPUBLIC")
	iDemocracy = gc.getInfoTypeForString("CIVIC_DEMOCRACY")

	szType = ""

	bFederal = player.canDoCivics(iFederal)
	bDemocracy = player.canDoCivics(iDemocracy)
	bRepublic = player.canDoCivics(iRepublic)

	if bFederal:
		szType = gc.getCivicInfo(iFederal).getTextKey()
	elif bDemocracy:
		szType = gc.getCivicInfo(iDemocracy).getTextKey()
	elif bRepublic:
		szType = gc.getCivicInfo(iRepublic).getTextKey()

	szHelp = BugUtil.getText("TXT_KEY_EVENT_CHANGE_CIVICS", szType)
	return szHelp

def canTriggerUnlimitedPower1(argsList):
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	eventCity = player.getCity(kTriggeredData.iCityId)

	if (eventCity.isPower()):
		return False
	return True

def TriggerUnlimitedPower1(argsList):
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	eventCity = player.getCity(kTriggeredData.iCityId)

	eventCity.changePowerCount(1, False)

def getHelpUnlimitedPower1(argsList):
	import BugUtil
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	eventCity = player.getCity(kTriggeredData.iCityId)

	szHelp = BugUtil.getText("TXT_KEY_EVENT_PROVIDES_POWER", eventCity.getNameKey())
	return szHelp

def canTriggerUnlimitedPower2(argsList):
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	eventCity = player.getCity(kTriggeredData.iCityId)

	if (eventCity.area().isNone()):
		return False
	if (eventCity.area().isCleanPower(player.getTeam())):
		return False
	if (player.getGold() < (eventCity.area().getNumCities() * 100)):
		return False
	return True

def TriggerUnlimitedPower2(argsList):
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	eventCity = player.getCity(kTriggeredData.iCityId)
	iRequireGold = (eventCity.area().getNumCities() * 100) # added by 45�
	player.changeGold(-iRequireGold) # added by 45�
	eventCity.area().changeCleanPowerCount(player.getTeam(), 1)

def getHelpUnlimitedPower2(argsList):
	import BugUtil
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	eventCity = player.getCity(kTriggeredData.iCityId)

	szHelp = BugUtil.getText("TXT_KEY_EVENT_PROVIDES_AREA_POWER")
	szHelp += u"\n"
	szTempBuffer = BugUtil.getText("TXT_KEY_EVENT_COSTS_GOLD", (eventCity.area().getNumCities() * 100))
	if (player.getGold() < (eventCity.area().getNumCities() * 100)):
		szTempBuffer = BugUtil.colorText(szTempBuffer, "COLOR_WARNING_TEXT")
	szHelp += szTempBuffer
	return szHelp

def TriggerSuperVirus1(argsList):
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	eventCity = player.getCity(kTriggeredData.iCityId)

	iNumCities = 4 + gc.getGame().getSorenRandNum(5, "Super Virus event number of cities")

	listCities = []
	(loopCity, iter) = player.firstCity(False)
	while(loopCity):
		loopCity.changeEventAnger(3)
		if loopCity.getPopulation() > 4:
			iDistance = plotDistance(eventCity.getX(), eventCity.getY(), loopCity.getX(), loopCity.getY())
			if iDistance > 0:
				listCities.append((iDistance, loopCity))
		(loopCity, iter) = player.nextCity(iter, False)

	listCities.sort(key=itemgetter(0))

	if iNumCities > len(listCities):
		iNumCities = len(listCities)

	iKilledPop = (eventCity.getPopulation() * 75) / 100

	eventCity.changePopulation(-iKilledPop)
	szBuffer = localText.getText("TXT_KEY_EVENT_SUPER_VIRUS_HIT_CITY", (iKilledPop, eventCity.getNameKey() , ))
	CyInterface().addMessage(kTriggeredData.ePlayer, False, gc.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_PILLAGE", InterfaceMessageTypes.MESSAGE_TYPE_INFO, None, gc.getInfoTypeForString("COLOR_RED"), eventCity.getX(), eventCity.getY(), True, True)

	for i in range(iNumCities):
		(iDist, loopCity) = listCities[i]
		loopCity.changePopulation(-4)
		szBuffer = localText.getText("TXT_KEY_EVENT_SUPER_VIRUS_HIT_CITY", (4, loopCity.getNameKey() , ))
		CyInterface().addMessage(kTriggeredData.ePlayer, False, gc.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_PILLAGE", InterfaceMessageTypes.MESSAGE_TYPE_INFO, None, gc.getInfoTypeForString("COLOR_RED"), loopCity.getX(), loopCity.getY(), True, True)

def getHelpSuperVirus1(argsList):
	import BugUtil
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	eventCity = player.getCity(kTriggeredData.iCityId)
	cityName = BugUtil.getText(eventCity.getNameKey())

	szHelp = BugUtil.getText("TXT_KEY_EVENT_DOOMED_OPTION")
	szHelp += u"\n"
	szHelp += localText.getText("TXT_KEY_EVENT_CITY_UNREST", (3, cityName, ))
	return szHelp


def TriggerSuperVirus2(argsList):
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	eventCity = player.getCity(kTriggeredData.iCityId)

	iNumCities = 1 + gc.getGame().getSorenRandNum(3, "Super Virus event number of cities")

	iRequireGold = 100 + player.getNumCities() * 35
	player.changeGold(-iRequireGold)

	listCities = []
	(loopCity, iter) = player.firstCity(False)
	while(loopCity):
		loopCity.changeEventAnger(1)
		if loopCity.getPopulation() > 4:
			iDistance = plotDistance(eventCity.getX(), eventCity.getY(), loopCity.getX(), loopCity.getY())
			if iDistance > 0:
				listCities.append((iDistance, loopCity))
		(loopCity, iter) = player.nextCity(iter, False)

	listCities.sort(key=itemgetter(0))

	if iNumCities > len(listCities):
		iNumCities = len(listCities)

	iKilledPop = (eventCity.getPopulation() * 45) / 100

	eventCity.changePopulation(-iKilledPop)
	szBuffer = localText.getText("TXT_KEY_EVENT_SUPER_VIRUS_HIT_CITY", (iKilledPop, eventCity.getNameKey() , ))
	CyInterface().addMessage(kTriggeredData.ePlayer, False, gc.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_PILLAGE", InterfaceMessageTypes.MESSAGE_TYPE_INFO, None, gc.getInfoTypeForString("COLOR_RED"), eventCity.getX(), eventCity.getY(), True, True)

	for i in range(iNumCities):
		(iDist, loopCity) = listCities[i]
		loopCity.changePopulation(-2)
		szBuffer = localText.getText("TXT_KEY_EVENT_SUPER_VIRUS_HIT_CITY", (2, loopCity.getNameKey() , ))
		CyInterface().addMessage(kTriggeredData.ePlayer, False, gc.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_PILLAGE", InterfaceMessageTypes.MESSAGE_TYPE_INFO, None, gc.getInfoTypeForString("COLOR_RED"), loopCity.getX(), loopCity.getY(), True, True)

def getHelpSuperVirus2(argsList):
	import BugUtil
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	eventCity = player.getCity(kTriggeredData.iCityId)

	szHelp = BugUtil.getText("TXT_KEY_EVENT_SMALL_VIRUS_SPREAD")
	szHelp += u"\n"
	szHelp += localText.getText("TXT_KEY_EVENT_CITY_UNREST", (1, eventCity.getNameKey(), ))
	szHelp += u"\n"
	szTempBuffer = BugUtil.getText("TXT_KEY_EVENT_COSTS_GOLD", (100 + player.getNumCities() * 35))
	if (player.getGold() < (100 + player.getNumCities() * 35)):
		szTempBuffer = BugUtil.colorText(szTempBuffer, "COLOR_WARNING_TEXT")
	szHelp += szTempBuffer

	return szHelp
def canTriggerSuperVirus2(argsList):
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	eventCity = player.getCity(kTriggeredData.iCityId)

	iRequireGold = 100 + player.getNumCities() * 35
	if (player.getGold() < (iRequireGold)):
		return False
	return True

def TriggerSuperVirus3(argsList):
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	eventCity = player.getCity(kTriggeredData.iCityId)

	iRequireGold = 100 + player.getNumCities() * 65
	player.changeGold(-iRequireGold)

	iChangePopulation = eventCity.getPopulation() * 25
	iChangePopulation /= 100
	eventCity.changePopulation(-iChangePopulation)
	szBuffer = localText.getText("TXT_KEY_EVENT_SUPER_VIRUS_HIT_CITY", (iChangePopulation, eventCity.getNameKey() , ))
	CyInterface().addMessage(kTriggeredData.ePlayer, False, gc.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_PILLAGE", InterfaceMessageTypes.MESSAGE_TYPE_INFO, None, gc.getInfoTypeForString("COLOR_RED"), eventCity.getX(), eventCity.getY(), True, True)

	eventCity.changeFreeSpecialistCount(gc.getInfoTypeForString("SPECIALIST_GREAT_SCIENTIST"), 2)

def getHelpSuperVirus3(argsList):
	import BugUtil
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	eventCity = player.getCity(kTriggeredData.iCityId)
	team = gc.getTeam(player.getTeam())

	iChangePopulation = eventCity.getPopulation() * 25
	iChangePopulation /= 100

	szHelp = localText.getText("TXT_KEY_EVENT_FREE_SPECIALISTS", (2, gc.getSpecialistInfo(gc.getInfoTypeForString("SPECIALIST_GREAT_SCIENTIST")).getTextKey(), eventCity.getNameKey(), ))
	szHelp += u"\n"
	szHelp += localText.getText("TXT_KEY_EVENT_LOSE_POPULATION", (iChangePopulation, eventCity.getNameKey(), ))
	szHelp += u"\n"
	szTempBuffer = BugUtil.getText("TXT_KEY_EVENT_COSTS_GOLD", (100 + player.getNumCities() * 65))
	if (player.getGold() < (100 + player.getNumCities() * 65)):
		szTempBuffer = BugUtil.colorText(szTempBuffer, "COLOR_WARNING_TEXT")
	szHelp += szTempBuffer

	iSmartMedicine = gc.getInfoTypeForString("BONUS_SMART_MEDICINE")
	iSmartDrugs = gc.getInfoTypeForString("TECH_SMART_DRUGS")
	if (iSmartMedicine > 0):
		if (not eventCity.hasBonus(iSmartMedicine)):
			szHelp += u"\n"
			szTempBuffer = BugUtil.getText("TXT_KEY_BUILDING_REQUIRES_STRING", gc.getBonusInfo(iSmartMedicine).getTextKey())
			szHelp += szTempBuffer
	if (iSmartDrugs > 0):
		if (not team.isHasTech(iSmartDrugs)):
			szHelp += u"\n"
			szTempBuffer = BugUtil.getText("TXT_KEY_BUILDING_REQUIRES_STRING", gc.getTechInfo(iSmartDrugs).getTextKey())
			szHelp += szTempBuffer

	return szHelp
def canTriggerSuperVirus3(argsList):
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	eventCity = player.getCity(kTriggeredData.iCityId)
	team = gc.getTeam(player.getTeam())

	iRequireGold = 100 + player.getNumCities() * 65
	if (player.getGold() < (iRequireGold)):
		return False
	iSmartMedicine = gc.getInfoTypeForString("BONUS_SMART_MEDICINE")
	iSmartDrugs = gc.getInfoTypeForString("TECH_SMART_DRUGS")
	if (iSmartMedicine > 0):
		if (not eventCity.hasBonus(iSmartMedicine)):
			return False
	if (iSmartDrugs > 0):
		if (not team.isHasTech(iSmartDrugs)):
			return False
	return True

def TriggerSuperVirus4(argsList):
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	eventCity = player.getCity(kTriggeredData.iCityId)

	iChangePopulation = eventCity.getPopulation() * 75
	iChangePopulation /= 100

	eventCity.changeOccupationTimer(iChangePopulation)
	eventCity.changeEventAnger(iChangePopulation)

	eventCity.setNumRealBuilding(gc.getInfoTypeForString("BUILDING_HOSPITAL"), 0)

	eventCity.changePopulation(-iChangePopulation)
	szBuffer = localText.getText("TXT_KEY_EVENT_SUPER_VIRUS_HIT_CITY", (iChangePopulation, eventCity.getNameKey() , ))
	CyInterface().addMessage(kTriggeredData.ePlayer, False, gc.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_PILLAGE", InterfaceMessageTypes.MESSAGE_TYPE_INFO, None, gc.getInfoTypeForString("COLOR_RED"), eventCity.getX(), eventCity.getY(), True, True)

	eventCity.changeFreeSpecialistCount(gc.getInfoTypeForString("SPECIALIST_GREAT_GENERAL"), 1)

def getHelpSuperVirus4(argsList):
	#import BugUtil
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	eventCity = player.getCity(kTriggeredData.iCityId)

	iChangePopulation = eventCity.getPopulation() * 75
	iChangePopulation /= 100

	szHelp = localText.getText("TXT_KEY_EVENT_FREE_SPECIALISTS", (1, gc.getSpecialistInfo(gc.getInfoTypeForString("SPECIALIST_GREAT_GENERAL")).getTextKey(), eventCity.getNameKey() , ))
	szHelp += u"\n"
	szHelp += localText.getText("TXT_KEY_EVENT_LOSE_POPULATION", (iChangePopulation, eventCity.getNameKey() , ))
	szHelp += u"\n"
	szHelp += localText.getText("TXT_KEY_EVENT_CITY_ANARCHY", (iChangePopulation, eventCity.getNameKey() , ))
	szHelp += u"\n"
	szHelp += localText.getText("TXT_KEY_EVENT_CITY_UNREST", (iChangePopulation, eventCity.getNameKey(), ))

	return szHelp


def canDoNewWorldTrigger(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	if (not canTriggerNewEvent(argsList)):
		return False

	#Room on the Map for 3 new cities
	iNeededCities = 3
	pBestPlots = []
	while (iNeededCities > 0):
		map = gc.getMap()
		iBestValue = 0
		pBestPlot = None
		for i in range(map.numPlots()):
			pLoopPlot = map.plotByIndex(i)
			if (pBestPlots.count(pLoopPlot) == 0):
				if (pLoopPlot.isCoastalLand()):
					if (player.canFound(pLoopPlot.getX(), pLoopPlot.getY())):
						if (pLoopPlot.getFoundValue(kTriggeredData.ePlayer) > iBestValue):
							pBestPlot = pLoopPlot
							iBestValue = pLoopPlot.getFoundValue(kTriggeredData.ePlayer)
		if (pBestPlot == None):
			return False
		pBestPlots.append(pBestPlot)
		iNeededCities -= 1

	return True


def triggerNewWorldCities(argsList):
	import BugUtil
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)

	iNeededCities = 0
	if (iEvent == gc.getInfoTypeForString("EVENT_NEW_WORLD_2")):
		iNeededCities = 1
	else:
		iNeededCities = 3

	(loopCity, iter) = player.firstCity(False)

	while(loopCity):
		if (iEvent == gc.getInfoTypeForString("EVENT_NEW_WORLD_2")):
			if (loopCity.getPopulation() > 4):
				loopCity.changePopulation(-1)
		else:
			if (loopCity.getPopulation() > 9):
				loopCity.changePopulation(-2)
			elif (loopCity.getPopulation() > 4):
				loopCity.changePopulation(-1)

		(loopCity, iter) = player.nextCity(iter, False)

	while (iNeededCities > 0):
		map = gc.getMap()
		iBestValue = 0
		pBestPlot = None
		for i in range(map.numPlots()):
			pLoopPlot = map.plotByIndex(i)
			if (pLoopPlot.isCoastalLand()):
				if (player.canFound(pLoopPlot.getX(), pLoopPlot.getY())):
					if (pLoopPlot.getFoundValue(kTriggeredData.ePlayer) > iBestValue):
						pBestPlot = pLoopPlot
						iBestValue = pLoopPlot.getFoundValue(kTriggeredData.ePlayer)

		player.found(pBestPlot.getX(), pBestPlot.getY())

		pNewCity = pBestPlot.getPlotCity()
		if (pNewCity == None):
			BugUtil.debug("Error in TriggerNewWorldCities - No City Created!")
			return

		if (iEvent == gc.getInfoTypeForString("EVENT_NEW_WORLD_2")):
			pNewCity.changePopulation(1)
		else:
			pNewCity.changePopulation(3)

		eStateReligion = player.getStateReligion()
		if (eStateReligion > 0):
			pNewCity.setHasReligion(eStateReligion, True, True, True)

		iNumNeededDefenders = 0
		if (iEvent == gc.getInfoTypeForString("EVENT_NEW_WORLD_2")):
			iNumNeededDefenders = 1
		else:
			iNumNeededDefenders = 2
		eBestUnit = -1
		iBestStrength = 0
		for iI in range(gc.getNumUnitClassInfos()):
			eLoopUnit = gc.getCivilizationInfo(pNewCity.getCivilizationType()).getCivilizationUnits(iI)
			if (eLoopUnit > 0):
				if (gc.getUnitInfo(eLoopUnit).getDomainType() == DomainTypes.DOMAIN_LAND):
					if (pNewCity.canTrain(eLoopUnit, False, False, False, False)):
						if (gc.getUnitInfo(eLoopUnit).getCombat() > iBestStrength):
							eBestUnit = eLoopUnit
							iBestStrength = gc.getUnitInfo(eLoopUnit).getCombat()

		if (eBestUnit > 0):
			while (iNumNeededDefenders > 0):
				player.initUnit(eBestUnit, pNewCity.getX(), pNewCity.getY(), UnitAITypes.UNITAI_CITY_DEFENSE, DirectionTypes.DIRECTION_NORTH)
				iNumNeededDefenders -= 1

		if (iEvent == gc.getInfoTypeForString("EVENT_NEW_WORLD_2")):
			pNewCity.setNumRealBuilding(gc.getCivilizationInfo(pNewCity.getCivilizationType()).getCivilizationBuildings(gc.getInfoTypeForString("BUILDINGCLASS_WALLS")), 1)
			pNewCity.setNumRealBuilding(gc.getCivilizationInfo(pNewCity.getCivilizationType()).getCivilizationBuildings(gc.getInfoTypeForString("BUILDINGCLASS_GRANARY")), 1)
			pNewCity.setNumRealBuilding(gc.getCivilizationInfo(pNewCity.getCivilizationType()).getCivilizationBuildings(gc.getInfoTypeForString("BUILDINGCLASS_FORGE")), 1)
		else:
			pNewCity.setNumRealBuilding(gc.getCivilizationInfo(pNewCity.getCivilizationType()).getCivilizationBuildings(gc.getInfoTypeForString("BUILDINGCLASS_WALLS")), 1)
			pNewCity.setNumRealBuilding(gc.getCivilizationInfo(pNewCity.getCivilizationType()).getCivilizationBuildings(gc.getInfoTypeForString("BUILDINGCLASS_BARRACKS")), 1)
			pNewCity.setNumRealBuilding(gc.getCivilizationInfo(pNewCity.getCivilizationType()).getCivilizationBuildings(gc.getInfoTypeForString("BUILDINGCLASS_GRANARY")), 1)
			pNewCity.setNumRealBuilding(gc.getCivilizationInfo(pNewCity.getCivilizationType()).getCivilizationBuildings(gc.getInfoTypeForString("BUILDINGCLASS_FORGE")), 1)
			pNewCity.setNumRealBuilding(gc.getCivilizationInfo(pNewCity.getCivilizationType()).getCivilizationBuildings(gc.getInfoTypeForString("BUILDINGCLASS_MARKET")), 1)
			if pNewCity.plot().isCoastalLand():
				pNewCity.setNumRealBuilding(gc.getCivilizationInfo(pNewCity.getCivilizationType()).getCivilizationBuildings(gc.getInfoTypeForString("BUILDINGCLASS_HARBOR")), 1)
				pNewCity.setNumRealBuilding(gc.getCivilizationInfo(pNewCity.getCivilizationType()).getCivilizationBuildings(gc.getInfoTypeForString("BUILDINGCLASS_LIGHTHOUSE")), 1)
				pNewCity.setNumRealBuilding(gc.getCivilizationInfo(pNewCity.getCivilizationType()).getCivilizationBuildings(gc.getInfoTypeForString("BUILDINGCLASS_FISHERMAN_HUT")), 1)

		iNeededCities -= 1

def canTriggerNewWorldCities3(argsList):
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	team = gc.getTeam(player.getTeam())

	iNavigation = gc.getInfoTypeForString("TECH_NAVIGATION")
	if (iNavigation > 0):
		if (not team.isHasTech(iNavigation)):
			return False
	return True

def getHelpNewWorld(argsList):
	import BugUtil
	iEvent = argsList[0]
	kTriggeredData = argsList[1]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	team = gc.getTeam(player.getTeam())

	if (iEvent == gc.getInfoTypeForString("EVENT_NEW_WORLD_2")):
		szHelp = BugUtil.getText("TXT_KEY_EVENT_FREE_OUTPOST_CITY")
		szHelp += u"\n"
		szHelp += localText.getText("TXT_KEY_EVENT_EMPIRE_POPULATION_LOSS", (4, 1, ))
	else:
		szHelp = BugUtil.getText("TXT_KEY_EVENT_FREE_OUTPOST_CITIES")
		szHelp += u"\n"
		szHelp += localText.getText("TXT_KEY_EVENT_EMPIRE_POPULATION_LOSS", (9, 2, ))
		szHelp += u"\n"
		szHelp += localText.getText("TXT_KEY_EVENT_EMPIRE_POPULATION_LOSS", (4, 1, ))

		iNavigation = gc.getInfoTypeForString("TECH_NAVIGATION")
		if (iNavigation > 0):
			if (not team.isHasTech(iNavigation)):
				szHelp += u"\n"
				szHelp += BugUtil.getText("TXT_KEY_BUILDING_REQUIRES_STRING", gc.getTechInfo(iNavigation).getTextKey())
	return szHelp

def canTriggerNewEvent(argsList):
	return gc.getGame().isModderGameOption(ModderGameOptionTypes.MODDERGAMEOPTION_STRATEGIC_EVENTS)

def canTriggerStorm(argsList):
	kTriggeredData = argsList[0]
	map = gc.getMap()

	if gc.getGame().isModderGameOption(ModderGameOptionTypes.MODDERGAMEOPTION_NO_STORMS):
		return False
	return True

# Blarney Castle trigger
def canTriggerBlarneyVisit(argsList):
	kTriggeredData = argsList[0]
	game = gc.getGame()
	iGameTurn = game.getGameTurn()
	iEndTurn = game.getMaxTurns()
	if (iEndTurn == 0):
		iEndTurn = game.getEstimateEndTurn()

	iCount = int(iEndTurn * 50 / 1200)

	if (iGameTurn % iCount == 0):
		return True
	return False

# Penicillin trigger
def canTriggerPenicillin(argsList):
	kTriggeredData = argsList[0]
	game = gc.getGame()
	iProjectPen = gc.getInfoTypeForString("PROJECT_PENICILLIN")
	player = gc.getPlayer(kTriggeredData.ePlayer)

	if gc.getTeam(player.getTeam()).getProjectCount(iProjectPen) == 0:
		return False
	return True


def canApplyMeteorStrike(argsList):
	kTriggeredData = argsList[0]

	import BugUtil

	BugUtil.debug("Checking plot (%d, %d) for meteor strike validity", kTriggeredData.iPlotX, kTriggeredData.iPlotY)

	if (not canTriggerNewEvent(argsList)):
		return False
		
	if (gc.getGame().isDarkAge() or gc.getGame().isDarkAgePending()):
		return False

	for iDX in range(-1, 2):
		for iDY in range(-1, 2):
			loopPlot = plotXY(kTriggeredData.iPlotX, kTriggeredData.iPlotY, iDX, iDY)
			if loopPlot.isNone() or loopPlot.isWater() or loopPlot.isCity():
				return False

	return True

def applyMeteorStrike1(argsList):
	iEvent = argsList[0]
	kTriggeredData = argsList[1]

	import BugUtil

	def applyPlotDamage(px, py):
		plot = gc.getMap().plot(px, py)
		ePlotType = plot.getPlotType()
		if (ePlotType == PlotTypes.PLOT_HILLS):
			plot.setPlotType(PlotTypes.PLOT_PEAK, True, True)
		elif (ePlotType == PlotTypes.PLOT_PEAK):
			pass
		else:
			plot.setPlotType(PlotTypes.PLOT_HILLS, True, True)

		#clear resources, improvements, or roads
		plot.setImprovementType(-1)
		plot.setRouteType(-1)
		plot.setBonusType(-1)

	plot = gc.getMap().plot(kTriggeredData.iPlotX, kTriggeredData.iPlotY)
	plot.setTerrainType(gc.getDefineINT("SHALLOW_WATER_TERRAIN"), True, True)
	plot.setImprovementType(-1)
	plot.setRouteType(-1)
	plot.setBonusType(-1)

	for iDX in range(-1, 2):
		for iDY in range(-1, 2):
			loopPlot = plotXY(kTriggeredData.iPlotX, kTriggeredData.iPlotY, iDX, iDY)
			if not loopPlot.isNone() and not (iDX == 0 and iDY == 0):
				applyPlotDamage(loopPlot.getX(), loopPlot.getY())

	iClassical = gc.getInfoTypeForString("ERA_CLASSICAL")
	
	if (not gc.getGame().isNetworkMultiPlayer() and gc.getGame().getHighestEra() > iClassical):

		iBaseOdds = 1000
		iBaseOdds /= (1 + gc.getGame().getNumDarkAgesEverOccurred())
			
		if (gc.getGame().getSorenRandNum(10000, "Meteor Strike Dark Age Chance") < iBaseOdds):
			delay = 4 + gc.getGame().getSorenRandNum(12, "Meteor Strike Dark Age Delay")
			lenTurns = 20 + gc.getGame().getSorenRandNum(30, "Meteor Strike Dark Age Delay")

			BugUtil.debug("unscaled delay: %d, unscaled lenTurns: %d", delay, lenTurns)

			delay *= gc.getGameSpeedInfo(gc.getGame().getGameSpeedType()).getGrowthPercent()
			delay /= 100

			lenTurns *= gc.getGameSpeedInfo(gc.getGame().getGameSpeedType()).getGrowthPercent()
			lenTurns /= 100

			if (lenTurns > 75): lenTurns = 75
			if (delay > 20): delay = 20

			BugUtil.debug("scaled delay: %d, scaled lenTurns: %d", delay, lenTurns)

			gc.getGame().triggerDarkAge(delay, lenTurns, "TXT_KEY_EVENT_DARK_AGE_METEOR_BEGINS", "TXT_KEY_EVENT_DARK_AGE_METEOR_ENDS")

	return True

# National Weather Service cancels negative weather events

def canTriggerNWS(argsList):
	kTriggeredData = argsList[0]
	player = gc.getPlayer(kTriggeredData.ePlayer)
	iNatWeather = gc.getInfoTypeForString("BUILDINGCLASS_NWS")
	if player.getBuildingClassCountWithUpgrades(iNatWeather) > 0:
		return False
	return True
