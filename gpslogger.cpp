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
                        vector<string> tkns;
                        int start = 0, pos;
                        pos = gps_string.find( ",", start );
                        if( gps_string.substr( start, pos-start ) == "$GPRMC" )
                        {
                                if( gps_string.find_first_of("A") == string::npos )
                                {
                                        cout<<"GPS warning!"<<endl;
                                        continue;
                                }
                                start = pos+1;
                                do {
                                        pos = gps_string.find( ",", start );
                                        tkns.push_back( gps_string.substr( start, pos-start ) );
                                        start = pos+1;
                                } while( tkns.back() != "E" );

                                cout<<"UTC:\t"<<tkns[0]<<endl;
                                cout<<tkns[2]<<" north\n";
                                cout<<tkns[4]<<" east"<<endl;
                        }

                }
        }
        return 0;
}
