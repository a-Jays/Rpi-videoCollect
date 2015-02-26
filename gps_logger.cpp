
std::atomic<bool> gps_lock = false;

void gps_thread_entry()
{
        int fd;
        if( wiringPiSetup() == -1 )                     // init wiringPi.
        {
                cout<<"wiring not done."<<endl;
                return;
        }

        fd = serialOpen("/dev/ttyAMA0", 9600);          // ensure correct string for serial port.
        if( fd < 0 )
        {
                cout<<"can't open port."<<endl;
                return;
        }

        // UTTERLY BAD (ALTHOUGH FUNCTIONAL) CODE FOLLOWS..
        while( true )
        {
                if( serialDataAvail(fd) )                       // if at an instance, data available,
                {
                        int idx = 0;
                        char gpsdata[512], ch=' ';              // create buffer for gps lines.
                        while( ch != '*' )                      // '*' is the end of a gps line
                        {
                                ch = serialGetchar(fd);
                                gpsdata[idx++] = ch;
                        }
                        gpsdata[idx] = '\0';                    // required C hack.
                        serialGetchar(fd); serialGetchar(fd); serialGetchar(fd); serialGetchar(fd);
                        // the above is to clear out the gps data in line (checksum and EOL). Think serial flush.
                        
                        string gps_string(gpsdata);
                        vector<string> tokns;
                        int start = 0, pos;
                        pos = gps_string.find( ",", start );
                        if( gps_string.substr( start, pos-start ) == "$GPRMC" )         // if first token is $GPRMC
                        {
                                if( gps_string.find_first_of("V") < gps_string.length() )     // if "A" not found in string.
                                {                                                       // "A" - available, "V" - warning.
                                        cout<<"GPS warning!"<<endl;
                                        continue;
                                }
                                gps_lock = true;                                        // for camera code.
                                start = pos+1;
                                do {
                                        pos = gps_string.find( ",", start );
                                        tokns.push_back( gps_string.substr( start, pos-start ) );
                                        start = pos+1;
                                } while( tokns.back() != "E" );                         // tokenise only till "E". Quick.
                                
                                // alternatively use boost::split(tokns, gps_string, boost::is_any_of(","));

                                cout<<"UTC:\t"<<tokns[0]<<endl;
                                cout<<tokns[2]<<" north\n";
                                cout<<tokns[4]<<" east"<<endl;
                        }

                }
        }
}
