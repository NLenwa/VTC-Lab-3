#ifndef _OBJECTS__H
#include "objects.h"
#endif

enum transfer_types_2 { MONEY, FUEL };
extern enum frame_types;
extern float TransferSending(int ID_receiver, int transfer_type, float transfer_value);
extern float my_fuel;
extern float my_price;
extern int response_count;
extern float distance_item;
extern int off_id;
extern int response_id;
extern float response_price;
extern bool is_trade;


extern void AskForFuel();
extern void ReplyForTrade(int rec_id, float price);
extern void TradeAgree(int rec_id);

int tracked_item;

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