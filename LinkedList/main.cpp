#include <iostream>
#include <ctime>

#include "LinkedList.h"


void FillList(LinkedList<int> &List, int Start, int End)
{
	for (int i = Start; i <= End; i++)
		List.PushBack(i);
}

template class LinkedList<int>;

int main()
{
	int ElementsCount = 0;
	std::cout << "\nHow many elements to test with?\n: ";
	std::cin >> ElementsCount;
	
	LinkedList<int> newList;

	time_t start, end;

	std::cout << "\nFilling " << ElementsCount << " element(s)...\n";
	start = std::time(nullptr);

	FillList(newList, 0, ElementsCount - 1);

	end = std::time(nullptr);
	std::cout << "   Execution took " << (end - start) << " sec.\n";


	std::cout << "\nIterating the list with the indexing operation...\n";
	start = std::time(nullptr);

	int foo;

	for(int i = 0; i < ElementsCount; i++)
		foo = newList.At(i);

	end = std::time(nullptr);
	std::cout << "   Execution took " << (end - start) << " sec.\n";

	std::cout << "\nIterating the list with an iterator...\n";
	start = std::time(nullptr);

	for (LinkedList<int>::Iterator it = newList.GetIterator();
         !it.EndReached();
	     it.MoveNext())
	{
		foo = it.GetCurrent();
	}

	end = std::time(nullptr);
	std::cout << "   Execution took " << (end - start) << " sec.\n";

	return 0;
}