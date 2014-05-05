// Qlearning

#include "Agent.h"


class QAgent : public Agent
{
public:

				QAgent(int states = 8, int actions = 4, int actionMemory = 0);
				~QAgent();

virtual	void	learn();

};
