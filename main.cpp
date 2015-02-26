#include <iostream>
#include <thread>
#include <atomic>

#include "frames.cpp"
#include "gps_logger.cpp"

using namespace std;

int main()
{
  thread gps( gps_thread_entry );
  while( frames_collect() != 1 );					// continue in the same thread. Loop for in case something's wrong there.
  gps.join();						// wait for gps to stop.
  cout<<"Threads joined. Exit."<<endl;
  system("sudo shutdown -h now");
  return 1;
}
