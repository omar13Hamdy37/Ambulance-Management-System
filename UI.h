#pragma once
#include "Organizer.h"
#include"LinkedQueue.h"
#include"priQueue.h"
#include "Hospital.h"
#include"Car.h"
#include"Patient.h"
#include "iostream"
using namespace std;

class UI {
public:

	void PrintOutput(int timestep, Hospital** Hospitals, RemovablePriQueue<Car*>* OutCars, priQueue<Car*>* BackCars, LinkedQueue<Patient*>* FinishedPatients, int totalNumFinishedP, int totalNumOutC, int totalNumBackC, int numHospitals)
	{
		cout << "Current Timestep: " << timestep << endl;

		for (int i = 0; i < numHospitals; i++) {
			Hospital* currentHospital = Hospitals[i];
			cout << "=========== HOSPITAL #" << i + 1 << " data ========= " << endl;
			cout << currentHospital->getNumEPRequests() << " EP requests: ";
			currentHospital->GetEPlist()->printPointers();

			cout << currentHospital->getNumSPRequests() << " SP requests: ";
			currentHospital->GetSPlist()->printPointers();

			cout << currentHospital->getNumNPRequests() << " NP requests: ";
			currentHospital->GetNPlist()->printPointers();
			cout << "Free Cars: " << currentHospital->getAvailableNumScars() << " SCars, " << currentHospital->getAvailableNumNCars() << " NCars\n";

			cout << "=========== HOSPITAL #" << i + 1 << " data end ===========" << endl;
			cout << "------------------------------------------------------" << endl;
			cout << totalNumOutC << "==> Out cars: ";
			OutCars->printPointers();

			cout << totalNumBackC << "==> Back cars: ";
			BackCars->printPointers();

			cout << "------------------------------------------------------" << endl;

			cout << totalNumFinishedP << " finished patients: ";
			FinishedPatients->printPointers();
			cout << "Press any key to display next hospital." << endl;

			cin.get();
		}
	}
};
