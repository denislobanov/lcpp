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
        cout<<"client: constructed query\n";
        tcp::resolver::iterator ei = resolver_.resolve(query);
        cout<<"client: found server\n";

        boost::asio::async_connect(socket_, ei,
            [this](boost::system::error_code ec, tcp::resolver::iterator i)
            {
                if(ec)
                    cout<<"client: connection error - "<<ec.message()<<endl;

                cout<<"client:  trying to connect to "<<i->endpoint()<<endl;
                return i;
            },
            [this](boost::system::error_code ec, tcp::resolver::iterator i)
            {
                if(!ec)
                    cout<<"client: connected to server: "<<i->endpoint()<<endl;
                else
                    cout<<"client: connection error - "<<ec.message()<<endl;
            });

        ipc_service.run();
    }

    void disconnect(void)
    {
        ipc_service.post([this]() { socket_.close(); });
    }

    void send_data(char d[32])
    {
        boost::asio::async_write(socket_, boost::asio::buffer(d, 32),
            [this, d](boost::system::error_code ec, std::size_t)
            {
                if(!ec) {
                    cout<<"client: data sent ["<<d<<"]\n";
                } else {
                    cout<<"client: write handler error "<<ec.message()<<endl;
                    socket_.close();
                }
            });
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
        ipc_service.run();
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
    test_client.disconnect();

    cout<<"done\n";
    srv.join();
    cout<<"server thread joined\n";
    return 0;
}
