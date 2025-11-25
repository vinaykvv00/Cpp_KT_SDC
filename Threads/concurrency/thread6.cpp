#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

std::chrono::milliseconds interval(1);
static std::atomic<int> count = 1;

void odd()
{
   while( count < 11 ) 
  {
    if(count %2 == 1)
    {
       std::cout<<count<<std::endl;
       count++;
    }
    std::this_thread::sleep_for(interval);
  }
}
  
void even()
{
  while(count < 11)
 {
    if(count %2 == 0){
        std::cout<<count<<std::endl;
        count++;
    }
     std::this_thread::sleep_for(interval);
 }
}

int main()
{
  std::thread thread1(odd);
  std::thread thread2(even);

  thread1.join();
  thread2.join();

  return 0;
}