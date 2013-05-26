#include <iostream>
#include <functional>
#include <fstream>
#include <sstream>

#include "database.hpp"
#include "page_data.hpp"

#define DEBUG 1

//Local defines
#if defined(DEBUG)
    #define dbg std::cout<<__FILE__<<"("<<__LINE__<<"): "
    #if DEBUG > 1
        #define dbg_1 std::cout<<__FILE__<<"("<<__LINE__<<"): "
    #else
        #define dbg_1 0 && std::cout
    #endif
#else
    #define dbg 0 && std::cout
    #define dbg_1 0 && std::cout
#endif

database::database(std::string path)
{
    db_path = path;
}

database::~database(void)
{

}

inline bool database::is_inst(std::string line, std::string inst)
{
    return line.compare(0, inst.length(), inst) == 0 ? true : false;
}

instruction database::process_instruction(std::string line)
{
    if(is_inst(line, "rank"))
        return RANK;

    else if(is_inst(line, "description"))
        return DESCRIPTION;

    else if(is_inst(line, "meta"))
        return META;

    else if(is_inst(line, "last_visit"))
        return LAST_VISIT;

    else if(is_inst(line, "crawl_delay"))
        return CRAWL_DELAY;

    else
        return NOT_INST;
}

struct page_data_s* database::get_page(std::string& url)
{
    std::hash<std::string> url_hash;
    std::stringstream ss;
    std::string filename;

    //create filename for lookup
    ss<<url_hash(url);
    filename = ss.str();

    //perform lookup
    file_io_lock.lock();
    std::ifstream file_stream(db_path+"page_data"+filename);
    file_io_lock.unlock();

    struct page_data_s* page_data = new struct page_data_s;
    if(file_stream) {
        std::string line;
        while(file_stream.good()) {
            getline(file_stream, line);

            switch(process_instruction(line)) {
            case RANK:
                getline(file_stream, line);

                ss.str(line);
                ss >> page_data->rank;
                break;

            case DESCRIPTION:
                while(line != "\n") {
                    getline(file_stream, line);
                    page_data->description = line;
                }
                break;

            case META:
                while(line != "\n") {
                    getline(file_stream, line);
                    page_data->meta.push_back(line);
                }
                break;

            case LAST_VISIT:
                getline(file_stream, line);

                ss.str(line);
                ss >> page_data->last_visit;
                break;

            case CRAWL_DELAY:
                getline(file_stream, line);

                ss.str(line);
                ss >> page_data->crawl_delay;
                break;

            default:
                //do nothing.
                break;
            }
        }
    }

    file_stream.close();
    return page_data;
}

void database::store_keywords(struct page_data_s* page_data, std::string page_hash)
{
    for(auto& keyword: page_data->meta) {
        dbg<<"keyword: "<<keyword<<std::endl;
        
        //each keyword has its own "meta" file, which contains hashes of matching pages
        file_io_lock.lock();
        //first we read to see if keyword is already associated with page
        std::fstream meta_file(db_path+"/meta_data/"+keyword, std::fstream::in);
        file_io_lock.unlock();

        //check if page-keyword association already exists.
        bool meta_present = false;
        
        if(meta_file) {
            while(meta_file.good()) {
                std::string line;
                getline(meta_file, line);

                //already in (keyword)meta_file
                if(is_inst(line, keyword)) {
                    meta_present = true;
                    break;
                }
            }
        }
        meta_file.close();
        
        //new keyword and/or file
        if(!meta_present) {
            //open for writing
            file_io_lock.lock();
            meta_file.open(db_path+"/meta_data/"+keyword, std::fstream::out);
            file_io_lock.unlock();
            
            meta_file<<page_hash<<std::endl;

            meta_file.flush();
            meta_file.close();
        }
    }
}

void database::put_page(std::string& url, struct page_data_s* page_data)
{
    std::hash<std::string> url_hash;
    std::stringstream ss;
    std::string filename;

    //create filename
    ss<<url_hash(url);
    filename = ss.str();

    //make sure page isnt in use
    page_data->access_lock.lock();

    //avoid concurrent writes to the same file
    file_io_lock.lock();
    std::ofstream file_data(db_path+"/page_data/"+filename);
    file_io_lock.unlock();

    //appending data is the responsibility of the crawler proccess
    if(file_data) {
        file_data<<"rank\n";
        file_data<<page_data->rank<<std::endl;

        file_data<<"\nmeta\n";
        for(auto& it: page_data->meta)
            file_data<<it<<std::endl;

        file_data<<"\ndescription\n";
        file_data<<page_data->description<<std::endl;

        file_data<<"\nlast_visit\n";
        file_data<<page_data->last_visit<<std::endl;

        file_data<<"\ncrawl_delay\n";
        file_data<<page_data->crawl_delay<<std::endl;
    }

    file_data.close();

    //update keywords library
    store_keywords(page_data, filename);
    page_data->access_lock.unlock();
}
