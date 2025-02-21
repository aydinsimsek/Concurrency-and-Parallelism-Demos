## Method 
When a new client connects to the server's socket, the server creates a new child process using the `fork()` system call.  
Each child process then handles the communication with the client by creating a new socket for the client connection.  
This allows the server to handle multiple clients concurrently, with each client being managed by its own child process.  

## Screenshot of an Example Usage  
![screenshot](https://github.com/user-attachments/assets/4e93d730-9188-4e5e-8b2a-48a4b614ca8b)
