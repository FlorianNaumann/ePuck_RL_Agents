// Lambda Agent

#include "cAgent.h"

class cAgent_l : public cAgent
{
protected:
	// variables
	double 	m_lambda;

	double**	m_MeanElegibilityTrace;
	double**	m_StdDevElegibilityTrace;

	double*		m_ValueElegibilityTrace;


public:
				cAgent_l(int states = 8, int number_of_actuators = 2);
				~cAgent_l();

		bool	setTraceDecay(double lambda);
		double	getTraceDecay();

		std::vector<double> getStdDevTrace(int actuator);
		std::vector<double>	getMeanTrace(int actuator);
		std::vector<double>	getStateTrace();

virtual	void	learn();
};
