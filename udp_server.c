/***************Server program**************************/

/* 
   NAME: Matthew Sleeth
   CS 4310 – 251 - Computer Networks

   udp_server.c is on eros.cs.txstate.edu
   open a window on eros.

   compile and run the server program first:
   gcc -O3 -g -std=c99 udp_server.c -o udp_server

   $./udp_server
*/

#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

void addUser(char *fname, char *lname) {
 
    FILE* f = fopen("students.txt", "a");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    for (int i = 0; i < 11; i++) {
        if (fname[i] == '\n') {
            fname[i] = fname[i + 1];
        }
        if (lname[i] == '\n') {
            lname[i] = lname[i + 1];
        }
    }

    int id = rand() % (900000) + 100000;  // generates random 6 digit id number
    fprintf(f, "%d ", id);  // write student id number to file

    fprintf(f, "%s ", fname);   // write student first name to file

    fprintf(f, "%s ", lname);   // write student last name to file

    int score = rand() % (41) + 60;  // generates random score between 60 and 99
    fprintf(f, "%d\n", score);  // write student score to file

    fclose(f);
}

char * displayID(int id) {
    
    char msg[MAX];
    char sid[7];
    char first6[7];
    char returnInfo[MAX];
    char studentInfo[MAX];
    int count = 0;
    bool keepReading = true;

    FILE* f = fopen("students.txt", "r");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    sprintf(sid, "%d", id);
    
    // searches through file to find and display user with matching id
    do {
        fgets(studentInfo, 50, f);

        for (int i = 0; i < 6; i++) {
            first6[i] = studentInfo[i];
        }


        if (feof(f)) {
            fclose(f);

            FILE* f = fopen("students.txt", "r");
            if (f == NULL)
            {
                printf("Error opening file!\n");
                exit(1);
            }
            count++;
        }
        else if (strcmp(first6, sid) == 0) {
            keepReading = false;
            strncpy(returnInfo, studentInfo, MAX);
        }

        if (count == 2) {
            strncpy(returnInfo, "Student not found...\n", MAX);
            keepReading = false;
        }

        memset(studentInfo, 0, sizeof(studentInfo));
    } while (keepReading);

    fclose(f);

    return returnInfo;
}

char * displayScore(int score) {

    char msg[MAX];
    char myScore[4];
    int j = 0, studentScore = 0, space = 0;
    char returnCopy[MAX];
    char studentInfo[50];
    char studentCopy[50];
    bool keepReading = true;

    FILE* f = fopen("students.txt", "r");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    bzero(returnCopy, MAX);
    strcat(returnCopy, "\n");
    
    // searches through file to check if a students score is greater than the desired score
    do {
        fgets(studentInfo, 50, f);

        for (int i = 0; i < 50; i++) {
            studentCopy[i] = studentInfo[i];
            if (space == 3) {
                myScore[j] = studentInfo[i];
                j++;
            }
            else {
                if (studentInfo[i] == ' ') {
                    space++;
                }
            }
        }

        studentScore = atoi(myScore);

        if (feof(f)) {
            keepReading = false;
        }
        else if (studentScore > score) {
            strcat(returnCopy, studentCopy);
        }

        space = 0;
        j = 0;
        memset(myScore, 0, sizeof(myScore));
        memset(studentInfo, 0, sizeof(studentInfo));
        memset(studentCopy, 0, sizeof(studentCopy));
    } while (keepReading);

    fclose(f);

    printf("%s\n", returnCopy);

    return returnCopy;
}

char * displayAll() {

    char returnCopy[MAX];
    char studentInfo[50];
    bool keepReading = true;

    FILE* f = fopen("students.txt", "r");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    bzero(returnCopy, MAX);
    strcat(returnCopy, "\n");
    
    do {
        fgets(studentInfo, 50, f);

        if (feof(f)) {
            keepReading = false;
        }
        else {
            strcat(returnCopy, studentInfo);
        }

        memset(studentInfo, 0, sizeof(studentInfo));

    } while (keepReading);

    fclose(f);

    return returnCopy;

}

void deleteUser(int id) {

    char msg[MAX];
    int count = 0;
    char sid[7];
    char first6[7];
    char studentInfo[50];
    bool keepReading = true;

    FILE* f = fopen("students.txt", "r");
    FILE* temp = fopen("temp____students.txt", "w");
    if (f == NULL || temp == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    do {
        fgets(studentInfo, 50, f);

        for (int i = 0; i < 6; i++) {
            first6[i] = studentInfo[i];
        }
        sprintf(sid, "%d", id);

        if (feof(f)) {
            keepReading = false;

        }
        else if (strcmp(first6, sid) != 0) {
            fputs(studentInfo, temp);
        }
        else {
            // Do nothing
        }

        memset(studentInfo, 0, sizeof(studentInfo));
        count++;
    } while (keepReading);

    fclose(f);
    fclose(temp);

    remove("students.txt");
    rename("temp____students.txt", "students.txt");
}

// Driver function
int main(int argc, char** argv)
{

    int s, connfd, client_address_size;
    struct sockaddr_in server, client;
    int num;
    char msg[MAX];

    int n, studentID, studentScore;
    char firstName[11];
    char lastName[11];
    char student[MAX];

    /* Create a datagram socket in the internet domain and
     use the UDP protocol. */
    s = socket(AF_INET, SOCK_DGRAM, 0);
    server.sin_family = AF_INET; /* Server is in Internet Domain */
    server.sin_port = htons(PORT); /* port */
    server.sin_addr.s_addr = INADDR_ANY; /* Server's Internet Address */
    bind(s, (struct sockaddr*)&server, sizeof(server));

    // communication starts from here
    client_address_size = sizeof(client);

    printf("Server is running...\n");
    for (;;) {
        // receive an integer from the client
        bzero(msg, MAX);
        recvfrom(s, &msg, sizeof(msg), 0, (struct sockaddr*)&client, &client_address_size);
        printf("integer received: %s\n", msg);
        num = atoi(msg);

        switch (num) {
        case 1:
            // get first name
            strcpy(msg, "Enter the student's First name (10 characters max): ");
            sendto(s, msg, sizeof(msg), 0, (struct sockaddr*)&client, sizeof(client));
            bzero(msg, MAX);
            recvfrom(s, &msg, sizeof(msg), 0, (struct sockaddr*)&client, &client_address_size);
            strncpy(firstName, msg, 11);
            bzero(msg, MAX);

            // get last name
            strcpy(msg, "Enter the student's last name (10 characters max): ");
            sendto(s, msg, sizeof(msg), 0, (struct sockaddr*)&client, sizeof(client));
            bzero(msg, MAX);
            recvfrom(s, &msg, sizeof(msg), 0, (struct sockaddr*)&client, &client_address_size);
            strncpy(lastName, msg, 11);
            bzero(msg, MAX);

            // Add student to data base
            addUser(firstName, lastName);

            // confirm student was added
            strcpy(msg, "Student added... ");
            sendto(s, msg, sizeof(msg), 0, (struct sockaddr*)&client, sizeof(client));
            bzero(msg, MAX);

            break;

        case 2:
            // get id number
            bzero(student, MAX);
            strcpy(msg, "Enter the student's ID whose profile you want to see(6 characters max): ");
            sendto(s, msg, sizeof(msg), 0, (struct sockaddr*)&client, sizeof(client));
            bzero(msg, MAX);
            recvfrom(s, &msg, sizeof(msg), 0, (struct sockaddr*)&client, &client_address_size);
            studentID = atoi(msg);
            bzero(msg, MAX);

            strncpy(student, displayID(studentID), MAX);
            studentID = 0;

            sendto(s, student, sizeof(msg), 0, (struct sockaddr*)&client, sizeof(client));

            break;

        case 3:
            // get score for comparison
            bzero(student, MAX);
            strcpy(msg, "Enter a benchmark score (0-100) to see who scored higher : ");
            sendto(s, msg, sizeof(msg), 0, (struct sockaddr*)&client, sizeof(client));
            bzero(msg, MAX);
            recvfrom(s, &msg, sizeof(msg), 0, (struct sockaddr*)&client, &client_address_size);
            studentScore = atoi(msg);
            bzero(msg, MAX);

            strncpy(student, displayScore(studentScore), MAX);

            sendto(s, student, sizeof(student), 0, (struct sockaddr*)&client, sizeof(client));
            studentScore = 0;

            break;

        case 4:
            // displays all students
            strncpy(student, displayAll(), MAX);
            sendto(s, student, sizeof(student), 0, (struct sockaddr*)&client, sizeof(client));
            bzero(student, MAX);

            break;

        case 5:
            // deletes a user
            bzero(msg, MAX);
            strcpy(msg, "Enter the student's ID whose profile you want to delete(6 characters max): ");
            sendto(s, msg, sizeof(msg), 0, (struct sockaddr*)&client, sizeof(client));
            bzero(msg, MAX);
            recvfrom(s, &msg, sizeof(msg), 0, (struct sockaddr*)&client, &client_address_size);
            studentID = atoi(msg);
            bzero(msg, MAX);

            deleteUser(studentID);

            strcpy(msg, "The student was deleted...\n ");
            sendto(s, msg, sizeof(msg), 0, (struct sockaddr*)&client, sizeof(client));

            break;

        case 6:
            // exits program
            strcpy(msg, "Exiting...\n ");
            sendto(s, msg, sizeof(msg), 0, (struct sockaddr*)&client, sizeof(client));
            return 0;
            break;

        case 0:
            // send menu to client
            strcpy(msg, "Select a command to run.\n0. Menu \n1. ADD \n2. Display ID \n3. Display Score \n4. Display All \n5. Delete ID \n6. Exit \n ");
            sendto(s, msg, sizeof(msg), 0, (struct sockaddr*)&client, sizeof(client));
            break;

        default:
            strcpy(msg, "Error: Not an option \n ");
            sendto(s, msg, sizeof(msg), 0, (struct sockaddr*)&client, sizeof(client));
            break;

        }
    }
    close(s);

    return 0;

}
