// basic reinforcement learning agent

#include <inttypes.h>
#include <vector>

class cAgent
{
protected:
	// variables

const	int		m_number_of_states;
const	int		m_number_of_actuators;

		double	m_discount; // gamma
		double  m_learnRate;
		double  m_epsilon; // curiosity

		// additional variables for continuous actions
		double  m_ActionLearnRate;
		double	m_initialActionStdDev;	// sigma_c
		double**	m_vActionMeanPrms;
		double**	m_vActionStdDevPrms;

		int		m_lastState, m_curState;
		std::vector<double>	m_lastAction, m_curAction;

		int		m_reward;

		double*	m_vStatePrms;

private:
		// functions
		double	normalDistribution(const double &mean = 0, const double &stdDev = 1);

public:
				cAgent(int states = 8, int number_of_actuators = 2);
				~cAgent();

		bool	setDiscount(double gamma);
		double	getDiscount();
		bool	setActionLearnRate(double alpha);
		double	getActionLearnRate();
		bool	setLearnRate(double alpha);
		double	getLearnRate();
		bool	setCuriosity(double curiosity);
		double	getCuriosity();

		bool	setReward(int reward);
		bool	setState(int state);

virtual	std::vector<double> getStdDevWeights(int actuator);
virtual	std::vector<double>	getMeanWeights(int actuator);

virtual	void	learn();
		std::vector<double>	getAction();
};
