#ifndef _OBJECTS__H
#include "objects.h"
#endif



extern void AskForFuel();
extern void ReplyForTrade(int rec_id, float price);
extern void TradeAgree(int rec_id);


class AutoPilot
{       
private:
	//float par[100]; // parametry pocz¹tkowe i parametry aktualne
	//long number_of_params;
public:
  AutoPilot();
  void AutoControl(MovableObject *ob);                        // pojedynczy krok sterowania
  void ControlTest(MovableObject *_ob,float krok_czasowy, float czas_proby); 
};