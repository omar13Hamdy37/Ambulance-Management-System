#pragma once
#include "Organizer.h"
#include"LinkedQueue.h"
#include"priQueue.h"
#include "Hospital.h"
#include"Car.h"
#include"Patient.h"



class UI {
public:

	void PrintOutput(LinkedQueue<Patient*>& np, priQueue<Patient*>& ep,LinkedQueue<Patient*>& sp,RemovablePriQueue<Car*>&outc,priQueue<Car*>&backc, LinkedQueue<Patient*>&finished, LinkedQueue<Car*>& scar, LinkedQueue<Car*>&ncar) {

		cout << ep.counter() << " EP requests: " << ep.print() << endl;
		cout << sp.counter() << " SP requests: " << sp.print() << endl;
		cout << np.counter() << " NP requests: " << np.print() << endl;
		cout << "Free Cars: " << scar.counter() << " Scars, " << ncar.counter() << " Ncars" << endl;
		cout << "---------------------------------------" << endl;
		cout << outc.counter() << "==>  Out Cars: " << outc.print() << endl;
		cout << backc.counter() << "==>  Back Cars; " << backc.print() << endl;
		cout << "---------------------------------------" << endl;
		cout << finished.counter() << " Finished Patients: " << finished.print() << endl;


	}

};
