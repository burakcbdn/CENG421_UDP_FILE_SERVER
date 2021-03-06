#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVERPORT 8888
#define MAXBUF 1024
#define MAX_USER_COUNT 10

int checkPassword(char *username, char *password);

int main() {
    int socket1, socket2;
    int addrlen;
    struct sockaddr_in xferServer, xferClient;
    int returnStatus;
    /* create a socket */
    socket1 = socket(AF_INET, SOCK_STREAM, 0);
    if (socket1 == -1) {
        fprintf(stderr, "Could not create socket!\n");
        exit(1);
    }
    /* bind to a socket, use INADDR_ANY for all local addresses */
    xferServer.sin_family = AF_INET;
    xferServer.sin_addr.s_addr = INADDR_ANY;
    xferServer.sin_port = htons(SERVERPORT);
    returnStatus = bind(socket1,
                        (struct sockaddr *)&xferServer,
                        sizeof(xferServer));

    if (returnStatus == -1) {
        fprintf(stderr, "Could not bind to socket!\n");
        exit(1);
    }

    returnStatus = listen(socket1, 5);
    if (returnStatus == -1) {
        fprintf(stderr, "Could not listen on socket!\n");
        exit(1);
    }

    for (;;) {
        int fd;
        int i, readCounter, writeCounter;
        char *bufptr;
        char buf[MAXBUF];
        char filename[MAXBUF];

        /* wait for an incoming connection */
        addrlen = sizeof(xferClient);

        /* use accept() to handle incoming connection requests */
        /* and free up the original socket for other requests */
        socket2 = accept(socket1, (struct sockaddr *)&xferClient, &addrlen);

        if (socket2 == -1) {
            fprintf(stderr, "Could not accept connection!\n");
            exit(1);
        }

        /* get the filename from the client over the socket */
        i = 0;
        if ((readCounter = read(socket2, filename + i, MAXBUF)) > 0) {
            i += readCounter;
        }
        if (readCounter == -1) {
            fprintf(stderr, "Could not read filename from socket!\n");
            close(socket2);
            continue;
        }
        filename[i + 1] = '\0';

        char requestedPath[64] = "";

        char *requestData = strtok(filename, ":");
        printf("Reading file %s\n", requestData);

        char requestedFile[32];
        strcpy(requestedFile, requestData);

        requestData = strtok(NULL, ":");
        char requestedUser[32];
        strcpy(requestedUser, requestData);

        requestData = strtok(NULL, ":");
        char userPass[32];
        strcpy(userPass, requestData);

        strcat(requestedPath, requestedUser);
        strcat(requestedPath, "/");
        strcat(requestedPath, requestedFile);

        printf("Path: %s\n", requestedPath);

        int passRess = checkPassword(requestedUser, userPass);

        if (passRess != 1) {
            write(socket2, "Password wrong\n", 14);
             close(socket2);
            continue;
        }

        /* open the file for reading */
        fd = open(requestedPath, O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "Could not open file for reading!\n");
            close(socket2);
            continue;
        }

        /* reset the read counter */
        readCounter = 0;

        /* read the file, and send it to the client in chunks of size MAXBUF */
        while ((readCounter = read(fd, buf, MAXBUF)) > 0) {
            writeCounter = 0;
            bufptr = buf;
            while (writeCounter < readCounter) {
                readCounter -= writeCounter;
                bufptr += writeCounter;
                writeCounter = write(socket2, bufptr, readCounter);
                if (writeCounter == -1) {
                    fprintf(stderr, "Could not write file to client!\n");
                    close(socket2);
                    continue;
                }
            }
        }
        close(fd);
        close(socket2);
    }
    close(socket1);
    return 0;
}

int checkPassword(char *username, char *password) {
    char userData[MAX_USER_COUNT][32] = {
        "burakcbdn-123123",
        "buraktest-123456"};

    for (int i = 0; i < MAX_USER_COUNT; i++) {
        char *cont;

        cont = strstr(userData[i], username);

        if (cont) {

            char *userDetail = strtok(userData[i], "-");

            if (strcmp(userDetail, username) != 0) {
                continue;
            }

            userDetail = strtok(NULL, "-");

            if (strcmp(userDetail, password) == 0) {

                return 1;
            } else {

                return 0;
            }

        } else {
            return 0;
        }
    }

    return 0;
}