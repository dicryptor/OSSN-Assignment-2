#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>

#define MAX_MSG 255
#define LINE_ARRAY_SIZE (MAX_MSG+1)

using namespace std;

struct destinations {
  string name;
  int adultPrice;
  int childPrice;


};

int main()
{
  int listenSocket, connectSocket, i;
  unsigned short int listenPort;
  socklen_t clientAddressLength;
  struct sockaddr_in clientAddress, serverAddress;
  char line[LINE_ARRAY_SIZE];
  char *p;
  string wmsg = 
  "Welcome to Tour Booking Server. We await to serve you\n"
  "Type [Destination] [Quantity of adult tickets]:  eg. HongKong 2\n";
  const char * welcomeMsg = wmsg.c_str ();
  int wmsglen = strlen(welcomeMsg);

  string mydest;
  int adultQty;
  int childQty;
  int adultTtl;
  int childTtl;
  int total;

  vector<destinations> dest(6);
  dest[0].name = "Malaysia";
  dest[0].adultPrice = 100;
  dest[0].childPrice = 60;

  dest[1].name = "Bali";
  dest[1].adultPrice = 400;
  dest[1].childPrice = 220;

  dest[2].name = "Perth";
  dest[2].adultPrice = 1100;
  dest[2].childPrice = 850;

  dest[3].name = "HongKong";
  dest[3].adultPrice = 800;
  dest[3].childPrice = 650;

  dest[4].name = "Switzerland";
  dest[4].adultPrice = 2800;
  dest[4].childPrice = 2200;

  dest[5].name = "Japan";
  dest[5].adultPrice = 2500;
  dest[5].childPrice = 2000;

  // cout << welcomeMsg << wmsglen;
  cout << "Enter port number to listen on (between 1500 and 65000): ";
  cin >> listenPort;

  // Create socket for listening for client connection
  // requests.
  listenSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (listenSocket < 0) {
    cerr << "cannot create listen socket";
    exit(1);
  }
  
  // Bind listen socket to listen port. First set various
  // fields in the serverAddress structure, then call
  // bind().

  // htonl() and htons() convert long integers and short
  // integers (respectively) from host byte order (on x86
  // this is Least Significant Byte first) to network byte
  // order (Most Significant Byte first).
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddress.sin_port = htons(listenPort);
  
  if (bind(listenSocket,
           (struct sockaddr *) &serverAddress,
           sizeof(serverAddress)) < 0) {
    cerr << "cannot bind socket";
    exit(1);
  }

  // Wait for connections from clients. This is a
  // non-blocking call; i.e., it registers this program with
  // the system as expecting connections on this socket, and
  // then this thread of execution continues on.
  listen(listenSocket, 5);
  
  while (1) {
    cout << "Waiting for TCP connection on port " << listenPort << " ...\n";

    // Accept a connection with a client that is requesting
    // one. The accept() call is a blocking call; i.e., this
    // thread of execution stops until a connection comes
    // in. connectSocket is a new socket that the system
    // provides, separate from listenSocket. We *could*
    // accept more connections on listenSocket, before
    // connectSocket is closed, but this program doesn't do
    // that.
    clientAddressLength = sizeof(clientAddress);
    connectSocket = accept(listenSocket,
                           (struct sockaddr *) &clientAddress,
                           &clientAddressLength);
    if (connectSocket < 0) {
      cerr << "cannot accept connection ";
      exit(1);
    }
    
    // Show the IP address of the client.
    // inet_ntoa() converts an IP address from binary form to the
    // standard "numbers and dots" notation.
    cout << "  connected to " << inet_ntoa(clientAddress.sin_addr);

    // Show the client's port number.
    // ntohs() converts a short int from network byte order (which is
    // Most Significant Byte first) to host byte order (which on x86,
    // for example, is Least Significant Byte first).
    cout << ":" << ntohs(clientAddress.sin_port) << "\n";

    if (send(connectSocket, welcomeMsg, wmsglen + 1, 0) < 0)
      cerr << "Error: cannot send welcome message";

    // Read lines from socket, using recv(), storing them in the line
    // array.  If no messages are currently available, recv() blocks
    // until one arrives.
    // First set line to all zeroes, so we'll know where the end of
    // the string is.
    memset(line, 0x0, LINE_ARRAY_SIZE);
    while (recv(connectSocket, line, MAX_MSG, 0) > 0) {
      cout << "  --  " << line << "\n";

      istringstream stream (line);
      stream >> mydest >> adultQty >> childQty;
      // cout << mydest << endl;
      // cout << adultQty << endl;
      // cout << childQty << endl;

      for(int count = 0; count < dest.size(); count++)
      {
        if (dest[count].name == mydest) {
          cout << "Destination name: " << dest[count].name << endl;
          cout << "Adult Price: " << dest[count].adultPrice * adultQty << endl;
          cout << "Child Price: " << dest[count].childPrice * childQty << endl;
          adultTtl = dest[count].adultPrice * adultQty;
          childTtl = dest[count].childPrice * childQty;
          if (childTtl > 9999)
            childTtl = 0;
          total = adultTtl + childTtl;

          ostringstream oss;
          oss << total;
          strcpy(line, "Your trip to ");
          strcat(line, mydest.c_str());
          strcat(line, " will cost you ");
          strcat(line, oss.str().c_str());

          break;
        }
        else
        {
          strcpy(line, "We currently do not serve that destination. Thank you and try again.");
        }
      }  

      // Send converted line back to client.
      if (send(connectSocket, line, strlen(line) + 1, 0) < 0)
        cerr << "Error: cannot send modified data";

      memset(line, 0x0, LINE_ARRAY_SIZE);  // set line to all zeroes
    }
  }
}
