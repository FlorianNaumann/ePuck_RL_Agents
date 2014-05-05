// Sarsa(lambda)

#include "Agent_l.h"


class QAgent_l : public Agent_l
{
public:

				QAgent_l(int states = 8, int actions = 4, int actionMemory = 0);
				~QAgent_l();

virtual	void	learn();

};
