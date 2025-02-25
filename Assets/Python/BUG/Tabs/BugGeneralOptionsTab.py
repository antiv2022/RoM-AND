## BugGeneralOptionsTab
##
## Tab for the BUG General Options (Main and City Screens).
##
## Copyright (c) 2007-2008 The BUG Mod.
##
## Author: EmperorFool

import BugOptionsTab
import Buffy

class BugGeneralOptionsTab(BugOptionsTab.BugOptionsTab):
	"BUG General Options Screen Tab"
	
	def __init__(self, screen):
		BugOptionsTab.BugOptionsTab.__init__(self, "General", "General")

	def create(self, screen):
		tab = self.createTab(screen)
		panel = self.createMainPanel(screen)
		column = self.addOneColumnLayout(screen, panel)
		
		left, center, right = self.addThreeColumnLayout(screen, column, "Top", True)
		
		self.createGreatPersonGeneralPanel(screen, left)
		self.addSpacer(screen, left, "General1")
		self.createTechSplashPanel(screen, left)
		self.addSpacer(screen, left, "General2")
		self.createLeaderheadPanel(screen, left)
		
		self.createAutoSavePanel(screen, center)
		self.addSpacer(screen, center, "General3")
		self.createActionsPanel(screen, center)
		
		self.createInfoPanePanel(screen, right)
		self.addSpacer(screen, right, "General4")
		self.createMiscellaneousPanel(screen, right)
		if Buffy.isEnabled():
			self.addSpacer(screen, right, "General5")
			self.createBuffyPanel(screen, right)
			
		self.createViewOptionsPanel(screen, column)
		
	def createGreatPersonGeneralPanel(self, screen, panel):
		self.addLabel(screen, panel, "ProgressBars", "Progress Bars:")
		self.addCheckboxTextDropdown(screen, panel, panel, "MainInterface__GPBar", "MainInterface__GPBar_Types")
		#self.addCheckbox(screen, panel, "MainInterface__GPBar")
		#self.addTextDropdown(screen, panel, panel, "MainInterface__GPBar_Types", True)
		self.addCheckbox(screen, panel, "MainInterface__Combat_Counter")
		
	def createLeaderheadPanel(self, screen, panel):
		self.addLabel(screen, panel, "Leaderheads", "Leaderheads:")
		self.addCheckbox(screen, panel, "MiscHover__LeaderheadWorstEnemy")
		self.addCheckbox(screen, panel, "MiscHover__LeaderheadDefensivePacts")
		
	def createAutoSavePanel(self, screen, panel):
		self.addLabel(screen, panel, "AutoSave", "AutoSave:")
		self.addCheckbox(screen, panel, "AutoSave__CreateStartSave")
		self.addCheckbox(screen, panel, "AutoSave__CreateEndSave")
		self.addCheckbox(screen, panel, "AutoSave__CreateExitSave")
		self.addCheckbox(screen, panel, "AutoSave__UsePlayerName")
		
	def createActionsPanel(self, screen, panel):
		self.addLabel(screen, panel, "Actions", "Actions:")
		self.addCheckbox(screen, panel, "Actions__AskDeclareWarUnits")
		self.addCheckbox(screen, panel, "Actions__SentryHealing")
		self.addCheckbox(screen, panel, "Actions__SentryHealingOnlyNeutral", True)
		self.addCheckbox(screen, panel, "Actions__PreChopForests")
		self.addCheckbox(screen, panel, "Actions__PreChopImprovements")
		
	def createTechSplashPanel(self, screen, panel):
		self.addLabel(screen, panel, "TechWindow", "Tech Splash Screen:")
		self.addTextDropdown(screen, panel, panel, "TechWindow__ViewType", True)
		self.addCheckbox(screen, panel, "TechWindow__CivilopediaText")
	
	def createBuffyPanel(self, screen, panel):
		self.addLabel(screen, panel, "BUFFY", "BUFFY:")
		self.addCheckbox(screen, panel, "BUFFY__WarningsDawnOfMan")
		self.addCheckbox(screen, panel, "BUFFY__WarningsSettings")
	
	def createInfoPanePanel(self, screen, panel):
		self.addLabel(screen, panel, "InfoPane", "Unit/Stack Info:")
		self.addCheckbox(screen, panel, "MainInterface__UnitMovementPointsFraction")
		self.addCheckbox(screen, panel, "MainInterface__StackMovementPoints")
		self.addCheckbox(screen, panel, "MainInterface__StackPromotions")
		self.addCheckbox(screen, panel, "MainInterface__StackPromotionCounts", True)
		
	def createMiscellaneousPanel(self, screen, panel):
		self.addLabel(screen, panel, "Misc", "Misc:")
		self.addCheckbox(screen, panel, "MainInterface__GoldRateWarning")
		self.addCheckbox(screen, panel, "MainInterface__MinMax_Commerce")
		self.addCheckbox(screen, panel, "MainInterface__ProgressBarsTickMarks")
		self.addTextDropdown(screen, panel, panel, "MainInterface__BuildIconSize", True)
		self.addCheckbox(screen, panel, "MainInterface__CityArrows")
		self.addCheckbox(screen, panel, "MainInterface__UseAIPathing")
		self.addCheckbox(screen, panel, "MainInterface__AutoEndDecisionlessTurns")
		self.addCheckbox(screen, panel, "MainInterface__InverseShiftForQueueing")

	def createViewOptionsPanel(self, screen, panel):
		self.addLabel(screen, panel, "View", "View options:")	
		left, right = self.addTwoColumnLayout(screen, panel, "ViewOptions", False)
		
		self.addCheckbox(screen, left, "MainInterface__EnableGraphicalPaging")
		
		#self.addTextEdit(screen, left, left, "MainInterface__GraphicalPageInRange", True)
		#self.addTextEdit(screen, left, left, "MainInterface__GraphicalPageOutRange", True)

		
		self.addCheckbox(screen, right, "MainInterface__EnableViewports")
		
		self.addTextEdit(screen, right, right, "MainInterface__ViewportX", True)
		self.addTextEdit(screen, right, right, "MainInterface__ViewportY", True)
		self.addTextEdit(screen, right, right, "MainInterface__ViewportAutoSwitchBorder", True)
		self.addTextEdit(screen, right, right, "MainInterface__ViewportAutoCenterBorder", True)
