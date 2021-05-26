# Rise of Mankind EventManager
# by Zappara

from CvPythonExtensions import *
import OOSLogger
#import autolog

gc = CyGlobalContext()
localText = CyTranslator()

SD_MOD_ID = "RiseOfMankind"

g_modEventManager = None
g_eventMgr = None

class RoMEventManager:
	def __init__(self, eventManager):
		eventManager.addEventHandler("OnLoad", self.onLoadGame)
		eventManager.addEventHandler("GameStart", self.onGameStart)
		eventManager.addEventHandler("buildingBuilt", self.onBuildingBuilt)
		eventManager.addEventHandler("projectBuilt", self.onProjectBuilt)
		eventManager.addEventHandler("unitBuilt", self.onUnitBuilt)
		eventManager.addEventHandler("cityRazed", self.onCityRazed)
		eventManager.addEventHandler("cityLost", self.onCityLost)
		eventManager.addEventHandler("gameUpdate", self.onGameUpdate)

		global g_modEventManager
		g_modEventManager = self

		global g_eventMgr
		g_eventMgr = eventManager
		self.eventManager = eventManager

		# RoM start Next War tracks cities that have been razed
		self.iArcologyCityID = -1

		self.iTECH_KNOWLEDGE_MANAGEMENT = -1
		self.iTECH_APPLIED_ECONOMICS = -1
		self.iPROJECT_EDEN = -1
		self.iBUILDING_NANITE_DEFUSER = -1


	def addCityBuildings(self, pCity, szBuilding):
		iBuilding = gc.getInfoTypeForString(szBuilding)
		iUniqueBuilding = gc.getCivilizationInfo(gc.getPlayer(pCity.getOwner()).getCivilizationType()).getCivilizationBuildings(iBuilding)
		if iUniqueBuilding > -1:
			if True or pCity.canConstruct(iUniqueBuilding, False, False, False):
				pCity.setNumRealBuilding(iUniqueBuilding, 1)

	def onLoadGame(self, argsList):
		self.iTECH_KNOWLEDGE_MANAGEMENT = gc.getInfoTypeForString("TECH_KNOWLEDGE_MANAGEMENT")
		self.iTECH_APPLIED_ECONOMICS = gc.getInfoTypeForString("TECH_APPLIED_ECONOMICS")
		self.iPROJECT_EDEN = gc.getInfoTypeForString("PROJECT_EDEN")
		self.iBUILDING_NANITE_DEFUSER = gc.getInfoTypeForString("BUILDING_NANITE_DEFUSER")

	def onGameStart(self, argsList):
		'Called at the start of the game'
		# def onBuildingBuilt / Additional tests variable
		self.iTECH_KNOWLEDGE_MANAGEMENT = gc.getInfoTypeForString("TECH_KNOWLEDGE_MANAGEMENT")
		self.iTECH_APPLIED_ECONOMICS = gc.getInfoTypeForString("TECH_APPLIED_ECONOMICS")
		self.iPROJECT_EDEN = gc.getInfoTypeForString('PROJECT_EDEN')
		self.iBUILDING_NANITE_DEFUSER = gc.getInfoTypeForString("BUILDING_NANITE_DEFUSER")

	def onBuildingBuilt(self, argsList):
		'Building Completed'
		pCity, iBuildingType = argsList

		# NANITE DEFUSER - destroyes all nukes from all players
		if iBuildingType == self.iBUILDING_NANITE_DEFUSER:
			import PlayerUtil
			pPlayer = gc.getPlayer(pCity.plot().getOwner())
			iX = pCity.getX()
			iY = pCity.getY()
			for player in PlayerUtil.players(alive=True):
				pPID = player.getID()
				for unit in PlayerUtil.playerUnits(player):
					if (unit.nukeRange() > -1 or unit.getUnitAIType() == UnitAITypes.UNITAI_ICBM and not unit.isNone()):
						unit.kill( 0, -1 )
				CyInterface().addMessage(pPID,False,15,localText.getText("TXT_KEY_NANITE_DEFUSER_PYTHON",()),'',0,'Art/Interface/Buttons/Buildings/Ascension_Gate.dds',ColorTypes(44), iX, iY, True,True)


	def onProjectBuilt(self, argsList):
		'Project Completed'
		pCity, iProjectType = argsList
		game = gc.getGame()

		# Eden project
		if iProjectType == self.iPROJECT_EDEN:
			pPlayer = gc.getPlayer(pCity.plot().getOwner())
			pPID = pPlayer.getID()
			pTID = pPlayer.getTeam()
			iX = pCity.getX()
			iY = pCity.getY()
			tt_desert = gc.getInfoTypeForString( 'TERRAIN_DESERT' )
			tt_plain = gc.getInfoTypeForString( 'TERRAIN_PLAINS' )
			tt_grass = gc.getInfoTypeForString( 'TERRAIN_GRASS' )
			tt_tundra = gc.getInfoTypeForString( 'TERRAIN_TUNDRA' )
			tt_snow = gc.getInfoTypeForString( 'TERRAIN_SNOW' )
			tt_ocean = gc.getInfoTypeForString( 'TERRAIN_OCEAN' )
			tt_marsh = gc.getInfoTypeForString( 'TERRAIN_MARSH' )
			tt_coast = gc.getInfoTypeForString( "TERRAIN_COAST" )

			for iXLoop in range(iX - 50, iX + 50, 1):
				for iYLoop in range(iY - 50, iY + 50, 1):
					pPlot = CyMap().plot(iXLoop, iYLoop)
					#if ( pPlot.isPlayerCityRadius(pPID)==True ):
					if ( pPlot.getTeam()==pTID ):
						if ( pPlot.getTerrainType()==tt_grass ):
							if ( pPlot.getImprovementType()!=gc.getInfoTypeForString( 'IMPROVEMENT_FARM' )) and ( pPlot.getImprovementType()!=gc.getInfoTypeForString( 'IMPROVEMENT_DOME_FARM' )) and ( pPlot.getImprovementType()!=gc.getInfoTypeForString( 'IMPROVEMENT_WINDMILL' )) and ( pPlot.getImprovementType()!=gc.getInfoTypeForString( 'IMPROVEMENT_MINE' )) and ( pPlot.getImprovementType()!=gc.getInfoTypeForString( 'IMPROVEMENT_SHAFT_MINE' )) and ( pPlot.getImprovementType()!=gc.getInfoTypeForString( 'IMPROVEMENT_MODERN_MINE' )) and ( pPlot.getImprovementType()!=gc.getInfoTypeForString( 'IMPROVEMENT_QUARRY' )) and ( pPlot.getImprovementType()!=gc.getInfoTypeForString( 'IMPROVEMENT_WORKSHOP' )) and ( pPlot.getImprovementType()!=gc.getInfoTypeForString( 'IMPROVEMENT_3FACTORY' )) and ( pPlot.getImprovementType()!=gc.getInfoTypeForString( 'IMPROVEMENT_5FACTORY' )) and ( pPlot.getImprovementType()!=gc.getInfoTypeForString( 'IMPROVEMENT_PLANTATION' )) and ( pPlot.getImprovementType()!=gc.getInfoTypeForString( 'IMPROVEMENT_WINERY' )) and ( pPlot.getImprovementType()!=gc.getInfoTypeForString( 'IMPROVEMENT_COTTAGE' )) and ( pPlot.getImprovementType()!=gc.getInfoTypeForString( 'IMPROVEMENT_HAMLET' )) and ( pPlot.getImprovementType()!=gc.getInfoTypeForString( 'IMPROVEMENT_VILLAGE' )) and ( pPlot.getImprovementType()!=gc.getInfoTypeForString( 'IMPROVEMENT_TOWN' )):
								if ( pPlot.getFeatureType()!=gc.getInfoTypeForString( 'FEATURE_JUNGLE' )):
									pPlot.setFeatureType(gc.getInfoTypeForString( "FEATURE_FOREST" ), 1)
						elif ( pPlot.getTerrainType()==tt_plain ):
							pPlot.setTerrainType(tt_grass, 1, 1)
						elif ( pPlot.getTerrainType()==tt_marsh ):
							pPlot.setTerrainType(tt_grass, 1, 1)
						elif ( pPlot.getTerrainType()==tt_tundra ):
							pPlot.setTerrainType(tt_plain, 1, 1)
						elif ( pPlot.getTerrainType()==tt_snow ):
							pPlot.setTerrainType(tt_tundra, 1, 1)
						elif ( pPlot.getTerrainType()==tt_ocean ):
							pPlot.setTerrainType(tt_coast, 1, 1)
						elif ( pPlot.getTerrainType()==tt_desert ):
							pPlot.setTerrainType(tt_plain, 1, 1)

			CyInterface().addMessage(pPID,False,15,localText.getText("TXT_KEY_EDEN_PYTHON",()),'',0,'Art/Interface/Buttons/Buildings/Eden.dds',ColorTypes(44), iX, iY, True,True)

	def onUnitBuilt(self, argsList):
		'Unit Completed'
		city = argsList[0]
		unit = argsList[1]

		# if player has Technocracy civic active, give free promotion to unit
		player = gc.getPlayer(city.getOwner())
		if gc.getTeam(player.getTeam()).isHasTech(self.iTECH_KNOWLEDGE_MANAGEMENT):
			iTechnocracyParentCivicOption = gc.getInfoTypeForString("CIVICOPTION_POWER")
			iTechnocracy = gc.getInfoTypeForString("CIVIC_TECHNOCRACY")

			if -1 not in (iTechnocracyParentCivicOption, iTechnocracy) and player.getCivics(iTechnocracyParentCivicOption) == iTechnocracy and gc.getTeam(player.getTeam()).isHasTech(gc.getInfoTypeForString("TECH_SMART_DUST")):
				iSensors = gc.getInfoTypeForString("PROMOTION_SENSORS")
				if iSensors > -1 and unit.isPromotionValid(iSensors):
					unit.setHasPromotion(iSensors, True)


	def onCityRazed(self, argsList):
		'City Razed'
		city, iPlayer = argsList

		self.iArcologyCityID = -1

		if city.getNumRealBuilding(gc.getInfoTypeForString("BUILDING_ARCOLOGY")) or city.getNumRealBuilding(gc.getInfoTypeForString("BUILDING_ARCOLOGY_SHIELDING")) or city.getNumRealBuilding(gc.getInfoTypeForString("BUILDING_ADVANCED_SHIELDING")):
			self.iArcologyCityID = city.getID()


	def onCityLost(self, argsList):
		'City Lost'
		city = argsList[0]

		if city.getID() == self.iArcologyCityID:
			city.plot().setImprovementType(gc.getInfoTypeForString("IMPROVEMENT_CITY_RUINS_ARCOLOGY"))

	def onGameUpdate(self, argsList):
		'sample generic event, called on each game turn slice'
		genericArgs = argsList[0][0]	# tuple of tuple of my args
		OOSLogger.doGameUpdate()