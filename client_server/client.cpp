/*
Zehan Sunesara
Assignment 2 - client
April 9, 2018
PSID - 1237998
*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
using namespace std;

int main() {
	char HostName[1024];
	int portNumber;
	char major[1024];
	struct hostent *server; // represents the host database
    string emptyString;
	reenterHostName:

	cout << "\nEnter server host name: "; //input host name
	cin >> HostName;

	server = gethostbyname(HostName); //resolves what the user input for host name

	if (server == NULL) {
		cout << "Incorrect host name";
		goto reenterHostName;
	}
	cout << "Enter server port number: "; // input port number
	cin >> portNumber;
    while(1){ //infinite loop
        // input college major with space
        cin.ignore();
        cout << "Enter a college major: ";
        cin.getline(major, 1024);
        string realMajor = string(major);

        int client;
        int portNum = portNumber;
        int x;

        struct sockaddr_in serverAddress;

        bzero((char *) &serverAddress, sizeof(serverAddress)); //copies zero's into strings
        serverAddress.sin_family = AF_INET;
        bcopy((char *) server->h_addr, (char *) &serverAddress.sin_addr.s_addr, server->h_length); //copies bytes from source to destination
        serverAddress.sin_port = htons(portNum); //get server address

        client = socket(AF_INET, SOCK_STREAM, 0); // get socket

        if (client < 0) { // if socket is less than zero
            cout << "\nInvalid socket connection" << endl;
            exit(1);
        }

        if (connect(client, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) { // if port number cannot be connected
            cout << "\nInvalid port number" << endl;
            break;
        }

        x = write(client, major, sizeof(major));

        if (realMajor != "") { // if there is not an empty string, write the early and mid salary for the major
            x = read(client, major, sizeof(major)); // read the server file
            emptyString = major;
            if (emptyString == "") { // if the major is not in the text file
                cout << "That major is not in the table" << endl;
				continue;
            }
            char *splitSalary[512];
            splitSalary[0] = strtok(major, "\t"); // Splits tab between words in char major
            splitSalary[1] = strtok (NULL, " ,.-"); // gets the other half of the word
            cout << "The average early career pay for a " << realMajor << " major is " << "$" << splitSalary[0] << endl; //print early career salary for the major
            cout << "The corresponding mid-career pay for a " << realMajor << " major is " << "$" << splitSalary[1]; //print mid career salary for the major
        }

        else if (realMajor == "") { // if there is an empty string, terminate client
            cout << "Client Terminated" << endl;
            break;
            close(client);
        }
    }
	return 0;
}
