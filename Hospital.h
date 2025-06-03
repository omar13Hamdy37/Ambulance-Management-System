#pragma once
#include "Patient.h"
#include "Car.h"
#include "priQueue.h"
#include "LinkedQueue.h"
#include "RemovableQueue.h"
#include "RemovablePriQueue.h"

class Hospital
{
private:

	// Normal data members
	int HID;

	int TotalNumScars, TotalNumNcars;

	int AvailableNumScars, AvailableNumNCars;

	int NumSPRequests, NumNPRequests, NumEPRequests;

	bool HasUnassignedEP;

	// Lists
	LinkedQueue<Patient*> SPlist;
	RemovableQueue<Patient*> NPlist;  // NP's have the ability to cancel.
	priQueue<Patient*> EPlist;       // Sorting emergency patients using priQueue

	RemovableQueue<Patient*> NpWaitList;
	LinkedQueue<Car*> Scars;
	LinkedQueue<Car*> Ncars;

	LinkedQueue<Car*> AssignedCars;

public:
	// Constructor

	Hospital(int HID) : HID(HID)
	{
		NumSPRequests = 0; NumEPRequests = 0; NumNPRequests = 0;
		AvailableNumScars = 0; AvailableNumNCars = 0;
	}
	// Add Cars
	void AddSCar(Car* car)
	{
		Scars.enqueue(car);
		AvailableNumScars++;
	}
	void AddNCar(Car* car)
	{
		Ncars.enqueue(car);
		AvailableNumNCars++;
	}
	// Remove Cars
	bool RemoveSCar(Car*& car)
	{
		if (Scars.isEmpty())
		{
			return false;
		}
		AvailableNumScars--;
		return Scars.dequeue(car);
	}

	bool RemoveNCar(Car*& car)
	{
		if (Ncars.isEmpty())
		{
			return false;
		}
		AvailableNumNCars--;
		return Ncars.dequeue(car);
	}

	// Add Request

	void AddSP(Patient* p)
	{
		SPlist.enqueue(p);
		NumSPRequests++;
	}
	void AddNP(Patient* p)
	{
		NPlist.enqueue(p);
		NumNPRequests++;
	}

	void AddEP(Patient* p, int severity)
	{
		EPlist.enqueue(p, severity);
		NumEPRequests++;
	}
	// Remove Request

	bool RemoveSP(Patient*& p)
	{
		if (SPlist.isEmpty())
		{
			p = nullptr;
			return false;
		}
		NumSPRequests--;
		return SPlist.dequeue(p);
	}

	bool RemoveNP(Patient*& p)
	{
		if (NPlist.isEmpty())
		{
			p = nullptr;
			return false;
		}
		NumNPRequests--;
		return NPlist.dequeue(p);
	}

	bool RemoveEP(Patient*& p, int& severity)
	{
		if (EPlist.isEmpty())
		{
			p = nullptr;
			return false;
		}
		NumEPRequests--;
		return EPlist.dequeue(p, severity);
	}

	// Adding and removing patients from wait list
	void AddNPWaitlist(Patient* p)
	{
		NpWaitList.enqueue(p);
	}

	bool RemoveNpWait(Patient*& p, int pid)
	{
		if (NpWaitList.isEmpty())
			return false;

		NpWaitList.removeItem(p, pid);
	}

	// functions to add a patient to the front of a queue
	void AddToFrontSP(Patient* p) {
		LinkedQueue<Patient*> temp;
		Patient* tempP;
		while (SPlist.dequeue(tempP)) {
			temp.enqueue(tempP);
		}
		SPlist.enqueue(p);
		NumSPRequests++;
		while (temp.dequeue(tempP)) {
			SPlist.enqueue(tempP);
		}
	}
	void AddToFrontNP(Patient* p) {
		LinkedQueue<Patient*> temp;
		Patient* tempP;
		while (NPlist.dequeue(tempP)) {
			temp.enqueue(tempP);
		}
		NPlist.enqueue(p);
		NumNPRequests++;
		while (temp.dequeue(tempP)) {
			NPlist.enqueue(tempP);
		}
	}
	void AddToFrontEP(Patient* p, int severity) {
		EPlist.enqueue(p, 1000);
		NumEPRequests++;
	}
	// Setters
	void setTotalNumScars(int totalScars) {
		TotalNumScars = totalScars;
		AvailableNumScars = totalScars;
	}

	void setTotalNumNcars(int totalNcars) {
		TotalNumNcars = totalNcars;
		AvailableNumNCars = totalNcars;
	}

	void setAvailableNumNcars(int n) {
		AvailableNumNCars = n;
	}
	void setAvailableNumScars(int n) {
		AvailableNumScars = n;
	}
	void setHasUnassignedEP(bool b)
	{
		HasUnassignedEP = b;
	}

	// Getters
	int getTotalNumScars() const {
		return TotalNumScars;
	}

	int getTotalNumNcars() const {
		return TotalNumNcars;
	}

	int getAvailableNumScars() const {
		return AvailableNumScars;
	}

	int getAvailableNumNCars() const {
		return AvailableNumNCars;
	}

	int getNumSPRequests() const {
		return NumSPRequests;
	}

	int getNumEPRequests() const {
		return NumEPRequests;
	}

	int getNumNPRequests() const {
		return NumNPRequests;
	}

	bool getHasUnassignedEP() const {
		return HasUnassignedEP;
	}
	bool getEPlistEmpty()const {
		return EPlist.isEmpty();
	}

	priQueue<Patient*>* GetEPlist() {
		return &EPlist;
	}
	RemovableQueue<Patient*>* GetNPlist() {
		return &NPlist;
	}
	LinkedQueue<Patient*>* GetSPlist() {
		return &SPlist;
	}
	LinkedQueue<Car*>* GetSCar() {
		return &Scars;
	}
	LinkedQueue<Car*>* GetNCar() {
		return &Ncars;
	}
	RemovableQueue<Patient*>* GetNpWaitList() {
		return &NpWaitList;
	}

	int getNCarSpeed() {
		Car* C = nullptr;
		Ncars.peek(C);
		return C->getSpeed();
	}
	int getSCarSpeed() {
		Car* C = nullptr;
		Scars.peek(C);
		return C->getSpeed();
	}

	// overloading to allow the comparison of Hospitals with int ID. To see if the Hospital object has that ID.
	bool operator==(int ID) const {
		return HID == ID;
	}
	// Check if hospital has available cars
	bool CarAvailable()
	{
		return !(Ncars.isEmpty() && Scars.isEmpty());
	}
	void AddAssignedCar(Car*& C)
	{
		AssignedCars.enqueue(C);
	}
	bool getAssignedCar(Car*& C)
	{
		if (AssignedCars.isEmpty())
			return false;

		AssignedCars.dequeue(C);
		return true;
	}

	// A function that handles all current requests for the hospital
	void HandlePatients()
	{
		// The patient that will be removed from requests list
		Patient* p;
		int severity;
		// The car of the patient
		Car* c;

		// Patient-Car assignment will stop, if no more requests present or no cars are available
		while (!EPlist.isEmpty() && !(Scars.isEmpty() && Ncars.isEmpty()))
		{
			// We have the patient
			RemoveEP(p, severity);
			// We have the car. (if both empty we wouldn't enter while loop)
			if (!Ncars.isEmpty())
			{
				RemoveNCar(c);
			}
			else
			{
				RemoveSCar(c);
			}

			c->setAssignedPatient(p);
			AddAssignedCar(c);
		}

		// Here we decide if the Hospital has unassigned EP (to be reassigned later)

		if (EPlist.isEmpty())
		{
			HasUnassignedEP = false;
		}
		else
		{
			HasUnassignedEP = true;
		}

		// Patient-Car assignment will stop, if no more requests present or no cars are available
		while (!SPlist.isEmpty() && !Scars.isEmpty())
		{
			// We have the patient
			RemoveSP(p);
			// We have the car. (if empty we wouldn't enter while loop)
			RemoveSCar(c);
			c->setAssignedPatient(p);
			AddAssignedCar(c);
		}

		// Patient-Car assignment will stop, if no more requests present or no cars are available
		while (!NPlist.isEmpty() && !Ncars.isEmpty())
		{
			// We have the patient
			RemoveNP(p);
			// We have the car. (if empty we wouldn't enter while loop)
			RemoveNCar(c);
			c->setAssignedPatient(p);
			AddAssignedCar(c);
		}
	}
};
