#pragma once

void serverFunction();
void clientFunction();

void sendMessage(const char *data, size_t s, ENetPeer *to);


constexpr size_t CHANNELS = 2;
