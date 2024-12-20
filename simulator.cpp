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
	Org.Simulate("small_input_test_2.txt");
}