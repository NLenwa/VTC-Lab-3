#include <stdlib.h>
#include <time.h>

#include "agents.h"





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

	// Finding closest item for pick up
	float my_pos_x = obj->state.vPos.x;
	float my_pos_y = obj->state.vPos.y;
	float temp_x = _terrain->p[0].vPos.x;
	float temp_y = _terrain->p[0].vPos.y;
	float temp2_x;
	float temp2_y;
	float dist2;

	distance_item = sqrtf(pow((my_pos_x - temp_x), 2) + pow((my_pos_y - temp_y), 2));

	for (int i = 1; i <= _terrain->number_of_items; i++)
	{
		temp2_x = _terrain->p[i].vPos.x;
		temp2_y = _terrain->p[i].vPos.y;
		dist2 = distance_item = sqrtf(pow((my_pos_x - temp2_x), 2) + pow((my_pos_y - temp2_y), 2));

		// If current item is closer
		if (dist2 < distance_item)
		{
			// If item is of type FUEL or MONEY (worth 100) and can be picked-up
			if (_terrain->p[i].to_take == 1 && 
				( _terrain->p[i].type == MONEY || _terrain->p[i].type == FUEL ))
			{
				temp_x = temp2_x;
				temp_y = temp2_y;
				distance_item = dist2;
				tracked_item = i;
			}
		}
	}

	// Driving to closest item
	Vector3 vector_item = _terrain->p[tracked_item].vPos;
	float scalar_prod_forward = vect_local_forward ^ vector_item;
	float scalar_prod_right = vect_local_right ^ vector_item;

	// scalar_prod_forward > 0 then item is in front, <0 in back
	// Driving forward or reverse
	if (scalar_prod_forward > 0)
	{
		obj->F = obj->F_max;
	}
	else 
	{
		obj->F = - (obj->F_max)/2;
	}
	// Steering left - right
	float vector_angle = angle_between_vectors2D(vect_local_forward, vector_item);
	
	// Steering to right, when item is more than 45 degrees to the right
	if ((vector_angle > 0 && vector_angle < (pi / 4)) || (vector_angle > (7 * pi / 4) && vector_angle < (2 * pi)))
	{
		obj->state.wheel_turn_angle = -45 / 180;
	}
	// Sterring to left, when item is more than 45 degrees to the left
	else if ((vector_angle > (3 * pi / 4) && vector_angle < (5 * pi / 4)))
	{
		obj->state.wheel_turn_angle = 45 / 180;
	}
	// When item is behind <-45; 45> degrees
	else if (vector_angle >= (5 * pi / 4) && vector_angle <= (7 * pi / 4))
	{
		obj->state.wheel_turn_angle = pi - vector_angle;
	}
	// When item is in front <-45; 45> degrees
	else if (vector_angle >= (pi / 4) && vector_angle <= (3 * pi / 4))
	{
		obj->state.wheel_turn_angle = vector_angle;
	}



	// BUY / SELL control code
	// when fuel if low, then ask to trade
	my_fuel = obj->state.amount_of_fuel;
	if (my_fuel < 10)
	{
		AskForFuel();
	}
	// when trade was initiated and have enough fuel for trade, then set selling price
	if (is_trade && obj->iID != off_id)
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
		if (obj->state.money >= response_price)
		if (obj->state.money >= response_price)
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

