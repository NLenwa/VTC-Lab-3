#ifndef _OBJECTS__H
#include "objects.h"
#endif

// Sell, buy variables
bool is_trade = false;
float response_price = 1000;
int response_count = 0;

float my_fuel;
int off_id;
int response_id;
float asking_price;
float my_price;

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