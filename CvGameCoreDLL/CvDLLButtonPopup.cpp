// buttonPopup.cpp

#include "CvGameCoreDLL.h"
#include "CvDLLButtonPopup.h"
#include "CvGlobals.h"
#include "CvPlayerAI.h"
#include "CvGameAI.h"
#include "CvTeamAI.h"
#include "CvMap.h"
#include "CvViewport.h"
#include "CvPlot.h"
#include "CvArtFileMgr.h"
#include "CyCity.h"
#include "CyArgsList.h"
#include "CvPopupReturn.h"
#include "CvInfos.h"
#include "CvInitCore.h"
#include "CvGameTextMgr.h"
#include "CvDLLPythonIFaceBase.h"
#include "CvDLLInterfaceIFaceBase.h"
#include "CvDLLEntityIFaceBase.h"
#include "CvGameCoreUtils.h"
#include "CvDLLEngineIFaceBase.h"
#include "CvEventReporter.h"
#include "CvMessageControl.h"

// BUG - start
// RevolutionDCM - BugMod included in CvGlobals.h
//#include "BugMod.h"
#include "CvBugOptions.h"
// BUG - end

// Public Functions...

#define PASSWORD_DEFAULT (L"*****")

CvDLLButtonPopup* CvDLLButtonPopup::m_pInst = NULL;

CvDLLButtonPopup& CvDLLButtonPopup::getInstance()
{
	if (m_pInst == NULL)
	{
		m_pInst = new CvDLLButtonPopup;
	}
	return *m_pInst;
}

void CvDLLButtonPopup::freeInstance()
{
	delete m_pInst;
	m_pInst = NULL;
}

CvDLLButtonPopup::CvDLLButtonPopup()
{
}


CvDLLButtonPopup::~CvDLLButtonPopup()
{
}

void CvDLLButtonPopup::OnAltExecute(CvPopup& popup, const PopupReturn& popupReturn, CvPopupInfo &info)
{
	CvPopupInfo* pInfo = new CvPopupInfo;
	if (pInfo)
	{
		*pInfo = info;
		gDLL->getInterfaceIFace()->addPopup(pInfo);
		gDLL->getInterfaceIFace()->popupSetAsCancelled(&popup);
	}
}


void CvDLLButtonPopup::OnOkClicked(CvPopup* pPopup, PopupReturn *pPopupReturn, CvPopupInfo &info)
{
	int iExamineCityID;
	int iI;

	switch (info.getButtonPopupType())
	{
	case BUTTONPOPUP_TEXT:
		break;

	case BUTTONPOPUP_CONFIRM_MENU:
		if ( pPopupReturn->getButtonClicked() == 0 )
		{
			switch (info.getData1())
			{
			case 0:
// BUG - Exit Save - start
				if (GC.getGameINLINE().getVictory() == NO_VICTORY)
				{
					PYTHON_CALL_FUNCTION2(__FUNCTION__, PYBugModule, "gameExitSave");
				}
// BUG - Exit Save - end
				gDLL->SetDone(true);
				break;
			case 1:
// BUG - Exit Save - start
				if (GC.getGameINLINE().getVictory() == NO_VICTORY)
				{
					PYTHON_CALL_FUNCTION2(__FUNCTION__, PYBugModule, "gameExitSave");
				}
// BUG - Exit Save - end
				gDLL->getInterfaceIFace()->exitingToMainMenu();
				break;
			case 2:
				GC.getGameINLINE().doControl(CONTROL_RETIRE);
				break;
			case 3:
				GC.getGameINLINE().regenerateMap();
				break;
			case 4:
				GC.getGameINLINE().doControl(CONTROL_WORLD_BUILDER);
				break;
			}
		}
		break;

	case BUTTONPOPUP_MAIN_MENU:
		if (pPopupReturn->getButtonClicked() == 0)
		{	// exit to desktop
			CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_CONFIRM_MENU);
			if (NULL != pInfo)
			{
				pInfo->setData1(0);
				gDLL->getInterfaceIFace()->addPopup(pInfo, GC.getGameINLINE().getActivePlayer(), true);
			}
		}
		else if (pPopupReturn->getButtonClicked() == 1)
		{	// exit to main menu
			CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_CONFIRM_MENU);
			if (NULL != pInfo)
			{
				pInfo->setData1(1);
				gDLL->getInterfaceIFace()->addPopup(pInfo, GC.getGameINLINE().getActivePlayer(), true);
			}
		}
		else if (pPopupReturn->getButtonClicked() == 2)
		{
			if( GC.getGameINLINE().isPitboss() ){
				CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_CONFIRM_MENU_KI);
				if (NULL != pInfo)
				{
					pInfo->setData1(10005);
					gDLL->getInterfaceIFace()->addPopup(pInfo, GC.getGameINLINE().getActivePlayer(), true);
				}
			}else{
				CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_CONFIRM_MENU);
				if (NULL != pInfo)
				{
					pInfo->setData1(2);
					gDLL->getInterfaceIFace()->addPopup(pInfo, GC.getGameINLINE().getActivePlayer(), true);
				}
			}
		}
		else if (pPopupReturn->getButtonClicked() == 3)
		{
			CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_CONFIRM_MENU);
			if (NULL != pInfo)
			{
				pInfo->setData1(3);
				gDLL->getInterfaceIFace()->addPopup(pInfo, GC.getGameINLINE().getActivePlayer(), true);
			}
		}
		else if (pPopupReturn->getButtonClicked() == 4)
		{	// load game
			GC.getGameINLINE().doControl(CONTROL_LOAD_GAME);
		}
		else if (pPopupReturn->getButtonClicked() == 5)
		{	// save game
			GC.getGameINLINE().doControl(CONTROL_SAVE_NORMAL);
		}
		else if (pPopupReturn->getButtonClicked() == 6)
		{	// options
			PYTHON_CALL_FUNCTION2(__FUNCTION__, "CvScreensInterface", "showOptionsScreen");
		}
		else if (pPopupReturn->getButtonClicked() == 7)
		{
			CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_CONFIRM_MENU);
			if (NULL != pInfo)
			{
				pInfo->setData1(4);
				gDLL->getInterfaceIFace()->addPopup(pInfo, GC.getGameINLINE().getActivePlayer(), true);
			}
		}
		else if (pPopupReturn->getButtonClicked() == 8)
		{	// Game details
			GC.getGameINLINE().doControl(CONTROL_ADMIN_DETAILS);
		}
		else if (pPopupReturn->getButtonClicked() == 9)
		{	// player details
			GC.getGameINLINE().doControl(CONTROL_DETAILS);
		}
		// 10 - cancel
		break;

	case BUTTONPOPUP_DECLAREWARMOVE:
		if (pPopupReturn->getButtonClicked() == 0)
		{
			CvMessageControl::getInstance().sendChangeWar((TeamTypes)info.getData1(), true);
			CvPlot::invalidateBestDefenderCache();
		}
		if (((pPopupReturn->getButtonClicked() == 0) || info.getOption2()) && info.getFlags() == 0)
		{
			GC.getGameINLINE().selectionListGameNetMessageInternal(GAMEMESSAGE_PUSH_MISSION, MISSION_MOVE_TO, info.getData2(), info.getData3(), info.getFlags(), false, info.getOption1(), false);
		}
		break;

	case BUTTONPOPUP_CONFIRMCOMMAND:
		if (pPopupReturn->getButtonClicked() == 0)
		{
			int iAction = info.getData1();
			GC.getGameINLINE().selectionListGameNetMessage(GAMEMESSAGE_DO_COMMAND, GC.getActionInfo(iAction).getCommandType(), GC.getActionInfo(iAction).getCommandData(), -1, 0, info.getOption1());
		}
		break;

	case BUTTONPOPUP_LOADUNIT:
		if (pPopupReturn->getButtonClicked() != 0)
		{
			CLLNode<IDInfo>* pUnitNode;
			CvSelectionGroup* pSelectionGroup;
			CvUnit* pLoopUnit;
			CvPlot* pPlot;
			int iCount;

			pSelectionGroup = gDLL->getInterfaceIFace()->getSelectionList();

			if (NULL != pSelectionGroup)
			{
				pPlot = pSelectionGroup->plot();

				iCount = pPopupReturn->getButtonClicked();

				pUnitNode = pPlot->headUnitNode();

				while (pUnitNode != NULL)
				{
					pLoopUnit = ::getUnit(pUnitNode->m_data);
					pUnitNode = pPlot->nextUnitNode(pUnitNode);

					if (pSelectionGroup->canDoCommand(COMMAND_LOAD_UNIT, pLoopUnit->getOwnerINLINE(), pLoopUnit->getID()))
					{
						iCount--;
						if (iCount == 0)
						{
							GC.getGameINLINE().selectionListGameNetMessage(GAMEMESSAGE_DO_COMMAND, COMMAND_LOAD_UNIT, pLoopUnit->getOwnerINLINE(), pLoopUnit->getID());
							break;
						}
					}
				}
			}
		}
		break;

	case BUTTONPOPUP_LEADUNIT:
		if (pPopupReturn->getButtonClicked() != 0)
		{
			CLLNode<IDInfo>* pUnitNode;
			CvSelectionGroup* pSelectionGroup;
			CvUnit* pLoopUnit;
			CvPlot* pPlot;
			int iCount;

			pSelectionGroup = gDLL->getInterfaceIFace()->getSelectionList();

			if (NULL != pSelectionGroup)
			{
				pPlot = pSelectionGroup->plot();

				iCount = pPopupReturn->getButtonClicked();

				pUnitNode = pPlot->headUnitNode();

				while (pUnitNode != NULL)
				{
					pLoopUnit = ::getUnit(pUnitNode->m_data);
					pUnitNode = pPlot->nextUnitNode(pUnitNode);

					if (pLoopUnit->canPromote((PromotionTypes) info.getData1(), info.getData2()))
					{
						iCount--;
						if (iCount == 0)
						{
							GC.getGameINLINE().selectionListGameNetMessage(GAMEMESSAGE_PUSH_MISSION, MISSION_LEAD, pLoopUnit->getID());
							break;
						}
					}
				}
			}
		}
		break;

	case BUTTONPOPUP_DOESPIONAGE:
		if (pPopupReturn->getButtonClicked() != NO_ESPIONAGEMISSION)
		{
			GC.getGameINLINE().selectionListGameNetMessage(GAMEMESSAGE_PUSH_MISSION, MISSION_ESPIONAGE, (EspionageMissionTypes) pPopupReturn->getButtonClicked());
		}
		break;

	case BUTTONPOPUP_DOESPIONAGE_TARGET:
		if (pPopupReturn->getButtonClicked() != -1)
		{
			GC.getGameINLINE().selectionListGameNetMessage(GAMEMESSAGE_PUSH_MISSION, MISSION_ESPIONAGE, (EspionageMissionTypes)info.getData1(), pPopupReturn->getButtonClicked());
		}
		break;

	case BUTTONPOPUP_CHOOSETECH:
		if (pPopupReturn->getButtonClicked() == GC.getNumTechInfos())
		{
			PYTHON_CALL_FUNCTION2(__FUNCTION__, "CvScreensInterface", "showTechChooser");
			GET_PLAYER(GC.getGameINLINE().getActivePlayer()).chooseTech(0, "", true);
		}
		break;

	case BUTTONPOPUP_RAZECITY:
		if (pPopupReturn->getButtonClicked() == 1)
		{
			CvMessageControl::getInstance().sendDoTask(info.getData1(), TASK_RAZE, -1, -1, false, false, false, false);
		}
		else if (pPopupReturn->getButtonClicked() == 2)
		{
			CvCity* pCity = GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCity(info.getData1());
			if (NULL != pCity)
			{
				CvMessageControl::getInstance().sendDoTask(info.getData1(), TASK_KEEP, GC.getGameINLINE().getActivePlayer(), -1, false, false, false, false);
			}

			CvMessageControl::getInstance().sendDoTask(info.getData1(), TASK_GIFT, info.getData2(), -1, false, false, false, false);
		}
		else if (pPopupReturn->getButtonClicked() == 3)
		{
			CvCity* pCity = GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCity(info.getData1());
			if (NULL != pCity)
			{
				gDLL->getInterfaceIFace()->selectCity(pCity, false);
			}

			CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_RAZECITY, info.getData1(), info.getData2(), info.getData3());
			gDLL->getInterfaceIFace()->addPopup(pInfo, GC.getGameINLINE().getActivePlayer(), false, true);
		}
		else if (pPopupReturn->getButtonClicked() == 4)
		{
			CvMessageControl::getInstance().sendDoTask(info.getData1(), TASK_RAZE, GC.getGameINLINE().getActivePlayer(), 1, false, false, false, false);
		}
		else if (pPopupReturn->getButtonClicked() == 0)
		{
			CvCity* pCity = GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCity(info.getData1());
			if (NULL != pCity)
			{
				pCity->chooseProduction();
				CvMessageControl::getInstance().sendDoTask(info.getData1(), TASK_KEEP, GC.getGameINLINE().getActivePlayer(), -1, false, false, false, false);
			}
		}
		break;

	case BUTTONPOPUP_DISBANDCITY:
		if (pPopupReturn->getButtonClicked() == 1)
		{
			CvMessageControl::getInstance().sendDoTask(info.getData1(), TASK_DISBAND, -1, -1, false, false, false, false);
		}
		else if (pPopupReturn->getButtonClicked() == 0)
		{
			CvCity* pCity = GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCity(info.getData1());
			if (NULL != pCity)
			{
				pCity->chooseProduction();
				CvMessageControl::getInstance().sendDoTask(info.getData1(), TASK_KEEP, GC.getGameINLINE().getActivePlayer(), -1, false, false, false, false);
			}
		}
// BUG - Examine Culture Flip - start
		else if (pPopupReturn->getButtonClicked() == 2)
		{
			CvCity* pCity = GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCity(info.getData1());
			if (NULL != pCity)
			{
				gDLL->getInterfaceIFace()->selectCity(pCity, false);
			}

			CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_DISBANDCITY, info.getData1());
			gDLL->getInterfaceIFace()->addPopup(pInfo, GC.getGameINLINE().getActivePlayer(), false, true);
		}
// BUG - Examine Culture Flip - end
		break;

	case BUTTONPOPUP_CHOOSEPRODUCTION:
		iExamineCityID = 0;
		iExamineCityID = std::max(iExamineCityID, GC.getNumUnitInfos());
		iExamineCityID = std::max(iExamineCityID, GC.getNumBuildingInfos());
		iExamineCityID = std::max(iExamineCityID, GC.getNumProjectInfos());
		iExamineCityID = std::max(iExamineCityID, GC.getNumProcessInfos());

		if (pPopupReturn->getButtonClicked() == iExamineCityID)
		{
			CvCity* pCity = GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCity(info.getData1());
			if (pCity != NULL)
			{
				gDLL->getInterfaceIFace()->selectCity(pCity, true);
			}
		}
		break;

	case BUTTONPOPUP_CHANGECIVIC:
		if (pPopupReturn->getButtonClicked() == 0)
		{
			std::vector<CivicTypes> aeNewCivics(GC.getNumCivicOptionInfos());
			for (iI = 0; iI < GC.getNumCivicOptionInfos(); iI++)
			{
				aeNewCivics[iI] = GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCivics((CivicOptionTypes)iI);
			}
			aeNewCivics[info.getData1()] = ((CivicTypes)(info.getData2()));

			CvMessageControl::getInstance().sendUpdateCivics(aeNewCivics);
		}
		else if (pPopupReturn->getButtonClicked() == 2)
		{
			PYTHON_CALL_FUNCTION2(__FUNCTION__, PYScreensModule, "showCivicsScreen");
		}
		break;

	case BUTTONPOPUP_CHANGERELIGION:
		if (pPopupReturn->getButtonClicked() == 0)
		{
			CvMessageControl::getInstance().sendConvert((ReligionTypes)(info.getData1()));
		}
		break;

	case BUTTONPOPUP_CHOOSEELECTION:
		{
			VoteSelectionData* pData = GC.getGameINLINE().getVoteSelection(info.getData1());
			if (NULL != pData && pPopupReturn->getButtonClicked() < (int)pData->aVoteOptions.size())
			{
				CvMessageControl::getInstance().sendChooseElection((VoteTypes)(pPopupReturn->getButtonClicked()), info.getData1());
			}
		}
		break;

	case BUTTONPOPUP_DIPLOVOTE:
		CvMessageControl::getInstance().sendDiploVote(info.getData1(), (PlayerVoteTypes)pPopupReturn->getButtonClicked());
		break;

	case BUTTONPOPUP_ALARM:
		break;

	case BUTTONPOPUP_DEAL_CANCELED:
		if (pPopupReturn->getButtonClicked() == 0)
		{
			gDLL->sendKillDeal(info.getData1(), info.getOption1());
		}
		break;

	case BUTTONPOPUP_PYTHON:
		if (!info.getOnClickedPythonCallback().IsEmpty())
		{
			PYTHON_ACCESS_LOCK_SCOPE

			FAssertMsg(!GC.getGameINLINE().isNetworkMultiPlayer(), "Danger: Out of Sync");
			CyArgsList argsList;
			argsList.add(pPopupReturn->getButtonClicked());
			argsList.add(info.getData1());
			argsList.add(info.getData2());
			argsList.add(info.getData3());
			argsList.add(info.getFlags());
			argsList.add(info.getText());
			argsList.add(info.getOption1());
			argsList.add(info.getOption2());
			PYTHON_CALL_FUNCTION(__FUNCTION__, (info.getPythonModule().IsEmpty() ? PYScreensModule : info.getPythonModule()), info.getOnClickedPythonCallback(), argsList.makeFunctionArgs());
			break;
		}
		break;

	case BUTTONPOPUP_DETAILS:
		{
			// Civ details
			PlayerTypes eID = GC.getInitCore().getActivePlayer();

			CvWString szLeaderName = GC.getInitCore().getLeaderName(eID);
			CvWString szCivDescription = GC.getInitCore().getCivDescription(eID);
			CvWString szCivShortDesc = GC.getInitCore().getCivShortDesc(eID);
			CvWString szCivAdjective = GC.getInitCore().getCivAdjective(eID);
			CvWString szCivPassword = PASSWORD_DEFAULT;
			CvString szEmail = GC.getInitCore().getEmail(eID);
			CvString szSmtpHost = GC.getInitCore().getSmtpHost(eID);

			if (pPopupReturn->getEditBoxString(0) && *(pPopupReturn->getEditBoxString(0)))
			{
				szLeaderName = pPopupReturn->getEditBoxString(0);
			}
			if (pPopupReturn->getEditBoxString(1) && *(pPopupReturn->getEditBoxString(1)))
			{
				szCivDescription = pPopupReturn->getEditBoxString(1);
			}
			if (pPopupReturn->getEditBoxString(2) && *(pPopupReturn->getEditBoxString(2)))
			{
				szCivShortDesc = pPopupReturn->getEditBoxString(2);
			}
			if (pPopupReturn->getEditBoxString(3) && *(pPopupReturn->getEditBoxString(3)))
			{
				szCivAdjective = pPopupReturn->getEditBoxString(3);
			}
			if (GC.getGameINLINE().isHotSeat() || GC.getGameINLINE().isPbem())
			{
				if (pPopupReturn->getEditBoxString(4) && *(pPopupReturn->getEditBoxString(4)))
				{
					szCivPassword = pPopupReturn->getEditBoxString(4);
				}
			}
			if (GC.getGameINLINE().isPitboss() || GC.getGameINLINE().isPbem())
			{
				if (pPopupReturn->getEditBoxString(5) && *(pPopupReturn->getEditBoxString(5)))
				{
					szEmail = CvString(pPopupReturn->getEditBoxString(5));
				}
			}
			if (GC.getGameINLINE().isPbem())
			{
				if (pPopupReturn->getEditBoxString(6) && *(pPopupReturn->getEditBoxString(6)))
				{
					szSmtpHost = CvString(pPopupReturn->getEditBoxString(6));
				}
			}

			GC.getInitCore().setLeaderName(eID, szLeaderName);
			GC.getInitCore().setCivDescription(eID, szCivDescription);
			GC.getInitCore().setCivShortDesc(eID, szCivShortDesc);
			GC.getInitCore().setCivAdjective(eID, szCivAdjective);
			if (szCivPassword != PASSWORD_DEFAULT)
			{
				GC.getInitCore().setCivPassword(eID, szCivPassword);
			}
			GC.getInitCore().setEmail(eID, szEmail);
			GC.getInitCore().setSmtpHost(eID, szSmtpHost);
			gDLL->sendPlayerInfo(eID);

			if (GC.getGameINLINE().isPbem() && pPopupReturn->getButtonClicked() == 0)
			{
				gDLL->sendPbemTurn(NO_PLAYER);
			}

		}
		break;

	case BUTTONPOPUP_ADMIN:
		{
			// Game details
			CvWString szGameName;
			CvWString szAdminPassword = GC.getInitCore().getAdminPassword();
			if (pPopupReturn->getEditBoxString(0) && *(pPopupReturn->getEditBoxString(0)))
			{
				szGameName = pPopupReturn->getEditBoxString(0);
			}
			if (pPopupReturn->getEditBoxString(1) && CvWString(pPopupReturn->getEditBoxString(1)) != PASSWORD_DEFAULT)
			{
				if (*(pPopupReturn->getEditBoxString(1)))
				{
					szAdminPassword = CvWString(gDLL->md5String((char*)CvString(pPopupReturn->getEditBoxString(1)).GetCString()));
				}
				else
				{
					szAdminPassword = L"";
				}
			}
			if (!GC.getGameINLINE().isGameMultiPlayer())
			{
				if (pPopupReturn->getCheckboxBitfield(2) && pPopupReturn->getCheckboxBitfield(2) > 0)
				{
					gDLL->setChtLvl(1);
				}
				else
				{
					gDLL->setChtLvl(0);
				}
			}

			gDLL->sendGameInfo(szGameName, szAdminPassword);

		}
		break;

	case BUTTONPOPUP_ADMIN_PASSWORD:
		{
			CvWString szAdminPassword;
			if (pPopupReturn->getEditBoxString(0) && CvWString(pPopupReturn->getEditBoxString(0)) != PASSWORD_DEFAULT)
			{
				szAdminPassword = pPopupReturn->getEditBoxString(0);
			}
			if (CvWString(gDLL->md5String((char*)CvString(szAdminPassword).GetCString())) == GC.getInitCore().getAdminPassword())
			{
				switch ((ControlTypes)info.getData1())
				{
				case CONTROL_WORLD_BUILDER:
					gDLL->getInterfaceIFace()->setWorldBuilder(!gDLL->GetWorldBuilderMode());
					break;
				case CONTROL_ADMIN_DETAILS:
					gDLL->getInterfaceIFace()->showAdminDetails();
					break;
				default:
					break;
				}
			}
			else
			{
				CvPopupInfo* pInfo = new CvPopupInfo();
				if (NULL != pInfo)
				{
					pInfo->setText(gDLL->getText("TXT_KEY_BAD_PASSWORD_DESC"));
					gDLL->getInterfaceIFace()->addPopup(pInfo, NO_PLAYER, true);
				}
			}
		}
		break;

	case BUTTONPOPUP_EXTENDED_GAME:
		if (pPopupReturn->getButtonClicked() == 0)
		{
			if (GC.getGameINLINE().isNetworkMultiPlayer())
			{

				CvMessageControl::getInstance().sendExtendedGame();
			}
			else
			{
				GC.getGameINLINE().setGameState(GAMESTATE_EXTENDED);
			}
		}
		else if (pPopupReturn->getButtonClicked() == 1)
		{
			// exit to main menu
			if (GC.getGameINLINE().isNetworkMultiPlayer() && GC.getGameINLINE().canDoControl(CONTROL_RETIRE) && GC.getGameINLINE().countHumanPlayersAlive() > 1)
			{
				GC.getGameINLINE().doControl(CONTROL_RETIRE);
			}
			else if (!gDLL->getInterfaceIFace()->isDebugMenuCreated())
			{
				gDLL->getInterfaceIFace()->exitingToMainMenu();
			}
			else
			{
				gDLL->SetDone(true);
			}
		}
		break;

	case BUTTONPOPUP_DIPLOMACY:
		if (pPopupReturn->getButtonClicked() != MAX_CIV_PLAYERS)
		{
			GET_PLAYER(GC.getGameINLINE().getActivePlayer()).contact((PlayerTypes)(pPopupReturn->getButtonClicked()));
		}
		break;

	case BUTTONPOPUP_ADDBUDDY:
		if (pPopupReturn->getButtonClicked() == 0)
		{
			gDLL->AcceptBuddy(CvString(info.getText()).GetCString(), info.getData1());
		}
		else
		{
			gDLL->RejectBuddy(CvString(info.getText()).GetCString(), info.getData1());
		}
		break;

	case BUTTONPOPUP_FORCED_DISCONNECT:
	case BUTTONPOPUP_PITBOSS_DISCONNECT:
	case BUTTONPOPUP_KICKED:
		gDLL->getInterfaceIFace()->exitingToMainMenu();
		break;

	case BUTTONPOPUP_VASSAL_DEMAND_TRIBUTE:
		if (pPopupReturn->getButtonClicked() < GC.getNumBonusInfos())
		{
			PlayerTypes eVassal = (PlayerTypes)info.getData1();
			if (GET_PLAYER(eVassal).isHuman())
			{
				CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_VASSAL_GRANT_TRIBUTE, GC.getGameINLINE().getActivePlayer(), pPopupReturn->getButtonClicked());
				if (NULL != pInfo)
				{
					gDLL->sendPopup(eVassal, pInfo);
				}
			}
			else
			{
				TradeData item;
				setTradeItem(&item, TRADE_RESOURCES, pPopupReturn->getButtonClicked());

				CLinkList<TradeData> ourList;
				CLinkList<TradeData> theirList;
				theirList.insertAtEnd(item);

				if (GET_PLAYER(eVassal).AI_considerOffer(GC.getGameINLINE().getActivePlayer(), &ourList, &theirList))
				{
					gDLL->sendImplementDealMessage(eVassal, &ourList, &theirList);

					MEMORY_TRACK_EXEMPT();

					CvWString szBuffer = gDLL->getText("TXT_KEY_VASSAL_GRANT_TRIBUTE_ACCEPTED", GET_PLAYER(eVassal).getNameKey(), GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getNameKey(), GC.getBonusInfo((BonusTypes)pPopupReturn->getButtonClicked()).getTextKeyWide());
					AddDLLMessage(GC.getGameINLINE().getActivePlayer(), false, GC.getEVENT_MESSAGE_TIME(), szBuffer);
				}
				else
				{
					CvMessageControl::getInstance().sendChangeWar(GET_PLAYER(eVassal).getTeam(), true);
				}
			}
		}
		break;

	case BUTTONPOPUP_VASSAL_GRANT_TRIBUTE:
		if (pPopupReturn->getButtonClicked() == 0)
		{
			TradeData item;
			setTradeItem(&item, TRADE_RESOURCES, info.getData2());

			CLinkList<TradeData> ourList;
			CLinkList<TradeData> theirList;
			ourList.insertAtEnd(item);

			gDLL->sendImplementDealMessage((PlayerTypes)info.getData1(), &ourList, &theirList);

			MEMORY_TRACK_EXEMPT();

			CvWString szBuffer = gDLL->getText("TXT_KEY_VASSAL_GRANT_TRIBUTE_ACCEPTED", GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getNameKey(), GET_PLAYER((PlayerTypes)info.getData1()).getNameKey(), GC.getBonusInfo((BonusTypes)info.getData2()).getTextKeyWide());
			AddDLLMessage((PlayerTypes)info.getData1(), false, GC.getEVENT_MESSAGE_TIME(), szBuffer);
		}
		else
		{
			CvMessageControl::getInstance().sendChangeWar(GET_PLAYER((PlayerTypes)info.getData1()).getTeam(), true);
		}

		break;

	case BUTTONPOPUP_EVENT:
		if (pPopupReturn->getButtonClicked() == GC.getNumEventInfos())
		{
			CvPlayer& kActivePlayer = GET_PLAYER(GC.getGameINLINE().getActivePlayer());
			EventTriggeredData* pTriggeredData = kActivePlayer.getEventTriggered(info.getData1());
			if (NULL != pTriggeredData)
			{
				CvCity* pCity = kActivePlayer.getCity(pTriggeredData->m_iCityId);
				if (NULL != pCity)
				{
					gDLL->getInterfaceIFace()->selectCity(pCity, true);
				}
			}

			CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_EVENT, info.getData1());
			gDLL->getInterfaceIFace()->addPopup(pInfo, GC.getGameINLINE().getActivePlayer(), false, true);
		}
		else if (-1 != pPopupReturn->getButtonClicked())
		{
			CvMessageControl::getInstance().sendEventTriggered(GC.getGameINLINE().getActivePlayer(), (EventTypes)pPopupReturn->getButtonClicked(), info.getData1());
		}
		break;

	case BUTTONPOPUP_FREE_COLONY:
		if (pPopupReturn->getButtonClicked() > 0)
		{
			CvMessageControl::getInstance().sendEmpireSplit(GC.getGameINLINE().getActivePlayer(), pPopupReturn->getButtonClicked());
		}
		else if (pPopupReturn->getButtonClicked() < 0)
		{
			CvMessageControl::getInstance().sendDoTask(-pPopupReturn->getButtonClicked(), TASK_LIBERATE, 0, -1, false, false, false, false);
		}
		break;

	case BUTTONPOPUP_LAUNCH:
		if (0 == pPopupReturn->getButtonClicked())
		{
			CvMessageControl::getInstance().sendLaunch(GC.getGameINLINE().getActivePlayer(), (VictoryTypes)info.getData1());
		}
		break;

	case BUTTONPOPUP_FOUND_RELIGION:
		CvMessageControl::getInstance().sendFoundReligion(GC.getGameINLINE().getActivePlayer(), (ReligionTypes)pPopupReturn->getButtonClicked(), (ReligionTypes)info.getData1());
		break;

	case BUTTONPOPUP_GET_SAVE_FORMAT:
		{
			CvTaggedSaveFormatWrapper&	wrapper = CvTaggedSaveFormatWrapper::getSaveFormatWrapper();

			if (1 == pPopupReturn->getButtonClicked())
			{
				wrapper.m_requestUseTaggedFormat = true;
			}

			gDLL->SaveGame(SAVEGAME_NORMAL);
		}
		break;

	case BUTTONPOPUP_SAVE_INFO_LOST:
		break;

	case BUTTONPOPUP_MODIFIER_RECALCULATION:
		{
			if (1 == pPopupReturn->getButtonClicked())
			{
				CvMessageControl::getInstance().sendRecalculateModifiers();;
			}
		}
		break;
	case BUTTONPOPUP_INVASION:
		break;
	case BUTTONPOPUP_SELECT_UNIT:
		if (pPopupReturn->getButtonClicked() != 0)
		{
			GC.getGameINLINE().selectionListGameNetMessage(GAMEMESSAGE_PUSH_MISSION, MISSION_SHADOW, info.getData2(), info.getData3(), pPopupReturn->getButtonClicked());
		}
		break;
	case BUTTONPOPUP_SELECT_DISCOVERY_TECH:
		if (pPopupReturn->getButtonClicked() != GC.getNumTechInfos())
		{
			GC.getGameINLINE().selectionListGameNetMessage(GAMEMESSAGE_PUSH_MISSION, MISSION_WAIT_FOR_TECH, pPopupReturn->getButtonClicked(), pPopupReturn->getButtonClicked(), GC.getNumTechInfos());
		}
		break;
	case BUTTONPOPUP_FIELD_COMMANDER_TUTORIAL:
		break;
	case BUTTONPOPUP_DARK_AGE:
		break;
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/

	case BUTTONPOPUP_GOTO_CITY:
		break;

/************************************************************************************************/
/* Great Diplomat MOD               START                                                  		*/
/************************************************************************************************/
	case BUTTONPOPUP_UNIT_FORCE_PEACE:
		if (pPopupReturn->getButtonClicked() != -1)
		{
			PlayerTypes eActivePlayer = GC.getGameINLINE().getActivePlayer();
			CvUnit* pUnit = GET_PLAYER(eActivePlayer).getUnit(info.getData1());
			pUnit->applyForcePeace((PlayerTypes)pPopupReturn->getButtonClicked());
		}
		break;
/************************************************************************************************/
/* Great Diplomat MOD               END                                                  		*/
/************************************************************************************************/

	case BUTTONPOPUP_CONFIRM_MENU_KI:
		{
			if ( pPopupReturn->getButtonClicked() == 0 )
			{
				switch (info.getData1())
				{
					case 10005:
						gDLL->getInterfaceIFace()->exitingToMainMenu();
						break;
				}
			}
			if ( pPopupReturn->getButtonClicked() == 2 )
			{
				switch (info.getData1())
				{
					case 10005:
						GC.getGameINLINE().doControl(CONTROL_RETIRE);
						break;
				}
			}
		}
		break;

	default:
		FAssert(false);
		break;
	}
}

void CvDLLButtonPopup::OnFocus(CvPopup* pPopup, CvPopupInfo &info)
{
	if (gDLL->getInterfaceIFace()->popupIsDying(pPopup))
	{
		return;
	}

	PlayerTypes ePlayer = GC.getGameINLINE().getActivePlayer();
	if ( ePlayer != NO_PLAYER )
	{
		//OutputDebugString("UI interaction - popup focus\n");
		FAssert(GET_PLAYER(ePlayer).isHuman());
		GET_PLAYER(ePlayer).setTurnHadUIInteraction(true);
	}

	switch (info.getButtonPopupType())
	{
	case BUTTONPOPUP_CHOOSETECH:
		if (info.getData1() == 0)
		{
			if ((GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCurrentResearch() != NO_TECH) || (GC.getGameINLINE().getGameState() == GAMESTATE_OVER))
			{
				gDLL->getInterfaceIFace()->popupSetAsCancelled(pPopup);
			}
		}
		break;

	case BUTTONPOPUP_CHANGERELIGION:
		if (!(GET_PLAYER(GC.getGameINLINE().getActivePlayer()).canChangeReligion()) || (GC.getGameINLINE().getGameState() == GAMESTATE_OVER))
		{
			gDLL->getInterfaceIFace()->popupSetAsCancelled(pPopup);
		}
		break;

	case BUTTONPOPUP_CHOOSEPRODUCTION:
		if (GC.getGameINLINE().getGameState() == GAMESTATE_OVER)
		{
			gDLL->getInterfaceIFace()->popupSetAsCancelled(pPopup);
		}
		else
		{
			PlayerTypes ePlayer = GC.getGameINLINE().getActivePlayer();
			CvCity* pCity = GET_PLAYER(ePlayer).getCity(info.getData1());

			if (NULL == pCity || pCity->getOwnerINLINE() != ePlayer || pCity->isProduction())
			{
				gDLL->getInterfaceIFace()->popupSetAsCancelled(pPopup);
				break;
			}

			//if (GC.getDefineINT("ZOOM_CITY_FOR_PRODUCTION_POPUP"))
			if (getBugOptionBOOL("RoMSettings__NoProductionPopup", false, "ZOOM_CITY_FOR_PRODUCTION_POPUP"))
			{
				gDLL->getInterfaceIFace()->popupSetAsCancelled(pPopup);
				gDLL->getInterfaceIFace()->selectCity(pCity);
				break;
			}

			if ( GC.getCurrentViewport()->isInViewport(pCity->getX(), pCity->getY(), GC.getViewportSelectionBorder()) )
			{
				gDLL->getInterfaceIFace()->lookAtCityOffset(pCity->getID());
			}
			else
			{
				GC.getCurrentViewport()->bringIntoView(pCity->getX_INLINE(), pCity->getY_INLINE(), NULL, true, true);
			}
		}
		break;

	case BUTTONPOPUP_RAZECITY:
	case BUTTONPOPUP_DISBANDCITY:
		{
			PlayerTypes ePlayer = GC.getGameINLINE().getActivePlayer();
			CvCity* pCity = GET_PLAYER(ePlayer).getCity(info.getData1());

			if (NULL == pCity || pCity->getOwnerINLINE() != ePlayer)
			{
				gDLL->getInterfaceIFace()->popupSetAsCancelled(pPopup);
				break;
			}

			if ( GC.getCurrentViewport()->isInViewport(pCity->getX(), pCity->getY(), GC.getViewportSelectionBorder()) )
			{
				gDLL->getInterfaceIFace()->lookAtCityOffset(pCity->getID());
			}
			else
			{
				GC.getCurrentViewport()->bringIntoView(pCity->getX_INLINE(), pCity->getY_INLINE(), NULL, true, true);
			}
		}
		break;

	case BUTTONPOPUP_CHANGECIVIC:
		if (!(GET_PLAYER(GC.getGameINLINE().getActivePlayer()).canRevolution(NULL)) || (GC.getGameINLINE().getGameState() == GAMESTATE_OVER))
		{
			gDLL->getInterfaceIFace()->popupSetAsCancelled(pPopup);
		}
		break;

	case BUTTONPOPUP_PYTHON:
	case BUTTONPOPUP_PYTHON_SCREEN:
		if (!info.getOnFocusPythonCallback().IsEmpty())
		{
			PYTHON_ACCESS_LOCK_SCOPE

			long iResult = 0;
			CyArgsList argsList;
			argsList.add(info.getData1());
			argsList.add(info.getData2());
			argsList.add(info.getData3());
			argsList.add(info.getFlags());
			argsList.add(info.getText());
			argsList.add(info.getOption1());
			argsList.add(info.getOption2());
			PYTHON_CALL_FUNCTION4(__FUNCTION__, PYScreensModule, info.getOnFocusPythonCallback(), argsList.makeFunctionArgs(), &iResult);
			if (0 != iResult)
			{
				gDLL->getInterfaceIFace()->popupSetAsCancelled(pPopup);
			}
		}
		break;

	}
}

// returns false if popup is not launched
bool CvDLLButtonPopup::launchButtonPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	bool bLaunched = false;

	//OutputDebugString("UI interaction - launch popup\n");
	GET_PLAYER(GC.getGameINLINE().getActivePlayer()).setTurnHadUIInteraction(true);

	switch (info.getButtonPopupType())
	{
	case BUTTONPOPUP_TEXT:
		bLaunched = launchTextPopup(pPopup, info);
		break;
	case BUTTONPOPUP_CHOOSEPRODUCTION:
		bLaunched = launchProductionPopup(pPopup, info);
		break;
	case BUTTONPOPUP_CHANGERELIGION:
		bLaunched = launchChangeReligionPopup(pPopup, info);
		break;
	case BUTTONPOPUP_CHOOSEELECTION:
		bLaunched = launchChooseElectionPopup(pPopup, info);
		break;
	case BUTTONPOPUP_DIPLOVOTE:
		bLaunched = launchDiploVotePopup(pPopup, info);
		break;
	case BUTTONPOPUP_RAZECITY:
		bLaunched = launchRazeCityPopup(pPopup, info);
		break;
	case BUTTONPOPUP_DISBANDCITY:
		bLaunched = launchDisbandCityPopup(pPopup, info);
		break;
	case BUTTONPOPUP_CHOOSETECH:
		bLaunched = launchChooseTechPopup(pPopup, info);
		break;
	case BUTTONPOPUP_CHANGECIVIC:
		bLaunched = launchChangeCivicsPopup(pPopup, info);
		break;
	case BUTTONPOPUP_ALARM:
		bLaunched = launchAlarmPopup(pPopup, info);
		break;
	case BUTTONPOPUP_DECLAREWARMOVE:
		bLaunched = launchDeclareWarMovePopup(pPopup, info);
		break;
	case BUTTONPOPUP_CONFIRMCOMMAND:
		bLaunched = launchConfirmCommandPopup(pPopup, info);
		break;
	case BUTTONPOPUP_LOADUNIT:
		bLaunched = launchLoadUnitPopup(pPopup, info);
		break;
	case BUTTONPOPUP_LEADUNIT:
		bLaunched = launchLeadUnitPopup(pPopup, info);
		break;
	case BUTTONPOPUP_DOESPIONAGE:
		bLaunched = launchDoEspionagePopup(pPopup, info);
		break;
	case BUTTONPOPUP_DOESPIONAGE_TARGET:
		bLaunched = launchDoEspionageTargetPopup(pPopup, info);
		break;
	case BUTTONPOPUP_MAIN_MENU:
		bLaunched = launchMainMenuPopup(pPopup, info);
		break;
	case BUTTONPOPUP_CONFIRM_MENU:
		bLaunched = launchConfirmMenu(pPopup, info);
		break;
	case BUTTONPOPUP_PYTHON_SCREEN:
		bLaunched = launchPythonScreen(pPopup, info);
		break;
	case BUTTONPOPUP_DEAL_CANCELED:
		bLaunched = launchCancelDeal(pPopup, info);
		break;
	case BUTTONPOPUP_PYTHON:
		bLaunched = launchPythonPopup(pPopup, info);
		break;
	case BUTTONPOPUP_DETAILS:
		bLaunched = launchDetailsPopup(pPopup, info);
		break;
	case BUTTONPOPUP_ADMIN:
		bLaunched = launchAdminPopup(pPopup, info);
		break;
	case BUTTONPOPUP_ADMIN_PASSWORD:
		bLaunched = launchAdminPasswordPopup(pPopup, info);
		break;
	case BUTTONPOPUP_EXTENDED_GAME:
		bLaunched = launchExtendedGamePopup(pPopup, info);
		break;
	case BUTTONPOPUP_DIPLOMACY:
		bLaunched = launchDiplomacyPopup(pPopup, info);
		break;
	case BUTTONPOPUP_ADDBUDDY:
		bLaunched = launchAddBuddyPopup(pPopup, info);
		break;
	case BUTTONPOPUP_FORCED_DISCONNECT:
		bLaunched = launchForcedDisconnectPopup(pPopup, info);
		break;
	case BUTTONPOPUP_PITBOSS_DISCONNECT:
		bLaunched = launchPitbossDisconnectPopup(pPopup, info);
		break;
	case BUTTONPOPUP_KICKED:
		bLaunched = launchKickedPopup(pPopup, info);
		break;
	case BUTTONPOPUP_VASSAL_DEMAND_TRIBUTE:
		bLaunched = launchVassalDemandTributePopup(pPopup, info);
		break;
	case BUTTONPOPUP_VASSAL_GRANT_TRIBUTE:
		bLaunched = launchVassalGrantTributePopup(pPopup, info);
		break;
	case BUTTONPOPUP_EVENT:
		bLaunched = launchEventPopup(pPopup, info);
		break;
	case BUTTONPOPUP_FREE_COLONY:
		bLaunched = launchFreeColonyPopup(pPopup, info);
		break;
	case BUTTONPOPUP_LAUNCH:
		bLaunched = launchLaunchPopup(pPopup, info);
		break;
	case BUTTONPOPUP_FOUND_RELIGION:
		bLaunched = launchFoundReligionPopup(pPopup, info);
		break;
	case BUTTONPOPUP_GET_SAVE_FORMAT:
		bLaunched = launchGetSaveFormatPopup(pPopup, info);
		break;
	case BUTTONPOPUP_SAVE_INFO_LOST:
		bLaunched = launchGetSaveInfoLostPopup(pPopup, info);
		break;
	case BUTTONPOPUP_MODIFIER_RECALCULATION:
		bLaunched = launchModifierRecalculationPopup(pPopup, info);
		break;
/************************************************************************************************/
/* Afforess	                  Start		 09/18/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	case BUTTONPOPUP_INVASION:
		bLaunched = invasionPopup(pPopup, info);
		break;
	case BUTTONPOPUP_SELECT_UNIT:
		bLaunched = launchSelectShadowUnitPopup(pPopup, info);
		break;
	case BUTTONPOPUP_SELECT_DISCOVERY_TECH:
		bLaunched = launchSelectDiscoveryTechPopup(pPopup, info);
		break;
	case BUTTONPOPUP_FIELD_COMMANDER_TUTORIAL:
		bLaunched = launchFieldCommanderTutorial(pPopup, info);
		break;
	case BUTTONPOPUP_DARK_AGE:
		bLaunched = launchDarkAgePopup(pPopup, info);
		break;
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
	//ls612: Viewport Goto
	case BUTTONPOPUP_GOTO_CITY:
		bLaunched = launchGoToCityPopup(pPopup, info);
		break;
/************************************************************************************************/
/* Great Diplomat MOD               START                                                  		*/
/************************************************************************************************/
	case BUTTONPOPUP_UNIT_FORCE_PEACE:
		bLaunched = launchUnitForcePeacePopup(pPopup, info);
		break;
/************************************************************************************************/
/* Great Diplomat MOD               END                                                  		*/
/************************************************************************************************/
	case BUTTONPOPUP_CONFIRM_MENU_KI:
		bLaunched = launchConfirmMenuKI(pPopup, info);
		break;

	default:
		FAssert(false);
		break;
	}
	return (bLaunched);
}


bool CvDLLButtonPopup::launchTextPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, info.getText());
	gDLL->getInterfaceIFace()->popupLaunch(pPopup, true, POPUPSTATE_IMMEDIATE);
	return (true);
}


bool CvDLLButtonPopup::launchProductionPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	CvCity* pCity = GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCity(info.getData1());
	if (NULL == pCity || pCity->isProductionAutomated())
	{
		return (false);
	}
	FAssertMsg(pCity->getOwnerINLINE() == GC.getGameINLINE().getActivePlayer(), "City must belong to Active Player");

	UnitTypes eTrainUnit = NO_UNIT;
	BuildingTypes eConstructBuilding = NO_BUILDING;
	ProjectTypes eCreateProject = NO_PROJECT;
	switch (info.getData2())
	{
	case (ORDER_TRAIN):
		eTrainUnit = (UnitTypes)info.getData3();
		break;
	case (ORDER_CONSTRUCT):
		eConstructBuilding = (BuildingTypes)info.getData3();
		break;
	case (ORDER_CREATE):
		eCreateProject = (ProjectTypes)info.getData3();
		break;
	default:
		break;
	}
	CvWString szBuffer;
	CvString szArtFilename;
	const bool bFinish = info.getOption1();

	if (eTrainUnit != NO_UNIT)
	{
		if (bFinish)
		{
			szBuffer = gDLL->getText(((isLimitedUnitClass((UnitClassTypes)(GC.getUnitInfo(eTrainUnit).getUnitClassType()))) ? "TXT_KEY_POPUP_TRAINED_WORK_ON_NEXT_LIMITED" : "TXT_KEY_POPUP_TRAINED_WORK_ON_NEXT"), GC.getUnitInfo(eTrainUnit).getTextKeyWide(), pCity->getNameKey());
		}
		else
		{
			szBuffer = gDLL->getText(((isLimitedUnitClass((UnitClassTypes)(GC.getUnitInfo(eTrainUnit).getUnitClassType()))) ? "TXT_KEY_POPUP_CANNOT_TRAIN_WORK_NEXT_LIMITED" : "TXT_KEY_POPUP_CANNOT_TRAIN_WORK_NEXT"), GC.getUnitInfo(eTrainUnit).getTextKeyWide(), pCity->getNameKey());
		}
		szArtFilename = GET_PLAYER(pCity->getOwnerINLINE()).getUnitButton(eTrainUnit);
	}
	else if (eConstructBuilding != NO_BUILDING)
	{
		if (bFinish)
		{
			szBuffer = gDLL->getText(((isLimitedWonderClass((BuildingClassTypes)(GC.getBuildingInfo(eConstructBuilding).getBuildingClassType()))) ? "TXT_KEY_POPUP_CONSTRUCTED_WORK_ON_NEXT_LIMITED" : "TXT_KEY_POPUP_CONSTRUCTED_WORK_ON_NEXT"), GC.getBuildingInfo(eConstructBuilding).getTextKeyWide(), pCity->getNameKey());
		}
		else
		{
			szBuffer = gDLL->getText(((isLimitedWonderClass((BuildingClassTypes)(GC.getBuildingInfo(eConstructBuilding).getBuildingClassType()))) ? "TXT_KEY_POPUP_CANNOT_CONSTRUCT_WORK_NEXT_LIMITED" : "TXT_KEY_POPUP_CANNOT_CONSTRUCT_WORK_NEXT"), GC.getBuildingInfo(eConstructBuilding).getTextKeyWide(), pCity->getNameKey());
		}
		szArtFilename = GC.getBuildingInfo(eConstructBuilding).getButton();
	}
	else if (eCreateProject != NO_PROJECT)
	{
		if (bFinish)
		{
			if(GC.getProjectInfo(eCreateProject).isSpaceship())
				szBuffer = gDLL->getText("TXT_KEY_POPUP_CREATED_WORK_ON_NEXT_SPACESHIP", GC.getProjectInfo(eCreateProject).getTextKeyWide(), pCity->getNameKey());
			else
				szBuffer = gDLL->getText(((isLimitedProject(eCreateProject)) ? "TXT_KEY_POPUP_CREATED_WORK_ON_NEXT_LIMITED" : "TXT_KEY_POPUP_CREATED_WORK_ON_NEXT"), GC.getProjectInfo(eCreateProject).getTextKeyWide(), pCity->getNameKey());
		}
		else
		{
			szBuffer = gDLL->getText(((isLimitedProject(eCreateProject)) ? "TXT_KEY_POPUP_CANNOT_CREATE_WORK_NEXT_LIMITED" : "TXT_KEY_POPUP_CANNOT_CREATE_WORK_NEXT"), GC.getProjectInfo(eCreateProject).getTextKeyWide(), pCity->getNameKey());
		}
		szArtFilename = GC.getProjectInfo(eCreateProject).getButton();
	}
	else
	{
		szBuffer = gDLL->getText("TXT_KEY_POPUP_WHAT_TO_BUILD", pCity->getNameKey());
		szArtFilename = ARTFILEMGR.getInterfaceArtInfo("INTERFACE_POPUPBUTTON_PRODUCTION")->getPath();
	}

	gDLL->getInterfaceIFace()->popupSetHeaderString(pPopup, szBuffer, DLL_FONT_LEFT_JUSTIFY);

	int iExamineCityID = GC.getNumUnitInfos();

	iExamineCityID = std::max(iExamineCityID, GC.getNumBuildingInfos());
	iExamineCityID = std::max(iExamineCityID, GC.getNumProjectInfos());
	iExamineCityID = std::max(iExamineCityID, GC.getNumProcessInfos());

// BUG - Zoom City Details - start
	if (getBugOptionBOOL("MiscHover__CDAZoomCityDetails", true, "BUG_CDA_ZOOM_CITY_DETAILS"))
	{
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_EXAMINE_CITY").c_str(), ARTFILEMGR.getInterfaceArtInfo("INTERFACE_BUTTONS_CITYSELECTION")->getPath(), iExamineCityID, WIDGET_ZOOM_CITY, GC.getGameINLINE().getActivePlayer(), info.getData1(), true, POPUP_LAYOUT_STRETCH, DLL_FONT_LEFT_JUSTIFY);
	}
	else
	{
		// unchanged
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_EXAMINE_CITY").c_str(), ARTFILEMGR.getInterfaceArtInfo("INTERFACE_BUTTONS_CITYSELECTION")->getPath(), iExamineCityID, WIDGET_GENERAL, -1, -1, true, POPUP_LAYOUT_STRETCH, DLL_FONT_LEFT_JUSTIFY);
	}
// BUG - Zoom City Details - end

	UnitTypes eProductionUnit = pCity->getProductionUnit();
	BuildingTypes eProductionBuilding = pCity->getProductionBuilding();
	ProjectTypes eProductionProject = pCity->getProductionProject();
	ProcessTypes eProductionProcess = pCity->getProductionProcess();

	AdvisorTypes eBuildingAdvisor = NO_ADVISOR;

	if (eProductionBuilding != NO_BUILDING)
	{
		eBuildingAdvisor = (AdvisorTypes)(GC.getBuildingInfo(eProductionBuilding).getAdvisorType());
	}

	if (eProductionUnit == NO_UNIT)
	{
		int iDummyValue;
		CvUnitSelectionCriteria criteria;
		criteria.m_eIgnoreAdvisor = eBuildingAdvisor;

		eProductionUnit = pCity->AI_bestUnit(iDummyValue, -1, NULL, true, NULL, false, false, &criteria);
	}

	AdvisorTypes eUnitAdvisor = NO_ADVISOR;

	if (eProductionUnit != NO_UNIT)
	{
		eUnitAdvisor = (AdvisorTypes)(GC.getUnitInfo(eProductionUnit).getAdvisorType());
	}

	if (eProductionBuilding == NO_BUILDING)
	{
		eProductionBuilding = pCity->AI_bestBuilding(0, 50, true, eUnitAdvisor);

		if (eProductionBuilding != NO_BUILDING)
		{
			eBuildingAdvisor = (AdvisorTypes)(GC.getBuildingInfo(eProductionBuilding).getAdvisorType());
		}
	}

	int iNumBuilds = 0;
	if (eUnitAdvisor != NO_ADVISOR)
	{
		int iTurns = pCity->getProductionTurnsLeft(eProductionUnit, 0);
		szBuffer = gDLL->getText("TXT_KEY_POPUP_RECOMMENDED", GC.getUnitInfo(eProductionUnit).getTextKeyWide(), iTurns, GC.getAdvisorInfo(eUnitAdvisor).getTextKeyWide());
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, GET_PLAYER(pCity->getOwnerINLINE()).getUnitButton(eProductionUnit), GC.getUnitInfo(eProductionUnit).getUnitClassType(), WIDGET_TRAIN, GC.getUnitInfo(eProductionUnit).getUnitClassType(), pCity->getID(), true, POPUP_LAYOUT_STRETCH, DLL_FONT_LEFT_JUSTIFY );
		iNumBuilds++;
	}

	if (eBuildingAdvisor != NO_ADVISOR)
	{
		int iTurns = pCity->getProductionTurnsLeft(eProductionBuilding, 0);
		szBuffer = gDLL->getText("TXT_KEY_POPUP_RECOMMENDED", GC.getBuildingInfo(eProductionBuilding).getTextKeyWide(), iTurns, GC.getAdvisorInfo(eBuildingAdvisor).getTextKeyWide());
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, GC.getBuildingInfo(eProductionBuilding).getButton(), GC.getBuildingInfo(eProductionBuilding).getBuildingClassType(), WIDGET_CONSTRUCT, GC.getBuildingInfo(eProductionBuilding).getBuildingClassType(), pCity->getID(), true, POPUP_LAYOUT_STRETCH, DLL_FONT_LEFT_JUSTIFY );
		iNumBuilds++;
	}

	if (eProductionProject != NO_PROJECT)
	{
		int iTurns = pCity->getProductionTurnsLeft(eProductionProject, 0);
		szBuffer = gDLL->getText("TXT_KEY_POPUP_RECOMMENDED_NO_ADV", GC.getProjectInfo(eProductionProject).getTextKeyWide(), iTurns);
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, GC.getProjectInfo(eProductionProject).getButton(), eProductionProject, WIDGET_CREATE, eProductionProject, pCity->getID(), true, POPUP_LAYOUT_STRETCH, DLL_FONT_LEFT_JUSTIFY );
		iNumBuilds++;
	}

	if (eProductionProcess != NO_PROCESS)
	{
		szBuffer = gDLL->getText("TXT_KEY_POPUP_RECOMMENDED_NO_ADV_OR_TURNS", GC.getProcessInfo(eProductionProcess).getTextKeyWide());
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, GC.getProcessInfo(eProductionProcess).getButton(), eProductionProcess, WIDGET_MAINTAIN, eProductionProcess, pCity->getID(), true, POPUP_LAYOUT_STRETCH, DLL_FONT_LEFT_JUSTIFY );
		iNumBuilds++;
	}

/************************************************************************************************/
/* Afforess	/ Calvitix                  Start		 09/12/10                                   */
/*                                                                                              */
/*  Sort Units, Regular Buildings and Wonders                                                          */
/************************************************************************************************/

	std::vector<CvWString> kUnitsSorted;

	//Get the Unit List before displaying it
	for (int iI = 0; iI < GC.getNumUnitClassInfos(); iI++)
	{
		UnitTypes eLoopUnit = (UnitTypes)GC.getCivilizationInfo(pCity->getCivilizationType()).getCivilizationUnits(iI);

		if (eLoopUnit != NO_UNIT)
		{
			if (eLoopUnit != eProductionUnit)
			{
				if (pCity->canTrain(eLoopUnit))
				{
					int iTurns = pCity->getProductionTurnsLeft(eLoopUnit, 0);
					szBuffer.Format(L"%d %s (%d) - [%d]", GC.getUnitInfo(eLoopUnit).getUnitCombatType(), GC.getUnitInfo(eLoopUnit).getDescription(), iTurns, iI);
					kUnitsSorted.push_back(szBuffer);
					//gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, GET_PLAYER(pCity->getOwnerINLINE()).getUnitButton(eLoopUnit), iI, WIDGET_TRAIN, iI, pCity->getID(), true, POPUP_LAYOUT_STRETCH, DLL_FONT_LEFT_JUSTIFY );
					//iNumBuilds++;
				}
			}
		}
	}

	// sort the Strings List
	std::sort(kUnitsSorted.begin(), kUnitsSorted.end());

	// retrieve the Index and generate the list
	int inumUnits = kUnitsSorted.size();
	for (int iJ = 0; iJ < inumUnits; iJ++)
	{
	    // retrieve the Index
		int iOriginalIndex = -1;
		int iBeginIndex = -1;
		int iEndIndex = -1;
		std::wstring szTempBuffer;
		szTempBuffer = kUnitsSorted[iJ];
		iBeginIndex = szTempBuffer.find('[');
		assert(iBeginIndex > 4);
		iEndIndex = szTempBuffer.find(']');
		assert(iEndIndex > 6);
		szTempBuffer = szTempBuffer.substr(iBeginIndex+1,iEndIndex-(iBeginIndex+1));
		iOriginalIndex = _wtoi(szTempBuffer.c_str());
		assert(iOriginalIndex > 0);
		UnitTypes eLoopUnit = (UnitTypes)GC.getCivilizationInfo(pCity->getCivilizationType()).getCivilizationUnits(iOriginalIndex);

		if (eLoopUnit != NO_UNIT)
		{
			if (eLoopUnit != eProductionUnit)
			{
				if (pCity->canTrain(eLoopUnit))
				{
					int iTurns = pCity->getProductionTurnsLeft(eLoopUnit, 0);
					szBuffer.Format(L"%s (%d)", GC.getUnitInfo(eLoopUnit).getDescription(), iTurns);
					gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, GET_PLAYER(pCity->getOwnerINLINE()).getUnitButton(eLoopUnit), iOriginalIndex, WIDGET_TRAIN, iOriginalIndex, pCity->getID(), true, POPUP_LAYOUT_STRETCH, DLL_FONT_LEFT_JUSTIFY );
					iNumBuilds++;
				}
			}
		}
	}


	std::vector<CvWString> kBuildingsSorted;
	std::vector<CvWString> kNatWondersSorted;
	std::vector<CvWString> kWondersSorted;

	for (int iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
	{
		BuildingTypes eLoopBuilding = ((BuildingTypes)(GC.getCivilizationInfo(pCity->getCivilizationType()).getCivilizationBuildings(iI)));

		if (eLoopBuilding != NO_BUILDING)
		{
			if (eLoopBuilding != eProductionBuilding)
			{
				if (pCity->canConstruct(eLoopBuilding))
				{
					int iTurns = pCity->getProductionTurnsLeft(eLoopBuilding, 0);
					szBuffer.Format(L"%s (%d) - [%d]", GC.getBuildingInfo(eLoopBuilding).getDescription(), iTurns, iI);
					kBuildingsSorted.push_back(szBuffer);
					//gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, GC.getBuildingInfo(eLoopBuilding).getButton(), iI, WIDGET_CONSTRUCT, iI, pCity->getID(), true, POPUP_LAYOUT_STRETCH, DLL_FONT_LEFT_JUSTIFY);
					//iNumBuilds++;
				}
			}
		}
	}

	// sort the Strings List
	std::sort(kBuildingsSorted.begin(), kBuildingsSorted.end());

	// retrieve the Index and generate the list for standard Buildings
	int inumBuildings = kBuildingsSorted.size();
	for (int iJ = 0; iJ < inumBuildings; iJ++)
	{
	    // retrieve the Index 'hidden' in the String between [ ]
		int iOriginalIndex = -1;
		int iBeginIndex = -1;
		int iEndIndex = -1;
		std::wstring szTempBuffer;
		std::wstring szOriginalBuffer;
		szTempBuffer = kBuildingsSorted[iJ];
		iBeginIndex = szTempBuffer.find('[');
		assert(iBeginIndex > 4);
		iEndIndex = szTempBuffer.find(']');
		assert(iEndIndex > 6);
		szOriginalBuffer = szTempBuffer;
		//regenerate the String to display
		szBuffer = szTempBuffer.substr(0,iBeginIndex-3);
		szTempBuffer = szTempBuffer.substr(iBeginIndex+1,iEndIndex-(iBeginIndex+1));
		iOriginalIndex = _wtoi(szTempBuffer.c_str());
		assert(iOriginalIndex >= 0);
		BuildingTypes eLoopBuilding = ((BuildingTypes)(GC.getCivilizationInfo(pCity->getCivilizationType()).getCivilizationBuildings(iOriginalIndex)));

		if (eLoopBuilding != NO_BUILDING)
		{
			if (eLoopBuilding != eProductionBuilding)
			{
				if (pCity->canConstruct(eLoopBuilding))
				{
					//Filter the standard buildings (displayed first) and the Wonders, that will be displayed later
					if (isNationalWonderClass((BuildingClassTypes)(GC.getBuildingInfo(eLoopBuilding).getBuildingClassType())))
					{
						kNatWondersSorted.push_back(szOriginalBuffer);
					}
					else if (isLimitedWonderClass((BuildingClassTypes)(GC.getBuildingInfo(eLoopBuilding).getBuildingClassType())) && !isNationalWonderClass((BuildingClassTypes)(GC.getBuildingInfo(eLoopBuilding).getBuildingClassType())))
					{
						kWondersSorted.push_back(szOriginalBuffer);
					}
					else
					{
					//int iTurns = pCity->getProductionTurnsLeft(eLoopBuilding, 0);
					//szBuffer.Format(L"%s (%d)", GC.getBuildingInfo(eLoopBuilding).getDescription(), iTurns);
					gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, GC.getBuildingInfo(eLoopBuilding).getButton(), iOriginalIndex, WIDGET_CONSTRUCT, iOriginalIndex, pCity->getID(), true, POPUP_LAYOUT_STRETCH, DLL_FONT_LEFT_JUSTIFY);
					iNumBuilds++;
					}
				}
			}
		}
	}

	// retrieve the Index and generate the list for National Wonders
	int inumNatWonders = kNatWondersSorted.size();
	for (int iJ = 0; iJ < inumNatWonders; iJ++)
	{
	    // retrieve the Index
		int iOriginalIndex = -1;
		int iBeginIndex = -1;
		int iEndIndex = -1;
		std::wstring szTempBuffer;
		szTempBuffer = kNatWondersSorted[iJ];
		iBeginIndex = szTempBuffer.find('[');
		assert(iBeginIndex > 4);
		iEndIndex = szTempBuffer.find(']');
		assert(iEndIndex > 6);
		szBuffer = szTempBuffer.substr(0,iBeginIndex-3);
		szTempBuffer = szTempBuffer.substr(iBeginIndex+1,iEndIndex-(iBeginIndex+1));
		iOriginalIndex = _wtoi(szTempBuffer.c_str());
		assert(iOriginalIndex >= 0);
		BuildingTypes eLoopBuilding = ((BuildingTypes)(GC.getCivilizationInfo(pCity->getCivilizationType()).getCivilizationBuildings(iOriginalIndex)));

		if (eLoopBuilding != NO_BUILDING)
		{
			if (eLoopBuilding != eProductionBuilding)
			{
				if (pCity->canConstruct(eLoopBuilding))
				{
					//int iTurns = pCity->getProductionTurnsLeft(eLoopBuilding, 0);
					//szBuffer.Format(L"%s (%d)", GC.getBuildingInfo(eLoopBuilding).getDescription(), iTurns);
					gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, GC.getBuildingInfo(eLoopBuilding).getButton(), iOriginalIndex, WIDGET_CONSTRUCT, iOriginalIndex, pCity->getID(), true, POPUP_LAYOUT_STRETCH, DLL_FONT_LEFT_JUSTIFY);
					iNumBuilds++;
				}
			}
		}
	}

	// retrieve the Index and generate the list for Wonders
	int inumWonders = kWondersSorted.size();
	for (int iJ = 0; iJ < inumWonders; iJ++)
	{
	    // retrieve the Index
		int iOriginalIndex = -1;
		int iBeginIndex = -1;
		int iEndIndex = -1;
		std::wstring szTempBuffer;
		szTempBuffer = kWondersSorted[iJ];
		iBeginIndex = szTempBuffer.find('[');
		assert(iBeginIndex > 4);
		iEndIndex = szTempBuffer.find(']');
		assert(iEndIndex > 6);
		szBuffer = szTempBuffer.substr(0,iBeginIndex-3);
		szTempBuffer = szTempBuffer.substr(iBeginIndex+1,iEndIndex-(iBeginIndex+1));
		iOriginalIndex = _wtoi(szTempBuffer.c_str());
		assert(iOriginalIndex >= 0);
		BuildingTypes eLoopBuilding = ((BuildingTypes)(GC.getCivilizationInfo(pCity->getCivilizationType()).getCivilizationBuildings(iOriginalIndex)));

		if (eLoopBuilding != NO_BUILDING)
		{
			if (eLoopBuilding != eProductionBuilding)
			{
				if (pCity->canConstruct(eLoopBuilding))
				{
					//int iTurns = pCity->getProductionTurnsLeft(eLoopBuilding, 0);
					//szBuffer.Format(L"%s (%d)", GC.getBuildingInfo(eLoopBuilding).getDescription(), iTurns);
					gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, GC.getBuildingInfo(eLoopBuilding).getButton(), iOriginalIndex, WIDGET_CONSTRUCT, iOriginalIndex, pCity->getID(), true, POPUP_LAYOUT_STRETCH, DLL_FONT_LEFT_JUSTIFY);
					iNumBuilds++;
				}
			}
		}
	}


	std::vector<CvWString> kProjectsSorted;

	for (int iI = 0; iI < GC.getNumProjectInfos(); iI++)
	{
		if (iI != eProductionProject)
		{
			if (pCity->canCreate((ProjectTypes)iI))
			{
				int iTurns = pCity->getProductionTurnsLeft((ProjectTypes)iI, 0);
				szBuffer.Format(L"%s (%d) - [%d]", GC.getProjectInfo((ProjectTypes) iI).getDescription(), iTurns, iI);
				kProjectsSorted.push_back(szBuffer);
				//gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, GC.getProjectInfo((ProjectTypes) iI).getButton(), iI, WIDGET_CREATE, iI, pCity->getID(), true, POPUP_LAYOUT_STRETCH, DLL_FONT_LEFT_JUSTIFY);
				//iNumBuilds++;
			}
		}
	}

	// sort the Strings List
	std::sort(kProjectsSorted.begin(), kProjectsSorted.end());

	// retrieve the Index and generate the list
	int inumProjects = kProjectsSorted.size();
	for (int iJ = 0; iJ < inumProjects; iJ++)
	{
	    // retrieve the Index
		int iOriginalIndex = -1;
		int iBeginIndex = -1;
		int iEndIndex = -1;
		std::wstring szTempBuffer;
		szTempBuffer = kProjectsSorted[iJ];
		iBeginIndex = szTempBuffer.find('[');
		assert(iBeginIndex > 4);
		iEndIndex = szTempBuffer.find(']');
		assert(iEndIndex > 6);
		szBuffer = szTempBuffer.substr(0,iBeginIndex-3);
		szTempBuffer = szTempBuffer.substr(iBeginIndex+1,iEndIndex-(iBeginIndex+1));
		iOriginalIndex = _wtoi(szTempBuffer.c_str());
		assert(iOriginalIndex >= 0);
		//BuildingTypes eLoopBuilding = ((BuildingTypes)(GC.getCivilizationInfo(pCity->getCivilizationType()).getCivilizationBuildings(iOriginalIndex)));

		if (iOriginalIndex != eProductionProject)
		{
			if (pCity->canCreate((ProjectTypes)iOriginalIndex))
			{
				//int iTurns = pCity->getProductionTurnsLeft((ProjectTypes)iI, 0);
				//szBuffer.Format(L"%s (%d)", GC.getProjectInfo((ProjectTypes) iI).getDescription(), iTurns);
				gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, GC.getProjectInfo((ProjectTypes) iOriginalIndex).getButton(), iOriginalIndex, WIDGET_CREATE, iOriginalIndex, pCity->getID(), true, POPUP_LAYOUT_STRETCH, DLL_FONT_LEFT_JUSTIFY);
				iNumBuilds++;
			}
		}
	}

/************************************************************************************************/
/* Afforess	/ Calvitix                     END                                                            */
/************************************************************************************************/


	for (int iI = 0; iI < GC.getNumProcessInfos(); iI++)
	{
		if (iI != eProductionProcess)
		{
			if (pCity->canMaintain((ProcessTypes)iI))
			{
				gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, GC.getProcessInfo((ProcessTypes) iI).getDescription(), GC.getProcessInfo((ProcessTypes) iI).getButton(), iI, WIDGET_MAINTAIN, iI, pCity->getID(), true, POPUP_LAYOUT_STRETCH, DLL_FONT_LEFT_JUSTIFY);
				iNumBuilds++;
			}
		}
	}

	if (0 == iNumBuilds)
	{
		// city cannot build anything, so don't show popup after all
		return (false);
	}

	gDLL->getInterfaceIFace()->popupSetPopupType(pPopup, POPUPEVENT_PRODUCTION, szArtFilename);

	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_MINIMIZED, 252);

	switch (info.getData2())
	{
	case ORDER_TRAIN:
		gDLL->getInterfaceIFace()->playGeneralSound(GC.getUnitInfo((UnitTypes)info.getData3()).getArtInfo(0, GET_PLAYER(pCity->getOwner()).getCurrentEra(), NO_UNIT_ARTSTYLE)->getTrainSound());
		break;

	case ORDER_CONSTRUCT:
		gDLL->getInterfaceIFace()->playGeneralSound(GC.getBuildingInfo((BuildingTypes)info.getData3()).getConstructSound());
		break;

	case ORDER_CREATE:
		gDLL->getInterfaceIFace()->playGeneralSound(GC.getProjectInfo((ProjectTypes)info.getData3()).getCreateSound());
		break;

	default:
		break;
	}

	return (true);
}


bool CvDLLButtonPopup::launchChangeReligionPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	CvWString szTemp;
	ReligionTypes eReligion = (ReligionTypes)info.getData1();

	if (NO_RELIGION == eReligion)
	{
		FAssert(false);
		return (false);
	}

	CvPlayer& activePlayer = GET_PLAYER(GC.getGameINLINE().getActivePlayer());
	if (!activePlayer.canConvert(eReligion))
	{
		return (false);
	}

	CvWString szBuffer;
	szBuffer = gDLL->getText("TXT_KEY_POPUP_RELIGION_SPREAD", GC.getReligionInfo(eReligion).getTextKeyWide());
	if (activePlayer.getStateReligionHappiness() != 0)
	{
		if (activePlayer.getStateReligionHappiness() > 0)
		{
			szBuffer += gDLL->getText("TXT_KEY_POPUP_CONVERTING_EFFECTS",
				activePlayer.getStateReligionHappiness(), gDLL->getSymbolID(HAPPY_CHAR), GC.getReligionInfo(eReligion).getChar());
		}
		else
		{
			szBuffer += gDLL->getText("TXT_KEY_POPUP_CONVERTING_EFFECTS",
				-activePlayer.getStateReligionHappiness(), gDLL->getSymbolID(UNHAPPY_CHAR), GC.getReligionInfo(eReligion).getChar());
		}
	}
	szBuffer += gDLL->getText("TXT_KEY_POPUP_LIKE_TO_CONVERT");
	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, szBuffer);

	if (activePlayer.getStateReligionHealth() != 0)
	{
		if (activePlayer.getStateReligionHealth() > 0)
		{
			szBuffer += gDLL->getText("TXT_KEY_POPUP_CONVERTING_EFFECTS",
				activePlayer.getStateReligionHealth(), gDLL->getSymbolID(HEALTHY_CHAR), GC.getReligionInfo(eReligion).getChar());
		}
		else
		{
			szBuffer += gDLL->getText("TXT_KEY_POPUP_CONVERTING_EFFECTS",
				-activePlayer.getStateReligionHealth(), gDLL->getSymbolID(UNHEALTHY_CHAR), GC.getReligionInfo(eReligion).getChar());
		}
	}
	szBuffer += gDLL->getText("TXT_KEY_POPUP_LIKE_TO_CONVERT");
	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, szBuffer);

	szBuffer = gDLL->getText("TXT_KEY_POPUP_CONVERT_RELIGION");
	int iAnarchyLength = activePlayer.getReligionAnarchyLength();
	if (iAnarchyLength > 0)
	{
		szBuffer += gDLL->getText("TXT_KEY_POPUP_TURNS_OF_ANARCHY", iAnarchyLength);
	}
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, NULL, 0);
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_NO_CONVERSION").c_str());
	gDLL->getInterfaceIFace()->popupSetPopupType(pPopup, POPUPEVENT_RELIGION, ARTFILEMGR.getInterfaceArtInfo("INTERFACE_POPUPBUTTON_RELIGION")->getPath());
	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_MINIMIZED);
	return (true);
}


bool CvDLLButtonPopup::launchChooseElectionPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	VoteSelectionData* pVoteSelectionData = GC.getGameINLINE().getVoteSelection(info.getData1());
	if (NULL == pVoteSelectionData)
	{
		return false;
	}

	VoteSourceTypes eVoteSource = pVoteSelectionData->eVoteSource;

	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, GC.getVoteSourceInfo(eVoteSource).getPopupText());

	for (int iI = 0; iI < (int)pVoteSelectionData->aVoteOptions.size(); ++iI)
	{
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, pVoteSelectionData->aVoteOptions[iI].szText, NULL, iI, WIDGET_GENERAL);
	}

	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_NONE").c_str(), NULL, GC.getNumVoteInfos()+100, WIDGET_GENERAL);  //45deg: added that +100 to make sure that "None" option is selectable, it didn't work otherwise and another resolution is chosen when clicking "None"
	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);
	return (true);
}


bool CvDLLButtonPopup::launchDiploVotePopup(CvPopup* pPopup, CvPopupInfo &info)
{
	VoteTriggeredData* pVoteTriggered = GC.getGameINLINE().getVoteTriggered(info.getData1());
	if (NULL == pVoteTriggered)
	{
		FAssert(false);
		return false;
	}

	VoteTypes eVote = pVoteTriggered->kVoteOption.eVote;
	VoteSourceTypes eVoteSource = pVoteTriggered->eVoteSource;

	TeamTypes eVassalOfTeam = NO_TEAM;
	bool bEligible = false;

	gDLL->getInterfaceIFace()->popupSetHeaderString(pPopup, GC.getVoteSourceInfo(eVoteSource).getDescription());
	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, pVoteTriggered->kVoteOption.szText);
	if (GC.getGameINLINE().isTeamVote(eVote))
	{
		for (int iI = 0; iI < MAX_CIV_TEAMS; iI++)
		{
			if (GET_TEAM((TeamTypes)iI).isAlive())
			{
				if (GC.getGameINLINE().isTeamVoteEligible((TeamTypes)iI, eVoteSource))
				{
					if (GET_TEAM(GC.getGameINLINE().getActiveTeam()).isVassal((TeamTypes)iI))
					{
						eVassalOfTeam = (TeamTypes)iI;
						break;
					}
				}
			}
		}

		for (int iI = 0; iI < MAX_CIV_TEAMS; iI++)
		{
			if (GET_TEAM((TeamTypes)iI).isAlive())
			{
				if (GC.getGameINLINE().isTeamVoteEligible((TeamTypes)iI, eVoteSource))
				{
					if (eVassalOfTeam == NO_TEAM || eVassalOfTeam == iI || iI == GC.getGameINLINE().getActiveTeam())
					{
						CvWString szBuffer;
						szBuffer = GET_PLAYER(GET_TEAM((TeamTypes)iI).getLeaderID()).getCivilizationShortDescription();
						szBuffer += gDLL->getText(" - ");
						szBuffer += GET_TEAM((TeamTypes)iI).getName().GetCString();
						gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, NULL, iI, WIDGET_GENERAL);
						// GET_PLAYER(GET_TEAM((TeamTypes)iI).getLeaderID()).getCivilizationShortDescription()
						bEligible = true;
					}
				}
			}
		}
	}
	else
	{
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_YES").c_str(), NULL, PLAYER_VOTE_YES, WIDGET_GENERAL);
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_NO").c_str(), NULL, PLAYER_VOTE_NO, WIDGET_GENERAL);

		if (GET_PLAYER(GC.getGameINLINE().getActivePlayer()).canDefyResolution(eVoteSource, pVoteTriggered->kVoteOption))
		{
			gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_VOTE_NEVER").c_str(), NULL, PLAYER_VOTE_NEVER, WIDGET_GENERAL);
		}
		bEligible = true;
	}

	if (eVassalOfTeam == NO_TEAM || !bEligible)
	{
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_ABSTAIN").c_str(), NULL, PLAYER_VOTE_ABSTAIN, WIDGET_GENERAL);
	}

	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);
	return (true);
}



bool CvDLLButtonPopup::launchRazeCityPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	CvPlayer& player = GET_PLAYER(GC.getGameINLINE().getActivePlayer());

	CvCity* pNewCity = player.getCity(info.getData1());
	if (NULL == pNewCity)
	{
		FAssert(false);
		return (false);
	}

	if (0 != GC.getDefineINT("PLAYER_ALWAYS_RAZES_CITIES"))
	{
		player.raze(pNewCity);
		return false;
	}

	PlayerTypes eHighestCulturePlayer = (PlayerTypes)info.getData2();

	int iCaptureGold = info.getData3();
	bool bRaze = player.canRaze(pNewCity);
	bool bGift = ((eHighestCulturePlayer != NO_PLAYER)
		&& (eHighestCulturePlayer != player.getID())
		&& ((player.getTeam() == GET_PLAYER(eHighestCulturePlayer).getTeam()) || GET_TEAM(player.getTeam()).isOpenBorders(GET_PLAYER(eHighestCulturePlayer).getTeam()) || GET_TEAM(GET_PLAYER(eHighestCulturePlayer).getTeam()).isVassal(player.getTeam())));

	bRaze = player.canRaze(pNewCity);

	CvWString szBuffer;
	if (iCaptureGold > 0)
	{
		szBuffer = gDLL->getText("TXT_KEY_POPUP_GOLD_CITY_CAPTURE", iCaptureGold, pNewCity->getNameKey());
	}
	else
	{
		szBuffer = gDLL->getText("TXT_KEY_POPUP_CITY_CAPTURE_KEEP", pNewCity->getNameKey());
	}
	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, szBuffer);
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_KEEP_CAPTURED_CITY").c_str(), NULL, 0, WIDGET_GENERAL);

	if (bRaze)
	{
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_RAZE_CAPTURED_CITY").c_str(), NULL, 1, WIDGET_GENERAL);

		int iRazeAndFortifyCost = player.getRazeAndFortifyCost(pNewCity);
		if (iRazeAndFortifyCost > 0 && player.getGold() > iRazeAndFortifyCost)
		{
			gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_RAZE_AND_FORTIFY_CAPTURED_CITY", iRazeAndFortifyCost).c_str(), NULL, 4, WIDGET_GENERAL);
		}
	}
	if (bGift)
	{
		szBuffer = gDLL->getText("TXT_KEY_POPUP_RETURN_ALLIED_CITY", GET_PLAYER(eHighestCulturePlayer).getCivilizationDescriptionKey());
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, NULL, 2, WIDGET_GENERAL, 2, eHighestCulturePlayer);
	}
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_CITY_WARNING_ANSWER3").c_str(), NULL, 3, WIDGET_GENERAL, -1, -1);
	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);
	gDLL->getInterfaceIFace()->playGeneralSound("AS2D_CITYCAPTURE");

	return (true);
}

bool CvDLLButtonPopup::launchDisbandCityPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	CvPlayer& player = GET_PLAYER(GC.getGameINLINE().getActivePlayer());

	CvCity* pNewCity = player.getCity(info.getData1());
	if (NULL == pNewCity)
	{
		FAssert(false);
		return (false);
	}

	CvWString szBuffer;
	szBuffer = gDLL->getText("TXT_KEY_POPUP_FLIPPED_CITY_KEEP", pNewCity->getNameKey());
	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, szBuffer);
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_KEEP_FLIPPED_CITY").c_str(), NULL, 0, WIDGET_GENERAL);
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_DISBAND_FLIPPED_CITY").c_str(), NULL, 1, WIDGET_GENERAL);
// BUG - Examine Culture Flip - start
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_CITY_WARNING_ANSWER3").c_str(), NULL, 2, WIDGET_GENERAL, -1, -1);
// BUG - Examine Culture Flip - end
	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);

	gDLL->getInterfaceIFace()->playGeneralSound("AS2D_CULTUREFLIP");

	return (true);
}
//ls612: City Goto in Viewports
bool CvDLLButtonPopup::launchGoToCityPopup(CvPopup *pPopup, CvPopupInfo &info)
{
	int iI = 0;
	int iJ = 0;
	CvPlayer& kPlayer = GET_PLAYER(GC.getGameINLINE().getActivePlayer());
	CvWString szBuffer = gDLL->getText("TXT_KEY_POPUP_GOTO_CITY");
	CvCity* pCity;
	CvUnit* pUnit = gDLL->getInterfaceIFace()->getHeadSelectedUnit();

	if ( pUnit != NULL )
	{
		switch (pUnit->getDomainType())
		{
		case DOMAIN_LAND:
			gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, szBuffer);

			for (pCity = kPlayer.firstCity(&iI); pCity != NULL; pCity = kPlayer.nextCity(&iI))
			{
				gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText(pCity->getName()).c_str(), NULL, iJ, WIDGET_CITY_GOTO, pCity->getID());
				iJ++;
			}

			gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_NEVER_MIND"), NULL, iJ, WIDGET_GENERAL);
			gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);
			break;

			//For Sea units we want the list to only include Coastal cities
		case DOMAIN_SEA:
			gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, szBuffer);
			for (pCity = kPlayer.firstCity(&iI); pCity != NULL; pCity = kPlayer.nextCity(&iI))
			{
				if (pCity->isCoastal(10))
				{
					gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText(pCity->getName()).c_str(), NULL, iJ, WIDGET_CITY_GOTO, pCity->getID());
					iJ++;
				}
			}

			gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_NEVER_MIND"), NULL, iJ , WIDGET_GENERAL);
			gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);
			break;
		}
	}

	return true;
}



bool CvDLLButtonPopup::launchChooseTechPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	PYTHON_ACCESS_LOCK_SCOPE

	/* PB Mod
	 * Oracle double tech bugfix. If a player is logged in during the building
	 * of the oracle in one of his cities, he get immediately the Tech selection
	 * and again, at the next login.
	 *
	 * Solution: Set a flag if the player was logged in during the oracle build
	 *           and skip second popup.
	 */
	if (info.getFlags() == 1)
	{
		return false;
	}

	CvPlayer& player = GET_PLAYER(GC.getGameINLINE().getActivePlayer());

	const int iDiscover = info.getData1();
	CvWString szHeader = info.getText();
	if (szHeader.empty())
	{
		szHeader = (iDiscover > 0) ? gDLL->getText("TXT_KEY_POPUP_CHOOSE_TECH").c_str() : gDLL->getText("TXT_KEY_POPUP_RESEARCH_NEXT").c_str();
	}
	gDLL->getInterfaceIFace()->popupSetHeaderString(pPopup, szHeader, DLL_FONT_LEFT_JUSTIFY);

	if (iDiscover == 0)
	{
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_SEE_BIG_PICTURE").c_str(), ARTFILEMGR.getInterfaceArtInfo("INTERFACE_POPUPBUTTON_TECH")->getPath(), GC.getNumTechInfos(), WIDGET_GENERAL, -1, MAX_INT, true, POPUP_LAYOUT_STRETCH, DLL_FONT_LEFT_JUSTIFY);
	}
	TechTypes eNextBestTech = NO_TECH;
	TechTypes eBestTech = player.AI_bestTech(1, (iDiscover > 0), true);

	int iNumTechs = 0;
	for (int iPass = 0; iPass < 2; iPass++)
	{
		for (int iI = 0; iI < GC.getNumTechInfos(); iI++)
		{
			if (((iI == eBestTech) || (iI == eNextBestTech)) == (iPass == 0))
			{
				if (player.canResearch((TechTypes)iI))
				{
					CvWString szBuffer;
					szBuffer.Format(L"%s (%d)", GC.getTechInfo((TechTypes)iI).getDescription(), ((iDiscover > 0) ? 0 : player.getResearchTurnsLeft(((TechTypes)iI), true)));

					if ((iI == eBestTech) || (iI == eNextBestTech))
					{
						szBuffer += gDLL->getText("TXT_KEY_POPUP_RECOMMENDED_ONLY_ADV", GC.getAdvisorInfo((AdvisorTypes)(GC.getTechInfo((TechTypes)iI).getAdvisorType())).getTextKeyWide());
					}

					CvString szButton = GC.getTechInfo((TechTypes) iI).getButton();

					// RevDCM - OC_LIMITED_RELIGIONS - 4/23/10
					if (player.canFoundReligion() && GC.getGameINLINE().isTechCanFoundReligion(TechTypes(iI)))
					{
						for (int iJ = 0; iJ < GC.getNumReligionInfos(); iJ++)
						{
							if (GC.getReligionInfo((ReligionTypes)iJ).getTechPrereq() == iI
							&& !GC.getGameINLINE().isReligionSlotTaken((ReligionTypes)iJ))
							{
								szButton = GC.getGameINLINE().isOption(GAMEOPTION_PICK_RELIGION) ? GC.getReligionInfo((ReligionTypes) iJ).getGenericTechButton() : GC.getReligionInfo((ReligionTypes) iJ).getTechButton();
								break;
							}
						}
					}
					// ! RevDCM - OC_LIMITED_RELIGIONS

					gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, szButton, iI, WIDGET_RESEARCH, iI, iDiscover, true, POPUP_LAYOUT_STRETCH, DLL_FONT_LEFT_JUSTIFY);
					iNumTechs++;
				}
			}
		}
	}
	if (0 == iNumTechs)
	{
		// player cannot research anything, so don't show this popup after all
		return false;
	}

	gDLL->getInterfaceIFace()->popupSetPopupType(pPopup, POPUPEVENT_TECHNOLOGY, ARTFILEMGR.getInterfaceArtInfo("INTERFACE_POPUPBUTTON_TECH")->getPath());

	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, ((iDiscover > 0) ? POPUPSTATE_QUEUED : POPUPSTATE_MINIMIZED));

	return true;
}

/************************************************************************************************/
/* Great Diplomat MOD               START                                                  		*/
/************************************************************************************************/
bool CvDLLButtonPopup::launchUnitForcePeacePopup(CvPopup* pPopup, CvPopupInfo &info)
{
	PlayerTypes eActivePlayer = GC.getGameINLINE().getActivePlayer();
	CvUnit* pUnit = GET_PLAYER(eActivePlayer).getUnit(info.getData1());

	if (eActivePlayer == NO_PLAYER || !pUnit->canForcePeace())
	{
		return (false);
	}

	CvWString szBuffer;
	szBuffer = gDLL->getText("TXT_KEY_POPUP_CHOOSE_FORCE_PEACE_ENEMY_WITH");
	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, szBuffer);

	for (int iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (pUnit->canForcePeaceWith((PlayerTypes)iI))
			{
				szBuffer = gDLL->getText("TXT_KEY_POPUP_FORCE_PEACE", GET_PLAYER((PlayerTypes)iI).getCivilizationDescriptionKey());
				gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, GC.getCivilizationInfo(GET_PLAYER((PlayerTypes)iI).getCivilizationType()).getButton(), (int)iI, WIDGET_GENERAL);
			}
		}
	}

	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_NEVER_MIND").c_str(), ARTFILEMGR.getInterfaceArtInfo("INTERFACE_BUTTONS_CANCEL")->getPath(), -1, WIDGET_GENERAL);

	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);

	return (true);
}
/************************************************************************************************/
/* Great Diplomat MOD               END                                                  		*/
/************************************************************************************************/

bool CvDLLButtonPopup::launchChangeCivicsPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	CivicTypes* paeNewCivics = new CivicTypes[GC.getNumCivicOptionInfos()];
	if (NULL == paeNewCivics)
	{
		return (false);
	}

	CivicOptionTypes eCivicOptionType = (CivicOptionTypes)info.getData1();
	CivicTypes eCivicType = (CivicTypes)info.getData2();
	bool bValid = false;

	if (eCivicType != NO_CIVIC)
	{
		for (int iI = 0; iI < GC.getNumCivicOptionInfos(); iI++)
		{
			if (iI == eCivicOptionType)
			{
				paeNewCivics[iI] = eCivicType;
			}
			else
			{
				paeNewCivics[iI] = GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCivics((CivicOptionTypes)iI);
			}
		}

		if (GET_PLAYER(GC.getGameINLINE().getActivePlayer()).canRevolution(paeNewCivics))
		{
			bValid = true;
		}
	}
	else
	{
		bValid = true;
	}

	if (bValid)
	{
		CvWString szBuffer;
		if (eCivicType != NO_CIVIC)
		{
			szBuffer = gDLL->getText("TXT_KEY_POPUP_NEW_CIVIC", GC.getCivicInfo(eCivicType).getTextKeyWide());
			if (!CvWString(GC.getCivicInfo(eCivicType).getStrategy()).empty())
			{
				CvWString szTemp;
				szTemp.Format(L" (%s)", GC.getCivicInfo(eCivicType).getStrategy());
				szBuffer += szTemp;
			}
			szBuffer += gDLL->getText("TXT_KEY_POPUP_START_REVOLUTION");
			gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, szBuffer);

			szBuffer = gDLL->getText("TXT_KEY_POPUP_YES_START_REVOLUTION");
			int iAnarchyLength = GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCivicAnarchyLength(paeNewCivics);
			if (iAnarchyLength > 0)
			{
				szBuffer += gDLL->getText("TXT_KEY_POPUP_TURNS_OF_ANARCHY", iAnarchyLength);
			}
			gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, NULL, 0, WIDGET_GENERAL);
		}
		else
		{
			gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_POPUP_FIRST_REVOLUTION"));
		}

		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_OLD_WAYS_BEST").c_str(), NULL, 1, WIDGET_GENERAL);
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_SEE_BIG_PICTURE").c_str(), NULL, 2, WIDGET_GENERAL);
		gDLL->getInterfaceIFace()->popupSetPopupType(pPopup, POPUPEVENT_CIVIC, ARTFILEMGR.getInterfaceArtInfo("INTERFACE_POPUPBUTTON_CIVICS")->getPath());
		gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_MINIMIZED);
	}

	SAFE_DELETE_ARRAY(paeNewCivics);

	return (bValid);
}


bool CvDLLButtonPopup::launchAlarmPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	gDLL->getInterfaceIFace()->playGeneralSound("AS2D_ALARM");

	gDLL->getInterfaceIFace()->popupSetHeaderString(pPopup, gDLL->getText("TXT_KEY_POPUP_ALARM_TITLE").c_str());
	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, info.getText());
	gDLL->getInterfaceIFace()->popupLaunch(pPopup, true, POPUPSTATE_IMMEDIATE);

	return (true);
}


bool CvDLLButtonPopup::launchDeclareWarMovePopup(CvPopup* pPopup, CvPopupInfo &info)
{
	TeamTypes eRivalTeam = (TeamTypes)info.getData1();
	int iX = info.getData2();
	int iY = info.getData3();

	FAssert(eRivalTeam != NO_TEAM);

	CvPlot* pPlot = GC.getMapINLINE().plotINLINE(iX, iY);

	CvWString szBuffer;
	if ((pPlot != NULL) && (pPlot->getTeam() == eRivalTeam))
	{
		szBuffer = gDLL->getText("TXT_KEY_POPUP_ENTER_LANDS_WAR", GET_PLAYER(pPlot->getOwnerINLINE()).getCivilizationAdjective());

		if (GET_TEAM(GC.getGameINLINE().getActiveTeam()).isOpenBordersTrading())
		{
			szBuffer += gDLL->getText("TXT_KEY_POPUP_ENTER_WITH_OPEN_BORDERS");
		}
	}
	else
	{
		szBuffer = gDLL->getText("TXT_KEY_POPUP_DOES_THIS_MEAN_WAR", GET_TEAM(eRivalTeam).getName().GetCString());
	}
	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, szBuffer);
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_DECLARE_WAR_YES").c_str(), NULL, 0);
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_DECLARE_WAR_NO").c_str());
	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);

	return (true);
}


bool CvDLLButtonPopup::launchConfirmCommandPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	int iAction = info.getData1();
	CvWString szBuffer;
	szBuffer = gDLL->getText("TXT_KEY_POPUP_ARE_YOU_SURE_ACTION", GC.getActionInfo(iAction).getTextKeyWide());
	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, szBuffer);
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_YES").c_str(), NULL, 0);
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_NO").c_str());
	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);

	return (true);
}


bool CvDLLButtonPopup::launchLoadUnitPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	CLLNode<IDInfo>* pUnitNode;
	CvSelectionGroup* pSelectionGroup;
	CvUnit* pLoopUnit;
	CvPlot* pPlot;
	CvWStringBuffer szBuffer;
	int iCount;
	CvUnit* pFirstUnit = NULL;

	pSelectionGroup = gDLL->getInterfaceIFace()->getSelectionList();

	if (NULL == pSelectionGroup)
	{
		return (false);
	}

	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_CHOOSE_TRANSPORT"));

	pPlot = pSelectionGroup->plot();
	if (NULL == pPlot)
	{
		return (false);
	}

	iCount = 1;

	pUnitNode = pPlot->headUnitNode();

	while (pUnitNode != NULL)
	{
		pLoopUnit = ::getUnit(pUnitNode->m_data);
		pUnitNode = pPlot->nextUnitNode(pUnitNode);

		if (pSelectionGroup->canDoCommand(COMMAND_LOAD_UNIT, pLoopUnit->getOwnerINLINE(), pLoopUnit->getID()))
		{
			if (!pFirstUnit)
			{
				pFirstUnit = pLoopUnit;
			}
			szBuffer.clear();
			GAMETEXT.setUnitHelp(szBuffer, pLoopUnit, true);
			szBuffer.append(L", ");
			// Thomas SG - AC: Advanced Cargo START
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_HELP_CARGO_SPACE", pLoopUnit->getCargo(), pLoopUnit->totalCargoSpace()));
			// Thomas SG - AC: Advanced Cargo END
			gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, CvWString(szBuffer.getCString()), NULL, iCount, WIDGET_GENERAL);
			iCount++;
		}
	}

	if (iCount <= 2)
	{
		if (pFirstUnit)
		{
			GC.getGameINLINE().selectionListGameNetMessage(GAMEMESSAGE_DO_COMMAND, COMMAND_LOAD_UNIT, pFirstUnit->getOwnerINLINE(), pFirstUnit->getID());
		}
		return (false);
	}

	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_NEVER_MIND"), NULL, 0, WIDGET_GENERAL);

	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);

	return (true);
}


bool CvDLLButtonPopup::launchLeadUnitPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	CLLNode<IDInfo>* pUnitNode;
	CvSelectionGroup* pSelectionGroup;
	CvUnit* pLoopUnit;
	CvPlot* pPlot;
	CvWStringBuffer szBuffer;
	int iCount;
	CvUnit* pFirstUnit = NULL;

	pSelectionGroup = gDLL->getInterfaceIFace()->getSelectionList();

	if (NULL == pSelectionGroup)
	{
		return (false);
	}

	pPlot = pSelectionGroup->plot();
	if (NULL == pPlot)
	{
		return (false);
	}

	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_CHOOSE_UNIT_TO_LEAD"));


	iCount = 1;

	pUnitNode = pPlot->headUnitNode();

	while (pUnitNode != NULL)
	{
		pLoopUnit = ::getUnit(pUnitNode->m_data);
		pUnitNode = pPlot->nextUnitNode(pUnitNode);

		if (pLoopUnit->canPromote((PromotionTypes) info.getData1(), info.getData2()))
		{
			if (!pFirstUnit)
			{
				pFirstUnit = pLoopUnit;
			}
			szBuffer.clear();
			GAMETEXT.setUnitHelp(szBuffer, pLoopUnit, true);
			gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, CvWString(szBuffer.getCString()), NULL, iCount, WIDGET_GENERAL);
			iCount++;
		}
	}

	if (iCount <= 2)
	{
		if (pFirstUnit)
		{
			GC.getGameINLINE().selectionListGameNetMessage(GAMEMESSAGE_PUSH_MISSION, MISSION_LEAD, pFirstUnit->getID());
		}
		return (false);
	}

	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_NEVER_MIND"), NULL, 0, WIDGET_GENERAL);

	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);

	return (true);
}

bool CvDLLButtonPopup::launchDoEspionagePopup(CvPopup* pPopup, CvPopupInfo &info)
{
	CvUnit* pUnit;
	CvPlot* pPlot;
	CvWString szBuffer;

	pUnit = gDLL->getInterfaceIFace()->getHeadSelectedUnit();
	if (NULL == pUnit)
	{
		return (false);
	}

	pPlot = pUnit->plot();
	if (NULL == pPlot)
	{
		return (false);
	}

	/* PB Mod.
	 * Espionage popup bugfix: Compare turn slice timestamp.
	 * This fails for all quequed messages in pitboss save.
	 */
	if(	GC.getGameINLINE().getTurnSlice() != info.getFlags() )
	{
		return (false);
	}

	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_CHOOSE_ESPIONAGE_MISSION"));

	for (int iLoop = 0; iLoop < GC.getNumEspionageMissionInfos(); iLoop++)
	{
		if (!GC.getEspionageMissionInfo((EspionageMissionTypes) iLoop).isPassive())
		{
			if (GET_PLAYER(pUnit->getOwnerINLINE()).canDoEspionageMission((EspionageMissionTypes) iLoop, pPlot->getOwnerINLINE(), pPlot, -1, pUnit))
			{
				if (GC.getEspionageMissionInfo((EspionageMissionTypes) iLoop).isTwoPhases())
				{
					szBuffer = GC.getEspionageMissionInfo((EspionageMissionTypes) iLoop).getDescription();
					gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, ARTFILEMGR.getInterfaceArtInfo("ESPIONAGE_BUTTON")->getPath(), iLoop, WIDGET_GENERAL);
				}
				else
				{
					int iCost = GET_PLAYER(pUnit->getOwnerINLINE()).getEspionageMissionCost((EspionageMissionTypes) iLoop, pPlot->getOwnerINLINE(), pPlot, -1, pUnit);
					if (iCost > 0)
					{
						szBuffer = gDLL->getText("TXT_KET_ESPIONAGE_MISSION_COST", GC.getEspionageMissionInfo((EspionageMissionTypes) iLoop).getTextKeyWide(), iCost);
						gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, ARTFILEMGR.getInterfaceArtInfo("ESPIONAGE_BUTTON")->getPath(), iLoop, WIDGET_HELP_ESPIONAGE_COST, iLoop, -1);
					}
				}
			}
		}
	}

	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_NEVER_MIND"), ARTFILEMGR.getInterfaceArtInfo("INTERFACE_BUTTONS_CANCEL")->getPath(), NO_ESPIONAGEMISSION, WIDGET_GENERAL);

	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);

	return (true);
}

bool CvDLLButtonPopup::launchDoEspionageTargetPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	CvUnit* pUnit = gDLL->getInterfaceIFace()->getHeadSelectedUnit();
	if (NULL == pUnit)
	{
		return false;
	}
	// espionage popup bugfix: Compare turn slice timestamp. This fails for all quequed messages in pitboss save.
	if(	GC.getGameINLINE().getTurnSlice() != info.getFlags() )
	{
		return (false);
	}
	CvPlot* pPlot = pUnit->plot();
	CvCity* pCity = pPlot->getPlotCity();
	PlayerTypes eTargetPlayer = pPlot->getOwnerINLINE();
	CvPlayer& kPlayer = GET_PLAYER(GC.getGameINLINE().getActivePlayer());

	if (NO_PLAYER == eTargetPlayer)
	{
		return false;
	}

	EspionageMissionTypes eMission = (EspionageMissionTypes)info.getData1();
	if (NO_ESPIONAGEMISSION == eMission)
	{
		return false;
	}

	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_ESPIONAGE_CHOOSE_TARGET"));

	CvEspionageMissionInfo& kMission = GC.getEspionageMissionInfo(eMission);
	if (kMission.getDestroyBuildingCostFactor() > 0)
	{
		if (NULL != pCity)
		{
			for (int iBuilding = 0; iBuilding < GC.getNumBuildingInfos(); ++iBuilding)
			{
				if (kPlayer.canDoEspionageMission(eMission, eTargetPlayer, pPlot, iBuilding, pUnit))
				{
					CvBuildingInfo& kBuilding = GC.getBuildingInfo((BuildingTypes)iBuilding);
					if (pCity->getNumRealBuilding((BuildingTypes)iBuilding) > 0)
					{
						int iCost = kPlayer.getEspionageMissionCost(eMission, eTargetPlayer, pPlot, iBuilding, pUnit);
						CvWString szBuffer = gDLL->getText("TXT_KET_ESPIONAGE_MISSION_COST", kBuilding.getDescription(), iCost);
						gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, kBuilding.getButton(), iBuilding, WIDGET_HELP_ESPIONAGE_COST, eMission, iBuilding);
					}
				}
			}
		}
	}
	else if (kMission.getDestroyUnitCostFactor() > 0)
	{
		// RevolutionDCM start
		if (NULL != pCity)
		{
			for (int iSpecialist = 7; iSpecialist < GC.getNumSpecialistInfos(); iSpecialist++)
			{
				if (kPlayer.canDoEspionageMission(eMission, eTargetPlayer, pPlot, iSpecialist, pUnit))
				{
					CvSpecialistInfo& kSpecialist = GC.getSpecialistInfo((SpecialistTypes)iSpecialist);
					//does this city contain this great specialist type?
					if (pCity->getFreeSpecialistCount((SpecialistTypes)iSpecialist) > 0)
					{
						int iCost = kPlayer.getEspionageMissionCost(eMission, eTargetPlayer, pPlot, iSpecialist, pUnit);
						CvWString szBuffer = gDLL->getText("TXT_KET_ESPIONAGE_MISSION_COST", kSpecialist.getDescription(), iCost);
						gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, kSpecialist.getButton(), iSpecialist, WIDGET_HELP_ESPIONAGE_COST, eMission, iSpecialist);
					}
				}
			}
		}
		// RevolutionDCM end
	}
	else if (kMission.getDestroyProjectCostFactor() > 0)
	{
		for (int iProject = 0; iProject < GC.getNumProjectInfos(); ++iProject)
		{
			if (kPlayer.canDoEspionageMission(eMission, eTargetPlayer, pPlot, iProject, pUnit))
			{
				if (GET_TEAM(GET_PLAYER(eTargetPlayer).getTeam()).getProjectCount((ProjectTypes)iProject) > 0)
				{
					int iCost = kPlayer.getEspionageMissionCost(eMission, eTargetPlayer, pPlot, iProject, pUnit);
					CvWString szBuffer = gDLL->getText("TXT_KET_ESPIONAGE_MISSION_COST", GC.getProjectInfo((ProjectTypes)iProject).getDescription(), iCost);
					gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, GC.getProjectInfo((ProjectTypes)iProject).getButton(), iProject, WIDGET_HELP_ESPIONAGE_COST, eMission, iProject);
				}
			}
		}
	}
	else if (kMission.getBuyTechCostFactor() > 0)
	{
		for (int iTech = 0; iTech < GC.getNumTechInfos(); ++iTech)
		{
			if (kPlayer.canDoEspionageMission(eMission, eTargetPlayer, pPlot, iTech, pUnit))
			{
				int iCost = kPlayer.getEspionageMissionCost(eMission, eTargetPlayer, pPlot, iTech, pUnit);
				CvTechInfo& kTech = GC.getTechInfo((TechTypes)iTech);
				CvWString szBuffer = gDLL->getText("TXT_KET_ESPIONAGE_MISSION_COST", kTech.getDescription(), iCost);
				gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, kTech.getButton(), iTech, WIDGET_HELP_ESPIONAGE_COST, eMission, iTech);
			}
		}
	}
	else if (kMission.getSwitchCivicCostFactor() > 0)
	{
		for (int iCivic = 0; iCivic < GC.getNumCivicInfos(); ++iCivic)
		{
			if (kPlayer.canDoEspionageMission(eMission, eTargetPlayer, pPlot, iCivic, pUnit))
			{
				int iCost = kPlayer.getEspionageMissionCost(eMission, eTargetPlayer, pPlot, iCivic, pUnit);
				CvCivicInfo& kCivic = GC.getCivicInfo((CivicTypes)iCivic);
				CvWString szBuffer = gDLL->getText("TXT_KET_ESPIONAGE_MISSION_COST", kCivic.getDescription(), iCost);
				gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, kCivic.getButton(), iCivic, WIDGET_HELP_ESPIONAGE_COST, eMission, iCivic);
			}
		}
	}
	else if (kMission.getSwitchReligionCostFactor() > 0)
	{
		for (int iReligion = 0; iReligion < GC.getNumReligionInfos(); ++iReligion)
		{
			if (kPlayer.canDoEspionageMission(eMission, eTargetPlayer, pPlot, iReligion, pUnit))
			{
				int iCost = kPlayer.getEspionageMissionCost(eMission, eTargetPlayer, pPlot, iReligion, pUnit);
				CvReligionInfo& kReligion = GC.getReligionInfo((ReligionTypes)iReligion);
				CvWString szBuffer = gDLL->getText("TXT_KET_ESPIONAGE_MISSION_COST", kReligion.getDescription(), iCost);
				gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, kReligion.getButton(), iReligion, WIDGET_HELP_ESPIONAGE_COST, eMission, iReligion);
			}
		}
	}
/************************************************************************************************/
/* Afforess	                  Start		 07/28/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	else if (kMission.getRemoveReligionsCostFactor() > 0)
	{
		for (int iReligion = 0; iReligion < GC.getNumReligionInfos(); ++iReligion)
		{
			if (kPlayer.canDoEspionageMission(eMission, eTargetPlayer, pPlot, iReligion, pUnit))
			{
				int iCost = kPlayer.getEspionageMissionCost(eMission, eTargetPlayer, pPlot, iReligion, pUnit);
				CvReligionInfo& kReligion = GC.getReligionInfo((ReligionTypes)iReligion);
				CvWString szBuffer = gDLL->getText("TXT_KET_ESPIONAGE_MISSION_COST", kReligion.getDescription(), iCost);
				gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, kReligion.getButton(), iReligion, WIDGET_HELP_ESPIONAGE_COST, eMission, iReligion);
			}
		}
	}
	else if (kMission.getRemoveCorporationsCostFactor() > 0)
	{
		for (int iCorp = 0; iCorp < GC.getNumCorporationInfos(); ++iCorp)
		{
			if (kPlayer.canDoEspionageMission(eMission, eTargetPlayer, pPlot, iCorp, pUnit))
			{
				int iCost = kPlayer.getEspionageMissionCost(eMission, eTargetPlayer, pPlot, iCorp, pUnit);
				CvCorporationInfo& kCorp = GC.getCorporationInfo((CorporationTypes)iCorp);
				CvWString szBuffer = gDLL->getText("TXT_KET_ESPIONAGE_MISSION_COST", kCorp.getDescription(), iCost);
				gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, kCorp.getButton(), iCorp, WIDGET_HELP_ESPIONAGE_COST, eMission, iCorp);
			}
		}
	}
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_NEVER_MIND"), ARTFILEMGR.getInterfaceArtInfo("INTERFACE_BUTTONS_CANCEL")->getPath(), NO_ESPIONAGEMISSION, WIDGET_GENERAL);

	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);

	return true;
}


bool CvDLLButtonPopup::launchMainMenuPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	gDLL->getInterfaceIFace()->popupSetStyle( pPopup, "Window_NoTitleBar_Style" );

	// 288,72
	gDLL->getInterfaceIFace()->popupAddDDS(pPopup, "resource/temp/civ4_title_small.dds", 192, 48);

	gDLL->getInterfaceIFace()->popupAddSeparator(pPopup);

	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_EXIT_TO_DESKTOP").c_str(), NULL, 0, WIDGET_GENERAL, 0, 0, true, POPUP_LAYOUT_STRETCH, DLL_FONT_CENTER_JUSTIFY);

	if (!gDLL->getInterfaceIFace()->isDebugMenuCreated())
	{
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_EXIT_TO_MAIN_MENU").c_str(), NULL, 1, WIDGET_GENERAL, 1, 0, true, POPUP_LAYOUT_STRETCH, DLL_FONT_CENTER_JUSTIFY);
	}

	if (GC.getGameINLINE().canDoControl(CONTROL_RETIRE))
	{
		if( GC.getGameINLINE().isPitboss()){
			gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_RETIRE_MOD_PITBOSS").c_str(), NULL, 2, WIDGET_GENERAL, 2, 0, true, POPUP_LAYOUT_STRETCH, DLL_FONT_CENTER_JUSTIFY);
		}else{
			gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_RETIRE").c_str(), NULL, 2, WIDGET_GENERAL, 2, 0, true, POPUP_LAYOUT_STRETCH, DLL_FONT_CENTER_JUSTIFY);
		}
	}

	if ((GC.getGameINLINE().getElapsedGameTurns() == 0) && !(GC.getGameINLINE().isGameMultiPlayer()) && !(GC.getInitCore().getWBMapScript()))
	{
		// Don't allow if there has already been diplomacy
		bool bShow = true;
		for (int i = 0; bShow && i < MAX_CIV_TEAMS; i++)
		{
			for (int j = i+1; bShow && j < MAX_CIV_TEAMS; j++)
			{
				if (GET_TEAM((TeamTypes)i).isHasMet((TeamTypes)j))
				{
					bShow = false;
				}
			}
		}
/********************************************************************************/
/**		REVOLUTION_MOD							2/29/08				jdog5000	*/
/**																				*/
/**																				*/
/********************************************************************************/
		/*if (bShow)
		{
			// Don't block regenerate on game script data
			if (!GC.getGameINLINE().getScriptData().empty())
			{
				bShow = false;
			}
		}*/
/********************************************************************************/
/**		REVOLUTION_MOD							END								*/
/********************************************************************************/
		if (bShow)
		{
			for (int i = 0; i < GC.getMapINLINE().numPlots(); ++i)
			{
				CvPlot* pPlot = GC.getMapINLINE().plotByIndexINLINE(i);
				if (!pPlot->getScriptData().empty())
				{
					bShow = false;
					break;
				}
			}

			if (bShow)
			{
				gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_REGENERATE_MAP").c_str(), NULL, 3, WIDGET_GENERAL, 3, 0, true, POPUP_LAYOUT_STRETCH, DLL_FONT_CENTER_JUSTIFY);
			}
		}
	}

	if (GC.getGameINLINE().canDoControl(CONTROL_LOAD_GAME))
	{
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_LOAD_GAME").c_str(), NULL, 4, WIDGET_GENERAL, 4, 0, true, POPUP_LAYOUT_STRETCH, DLL_FONT_CENTER_JUSTIFY);
	}
	if (GC.getGameINLINE().canDoControl(CONTROL_SAVE_NORMAL))
	{
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_SAVE_GAME").c_str(), NULL, 5, WIDGET_GENERAL, 5, 0, true, POPUP_LAYOUT_STRETCH, DLL_FONT_CENTER_JUSTIFY);
	}
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_OPTIONS").c_str(), NULL, 6, WIDGET_GENERAL, 6, 0, true, POPUP_LAYOUT_STRETCH, DLL_FONT_CENTER_JUSTIFY);

	if (GC.getGameINLINE().canDoControl(CONTROL_WORLD_BUILDER))
	{
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_ENTER_WB").c_str(), NULL, 7, WIDGET_GENERAL, 7, 0, true, POPUP_LAYOUT_STRETCH, DLL_FONT_CENTER_JUSTIFY);
	}

	if (GC.getGameINLINE().canDoControl(CONTROL_ADMIN_DETAILS))
	{
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_GAME_DETAILS").c_str(), NULL, 8, WIDGET_GENERAL, 8, 0, true, POPUP_LAYOUT_STRETCH, DLL_FONT_CENTER_JUSTIFY);
	}

	if (GC.getGameINLINE().canDoControl(CONTROL_DETAILS))
	{
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_DETAILS_TITLE").c_str(), NULL, 9, WIDGET_GENERAL, 9, 0, true, POPUP_LAYOUT_STRETCH, DLL_FONT_CENTER_JUSTIFY);
	}

	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_CANCEL").c_str(), NULL, 10, WIDGET_GENERAL, 10, 0, true, POPUP_LAYOUT_STRETCH, DLL_FONT_CENTER_JUSTIFY);
	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);

	return (true);
}

bool CvDLLButtonPopup::launchConfirmMenu(CvPopup *pPopup, CvPopupInfo &info)
{
	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_POPUP_ARE_YOU_SURE").c_str());
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_YES").c_str(), NULL, 0, WIDGET_GENERAL);
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_NO").c_str(), NULL, 1, WIDGET_GENERAL);
	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);

	return true;
}

bool CvDLLButtonPopup::launchPythonScreen(CvPopup* pPopup, CvPopupInfo &info)
{
	PYTHON_ACCESS_LOCK_SCOPE
	// this is not really a popup, but a Python screen
	// we trick the app into thinking that it's a popup so that we can take advantage of the popup queuing system

	CyArgsList argsList;
	argsList.add(info.getData1());
	argsList.add(info.getData2());
	argsList.add(info.getData3());
	argsList.add(info.getOption1());
	argsList.add(info.getOption2());
	PYTHON_CALL_FUNCTION(__FUNCTION__, PYScreensModule, CvString(info.getText()).GetCString(), argsList.makeFunctionArgs());

	return (false); // return false, so the Popup object is deleted, since it's just a dummy
}

bool CvDLLButtonPopup::launchCancelDeal(CvPopup* pPopup, CvPopupInfo &info)
{
	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup,  gDLL->getText("TXT_KEY_POPUP_CANCEL_DEAL") );

	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_CANCEL_DEAL_YES"), NULL, 0, WIDGET_GENERAL);
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_CANCEL_DEAL_NO"), NULL, 1, WIDGET_GENERAL);

	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);

	return true;
}

bool CvDLLButtonPopup::launchPythonPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, info.getText());
	for (int i = 0; i < info.getNumPythonButtons(); i++)
	{
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, info.getPythonButtonText(i), info.getPythonButtonArt(i).IsEmpty() ? NULL : info.getPythonButtonArt(i).GetCString(), i);
	}

	gDLL->getInterfaceIFace()->popupSetPopupType(pPopup, POPUPEVENT_WARNING, ARTFILEMGR.getInterfaceArtInfo("INTERFACE_POPUPBUTTON_WARNING")->getPath());
	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);

	return (true);
}

bool CvDLLButtonPopup::launchDetailsPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	if (!info.getOption1())
	{
		gDLL->getInterfaceIFace()->popupSetHeaderString(pPopup, gDLL->getText("TXT_KEY_POPUP_DETAILS_TITLE"));

		gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_MENU_LEADER_NAME"));
		gDLL->getInterfaceIFace()->popupCreateEditBox(pPopup, GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getName(), WIDGET_GENERAL, gDLL->getText("TXT_KEY_MENU_LEADER_NAME"), 0, POPUP_LAYOUT_STRETCH, 0, MAX_PLAYERINFO_CHAR_COUNT);
		gDLL->getInterfaceIFace()->popupAddSeparator(pPopup);
		gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_MENU_CIV_DESC"));
		gDLL->getInterfaceIFace()->popupCreateEditBox(pPopup, GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCivilizationDescription(), WIDGET_GENERAL, gDLL->getText("TXT_KEY_MENU_CIV_DESC"), 1, POPUP_LAYOUT_STRETCH, 0, MAX_PLAYERINFO_CHAR_COUNT);
		gDLL->getInterfaceIFace()->popupAddSeparator(pPopup);
		gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_MENU_CIV_SHORT_DESC"));
		gDLL->getInterfaceIFace()->popupCreateEditBox(pPopup, GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCivilizationShortDescription(), WIDGET_GENERAL, gDLL->getText("TXT_KEY_MENU_CIV_SHORT_DESC"), 2, POPUP_LAYOUT_STRETCH, 0, MAX_PLAYERINFO_CHAR_COUNT);
		gDLL->getInterfaceIFace()->popupAddSeparator(pPopup);
		gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_MENU_CIV_ADJ"));
		gDLL->getInterfaceIFace()->popupCreateEditBox(pPopup, GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCivilizationAdjective(), WIDGET_GENERAL, gDLL->getText("TXT_KEY_MENU_CIV_ADJ"), 3, POPUP_LAYOUT_STRETCH, 0, MAX_PLAYERINFO_CHAR_COUNT);
		gDLL->getInterfaceIFace()->popupAddSeparator(pPopup);
	}
	else if (!GC.getInitCore().getCivPassword(GC.getInitCore().getActivePlayer()).empty())
	{
		// the purpose of the popup with the option set to true is to ask for the civ password if it's not set
		return false;
	}
	if (GC.getGameINLINE().isPbem() || GC.getGameINLINE().isHotSeat() || GC.getGameINLINE().isPitboss())
	{
		gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_MAIN_MENU_PASSWORD"));
		gDLL->getInterfaceIFace()->popupCreateEditBox(pPopup, PASSWORD_DEFAULT, WIDGET_GENERAL, gDLL->getText("TXT_KEY_MAIN_MENU_PASSWORD"), 4, POPUP_LAYOUT_STRETCH, 0, MAX_PASSWORD_CHAR_COUNT);
		gDLL->getInterfaceIFace()->popupAddSeparator(pPopup);
	}
	if ( (GC.getGameINLINE().isPitboss() || GC.getGameINLINE().isPbem()) && !info.getOption1() )
	{
		gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_POPUP_DETAILS_EMAIL"));
		gDLL->getInterfaceIFace()->popupCreateEditBox(pPopup, CvWString(GC.getInitCore().getEmail(GC.getInitCore().getActivePlayer())), WIDGET_GENERAL, gDLL->getText("TXT_KEY_POPUP_DETAILS_EMAIL"), 5, POPUP_LAYOUT_STRETCH, 0, MAX_PLAYEREMAIL_CHAR_COUNT);
		gDLL->getInterfaceIFace()->popupAddSeparator(pPopup);
	}
	if (GC.getGameINLINE().isPbem() && !info.getOption1())
	{
		gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_POPUP_DETAILS_SMTP"));
		gDLL->getInterfaceIFace()->popupCreateEditBox(pPopup, CvWString(GC.getInitCore().getSmtpHost(GC.getInitCore().getActivePlayer())), WIDGET_GENERAL, gDLL->getText("TXT_KEY_POPUP_DETAILS_SMTP"), 6, POPUP_LAYOUT_STRETCH, 0, MAX_PLAYEREMAIL_CHAR_COUNT);
		gDLL->getInterfaceIFace()->popupAddSeparator(pPopup);

		if (GC.getGameINLINE().getPbemTurnSent())
		{
			gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_MISC_SEND"), NULL, 0, WIDGET_GENERAL);
		}
	}

	// Disable leader name edit box for internet games
	if (GC.getInitCore().getMultiplayer() && gDLL->isFMPMgrPublic())
	{
		gDLL->getInterfaceIFace()->popupEnableEditBox(pPopup, 0, false);
	}

	gDLL->getInterfaceIFace()->popupLaunch(pPopup, true, POPUPSTATE_IMMEDIATE);
	return (true);
}

bool CvDLLButtonPopup::launchAdminPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	gDLL->getInterfaceIFace()->popupSetHeaderString(pPopup, gDLL->getText("TXT_KEY_POPUP_GAME_DETAILS"));

	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_MAIN_MENU_GAME_NAME"));
	gDLL->getInterfaceIFace()->popupCreateEditBox(pPopup, GC.getInitCore().getGameName(), WIDGET_GENERAL, gDLL->getText("TXT_KEY_MAIN_MENU_GAME_NAME"), 0, POPUP_LAYOUT_STRETCH, 0, MAX_GAMENAME_CHAR_COUNT);
	gDLL->getInterfaceIFace()->popupAddSeparator(pPopup);
	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_POPUP_ADMIN_PASSWORD"));
	gDLL->getInterfaceIFace()->popupCreateEditBox(pPopup, PASSWORD_DEFAULT, WIDGET_GENERAL, gDLL->getText("TXT_KEY_POPUP_ADMIN_PASSWORD"), 1, POPUP_LAYOUT_STRETCH, 0, MAX_PASSWORD_CHAR_COUNT);
	gDLL->getInterfaceIFace()->popupAddSeparator(pPopup);
	if (!GC.getGameINLINE().isGameMultiPlayer())
	{
		gDLL->getInterfaceIFace()->popupCreateCheckBoxes(pPopup, 1, 2);
		gDLL->getInterfaceIFace()->popupSetCheckBoxText(pPopup, 0, gDLL->getText("TXT_KEY_POPUP_ADMIN_ALLOW_CHEATS"), 2);
		gDLL->getInterfaceIFace()->popupSetCheckBoxState(pPopup, 0, gDLL->getChtLvl() > 0, 2);
	}

	gDLL->getInterfaceIFace()->popupLaunch(pPopup, true, POPUPSTATE_IMMEDIATE);
	return (true);
}


bool CvDLLButtonPopup::launchAdminPasswordPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_POPUP_ADMIN_PASSWORD"));
	gDLL->getInterfaceIFace()->popupCreateEditBox(pPopup, L"", WIDGET_GENERAL, gDLL->getText("TXT_KEY_POPUP_ADMIN_PASSWORD"), 0, POPUP_LAYOUT_STRETCH, 0, MAX_PASSWORD_CHAR_COUNT);

	gDLL->getInterfaceIFace()->popupLaunch(pPopup, true, POPUPSTATE_IMMEDIATE);
	return (true);
}


bool CvDLLButtonPopup::launchExtendedGamePopup(CvPopup* pPopup, CvPopupInfo &info)
{
	gDLL->getInterfaceIFace()->popupSetHeaderString(pPopup, gDLL->getText("TXT_KEY_EXTENDED_GAME_TITLE"));

	if (GC.getGameINLINE().countHumanPlayersAlive() > 0)
	{
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_EXTENDED_GAME_YES"), NULL, 0, WIDGET_GENERAL);
	}

	if (!gDLL->getInterfaceIFace()->isDebugMenuCreated())
	{
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_EXTENDED_GAME_NO_MENU"), NULL, 1, WIDGET_GENERAL);
	}
	else
	{
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_EXTENDED_GAME_NO_DESKTOP"), NULL, 1, WIDGET_GENERAL);
	}

	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false);
	return (true);
}

bool CvDLLButtonPopup::launchDiplomacyPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	int iCount;
	int iI;

	gDLL->getInterfaceIFace()->popupSetHeaderString(pPopup, gDLL->getText("TXT_KEY_DIPLOMACY_TITLE"));

	iCount = 0;

	for (iI = 0; iI < MAX_CIV_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER(GC.getGameINLINE().getActivePlayer()).canContact((PlayerTypes)iI))
			{
				gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, GET_PLAYER((PlayerTypes)iI).getName(), NULL, iI, WIDGET_GENERAL);
				iCount++;
			}
		}
	}

	if (iCount == 0)
	{
		return (false);
	}

	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_NEVER_MIND"), NULL, MAX_CIV_PLAYERS, WIDGET_GENERAL);

	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);
	return (true);
}


bool CvDLLButtonPopup::launchAddBuddyPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	gDLL->getInterfaceIFace()->popupSetHeaderString( pPopup, gDLL->getText("TXT_KEY_SYSTEM_ADD_BUDDY", info.getText().GetCString()) );
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_YES"), NULL, 0, WIDGET_GENERAL);
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_NO"), NULL, 1, WIDGET_GENERAL);
	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false);
	return (true);
}

bool CvDLLButtonPopup::launchForcedDisconnectPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	gDLL->getInterfaceIFace()->popupSetHeaderString( pPopup, gDLL->getText("TXT_KEY_MAIN_MENU_FORCED_DISCONNECT_INGAME") );
	gDLL->getInterfaceIFace()->popupLaunch(pPopup, true);
	return (true);
}

bool CvDLLButtonPopup::launchPitbossDisconnectPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	gDLL->getInterfaceIFace()->popupSetHeaderString( pPopup, gDLL->getText("TXT_KEY_PITBOSS_DISCONNECT") );
	gDLL->getInterfaceIFace()->popupLaunch(pPopup, true);
	return (true);
}

bool CvDLLButtonPopup::launchKickedPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	gDLL->getInterfaceIFace()->popupSetHeaderString( pPopup, gDLL->getText("TXT_KEY_POPUP_KICKED") );
	gDLL->getInterfaceIFace()->popupLaunch(pPopup, true);
	return (true);
}

bool CvDLLButtonPopup::launchVassalDemandTributePopup(CvPopup* pPopup, CvPopupInfo &info)
{
	if (info.getData1() == NO_PLAYER)
	{
		return false;
	}

	CvPlayer& kVassal = GET_PLAYER((PlayerTypes)info.getData1());
	if (!GET_TEAM(kVassal.getTeam()).isVassal(GC.getGameINLINE().getActiveTeam()))
	{
		return false;
	}

	int iNumResources = 0;
	if (kVassal.canTradeNetworkWith(GC.getGameINLINE().getActivePlayer()))
	{
		gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_VASSAL_DEMAND_TRIBUTE", kVassal.getNameKey()));

		for (int iBonus = 0; iBonus < GC.getNumBonusInfos(); iBonus++)
		{
			if (kVassal.getNumTradeableBonuses((BonusTypes)iBonus) > 0 && GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getNumAvailableBonuses((BonusTypes)iBonus) == 0)
			{
				CvBonusInfo& info = GC.getBonusInfo((BonusTypes)iBonus);
				gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, info.getDescription(), info.getButton(), iBonus, WIDGET_GENERAL, iBonus, -1, true, POPUP_LAYOUT_STRETCH, DLL_FONT_LEFT_JUSTIFY);
				++iNumResources;
			}
		}
	}

	if (iNumResources > 0)
	{
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_NEVER_MIND"), ARTFILEMGR.getInterfaceArtInfo("INTERFACE_BUTTONS_CANCEL")->getPath(), GC.getNumBonusInfos(), WIDGET_GENERAL);
	}
	else
	{
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_VASSAL_TRIBUTE_NOT_POSSIBLE"), ARTFILEMGR.getInterfaceArtInfo("INTERFACE_BUTTONS_CANCEL")->getPath(), GC.getNumBonusInfos(), WIDGET_GENERAL);
	}

	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);

	return true;
}

bool CvDLLButtonPopup::launchVassalGrantTributePopup(CvPopup* pPopup, CvPopupInfo &info)
{
	if (info.getData1() == NO_PLAYER)
	{
		return false;
	}

	CvPlayer& kMaster = GET_PLAYER((PlayerTypes)info.getData1());
	if (!GET_TEAM(GC.getGameINLINE().getActiveTeam()).isVassal(kMaster.getTeam()))
	{
		return false;
	}

	if (info.getData2() == NO_BONUS)
	{
		return false;
	}

	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_VASSAL_GRANT_TRIBUTE", kMaster.getCivilizationDescriptionKey(), kMaster.getNameKey(), GC.getBonusInfo((BonusTypes)info.getData2()).getTextKeyWide()));
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_VASSAL_GRANT_TRIBUTE_YES"), NULL, 0, WIDGET_GENERAL);
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_VASSAL_GRANT_TRIBUTE_NO"), NULL, 1, WIDGET_GENERAL);
	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);

	return true;
}

bool CvDLLButtonPopup::launchEventPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	CvPlayer& kActivePlayer = GET_PLAYER(GC.getGameINLINE().getActivePlayer());
	EventTriggeredData* pTriggeredData = kActivePlayer.getEventTriggered(info.getData1());
	if (NULL == pTriggeredData)
	{
		return false;
	}

	if (pTriggeredData->m_eTrigger == NO_EVENTTRIGGER)
	{
		return false;
	}

	CvEventTriggerInfo& kTrigger = GC.getEventTriggerInfo(pTriggeredData->m_eTrigger);
/************************************************************************************************/
/* Afforess	                  Start		 05/20/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
// Begin EmperorFool: Events with Images
	if (GET_PLAYER(GC.getGameINLINE().getActivePlayer()).isModderOption(MODDEROPTION_EVENT_IMAGES))
	{
		if (kTrigger.getEventArt())
		{
			gDLL->getInterfaceIFace()->popupAddDDS(pPopup, kTrigger.getEventArt());
		}
	}
// End EmperorFool: Events with Images
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/


	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, pTriggeredData->m_szText);

	bool bEventAvailable = false;
	for (int i = 0; i < kTrigger.getNumEvents(); i++)
	{
		if (GET_PLAYER(GC.getGameINLINE().getActivePlayer()).canDoEvent((EventTypes)kTrigger.getEvent(i), *pTriggeredData))
		{
			gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, GC.getEventInfo((EventTypes)kTrigger.getEvent(i)).getDescription(), GC.getEventInfo((EventTypes)kTrigger.getEvent(i)).getButton(), kTrigger.getEvent(i), WIDGET_CHOOSE_EVENT, kTrigger.getEvent(i), info.getData1());
			bEventAvailable = true;
		}
		else
		{
			gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, GC.getEventInfo((EventTypes)kTrigger.getEvent(i)).getDescription(), ARTFILEMGR.getInterfaceArtInfo("INTERFACE_EVENT_UNAVAILABLE_BULLET")->getPath(), -1, WIDGET_CHOOSE_EVENT, kTrigger.getEvent(i), info.getData1(), false);
		}
	}

	if (!bEventAvailable)
	{
		return false;
	}

	if (kTrigger.isPickCity())
	{
		CvCity* pCity = kActivePlayer.getCity(pTriggeredData->m_iCityId);
		FAssert(NULL != pCity);
		if (NULL != pCity)
		{
			gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_EXAMINE_CITY").c_str(), ARTFILEMGR.getInterfaceArtInfo("INTERFACE_BUTTONS_CITYSELECTION")->getPath(), GC.getNumEventInfos(), WIDGET_GENERAL, -1, -1);
		}
	}

	if (kTrigger.isShowPlot())
	{
		CvPlot* pPlot = GC.getMapINLINE().plot(pTriggeredData->m_iPlotX, pTriggeredData->m_iPlotY);
		if (NULL != pPlot && pPlot->isInViewport())
		{
			gDLL->getEngineIFace()->addColoredPlot(pPlot->getViewportX(), pPlot->getViewportY(), GC.getColorInfo((ColorTypes)GC.getInfoTypeForString("COLOR_WARNING_TEXT")).getColor(), PLOT_STYLE_CIRCLE, PLOT_LANDSCAPE_LAYER_RECOMMENDED_PLOTS);
			gDLL->getInterfaceIFace()->lookAt(pPlot->getPoint(), CAMERALOOKAT_NORMAL);
		}
	}

	gDLL->getInterfaceIFace()->popupLaunch(pPopup, !bEventAvailable, POPUPSTATE_IMMEDIATE);

	return true;
}

bool CvDLLButtonPopup::launchFreeColonyPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	int iLoop = 0;
	PlayerTypes ePlayer = GC.getGameINLINE().getActivePlayer();
	if (ePlayer == NO_PLAYER)
	{
		return false;
	}

	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_FREE_COLONY"));

	if (GET_PLAYER(ePlayer).canSplitEmpire())
	{
		for(CvArea* pLoopArea = GC.getMapINLINE().firstArea(&iLoop); pLoopArea != NULL; pLoopArea = GC.getMapINLINE().nextArea(&iLoop))
		{
			if (GET_PLAYER(ePlayer).canSplitArea(pLoopArea->getID()))
			{
				CvWString szCityList;
				int iCityLoop = 0;
				int iNumCities = 0;
				for (CvCity* pLoopCity = GET_PLAYER(ePlayer).firstCity(&iCityLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER(ePlayer).nextCity(&iCityLoop))
				{
					if (pLoopCity->area()->getID() == pLoopArea->getID())
					{
						if (!szCityList.empty())
						{
							szCityList += L", ";
						}
						++iNumCities;

						szCityList += pLoopCity->getName();
					}
				}

				CvWString szBuffer = gDLL->getText("TXT_KEY_SPLIT_EMPIRE", szCityList.GetCString());
				gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, ARTFILEMGR.getInterfaceArtInfo("INTERFACE_BUTTONS_CITYSELECTION")->getPath(), pLoopArea->getID(), WIDGET_GENERAL);
			}
		}
	}

	for (CvCity* pLoopCity = GET_PLAYER(ePlayer).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER(ePlayer).nextCity(&iLoop))
	{
/************************************************************************************************/
/* UNOFFICIAL_PATCH                       08/04/09                                jdog5000      */
/*                                                                                              */
/* Bugfix                                                                                       */
/************************************************************************************************/
/* original bts code
		PlayerTypes ePlayer = pLoopCity->getLiberationPlayer(false);
		if (NO_PLAYER != ePlayer)
		{
			CvWString szCity = gDLL->getText("TXT_KEY_CITY_LIBERATE", pLoopCity->getNameKey(), GET_PLAYER(ePlayer).getNameKey());
			gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szCity, ARTFILEMGR.getInterfaceArtInfo("INTERFACE_BUTTONS_CITYSELECTION")->getPath(), -pLoopCity->getID(), WIDGET_GENERAL);
		}
*/
		// Avoid potential variable name conflict
		PlayerTypes eLibPlayer = pLoopCity->getLiberationPlayer(false);
		if (NO_PLAYER != eLibPlayer)
		{
			// Don't offer liberation during war
			if( !(GET_TEAM(GET_PLAYER(ePlayer).getTeam()).isAtWar(GET_PLAYER(eLibPlayer).getTeam())) )
			{
				CvWString szCity = gDLL->getText("TXT_KEY_CITY_LIBERATE", pLoopCity->getNameKey(), GET_PLAYER(eLibPlayer).getNameKey());
				gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szCity, ARTFILEMGR.getInterfaceArtInfo("INTERFACE_BUTTONS_CITYSELECTION")->getPath(), -pLoopCity->getID(), WIDGET_GENERAL);
			}
		}
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/
	}

	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_NEVER_MIND"), ARTFILEMGR.getInterfaceArtInfo("INTERFACE_BUTTONS_CANCEL")->getPath(), 0, WIDGET_GENERAL);

	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);

	return true;
}

bool CvDLLButtonPopup::launchLaunchPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	PlayerTypes ePlayer = GC.getGameINLINE().getActivePlayer();
	if (ePlayer == NO_PLAYER)
	{
		return false;
	}

	VictoryTypes eVictory = (VictoryTypes)info.getData1();
	if (NO_VICTORY == eVictory)
	{
		return false;
	}

	CvTeam& kTeam = GET_TEAM(GET_PLAYER(ePlayer).getTeam());

	if (kTeam.getVictoryCountdown(eVictory) > 0 || GC.getGameINLINE().getGameState() != GAMESTATE_ON)
	{
		return false;
	}

	CvWString szDate;
	GAMETEXT.setTimeStr(szDate, GC.getGame().getGameTurn() + kTeam.getVictoryDelay(eVictory), false);

	gDLL->getInterfaceIFace()->popupSetHeaderString(pPopup, GC.getVictoryInfo(eVictory).getCivilopedia());
	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_ESTIMATED_VICTORY_DATE", szDate.GetCString()));

	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_YES"), NULL, 0, WIDGET_GENERAL);
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_NO"), NULL, 1, WIDGET_GENERAL);

	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);

	return true;
}

bool CvDLLButtonPopup::launchGetSaveFormatPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	CvWString szBuffer = gDLL->getText("TXT_KEY_POPUP_CHOOSE_SAVE_GAME_FORMAT");

	gDLL->getInterfaceIFace()->popupSetHeaderString(pPopup, szBuffer);

	szBuffer = gDLL->getText("TXT_KEY_POPUP_NEW_GAME_SAVE_FORMAT_BODY");
	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, szBuffer);

	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_OLD_GAME_SAVE_FORMAT").c_str(), NULL, 0, WIDGET_GENERAL);
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_NEW_GAME_SAVE_FORMAT").c_str(), NULL, 1, WIDGET_GENERAL);

	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);
	return true;
}

bool CvDLLButtonPopup::launchGetSaveInfoLostPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	CvWString szBuffer = gDLL->getText("TXT_KEY_POPUP_NON_FATAL_FORMAT_LOAD_ERROR");

	gDLL->getInterfaceIFace()->popupSetHeaderString(pPopup, szBuffer);

	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, info.getText());

	gDLL->getInterfaceIFace()->popupLaunch(pPopup);
	return true;
}

bool CvDLLButtonPopup::launchModifierRecalculationPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	CvWString szBuffer = gDLL->getText("TXT_KEY_POPUP_MODIFIER_RECALCULATION");

	gDLL->getInterfaceIFace()->popupSetHeaderString(pPopup, szBuffer);

	szBuffer = gDLL->getText("TXT_KEY_POPUP_MODIFIER_RECALCULATION_BODY");
	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, szBuffer);

	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_YES"), NULL, 1, WIDGET_GENERAL);
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_NO"), NULL, 0, WIDGET_GENERAL);

	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);
	return true;
}

bool CvDLLButtonPopup::launchFoundReligionPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	PlayerTypes ePlayer = GC.getGameINLINE().getActivePlayer();
	if (ePlayer == NO_PLAYER)
	{
		return false;
	}
/************************************************************************************************/
/* Afforess	                  Start		 06/17/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	if (!GET_PLAYER(ePlayer).m_bChoosingReligion || GET_PLAYER(ePlayer).getNumCities() == 0)
	{
		//AddDLLMessage(GC.getGameINLINE().getActivePlayer(), true, GC.getEVENT_MESSAGE_TIME(), gDLL->getText("TXT_KEY_CHEATERS_NEVER_PROSPER"), NULL, MESSAGE_TYPE_MAJOR_EVENT);
		return true;
	}
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/

	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_FOUNDED_RELIGION"));

	bool bFound = false;
	for (int iReligion = 0; iReligion < GC.getNumReligionInfos(); ++iReligion)
	{
		CvReligionInfo& kReligion = GC.getReligionInfo((ReligionTypes)iReligion);
		if (!GC.getGameINLINE().isReligionFounded((ReligionTypes)iReligion))
		{
			gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, kReligion.getDescription(), kReligion.getButton(), iReligion, WIDGET_GENERAL);
			bFound = true;
		}
	}

	if (!bFound)
	{
		return false;
	}

	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);

	return true;
}

bool CvDLLButtonPopup::launchConfirmMenuKI(CvPopup *pPopup, CvPopupInfo &info)
{
	bool bTakeoverAI = GC.getInitCore().getMPOption(MPOPTION_TAKEOVER_AI);
	if( bTakeoverAI ){
		gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_POPUP_ARE_YOU_SURE_MOD_PITBOSS").c_str());
	}else{
		gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_POPUP_ARE_YOU_SURE_MOD_PITBOSS2").c_str());
	}
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_MOD_NO_BUT_EXIT").c_str(), NULL, 0, WIDGET_GENERAL);
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_NO").c_str(), NULL, 1, WIDGET_GENERAL);
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_MOD_YES_KI").c_str(), NULL, 2, WIDGET_GENERAL);
	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);

	return true;
}

/************************************************************************************************/
/* Afforess	                  Start		 09/18/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
bool CvDLLButtonPopup::invasionPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	const CvCity* pCity = GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCity(info.getData1());

	if ( pCity != NULL )
	{
		BuildingTypes eBuilding = NO_BUILDING;

		for (int iI = 0; iI < GC.getNumBuildingInfos(); iI++)
		{
			if (pCity->getNumBuilding((BuildingTypes)iI) > 0)
			{
				if (GC.getBuildingInfo((BuildingTypes)iI).getInvasionChance() > 0)
				{
					eBuilding = (BuildingTypes)iI;
					break;
				}
			}
		}
		if (eBuilding == NO_BUILDING)
		{
			FAssertMsg(eBuilding != NO_BUILDING, "No Invasion Building Found! Error in CvDLLButtonPopup::invasionPopup(...)!");
			return false;
		}

		gDLL->getInterfaceIFace()->popupSetHeaderString(pPopup, gDLL->getText("TXT_KEY_POPUP_INVASION", pCity->getNameKey()), DLL_FONT_LEFT_JUSTIFY);

		gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_POPUP_INVADED_CITY", GC.getBuildingInfo(eBuilding).getDescription(), pCity->getNameKey()));
		gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_THATS_TERRIBLE"), NULL, 0, WIDGET_GENERAL);
		gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);
		gDLL->getInterfaceIFace()->playGeneralSound("AS2D_BUILD_BARRACKS");
	}

	return true;
}

bool CvDLLButtonPopup::launchSelectShadowUnitPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	int iUnitID = info.getData1();
	int iX = info.getData2();
	int iY = info.getData3();
	PlayerTypes ePlayer = GC.getGameINLINE().getActivePlayer();
	if (ePlayer == NO_PLAYER)
	{
		return false;
	}

	CvUnit* pUnit = GET_PLAYER(ePlayer).getUnit(iUnitID);
	if (pUnit == NULL)
	{
		return false;
	}

	CvPlot* pPlot = GC.getMapINLINE().plotINLINE(iX, iY);
	if (pPlot == NULL)
	{
		return false;
	}

	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_CHOOSE_UNIT_TO_SHADOW"));


	int iCount = 1;

	CLLNode<IDInfo>* pUnitNode;
	CvUnit* pLoopUnit;
	CvWStringBuffer szBuffer;

	pUnitNode = pPlot->headUnitNode();

	while (pUnitNode != NULL)
	{
		pLoopUnit = ::getUnit(pUnitNode->m_data);
		pUnitNode = pPlot->nextUnitNode(pUnitNode);

		if (pUnit->canShadowAt(pPlot, pLoopUnit) && pLoopUnit->getID() != 0)
		{
			szBuffer.clear();
			GAMETEXT.setUnitHelp(szBuffer, pLoopUnit, true);
			gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, CvWString(szBuffer.getCString()), GC.getUnitInfo(pLoopUnit->getUnitType()).getButton(), pLoopUnit->getID(), WIDGET_GENERAL);
			iCount++;
		}
	}

	//pPopup->setData1(iUnitID);
	//pPopup->setData2(iX);
	//pPopup->setData3(iY);

	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_NEVER_MIND"), NULL, 0, WIDGET_GENERAL);

	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);

	return (true);
}

bool CvDLLButtonPopup::launchSelectDiscoveryTechPopup(CvPopup* pPopup, CvPopupInfo &info)
{
	int iUnitID = info.getData1();
	PlayerTypes ePlayer = GC.getGameINLINE().getActivePlayer();
	if (ePlayer == NO_PLAYER)
	{
		return false;
	}

	CvUnit* pUnit = GET_PLAYER(ePlayer).getUnit(iUnitID);
	if (pUnit == NULL)
	{
		return false;
	}

	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_CHOOSE_TECH_TO_WAIT_FOR"));

	CvWString szBuffer;
	//Find the last tech
	int iBestX = 0;
	for (int iJ = 0; iJ < GC.getNumTechInfos(); iJ++)
	{
		if (GET_PLAYER(ePlayer).canEverResearch((TechTypes)iJ))
		{
			if (GC.getTechInfo((TechTypes)iJ).getGridX() > iBestX)
			{
				iBestX = GC.getTechInfo((TechTypes)iJ).getGridX();
			}
		}
	}
	//Forces sorting by the columns in the tech screen
	TechTypes eBestTech;
	int iBestValue;
	for (int iColumn = 0; iColumn < iBestX + 1; iColumn++)
	{
		eBestTech = NO_TECH;
		iBestValue = 0;
		for (int iI = 0; iI < GC.getNumTechInfos(); iI++)
		{
			if (GC.getTechInfo((TechTypes)iI).getGridX() == iColumn)
			{
				if (GET_PLAYER(ePlayer).canEverResearch((TechTypes)iI) && !GET_TEAM(GET_PLAYER(ePlayer).getTeam()).isHasTech((TechTypes)iI))
				{
					int iValue = 0;

					for (int iJ = 0; iJ < GC.getNumFlavorTypes(); iJ++)
					{
						iValue += (GC.getTechInfo((TechTypes) iI).getFlavorValue(iJ) * GC.getUnitInfo(pUnit->getUnitType()).getFlavorValue(iJ));
					}
					if (iValue > iBestValue)
					{
						eBestTech = (TechTypes)iI;
						iBestValue = iValue;
					}
				}
			}
		}
		if (eBestTech != NO_TECH)
		{
			szBuffer.Format(L"%s", GC.getTechInfo(eBestTech).getDescription());
			gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, szBuffer, GC.getTechInfo(eBestTech).getButton(), int(eBestTech), WIDGET_GENERAL);
		}
	}



	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_NEVER_MIND"), NULL, GC.getNumTechInfos(), WIDGET_GENERAL);

	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);

	return (true);
}

bool CvDLLButtonPopup::launchFieldCommanderTutorial(CvPopup* pPopup, CvPopupInfo &info)
{
	PlayerTypes ePlayer = GC.getGameINLINE().getActivePlayer();
	if (ePlayer == NO_PLAYER)
	{
		return false;
	}

	gDLL->getInterfaceIFace()->popupSetHeaderString(pPopup, gDLL->getText("TXT_KEY_POPUP_FIELD_COMMANDER_TUTORIAL_HEADER"), DLL_FONT_CENTER_JUSTIFY);
	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText("TXT_KEY_POPUP_FIELD_COMMANDER_TUTORIAL"));
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText("TXT_KEY_POPUP_FIELD_COMMANDER_OK"), NULL, 0, WIDGET_GENERAL);
	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);
	gDLL->getInterfaceIFace()->playGeneralSound("AS2D_BUILD_BARRACKS");

	return (true);
}


bool CvDLLButtonPopup::launchDarkAgePopup(CvPopup* pPopup, CvPopupInfo &info)
{
	PlayerTypes ePlayer = GC.getGameINLINE().getActivePlayer();
	if (ePlayer == NO_PLAYER)
	{
		return false;
	}

	CvWString szStartTxtKey = info.getText();

	CvArtFileMgr ArtFileMgr = ArtFileMgr.GetInstance();

	gDLL->getInterfaceIFace()->popupSetHeaderString(pPopup, gDLL->getText(szStartTxtKey + "_HEADER"), DLL_FONT_CENTER_JUSTIFY);
	if ( ArtFileMgr.getInterfaceArtInfo(CvString(szStartTxtKey + "_ART").c_str()) != NULL)
	{
		gDLL->getInterfaceIFace()->popupAddDDS(pPopup, ArtFileMgr.getInterfaceArtInfo(CvString(szStartTxtKey + "_ART").c_str())->getPath());
	}
	else
	{
		GC.getGameINLINE().logMsg("Art is null for %S", CvString(szStartTxtKey + "_ART").c_str());
	}

	gDLL->getInterfaceIFace()->popupSetBodyString(pPopup, gDLL->getText(szStartTxtKey + "_TUTORIAL"));
	gDLL->getInterfaceIFace()->popupAddGenericButton(pPopup, gDLL->getText(szStartTxtKey + "_OK_BTN"), NULL, 0, WIDGET_GENERAL);

	gDLL->getInterfaceIFace()->popupLaunch(pPopup, false, POPUPSTATE_IMMEDIATE);

	return (true);
}
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
