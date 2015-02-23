#include <iostream>
#include <thread>
#include <atomic>
#include "opencv2/opencv.hpp"
#include <wiringPi.h>

using namespace std;
using namespace cv;

#define REFRACTORY_DURATION 5000

long i=0;
atomic<bool> done(true);

void write_out( vector<Mat> collect )
{
	done = false;
	cout<<"new thread"<<endl;
	for( auto f: collect )
		imwrite( "Parikshit/"+std::to_string(i++)+".jpg", f );
	done = true;
	cout<<"del thread"<<endl;

}
int main()
{

	system("sudo rm -r Parikshit");
	system("mkdir Parikshit");
	vector<thread> t;
	if( wiringPiSetup() == -1 )
	{
		cout<<"wiring failed!\n";
		return -1;
	}
	pinMode(11, INPUT);
	digitalWrite(11, LOW);
	VideoCapture cap(0);
	if( !cap.isOpened() )
	{	cout<<"cam not ready"<<endl; return 0;	}

	delay(REFRACTORY_DURATION);

	vector<Mat> collection;
	Mat frame;
	cout<<"capture running.. "<<endl;
	while( digitalRead(11) == LOW )
	{
		cap >> frame;
		//cvtColor( frame, frame, COLOR_BGR2GRAY );
		delay(25);
		collection.push_back( frame.clone() );
		if( collection.size() == 30 )
		{
			cout<<"can I launch another thread?"<<endl;
			if(!done)
			{
				cout<<"waiting.."<<endl;
				t.back().join();
			}
			t.push_back( thread( write_out, collection ) );
			collection.clear();

		}
		cout<<"current buffer: "<<collection.size()<<endl;
	}
	cout<<"done capturing "<<collection.size()<<" images."<<endl;
	cap.release();
	t.back().join();

	for( auto f: collection )
		imwrite( "Parikshit/"+std::to_string(i++)+".jpg", f );

	cout<<"done writing.\n";
	system("sudo shutdown -h now");
	return 1;
}
