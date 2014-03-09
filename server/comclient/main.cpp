/* 
 * File:   main.cpp
 * Author: mjirik
 * based on:
 * http://www.nabble.com/Simple-serial-port-demonstration-with-boost-asio-asynchronous-I-O-td19849520.html
 *
 * Created on 4. září 2009, 18:27
 */
#include <boost/thread/thread.hpp> // toto musíme naincludovat pro thread
#include <iostream>                // bežná knihovna pro výstup
#include <unistd.h>                // kvůli funkci sleep()

using namespace std;               // abychom nemuseli opisovat pořád std::

void hello_world()                 // funkce, která poběží v paralelním vláknu
{
  cout << "Hello world, I'm a thread!" << endl;
}

void tecky()                 // funkce, která poběží v paralelním vláknu a vypisuje tečky
{
 for(int i=0;i<61;++i) {cout<<"."<<flush; sleep(1);} // každou vteřinu napíšeme tečku
}

void carky()                 // funkce, která poběží v paralelním vláknu  a vypisuje čárky
{
 for(int i=0;i<31;++i) {cout<<","<<flush; sleep(2);} // každé 2 vteřiny napíšeme čárku
}

int main(int argc, char* argv[])  // tělo
{
  cout<<"Hi, I am main !"<<endl;    // Ohlásíme, že jsme stále v hlavním vlákně
  boost::thread my_thread(&hello_world); //spustíme funkci hello_world v samostatném vlákně
  boost::thread my_thread1(&tecky); //spustíme funkci tecky v samostatném vlákně
  boost::thread my_thread2(&carky); //spustíme funkci carky v samostatném vlákně
  for(int i=0;i<10;++i) {cout<<"-"<<flush; sleep(5);} // Každých 5s vypíšeme pomlčku
  my_thread.join();                      // čekáme dokud vlákno neskončí
  my_thread1.join();                      // čekáme dokud vlákno neskončí
  my_thread2.join();                      // čekáme dokud vlákno neskončí
  cout<<endl<<"Hi, I am main !";          // a zase zahlásíme, že jsme doma
  return 0;                              // int main má vracet hodnotu, pro forma ...
}


/* minicom.cpp
        A simple demonstration minicom client with Boost asio

        Parameters:
                baud rate
                serial port (eg /dev/ttyS0 or COM1)

        To end the application, send Ctrl-C on standard input
 */
/*
#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <boost/asio/serial_port.hpp>

int main()
{
    using namespace boost::lambda;
    typedef std::istream_iterator<int> in;

    std::for_each(
        in(std::cin), in(), std::cout << (_1 * 3) << " " );
}
 */


/*
#define _WIN32_WINNT 0x0600



#include <deque>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
//#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#ifdef POSIX
#include <termios.h>
#endif

using namespace std;

class minicom_client
{
public:
        minicom_client(boost::asio::io_service& io_service, unsigned int baud, const string& device)
                : active_(true),
                  io_service_(io_service),
                  serialPort(io_service, device)
        {
                if (! serialPort.is_open())
                {
                        cerr << "Failed to open serial port\n";
                        return;
                }
                boost::asio::serial_port_base::baud_rate baud_option(baud);
                serialPort.set_option(baud_option); // set the baud rate after the port has been opened
                read_start();
        }

        void write(const char msg) // pass the write data to the do_write function via the io service in the other thread
        {
                io_service_.post(boost::bind(&minicom_client::do_write, this, msg));
        }

        void close() // call the do_close function via the io service in the other thread
        {
                io_service_.post(boost::bind(&minicom_client::do_close, this, boost::system::error_code()));
        }

        bool active() // return true if the socket is still active
        {
                return active_;
        }

private:

        static const int max_read_length = 512; // maximum amount of data to read in one operation

        void read_start(void)
        { // Start an asynchronous read and call read_complete when it completes or fails
                serialPort.async_read_some(boost::asio::buffer(read_msg_, max_read_length),
                        boost::bind(&minicom_client::read_complete,
                                this,
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred));
        }

        void read_complete(const boost::system::error_code& error, size_t bytes_transferred)
        { // the asynchronous read operation has now completed or failed and returned an error
                if (!error)
                { // read completed, so process the data
                        cout.write(read_msg_, bytes_transferred); // echo to standard output
                        read_start(); // start waiting for another asynchronous read again
                }
                else
                        do_close(error);
        }

        void do_write(const char msg)
        { // callback to handle write call from outside this class
                bool write_in_progress = !write_msgs_.empty(); // is there anything currently being written?
                write_msgs_.push_back(msg); // store in write buffer
                if (!write_in_progress) // if nothing is currently being written, then start
                        write_start();
        }

        void write_start(void)
        { // Start an asynchronous write and call write_complete when it completes or fails
                boost::asio::async_write(serialPort,
                        boost::asio::buffer(&write_msgs_.front(), 1),
                        boost::bind(&minicom_client::write_complete,
                                this,
                                boost::asio::placeholders::error));
        }

        void write_complete(const boost::system::error_code& error)
        { // the asynchronous read operation has now completed or failed and returned an error
                if (!error)
                { // write completed, so send next write data
                        write_msgs_.pop_front(); // remove the completed data
                        if (!write_msgs_.empty()) // if there is anthing left to be written
                                write_start(); // then start sending the next item in the buffer
                }
                else
                        do_close(error);
        }

        void do_close(const boost::system::error_code& error)
        { // something has gone wrong, so close the socket & make this object inactive
                if (error == boost::asio::error::operation_aborted) // if this call is the result of a timer cancel()
                        return; // ignore it because the connection cancelled the timer
                if (error)
                        cerr << "Error: " << error.message() << endl; // show the error message
                else
                        cout << "Error: Connection did not succeed.\n";
                cout << "Press Enter to exit\n";
                serialPort.close();
                active_ = false;
        }

private:
        bool active_; // remains true while this object is still operating
        boost::asio::io_service& io_service_; // the main IO service that runs this connection
        boost::asio::serial_port serialPort; // the serial port this instance is connected to
        char read_msg_[max_read_length]; // data read from the socket
        deque<char> write_msgs_; // buffered write data
};

int main(int argc, char* argv[])
{
// on Unix POSIX based systems, turn off line buffering of input, so cin.get() returns after every keypress
// On other systems, you'll need to look for an equivalent
#ifdef POSIX
        termios stored_settings;
        tcgetattr(0, &stored_settings);
        termios new_settings = stored_settings;
        new_settings.c_lflag &= (~ICANON);
        new_settings.c_lflag &= (~ISIG); // don't automatically handle control-C
        tcsetattr(0, TCSANOW, &new_settings);
#endif
        try
        {
                if (argc != 3)
                {
                        cerr << "Usage: minicom <baud> <device>\n";
                        return 1;
                }
                boost::asio::io_service io_service;
                // define an instance of the main class of this program
                minicom_client c(io_service, boost::lexical_cast<unsigned int>(argv[1]), argv[2]);
                // run the IO service as a separate thread, so the main thread can block on standard input
                //boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));
                while (c.active()) // check the internal state of the connection to make sure it's still running
                {
                        char ch;
                        cin.get(ch); // blocking wait for standard input
                        if (ch == 3) // ctrl-C to end program
                                break;
                        c.write(ch);
                }
                c.close(); // close the minicom client connection
                //t.join(); // wait for the IO service thread to close
        }
        catch (exception& e)
        {
                cerr << "Exception: " << e.what() << "\n";
        }
#ifdef POSIX // restore default buffering of standard input
        tcsetattr(0, TCSANOW, &stored_settings);
#endif
        return 0;
}
 */
