#include "request_handler.h"
#include <iostream>
#include <sys/socket.h>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include <unistd.h>


void RequestHandler::handleClient(int clientFD){

    /*
    Read raw bytes from socket, recv returns number of bytes read
    */
    char buffer[4096];

    memset(buffer, 0, sizeof(buffer));  

    int bytesRead = recv(clientFD, buffer, sizeof(buffer), 0);

    if (bytesRead <= 0){
        std::cerr << "Error handling request with clientFD " << clientFD << std::endl;
        return;
    }
    
    /*
    Parse HTTP request -> extracts first line only contains method, path and version
    */
    std::string request(buffer);
    std::string firstLine = request.substr(0, request.find("\r\n"));

    std::istringstream stream(firstLine);
    std::string method, path, version;

    stream >> method >> path >> version;

    /*
    Find and read the requested file -> map path to static folder on disk and read entire file contents into memory 
    */
    if (path == "/") path = "/index.html";
    std::string filePath = "static" + path;
    std::ifstream inFile;
    inFile.open(filePath);

    /*
    Handle HTTP 404 response 
    File not found -> send 404 statuc code with error body and close connection with this client 
    */
    if (!inFile.is_open()){
        std::string body = "<html><body><h1>404 Not Found</h1></body></html>";

        std::string response =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: " + std::to_string(body.size()) + "\r\n"
            "\r\n" +
            body;

        send(clientFD, response.c_str(), response.size(), 0);
        close(clientFD);
        return;
    }

    std::string fileContents((std::istreambuf_iterator<char>(inFile)),  std::istreambuf_iterator<char>());

    inFile.close();

    /*
    Serve HTTP response -> send metadata first(status, content type) and then sends file contents as bosy then close conenction
    */
    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: " + std::to_string(fileContents.size()) + "\r\n"
        "Content-Type: text/html\r\n"
        "\r\n" +
        fileContents;

   

    send(clientFD, response.c_str(), response.size(), 0);

    close(clientFD);
}