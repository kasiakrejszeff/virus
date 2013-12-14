// g++ ważne_rzeczy_z_czytanek.cpp -o w -std=c++11
#include <iostream>

using namespace std;

int main() {
	// Ważne rzeczy z czytanek:

	// a) Kiedy zabraknie miejsca, alokacja nie powiedzie się.
	//    Rozwiązania:
	{
		// 1. Chwytanie wyjątków:
		int * a1, * a2;
		try {
			a1 = new int [5];
		}
		catch (const std::bad_alloc&) {
			cerr << "Allocation failed." << endl;
		}

		// 2. Użycie nothrow:
		a2 = new (nothrow) int [5];
		if (a2 == nullptr) {
			cerr << "Allocation failed." << endl;
		}
	}
}