#include "stdafx.h"
#include "ListPointer.h"

namespace MyExplorer
{
	NodePointer::NodePointer()
	{
		this->data = NULL;
		this->next = NULL;
	}

	ListPointer::ListPointer()
	{
		head_ = NULL;
	}

	void ListPointer::AddPointer(TCHAR* data)
	{
		NodePointer* result = new NodePointer;
		if (result == NULL)
		{
			return;
		}

		result->data = data;

		if (head_ == NULL)
		{
			head_ = result;
		}
		else
		{
			result->next = head_;
			head_ = result;
		}
	}
	
	ListPointer::~ListPointer()
	{
		NodePointer* current = head_;
		while (head_)
		{
			head_ = current->next;
			TCHAR* temp = current->data;
			delete[] temp;
			delete current;
			current = head_;
		}
	}
}