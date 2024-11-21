#include <iostream>
#include"Car.h"
#include"Hospital.h"
#include"Patient.h"
#include"Organizer.h"
#include<cstdlib>
using namespace std;

int main() {
	//initializations
	// call file loading function
	LinkedQueue<Patient*> patients;
	RemovablePriQueue<Car*> outcars; //out cars
	priQueue<Car*> backcars; //  back cars
	Patient pat1 = Patient(PatientType::SP, 1, 3, 4, -1);
	Patient pat2 = Patient(PatientType::NP, 2, 5, 7, -1);
	Patient pat3 = Patient(PatientType::Ep, 5, 8, 9, -1);
	Car car1 = Car(1, CarType::SC, 1, 1);
	Car car2 = Car(2, CarType::NC, 1, 1);

	
	int timestep = 1;
	srand(static_cast<unsigned>(time(0)));  // to generate a new number in each run
	int random = rand() % 100; // generate a random number from 1 to 100
	cout << random << endl;

	if (random >= 10 && random < 20) {
		patients.enqueue(&pat1);
	}
	else if (random >= 20 && random < 25) {
		patients.enqueue(&pat3);
	}
	else if (random >= 30 && random < 40) {
		patients.enqueue(&pat3);
	}
	else if (random >= 40 && random < 45) {
		outcars.enqueue(&car1,1);
	}
	else if (random >= 70 && random < 75) {
		outcars.enqueue(&car2, 1);
	}
	else if (random >= 80 && random < 90) {
		int x = 1;
		Car* itm;
		outcars.dequeue(itm, x);
		backcars.enqueue(&car1,1);
	}
	else if (random >= 91 && random < 95) {
		int x = 1;
		Car* itm;
		backcars.dequeue(itm, x);
	}


	int x = 1;
	Car* itm;
	while (outcars.dequeue(itm, x)) { //Test for random=41
		cout << car1.getCarID() << endl;
		cout << car1.getHID();
	}

}


