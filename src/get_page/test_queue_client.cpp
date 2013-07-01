#include <iostream>
#include <sstream>

#include "queue_client.hpp"

using std::cout;
using std::endl;

int main(void)
{
    qc_config test_config = 1;
    queue_client test_queue(test_config);

    //local control
    const int loops = 10;

    int i;
    cout<<"begin test"<<endl;
    for(i = 0; i < loops; ++i) {
        queue_node_s data;
        std::stringstream ss;

        data.credit = i;
        ss<<"http://test_url_"<<i<<".com/?test_page"<<i<<".html";
        data.url = ss.str();
        
        cout<<"adding item "<<i<<"to queue\n\tcredit: "<<data.credit<<"\n\ttest_url ["<<data.url<<"]"<<endl;
        test_queue.send(data);
    }

    cout<<"---\nretireiving data.."<<endl;
    for(i=0; i< loops; ++i) {
        queue_node_s data = test_queue.fetch();
        cout<<"item "<<i<<"on queue\n\tcredit: "<<data.credit<<"\n\ttest_url ["<<data.url<<"]"<<endl;
    }

    return 0;
}
