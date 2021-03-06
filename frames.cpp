
#define REFRACTORY_DURATION 5000
#define WRITE_BUFFER_SIZE 30

long i=0;						// Lord in heaven, global variable!
std::atomic<bool> buffer_empty(true);

using namespace std;
using namespace cv;

void write_out( vector<Mat> collect )
{
	buffer_empty = false;
	for( auto f: collect )
		imwrite( "Parikshit/"+std::to_string(i++)+".jpg", f );
	buffer_empty = true;
}
int frames_collect()
{

	vector<thread> t;
	if( wiringPiSetup() == -1 )
	{
		cout<<"wiring failed!\n";
		return -1;
	}
	pinMode(11, INPUT);
	digitalWrite(11, LOW);				// this pin going high (3.3V) terminates code.
	
	VideoCapture cap(0);
	if( !cap.isOpened() )
	{	cout<<"cam not ready"<<endl; return 0;	}

	delay(REFRACTORY_DURATION);

	vector<Mat> collection(WRITE_BUFFER_SIZE);			// pre-allocate for speed.
	Mat frame;
	cout<<"capture running.. "<<endl;
	while( digitalRead(11) == LOW )
	{
		cap >> frame;
		//cvtColor( frame, frame, COLOR_BGR2GRAY );
		delay(25);
		if( !gps_lock )
			continue;
		
		collection.push_back( frame.clone() );
		if( collection.size() == WRITE_BUFFER_SIZE )
		{
			if( !buffer_empty )
			{
				cout<<"waiting for previous thread to finish.."<<endl;
				t.back().join();
			}
			t.push_back( thread( write_out, collection ) );
			collection.clear();
		}
	}
	cout<<"done capturing "<<collection.size()<<" images."<<endl;
	cap.release();
	t.back().join();					// wait for the last thread to finish.

	for( auto f: collection )
		imwrite( "Parikshit/"+std::to_string(i++)+".jpg", f );		// write the remaining stuff.

	cout<<"done writing."<<endl;
	// shutdown has been moved to main.cpp -- just return from here.
	return 1;
}
