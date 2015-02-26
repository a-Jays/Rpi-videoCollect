#include <iostream>
#include <thread>
#include <atomic>
#include "opencv2/opencv.hpp"
#include <wiringPi.h>
#include <wiringSerial.h>

using namespace std;
using namespace cv;

#include "gps_logger.cpp"
#include "frames.cpp"

int main()
{
  thread gps( gps_thread_entry );
  system("sudo rm -rf Images");
  system("mkdir Images");
  while( frames_collect() != 1 );					// continue in the same thread. Loop for in case something's wrong there.
  gps.join();						// wait for gps to stop.
  cout<<"Threads joined. Exit."<<endl;
  system("sudo shutdown -h now");
  return 1;
}
