#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <winsock2.h>

static const LPCTSTR lpCaption = TEXT("WakeOnLAN");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	WORD    wVersionRequested = MAKEWORD(1, 1);
	WSADATA wsaData; 
	CHAR    errorMessage[255];
	BYTE    buffer[102];
	int     wsaError;
	int     ii;
	int     idx;
	char    opt = 1;
	LPSTR   lpCopy;
	LPSTR   lpTok;
	SOCKET  sock;
	struct  sockaddr_in dest;
	
	/* Check for command line arguments */
	if (lpszCmdLine == NULL || lstrlen(lpszCmdLine) == 0)
	{
		MessageBox((HWND)NULL, TEXT("No MAC address specified"), lpCaption, MB_ICONERROR);
		return 1;
	}
	
	if (WSAStartup(wVersionRequested, &wsaData) == 0)
	{
		/* Create magic packet
		 * See https://en.wikipedia.org/wiki/Wake-on-LAN#Magic_packet for format
		 */
		for (ii = 0; ii < 6; ++ii)
		{
			buffer[ii] = 0xff;
		}
		/* Parse MAC address from command line */
		lpCopy  = (LPSTR)GlobalAlloc(0, (lstrlen(lpszCmdLine) + 1) * sizeof(CHAR));
		lstrcpy(lpCopy, lpszCmdLine);
		idx = 6;
		for (ii = 0; ii < 6; ++ii)
		{
			if (ii == 0)
				lpTok = (LPSTR)strtok(lpCopy, ":-");
			else
				lpTok = (LPSTR)strtok(NULL, ":-");
			if (lpTok == NULL)
			{
				MessageBox((HWND)NULL, TEXT("Error parsing MAC address"), lpCaption, MB_ICONERROR);
				return 1;
			}
			UINT value;
			sscanf(lpTok, "%2x", &value);
			buffer[idx++] = (BYTE)value;
		}
		GlobalFree(lpCopy);
		/* Make copies of MAC address */
		idx = 6;
		for( ii = 0; ii < 15; ++ii)
		{
			memcpy(&buffer[idx + 6], &buffer[idx], 6);
			idx += 6;
		}
		
		/* Send magic packet */
		sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(sock == INVALID_SOCKET)
		{
			wsaError = WSAGetLastError();
			sprintf(errorMessage, "Error calling socket()\nError code: %d", wsaError);
			MessageBox((HWND)NULL, (LPCTSTR)errorMessage, lpCaption, MB_ICONERROR);
			return 1;
		}
		if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(int)) != 0)
		{
			wsaError = WSAGetLastError();
			sprintf(errorMessage, "Error calling setsockopt()\nError code: %d", wsaError);
			MessageBox((HWND)NULL, (LPCTSTR)errorMessage, lpCaption, MB_ICONERROR);
			closesocket(sock);
			return 1;
		}
		dest.sin_family = AF_INET;
		dest.sin_port = 9;
		dest.sin_addr.s_addr = inet_addr("255.255.255.255");
		if (sendto(sock, (char *)buffer, 102, 0, (struct sockaddr *) &dest, sizeof(dest)) == SOCKET_ERROR)
		{
			wsaError = WSAGetLastError();
			sprintf(errorMessage, "Error calling sendto()\nError code: %d", wsaError);
			MessageBox((HWND)NULL, (LPCTSTR)errorMessage, lpCaption, MB_ICONERROR);
		}
		closesocket(sock);
	}
	else
	{
		wsaError = WSAGetLastError();
		sprintf(errorMessage, "Error initializing Winsock\nError code: %d", wsaError);
		MessageBox((HWND)NULL, (LPCTSTR)errorMessage, lpCaption, MB_ICONERROR);
	}
	
	return 0;
}
