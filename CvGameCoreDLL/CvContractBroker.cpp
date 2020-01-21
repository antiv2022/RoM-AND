// unitAI.cpp

#include "CvGameCoreDLL.h"
#include "BetterBTSAI.h"
#include "CvPathGenerator.h"
#include "CvContractBroker.h"
#include "CvGameAI.h"
#include "CvPlayerAI.h"
#include "CvMap.h"
#include "CvGameCoreUtils.h"

CvContractBroker::CvContractBroker() : m_eOwner(NO_PLAYER)
{
	InitializeCriticalSectionAndSpinCount(&m_cWorkRequestsSection, 4000);
	InitializeCriticalSectionAndSpinCount(&m_cContractedUnitsSection, 4000);

	reset();
}

CvContractBroker::~CvContractBroker(void)
{
	DeleteCriticalSection(&m_cWorkRequestsSection);
	DeleteCriticalSection(&m_cContractedUnitsSection);
}

//	Delete all work requests and looking for work records
void	CvContractBroker::reset()
{
	//	No need to lock here as this is (currently) always run in a single-threaded context

	m_workRequests.clear();
	m_advertisingUnits.clear();
	m_advertisingTenders.clear();
	m_contractedUnits.clear();

	m_iNextWorkRequestId = 0;
}

//	Initialize
void	CvContractBroker::init(PlayerTypes eOwner)
{
	m_eOwner = eOwner;
#ifdef	USE_UNIT_TENDERING
	logBBAI("Initializing %d player's contract broker - UNIT_TENDERING enabled", m_eOwner);
#else
	logBBAI("Initializing %d player's contract broker - UNIT_TENDERING disabled", m_eOwner);
#endif
}

//	Note a unit looking for work
void	CvContractBroker::lookingForWork(CvUnit* pUnit, int iMinPriority)
{
	PROFILE_FUNC();

	advertisingUnit	unitDetails;
	int				iUnitStr = GC.getGameINLINE().AI_combatValue(pUnit->getUnitType());

	unitDetails.bIsWorker = (pUnit->AI_getUnitAIType() == UNITAI_WORKER);

	//	Combat values are just the crude value of the unit type for now - should add in promotions
	//	here for sure
	if ( pUnit->canDefend() )
	{
		unitDetails.iDefensiveValue = iUnitStr;
	}
	//TB OOS Debug: Almost certainly a solid fix here
	else
	{
		unitDetails.iDefensiveValue = 0;
	}
	if ( pUnit->canAttack() )
	{
		unitDetails.iOffensiveValue = iUnitStr;
	}
	else
	{
		unitDetails.iOffensiveValue = 0;
	}

	unitDetails.iUnitId			= pUnit->getID();
	unitDetails.iAtX			= pUnit->getX_INLINE();
	unitDetails.iAtY			= pUnit->getY_INLINE();
	unitDetails.iMinPriority	= iMinPriority;

	//	Initially not assigned to a contract
	unitDetails.iContractedWorkRequest = -1;
	//	and no attempt has been made yet to match any work requests
	unitDetails.iMatchedToRequestSeqThisPlot = -1;
	unitDetails.iMatchedToRequestSeqAnyPlot = -1;

//TB OOS fix: another one that only MIGHT have had an effect to repair things at all
	MEMORY_TRACK_EXEMPT();

	m_advertisingUnits.push_back(unitDetails);
}

//	Unit fulfilled its work and is no longer advertising as available
void	CvContractBroker::removeUnit(CvUnit* pUnit)
{
	int iI;

	for(iI = 0; iI < (int)m_advertisingUnits.size(); iI++)
	{
		if ( m_advertisingUnits[iI].iUnitId == pUnit->getID() )
		{
			m_advertisingUnits[iI].iUnitId = -1;	//	Render it unmatchable
			break;
		}
	}
}

//	Make a work request
//		iPriority should be in the range 0-100 ideally
//		eUnitFlags indicate the type(s) of unit sought
//		(iAtX,iAtY) is (roughly) where the work will be
//		pJoinUnit may be NULL but if not it is a request to join that unit's group
void	CvContractBroker::advertiseWork(int iPriority, unitCapabilities eUnitFlags, int iAtX, int iAtY, CvUnit* pJoinUnit, UnitAITypes eAIType, int iUnitStrength, CvUnitSelectionCriteria* criteria, int iMaxPath)
{
	PROFILE_FUNC();

	workRequest	newRequest;
	int			iLoop = 0;
	int			iUnitStrengthTimes100 = (iUnitStrength == -1 ? -1 : iUnitStrength*100);

	//	First check that there are not already units on the way to meet this need
	//	else concurrent builds will get queued while they are in transit
	for(CvSelectionGroup* pLoopSelectionGroup = GET_PLAYER(m_eOwner).firstSelectionGroup(&iLoop); pLoopSelectionGroup; pLoopSelectionGroup = GET_PLAYER(m_eOwner).nextSelectionGroup(&iLoop))
	{
		CvPlot* pMissionPlot = pLoopSelectionGroup->AI_getMissionAIPlot();

		if ( pMissionPlot == GC.getMapINLINE().plotINLINE(iAtX, iAtY) &&
			 pLoopSelectionGroup->AI_getMissionAIType() == (pJoinUnit == NULL ? MISSIONAI_CONTRACT : MISSIONAI_CONTRACT_UNIT) &&
			 pLoopSelectionGroup->getNumUnits() > 0 && 	//	Allow for the last unit having died so that this group is about to vanish
			 (eAIType == NO_UNITAI || pLoopSelectionGroup->getHeadUnitAI() == eAIType) &&
			 pLoopSelectionGroup->meetsUnitSelectionCriteria(criteria))
		{
			EnterCriticalSection(&m_cContractedUnitsSection);

			std::map<int,bool>::const_iterator itr = m_contractedUnits.find(pLoopSelectionGroup->getID());

			bool bNew = (itr == m_contractedUnits.end());
			if ( bNew )
			{
				m_contractedUnits[pLoopSelectionGroup->getID()] = true;
			}

			LeaveCriticalSection(&m_cContractedUnitsSection);

			if ( bNew )
			{
				if( gUnitLogLevel >= 3 ) logBBAIForTeam(GET_PLAYER((PlayerTypes)m_eOwner).getTeam(), "      Unit %S (%d) at (%d,%d) already responding to contract at (%d,%d)",
												 pLoopSelectionGroup->getHeadUnit()->getDescription().GetCString(),
												 pLoopSelectionGroup->getHeadUnit()->getID(),
												 pLoopSelectionGroup->getX(), 
												 pLoopSelectionGroup->getY(), 
												 iAtX, iAtY);

				if ( iUnitStrengthTimes100 == -1 )
				{
					//	Request already handled by existing mission
					return;
				}
				else
				{
					int iMissionGroupStrengthTimes100 = pLoopSelectionGroup->AI_getGenericValueTimes100(unitCapabilities2UnitValueFlags(eUnitFlags));
					if ( iMissionGroupStrengthTimes100 >= iUnitStrengthTimes100 )
					{
						//	Request is entirely fulfilled by existing mission
						return;
					}
					else
					{
						//	It's partially fulfilled so lower the priority of the remainder
						iPriority = lowerPartiallyFulfilledRequestPriority(iPriority, iUnitStrengthTimes100, iMissionGroupStrengthTimes100);
						iUnitStrengthTimes100 -= iMissionGroupStrengthTimes100;
					}
				}
			}
		}
	}

	newRequest.iPriority				= iPriority;
	newRequest.eUnitFlags				= eUnitFlags;
	newRequest.eAIType					= eAIType;
	newRequest.iAtX						= iAtX;
	newRequest.iAtY						= iAtY;
	newRequest.iMaxPath					= iMaxPath;
	newRequest.iWorkRequestId			= ++m_iNextWorkRequestId;
	newRequest.bFulfilled				= false;
	newRequest.iRequiredStrengthTimes100 = iUnitStrengthTimes100;
	if ( criteria != NULL )
	{
		newRequest.criteria				= *criteria;
	}

	//OutputDebugString(CvString::format("Adding new work request, index %d with priority %d\n", newRequest.iWorkRequestId, iPriority).c_str());

	if ( pJoinUnit == NULL )
	{
		newRequest.iUnitId	= -1;
	}
	else
	{
		newRequest.iUnitId	= pJoinUnit->getID();
	}

	//	Insert in priority order, highest first
	std::vector<workRequest>::iterator insertAt;

	EnterCriticalSection(&m_cWorkRequestsSection);

	for( insertAt = m_workRequests.begin(); insertAt != m_workRequests.end(); ++insertAt )
	{
		if ( iPriority > (*insertAt).iPriority )
		{
			break;
		}
	}

	{
		MEMORY_TRACK_EXEMPT();

		m_workRequests.insert(insertAt, newRequest);
	}

	LeaveCriticalSection(&m_cWorkRequestsSection);
}

//	Advertise a tender to build units
//		iMinPriority indicates the lowest priority request this tender is appropriate for
void	CvContractBroker::advertiseTender(CvCity* pCity, int iMinPriority)
{
	PROFILE_FUNC();

	if( gCityLogLevel >= 3 ) logBBAIForTeam(GET_PLAYER((PlayerTypes)m_eOwner).getTeam(), "      City %S tenders for unit builds at priority %d", pCity->getName().GetCString(), iMinPriority);
	
	cityTender	newTender;

	newTender.iMinPriority		= iMinPriority;
	newTender.iCityId			= pCity->getID();

	{
		MEMORY_TRACK_EXEMPT();

		m_advertisingTenders.push_back(newTender);
	}
}

//	Find out how many requests have already been made for units of a specified AI type
//	This is used by cities requesting globally needed units like settlers to avoid multiple
//	tenders all ocurring at once
int		CvContractBroker::numRequestsOutstanding(UnitAITypes eUnitAI, bool bAtCityOnly) const
{
	int iCount = 0;

	EnterCriticalSection(&m_cWorkRequestsSection);

	for(int iI = 0; iI < (int)m_workRequests.size(); iI++)
	{
		if ( !m_workRequests[iI].bFulfilled )
		{
			if ( m_workRequests[iI].eAIType == eUnitAI )
			{
				CvCity* targetCity = GC.getMapINLINE().plotINLINE(m_workRequests[iI].iAtX, m_workRequests[iI].iAtY)->getPlotCity();

				if ( !bAtCityOnly || (targetCity != NULL && targetCity->getOwnerINLINE() == m_eOwner) )
				{
					iCount++;
				}
			}
		}
	}

	LeaveCriticalSection(&m_cWorkRequestsSection);

	return iCount;
}

void CvContractBroker::finalizeTenderContracts(void)
{
	PROFILE_FUNC();

	std::map<int,int>	tenderAllocations;

	CvPathGenerator::EnableMaxPerformance(true);

	//	No need to lock here - this is always run in a single threaded context

	for(int iI = 0; iI < (int)m_workRequests.size(); iI++)
	{
		if ( !m_workRequests[iI].bFulfilled )
		{
			int iBestValue = 0;
 			//TB OOS Debug: from here we were not given a beginning definition
			int iBestCityTenderKey = 0;
			int iValue = 0;
			UnitTypes eUnit = NO_UNIT;
			//to here
			UnitTypes eBestUnit = NO_UNIT;
			UnitAITypes eBestAIType = NO_UNITAI;
			CvCity*	pBestCity = NULL;
			CvPlot* pDestPlot = GC.getMapINLINE().plotINLINE(m_workRequests[iI].iAtX, m_workRequests[iI].iAtY);

			if( gCityLogLevel >= 3 )
			{
				CvString	unitAIType;

				if ( m_workRequests[iI].eAIType == NO_UNITAI )
				{
					unitAIType = "NO_UNITAI";
				}
				else
				{
					CvInfoBase& AIType = GC.getUnitAIInfo(m_workRequests[iI].eAIType);
					unitAIType = AIType.getType();
				}

				CvString szCriteriaDescription = m_workRequests[iI].criteria.getDescription();

				if ( !szCriteriaDescription.empty() )
				{
					szCriteriaDescription.Format(" (%s)", szCriteriaDescription.c_str());
				}

				logBBAIForTeam(GET_PLAYER((PlayerTypes)m_eOwner).getTeam(), "      Processing bids for tender for unitAI %s at (%d,%d) with priority %d%s",
						unitAIType.c_str(),
						m_workRequests[iI].iAtX, m_workRequests[iI].iAtY,
						m_workRequests[iI].iPriority,
						szCriteriaDescription.c_str());
			}

			for(unsigned int iJ = 0; iJ < m_advertisingTenders.size(); iJ++)
			{
				if ( m_advertisingTenders[iJ].iMinPriority <= m_workRequests[iI].iPriority )
				{
					CvCity* pCity = GET_PLAYER(m_eOwner).getCity(m_advertisingTenders[iJ].iCityId);

					if ( pCity != NULL && pDestPlot != NULL )
					{
						if ( pCity->area() == pDestPlot->area() || (pDestPlot->getPlotCity() != NULL && pCity->waterArea() == pDestPlot->getPlotCity()->waterArea()) )
						{
							int	iTendersAlreadyInProcess = pCity->numQueuedUnits(m_workRequests[iI].eAIType, pDestPlot);
							int iTenderAllocationKey = 0;
							//unsigned int iAnyAITenderAllocationKey = 0;

							CvChecksum xSum;

							xSum.add(pCity->getID());
							xSum.add(GC.getMapINLINE().plotNumINLINE(m_workRequests[iI].iAtX, m_workRequests[iI].iAtY));
							xSum.add((int)m_workRequests[iI].eAIType);

							iTenderAllocationKey = xSum.get();

							std::map<int, int>::const_iterator itr = tenderAllocations.find(iTenderAllocationKey);
							if ( itr != tenderAllocations.end() )
							{
								iTendersAlreadyInProcess -= itr->second;
							}
							else
							{
								tenderAllocations[iTenderAllocationKey] = 0;
							}

							FAssert(iTendersAlreadyInProcess >= 0);

							if ( iTendersAlreadyInProcess <= 0 )
							{
 //TB OOS Debug: Undefined variable being called as a pointer in a later function call
								UnitAITypes eAIType = NO_UNITAI;

								if ( m_workRequests[iI].eAIType == NO_UNITAI )
								{
									UnitAITypes*	pUnitAIs = NULL;
									int				iNumAIs = -1;

									if ( (m_workRequests[iI].eUnitFlags & DEFENSIVE_UNITCAPABILITIES) != 0 )
									{
										static UnitAITypes defensiveAIs[] = { UNITAI_CITY_DEFENSE, UNITAI_ATTACK };
										pUnitAIs = defensiveAIs;
										iNumAIs = sizeof(defensiveAIs)/sizeof(UnitAITypes);
									}
									if ( (m_workRequests[iI].eUnitFlags & OFFENSIVE_UNITCAPABILITIES) != 0 )
									{
										static UnitAITypes offensiveAIs[] = { UNITAI_ATTACK };
										pUnitAIs = offensiveAIs;
										iNumAIs = sizeof(offensiveAIs)/sizeof(UnitAITypes);
									}
									if ( (m_workRequests[iI].eUnitFlags & WORKER_UNITCAPABILITIES) != 0 )
									{
										static UnitAITypes workerAIs[] = { UNITAI_WORKER };
										pUnitAIs = workerAIs;
										iNumAIs = sizeof(workerAIs)/sizeof(UnitAITypes);
									}

									eUnit = pCity->AI_bestUnit(iValue, iNumAIs, pUnitAIs, false, &eAIType, false, true, &m_workRequests[iI].criteria);
								}
								else
								{
									eAIType = m_workRequests[iI].eAIType;
									if ( pCity->area() != pDestPlot->area() && !IS_NAVAL_AITYPE(eAIType) && !IS_AIR_AITYPE(eAIType) )
									{
										continue;
									}

									eUnit = pCity->AI_bestUnitAI(eAIType, iValue, false, false, &m_workRequests[iI].criteria);
								}
								if ( eUnit != NO_UNIT )
								{
									//	Adjust value for production time and distance
									int iTurns;
									int iBaseValue = iValue;
									
									if ( (pCity->isProduction() && pCity->getOrderData(0).eOrderType == ORDER_TRAIN) )
									{
										iTurns = pCity->getTotalProductionQueueTurnsLeft() + pCity->getProductionTurnsLeft(eUnit, 1);
									}
									else
									{
										iTurns = pCity->getProductionTurnsLeft(eUnit, 1);
									}

									//	Decrease the value 10% per (standard speed) turn
									iValue *= 100 - 10*std::min((iTurns*100)/GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getTrainPercent(),10);
									iValue /= 100;

									if ( iValue > 0 )
									{
										//	Decrease by 5% per turn of separation from destination
										//int iDistance = GC.getMapINLINE().calculatePathDistance(pCity->plot(),pDestPlot);
										int iMaxPath = m_workRequests[iI].iMaxPath;
										int iMaxWinPath = std::max(0,20 - (20*iBestValue)/iValue);

										if ( iMaxPath > iMaxWinPath )
										{
											iMaxPath = iMaxWinPath;
										}

										if ( CvSelectionGroup::getPathGenerator()->generatePathForHypotheticalUnit(pCity->plot(), pDestPlot, m_eOwner, eUnit, MOVE_NO_ENEMY_TERRITORY, iMaxPath == MAX_INT ? 19 : iMaxPath) )
										{
											int iDistance = CvSelectionGroup::getPathGenerator()->getLastPath().length();
											iValue *= 100 - 5*std::min(20, iDistance);
											iValue /= 100;
											
											if( gCityLogLevel >= 3 )
											{
												logBBAIForTeam(GET_PLAYER((PlayerTypes)m_eOwner).getTeam(), "      City %S could supply unit %S with base value %d, depreciated value (after %d turn production at distance %d) to %d",
														pCity->getName().GetCString(),
														GC.getUnitInfo(eUnit).getDescription(),
														iBaseValue,
														iTurns,
														iDistance,
														iValue);
											}

											if ( iValue > iBestValue )
											{
												iBestValue = iValue;
												iBestCityTenderKey = iTenderAllocationKey;
												eBestUnit = eUnit;
												eBestAIType = eAIType;
												pBestCity = pCity;
											}
										}
									}
								}
								else
								{
									if( gCityLogLevel >= 3 )
									{
										logBBAIForTeam(GET_PLAYER((PlayerTypes)m_eOwner).getTeam(), "      City %S has no suitable units to offer",
												pCity->getName().GetCString());
									}
								}
							}
							else
							{
								//	Already being built
								m_workRequests[iI].bFulfilled = true;
								eBestUnit = NO_UNIT;

								tenderAllocations[iTenderAllocationKey] = tenderAllocations[iTenderAllocationKey] + 1;
								
								if( gCityLogLevel >= 3 )
								{
									logBBAIForTeam(GET_PLAYER((PlayerTypes)m_eOwner).getTeam(), "      City %S is already building a unit",
											pCity->getName().GetCString());
								}
								break;
							}
						}
					}
				}
			}

			if ( eBestUnit != NO_UNIT )
			{
				if( gCityLogLevel >= 2 )
				{
					CvString	unitAIType;

					if ( m_workRequests[iI].eAIType == NO_UNITAI )
					{
						unitAIType = "NO_UNITAI";
					}
					else
					{
						CvInfoBase& AIType = GC.getUnitAIInfo(m_workRequests[iI].eAIType);
						unitAIType = AIType.getType();
					}

					logBBAIForTeam(GET_PLAYER((PlayerTypes)m_eOwner).getTeam(), "      City %S wins business for unitAI build %s (training %S)",
							pBestCity->getName().GetCString(),
							unitAIType.c_str(),
							GC.getUnitInfo(eBestUnit).getDescription());
				}
				
				m_workRequests[iI].bFulfilled = true;
				tenderAllocations[iBestCityTenderKey] = tenderAllocations[iBestCityTenderKey] + 1;

				//	Queue up the build.  Add to queue head if the current build is not a unit (implies
				//	a local build below the priority of work the city tendered for)
				bool bAppend = (pBestCity->isProduction() && pBestCity->getOrderData(0).eOrderType == ORDER_TRAIN);

				pBestCity->pushOrder(ORDER_TRAIN,
									 eBestUnit,
									 eBestAIType,
									 false,
									 !bAppend,
									 bAppend,
									 false,
									 pDestPlot,
									 m_workRequests[iI].eAIType,
									 (m_workRequests[iI].iUnitId == -1 ? 0 : AUX_CONTRACT_FLAG_IS_UNIT_CONTRACT));
			}
		}
	}

	CvPathGenerator::EnableMaxPerformance(false);

	if( gUnitLogLevel >= 3 )
	{
		int	iIdleUnits = 0;
		int iEmployedUnits = 0;
		int	iSatisfiedContracts = 0;
		int	iI;

		for(iI = 0; iI < (int)m_workRequests.size(); iI++)
		{
			if ( m_workRequests[iI].bFulfilled )
			{
				iSatisfiedContracts++;
			}
		}

		for(iI = 0; iI < (int)m_advertisingUnits.size(); iI++)
		{
			if ( m_advertisingUnits[iI].iContractedWorkRequest == -1 )
			{
				iIdleUnits++;
			}
			else
			{
				iEmployedUnits++;
			}
		}

		logBBAIForTeam(GET_PLAYER((PlayerTypes)m_eOwner).getTeam(), "%d out of %d contracts satisfied, %d unit employed, %d left without work", iSatisfiedContracts, m_workRequests.size(), iEmployedUnits, iIdleUnits);
	}
}

//	Make a contract
//	This will attempt to make the best contracts between currently
//	advertising units and work, then search the resuilting set for the work 
//	of the requested unit
//	returns true if a contract is made along with the details of what to do
bool	CvContractBroker::makeContract(CvUnit* pUnit, int& iAtX, int& iAtY, CvUnit*& pJoinUnit, bool bThisPlotOnly)
{
	PROFILE_FUNC();

	int			iI;

	//	Currently no need to lock here as this is (currently) always run in a single-threaded context

	//	Satisfy the highest priority requests first (sort order of m_workRequests)
	for(iI = 0; iI < (int)m_workRequests.size(); iI++)
	{
		if ( !m_workRequests[iI].bFulfilled )
		{
			bool	bFound;

			//	If this is a request to join a unit check the unit is still in a joinable state
			//	(ie - it has not joined someone else!)
			if ( m_workRequests[iI].iUnitId != -1 )
			{
				CvUnit*	pTargetUnit = findUnit(m_workRequests[iI].iUnitId);

				if ( pTargetUnit == NULL || pTargetUnit->getGroup()->getHeadUnit() != pTargetUnit )
				{
					m_workRequests[iI].bFulfilled = true;
					continue;
				}
			}

			do
			{
				bFound = false;

				advertisingUnit*	suitableUnit = findBestUnit( m_workRequests[iI], bThisPlotOnly );

				if ( NULL != suitableUnit )
				{
					CvUnit*		pUnit = findUnit(suitableUnit->iUnitId);

					if ( pUnit != NULL )
					{
						bFound = true;

						suitableUnit->iContractedWorkRequest = m_workRequests[iI].iWorkRequestId;

						int iUnitStrengthTimes100 = pUnit->AI_genericUnitValueTimes100(unitCapabilities2UnitValueFlags(m_workRequests[iI].eUnitFlags));
						if ( m_workRequests[iI].iRequiredStrengthTimes100 == -1 || iUnitStrengthTimes100 >= m_workRequests[iI].iRequiredStrengthTimes100 )
						{
							//	Request is entirely fulfilled by this unit
							m_workRequests[iI].bFulfilled = true;

							//OutputDebugString(CvString::format("work request %d satisfied by unit %d\n", m_workRequests[iI].iWorkRequestId, suitableUnit->iUnitId).c_str());
						}
						else
						{
							//	It's partially fulfilled so lower the priority of the remainder
							m_workRequests[iI].iPriority = lowerPartiallyFulfilledRequestPriority(m_workRequests[iI].iPriority, m_workRequests[iI].iRequiredStrengthTimes100, iUnitStrengthTimes100);
							m_workRequests[iI].iRequiredStrengthTimes100 -= iUnitStrengthTimes100;

							//OutputDebugString(CvString::format("work request %d partially satisfied by unit %d\n", m_workRequests[iI].iWorkRequestId, suitableUnit->iUnitId).c_str());
						}
					}
				}
			} while( bFound && !m_workRequests[iI].bFulfilled );
		}
	}

	for(iI = 0; iI < (int)m_advertisingUnits.size(); iI++)
	{
		//	Note that all existing advertising units have attempted to match
		//	against existing work requests
		if ( bThisPlotOnly )
		{
			m_advertisingUnits[iI].iMatchedToRequestSeqThisPlot = m_iNextWorkRequestId;
		}
		else
		{
			m_advertisingUnits[iI].iMatchedToRequestSeqAnyPlot = m_iNextWorkRequestId;
		}
	}

	//	Now see if this unit has work assigned
	for(iI = 0; iI < (int)m_advertisingUnits.size(); iI++)
	{
		if ( m_advertisingUnits[iI].iUnitId == pUnit->getID() )
		{
			int	iWorkRequest = m_advertisingUnits[iI].iContractedWorkRequest;

			if ( -1 != iWorkRequest )
			{
				const workRequest*	contractedRequest = findWorkRequest( iWorkRequest );

				FAssert(NULL != contractedRequest);

				iAtX		= contractedRequest->iAtX;
				iAtY		= contractedRequest->iAtY;

				pJoinUnit = findUnit(contractedRequest->iUnitId);
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	return false;
}

const workRequest*	CvContractBroker::findWorkRequest(int iWorkRequestId) const
{
	PROFILE_FUNC();

	for(int iI = 0; iI < (int)m_workRequests.size(); iI++ )
	{
		if ( m_workRequests[iI].iWorkRequestId == iWorkRequestId )
		{
			return &m_workRequests[iI];
		}
	}

	return NULL;
}

advertisingUnit*	CvContractBroker::findBestUnit(workRequest& request, bool bThisPlotOnly)
{
	PROFILE_FUNC();

	int	iBestValue = 0;
	int iBestUnitIndex = -1;

	for(int iI = 0; iI < (int)m_advertisingUnits.size(); iI++)
	{
		advertisingUnit&	unitInfo = m_advertisingUnits[iI];
		int	iValue = 0;

		//OutputDebugString(CvString::format("Assess unit %d suitability for work request %d\n", unitInfo.iUnitId, request.iWorkRequestId).c_str());
		//	Don't bother recalculating this advertiser/requestor pair if they have already been
		//	calculated previously
		if ( (bThisPlotOnly ? unitInfo.iMatchedToRequestSeqThisPlot : unitInfo.iMatchedToRequestSeqAnyPlot) < request.iWorkRequestId &&
			 unitInfo.iContractedWorkRequest == -1 )
		{
			CvUnit* pLoopUnit = findUnit(unitInfo.iUnitId);

			if ( pLoopUnit != NULL && pLoopUnit->meetsUnitSelectionCriteria(&request.criteria) && unitInfo.iMinPriority <= request.iPriority)
			{
				if ( (request.eUnitFlags & WORKER_UNITCAPABILITIES) == 0 )
				{
					if ( request.eAIType == NO_UNITAI || pLoopUnit->getUnitInfo().getUnitAIType(request.eAIType) )
					{
						iValue += 10;

						if ( unitInfo.iDefensiveValue > 0 && (request.eUnitFlags == 0 || (request.eUnitFlags & DEFENSIVE_UNITCAPABILITIES) != 0) )
						{
							iValue += unitInfo.iDefensiveValue;
						}
						if ( unitInfo.iDefensiveValue > 0 && (request.eUnitFlags == 0 || (request.eUnitFlags & OFFENSIVE_UNITCAPABILITIES) != 0) )
						{
							iValue += unitInfo.iOffensiveValue;
						}
					}
				}
				else if ( unitInfo.bIsWorker )
				{
					iValue = 100;
				}

				if ( iValue*1000 > iBestValue )
				{
					CvPlot*	pTargetPlot = GC.getMapINLINE().plotINLINE(request.iAtX, request.iAtY);
					int		iPathTurns = 0;
					int		iMaxPathTurns = std::min((request.iPriority > LOW_PRIORITY_ESCORT_PRIORITY ? MAX_INT : 10), (iBestValue == 0 ? MAX_INT : (1000*iValue)/iBestValue));

					if ( request.iMaxPath < iMaxPathTurns )
					{
						iMaxPathTurns = request.iMaxPath;
					}

					//	For low priority work never try to satisfy it with a distant unit
					if ( pLoopUnit != NULL &&
						 (pLoopUnit->atPlot(pTargetPlot) ||
						  (!bThisPlotOnly && pLoopUnit->generatePath(pTargetPlot,
																	 MOVE_SAFE_TERRITORY | MOVE_AVOID_ENEMY_UNITS,
																	 true,
																	 &iPathTurns,
																	 iMaxPathTurns))) )
					{
						iValue *= 1000;
						iValue /= (iPathTurns+1);

						if ( iValue > iBestValue )
						{
							iBestValue		= iValue;
							iBestUnitIndex	= iI;
						}
					}
				}
			}
		}
	}

	if ( iBestUnitIndex == -1 )
	{
		return NULL;
	}
	else
	{
		return &m_advertisingUnits[iBestUnitIndex];
	}
}

CvUnit* CvContractBroker::findUnit(int iUnitId) const
{
	if ( iUnitId == -1 )
	{
		return NULL;
	}
	else
	{
		return GET_PLAYER((PlayerTypes)m_eOwner).getUnit(iUnitId);
	}
}

int	CvContractBroker::lowerPartiallyFulfilledRequestPriority(int iPreviousPriority, int iPreviousRequestStrength, int iStrengthProvided)
{
	return (iPreviousPriority*(iPreviousRequestStrength-iStrengthProvided))/iPreviousRequestStrength;
}

UnitValueFlags	CvContractBroker::unitCapabilities2UnitValueFlags(unitCapabilities eCapabilities)
{
	UnitValueFlags	valueFlags = (UnitValueFlags)0;

	if ( (eCapabilities & DEFENSIVE_UNITCAPABILITIES) != 0 )
	{
		valueFlags |= UNITVALUE_FLAGS_DEFENSIVE;
	}
	if ( (eCapabilities & OFFENSIVE_UNITCAPABILITIES) != 0 )
	{
		valueFlags |= UNITVALUE_FLAGS_OFFENSIVE;
	}

	if ( valueFlags == 0 )
	{
		valueFlags = UNITVALUE_FLAGS_ALL;
	}

	return valueFlags;
}
