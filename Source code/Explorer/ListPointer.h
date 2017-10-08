#pragma once

namespace MyExplorer
{
	class NodePointer
	{
	private:
		friend class ListPointer;
		TCHAR* data;
		NodePointer* next;
		NodePointer();
	};

	class ListPointer
	{
	private:
		NodePointer* head_;
	public:
		void AddPointer(TCHAR* data);
		ListPointer();
		~ListPointer();
	};
}