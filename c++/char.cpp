


#include <iostream>

int main()
{
	char str[20] = "realize this?";
	char *p = str;
	std::cout << *p << std::endl; //prints the first element
	std::cout << *(p + 1) << std::endl; //prints the second element
	return 0;
}
