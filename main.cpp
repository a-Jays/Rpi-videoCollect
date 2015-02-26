#include <iostream>
#include <thread>
#include <atomic>

#include "frames.cpp"
#include "gps_logger.cpp"

using namespace std;

int main()
{
  thread gps( gps_thread_entry );
  frame_collect();					// continue in the same thread.
  gps.join();						// wait for gps to stop.
  cout<<"Threads joined. Exit."<<endl;
  system("sudo shutdown -h now");
  return 1;
}
