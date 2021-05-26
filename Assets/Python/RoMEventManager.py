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
		eventManager.addEventHandler("cityRazed", self.onCityRazed)
		eventManager.addEventHandler("cityLost", self.onCityLost)
		eventManager.addEventHandler("gameUpdate", self.onGameUpdate)

		global g_modEventManager
		g_modEventManager = self

		global g_eventMgr
		g_eventMgr = eventManager
		self.eventManager = eventManager

		self.bArcology = False

		self.iPROJECT_EDEN = -1
		self.iBUILDING_NANITE_DEFUSER = -1


	def addCityBuildings(self, pCity, szBuilding):
		iBuilding = gc.getInfoTypeForString(szBuilding)
		iUniqueBuilding = gc.getCivilizationInfo(gc.getPlayer(pCity.getOwner()).getCivilizationType()).getCivilizationBuildings(iBuilding)
		if iUniqueBuilding > -1:
			if True or pCity.canConstruct(iUniqueBuilding, False, False, False):
				pCity.setNumRealBuilding(iUniqueBuilding, 1)

	def onLoadGame(self, argsList):
		self.iPROJECT_EDEN = gc.getInfoTypeForString("PROJECT_EDEN")
		self.iBUILDING_NANITE_DEFUSER = gc.getInfoTypeForString("BUILDING_NANITE_DEFUSER")

	def onGameStart(self, argsList):
		self.iPROJECT_EDEN = gc.getInfoTypeForString('PROJECT_EDEN')
		self.iBUILDING_NANITE_DEFUSER = gc.getInfoTypeForString("BUILDING_NANITE_DEFUSER")

	def onBuildingBuilt(self, argsList):
		'Building Completed'
		pCity, iBuildingType = argsList

		# NANITE DEFUSER - destroyes all nukes from all players
		if iBuildingType == self.iBUILDING_NANITE_DEFUSER:
			import PlayerUtil
			iX = pCity.getX()
			iY = pCity.getY()
			for player in PlayerUtil.players(alive=True):
				for unit in PlayerUtil.playerUnits(player):
					if (unit.nukeRange() > -1 or unit.getUnitAIType() == UnitAITypes.UNITAI_ICBM and not unit.isNone()):
						unit.kill( 0, -1 )
				CyInterface().addMessage(player.getID(),False,15,localText.getText("TXT_KEY_NANITE_DEFUSER_PYTHON",()),'',0,'Art/Interface/Buttons/Buildings/Ascension_Gate.dds',ColorTypes(44), iX, iY, True,True)


	def onProjectBuilt(self, argsList):
		pCity, iProjectType = argsList

		# Eden project
		if iProjectType == self.iPROJECT_EDEN:
			pPlayer = gc.getPlayer(pCity.plot().getOwner())
			iX = pCity.getX()
			iY = pCity.getY()
			tt_desert = gc.getInfoTypeForString( 'TERRAIN_DESERT' )
			tt_plain = gc.getInfoTypeForString( 'TERRAIN_PLAINS' )
			tt_grass = gc.getInfoTypeForString( 'TERRAIN_GRASS' )
			tt_tundra = gc.getInfoTypeForString( 'TERRAIN_TUNDRA' )
			tt_snow = gc.getInfoTypeForString( 'TERRAIN_SNOW' )
			tt_marsh = gc.getInfoTypeForString( 'TERRAIN_MARSH' )
			iForest = gc.getInfoTypeForString("FEATURE_FOREST")

			for iXLoop in range(iX - 50, iX + 50, 1):
				for iYLoop in range(iY - 50, iY + 50, 1):
					pPlot = CyMap().plot(iXLoop, iYLoop)
					if pPlot.getTerrainType()==tt_grass and pPlot.getFeatureType() == -1:
						iImp = pPlot.getImprovementType()
						if iImp == -1 or iImp not in (gc.getInfoTypeForString('IMPROVEMENT_FARM'), gc.getInfoTypeForString('IMPROVEMENT_DOME_FARM'), gc.getInfoTypeForString('IMPROVEMENT_WINDMILL'), gc.getInfoTypeForString('IMPROVEMENT_MINE'), gc.getInfoTypeForString('IMPROVEMENT_SHAFT_MINE'), gc.getInfoTypeForString('IMPROVEMENT_MODERN_MINE' ), gc.getInfoTypeForString('IMPROVEMENT_QUARRY'), gc.getInfoTypeForString('IMPROVEMENT_WORKSHOP'), gc.getInfoTypeForString('IMPROVEMENT_3FACTORY'), gc.getInfoTypeForString('IMPROVEMENT_5FACTORY'), gc.getInfoTypeForString('IMPROVEMENT_PLANTATION'), gc.getInfoTypeForString('IMPROVEMENT_WINERY'), gc.getInfoTypeForString('IMPROVEMENT_COTTAGE'), gc.getInfoTypeForString('IMPROVEMENT_HAMLET'), gc.getInfoTypeForString('IMPROVEMENT_VILLAGE'), gc.getInfoTypeForString('IMPROVEMENT_TOWN')):
							pPlot.setFeatureType(iForest, 1)
					elif pPlot.getTerrainType() == tt_plain:
						pPlot.setTerrainType(tt_grass, 1, 1)
					elif pPlot.getTerrainType() == tt_marsh:
						pPlot.setTerrainType(tt_grass, 1, 1)
					elif pPlot.getTerrainType() == tt_tundra:
						pPlot.setTerrainType(tt_plain, 1, 1)
					elif pPlot.getTerrainType() == tt_snow:
						pPlot.setTerrainType(tt_tundra, 1, 1)
					elif pPlot.getTerrainType() == tt_desert:
						pPlot.setTerrainType(tt_plain, 1, 1)

			CyInterface().addMessage(pPlayer.getID(),False,15,localText.getText("TXT_KEY_EDEN_PYTHON",()),'',0,'Art/Interface/Buttons/Buildings/Eden.dds',ColorTypes(44), iX, iY, True,True)


	def onCityRazed(self, argsList):
		city = argsList[0]
		if city.getNumRealBuilding(gc.getInfoTypeForString("BUILDING_ARCOLOGY")) or city.getNumRealBuilding(gc.getInfoTypeForString("BUILDING_ARCOLOGY_SHIELDING")) or city.getNumRealBuilding(gc.getInfoTypeForString("BUILDING_ADVANCED_SHIELDING")):
			self.bArcology = True
		else: self.bArcology = False

	def onCityLost(self, argsList):
		if self.bArcology:
			argsList[0].plot().setImprovementType(gc.getInfoTypeForString("IMPROVEMENT_CITY_RUINS_ARCOLOGY"))
			self.bArcology = False

	def onGameUpdate(self, argsList):
		genericArgs = argsList[0][0]
		OOSLogger.doGameUpdate()