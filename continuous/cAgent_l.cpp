/*
 *   A robot may not injure a human being or, through inaction, allow a human being to come to harm.
 *   A robot must obey the orders given to it by human beings, except where such orders would conflict with the First Law.
 *   A robot may not injure its own kind and defend its own kind unless it is interfering with the first or second rule.
 *
 */



#include <iostream>
#include <algorithm>
#include <cmath>
#include "cAgent_l.h"

//////////////////////////////////////////////////
////////////////// constructor ///////////////////
//////////////////////////////////////////////////
cAgent_l::cAgent_l(int states, int number_of_actuators) :
	cAgent( states, number_of_actuators)
{
	// initialize ElegTraces

	m_ValueElegibilityTrace		= new double[states];

	m_MeanElegibilityTrace		= new double*[m_number_of_actuators];
	m_StdDevElegibilityTrace	= new double*[m_number_of_actuators];

	for(int i=0; i<m_number_of_actuators; i++)
	{
		m_StdDevElegibilityTrace[i] = new double[states];
		m_MeanElegibilityTrace[i] = new double[states];
	}

	m_lambda    = 0.5;	// association of current reward with recent actions
}
//////////////////////////////////////////////////
////////////////// destructor ////////////////////
//////////////////////////////////////////////////
cAgent_l::~cAgent_l()
{
	for(int i=0; i<m_number_of_actuators; i++)
	{
		delete[] m_MeanElegibilityTrace[i];
		delete[] m_StdDevElegibilityTrace[i];
	}

	delete[] m_MeanElegibilityTrace;
	delete[] m_StdDevElegibilityTrace;
	delete[] m_ValueElegibilityTrace;
}

//////////////////////////////////////////////////
////////////// get and set methods ///////////////
//////////////////////////////////////////////////


// Trace decay lambda
// lambda takes values of [0,1], where 0 = no Traces, 1 = long Trace(pseudo Monte Carlo)
//---------

double cAgent_l::getTraceDecay()
{
	return m_lambda;
}

bool cAgent_l::setTraceDecay(double lambda)
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

std::vector<double>	cAgent_l::getMeanTrace(int actuator)
{
	if(actuator < m_number_of_actuators)
		return std::vector<double>(m_MeanElegibilityTrace[actuator],m_MeanElegibilityTrace[actuator]+m_number_of_states);
	else
		return std::vector<double>(0);
}

std::vector<double>	cAgent_l::getStdDevTrace(int actuator)
{
	if(actuator < m_number_of_actuators)
		return std::vector<double>(m_StdDevElegibilityTrace[actuator],m_StdDevElegibilityTrace[actuator]+m_number_of_states);
	else
		return std::vector<double>(0);
}

std::vector<double>	cAgent_l::getStateTrace()
{
	return std::vector<double>(m_ValueElegibilityTrace,m_ValueElegibilityTrace+m_number_of_states);
}


//////////////////////////////////////////////////
//////////////// learning methods ////////////////
//////////////////////////////////////////////////

void cAgent_l::learn()
{
	double delta; // TD error
	double sigma; // std deviation

	delta = m_reward + m_discount * m_vStatePrms[m_curState] - m_vStatePrms[m_lastState];

	m_ValueElegibilityTrace[m_lastState]  = 1;
	for(int i=0; i<m_number_of_actuators; i++)
	{
		m_MeanElegibilityTrace[i][m_lastState]   += (m_lastAction[i]-m_vActionMeanPrms[i][m_lastState]);
		sigma = std::max( 1.0, exp( m_vActionStdDevPrms[i][m_lastState] + log(m_initialActionStdDev) ) );
		m_StdDevElegibilityTrace[i][m_lastState] += ( pow( (m_lastAction[i]-m_vActionMeanPrms[i][m_lastState]) / sigma, 2 ) -1 );
	}

	// update Traces and weights
	for(int state=0; state < m_number_of_states; state++)
	{

		m_vStatePrms[state] += m_learnRate * delta * m_ValueElegibilityTrace[state];
		m_ValueElegibilityTrace[state] *= m_lambda;

		for(int i=0; i<m_number_of_actuators; i++)
		{
			m_vActionMeanPrms[i][state]		+= m_ActionLearnRate * delta * m_MeanElegibilityTrace[i][state];
			m_MeanElegibilityTrace[i][state] *= m_lambda;
			m_vActionStdDevPrms[i][state]	+= m_ActionLearnRate * delta * m_StdDevElegibilityTrace[i][state];
			m_StdDevElegibilityTrace[i][state] *= m_lambda;
		}
	}
}


