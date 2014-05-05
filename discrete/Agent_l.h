// Lambda Agent

#include "Agent.h"

class Agent_l : public Agent
{
protected:
	// variables
	double 	m_lambda;

	double**	m_ElegibilityTrace;


public:
				Agent_l(int states = 8, int actions = 4, int actionMemory = 0);
				~Agent_l();

		bool	setTraceDecay(double lambda);
		double	getTraceDecay();

virtual	double	getT(int state, int action);
virtual	void	learn() = 0;
};
