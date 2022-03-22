#pragma once
//This Header file contains all of includes which are being used in main.c
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#define BUFFER_SIZE 3101
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#include <math.h>
#include <crtdbg.h>
#include <string.h>
#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <stdbool.h>
SOCKET reciever_s;

int Decoder(char* uncoded, char* coded) {
	int filter = 128, i, j, k, k1, k2, k3, k4, k5, err_count = 0, CodedInt = 0;
	char data[8*31], decoded_data[26*8];

	for (int i = 0; i < 31; i++) {//extracting 15bits per tour (bit number i of each byte)
		if (coded[i] < 0)
		{
			CodedInt = (int)coded[i] + 256;
		}
		else
		{
			CodedInt = (int)coded[i];
		}
		filter = 128;
		for (int j = 0; j < 8; j++) {
			data[i * 8 + j] = ((CodedInt & filter) == 0) ? 0 : 1;
			filter /= 2;
		}
	}
	
	for (int i = 0; i < 8; i++) {// checking mistakes
	do {
			k1 = data[i * 31] ^ data[i * 31 + 2] ^ data[i * 31 + 4] ^ data[i * 31 + 6] ^ data[i * 31 + 8] ^ data[i * 31 + 10] ^ data[i * 31 + 12] ^ data[i * 31 + 14] ^ data[i * 31 + 16] ^ data[i * 31 + 18] ^ data[i * 31 + 20] ^ data[i * 31 + 22] ^ data[i * 31 + 24] ^ data[i * 31 + 26] ^ data[i * 31 + 28] ^ data[i * 31 + 30];
			k2 = data[(i * 31) + 1] ^ data[i * 31 + 2] ^ data[i * 31 + 5] ^ data[i * 31 + 6] ^ data[i * 31 + 9] ^ data[i * 31 + 10] ^ data[i * 31 + 13] ^ data[i * 31 + 14] ^ data[i * 31 + 17] ^ data[i * 31 + 18] ^ data[i * 31 + 21] ^ data[i * 31 + 22] ^ data[i * 31 + 25] ^ data[i * 31 + 26] ^ data[i * 31 + 29] ^ data[i * 31 + 30];
			//k1 = data[i * 15] ^ data[i * 15 + 2] ^ data[i * 15 + 4] ^ data[i * 15 + 6] ^ data[i * 15 + 8] ^ data[i * 15 + 10] ^ data[i * 15 + 12] ^ data[i * 15 + 14];
			//k2 = data[i * 15 + 1] ^ data[i * 15 + 2] ^ data[i * 15 + 5] ^ data[i * 15 + 6] ^ data[i * 15 + 9] ^ data[i * 15 + 10] ^ data[i * 15 + 13] ^ data[i * 15 + 14];
			k3= data[(i * 31) + 3] ^ data[i * 31 + 4] ^ data[i * 31 + 5] ^ data[i * 31 + 6] ^ data[i * 31 + 11] ^ data[i * 31 + 12] ^ data[i * 31 + 13] ^ data[i * 31 + 14] ^ data[i * 31 + 19] ^ data[i * 31 + 20] ^ data[i * 31 + 21] ^ data[i * 31 + 22] ^ data[i * 31 + 27] ^ data[i * 31 + 28] ^ data[i * 31 + 29] ^ data[i * 31 + 30];
			k4= data[(i * 31) + 7] ^ data[i * 31 + 8] ^ data[i * 31 + 9] ^ data[i * 31 + 10] ^ data[i * 31 + 11] ^ data[i * 31 + 12] ^ data[i * 31 + 13] ^ data[i * 31 + 14] ^ data[i * 31 + 23] ^ data[i * 31 + 24] ^ data[i * 31 + 25] ^ data[i * 31 + 26] ^ data[i * 31 + 27] ^ data[i * 31 + 28] ^ data[i * 31 + 29] ^ data[i * 31 + 30];
			k5= data[(i * 31) + 15] ^ data[i * 31 + 16] ^ data[i * 31 + 17] ^ data[i * 31 + 18] ^ data[i * 31 + 19] ^ data[i * 31 + 20] ^ data[i * 31 + 21] ^ data[i * 31 + 22];
			//k3 = data[i * 15 + 3] ^ data[i * 15 + 4] ^ data[i * 15 + 5] ^ data[i * 15 + 6] ^ data[i * 15 + 11] ^ data[i * 15 + 12] ^ data[i * 15 + 13] ^ data[i * 15 + 14];
			//k4 = data[i * 15 + 7] ^ data[i * 15 + 8] ^ data[i * 15 + 9] ^ data[i * 15 + 10] ^ data[i * 15 + 11] ^ data[i * 15 + 12] ^ data[i * 15 + 13] ^ data[i * 15 + 14];
			k = k1 + 2 * k2 + 4 * k3 + 8 * k4 +16*k5 ;
			/*data[(i * 31) + 3] = data[i * 31 + 4] ^ data[i * 31 + 5] ^ data[i * 31 + 6] ^ data[i * 31 + 11] ^ data[i * 31 + 12] ^ data[i * 31 + 13] ^ data[i * 31 + 14] ^ data[i * 31 + 19] ^ data[i * 31 + 20] ^ data[i * 31 + 21] ^ data[i * 31 + 22] ^ data[i * 31 + 27] ^ data[i * 31 + 28] ^ data[i * 31 + 29] ^ data[i * 31 + 30];
		data[(i * 31) + 7] = data[i * 31 + 8] ^ data[i * 31 + 9] ^ data[i * 31 + 10] ^ data[i * 31 + 11] ^ data[i * 31 + 12] ^ data[i * 31 + 13] ^ data[i * 31 + 14]^ data[i * 31 + 23] ^ data[i * 31 + 24] ^ data[i * 31 + 25] ^ data[i * 31 + 26] ^ data[i * 31 + 27] ^ data[i * 31 + 28] ^ data[i * 31 + 29] ^ data[i * 31 + 30];
		data[(i * 31) + 15] = data[i * 31 + 16] ^ data[i * 31 + 17] ^ data[i * 31 + 18] ^ data[i * 31 + 19] ^ data[i * 31 + 20] ^ data[i * 31 + 21] ^ data[i * 31 + 22];
	}*/
			
			if (k != 0)
			{
				data[i * 31 + k - 1] = data[i * 31 + k - 1] ^ 1;
				err_count++;
			}

		} while (k != 0);
		
	}

	filter = 0;
	for (int i = 0; i < 8; i++) {//extracting only data_bits
		for (int j = 0; j < 31; j++) {
			if (j > 1 && j != 3 && j != 7 && j != 15) {
				decoded_data[filter] = data[i * 31 + j];
				filter++;
			}
		}
	}

	for (int i = 0; i < 26; i++) uncoded[i] = 0;

	filter = 128;

	for (int i = 0; i < 26; i++) {// writting dataout
		for (int j = 0; j < 8; j++) {
			uncoded[i] |= decoded_data[i * 8 + j] * filter;
			filter /= 2;
		}
		filter = 128;
	}
	return err_count;
}
int decode(char* buffer, int blen, FILE* f)
{
	/*
		Receives encoded (char*) buffer and a (FILE*) f, and writes to the file the decoded data from the buffer
	*/

	int errcount = 0;
	char decoded[27] = { 0 };
	int a = strlen(buffer);
	while (blen > 0) {
		errcount += Decoder(decoded, buffer);
		//strncat(dec_buffer, decoded, 12);
		fwrite(decoded, 26, 1, f);
		blen -= 31;
		buffer += 31;
	}
	return errcount;
}

int cleanupAll(char* recieved, int connected) {
	int to_return = 0;
	int result = WSACleanup();
	if (result != 0) {
		printf("WSACleanup failed: %d\n", result);
		to_return = 1;
	}
	closesocket(reciever_s);
	return to_return;
}

int main(int argc, char* argv[])
{
	//reading Arguments
	char IP[20], port[40];
	strcpy(IP, argv[1]);
	strcpy(port, argv[2]);
	int buff_length, flipped_bits = 0;
	int  retval, buffer_len, err_num = 0, received = 0;
	unsigned char dec_buff[2600] = { 0 }, buffer[BUFFER_SIZE] = { 0 };
	char str[4] = { 0 }, output[200];


	// Infinite iteration for socket opening until recieving quit:
	while (1) {

		//socket creation and conecction:
		WSADATA wsa_data; 	// Initialize Winsock
		int result;
		result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
		if (result != 0) {
			printf("WSAStartup failed: %d\n", result);
			return 1;
		}
		if ((reciever_s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) // Create and init socket in TCP protocol
		{
			printf("Could not create socket : %d", WSAGetLastError());
			return 1;
		}
		struct sockaddr_in client_addr; //Create a sockaddr_in object client_addr and set values.
		client_addr.sin_family = AF_INET;
		client_addr.sin_port = htons(atoi(port));			// port address
		client_addr.sin_addr.s_addr = inet_addr(IP);		// IP address
		int client_addr_len = sizeof(client_addr);



		while (1) {
			//Try to connect to Server
			if (connect(reciever_s, (SOCKADDR*)&client_addr, sizeof(client_addr)) == SOCKET_ERROR) {
				printf("Failed to connect to server on %s:%s  %d", argv[1], argv[2], WSAGetLastError());
				//	return 1;
			}
			printf("enter a file name or quit if done:\n");
			char f_name[100];
			// Read a char type variable,
			// store in "f_name"
			scanf("%s", f_name);
			if (!strcmp(f_name, "quit"))
				return cleanupAll(f_name, 0);
			FILE* f = NULL;
			f = fopen(f_name, "wb"); //Create an empty binary file for writing. If the file exists, its contents are cleared unless it is a logical file.
			if (f == NULL)
			{
				printf("File error. Coudn't open file\n");// CHECKING IF OPENED SUCCSESSFULLY
				return -1;
			}

			do {
				buff_length = recv(reciever_s, buffer, BUFFER_SIZE + 2, 0);
				received += buff_length;
				if (buff_length > 0)
					printf("Bytes received: %d\n the strins is %s\n", buff_length, buffer);
				else if (buff_length < 0) {
					printf("recv failed: %d\n", WSAGetLastError());
					//cleanup
				}

			} while (buff_length > 0);
			err_num += decode(buffer,sizeof(buffer), f);
			//err_num += decode(buffer, buff_length, f);
			sprintf(output, "received: %d bytes\nwrote: %d bytes\ndetected and corrected %d errors\n", received, received * 2600 / 3100, err_num);
			fclose(f);
		}
	}
}