#include <iostream>
#include <stdexcept>
#include <exception>
#include <boost/asio.hpp>                   //all ipc
#include <boost/bind.hpp>
#include <thread>

#define MASTER_SERVICE_NAME "crawler_cnc"
#define MASTER_SERVICE_PORT 23331

using boost::asio::ip::tcp;
using std::cout;
using std::endl;

class ipc_client
{
    public:
    ipc_client(std::string addr): resolver_(ipc_service), socket_(ipc_service)
    {
        cout<<"initiating client\n";
        tcp::resolver::query query(tcp::v4(), "127.0.0.1", "23331");
        cout<<"\tconstructed query\n";
        tcp::resolver::iterator ei = resolver_.resolve(query);
        cout<<"\tfound server\n";

        boost::asio::connect(socket_, ei);
        cout<<"\tconnected to server\n";
    }

    void send_data(char d[32])
    {
        boost::asio::write(socket_, boost::asio::buffer(d, 32));
        cout<<"data sent ["<<d<<"]\n";
    }

    private:
    boost::asio::io_service ipc_service;
    tcp::resolver resolver_;
    tcp::socket socket_;
};

class ipc_server
{
    public:
    ipc_server(): acceptor_(ipc_service, tcp::endpoint(tcp::v4(), MASTER_SERVICE_PORT))
    {
        cout<<"initiated server\n";
        do_accept();
    }

    private:
    boost::asio::io_service ipc_service;
    tcp::acceptor acceptor_;

    void do_accept(void)
    {
        cout<<"server accepting connections\n";
        for(;;) {
            tcp::socket socket(ipc_service);
            acceptor_.accept(socket);
            cout<<"client connected\n";

            char data[32];

            boost::system::error_code error;
            boost::asio::read(socket, boost::asio::buffer(data, sizeof(data)), error);
            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            cout<<"client sent "<<data<<endl;
        }
    }
};

void run_server(void)
{
    ipc_server test_server;
    cout<<"end of run_server()\n";
}

int main(void)
{
    cout<<"instantiating server\n";
    std::thread srv(run_server);

    cout<<"instantiating client\n";
    ipc_client test_client("127.0.0.1");

    char data[32] = {'x'};
    cout<<"sending data ["<<data<<"] to server\n";

    test_client.send_data(data);

    cout<<"done\n";
    srv.join();
    cout<<"server thread joined\n";

    return 0;
}
