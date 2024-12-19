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
	LinkedQueue<Car*>CheckupList;
	RemovablePriQueue<Car*> OutCars; // TODO: Should be removable priority queue
	priQueue<Car*> BackCars;

	int NumHospitals; //  Number of hospitals

	// Other members

	Hospital** Hospitals; // Pointer to a dynamic array of hospitals.
	int** DistanceMatrix;
	int NumOutCars, NumBackCars;
	int NumFinishedPatients;
	int NumCheckupCars;
	int TotalNumRequests;
	int NumCancellations;

	int TotalNumEP, TotalNumSP, TotalNumNP;

	int TotalNumSC, TotalNumNC;
	int timestep;

	// Phase 2 data members
	int AvgWaitTime;
	int AvgBusyTime;
	int Utilization;
	int failprob;

public:
	// Constructor
	Organizer() : NumOutCars(0), NumBackCars(0), NumFinishedPatients(0), TotalNumRequests(0), TotalNumEP(0), TotalNumSP(0), TotalNumNP(0), TotalNumNC(0), TotalNumSC(0) {}

	// Utility functions
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

	void AddCancellation(int PID, int CancellationTime, int HID)
	{
		Cancellation* c = new Cancellation(PID, CancellationTime, HID);
		AllCancellations.enqueue(c);
	}

	void AddPatientFinishedList(Patient* p)
	{
		FinishedPatients.enqueue(p);
		NumFinishedPatients++;
	}

	int getTimeStep()const { return timestep; }
	int setTimeStep(int t) { timestep = t; }

	// Omar: Function to create the hospitals given the num of hospials.
	void CreateHospitals(int num)
	{
		Hospitals = new Hospital * [num];
		for (int i = 1; i <= num; i++)
			Hospitals[i - 1] = new Hospital(i);  //set id to i
	}

	void LoadFile(string fileName)
	{
		float ScarSpeed, NcarSpeed;
		int carfailprob;
		int ScarNum, NcarNum, ReqNum, ReqTime,
			PID, HID, PatientDistance, CancellationReqNum, CancelTime;
		string PT;
		PatientType PTenum;

		ifstream file(fileName);

		if (!file.is_open()) {
			cout << "Error opening file: " << fileName << endl;
			return;
		}

		file >> NumHospitals >> ScarSpeed >> NcarSpeed;

		CreateHospitals(NumHospitals);

		DistanceMatrix = new int* [NumHospitals];
		for (int i = 0; i < NumHospitals; i++)
			DistanceMatrix[i] = new int[NumHospitals];

		for (int i = 0; i < NumHospitals; i++)
		{
			for (int j = 0; j < NumHospitals; j++)
				file >> DistanceMatrix[i][j];
		}

		for (int i = 0; i < NumHospitals; i++)
		{
			file >> ScarNum >> NcarNum;
			TotalNumSC += ScarNum;
			TotalNumNC += NcarNum;
			for (int j = 0; j < ScarNum; j++)
			{
				Car* C = new Car(j + 1, CarType::SC, ScarSpeed, i + 1);
				Hospitals[i]->AddSCar(C);

			}
			for (int j = 0; j < NcarNum; j++)
			{
				Car* C = new Car(j + 1 + ScarNum, CarType::NC, NcarSpeed, i + 1);
				Hospitals[i]->AddNCar(C);
			}
			Hospitals[i]->setTotalNumNcars(NcarNum);
			Hospitals[i]->setTotalNumScars(ScarNum);

		}
		//set min and max outcars failure probability
		file >> carfailprob;
		failprob = carfailprob;

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
	//setters and getters for checkuplist
	void AddcarCheckupList(Car* c) {
		CheckupList.enqueue(c);
		NumCheckupCars++;
	}
	int getNumCheckupCars() {
		return NumCheckupCars;
	}

	// Setters
	void SetNumHospitals(int n) { NumHospitals = n; }
	void SetTotalNumRequests(int n) { TotalNumRequests = n; }
	void SetNumCancellations(int n) { NumCancellations = n; }

	// Getters
	int getOUTfailprob() { return failprob; }
	int GetTotalNumReq() { return TotalNumRequests; }
	int GetTotalNumCanellation() { return NumCancellations; }
	int GetTotalNumFinished() { return NumFinishedPatients; }
	int GetNumHospitals() { return NumHospitals; }
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
	LinkedQueue<Car*>& GetCheckupList() { return CheckupList; }

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
			// If the "top" patient's requeset time is the ssame as this time step we move handle it.
			if (patient->getRequestTime() == time)
			{
				AllPatients.dequeue(patient);
				return true;
			}
			// Else all patient requests for this time step have been allocated
			return false;
		}
	}

	// Given the type of patient, for all hospitals, a patient is removed from hospital list and added to patient finished list.
	void HandleHospital(PatientType type) {
		if (type == PatientType::SP) {
			Patient* RemovedPatient;
			for (int i = 0; i < NumHospitals; i++) {
				if (Hospitals[i]->RemoveSP(RemovedPatient))
				{
					//Car* assignedCar = Hospitals[i]->(assignedCar);
					//int assignmentTime = getTimeStep(); // Assuming this is the current time

					//// Calculate Pickup Time (PT)
					//int pickupTime = assignmentTime + (DistanceMatrix[RemovedPatient->getHID() - 1][RemovedPatient->getHID()] / assignedCar->getSpeed());

					//// Calculate Finish Time (FT)
					//int finishTime = pickupTime + (DistanceMatrix[RemovedPatient->getHID() - 1][RemovedPatient->getHID()] / assignedCar->getSpeed());

					//// Calculate Patient Waiting Time (WT)
					//int waitingTime = pickupTime - RemovedPatient->getRequestTime();
					//RemovedPatient->setWaitingTime(waitingTime);
					//RemovedPatient->setFinishTime(finishTime);


					AddPatientFinishedList(RemovedPatient);
				}
			}
		}
		else if (type == PatientType::Ep) {
			Patient* RemovedPatient;
			int severity;
			for (int i = 0; i < NumHospitals; i++) {
				if (Hospitals[i]->RemoveEP(RemovedPatient, severity))
				{
					AddPatientFinishedList(RemovedPatient);
				}
			}
		}
		else if (type == PatientType::NP) {
			Patient* RemovedPatient;
			for (int i = 0; i < NumHospitals; i++) {
				if (Hospitals[i]->RemoveNP(RemovedPatient))
				{
					AddPatientFinishedList(RemovedPatient);
				}
			}
		}
	}

	// Handles the transferring of cars from hospital to out.
	void HandleCars(CarType type) {
		// Checks the type of the car

		if (type == CarType::SC) {
			Car* movedcar;
			// For each hospital
			for (int i = 0; i < NumHospitals; i++) {
				// If I can remove a car i will do so and enqueue it.
				if (Hospitals[i]->RemoveSCar(movedcar))
				{
					int pri = 1;
					OutCars.enqueue(movedcar, pri);
					NumOutCars++;
				}
			}
		}
		else if (type == CarType::NC) {
			Car* movedcar;

			for (int i = 0; i < NumHospitals; i++) {

				if (Hospitals[i]->RemoveNCar(movedcar))
				{
					int pri = 1;
					OutCars.enqueue(movedcar, pri);
					NumOutCars++;
				}
			}
		}
	}
	// Handles moving cars from out to back
	void MoveOutToBack(int Timestep) {
		Car* tomove = nullptr;
		Patient* P = nullptr;
		int  pri;

		while (OutCars.peek(tomove, pri)) // pri = (-1) * picktime
		{
			if (pri * -1 != Timestep)
				break; //car shouldnt be returned to backlist yet

			P = tomove->getAssignedPatient();
			if (!P)
				break;
			P->setPickupTime(pri * -1);
			P->setFinishTime(P->getPickupTime() + P->getHospitalDistance() / tomove->getSpeed());
			tomove->updateBusyTime(P->getPickupTime() - P->getAssignmentTime());
			OutCars.dequeue(tomove, pri);
			int newPri = -1 * P->getFinishTime();
			tomove->setStatus(CarStatus::Loaded);
			BackCars.enqueue(tomove, newPri); //least finishtime should be first in queue
			NumBackCars++;
			NumOutCars--;
		}

		// Checks if outcars has a car
		/*if (OutCars.dequeue(tomove, pri))
		{
			BackCars.enqueue(tomove, pri);
			NumOutCars--;
			NumBackCars++;
		}*/
	}
	// loops on back cars list
	void MoveBackToFree(int Timestep) {

		Car* tomove = nullptr;
		Patient* P = nullptr;
		int pri;

		// loop on all back cars with same timestep
		while (BackCars.peek(tomove, pri))
		{

			if ((pri * -1) != Timestep || (P = tomove->getAssignedPatient()))
				break;

			if (!BackCars.dequeue(tomove, pri))
				break;

			NumBackCars--;
			tomove->updateBusyTime(P->getFinishTime() - P->getPickupTime());
			P->setCarId(-1); // -1: is not assigned a car
			AddPatientFinishedList(P);
			tomove->setAssignedPatient(nullptr);
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
			P = nullptr;
		}
	}
	// Assigning a car, removing it from its hospital and puting it in OutCars
	bool MoveFreeToOut(int Timestep, int hid)
	{
		Patient* AssignedP = nullptr;
		Car* AssignedC = nullptr;
		bool Cassigned = false;
		int pri;

		if (hid < 1 || hid > NumHospitals)
			return false;

		Hospital* H = Hospitals[hid - 1];

		H->HandlePatients();
		while (H->getAssignedCar(AssignedC))
		{
			AssignedP = AssignedC->getAssignedPatient();

			if (AssignedP == nullptr)
				continue; //just in case there was an error when assigning patients

			AssignedP->setAssignmentTime(Timestep);
			int Picktime = AssignedP->getAssignmentTime() + AssignedP->getHospitalDistance() / AssignedC->getSpeed();
			pri = -1 * Picktime;  // prioritize based on pick time lower pick time = higher priority

			// enqueue car to OutCars
			OutCars.enqueue(AssignedC, pri);
			NumOutCars++;
			Cassigned = true;
		}
		return Cassigned;
	}
	bool CancelRequest(int Timestep)
	{
		Cancellation* CancelReq = nullptr;
		Car* Car = nullptr;
		Patient* P = nullptr;
		int pri;
		while (AllCancellations.dequeue(CancelReq))
		{

			int hid = CancelReq->HID;
			if (hid < 1 || hid > NumHospitals) { //check validity of hid
				delete CancelReq; //this request shouldn't happen
				continue;
			}
			Hospital* H = Hospitals[hid - 1];
			H->GetNPlist()->removeItem(P, CancelReq->PID);

			if (!P)
				continue;

			if (Timestep > P->getAssignmentTime() + P->getHospitalDistance() / H->getNCarspeed())
				continue; //patient cannot cancel a request while in a car ***waiting P only***

			int Carid = P->getCarId();

			if (Carid == -1) { // no car assigned
				continue; // No car to handle
			}

			//getting car that was assigned this patient
			if (OutCars.removeItem(Car, pri, Carid))
			{
				Car->setAssignedPatient(nullptr);
				Car->updateBusyTime((Timestep - P->getAssignmentTime()) * 2);
				P->setCarId(-1); //car removed from patient
				BackCars.enqueue(Car, pri);
				NumOutCars--;
				NumBackCars++;
			}

		}
		CancelReq = nullptr;
		P = nullptr;
		return true;
	}
	void FailAction() {
		Car* tofail;
		int pri;
		if (OutCars.dequeue(tofail, pri))
		{
			NumOutCars--;
			BackCars.enqueue(tofail, pri);
			NumBackCars++;
			if (tofail->getStatus() == CarStatus::Assigned) {
				Patient* movedpatient = tofail->getAssignedPatient();
				Hospital* currentH = Hospitals[movedpatient->getHID() - 1];
				PatientType type = movedpatient->getType();
				if (type == PatientType::Ep) {
					int severity = movedpatient->getSeverity();
					currentH->AddtoFrontEp(movedpatient, severity);
				}
				else if (type == PatientType::NP) {
					currentH->AddtoFrontNp(movedpatient);
				}
				else if (type == PatientType::SP) {
					currentH->AddtoFrontNp(movedpatient);
				}
			}
			BackCars.dequeue(tofail, pri);
			NumBackCars--;
			CheckupList.enqueue(tofail);
			NumCheckupCars++;
		}
		else {
			//To test
			//cout << "No OUT cars" << endl;
		}
	}

	void InteractiveMode(int timestep) {
		UI call;
		call.PrintOutput(timestep, Hospitals, &OutCars, &BackCars, &FinishedPatients, &CheckupList, NumFinishedPatients, NumOutCars, NumBackCars, NumHospitals, NumCheckupCars);

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
			Output << std::setw(6) << "[NP: " << TotalNumNP << "," << "SP: " << TotalNumSP << "," << "EP: " << TotalNumEP << "]" << endl;
			Output << "Hospitals= " << NumHospitals << endl;
			Output << "Cars: " << TotalNumNC + TotalNumSC;
			Output << std::setw(6) << "[SCar: " << TotalNumSC << "," << "NCar: " << TotalNumNC << "]" << endl;
			Output << "Avg Wait Time= " << GetAvgWaitTime() << endl;
			Output << "Avg Busy Time= " << GetAvgBusyTime() << endl;
			Output << "Avg Utilization= " << Utilization << endl;
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
				Hospitals[j]->AddEP(p, severity);

				// Update the "has unassigned ep" bool of the hospital after moving
				Hospitals[i]->setHasUnassignedEP(Hospitals[i]->getEPlistEmpty());
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

	void Simulate(string sample_input)
	{
		UI ui;
		LoadFile("sample_input_5.txt");
		int mode = ui.GetInput();

		int timestep = 0;
		srand(static_cast<unsigned>(time(0)));
		// Program will end when all patients have been moved to the finish list
		// Cancel requests are not handled in phase 1.2
		srand(static_cast<unsigned>(time(0)));  // to generate a new number in each run
		while (GetTotalNumFinished() != GetTotalNumReq()) {
			timestep++;
			// Patient to be moved from the AllPatients to its hospital
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
			ReassignEPs();


			int random_fail = randomExcluding(1, 100, -1);


			
			int OUTfailprob = getOUTfailprob();
			if (random_fail > 0 && random_fail <= OUTfailprob) {
				FailAction();
			}


			if (mode == 1) { // if it's in interactive mode print all necessary data from the organizer class
				cout << "Random: " << random_fail << endl;
				InteractiveMode(timestep);
			}
		}
		if (mode == 2) {
			SilentMode();
		}
		OutputFile();
		
	}

};
