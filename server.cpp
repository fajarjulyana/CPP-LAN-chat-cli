#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define MAX_CLIENTS 5

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char* welcome_message = "Selamat datang di chat room!";

    // Membuat socket server
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Gagal membuat socket");
        exit(EXIT_FAILURE);
    }

    // Mengatur opsi socket
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Gagal mengatur opsi socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Melakukan binding socket ke localhost:8080
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Binding gagal");
        exit(EXIT_FAILURE);
    }

    // Mendengarkan koneksi masuk
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen gagal");
        exit(EXIT_FAILURE);
    }

    // Menerima koneksi masuk dan menangani pesan
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Penerimaan koneksi gagal");
            exit(EXIT_FAILURE);
        }

        // Mengirim pesan selamat datang ke client
        send(new_socket, welcome_message, strlen(welcome_message), 0);

        // Menerima pesan dari client dan membalas
        while (1) {
            memset(buffer, 0, sizeof(buffer));
            valread = read(new_socket, buffer, 1024);
            printf("Client: %s\n", buffer);

            if (strcmp(buffer, "bye") == 0) {
                const char* goodbye_message = "Terima kasih telah bergabung. Sampai jumpa!";
                send(new_socket, goodbye_message, strlen(goodbye_message), 0);
                break;
            }

            memset(buffer, 0, sizeof(buffer));
            std::cout << "Server: ";
            std::cin.getline(buffer, sizeof(buffer));
            send(new_socket, buffer, strlen(buffer), 0);
        }

        close(new_socket);
    }

    return 0;
}
