# Rise of Mankind EventManager
# by Zappara

from CvPythonExtensions import *
import CvUtil
import PyHelpers
import BugUtil
import OOSLogger
import PlayerUtil
#import autolog

# BUG - Mac Support - start
BugUtil.fixSets(globals())
# BUG - Mac Support - end

gc = CyGlobalContext()
localText = CyTranslator()
PyPlayer = PyHelpers.PyPlayer

SD_MOD_ID = "RiseOfMankind"

g_modEventManager = None
g_eventMgr = None

class RoMEventManager:
	def __init__(self, eventManager):
		eventManager.addEventHandler("ModNetMessage", self.onModNetMessage)
		eventManager.addEventHandler("OnLoad", self.onLoadGame)
		eventManager.addEventHandler("GameStart", self.onGameStart)
		eventManager.addEventHandler("buildingBuilt", self.onBuildingBuilt)
		eventManager.addEventHandler("projectBuilt", self.onProjectBuilt)
		eventManager.addEventHandler("unitBuilt", self.onUnitBuilt)
		eventManager.addEventHandler("cityBuilt", self.onCityBuilt)
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

		self.iBUILDING_DJENNE = -1
		self.iBUILDING_WORLD_BANK = -1
		self.iTECH_KNOWLEDGE_MANAGEMENT = -1
		self.iTECH_APPLIED_ECONOMICS = -1
		self.m_iNetMessage_Colonist = 410
		self.m_iNetMessage_Pioneer = 411
		self.iPROJECT_EDEN = -1
		self.iBUILDING_NANITE_DEFUSER = -1

		BugUtil.debug("Initializing RoMEventManager")

	def onModNetMessage(self, argsList):
		'Called whenever CyMessageControl().sendModNetMessage() is called - this is all for you modders!'

		iData1, iData2, iData3, iData4, iData5 = argsList

		# Rise of Mankind start
		iMessageID = iData1

		BugUtil.debug("on Mod Net Message: %d", iData1)

# Rise of Mankind 2.71
		# NetModMessage 410
		# SettlersEventManager.py / Colonist
		# Add additional buildings and change city size
		if ( iMessageID == self.m_iNetMessage_Colonist ):
			iPlotX = iData2
			iPlotY = iData3
			iOwner = iData4
			iUnitID = iData5

			pPlot = CyMap( ).plot( iPlotX, iPlotY )
			pCity = pPlot.getPlotCity( )
			pPlayer = gc.getPlayer( iOwner )

			pCity.setPopulation(3)
			self.addCityBuildings(pCity, "BUILDINGCLASS_BARRACKS")
			self.addCityBuildings(pCity, "BUILDINGCLASS_GRANARY")
			self.addCityBuildings(pCity, "BUILDINGCLASS_FORGE")
			self.addCityBuildings(pCity, "BUILDINGCLASS_MARKET")
			if pCity.plot().isCoastalLand():
				self.addCityBuildings(pCity, "BUILDINGCLASS_HARBOR")
				self.addCityBuildings(pCity, "BUILDINGCLASS_LIGHTHOUSE")
				self.addCityBuildings(pCity, "BUILDINGCLASS_FISHERMAN_HUT")

		# NetModMessage 411
		# SettlersEventManager.py / Pioneer
		# Add additional buildings and change city size
		if ( iMessageID == self.m_iNetMessage_Pioneer ):
			iPlotX = iData2
			iPlotY = iData3
			iOwner = iData4
			iUnitID = iData5

			pPlot = CyMap( ).plot( iPlotX, iPlotY )
			pCity = pPlot.getPlotCity( )
			pPlayer = gc.getPlayer( iOwner )

			pCity.setPopulation(4)
			self.addCityBuildings(pCity, "BUILDINGCLASS_GARRISON")
			self.addCityBuildings(pCity, "BUILDINGCLASS_GRANARY")
			self.addCityBuildings(pCity, "BUILDINGCLASS_FORGE")
			self.addCityBuildings(pCity, "BUILDINGCLASS_COURTHOUSE")
			self.addCityBuildings(pCity, "BUILDINGCLASS_MARKET")
			self.addCityBuildings(pCity, "BUILDINGCLASS_STABLE")
			self.addCityBuildings(pCity, "BUILDINGCLASS_GROCER")
			self.addCityBuildings(pCity, "BUILDINGCLASS_DOCTOR")
			self.addCityBuildings(pCity, "BUILDINGCLASS_BANK")
			self.addCityBuildings(pCity, "BUILDINGCLASS_LIBRARY")
			if pCity.plot().isCoastalLand():
				self.addCityBuildings(pCity, "BUILDINGCLASS_PORT")
				self.addCityBuildings(pCity, "BUILDINGCLASS_LIGHTHOUSE")
				self.addCityBuildings(pCity, "BUILDINGCLASS_FISHERMAN_HUT")

	def addCityBuildings(self, pCity, szBuilding):
		iBuilding = gc.getInfoTypeForString(szBuilding)
		iUniqueBuilding = gc.getCivilizationInfo(gc.getPlayer(pCity.getOwner()).getCivilizationType()).getCivilizationBuildings(iBuilding)
		if iUniqueBuilding > -1:
			if True or pCity.canConstruct(iUniqueBuilding, False, False, False):
				pCity.setNumRealBuilding(iUniqueBuilding, 1)

	def onLoadGame(self, argsList):
		self.iBUILDING_DJENNE = gc.getInfoTypeForString("BUILDING_DJENNE")
		self.iBUILDING_ICE_HOTEL = gc.getInfoTypeForString("BUILDING_ICE_HOTEL")
		self.iBUILDING_WORLD_BANK = gc.getInfoTypeForString("BUILDING_WORLD_BANK")
		self.iTECH_KNOWLEDGE_MANAGEMENT = gc.getInfoTypeForString("TECH_KNOWLEDGE_MANAGEMENT")
		self.iTECH_APPLIED_ECONOMICS = gc.getInfoTypeForString("TECH_APPLIED_ECONOMICS")
		self.iPROJECT_EDEN = gc.getInfoTypeForString("PROJECT_EDEN")
		self.iBUILDING_NANITE_DEFUSER = gc.getInfoTypeForString("BUILDING_NANITE_DEFUSER")

	def onGameStart(self, argsList):
		'Called at the start of the game'
		# def onBuildingBuilt / Additional tests variable
		self.iBUILDING_DJENNE = gc.getInfoTypeForString("BUILDING_DJENNE")
		self.iBUILDING_ICE_HOTEL = gc.getInfoTypeForString("BUILDING_ICE_HOTEL")
		self.iBUILDING_WORLD_BANK = gc.getInfoTypeForString("BUILDING_WORLD_BANK")
		self.iTECH_KNOWLEDGE_MANAGEMENT = gc.getInfoTypeForString("TECH_KNOWLEDGE_MANAGEMENT")
		self.iTECH_APPLIED_ECONOMICS = gc.getInfoTypeForString("TECH_APPLIED_ECONOMICS")
		self.iPROJECT_EDEN = gc.getInfoTypeForString('PROJECT_EDEN')
		self.iBUILDING_NANITE_DEFUSER = gc.getInfoTypeForString("BUILDING_NANITE_DEFUSER")

	def onBuildingBuilt(self, argsList):
		'Building Completed'
		pCity, iBuildingType = argsList
		game = gc.getGame()

		# Djenne
		if (iBuildingType == self.iBUILDING_DJENNE):
			pPlayer = gc.getPlayer(pCity.plot().getOwner())
			pPID = pPlayer.getID()
			pTID = pPlayer.getTeam()
			iX = pCity.getX()
			iY = pCity.getY()
			tt_desert = gc.getInfoTypeForString( 'TERRAIN_DESERT' )

			for iXLoop in range(iX - 2, iX + 3, 1):
				for iYLoop in range(iY - 2, iY + 3, 1):
					pPlot = CyMap().plot(iXLoop, iYLoop)
					if ( pPlot.isPlayerCityRadius(pPID)==True ):
						if ( pPlot.getTeam()==pTID ):
							if ( pPlot.getTerrainType()==tt_desert ):
								CyGame().setPlotExtraYield(iXLoop, iYLoop, YieldTypes.YIELD_COMMERCE, 2)

			CyInterface().addMessage(pPID,False,15,localText.getText("TXT_KEY_DJENNE_PYTHON",()),'',0,',Art/Interface/Buttons/Empty.dds,Art/Interface/Buttons/Atlases/and_wonders_atlas.dds,6,2',ColorTypes(44), iX, iY, True,True)

		# SnowCastle of Kemi
		if (iBuildingType == self.iBUILDING_ICE_HOTEL):
			pPlayer = gc.getPlayer(pCity.plot().getOwner())
			pPID = pPlayer.getID()
			pTID = pPlayer.getTeam()
			iX = pCity.getX()
			iY = pCity.getY()
			tt_tundra = gc.getInfoTypeForString( 'TERRAIN_TUNDRA' )
			tt_ice = gc.getInfoTypeForString( 'TERRAIN_SNOW' )

			for iXLoop in range(iX - 2, iX + 3, 1):
				for iYLoop in range(iY - 2, iY + 3, 1):
					pPlot = CyMap().plot(iXLoop, iYLoop)
					if ( pPlot.isPlayerCityRadius(pPID)==True ):
						if ( pPlot.getTeam()==pTID ):
							if ( pPlot.getTerrainType()==tt_tundra or pPlot.getTerrainType()==tt_ice ):
								CyGame().setPlotExtraYield(iXLoop, iYLoop, YieldTypes.YIELD_COMMERCE, 2)

			CyInterface().addMessage(pPID,False,15,localText.getText("TXT_KEY_ICE_HOTEL_PYTHON",()),'',0,',Art/Interface/Buttons/Empty.dds,Art/Interface/Buttons/Atlases/Afforess_Atlas.dds,4,7',ColorTypes(44), iX, iY, True,True)



		# world bank national wonder commented out by Vokarya; too much gold
		#if iBuildingType == self.iBUILDING_WORLD_BANK:

			#pPlayer = gc.getPlayer(pCity.plot().getOwner())
			#pPID = pPlayer.getID()
			#iGold = pPlayer.getGold( )
			#pPlayer.changeGold( iGold//2 )

		# NANITE DEFUSER - destroyes all nukes from all players
		if (iBuildingType == self.iBUILDING_NANITE_DEFUSER):
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
		player = PyPlayer(city.getOwner())

		# if player has Technocracy civic active, give free promotion to unit
		iPlayer = gc.getPlayer(city.getOwner())
		if gc.getTeam(iPlayer.getTeam()).isHasTech(self.iTECH_KNOWLEDGE_MANAGEMENT):
			iTechnocracyParentCivicOption = CvUtil.findInfoTypeNum(gc.getCivicOptionInfo,gc.getNumCivicOptionInfos(),"CIVICOPTION_POWER")
			iTechnocracy = CvUtil.findInfoTypeNum(gc.getCivicInfo,gc.getNumCivicInfos(),"CIVIC_TECHNOCRACY")

			if ( (iTechnocracyParentCivicOption != -1) and (iTechnocracy != -1) ):
				iCivic = iPlayer.getCivics(iTechnocracyParentCivicOption)

				if (iCivic == iTechnocracy):
					if ( gc.getTeam(iPlayer.getTeam()).isHasTech(gc.getInfoTypeForString("TECH_SMART_DUST")) == True ):
						iSensors =  CvUtil.findInfoTypeNum(gc.getPromotionInfo, gc.getNumPromotionInfos(),'PROMOTION_SENSORS')
						if (unit.isPromotionValid(iSensors)):
							unit.setHasPromotion(iSensors, True)



	def onCityBuilt(self, argsList):
		'City Built'
		city = argsList[0]
		iUnitID = argsList[1]

		BugUtil.info("City built by unit %d", iUnitID)

		if iUnitID > -1:
			pUnit = gc.getPlayer(city.getOwner()).getUnit(iUnitID)
			if pUnit:
				if pUnit.getUnitClassType() == gc.getInfoTypeForString("UNITCLASS_COLONIST"):
					pPlot = CyMap( ).plot( pUnit.getX( ), pUnit.getY( ) )
					iMessageID = 410
					BugUtil.info("RoM - Colonist messageID: %d", iMessageID)
					iPlotX = pPlot.getX()
					iPlotY = pPlot.getY()
					iOwner = pUnit.getOwner()
					CyMessageControl( ).sendModNetMessage( iMessageID, iPlotX, iPlotY, iOwner, iUnitID )
				elif pUnit.getUnitClassType() == gc.getInfoTypeForString("UNITCLASS_PIONEER"):
					pPlot = CyMap( ).plot( pUnit.getX( ), pUnit.getY( ) )
					iMessageID = 411
					BugUtil.info("RoM - Pioneer messageID: %d", iMessageID)
					iPlotX = pPlot.getX()
					iPlotY = pPlot.getY()
					iOwner = pUnit.getOwner()
					CyMessageControl( ).sendModNetMessage( iMessageID, iPlotX, iPlotY, iOwner, iUnitID )


	def onCityRazed(self, argsList):
		'City Razed'
		city, iPlayer = argsList

		self.iArcologyCityID = -1

		if city.getNumRealBuilding(gc.getInfoTypeForString("BUILDING_ARCOLOGY")) or city.getNumRealBuilding(gc.getInfoTypeForString("BUILDING_ARCOLOGY_SHIELDING")) or city.getNumRealBuilding(gc.getInfoTypeForString("BUILDING_ADVANCED_SHIELDING")):
			self.iArcologyCityID = city.getID()


	def onCityLost(self, argsList):
		'City Lost'
		city = argsList[0]
		player = PyPlayer(city.getOwner())

		if city.getID() == self.iArcologyCityID:
			city.plot().setImprovementType(gc.getInfoTypeForString("IMPROVEMENT_CITY_RUINS_ARCOLOGY"))

	def onGameUpdate(self, argsList):
		'sample generic event, called on each game turn slice'
		genericArgs = argsList[0][0]	# tuple of tuple of my args
		OOSLogger.doGameUpdate()