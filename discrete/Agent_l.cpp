/*
 *   A robot may not injure a human being or, through inaction, allow a human being to come to harm.
 *   A robot must obey the orders given to it by human beings, except where such orders would conflict with the First Law.
 *   A robot may not injure its own kind and defend its own kind unless it is interfering with the first or second rule.
 *
 */



#include <iostream>
#include <algorithm>
#include <cmath>
#include "Agent_l.h"

//////////////////////////////////////////////////
////////////////// constructor ///////////////////
//////////////////////////////////////////////////
Agent_l::Agent_l(const int number_of_states,const int number_of_actions, const int action_memory) :
	Agent( number_of_states, number_of_actions, action_memory)
{
	// initialize qMatrix & ElegTraces
	m_ElegibilityTrace = new double*[m_actionStates];

	for(int i=0; i<Agent::m_actionStates; i++)
	{
		m_ElegibilityTrace[i] = new double[number_of_actions];
	}

	m_lambda    = 0.5;	// association of current reward with recent actions
}
//////////////////////////////////////////////////
////////////////// destructor ////////////////////
//////////////////////////////////////////////////
Agent_l::~Agent_l()
{
	for(int i=0; i<Agent::m_actionStates; i++)
	{
		delete[] m_ElegibilityTrace[i];
	}
	delete[] m_ElegibilityTrace;
}

//////////////////////////////////////////////////
////////////// get and set methods ///////////////
//////////////////////////////////////////////////


// Trace decay lambda
// lambda takes values of [0,1], where 0 = no Traces, 1 = long Trace(pseudo Monte Carlo)
//---------

double Agent_l::getTraceDecay()
{
	return m_lambda;
}

bool Agent_l::setTraceDecay(double lambda)
{
	if( 0 <= lambda && lambda <= 1)
		m_lambda = lambda;
	else
		return false;

	return true;
}

//////////////////////////////////////////////////
////////// output of learned values //////////////
//////////////////////////////////////////////////


double	Agent_l::getT(int state, int action)
{
	if( state < m_actionStates )
		return m_ElegibilityTrace[state][action];
	else
		std::cerr << "this should not happen" << std::endl;

	return -100;
}

