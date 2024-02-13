#include <enet/enet.h>
#include <iostream>
#include <networking.h>



void sendMessage(const char *data, size_t s, ENetPeer *to)
{
	ENetPacket *packet = enet_packet_create(data, s, ENET_PACKET_FLAG_RELIABLE);

	//the second parameter is the channel id
	enet_peer_send(to, 0, packet);

}


void serverFunction()
{


	ENetAddress address = {};
	ENetHost *server = nullptr;
	address.host = ENET_HOST_ANY; //we accept cleints from any ip address since we are the server
	address.port = 1233;


	server = enet_host_create(&address /* the address to bind the server host to */,
		32			/* allow up to 32 clients and/or outgoing connections */,
		CHANNELS	/* allow up to 2 channels to be used, 0 and 1 */,
		0			/* assume any amount of incoming bandwidth */,
		0			/* assume any amount of outgoing bandwidth */);

	if (server == NULL)
	{
		std::cout << "Problem creating server\n";
		return;
	}
	
	ENetPeer *client = 0;
	
	//server game loop
	while (true)
	{

		//recieve events

		ENetEvent event = {};
		/* Wait up to 10 milliseconds for an event. */
		//you probably don't want to wait here in a game and do other stuff,
		//or calculate how much you can wait each time
		while (enet_host_service(server, &event, 10) > 0)
		{
			switch (event.type)
			{

				case ENET_EVENT_TYPE_CONNECT:
				{
					printf("A new client connected from %x:%u.\n",
						event.peer->address.host,
						event.peer->address.port);

					client = event.peer;
						
					break;
				}
				case ENET_EVENT_TYPE_RECEIVE:
				{
					printf("A packet of length %u containing %s was received on channel %u.\n",
						event.packet->dataLength,
						event.packet->data,
						event.channelID);
					/* Clean up the packet now that we're done using it. */
					enet_packet_destroy(event.packet);
				
					break;
				}

				case ENET_EVENT_TYPE_DISCONNECT:
				{
					printf("client disconnected.\n");


					//we should also check what client disconected but we will just assume
					//we can have only one connection in this simple example.
					client = 0;

					break;
				}

			}

			if (client)
			{
				std::string message = "";
				std::cin >> message;

				if (message != "")
				{
					sendMessage(message.c_str(), message.size()+1, client);
				}
			}



		}


	}


		
	enet_host_destroy(server);
	
}