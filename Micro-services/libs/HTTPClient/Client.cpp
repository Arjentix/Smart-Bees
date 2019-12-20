#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;

int main()
{
    
    int client;
    int portNum = 1500; 
    int bufsize = 1024;
    char buffer[bufsize];
    const string ip("127.0.0.1");

    HTTPClient::connect(ip, portNum);

    client = socket(AF_INET, SOCK_STREAM, 0);

    /* ---------- ESTABLISHING SOCKET CONNECTION ----------*/
    /* --------------- socket() function ------------------*/

    if (client < 0) 
    {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }

    cout << "\n=> Socket client has been created..." << endl;
    
    sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);
    inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr);


    /* ---------- CONNECTING THE SOCKET ---------- */
    /* ---------------- connect() ---------------- */

    int res = connect(client,(struct sockaddr *)
            &server_addr, sizeof(server_addr));
    
    if(res == 0){

        cout << "=> Connection to the server "
        << inet_ntoa(server_addr.sin_addr) 
        << "port number: " << portNum << endl;
    }

    cout << "=> Awaiting confirmation from the server..." 
        << endl; //line 40
        
    recv(client, buffer, bufsize, 0);
    cout << "=> Connection confirmed, you are good to go...";

    cout << "\n\n=> Enter # to end the connection\n" << endl;

    // Once it reaches here, the client can send a message first.

        cout << "Client: ";

            cin >> buffer;
            send(client, buffer, bufsize, 0);

        cout << "Server: ";
            recv(client, buffer, bufsize, 0);
            cout << buffer << " ";
        cout << endl;


    /* ---------------- CLOSE CALL ------------- */
    /* ----------------- close() --------------- */

    /* 
        Once the server presses # to end the connection,
        the loop will break and it will close the server 
        socket connection and the client connection.
    */

    cout << "\n=> Connection terminated.\nGoodbye...\n";

    close(client);
    return 0;
}