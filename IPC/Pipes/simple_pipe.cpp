#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

int main() {
    std::cout << " Normal PIPE DEMO " << std::endl;
    
    int pipefd[2];
    
    if (pipe(pipefd) == -1) {
        std::cerr << "Pipe creation failed!" << std::endl;
        return 1;
    }
    
    std::cout << "  Pipe created successfully" << std::endl;
    std::cout << "  Read end:  fd[0] = " << pipefd[0] << std::endl;
    std::cout << "  Write end: fd[1] = " << pipefd[1] << std::endl;
    
    pid_t pid = fork();
    
    if (pid == -1) {
        std::cerr << "Fork failed!" << std::endl;
        return 1;
    }
    
    if (pid == 0) {
        std::cout << "\n[CHILD] I am the reader (PID: " << getpid() << ")" << std::endl;
        
        close(pipefd[1]);
        std::cout << "[CHILD] Closed write end of pipe" << std::endl;
        
        char buffer[100];
        std::cout << "[CHILD] Waiting to read from pipe..." << std::endl;
        
        int bytes_read = read(pipefd[0], buffer, sizeof(buffer));
        
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';  
            std::cout << "[CHILD] Read " << bytes_read << " bytes: \"" << buffer << "\"" << std::endl;
        }
        
        close(pipefd[0]);
        std::cout << "[CHILD] Closed read end, exiting" << std::endl;
        
        return 0;  
    }
    
    else {
        std::cout << "\n[PARENT] I am the writer (PID: " << getpid() << ")" << std::endl;
        std::cout << "[PARENT] Child PID: " << pid << std::endl;
        
        close(pipefd[0]);
        std::cout << "[PARENT] Closed read end of pipe" << std::endl;
        
        const char* message = "Hello from Parent!";
        std::cout << "[PARENT] Writing to pipe: \"" << message << "\"" << std::endl;
        
        write(pipefd[1], message, strlen(message));
        
        close(pipefd[1]);
        std::cout << "[PARENT] Closed write end" << std::endl;

        std::cout << "[PARENT] Waiting for child to finish..." << std::endl;
        wait(nullptr);
        std::cout << "[PARENT] Child finished, exiting" << std::endl;
    }
    
    return 0;
}