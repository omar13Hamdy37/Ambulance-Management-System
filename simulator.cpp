#include <iostream>
#include"Car.h"
#include"Hospital.h"
#include"Patient.h"
#include"Organizer.h"
#include<cstdlib>
#include"UI.h"
using namespace std;

int main() {

	
	// Call file loading function

	Organizer Org;
	Org.LoadFile("sample_input.txt");

	int timestep = 0;
	srand(static_cast<unsigned>(time(0)));
	// Program will end when all patients have been moved to the finish list
	// Cancel requests are not handled in phase 1.2
	srand(static_cast<unsigned>(time(0)));  // to generate a new number in each run
	while (Org.GetTotalNumFinished() != Org.GetTotalNumReq()) {
		timestep++;
		// Patient to be moved from the AllPatients to its hospital
		Patient* CurrentPatient;
		// Loop stops when all requests at this time step have been allocated to their hospital
		while (Org.AllocatePatient(timestep, CurrentPatient))
		{
			// Hospital the patient's request belongs to
			Hospital* PatientsHospital = Org.getHospital(CurrentPatient->getHID());
			PatientType patientType = CurrentPatient->getType();
			if (patientType == PatientType::Ep) {
				PatientsHospital->AddEP(CurrentPatient, CurrentPatient->getSeverity());
			}
			else if (patientType == PatientType::NP) {
				PatientsHospital->AddNP(CurrentPatient);

			}
			else if (patientType == PatientType::SP) {
				PatientsHospital->AddSP(CurrentPatient);
			}

		}
		// After loop finishes, all relevant patients have been added to their hospitals.
		// CurrentPatient is now useless
		CurrentPatient = NULL;

		// All hospitals should now check their requests and handle as much as possible
		Org.HandleHospitalPatients();









		// Old simulator function
		//int min = 1;
		//int max = 100;

		//// Generate a random number in the range
		//int random = min + (std::rand() % (max - min + 1));


		// // to generate a new number in each run
		////int random = rand() % 100; // generate a random number from 1 to 100
		//cout << "Random: " << random << endl;
		/*if (random >= 10 && random < 20) {
			Org.HandleHospital(PatientType::SP);

		}
		else if (random >= 20 && random < 25) {
			Org.HandleHospital(PatientType::Ep);

		}
		else if (random >= 30 && random < 40) {
			Org.HandleHospital(PatientType::NP);

		}
		else if (random >= 40 && random < 45) {
			Org.HandleCars(CarType::SC);
		}
		else if (random >= 70 && random < 75) {
			Org.HandleCars(CarType::NC);
		}
		else if (random >= 80 && random < 90) {
			Org.MoveOutToBack();
		}
		else if (random >= 91 && random < 95) {
			Org.MoveBackToFree();
		}*/


		Org.CallUI(timestep);
	}


	// Produce output file
	


}


