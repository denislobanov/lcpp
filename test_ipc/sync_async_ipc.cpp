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
        cout<<"client: initiating client\n";
        tcp::resolver::query query(tcp::v4(), "127.0.0.1", "23331");
        cout<<"client: \tconstructed query\n";
        tcp::resolver::iterator ei = resolver_.resolve(query);
        cout<<"client: \tfound server\n";

        boost::asio::connect(socket_, ei);
        cout<<"client: \tconnected to server\n";
    }

    void send_data(char d[32])
    {
        boost::asio::write(socket_, boost::asio::buffer(d, 32));
        cout<<"client: data sent ["<<d<<"]\n";
    }

    private:
    boost::asio::io_service ipc_service;
    tcp::resolver resolver_;
    tcp::socket socket_;
};

class ipc_server
{
    public:
    ipc_server():
        acceptor_(ipc_service, tcp::endpoint(tcp::v4(), MASTER_SERVICE_PORT)),
        socket_(ipc_service)
    {
        cout<<"server: initiated server\n";
        do_accept();
    }

    private:
    boost::asio::io_service ipc_service;
    tcp::acceptor acceptor_;
    tcp::socket socket_;
    char data[32];

    void do_accept(void)
    {
        acceptor_.async_accept(socket_,
            [this](boost::system::error_code ec)
            {
                cout<<"server: lambda acceptor handler\n";
                if(!ec) {
                    cout<<"server: async read\n";
                    boost::asio::async_read(socket_,
                        boost::asio::buffer(data, 32),
                        boost::bind(&ipc_server::read_handler,
                                    this,
                                    boost::asio::placeholders::error)
                    );
                }

                do_accept();
            });
    }

    void read_handler(boost::system::error_code ec)
    {
        if(!ec)
            cout<<"server: client sent ["<<data<<"]\n";
    }
};

int main(void)
{
    cout<<"instantiating server\n";
    ipc_server test_server;

    cout<<"instantiating client\n";
    ipc_client test_client("127.0.0.1");

    char data[32] = {'x'};
    cout<<"sending data ["<<data<<"] to server\n";

    test_client.send_data(data);

    cout<<"done\n";
    return 0;
}
