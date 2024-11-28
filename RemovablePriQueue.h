#pragma once
#include "priQueue.h"

// Note: Removable PriQueue only works on pointers of objects. As  dereferencing "*" is used to get the item.
// The T == (int ID) operator has been updated for the classes: Hospital, Car, and patient. To allow for the comparison of these classes with the ids.

template <typename T>
class RemovablePriQueue :public priQueue<T>
{
public:

	bool removeItem(T& rmvItem, int& priority, int ID)
	{
		if (this->isEmpty()) {
			return false; // Nothing to remove
		}

		// The item is at the front
		if (*(this->head->getItem(priority)) == ID) {
			priNode<T>* temp = this->head;
			this->head = this->head->getNext();
			rmvItem = temp->getItem(priority);
			delete temp;
			return true;
		}

		// General case
		priNode<T>* current = this->head;
		while (current->getNext() != nullptr) {
			if (*(current->getNext()->getItem(priority)) == ID) {
				priNode<T>* temp = current->getNext();
				current->setNext(temp->getNext());

				rmvItem = temp->getItem(priority);
				delete temp;
				return true;
			}
			current = current->getNext();
		}

		return false; // Item not found
	}
	//template<typename T>
	//void RemovablePriQueue<T>::print() {
	//	if (isEmpty)
	//		cout << "No elements to print! " << endl;
	//	else
	//	{
	//		priNode<T>* current = head;
	//		while (current)
	//		{
	//			cout << current->getItem() << endl;
	//			current = current->getNext();
	//		}
	//	}
	//}

	//template<typename T>
	//void RemovablePriQueue<T>::counter() {
	//	priNode<T>* current = head;
	//	int count = 1;
	//	while (current) {
	//		if (!current->getNext())
	//			count = 1;
	//		else
	//		{
	//			current = current->getNext();
	//			count++;
	//		}
	//	}
	//	cout << count;
	//}
};	//
