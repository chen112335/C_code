#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // ¡¥Ω”winsockø‚

#define PORT 8080
#define BUF_SIZE 1024

const char* response = "HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nContent-Length:13\r\n\r\nHello Server!";

int main()
{
    WSADATA wsaData;
    // ≥ı ºªØWinsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed\n");
        return 1;
    }

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET)
    {
        printf("socket create fail\n");
        WSACleanup();
        return 1;
    }

    BOOL opt = TRUE;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR)
    {
        printf("bind fail\n");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    if (listen(server_fd, 3) == SOCKET_ERROR)
    {
        printf("listen fail\n");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }
    printf("Server running on http://127.0.0.1:%d\n", PORT);

    while (1)
    {
        SOCKET new_socket = accept(server_fd, NULL, NULL);
        if (new_socket == INVALID_SOCKET) continue;

        char buffer[BUF_SIZE] = { 0 };
        recv(new_socket, buffer, BUF_SIZE, 0);
        printf("Request:\n%s\n", buffer);

        send(new_socket, response, strlen(response), 0);
        closesocket(new_socket);
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}
