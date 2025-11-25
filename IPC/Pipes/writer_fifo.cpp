#include <iostream>
#include <fcntl.h>     
#include <sys/stat.h>  
#include <unistd.h>    
#include <cstring>

int main() {
    std::cout << " NAMED PIPE WRITER " << std::endl;
    
    const char* fifo_path = "/tmp/my_pipe";
    
    std::cout << "Creating named pipe: " << fifo_path << std::endl;
    
    if (mkfifo(fifo_path, 0666) == -1) {
        std::cout << "Note: Named pipe already exists " << std::endl;
    } else {
        std::cout << " Named pipe created successfully" << std::endl;
    }

    std::cout << "\nOpening pipe for writing..." << std::endl;
    std::cout << " Waiting for reader to connect..." << std::endl;
    
    int fd = open(fifo_path, O_WRONLY);  
    
    if (fd == -1) {
        std::cerr << "Failed to open pipe!" << std::endl;
        return 1;
    }
    
    std::cout << " Reader connected Pipe is open." << std::endl;
    
    const char* messages[] = {
        "Hello from Writer!",
        "This is message 2",
        "Final message - Goodbye!"
    };
    
    for (int i = 0; i < 3; i++) {
        std::cout << "\n[WRITER] Sending: \"" << messages[i] << "\"" << std::endl;
        write(fd, messages[i], strlen(messages[i]) + 1);  
        sleep(1); 
    }
    
    std::cout << "\n[WRITER] Closing pipe" << std::endl;
    close(fd);
    std::cout << " Done by writer to write the message" << std::endl;
    
    return 0;
}

/*
SIMPLE EXPLANATION:
==================

1. mkfifo("/tmp/my_pipe", 0666)
   - Creates a SPECIAL FILE (not regular file)
   - Like creating a mailbox with address "/tmp/my_pipe"
   - 0666 = everyone can read/write

2. open(path, O_WRONLY)
   - Opens the pipe for WRITING only
   - BLOCKS (waits) until someone opens it for reading
   - Like waiting for mailman to arrive

3. write(fd, message, size)
   - Sends data through the pipe
   - Data goes into kernel buffer
   - Reader will receive it

4. close(fd)
   - Closes our end of the pipe
   - Like closing the mailbox door

KEY POINT:
=========
The named pipe file "/tmp/my_pipe" stays in filesystem!
You can see it with: ls -l /tmp/my_pipe
*/
