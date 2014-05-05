// basic reinforcement learning agent

class Agent
{
protected:
	// variables

const	int	m_number_of_states;
const	int	m_number_of_actions;
const	int	m_actionMemory;
const	int		m_actionStates;

		double	m_discount; // gamma
		double  m_learnRate;
		double  m_epsilon; // curiosity

		int		m_lastState, m_curState;
		int		m_lastAction, m_curAction;

		int		m_reward;

		double**	m_QMatrix;

		// functions
		int		getBestAction(int state);

public:
				Agent(int states = 8, int actions = 4, int actionMemory = 0);
				~Agent();

		bool	setDiscount(double gamma);
		double	getDiscount();
		bool	setLearnRate(double alpha);
		double	getLearnRate();
		bool	setCuriosity(double curiosity);
		double	getCuriosity();

		bool	setReward(int reward);
		bool	setState(int state);
virtual	double	getT(int state, int action);
		double	getQ(int state, int action);
virtual	void	learn() = 0;
		int		getAction();
};
