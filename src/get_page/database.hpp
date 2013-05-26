#if !defined(DATABASE_H)
#define DATABASE_H

#include <iostream>
#include <functional>

#include "page_data.hpp"

enum instruction {
    RANK,
    DESCRIPTION,
    META,
    LAST_VISIT,
    CRAWL_DELAY,
    NOT_INST
};

class database
{
    public:
    database(std::string path);
    ~database(void);

    /**
     * url is hashed and a page_data_s retrieved.
     * if no page_data_s is present, a new one is allocated.
     */
    struct page_data_s* get_page(std::string& url);

    /**
     * page is potentially cached, depending on criteria and sent to the db
     */
    void put_page(std::string& url, struct page_data_s* page_data);

    private:
    std::string db_path;
    std::mutex file_io_lock;    //concurrent open to the same file

    bool is_inst(std::string line, std::string inst);
    instruction process_instruction(std::string line);
    void store_keywords(struct page_data_s* page_data, std::string page_hash);
};

#endif
