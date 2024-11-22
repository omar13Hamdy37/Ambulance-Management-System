#include <iostream>
using namespace std;
#include "RemovableQueue.h"
#include "Patient.h"
#include "Car.h"
#include "RemovablePriQueue.h"
#include "Organizer.h"

int main(void)
{
	//type, int rT, int PID, int HID, int HD, int severity = -1)
	RemovablePriQueue<Car*> cars;

	Car car1 = Car(1, CarType::NC, 1, 1);
	Car car2 = Car(2, CarType::NC, 1, 1);
	Car car3 = Car(3, CarType::NC, 1, 1);
	Car car4 = Car(4, CarType::NC, 1, 1);
	Car car5 = Car(5, CarType::NC, 1, 1);
	Car car6 = Car(6, CarType::NC, 1, 1);

	cars.enqueue(&car1, 1);
	cars.enqueue(&car2, 2);
	cars.enqueue(&car3, 3);
	cars.enqueue(&car4, 4);
	cars.enqueue(&car5, 5);
	cars.enqueue(&car6, 6);

	Car* smt;
	int priority;

	cars.removeItem(smt, priority, 1);
	cars.removeItem(smt, priority, 6);
	cars.removeItem(smt, priority, 1);
	cars.removeItem(smt, priority, 2);
	cars.removeItem(smt, priority, 4);


	Organizer Or;
	Or.LoadFile("sample_input.txt");
	cout << Or.GetTotalNumReq() << " " << Or.GetTotalNumCanellation();

}