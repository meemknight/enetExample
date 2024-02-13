#include <enet/enet.h>
#include <iostream>
#include <networking.h>



void clientFunction()
{

	ENetHost *client = nullptr;
	ENetPeer *server = nullptr;

	client = enet_host_create(NULL /* create a client host */,
		1 /* only allow 1 outgoing connection */,
		CHANNELS /* allow up 2 channels to be used, 0 and 1 */,
		0 /* assume any amount of incoming bandwidth */,
		0 /* assume any amount of outgoing bandwidth */);

	if (client == NULL)
	{
		std::cout << "An error occurred while trying to create an ENet client host.\n";
		return;
	}

	ENetAddress address = {};
	//enet_address_set_host(&address, "127.0.0.1");
	enet_address_set_host(&address, "localhost");
	address.port = 1233;


	server = enet_host_connect(client, &address, CHANNELS, 0);

	if (server == nullptr)
	{
		std::cout << "Wasn't able to initialize connection\n";

		//don't forget this
		enet_host_destroy(client);

	}
	else
	{

		/* Wait up to 5 seconds for the connection attempt to succeed. */
		ENetEvent event = {};
		if (enet_host_service(client, &event, 5000) > 0 &&
			event.type == ENET_EVENT_TYPE_CONNECT)
		{
			std::cout << "Got a connection!\n";

			while (true)
			{

				ENetEvent event;
				/* Wait up to 10 milliseconds for an event. */
				while (enet_host_service(client, &event, 10) > 0)
				{
					switch (event.type)
					{
					case ENET_EVENT_TYPE_CONNECT:
					break;

					case ENET_EVENT_TYPE_RECEIVE:
					printf("A packet of length %u containing %s was received on channel %u.\n",
						event.packet->dataLength,
						event.packet->data,
						event.channelID);
					/* Clean up the packet now that we're done using it. */
					enet_packet_destroy(event.packet);

					break;

					case ENET_EVENT_TYPE_DISCONNECT:
					std::cout << "Server Disconected\n";
					break;
					}
				}

				//do other stuff

				std::string message = "";
				std::cin >> message;

				if (message != "")
				{
					sendMessage(message.c_str(), message.size()+1, server);
				}

			}
		}
		else
		{
			std::cout << "Wasn't able to connect\n";

		}

		enet_peer_reset(server);
		enet_host_destroy(client);

	}


}

