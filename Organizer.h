#pragma once
#include <iostream>
#include "LinkedQueue.h"
#include "priQueue.h"
#include "Patient.h"
#include "Car.h"
#include "Hospital.h"
#include"RemovablePriQueue.h"
#include <fstream>
#include"UI.h"
#include<iomanip>
#include <cmath>
#include <random>
using namespace std;

// Cancellation struct to store patient's id and time of cancellation

struct Cancellation {
	int PID;       // Patient ID
	int HID; // Hospital ID where patient cancelled
	int cancelTime;      // Cancellation time

	// Constructor
	Cancellation(int id, int time, int HID)
		: PID(id), cancelTime(time), HID(HID) {
	}
};

class Organizer
{
private:

	// Lists

	LinkedQueue<Patient*> AllPatients;
	LinkedQueue<Cancellation*> AllCancellations;
	LinkedQueue<Patient*> FinishedPatients;
	priQueue<Car*> CheckupList;
	RemovablePriQueue<Car*> OutCars;
	priQueue<Car*> BackCars;

	// Other members

	int** DistanceMatrix;
	Hospital** Hospitals; // Pointer to a dynamic array of hospitals.
	int NumHospitals; //  Number of hospitals
	int NumOutCars, NumBackCars;
	int NumFinishedPatients;
	int NumCheckupCars;
	int TotalNumRequests;
	int NumCancellations;

	int TotalNumEP, TotalNumSP, TotalNumNP;

	int TotalNumSC, TotalNumNC;

	int AvgWaitTime;
	int AvgBusyTime;
	int Utilization;
	int FailProb;

	// UI member
	UI ui;

public:
	// Constructor
	Organizer() : NumOutCars(0), NumBackCars(0), NumFinishedPatients(0), TotalNumRequests(0), TotalNumEP(0), TotalNumSP(0), TotalNumNP(0), TotalNumNC(0), TotalNumSC(0) {}

	// Utility functions

	// Adds patient to the system
	void AddPatient(PatientType type, int rT, int PID, int HID, int HD, int severity = -1)
	{
		// Creating the patient
		Patient* p = new Patient(type, rT, PID, HID, HD, severity);
		// Enqueuing patient
		AllPatients.enqueue(p);

		// Updating total number of patients in the simulation

		if (type == PatientType::Ep)
			TotalNumEP++;
		else if (type == PatientType::NP)
			TotalNumNP++;
		else if (type == PatientType::SP)
			TotalNumSP++;
	}
	// returns hospital pointer given its ID
	Hospital* getHospital(int HID)
	{
		// Hospital is stored at HID - 1
		return Hospitals[HID - 1];
	}

	// Adds cancellation to the system
	void AddCancellation(int PID, int CancellationTime, int HID)
	{
		Cancellation* c = new Cancellation(PID, CancellationTime, HID);
		AllCancellations.enqueue(c);
	}

	// Adds patient to finished lsit
	void AddPatientFinishedList(Patient* p)
	{
		FinishedPatients.enqueue(p);
		NumFinishedPatients++;
	}

	// Function to create the hospitals given the num of hospitals.
	void CreateHospitals(int num)
	{
		Hospitals = new Hospital * [num];
		for (int i = 1; i <= num; i++)
			Hospitals[i - 1] = new Hospital(i);  //set id to i
	}

	// Loading the file
	void LoadFile(string fileName)
	{
		float ScarSpeed, NCarSpeed;
		int CarFailProb;
		int SCtime, NCtime;
		int ScarNum, NCarNum, ReqNum, ReqTime,
			PID, HID, PatientDistance, CancellationReqNum, CancelTime;
		string PT;
		PatientType PTenum;

		ifstream file(fileName);

		if (!file.is_open()) {
			cout << "Error opening file: " << fileName << endl;
			return;
		}

		file >> NumHospitals >> ScarSpeed >> NCarSpeed;
		// Setting The constant speed for all car types
		Car::setScarSpeed(ScarSpeed);
		Car::setNcarSpeed(NCarSpeed);

		CreateHospitals(NumHospitals);

		DistanceMatrix = new int* [NumHospitals];
		for (int i = 0; i < NumHospitals; i++)
			DistanceMatrix[i] = new int[NumHospitals];

		for (int i = 0; i < NumHospitals; i++)
		{
			for (int j = 0; j < NumHospitals; j++)
				file >> DistanceMatrix[i][j];
		}
		file >> SCtime >> NCtime;

		for (int i = 0; i < NumHospitals; i++)
		{
			file >> ScarNum >> NCarNum;
			TotalNumSC += ScarNum;
			TotalNumNC += NCarNum;
			for (int j = 0; j < ScarNum; j++)
			{
				Car* C = new Car(j + 1, CarType::SC, ScarSpeed, i + 1, SCtime);
				Hospitals[i]->AddSCar(C);
			}
			for (int j = 0; j < NCarNum; j++)
			{
				Car* C = new Car(j + 1 + ScarNum, CarType::NC, NCarSpeed, i + 1, NCtime);
				Hospitals[i]->AddNCar(C);
			}
			Hospitals[i]->setTotalNumNcars(NCarNum);
			Hospitals[i]->setTotalNumScars(ScarNum);
		}

		//set min and max outcars failure probability
		file >> CarFailProb;
		FailProb = CarFailProb;

		file >> ReqNum;
		TotalNumRequests = ReqNum;
		for (int i = 0; i < ReqNum; i++)
		{
			file >> PT >> ReqTime >> PID >> HID >> PatientDistance;

			if (PT == "NP")
				PTenum = PatientType::NP;
			else if (PT == "SP")
				PTenum = PatientType::SP;
			else if (PT == "EP")
				PTenum = PatientType::Ep;
			else
				cout << "invalid patient type";

			if (PTenum == PatientType::Ep)
			{
				int Severity;
				file >> Severity;
				AddPatient(PTenum, ReqTime, PID, HID, PatientDistance, Severity);
			}
			else
				AddPatient(PTenum, ReqTime, PID, HID, PatientDistance);
		}

		file >> CancellationReqNum;
		NumCancellations = CancellationReqNum;
		for (int i = 0; i < CancellationReqNum; i++)
		{
			file >> CancelTime >> PID >> HID;

			AddCancellation(PID, CancelTime, HID);
		}

		file.close();
	}

	// Setters
	void SetNumHospitals(int n) { NumHospitals = n; }
	void SetTotalNumRequests(int n) { TotalNumRequests = n; }
	void SetNumCancellations(int n) { NumCancellations = n; }

	// Getters
	int getOUTfailprob() { return FailProb; }
	int GetTotalNumReq() { return TotalNumRequests; }
	int GetTotalNumCanellation() { return NumCancellations; }
	int GetTotalNumFinished() { return NumFinishedPatients; }
	int GetNumHospitals() { return NumHospitals; }
	int getNumCheckupCars() {
		return NumCheckupCars;
	}

	int GetAvgWaitTime() {
		Patient* p;
		int totalwaitingtime = 0;
		while (!FinishedPatients.isEmpty()) {
			FinishedPatients.dequeue(p);
			totalwaitingtime += p->getWaitingTime();
		}
		AvgWaitTime = totalwaitingtime / NumFinishedPatients;
		return AvgWaitTime;
	}
	int GetAvgBusyTime() {
		Car* c;
		int pri;
		int totalbusytime = 0;
		while (!OutCars.isEmpty()) {
			OutCars.dequeue(c, pri);
			totalbusytime += c->getBusyTime();
		}
		AvgBusyTime = totalbusytime / NumFinishedPatients;
		return AvgBusyTime;
	}
	/*int GetAvgUtilization() {
		Patient* p;
		while (!FinishedPatients.isEmpty()) {
			FinishedPatients.dequeue(p);
			timestep += getTimeStep();
		}
		Utilization = AvgBusyTime /timestep ;
	}*/

	RemovablePriQueue<Car*>& GetOutCars() { return OutCars; }
	priQueue<Car*>& GetBackCars() { return BackCars; }
	LinkedQueue<Patient*>& GetFinished() { return FinishedPatients; }
	priQueue<Car*>& GetCheckupList() { return CheckupList; }

	//Destructor
	~Organizer()
	{
		for (int i = 0; i < NumHospitals; i++) {
			// Delete dynamically allocated memory in DistanceMatrix
			delete[] DistanceMatrix[i];
		}
		delete[] DistanceMatrix;

		Patient* p;
		while (!AllPatients.isEmpty()) {
			AllPatients.dequeue(p);  // Remove patient from queue
			delete p;  // Free memory
		}

		Cancellation* c;
		while (!AllCancellations.isEmpty()) {
			AllCancellations.dequeue(c);  // Remove cancellation from queue
			delete c;  // Free memory
		}
	}

	// Function that takes time step and checks if there is a request.
	// If there is a request, enqueue the patient to its hospital.
	// Returns true if the time step matches the request time

	bool AllocatePatient(int time, Patient*& patient)
	{
		// If AllPatients list is empty return false.
		if (AllPatients.peek(patient) == false)
		{
			return false;
		}
		else
		{
			// If the "top" patient's request time is the same as this time step we move handle it.
			if (patient->getRequestTime() == time)
			{
				AllPatients.dequeue(patient);
				return true;
			}
			// Else all patient requests for this time step have been allocated
			return false;
		}
	}

	void MoveOutToBack(int Timestep)
	{
		Car* tomove = nullptr;
		Patient* P = nullptr;
		int  pri;

		// We will loop until all cars of the current time steps move to back

		while (OutCars.peek(tomove, pri)) // pri = (-1) * picktime
		{
			// Checking if car's arrival time is this time step
			if (pri * -1 != Timestep)
				break; //car shouldnt be returned to backlist yet

			P = tomove->getAssignedPatient();
			if (!P)
				break;

			Hospital* H = Hospitals[P->getHID() - 1];

			// If Patient is a normal patient remove it from NPWaitList once pickedup
			if (P->getType() == PatientType::NP)
				H->RemoveNpWait(P, P->getPID());
			// Setting the time patient was picked
			P->setPickupTime(pri * -1);

			// Setting the wait time of patient
			P->setWaitingTime(P->getPickupTime() - P->getAssignmentTime());

			// Setting time patient will arrive to hospital 
			P->setFinishTime(P->getPickupTime() + ceil(P->getHospitalDistance() / tomove->getSpeed()));

			// += busy time to add time car traveled to patient 
			tomove->updateBusyTime(P->getPickupTime() - P->getAssignmentTime());

			// Take that out car and move it to back
			OutCars.dequeue(tomove, pri);
			int newPri = -1 * P->getFinishTime();

			tomove->setStatus(CarStatus::Loaded);
			BackCars.enqueue(tomove, newPri); //least finish time should be first in queue
			NumBackCars++;
			NumOutCars--;
		}
	}



	void MoveBackToFree(int Timestep) {

		Car* tomove = nullptr;
		Patient* P = nullptr;
		int pri;

		// loop on all back cars with same timestep
		while (BackCars.peek(tomove, pri))
		{
			// Not time then break
			if ((pri * -1) != Timestep)
				break;

			// if car dequeued null then break
			if (!BackCars.dequeue(tomove, pri))
				break;

			NumBackCars--;
			// If the car has not failed
			if (!tomove->getFailedCar())
			{
				P = tomove->getAssignedPatient();

				// Patient can cancel his request -> no need to handle the patient 
				if (P)
				{
					tomove->updateBusyTime(P->getFinishTime() - P->getPickupTime());
					P->setCarId(-1); // -1: is not assigned a car
					AddPatientFinishedList(P);
					tomove->setAssignedPatient(nullptr);
					tomove->setStatus(CarStatus::Ready);
				}

				int hid = tomove->getHID();
				Hospital* H = Hospitals[hid - 1];
				CarType Ctype = tomove->getType();
				if (Ctype == CarType::NC)
				{
					H->AddNCar(tomove);
				}
				else if (Ctype == CarType::SC)
				{
					H->AddSCar(tomove);
				}
				H = nullptr;
				tomove = nullptr;
				P = nullptr;
			}
			else
			{
				// Car needs to be added to checkup list
				CheckupList.enqueue(tomove, -(Timestep + tomove->getCheckupTime()));
				tomove->setStatus(CarStatus::Checkup);
				NumCheckupCars++;
			}
		}
	}
	// Function that moves all the assigned cars from free to out (to go to their patients0
	// Does so for all hospitals
	void MoveFreeToOut(int Timestep)
	{
		Patient* AssignedP = nullptr;
		Car* AssignedC = nullptr;
		int pri;

		// For all hospitals
		for (int i = 0; i < NumHospitals; i++)
		{
			// Get the hospital
			Hospital* H = Hospitals[i];

			// For all its assigned cars
			while (H->getAssignedCar(AssignedC))
			{
				AssignedP = AssignedC->getAssignedPatient();

				if (AssignedP == nullptr)
					continue; //just in case there was an error when assigning patients

				// We set the time patient was assigned
				AssignedP->setAssignmentTime(Timestep);

				// We assign the car id to the patient
				AssignedP->setCarId(AssignedC->getCarID());

				// Patient's actual pickup time will be set to patient when they are picked
				// Just in case they cancel
				int PickupTime = AssignedP->getAssignmentTime() + ceil(AssignedP->getHospitalDistance() / AssignedC->getSpeed());
				pri = -1 * PickupTime;  // prioritize based on pick time lower pick time = higher priority

				// enqueue car to OutCars
				OutCars.enqueue(AssignedC, pri);
				NumOutCars++;
			}
		}
	}
	void CancelRequest(int Timestep)
	{
		Cancellation* CancelReq = nullptr;
		Car* Car = nullptr;
		Patient* P = nullptr;
		int pri;

		// We do this for all cancellations of the current time step
		while (AllCancellations.peek(CancelReq))
		{
			// Check if cancellation belongs to this timestep
			if (CancelReq == nullptr || CancelReq->cancelTime != Timestep)
				break;

			AllCancellations.dequeue(CancelReq);

			// Get hospital id
			int hid = CancelReq->HID;
			if (hid < 1 || hid > NumHospitals) { //check validity of hid
				delete CancelReq; //this request shouldn't happen
				continue;
			}
			Hospital* H = Hospitals[hid - 1];

			// We will remove that patient
			H->GetNPlist()->removeItem(P, CancelReq->PID);
			// Patient found and removed from np list
			if (P)
			{
				continue;
			}

			// If patient was not in the list -> then a car is moving to them or patient has arrived to hospital
			int CarID;
			if (!P)
			{

				H->GetNpWaitList()->removeItem(P, CancelReq->PID);
				// if patient didnt exist in waitlist remove the cancel request
				if (!P)
				{
					delete CancelReq;
					break;
				}

				CarID = P->getCarId();

				// If patient arrived then no car assigned
				if (CarID == -1) { // no car assigned
					continue; // No car to handle
				}

				// If patient is on their way to the hospital
				if (Timestep > P->getAssignmentTime() + ceil(P->getHospitalDistance() / Car::getNcarSpeed()))
					continue; //patient cannot cancel a request while in a car **waiting P only**

				// Getting car that was assigned this patient
				if (OutCars.removeItem(Car, pri, CarID))
				{
					Car->setAssignedPatient(nullptr);
					Car->updateBusyTime((Timestep - P->getAssignmentTime()) * 2);
					P->setCarId(-1); //car removed from patient

					pri = -1 * (Timestep + Timestep - P->getAssignmentTime());
					BackCars.enqueue(Car, pri);
					NumOutCars--;
					NumBackCars++;
				}

			}
		}
		CancelReq = nullptr;
		P = nullptr;
	}
	// A car has failed function
	void FailAction(int timestep) {
		Car* tofail;
		int pri;
		// Dequeue a car to fail
		if (OutCars.dequeue(tofail, pri))
		{
			NumOutCars--;
			// The time of arrival to hospital
			int time_arrival = 2 * timestep - tofail->getAssignedPatient()->getAssignmentTime();
			// Move to back
			BackCars.enqueue(tofail, -time_arrival);
			tofail->setCarFailed(true);
			NumBackCars++;

			// Patient should be at the top of its list.
			Patient* movedpatient = tofail->getAssignedPatient();
			// Car is back to the hospital so it is no longer assigned to a patient
			tofail->setAssignedPatient(NULL);
			Hospital* currentH = Hospitals[movedpatient->getHID() - 1];
			PatientType type = movedpatient->getType();
			if (type == PatientType::Ep) {
				int severity = movedpatient->getSeverity();
				currentH->AddToFrontEP(movedpatient, severity);
			}
			else if (type == PatientType::NP) {
				currentH->AddToFrontNP(movedpatient);
			}
			else if (type == PatientType::SP) {
				currentH->AddToFrontNP(movedpatient);
			}
		}
	}

	// Should loop on checkup list
	void MoveCheckupToFree(int Timestep)
	{
		Car* tomove = nullptr;
		int pri;

		// loop on all checkup cars with same timestep
		while (CheckupList.peek(tomove, pri))
		{
			// Not time then break
			if ((pri * -1) != Timestep)
				break;

			// if car dequeued null then break
			if (!CheckupList.dequeue(tomove, pri))
				break;

			// Car has been checked so it is done
			NumCheckupCars--;
			tomove->setCarFailed(false);

			// Car is now ready
			tomove->setStatus(CarStatus::Ready);
			int hid = tomove->getHID();

			Hospital* H = Hospitals[hid - 1];

			CarType Ctype = tomove->getType();

			if (Ctype == CarType::NC)
			{
				H->AddNCar(tomove);
			}
			else if (Ctype == CarType::SC)
			{
				H->AddSCar(tomove);
			}

			H = nullptr;
			tomove = nullptr;
		}
	}

	void InteractiveMode(int timestep) {
		ui.PrintOutput(timestep, Hospitals, &OutCars, &BackCars, &FinishedPatients, &CheckupList, NumFinishedPatients, NumOutCars, NumBackCars, NumHospitals, NumCheckupCars);
	}
	void SilentMode() {
		cout << "Silent mode, simulation starts..." << endl;
		cout << "Simulation ends, output file created" << endl;
	}

	void OutputFile() {
		ofstream Output("Output File.txt"); //open output file
		Patient* p;

		if (Output.is_open()) {
			//print headers
			Output << std::setw(10) << "FT";
			Output << std::setw(10) << "PID";
			Output << setw(10) << "QT";
			Output << setw(10) << "WT" << endl;
			for (int i = 0; i < TotalNumRequests && !FinishedPatients.isEmpty(); i++) { // loop on finished patients and print the data
				FinishedPatients.dequeue(p);
				Output << std::setw(10) << p->getFinishTime();
				Output << std::setw(10) << p->getPID();
				Output << std::setw(10) << p->getRequestTime();
				Output << std::setw(10) << p->getWaitingTime() << endl;
			}

			Output << "Patients: " << TotalNumRequests;
			Output << std::setw(6) << "[NP: " << TotalNumNP << ", " << "SP: " << TotalNumSP << ", " << "EP: " << TotalNumEP << "]" << endl;
			Output << "Hospitals = " << NumHospitals << endl;
			Output << "Cars: " << TotalNumNC + TotalNumSC;
			Output << std::setw(6) << " [SCar: " << TotalNumSC << ", " << "NCar: " << TotalNumNC << "]" << endl;
			Output << "Avg Wait Time = " << GetAvgWaitTime() << endl;
			Output << "Avg Busy Time = " << GetAvgBusyTime() << endl;
			Output << "Avg Utilization = " << Utilization << endl;
		}
		else {
			cout << "Error opening file!" << endl;
		}
	}

	// A function that calls all hospitals to handle all requests at the current timestep
	void HandleHospitalPatients()
	{
		for (int i = 0; i < NumHospitals; i++)
		{
			Hospitals[i]->HandlePatients();
		}
	}

	// A function that lets each hospital reassign their unhandled EPs to another hospital (b4 bonus)
	// It says "pick any other hospital" to reassign ep to, so only basic check is done (not reassigning to the same hospital)
	void ReassignEPs()
	{
		// For each hospital
		for (int i = 0; i < NumHospitals; i++)
		{
			// Check if the hospital  has unassigned EP
			while (Hospitals[i]->getHasUnassignedEP())
			{
				// we get the number of the new hospital
				int j = randomExcluding(1, NumHospitals, i);

				// move the ep
				Patient* p; int severity;
				Hospitals[i]->RemoveEP(p, severity);
				// if patient is not a null pointer
				if (p)
				{
					Hospitals[j - 1]->AddEP(p, severity);
				}

				// Update the "has unassigned ep" bool of the hospital after moving
				Hospitals[i]->setHasUnassignedEP(!(Hospitals[i]->getEPlistEmpty()));
			}
		}
	}

	// Utility function that returns a random num. min, max, excluding.
	int randomExcluding(int min, int max, int exclude) {
		// the seed
		random_device rd;
		mt19937 gen(rd()); // Mersenne Twister random number generator
		uniform_int_distribution<> dist(min, max);

		int result;
		do {
			result = dist(gen);
		} while (result == exclude); // Re-roll if the number is the excluded one

		return result;
	}

	// Simulating the Ambulance Management System given a certain input file.
	void Simulate(string sample_input)
	{

		LoadFile(sample_input);
		int mode = ui.GetInput();

		int timestep = 0;

		do
		{
			timestep++;
			// Patient to be moved from the AllPa
			// tients to its hospital
			Patient* CurrentPatient;
			// Loop stops when all requests at this time step have been allocated to their hospital
			while (AllocatePatient(timestep, CurrentPatient))
			{
				// Hospital the patient's request belongs to
				Hospital* PatientsHospital = getHospital(CurrentPatient->getHID());
				PatientType patientType = CurrentPatient->getType();
				if (patientType == PatientType::Ep) {
					PatientsHospital->AddEP(CurrentPatient, CurrentPatient->getSeverity());
				}
				else if (patientType == PatientType::NP) {
					PatientsHospital->AddNP(CurrentPatient);
					PatientsHospital->AddNPWaitlist(CurrentPatient);
				}
				else if (patientType == PatientType::SP) {
					PatientsHospital->AddSP(CurrentPatient);
				}

			}
			// After loop finishes, all relevant patients have been added to their hospitals.
			// CurrentPatient is now useless
			CurrentPatient = NULL;

			// All hospitals should now check their requests and handle as much as possible
			HandleHospitalPatients();

			// The cars should start moving from free to back. (All hospitals , All cars)
			MoveCheckupToFree(timestep);
			MoveBackToFree(timestep);
			MoveOutToBack(timestep);
			MoveFreeToOut(timestep);
			CancelRequest(timestep);


			ReassignEPs();


			// Reassigned patients will be handled in the next time step. To imitate real life.

			// uncomment later
			// 
			//int random_fail = randomExcluding(1, 100, -1);
			//
			//int OUTfailprob = getOUTfailprob();
			//if (random_fail > 0 && random_fail <= OUTfailprob) {
			//	FailAction();
			//}


			if (mode == 1) { // if it's in interactive mode print all necessary data from the organizer class
				//cout << "Random: " << random_fail << endl;
				InteractiveMode(timestep);
			}

		} while (GetTotalNumFinished() != (GetTotalNumReq() - GetTotalNumCanellation()) || (!OutCars.isEmpty() || !BackCars.isEmpty()));

		if (mode == 2) {
			SilentMode();
		}
		OutputFile();
	}

	
};
