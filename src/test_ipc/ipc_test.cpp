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
    ipc_client(std::string addr): resolver_(ipc_service), socket_(ipc_service)
    {
        tcp::resolver::query query(addr, MASTER_SERVICE_NAME);
        resolver_.async_resolve(query,
            boost::bind(&ipc_client::handle_resolved, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::iterator));

        ipc_service.run();
    }

    void send_data(int d)
    {
        boost::asio::async_write(socket_, d,
            boost::asio::transfer_all(),
            boost::bind(&ipc_client::sent_data, this,
                boost::asio::placeholders::error));
    }
    
    private:
    boost::asio::io_service ipc_service;
    tcp::resolver resolver_;
    tcp::socket socket_;

    void sent_data(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator)
    {
        if(!err) {
            cout<<"write successful\n";
        } else {
            cout<<"send_data failed - "<<err.message()<<endl;
            exit(-1);
        }
    }

    //master address resolved, connect
    void handle_resolved(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator)
    {
        if(err) {
            cout<<"handle_resolve: "<<err.message()<<std::endl;
            exit(-1);

        } else {
            boost::asio::async_connect(socket_, endpoint_iterator,
                boost::bind(&ipc_client::handle_connected, this,
                    boost::asio::placeholders::error));
        }
        
        cout<<"resolved master address\n";
    }

    //connected to master, begin processing/scheduling tasks
    void handle_connected(const boost::system::error_code& err)
    {
        if(err)
            cout<<"handle_connect: "<<err.message()<<std::endl;
            exit(-1);
        cout<<"connected.\n";
    }
};

class ipc_server
{
    public:
    ipc_server(): acceptor_(ipc_service, tcp::endpoint(tcp::v4(), MASTER_SERVICE_PORT)),
        socket_(ipc_service)
    {
        do_accept();
    }

    private:
    boost::asio::io_service ipc_service;
    tcp::acceptor acceptor_;
    tcp::socket socket_;

    int data;

    void read_from_uut(const boost::system::error_code& err)
    {
        cout<<"read from client ["<<data<<"]\n";
    }

    void handle_accepted(const boost::system::error_code& err)
    {
        if(!err) {
            boost::asio::async_read(socket_, data,
                boost::asio::transfer_all(),
                boost::bind(&ipc_server::read_from_uut, this,
                    boost::asio::placeholders::error));
        } else {
            cout<<"do_accept asio error: "<<err.message()<<endl;
            exit(-1);
        }
    }
    
    void do_accept(void)
    {
        acceptor_.async_accept(socket_,
            boost::bind(&ipc_server::handle_accepted, this,
                boost::asio::placeholders::error));
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
