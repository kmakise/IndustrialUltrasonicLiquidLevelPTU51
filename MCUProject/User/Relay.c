
#include "global.h"

void RelayTask(void)
{
	RL1OFF;
	while(1)
	{
		OSTimeDly(mS(100));	
		
		if(UsRLVal != 0 && UsRLVal < 100)
		{
			if(UsLevelmm > UsRLMax)
			{
				OSTimeDly(Sec(UsRLVal));
				RL1ON;
			}
			else if(UsLevelmm < UsRLMin)
			{
				OSTimeDly(Sec(UsRLVal));
				RL1ON;
			}
			else
			{
				OSTimeDly(Sec(UsRLVal));
				RL1OFF;
			}
		}
		else
			RL1OFF;
	}
}