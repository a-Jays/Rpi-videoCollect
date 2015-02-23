#include <wiringPi.h>
#include <wiringSerial.h>
#include <iostream>

using namespace std;

int main()
{
        int fd;
        if( wiringPiSetup() == -1 )
        {
                cout<<"wiring not done."<<endl;
                return 0;
        }

        fd = serialOpen("/dev/ttyAMA0", 9600);
        if( fd < 0 )
        {
                cout<<"can't open port."<<endl;
                return 0;
        }

        // UTTERLY BAD (ALTHOUGH FUNCTIONAL) CODE FOLLOWS..
        while( true )
        {
                if( serialDataAvail(fd) )
                {
                        int idx = 0;
                        char gpsdata[512], ch=' ';
                        while( ch != '*' )
                        {
                                ch = serialGetchar(fd);
                                gpsdata[idx++] = ch;
                                //if( ch == '*' )
                                //      break;
                        }
                        gpsdata[idx] = '\0';
                        serialGetchar(fd); serialGetchar(fd); serialGetchar(fd); serialGetchar(fd);
                        
                        string ss(gpsdata);
                        if( ss.find("$GPRMC") == 0 )
                                cout<<ss<<endl;

                }
        }
        return 0;
}
