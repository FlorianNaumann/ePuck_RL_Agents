/*
 *   A robot may not injure a human being or, through inaction, allow a human being to come to harm.
 *   A robot must obey the orders given to it by human beings, except where such orders would conflict with the First Law.
 *   A robot may not injure its own kind and defend its own kind unless it is interfering with the first or second rule.
 *
 */

#include "cAgent.h"


#include <algorithm>
#include <cmath>
#include <iostream>
#include <cstring>
#include <ctime>


//////////////////////////////////////////////////
////////////////// constructor ///////////////////
//////////////////////////////////////////////////
cAgent::cAgent(const int number_of_states, const int number_of_actuators) :
	m_number_of_states(number_of_states),
	m_number_of_actuators(number_of_actuators)
{
	// initialize vectors
	m_vStatePrms		= new double[m_number_of_states];

	m_vActionMeanPrms	= new double*[m_number_of_actuators];
	m_vActionStdDevPrms	= new double*[m_number_of_actuators];

	for(int i=0; i<m_number_of_actuators; i++)
	{
		m_vActionMeanPrms[i]	= new double[m_number_of_states];
		m_vActionStdDevPrms[i]	= new double[m_number_of_states];
	}

	m_curState  = 0x0; // alle Sensoren sehen keine Linie
	m_lastState = m_curState;

	m_curAction  = std::vector<double>(m_number_of_actuators, 0.0);
	m_lastAction = std::vector<double>(m_number_of_actuators, 0.0);

	m_epsilon 	= 0.25;	// curiosity
	m_discount  = 0.5;	// foresight
	m_learnRate = 0.15;	// learnRate

	m_reward = 0;

	// continious parameters
	m_ActionLearnRate		= 0.01;
	m_initialActionStdDev	= 100;

	srand(time(NULL));
}
//////////////////////////////////////////////////
////////////////// destructor ////////////////////
//////////////////////////////////////////////////
cAgent::~cAgent()
{
	for(int i=0; i<m_number_of_actuators; i++)
	{
		delete[] m_vActionMeanPrms[i];
		delete[] m_vActionStdDevPrms[i];
	}
}

//////////////////////////////////////////////////
////////////// get and set methods ///////////////
//////////////////////////////////////////////////


// Learn Rate alpha
// alpha takes values of [0,1] and describes the impact of new experiences on learned values
// high learnRates let the robot learn fast but settle slowly
//------------------
double cAgent::getLearnRate()
{
	return m_learnRate;
}

bool cAgent::setLearnRate(double alpha)
{
	if( 0 <= alpha && alpha <= 1)
		m_learnRate = alpha;
	else
		return false;

	return true;
}


// Action Learn Rate alpha
// alpha takes values of [0,1] and describes the impact of new experiences on learned values
// high learnRates let the robot learn fast but settle slowly
//------------------
double cAgent::getActionLearnRate()
{
	return m_ActionLearnRate;
}

bool cAgent::setActionLearnRate(double alpha)
{
	if( 0 <= alpha && alpha <= 1)
		m_ActionLearnRate = alpha;
	else
		return false;

	return true;
}


// Discount gamma
// gamma takes values of [0,1] and denotes the importance of expected future rewards
// 0 = #YOLO , 1 = chinese student
//---------------------------------
double cAgent::getDiscount()
{
	return m_discount;
}

bool cAgent::setDiscount(double gamma)
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
double cAgent::getCuriosity()
{
	return m_epsilon;
}

bool cAgent::setCuriosity(double curiosity)
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


std::vector<double>	cAgent::getStdDevWeights(int actuator)
{
	if(actuator < m_number_of_actuators)
			return std::vector<double>(m_vActionStdDevPrms[actuator],m_vActionStdDevPrms[actuator]+m_number_of_states);
		else
			return std::vector<double>(0);
}


std::vector<double>	cAgent::getMeanWeights(int actuator)
{
	if(actuator < m_number_of_actuators)
		return std::vector<double>(m_vActionMeanPrms[actuator],m_vActionMeanPrms[actuator]+m_number_of_states);
	else
		return std::vector<double>(0);
}


//////////////////////////////////////////////////
//////////////// learning methods ////////////////
//////////////////////////////////////////////////

bool cAgent::setState(int state)
{
	// merge new state from the given state and the last action

	//    01...      m_curAction << (number of state bits)
	// +    010      + state
	//   ------
	//    01010      new state

	// validation check:
	if( state < m_number_of_states )
	{
		m_lastState = m_curState;
		m_curState  = state;

		return true;
	}
	else
		return false;
}


bool cAgent::setReward(int reward)
{
	m_reward = reward;

	return true;
}

// stolen from "http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform" and modified
#define TWO_PI 6.2831853071795864769252866
double cAgent::normalDistribution(const double &mean, const double &stdDev)
{
	static bool hasSpare = false;
	static double rand1, rand2;

	if(hasSpare)
	{
		hasSpare = false;
		return mean + stdDev * sqrt(rand1) * sin(rand2);
	}

	hasSpare = true;

	rand1 = rand() / ((double) RAND_MAX);
	if(rand1 < 1e-100) rand1 = 1e-100;
	rand1 = -2 * log(rand1);
	rand2 = (rand() / ((double) RAND_MAX)) * TWO_PI;

	return mean + stdDev * sqrt(rand1) * cos(rand2);
}


std::vector<double> cAgent::getAction()
{
	std::vector<double> nextAction = std::vector<double>(m_number_of_actuators, 0.0);

	for(int i=0; i<m_number_of_actuators; i++)
	{
		// select Action from normal distribution
		double mean		= m_vActionMeanPrms[i][m_curState];
		double stdDev	= std::max( 1.0, exp(m_vActionStdDevPrms[i][m_curState] + log(m_initialActionStdDev)) ); // >= 1

		nextAction[i] = normalDistribution( mean, stdDev );
	}

	m_lastAction = m_curAction;
	m_curAction  = nextAction;

	return nextAction;
}

void cAgent::learn()
{
	double delta; // TD error
	double sigma; // std deviation

	delta = m_reward + m_discount * m_vStatePrms[m_curState] - m_vStatePrms[m_lastState];

	m_vStatePrms[m_lastState] += m_learnRate * delta * 1;

	//std::cout << m_vActionMeanPrms[m_lastState] << std::endl;
	//std::cout << m_vActionStdDevPrms[m_lastState] << std::endl;

	for(int i=0; i<m_number_of_actuators; i++)
	{
		m_vActionMeanPrms[i][m_lastState]	+= m_ActionLearnRate * delta * (m_lastAction[i]-m_vActionMeanPrms[i][m_lastState]);

		sigma = std::max( 1.0, exp( m_vActionStdDevPrms[i][m_lastState] + log(m_initialActionStdDev) ) );
		m_vActionStdDevPrms[i][m_lastState]+= m_ActionLearnRate
											* delta
											* (	pow( (m_lastAction[i]-m_vActionMeanPrms[i][m_lastState]) / sigma, 2 ) -1 );
	}

}



