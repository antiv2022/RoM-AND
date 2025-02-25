## RevolutionDCM Mod Code
##
## This code uses the BUG standard to control RevolutionDCM options.
## RevolutionDCM.ini is generated by BUG and is the repository of all option states.
## Options are passed to the SDK via python calls that modify GlobalDefinesAlt.xml.
## Note:
## This code will only alter values in GlobalDefinesAlt.xml that have
## been defined in /Assets/Config/RevDCM.xml. Other values in GlobalDefinesAlt 
## may be changed manually as per usual. Values that have been defined in RevDCM.xml
## may also be manually changed, but will be overwritten by this code either on
## new turns or on game load or game initialisation.
##
## Copyright (c) 2007-2008 The BUG Mod.
##
## Author: Glider1

from CvPythonExtensions import *
gc = CyGlobalContext()
import BugOptions
import BugCore
import BugUtil
import Popup as PyPopup
import CvUtil
RevDCMOpt = BugCore.game.RevDCM

class RevDCM:
	def __init__(self, eventManager):
		eventManager.addEventHandler("OnLoad", self.onLoadGame)
		eventManager.addEventHandler("GameStart", self.onGameStart)

	def onLoadGame(self,argsList):
		self.optionUpdate()

	def onGameStart(self,argsList):
		self.optionUpdate()

	def optionUpdate(self):
		if RevDCMOpt.isReset():
			resetOptions()
		else:
			setXMLOptionsfromIniFile()



##################################################		
# Module level functions defined in RevDCM.xml	
##################################################	

def changedReset (option, value):
	resetOptions()
	return True
	
########################################################################
# Functions that change the SDK variable states in global alt defines
########################################################################

#DCM
def changedDCM_STACK_ATTACK (option, value):
	gc.setDefineINT("DCM_STACK_ATTACK", RevDCMOpt.isDCM_STACK_ATTACK())
def changedDCM_OPP_FIRE (option, value):
	gc.setDefineINT("DCM_OPP_FIRE", RevDCMOpt.isDCM_OPP_FIRE())
def changedDCM_AIR_BOMBING (option, value):
	gc.setDefineINT("DCM_AIR_BOMBING", RevDCMOpt.isDCM_AIR_BOMBING())
def changedDCM_ACTIVE_DEFENSE (option, value):
	gc.setDefineINT("DCM_ACTIVE_DEFENSE", RevDCMOpt.isDCM_ACTIVE_DEFENSE())
def changedDCM_FIGHTER_ENGAGE (option, value):
	gc.setDefineINT("DCM_FIGHTER_ENGAGE", RevDCMOpt.isDCM_FIGHTER_ENGAGE())
def changedDCM_BATTLE_EFFECTS (option, value):
	gc.setDefineINT("DCM_BATTLE_EFFECTS", RevDCMOpt.isDCM_BATTLE_EFFECTS())
	
#IDW
def changedIDW_ENABLED (option, value):
	gc.setDefineINT("IDW_ENABLED", RevDCMOpt.isIDW_ENABLED())
def changedIDW_EMERGENCY_DRAFT_ENABLED (option, value):
	gc.setDefineINT("IDW_EMERGENCY_DRAFT_ENABLED", RevDCMOpt.isIDW_EMERGENCY_DRAFT_ENABLED())
def changedIDW_NO_BARBARIAN_INFLUENCE (option, value):
	gc.setDefineINT("IDW_NO_BARBARIAN_INFLUENCE", RevDCMOpt.isIDW_NO_BARBARIAN_INFLUENCE())
def changedIDW_NO_NAVAL_INFLUENCE (option, value):
	gc.setDefineINT("IDW_NO_NAVAL_INFLUENCE", RevDCMOpt.isIDW_NO_NAVAL_INFLUENCE())
def changedIDW_PILLAGE_INFLUENCE_ENABLED (option, value):
	gc.setDefineINT("IDW_PILLAGE_INFLUENCE_ENABLED", RevDCMOpt.isIDW_PILLAGE_INFLUENCE_ENABLED())
	
#SS	
def changedSS_ENABLED (option, value):
	gc.setDefineINT("SS_ENABLED", RevDCMOpt.isSS_ENABLED())
def changedSS_BRIBE (option, value):
	gc.setDefineINT("SS_BRIBE", RevDCMOpt.isSS_BRIBE())
def changedSS_ASSASSINATE (option, value):
	gc.setDefineINT("SS_ASSASSINATE", RevDCMOpt.isSS_ASSASSINATE())

#Religion
def changedOC_RESPAWN_HOLY_CITIES (option, value):
	gc.setDefineINT("OC_RESPAWN_HOLY_CITIES", RevDCMOpt.isOC_RESPAWN_HOLY_CITIES())
def changedLIMITED_RELIGIONS_EXCEPTIONS (option, value):
	gc.setDefineINT("LIMITED_RELIGIONS_EXCEPTIONS", RevDCMOpt.isLIMITED_RELIGIONS_EXCEPTIONS())

#Dynamic Civ Names
def changedDYNAMIC_CIV_NAMES(option, value):
	gc.setDefineINT("DYNAMIC_CIV_NAMES", RevDCMOpt.isDYNAMIC_CIV_NAMES())


def setXMLOptionsfromIniFile():
	print "Reinitialising RevDCM SDK variables"
	#DCM
	gc.setDefineINT("DCM_STACK_ATTACK", RevDCMOpt.isDCM_STACK_ATTACK())
	gc.setDefineINT("DCM_OPP_FIRE", RevDCMOpt.isDCM_OPP_FIRE())
	gc.setDefineINT("DCM_AIR_BOMBING", RevDCMOpt.isDCM_AIR_BOMBING())
	gc.setDefineINT("DCM_ACTIVE_DEFENSE", RevDCMOpt.isDCM_ACTIVE_DEFENSE())
	gc.setDefineINT("DCM_FIGHTER_ENGAGE", RevDCMOpt.isDCM_FIGHTER_ENGAGE())
	gc.setDefineINT("DCM_BATTLE_EFFECTS", RevDCMOpt.isDCM_BATTLE_EFFECTS())
	#IDW
	gc.setDefineINT("IDW_ENABLED", RevDCMOpt.isIDW_ENABLED())
	gc.setDefineINT("IDW_EMERGENCY_DRAFT_ENABLED", RevDCMOpt.isIDW_EMERGENCY_DRAFT_ENABLED())
	gc.setDefineINT("IDW_NO_BARBARIAN_INFLUENCE", RevDCMOpt.isIDW_NO_BARBARIAN_INFLUENCE())
	gc.setDefineINT("IDW_NO_NAVAL_INFLUENCE", RevDCMOpt.isIDW_NO_NAVAL_INFLUENCE())
	gc.setDefineINT("IDW_PILLAGE_INFLUENCE_ENABLED", RevDCMOpt.isIDW_PILLAGE_INFLUENCE_ENABLED())
	#SS
	gc.setDefineINT("SS_ENABLED", RevDCMOpt.isSS_ENABLED())
	gc.setDefineINT("SS_BRIBE", RevDCMOpt.isSS_BRIBE())
	gc.setDefineINT("SS_ASSASSINATE", RevDCMOpt.isSS_ASSASSINATE())
	#Religion
	gc.setDefineINT("OC_RESPAWN_HOLY_CITIES", RevDCMOpt.isOC_RESPAWN_HOLY_CITIES())
	gc.setDefineINT("LIMITED_RELIGIONS_EXCEPTIONS", 0)#RevDCMOpt.isLIMITED_RELIGIONS_EXCEPTIONS())
	#Dynamic Civ Names
	gc.setDefineINT("DYNAMIC_CIV_NAMES", RevDCMOpt.isDYNAMIC_CIV_NAMES())


def resetOptions():
	revDCMoptions = BugOptions.getOptions("RevDCM").options
	RevolutionOptions = BugOptions.getOptions("Revolution").options
	ActionOptions = BugOptions.getOptions("Actions").options
	for i in range(len(revDCMoptions)):
		revDCMoptions[i].resetValue()
	for i in range(len(RevolutionOptions)):
		RevolutionOptions[i].resetValue()
	for i in range(len(ActionOptions)):
		ActionOptions[i].resetValue()
	
	setXMLOptionsfromIniFile()
	RevDCMOpt.setReset(False)