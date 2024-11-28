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

	// Lists
	LinkedQueue<Patient*> SPlist;
	RemovableQueue<Patient*> NPlist;  // NP's have the ability to cancel.
	priQueue<Patient*> EPlist;       // Sorting emergy patients using priQueue

	LinkedQueue<Car*> Scars;
	LinkedQueue<Car*> Ncars;

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
		AvailableNumScars--;
		return Scars.dequeue(car);
	}

	bool RemoveNCar(Car*& car)
	{
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
		NumSPRequests--;
		return SPlist.dequeue(p);
	}

	bool RemoveNP(Patient*& p)
	{
		NumNPRequests--;
		return NPlist.dequeue(p);
	}

	bool RemoveEP(Patient*& p, int& severity)
	{
		NumEPRequests--;
		return EPlist.dequeue(p, severity);
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
	priQueue<Patient*>& GetEPlist() {
		return EPlist;
	}
	RemovableQueue<Patient*>& GetNplist() {
		return NPlist;
	}
	LinkedQueue<Patient*>& GetSplist() {
		return SPlist;
	}
	LinkedQueue<Car*>& GetSCar() {
		return Scars;
	}
	LinkedQueue<Car*>& GetNCar() {
		return Ncars;
	}
	// Operator overloading

	// overloading to allow the comparison of Hospitals with int ID. To see if the Hospital object has that ID.
	bool operator==(int ID) const {
		return HID == ID;
	}
};
