/*
 *   A robot may not injure a human being or, through inaction, allow a human being to come to harm.
 *   A robot must obey the orders given to it by human beings, except where such orders would conflict with the First Law.
 *   A robot may not injure its own kind and defend its own kind unless it is interfering with the first or second rule.
 *
 */

#include "QAgent_l.h"


#include <algorithm>
#include <cmath>


//////////////////////////////////////////////////
////////////////// constructor ///////////////////
//////////////////////////////////////////////////
QAgent_l::QAgent_l(const int number_of_states,const int number_of_actions, const int action_memory) :
	Agent_l( number_of_states, number_of_actions, action_memory)
{}
//////////////////////////////////////////////////
////////////////// destructor ////////////////////
//////////////////////////////////////////////////
QAgent_l::~QAgent_l(){}

//////////////////////////////////////////////////
//////////////// learning methods ////////////////
//////////////////////////////////////////////////
void QAgent_l::learn()
{
	// TD error
	double delta =	m_reward
					+ m_discount *  m_QMatrix[m_curState][getBestAction(m_curState)]
					- m_QMatrix[m_lastState][m_lastAction];

	m_ElegibilityTrace[m_lastState][m_lastAction] += 1;

	for(int s=0; s < m_actionStates; s++)
	{
		for(int a=0; a < m_number_of_actions; a++)
		{
			m_QMatrix[s][a] += m_learnRate * delta * m_ElegibilityTrace[s][a];
			m_ElegibilityTrace[s][a] *= m_discount * m_lambda;
		}
	}
}
