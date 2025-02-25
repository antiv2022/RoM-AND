# Sid Meier's Civilization 4
# Copyright Firaxis Games 2005

#
# Sevopedia 2.3
#   sevotastic.blogspot.com
#   sevotastic@yahoo.com
#
# additional work by Gaurav, Progor, Ket, Vovan, Fitchn, LunarMongoose
# see ReadMe for details
#
from CvPythonExtensions import *

class SevoPediaCivilization:

	def __init__(self, main):
		self.top = main

		self.X_MAIN_PANE = self.top.X_PEDIA_PAGE
		self.Y_MAIN_PANE = self.top.Y_PEDIA_PAGE

		self.Y_TECH = self.Y_MAIN_PANE
		self.H_TECH = 110

		self.Y_LEADER = self.Y_TECH + self.H_TECH + 10
		self.H_LEADER = 110

		self.H_MAIN_PANE = self.Y_LEADER + self.H_LEADER - self.Y_MAIN_PANE
		self.W_MAIN_PANE = self.H_MAIN_PANE

		self.X_TECH = self.X_MAIN_PANE + self.W_MAIN_PANE + 10
		self.W_TECH = self.top.R_PEDIA_PAGE - self.X_TECH

		self.X_LEADER = self.X_TECH
		self.W_LEADER = self.W_TECH

		self.W_ICON = 150
		self.H_ICON = 150
		self.X_ICON = self.X_MAIN_PANE + (self.H_MAIN_PANE - self.H_ICON) / 2
		self.Y_ICON = self.Y_MAIN_PANE + (self.H_MAIN_PANE - self.H_ICON) / 2
		self.ICON_SIZE = 64

		self.X_BUILDING = self.X_MAIN_PANE
		self.Y_BUILDING = self.Y_LEADER + self.H_LEADER + 10
		self.W_BUILDING = self.W_MAIN_PANE
		self.H_BUILDING = 110

		self.X_UNIT = self.X_BUILDING + self.W_BUILDING + 10
		self.Y_UNIT = self.Y_BUILDING
		self.W_UNIT = self.top.R_PEDIA_PAGE - self.X_UNIT
		self.H_UNIT = 110

		self.X_TEXT = self.X_MAIN_PANE
		self.Y_TEXT = self.Y_BUILDING + self.H_BUILDING + 10
		self.W_TEXT = self.top.R_PEDIA_PAGE - self.X_TEXT
		self.H_TEXT = self.top.B_PEDIA_PAGE - self.Y_TEXT



	def interfaceScreen(self, iCivilization):
		self.iCivilization = iCivilization
		gc = CyGlobalContext()
		localText = CyTranslator()
		ePnlBlue50 = PanelStyles.PANEL_STYLE_BLUE50

		screen = self.top.getScreen()
		screen.addPanel(self.top.getNextWidgetName(), "", "", False, False, self.X_MAIN_PANE, self.Y_MAIN_PANE, self.W_MAIN_PANE, self.H_MAIN_PANE, ePnlBlue50)
		screen.addPanel(self.top.getNextWidgetName(), "", "", False, False, self.X_ICON, self.Y_ICON, self.W_ICON, self.H_ICON, PanelStyles.PANEL_STYLE_MAIN)
		screen.addDDSGFC(self.top.getNextWidgetName(), CyArtFileMgr().getCivilizationArtInfo(gc.getCivilizationInfo(self.iCivilization).getArtDefineTag()).getButton(), self.X_ICON + self.W_ICON/2 - self.ICON_SIZE/2, self.Y_ICON + self.H_ICON/2 - self.ICON_SIZE/2, self.ICON_SIZE, self.ICON_SIZE, WidgetTypes.WIDGET_GENERAL, -1, -1)

		# place techs
		pnl = self.top.getNextWidgetName()
		screen.addPanel(pnl, localText.getText("TXT_KEY_FREE_TECHS", ()), "", False, True, self.X_TECH, self.Y_TECH, self.W_TECH, self.H_TECH, ePnlBlue50)
		screen.attachLabel(pnl, "", "  ")
		for iTech in range(gc.getNumTechInfos()):
			if (gc.getCivilizationInfo(self.iCivilization).isCivilizationFreeTechs(iTech)):
				screen.attachImageButton(pnl, "", gc.getTechInfo(iTech).getButton(), GenericButtonSizes.BUTTON_SIZE_CUSTOM, WidgetTypes.WIDGET_PEDIA_JUMP_TO_TECH, iTech, 1, False)

		# place buildings
		pnl = self.top.getNextWidgetName()
		screen.addPanel(pnl, localText.getText("TXT_KEY_UNIQUE_BUILDINGS", ()), "", False, True, self.X_BUILDING, self.Y_BUILDING, self.W_BUILDING, self.H_BUILDING, ePnlBlue50)
		screen.attachLabel(pnl, "", "  ")
		for iBuilding in range(gc.getNumBuildingClassInfos()):
			iUniqueBuilding = gc.getCivilizationInfo(self.iCivilization).getCivilizationBuildings(iBuilding)
			if iUniqueBuilding < 0: continue
			iDefaultBuilding = gc.getBuildingClassInfo(iBuilding).getDefaultBuildingIndex()
			if iDefaultBuilding != iUniqueBuilding:
				screen.attachImageButton(pnl, "", gc.getBuildingInfo(iUniqueBuilding).getButton(), GenericButtonSizes.BUTTON_SIZE_CUSTOM, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BUILDING, iUniqueBuilding, 1, False)

		# place units
		pnl = self.top.getNextWidgetName()
		screen.addPanel(pnl, localText.getText("TXT_KEY_FREE_UNITS", ()), "", False, True, self.X_UNIT, self.Y_UNIT, self.W_UNIT, self.H_UNIT, ePnlBlue50)
		screen.attachLabel(pnl, "", "  ")
		for iUnit in range(gc.getNumUnitClassInfos()):
			iUniqueUnit = gc.getCivilizationInfo(self.iCivilization).getCivilizationUnits(iUnit)
			if iUniqueUnit < 0: continue
			iDefaultUnit = gc.getUnitClassInfo(iUnit).getDefaultUnitIndex()
			if iDefaultUnit != iUniqueUnit:
				screen.attachImageButton(pnl, "", gc.getUnitInfo(iUniqueUnit).getButton(), GenericButtonSizes.BUTTON_SIZE_CUSTOM, WidgetTypes.WIDGET_PEDIA_JUMP_TO_UNIT, iUniqueUnit, 1, False)

		# place leader
		pnl = self.top.getNextWidgetName()
		screen.addPanel(pnl, localText.getText("TXT_KEY_CONCEPT_LEADERS", ()), "", False, True, self.X_LEADER, self.Y_LEADER, self.W_LEADER, self.H_LEADER, ePnlBlue50)
		screen.attachLabel(pnl, "", "  ")
		for iLeader in range(gc.getNumLeaderHeadInfos()):
			civ = gc.getCivilizationInfo(self.iCivilization)
			if civ.isLeaders(iLeader):
				screen.attachImageButton(pnl, "", gc.getLeaderHeadInfo(iLeader).getButton(), GenericButtonSizes.BUTTON_SIZE_CUSTOM, WidgetTypes.WIDGET_PEDIA_JUMP_TO_LEADER, iLeader, self.iCivilization, False)

		# place text
		pnl = self.top.getNextWidgetName()
		screen.addPanel(pnl, "", "", True, True, self.X_TEXT, self.Y_TEXT, self.W_TEXT, self.H_TEXT, ePnlBlue50)
		szText = gc.getCivilizationInfo(self.iCivilization).getCivilopedia()
		screen.attachMultilineText(pnl, "Text", szText, WidgetTypes.WIDGET_GENERAL, -1, -1, 1<<0)


	def handleInput (self, inputClass):
		return 0
