/*
 *   A robot may not injure a human being or, through inaction, allow a human being to come to harm.
 *   A robot must obey the orders given to it by human beings, except where such orders would conflict with the First Law.
 *   A robot may not injure its own kind and defend its own kind unless it is interfering with the first or second rule.
 *
 */

#include "SAgent.h"


#include <algorithm>
#include <cmath>


//////////////////////////////////////////////////
////////////////// constructor ///////////////////
//////////////////////////////////////////////////
SAgent::SAgent(const int number_of_states,const int number_of_actions, const int action_memory) :
	Agent( number_of_states, number_of_actions, action_memory)
{}
//////////////////////////////////////////////////
////////////////// destructor ////////////////////
//////////////////////////////////////////////////
SAgent::~SAgent(){}

//////////////////////////////////////////////////
//////////////// learning methods ////////////////
//////////////////////////////////////////////////
void SAgent::learn()
{
	// TD error
	double delta =	m_reward
					+ m_discount *  m_QMatrix[m_curState][m_curAction]
					- m_QMatrix[m_lastState][m_lastAction];

	m_QMatrix[m_lastState][m_lastAction] += m_learnRate * delta;
}
