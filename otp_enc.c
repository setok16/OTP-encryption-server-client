#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

void error(const char *msg) { perror(msg); exit(1); } // Error function used for reporting issues

int main(int argc, char *argv[]) {
	int socketFD, portNumber, charsWritten, charsRead;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	char buffer[131072];

	if (argc < 4) { fprintf(stderr, "USAGE: %s plaintext key port\n", argv[0]); exit(1); } // Check usage & args

	// Set up the server address struct
	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(argv[3]); // Get the port number, convert to an int from a string
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverHostInfo = gethostbyname("localhost"); // Convert the machine name into a special form of address
	if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(1); }
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address
	
	// Set up the socket (TCP socket)
	socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (socketFD < 0) error("CLIENT: ERROR opening socket");

	// Connect to server
	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {// Connect socket to addy
		error("CLIENT: ERROR connecting");
	}

	// Get input message from user
	/*
	printf("CLIENT: Enter text to send to the server, and then hit enter: ");
	memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer array
	fgets(buffer, sizeof(buffer) - 1, stdin); // Get input from the user, trunc to buffer - 1 chars, leaving \0
	buffer[strcspn(buffer, "\n")] = '\0'; // Remove the trailing \n that fgets adds (replaces \n with \0)
	*/

	// Get files to send
	int file_descriptor;
	char* plaintext = argv[1];
	char* key = argv[2];
	char sendBuffer[sizeof(buffer) * 3]; // Making enough space for the key.
	// Reading in the plaintext
	file_descriptor = open(plaintext, O_RDONLY);
	if (file_descriptor < 0) {
		fprintf(stderr, "Could not open %s\n", plaintext);
		exit(1);
	} 

	memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer array
	read(file_descriptor, buffer, sizeof(buffer)-1);

	// Chicking for invalid characters
	int i;
	for (i = 0; i < strlen(buffer); i++) {
		if (buffer[i] > 90 ||( buffer[i] > 32 && buffer[i] < 65)) {
			fprintf(stderr, "Error: Bad caracter(s) detected in %s\n", plaintext);
			exit(1);
		}
	}

	strcpy(sendBuffer, "e"); // Unique char to identify server that this is otp_enc
	strcat(sendBuffer, buffer);
	sendBuffer[strlen(sendBuffer)-1] = '\0'; // Removing last newline char with null terminator
	strcat(sendBuffer, "@"); // Indicates end of plaintext
	// Reading in the key
	file_descriptor = open(key, O_RDONLY);
	if (file_descriptor < 0) {
		fprintf(stderr, "Could not open %s\n", plaintext);
		exit(1);
	}
	memset(buffer, '\0', sizeof(buffer));
	read(file_descriptor, buffer, sizeof(buffer));
	if (strlen(sendBuffer) > strlen(buffer)) { // Terminating if key is too short
		fprintf(stderr, "Error: key \'%s\' is too short\n", key);
		exit(1);
	}
	// Chicking for invalid characters
	for (i = 0; i < strlen(buffer); i++) {
		if (buffer[i] > 90 ||( buffer[i] > 32 && buffer[i] < 65)) {
			fprintf(stderr, "Error: Bad caracter(s) detected in %s\n", plaintext);
			exit(1);
		}
	}
	strcat(sendBuffer, buffer);
	sendBuffer[strlen(sendBuffer)-1] = '\0'; // Removing last newline char with null terminator
	strcat(sendBuffer, "$"); // Indicates end of transmission
	
	// Send message to server
	charsWritten = send(socketFD, sendBuffer, strlen(sendBuffer), 0); // Write to the server
	if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
	if (charsWritten < strlen(sendBuffer)) printf("CLIENT: WARNING: Not all data written to socket!\n");

	// Get return message from server
	memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer again for reuse
	charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0); // Read data from teh socket, leaving \0 at end
	if (charsRead < 0) error("CLIENT: ERROR reading from socket");
	if (strstr(buffer, "bad") != NULL) { // If server isn't otp_enc_d
		fprintf(stderr, "Error: Attempting to connect to a non-otp_enc_d server.\n");
		exit(1);
	}
	printf("%s\n", buffer);

	close(socketFD); // Close socket
	return 0;
}
