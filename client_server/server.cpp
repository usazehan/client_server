/*
Zehan Sunesara
Assignment 2 - server
April 9, 2018
PSID - 1237998
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <map>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;

queue<string> SalaryQueue;
map<string, string> SalaryMap;

int main() {
    // input for file name and port number
	string fileName;
	int portNumber;
	cout << "Enter a file name: ";
	cin >> fileName;
	cout << "Enter server port number: ";
	cin >> portNumber;

	//read the text file and put it in a queue
	string line;
	ifstream file(fileName.c_str());
	if(file.is_open()) {
		while(getline(file, line)) {
			SalaryQueue.push(line);
		}
	}
	// file doesn't exist
	else {
		cout << "Invalid file" << endl;
		return 0;
	}
    //parse the text file in terms of major and salaries
	string major, salaries;
	while (!SalaryQueue.empty()) {
		stringstream input(SalaryQueue.front());
		getline(input, major, '\t');
		getline(input, salaries, '\n');
		input >> major >> salaries ;
		SalaryMap[major] = salaries; //map the key and the value which are major and salary
		SalaryQueue.pop();
	}


	int client, server;
	int getPort = portNumber;
	char buffer[1024];
	int x;
	struct sockaddr_in serverAddress, clientAddress;
	socklen_t size;
    string SendToClient;

	bzero((char *) &serverAddress, sizeof(serverAddress)); //copies zero's into strings
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htons(INADDR_ANY); //this is the host address
	serverAddress.sin_port = htons(getPort); //this is the port number

	client = socket(AF_INET, SOCK_STREAM, 0);

	if (client < 0) { //connection doesn't exist
		cout << "Error in establishing connection with client." << endl;
		exit(1);
	}
    cout << "Created Server Connection"<< endl;
	if (bind(client, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) { //socket is not binded correctly
		cout << "Bind Error" << endl;
		return(-1);
	}
	size = sizeof(clientAddress);
    while(1){
        listen(client, 5); //max # of queued connections
        server = accept(client, (struct sockaddr *) &clientAddress, &size); // accept connection
        x = read(server, buffer, sizeof(buffer)); // read the client request
        SendToClient = buffer;
        if (SendToClient != "") { // if major input is not an empty string
            strncpy(buffer, SalaryMap[SendToClient].c_str(), sizeof(buffer) - 1); //Copies the first (buffer size -1) characters of SalaryMap to buffer.
            buffer[sizeof(buffer) - 1] = 0;
            x = write(server, buffer, sizeof(buffer));// write to client
        }
    }
	close(server);
	close(client);
}
