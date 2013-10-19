#if !defined (IPC_COMMON_H)
#define IPC_COMMON_H


/**
 * status of crawler_worker
 */
enum worker_status {
    ZOMBIE,         //dead
    IDLE,           //waiting for instructions
    READY,          //configured
    ACTIVE,         //working
    SLEEP,          //blocked (queue)
    STOPPNIG        //will idle when current crawls complete
};

/**
 * CnC intructions to workers
 */
enum worker_intruction {
    START,          //begin processing data from work queue
    STOP,           //finish current processing and idle
    CAPABILITIES,   //report capabilities
    CONFIGURE,      //configure worker
    GET_CONF,       //report current config
    KILL            //die now
}

/**
 * configuration sent to worker
 */
struct worker_config {
    std::string user_agent;

    //cache config
    unsigned int page_cache_max;
    unsigned int page_cache_res;
    unsigned int robots_cache_max;
    unsigned int robots_cache_res;

    //database
    std::string db_path;
};

/**
 * capabilities reported by worker
 */
struct capabilities {
    unsigned int parsers;
    unsigned int total_threads;
};

#endif
