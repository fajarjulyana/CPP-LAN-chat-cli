#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // Membuat soket client
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nKesalahan saat membuat soket \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Mengonversi alamat IP dari format string ke format biner
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nAlamat tidak valid/Tidak didukung \n");
        return -1;
    }

    // Terhubung ke server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nKoneksi gagal \n");
        return -1;
    }

    // Menerima pesan selamat datang dari server
    valread = read(sock, buffer, 1024);
    printf("%s\n", buffer);

    // Menerima dan mengirim pesan antara client dan server
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        std::cout << "Client: ";
        std::cin.getline(buffer, sizeof(buffer));

        send(sock, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "bye") == 0) {
            memset(buffer, 0, sizeof(buffer));
            valread = read(sock, buffer, 1024);
            printf("%s\n", buffer);
            break;
        }

        memset(buffer, 0, sizeof(buffer));
        valread = read(sock, buffer, 1024);
        printf("Server: %s\n", buffer);
    }

    return 0;
}
