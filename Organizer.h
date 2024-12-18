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
	void MoveOutToBack() {
		Car* tomove;
		int  pri;

		// Checks if outcars has a car
		if (OutCars.dequeue(tomove, pri))
		{
			BackCars.enqueue(tomove, pri);
			NumOutCars--;
			NumBackCars++;
		}
	}
	// Moves one car from back list to its hospital
	void MoveBackToFree() {

		Car* tomove;
		int pri;
		if (BackCars.dequeue(tomove, pri))
		{
			NumBackCars--;
			Hospital* currentH = Hospitals[tomove->getHID() - 1];

			CarType cartype = tomove->getType();
			if (cartype == CarType::NC)
			{
				currentH->AddNCar(tomove);
			}
			else
			{
				currentH->AddSCar(tomove);
			}
		}


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

	};

