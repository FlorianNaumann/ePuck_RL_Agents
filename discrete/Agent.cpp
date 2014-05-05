/*
 *   A robot may not injure a human being or, through inaction, allow a human being to come to harm.
 *   A robot must obey the orders given to it by human beings, except where such orders would conflict with the First Law.
 *   A robot may not injure its own kind and defend its own kind unless it is interfering with the first or second rule.
 *
 */

#include "Agent.h"

#include <iostream>
#include <algorithm>
#include <cmath>


//////////////////////////////////////////////////
////////////////// constructor ///////////////////
//////////////////////////////////////////////////
Agent::Agent(const int number_of_states,const int number_of_actions, const int actionMemory) :
	m_number_of_states(number_of_states),
	m_number_of_actions(number_of_actions),
	m_actionMemory(actionMemory),
	m_actionStates(m_number_of_states*(pow(m_number_of_actions,m_actionMemory)))
{
	// initialize qMatrix & ElegTraces
	m_QMatrix          = new double*[m_actionStates];

	for(int i=0; i<m_actionStates; i++)
	{
		m_QMatrix[i]          = new double[number_of_actions];
	}

	m_curState  = 0x0; // alle Sensoren sehen keine Linie
	m_lastState = m_curState;

	m_curAction  = 0x0; // stehen
	m_lastAction = m_curAction;

	m_epsilon 	= 0.15;	// curiosity
	m_discount  = 0.3;	// foresight
	m_learnRate = 0.15;	// learnRate

	m_reward = 0;
}
//////////////////////////////////////////////////
////////////////// destructor ////////////////////
//////////////////////////////////////////////////
Agent::~Agent()
{
	for(int i=0; i<m_actionStates; i++)
	{
		delete[] m_QMatrix[i];
	}
	delete[] m_QMatrix;
}

//////////////////////////////////////////////////
////////////// get and set methods ///////////////
//////////////////////////////////////////////////


// Learn Rate alpha
// alpha takes values of [0,1] and describes the impact of new experiences on learned values
// high learnRates let the robot learn fast but settle slowly
//------------------
double Agent::getLearnRate()
{
	return m_learnRate;
}

bool Agent::setLearnRate(double alpha)
{
	if( 0 <= alpha && alpha <= 1)
		m_learnRate = alpha;
	else
		return false;

	return true;
}

// Discount gamma
// gamma takes values of [0,1] and denotes the importance of expected future rewards
// 0 = #YOLO , 1 = chinese student
//---------------------------------
double Agent::getDiscount()
{
	return m_discount;
}

bool Agent::setDiscount(double gamma)
{
	if( 0 <= gamma && gamma <= 1)
		m_discount = gamma;
	else
		return false;

	return true;
}

// curiosity or greed
// epsilon takes values of [0,1] and is the probability of taking random actions instead of the best possible one
// 1 = max exploration, 0 = max exploitation
//--------------------
double Agent::getCuriosity()
{
	return m_epsilon;
}

bool Agent::setCuriosity(double curiosity)
{
	if( 0 <= curiosity && curiosity <= 1)
		m_epsilon = curiosity;
	else
		return false;

	return true;
}

//////////////////////////////////////////////////
////////// output of learned values //////////////
//////////////////////////////////////////////////

double	Agent::getQ(int state, int action)
{
	if(state < m_actionStates)
		return m_QMatrix[state][action];
	else
		std::cerr << "state is not part of the statespace" << std::endl;

	return -100;
}

// this is a dummy-function, designed to be reimplemented by Agents that incoorporate elegibility traces
double	Agent::getT(int state, int action)
{
	if(state < m_actionStates )
		if(state == m_curState && action == m_curAction)
			return 1;
		else
			return 0;
	else
		std::cerr << "this should not happen" << std::endl;

	return -100;
}

//////////////////////////////////////////////////
//////////////// learning methods ////////////////
//////////////////////////////////////////////////

bool Agent::setState(int state)
{
	// merge new state from the given state and the last action

	//    01...      m_curAction << (number of state bits)
	// +    010      + state
	//   ------
	//    01010      new state

	if( m_actionMemory == 1 ) // TODO allgemeine Form finden!
		state = ( m_curAction << (int)ceil(log2(m_number_of_states)) ) + state;

	// validation check:
	if( state < m_actionStates )
	{
		m_lastState = m_curState;
		m_curState  = state;

		return true;
	}
	else
		return false;
}


bool Agent::setReward(int reward)
{
	m_reward = reward;

	return true;
}

int Agent::getBestAction(int state)
{
	int bestAction = 0;

	if(state >= m_actionStates) // error! TODO not completely correct
	{
		std::cerr << "shitty" << std::endl;
		return 0;
	}

	for(int i=0; i < m_number_of_actions; i++)
	{
		if ( m_QMatrix[state][i] > m_QMatrix[state][bestAction] )
			bestAction = i;
	}

	return bestAction;
}

int Agent::getAction()
{
	int nextAction = 0;

	// Choose A from S using policy derived from Q (e.g., epsilon-greedy)
	if( random()%101 > 100*m_epsilon )
	{
		nextAction = getBestAction(m_curState);
	}
	else // be curious and do random stuff
	{
		nextAction = random()%m_number_of_actions;
	}

	m_lastAction = m_curAction;
	m_curAction  = nextAction;

	return nextAction;
}
