#include <iostream>
#include <enet/enet.h>
#include <networking.h>
#include <cctype>



//IMPORTANT NOTE

//THIS EXAMPLE USES IOSTREAM FOR INPUT OUTPUT AND THIS BLOCKS THE PROGRAM WAITING FOR INPUT.
//THIS CAN CAUSE THE SERVER TO CRASH, IN A REAL WORLD APPLICATION YOU SHOULD NOT BLOCK
//THE PROGRAM AND CALL enet_host_service FREQUENTLY


int main()
{

	if (enet_initialize() != 0)
	{
		std::cout << "Error initializing enet...";
		return 0;
	}


	while (true)
	{
		std::cout << "(S)erver or (C)lient?: ";

		char c = 0;
		if (std::cin >> c)
		{
			if (std::tolower(c) == 's')
			{
				serverFunction();
				break;
			}
			else if (std::tolower(c) == 'c')
			{
				clientFunction();
				break;
			}
		}
	}


	return 0;
}