#include <iostream>
#include <functional>
#include <fstream>
#include <sstream>

#include "database.hpp"
#include "page_data.hpp"
#include "robots_txt.hpp"

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

void database::get_page_data(struct page_data_s* page_data, std::string& url)
{
    std::hash<std::string> url_hash;
    std::stringstream ss;
    std::string filename;

    //create filename for lookup
    ss<<url_hash(url);
    filename = ss.str();

    dbg<<"page_data_s filename: "<<filename<<std::endl;

    //perform lookup
    file_io_lock.lock();
    std::ifstream file_stream(db_path+"/page_data/"+filename);
    file_io_lock.unlock();

    if(file_stream) {
        dbg<<"stream valid"<<std::endl;
        std::string line;
        while(file_stream.good()) {
            //first line of record is the page rank
            getline(file_stream, line);
            ss.str(line);
            ss >> page_data->rank;
            dbg<<"rank line: "<<line<<std::endl;


            //page title
            getline(file_stream, line);
            getline(file_stream, line);
            page_data->title += line;

            //description
            getline(file_stream, line);     //blank line seperates field data
            getline(file_stream, line);     //first line of description
            while(!line.empty()) {          //rest of description
                page_data->description += line + "\n";
                getline(file_stream, line);
            }

            //meta data
            getline(file_stream, line);
            while(!line.empty()) {
                page_data->meta.push_back(line);
                getline(file_stream, line);
            }
        }

        //perform some sort of file/access lock here (db side)
    }

    file_stream.close();
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
            meta_file.open(db_path+"/meta_data/"+keyword, std::fstream::out|std::fstream::app);
            file_io_lock.unlock();

            meta_file<<page_hash<<std::endl;

            meta_file.flush();
            meta_file.close();
        }
    }
}

void database::put_page_data(struct page_data_s* page_data, std::string& url)
{
    std::hash<std::string> url_hash;
    std::stringstream ss;
    std::string filename;

    //create filename
    ss<<url_hash(url);
    filename = ss.str();

    //avoid concurrent writes to the same file
    file_io_lock.lock();
    std::ofstream file_data(db_path+"/page_data/"+filename);
    file_io_lock.unlock();

    //appending data is the responsibility of the crawler proccess
    if(file_data) {
        //rank
        file_data<<page_data->rank<<std::endl;
        file_data<<std::endl;   //each filed is seperated by a new line

        //title
        file_data<<page_data->title<<std::endl;
        file_data<<std::endl;

        //description
        file_data<<page_data->description<<std::endl;
        file_data<<std::endl;

        //meta - this avoid searching meta_data dir on get_page_data
        for(auto& it: page_data->meta)
            file_data<<it<<std::endl;
    }

    file_data.close();

    //update keywords library
    store_keywords(page_data, filename);
}

void database::get_robots_txt(robots_txt* robots, std::string& url)
{
    std::hash<std::string> url_hash;
    std::stringstream ss;
    std::string filename;

    //create filename for lookup
    ss<<url_hash(url);
    filename = ss.str();

    dbg<<"robots_txt filename: "<<filename<<std::endl;

    //perform lookup
    file_io_lock.lock();
    std::ifstream file_stream(db_path+"/robots/"+filename);
    file_io_lock.unlock();

    if(file_stream) {
        dbg<<"stream valid"<<std::endl;
        std::string line;
        while(file_stream.good()) {
            //first line of record is last_visit
            getline(file_stream, line); //blank line seperates field data
            getline(file_stream, line);
            ss.str(line);
            ss >> robots->last_visit;

            //crawl_delay
            getline(file_stream, line);
            getline(file_stream, line);
            ss.str(line);
            ss >> robots->crawl_delay;

            //robots txt exclusion list
            getline(file_stream, line);
            if(!line.empty()) {
                std::vector<std::string> exclusions;

                while(!line.empty()) {
                    exclusions.push_back(line);
                    getline(file_stream, line);
                }
                robots->import_exclusions(exclusions);
            }

            //robots txt inclusion list
            getline(file_stream, line);
            if(!line.empty()) {
                std::vector<std::string> inclusions;

                while(!line.empty()) {
                    inclusions.push_back(line);
                    getline(file_stream, line);
                }
                robots->import_inclusions(inclusions);
            }
        }

        //perform some sort of file/access lock here (db side)
    }

    file_stream.close();
}


void database::put_robots_txt(robots_txt* robots, std::string& url)
{
    std::hash<std::string> url_hash;
    std::stringstream ss;
    std::string filename;

    std::vector<std::string> exclusions;
    std::vector<std::string> inclusions;

    //create filename
    ss<<url_hash(url);
    filename = ss.str();

    //avoid concurrent writes to the same file
    file_io_lock.lock();
    std::ofstream file_data(db_path+"/robots/"+filename);
    file_io_lock.unlock();

    //appending data is the responsibility of the crawler proccess
    if(file_data) {
        //last visit
        file_data<<robots->last_visit<<std::endl;
        file_data<<std::endl;

        //crawl delay
        file_data<<robots->crawl_delay<<std::endl;
        file_data<<std::endl;

        //both inclusions and exclusions vectors are independantly optional
        if(robots->export_exclusions(exclusions)) {
            for(auto& ex_line : exclusions)
                file_data<<ex_line<<std::endl;
        }

        if(robots->export_inclusions(inclusions)) {
            file_data<<std::endl;
            for(auto& inc_line: inclusions)
                file_data<<inc_line<<std::endl;
        }
    }

    file_data.close();
}




