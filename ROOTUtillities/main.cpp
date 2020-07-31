#include <iostream>
#include "DataTable.h"

int main()
{
	RootUtilities::DataTable t;
	t.load_from_csv("data.csv", "L8:O25", X_DATA, X_ERROR_DATA, Y_DATA, Y_ERROR_DATA);
	t.print_table();
	std::cout<<t.get_xMax()<<'\n';
	std::cout << t.get_length();
	return 0;
}
