// Sarsa(lambda)

#include "Agent.h"


class SAgent : public Agent
{
public:

				SAgent(int states = 8, int actions = 4, int actionMemory = 0);
				~SAgent();

virtual	void	learn();

};
