// continuous Actions Sarsa(lambda)

#include "cAgent_l.h"


class cSAgent_l  : public cAgent_l
{
public:

				cSAgent_l(int states = 8, int actionMemory = 0);
				~cSAgent_l();

virtual	void	learn();

};
