#pragma once
//This Header file contains all of includes which are being used in main.c
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#define BUFFER_SIZE 1488 //closest multiplication of 31 to 1500
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

int hamming_decode(char* res_str, char* coded) {
	/*
		Receives coded 31 bits char and an empty char res_str.
		fills res_str with the 26 bits correction of the 31 bits string accordint to hamming correction code 
	*/
	int filter = 128, i, j, k, k1, k2, k3, k4, k5, err_count = 0, CodedInt = 0;
	char data[8*31], decoded_data[26*8];

	//extracting 26 bits per tour (bit number i of each byte)
	for (int i = 0; i < 31; i++) {
	
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
	
	// detecting errors
	for (int i = 0; i < 8; i++) {
	do {
			k1 = data[i * 31] ^ data[i * 31 + 2] ^ data[i * 31 + 4] ^ data[i * 31 + 6] ^ data[i * 31 + 8] ^ data[i * 31 + 10] ^ data[i * 31 + 12] ^ data[i * 31 + 14] ^ data[i * 31 + 16] ^ data[i * 31 + 18] ^ data[i * 31 + 20] ^ data[i * 31 + 22] ^ data[i * 31 + 24] ^ data[i * 31 + 26] ^ data[i * 31 + 28] ^ data[i * 31 + 30];
			k2 = data[(i * 31) + 1] ^ data[i * 31 + 2] ^ data[i * 31 + 5] ^ data[i * 31 + 6] ^ data[i * 31 + 9] ^ data[i * 31 + 10] ^ data[i * 31 + 13] ^ data[i * 31 + 14] ^ data[i * 31 + 17] ^ data[i * 31 + 18] ^ data[i * 31 + 21] ^ data[i * 31 + 22] ^ data[i * 31 + 25] ^ data[i * 31 + 26] ^ data[i * 31 + 29] ^ data[i * 31 + 30];
			k3= data[(i * 31) + 3] ^ data[i * 31 + 4] ^ data[i * 31 + 5] ^ data[i * 31 + 6] ^ data[i * 31 + 11] ^ data[i * 31 + 12] ^ data[i * 31 + 13] ^ data[i * 31 + 14] ^ data[i * 31 + 19] ^ data[i * 31 + 20] ^ data[i * 31 + 21] ^ data[i * 31 + 22] ^ data[i * 31 + 27] ^ data[i * 31 + 28] ^ data[i * 31 + 29] ^ data[i * 31 + 30];
			k4= data[(i * 31) + 7] ^ data[i * 31 + 8] ^ data[i * 31 + 9] ^ data[i * 31 + 10] ^ data[i * 31 + 11] ^ data[i * 31 + 12] ^ data[i * 31 + 13] ^ data[i * 31 + 14] ^ data[i * 31 + 23] ^ data[i * 31 + 24] ^ data[i * 31 + 25] ^ data[i * 31 + 26] ^ data[i * 31 + 27] ^ data[i * 31 + 28] ^ data[i * 31 + 29] ^ data[i * 31 + 30];
			k5= data[(i * 31) + 15] ^ data[i * 31 + 16] ^ data[i * 31 + 17] ^ data[i * 31 + 18] ^ data[i * 31 + 19] ^ data[i * 31 + 20] ^ data[i * 31 + 21] ^ data[i * 31 + 22]^ data[(i * 31) + 23] ^ data[i * 31 + 24] ^ data[i * 31 + 25] ^ data[i * 31 + 26] ^ data[i * 31 + 27] ^ data[i * 31 + 28] ^ data[i * 31 + 29] ^ data[i * 31 + 30];
			k = k1 + 2 * k2 + 4 * k3 + 8 * k4 + 16 * k5 ;
			
			if (k != 0)
			{
				data[i * 31 + k - 1] = data[i * 31 + k - 1] ^ 1;
				err_count++;
			}

		} while (k != 0);
		
	}

	//extracting only data_bits
	filter = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 31; j++) {
			if (j > 1 && j != 3 && j != 7 && j != 15) {
				decoded_data[filter] = data[i * 31 + j];
				filter++;
			}
		}
	}

	for (int i = 0; i < 26; i++) res_str[i] = 0;

	filter = 128;

	// writting decoded data
	for (int i = 0; i < 26; i++) {
		for (int j = 0; j < 8; j++) {
			res_str[i] |= decoded_data[i * 8 + j] * filter;
			filter /= 2;
		}
		filter = 128;
	}
	return err_count;

}
int decode(char* buffer, int blen, FILE* f)
{
	/*
		Receives encoded (char*) buffer and a (FILE*) f.
		calls decoder function and writes to the file the decoded data from the buffer.
	*/

	int errcount = 0;
	char decoded[27] = { 0 };

	while (blen > 0 ) {
		errcount += hamming_decode(decoded, buffer);
		fwrite(decoded, 26, 1, f);
		blen -= 31;
		buffer += 31;
	}
	return errcount;
}

int cleanupAll() {
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
	unsigned char dec_buff[2600] = { 0 }, buffer[BUFFER_SIZE + 1] = { 0 };
	char str[4] = { 0 }, output[200];
	WSADATA wsa_data; 	// Initialize Winsock
	int result,degel=1;
	result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (result != 0) {
		printf("WSAStartup failed: %d\n", result);
		return 1;
	}
	// Infinite iteration for socket opening until recieving quit:
	while (1) {
		//socket creation and conecction:
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

			if (connect(reciever_s, (SOCKADDR*)&client_addr, sizeof(client_addr)) == SOCKET_ERROR) {
				printf("Failed to connect to server on %s:%s  %d", argv[1], argv[2], WSAGetLastError());
				//	return 1;
			}
			char f_name[100];
			if (degel) {
				printf("enter a file name or quit if done:\n");
				
				// Read a char type variable,
				// store in "f_name"
				scanf("%s", f_name);

				if (!strcmp(f_name, "quit"))
					return cleanupAll();
			}
			FILE* f = NULL;
			f = fopen(f_name, "wb"); //open file in write bits mode, if exists it will overwrite.
			if (f == NULL)
			{
				printf("File error. Coudn't open file\n");// CHECKING IF OPENED SUCCSESSFULLY
				return -1;
			}

			do {
				buff_length = recv(reciever_s, buffer, BUFFER_SIZE , 0);
				received += buff_length;
				for (int i = buff_length; i < BUFFER_SIZE; i++)
				{
					buffer[i] = '\0';
				}
				
				if (buff_length < 0) {
					printf("recv failed: %d\n", WSAGetLastError());
					return cleanupAll();
										
				}
				err_num += decode(buffer, buff_length, f);

			} while (buff_length > 0);
			int a = received * 26 / 31;
			printf("\nreceived: %d bytes\nwrote: %d bytes\ncorrected %d errors\n", received, received*26/31, err_num);
			
			fclose(f);
			received = 0;
			err_num = 0;
			degel = 0;
			printf("enter a file name or quit if done:\n");
			scanf("%s", f_name);
			if (!strcmp(f_name, "quit"))
				return cleanupAll();
		
	}
}
