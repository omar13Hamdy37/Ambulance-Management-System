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

	void PrintOutput(int timestep, Hospital** Hospitals, RemovablePriQueue<Car*>* OutCars, priQueue<Car*>* BackCars, LinkedQueue<Patient*>* FinishedPatients, LinkedQueue<Car*>* Checkuplist, int totalNumFinishedP, int totalNumOutC, int totalNumBackC, int numHospitals, int NumCheckupCars)
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
			cout << "------------------------------------------------------" << endl;
			cout << NumCheckupCars << "==> Checked Cars:";
			Checkuplist->printPointers();
			cout << "Press any key to display next hospital." << endl;



			cin.get();
		}


	}

	int GetInput() {
		int mode;
		cout << "Select program mode" << endl;
		cout << "1- Interactive mode " << endl;
		cout << "2- Silent mode" << endl;
		cin >> mode;

		while (mode != 1 && mode != 2) { // user should put 1 or 2 only
			cout << "Enter a valid value! " << endl;
			cout << "Your input should be 1 or 2" << endl;
			cout << "Select program mode" << endl;
			cout << "1- Interactive mode " << endl;
			cout << "2- Silent mode" << endl;
			cin >> mode;
		}

		return mode;
	}

};
