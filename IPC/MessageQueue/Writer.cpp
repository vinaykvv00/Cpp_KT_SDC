#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#define MAX 10

struct msg_buffer {
    long msg_type;
    char msg_text[MAX];
} message;

int main()
{
    key_t key;
    int msgid;

    key = ftok("progfile", 65);

    msgid = msgget(key, 0666 | IPC_CREAT);
    message.msg_type = 1;

    std::cout << "Write Data : ";
    std::cin.getline(message.msg_text, MAX);

    msgsnd(msgid, &message, sizeof(message), 0);

    std::cout << "Data send is : " << message.msg_text << std::endl;

    return 0;
}