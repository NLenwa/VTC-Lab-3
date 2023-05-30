#include <stdlib.h>
#include <time.h>

#include "agents.h"
#include "main.cpp"


AutoPilot::AutoPilot()
{
}

void AutoPilot::AutoControl(MovableObject *obj)
{
	Terrain *_terrain = obj->terrain;

	float pi = 3.1415;
	//podnoszenie_przedm = 1;
	Vector3 vect_local_forward = obj->state.qOrient.rotate_vector(Vector3(1, 0, 0));
	Vector3 vect_local_right = obj->state.qOrient.rotate_vector(Vector3(0, 0, 1));
	// TUTAJ NALE¯Y UMIEŒCIÆ ALGORYTM AUTONOMICZNEGO STEROWANIA POJAZDEM
	// .................................................................
	// .................................................................






	// BUY / SELL control code
	// when fuel if low, then ask to trade
	my_fuel = my_vehicle->state.amount_of_fuel;
	if (my_fuel < 10)
	{
		AskForFuel();
	}
	// when trade was initiated and have enough fuel for trade, then set selling price
	if (is_trade && my_vehicle->iID != off_id)
	{
		if (my_fuel > 40)
		{
			my_price = 100;
		}
		else if (my_fuel >= 15)
		{
			my_price = 10 * (100 - my_fuel);
		}
		ReplyForTrade(off_id, my_price);
	}

	// agree to deal and transfer payment
	// 1 for testing purposes, change for:
	//	(network_vehicles.size() / 10)
	if (response_count >= 1 || response_price == 100)
	{
		if (my_vehicle->state.money >= response_price)
		{
			TransferSending(response_id, MONEY, response_price);
			TradeAgree(response_id);
		}
	}
	// ************************
}

void AutoPilot::ControlTest(MovableObject *_ob, float krok_czasowy, float czas_proby)
{
	bool koniec = false;
	float _czas = 0;               // czas liczony od pocz¹tku testu
	//FILE *pl = fopen("test_sterowania.txt","w");
	while (!koniec)
	{
		_ob->Simulation(krok_czasowy);
		AutoControl(_ob);
		_czas += krok_czasowy;
		if (_czas >= czas_proby) koniec = true;
		//fprintf(pl,"czas %f, vPos[%f %f %f], got %d, pal %f, F %f, wheel_turn_angle %f, breaking_degree %f\n",_czas,_ob->vPos.x,_ob->vPos.y,_ob->vPos.z,_ob->money,_ob->amount_of_fuel,_ob->F,_ob->wheel_turn_angle,_ob->breaking_degree);
	}
	//fclose(pl);
}

// losowanie liczby z rozkladu normalnego o zadanej sredniej i wariancji
float Randn(float srednia, float wariancja, long liczba_iter)
{
	//long liczba_iter = 10;  // im wiecej iteracji tym rozklad lepiej przyblizony
	float suma = 0;
	for (long i = 0; i < liczba_iter; i++)
		suma += (float)rand() / RAND_MAX;
	return (suma - (float)liczba_iter / 2)*sqrt(12 * wariancja / liczba_iter) + srednia;
}

