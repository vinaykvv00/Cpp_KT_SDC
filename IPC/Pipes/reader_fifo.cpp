#include <iostream>
#include <fcntl.h>      
#include <unistd.h>   

int main() {
    std::cout << " NAMED PIPE READER " << std::endl;
    
    const char* fifo_path = "/tmp/my_pipe";
    
    std::cout << "Looking for named pipe: " << fifo_path << std::endl;
    std::cout << "Opening pipe for reading..." << std::endl;
    
    int fd = open(fifo_path, O_RDONLY); 
    
    if (fd == -1) {
        std::cerr << "Failed to open pipe!" << std::endl;
        std::cerr << "Make sure writer created the pipe first!" << std::endl;
        return 1;
    }
    
    std::cout << " Pipe opened successfully!" << std::endl;

    char buffer[100];
    int message_count = 0;
    
    std::cout << "\n Waiting for messages...\n" << std::endl;
    while (true) {
        int bytes_read = read(fd, buffer, sizeof(buffer));
        
        if (bytes_read > 0) {
            message_count++;
            std::cout << "[READER] Message " << message_count 
                      << ": \"" << buffer << "\"" << std::endl;
        } 
        else if (bytes_read == 0) {
            std::cout << "\n[READER] Writer closed the pipe (no more data)" << std::endl;
            break;
        }
        else {
            std::cerr << "Read error!" << std::endl;
            break;
        }
    }
    
    std::cout << "[READER] Closing pipe" << std::endl;
    close(fd);
    
    std::cout << " Done! Received " << message_count << " messages total." << std::endl;
    
    return 0;
}

/*
SIMPLE EXPLANATION:
==================

1. open(path, O_RDONLY)
   - Opens the pipe for READING only
   - BLOCKS (waits) until someone opens it for writing
   - Like waiting for mail to arrive

2. read(fd, buffer, size)
   - Reads data from the pipe
   - Returns number of bytes read
   - Returns 0 when writer closes (no more data)
   - BLOCKS if pipe is empty (waits for data)

3. Loop until bytes_read == 0
   - Keeps reading messages
   - Stops when writer closes

4. close(fd)
   - Closes our end of the pipe

KEY POINTS:
==========
- Reader and Writer are COMPLETELY SEPARATE programs
- They communicate through "/tmp/my_pipe" file
- No parent-child relationship needed!
- Writer must create pipe first (mkfifo)
*/
