#include <iostream>
#include <stdexcept>
#include <exception>
#include <boost/asio.hpp>                   //all ipc
#include <boost/bind.hpp>

#define MASTER_SERVICE_NAME "crawler_cnc"
#define MASTER_SERVICE_PORT 23331

using boost::asio::ip::tcp;
using std::cout;
using std::endl;

class ipc_client
{
    public:
    ipc_client(std::string addr)
    {
        tcp::resolver::query query("127.0.0.1", MASTER_SERVICE_NAME);
        tcp::resolver::iterator ei = resolver_.resolve(query);

        socket_(ipc_service);
        boost::asio::connect(socket_, ei);
    }

    void send_data(int d)
    {
        boost::asio::write(socket_, d, boost::asio::transfer_all());
    }
    
    private:
    boost::asio::io_service ipc_service;
    tcp::resolver resolver_;
    tcp::socket socket_;
};

class ipc_server
{
    public:
    ipc_server()
    {
        acceptor_(ipc_service, tcp::endpoint(tcp::v4(), MASTER_SERVICE_PORT));
        do_accept();
    }

    private:
    boost::asio::io_service ipc_service;
    tcp::acceptor acceptor_;

    int data;

    void do_accept(void)
    {
        for(;;) {
            tcp::socket socket(ipc_service);
            acceptor_.accept(socket);

            int data;
            
            boost::system::error_code ignored_error;
            boost::asio::read(socket, boost::asio::buffer(data), ignored_error);
            cout<<"client sent "<<data<<endl;
        }
    }
};

int main(void)
{
    cout<<"instantiating server\n";
    ipc_server test_server;

    cout<<"instantiating client\n";
    ipc_client test_client("127.0.0.1");

    cout<<"sending data [int 42] to server\n";
    test_client.send_data(42);

    cout<<"done\n";

    return 0;
}
