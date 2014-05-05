// Sarsa(lambda)

#include "Agent_l.h"


class SAgent_l : public Agent_l
{
public:

				SAgent_l(int states = 8, int actions = 4, int actionMemory = 0);
				~SAgent_l();

virtual	void	learn();

};
