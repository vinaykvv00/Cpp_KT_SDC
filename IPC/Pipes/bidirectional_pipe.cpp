#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

int main() {
    std::cout << " BIDIRECTIONAL PIPE (2 pipes) " << std::endl;

    int pipe1[2];  
    int pipe2[2];  
    
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        std::cerr << "Pipe creation failed!" << std::endl;
        return 1;
    }
    
    std::cout << "  Two pipes created" << std::endl;
    std::cout << "  Pipe1 (Parent→Child): fd[0]=" << pipe1[0] << " fd[1]=" << pipe1[1] << std::endl;
    std::cout << "  Pipe2 (Child→Parent): fd[0]=" << pipe2[0] << " fd[1]=" << pipe2[1] << std::endl;
    
    pid_t pid = fork();
    
    if (pid == 0) {

        std::cout << "\n[CHILD] Started (PID: " << getpid() << ")" << std::endl;
        
        close(pipe1[1]);  
        close(pipe2[0]);  
        
        char buffer[100];
        int bytes = read(pipe1[0], buffer, sizeof(buffer));
        buffer[bytes] = '\0';
        std::cout << "[CHILD] Received from parent: \"" << buffer << "\"" << std::endl;
        
        const char* reply = "Hello Parent, message received!";
        std::cout << "[CHILD] Sending reply: \"" << reply << "\"" << std::endl;
        write(pipe2[1], reply, strlen(reply));

        close(pipe1[0]);
        close(pipe2[1]);
        
        std::cout << "[CHILD] Done, exiting" << std::endl;
        return 0;
    }
    else {

        std::cout << "\n[PARENT] Started (PID: " << getpid() << ")" << std::endl;

        close(pipe1[0]); 
        close(pipe2[1]);  
        
        const char* message = "Hello Child, this is Parent!";
        std::cout << "[PARENT] Sending to child: \"" << message << "\"" << std::endl;
        write(pipe1[1], message, strlen(message));
        
        char buffer[100];
        int bytes = read(pipe2[0], buffer, sizeof(buffer));
        buffer[bytes] = '\0';
        std::cout << "[PARENT] Received from child: \"" << buffer << "\"" << std::endl;
        
        close(pipe1[1]);
        close(pipe2[0]);
        
        wait(nullptr);
        std::cout << "[PARENT] Done, exiting" << std::endl;
    }
    
    return 0;
}