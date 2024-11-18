#pragma once
#include "LinkedQueue.h"

// Note: Removable Queue only works on pointers of objects. As  dereferencing "*" is used to get the item.
// The T == (int ID) operator has been updated for the classes: Hospital, Car, and patient. To allow for the comparison of these classes with the ids.
template <typename T>
class RemovableQueue :public LinkedQueue<T>
{
public:

	bool removeItem(T& rmvItem, int ID)
	{
		if (this->isEmpty()) {
			return false; // Nothing to remove
		}

		// The item is at the front
		if (*(this->frontPtr->getItem()) == ID) {
			Node<T>* temp = this->frontPtr;
			this->frontPtr = this->frontPtr->getNext();
			rmvItem = temp->getItem();
			delete temp;

			// If the list is now empty, update this->backPtr
			if (this->frontPtr == nullptr) {
				this->backPtr = nullptr;
			}
			return true;
		}

		// General case
		Node<T>* current = this->frontPtr;
		while (current->getNext() != nullptr) {
			if (*(current->getNext()->getItem()) == ID) {
				Node<T>* temp = current->getNext();
				current->setNext(temp->getNext());

				// if node to be deleted at the end, this->backPtr should point at current node (one before the deleted)
				if (temp == this->backPtr) {
					this->backPtr = current;
				}
				rmvItem = temp->getItem();
				delete temp;
				return true;
			}
			current = current->getNext();
		}

		return false; // Item not found
	}
};
