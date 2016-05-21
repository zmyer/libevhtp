#ifndef __EVTHR_H__
#define __EVTHR_H__

#include <pthread.h>
#include <event2/event.h>
#include <evhtp-config.h>

#ifdef __cplusplus
extern "C" {
#endif

enum evthr_res {
    EVTHR_RES_OK = 0,
    EVTHR_RES_BACKLOG,
    EVTHR_RES_RETRY,
    EVTHR_RES_NOCB,
    EVTHR_RES_FATAL
};

struct evthr_pool;
struct evthr;

typedef struct event_base evbase_t;
typedef struct event      ev_t;

typedef struct evthr_pool evthr_pool_t;
typedef struct evthr      evthr_t;
typedef enum evthr_res    evthr_res;

typedef void (* evthr_cb)(evthr_t * thr, void * cmd_arg, void * shared);
typedef void (* evthr_init_cb)(evthr_t * thr, void * shared);
typedef void (* evthr_exit_cb)(evthr_t * thr, void * shared);


/**
 * @brief creates a single new thread
 * @note to be deprecated by evthr_wexit_new
 *
 * @param evthr_init_cb callback to execute upon spawn
 * @param shared data which is passed to every callback
 *
 * @return NULL on error
 * @see evthr_free(), evthr_wexit_new()
 */
EVHTP_EXPORT evthr_t * evthr_new(evthr_init_cb, void *) DEPRECATED("will take on the syntax of evthr_wexit_new");


/**
 * @brief same as evthr_new() but execs a callback on a thread exit
 *
 * @param evthr_init_cb
 * @param evthr_exit_cb
 * @param shared
 *
 * @return
 * @see evthr_new()
 */
EVHTP_EXPORT evthr_t * evthr_wexit_new(evthr_init_cb, evthr_exit_cb, void * shared);


/**
 * @brief free all underlying data in a ssingle evthr_t
 *
 * @param evthr
 *
 * @return
 * @see evthr_new(), evthr_wexit_new()
 */
EVHTP_EXPORT void evthr_free(evthr_t * evthr);

/**
 * @brief get the thread-specific event_base
 *
 * @param thr a single thread
 *
 * @return the event_base of the thread, NULL on error
 */
EVHTP_EXPORT evbase_t * evthr_get_base(evthr_t * thr);

/**
 * @brief set non-shared thread-specific arguments
 *
 * @param thr the thread context
 * @param aux the data to set
 *
 * @return
 * @see evthr_get_aux()
 */
EVHTP_EXPORT void evthr_set_aux(evthr_t * thr, void * aux);


/**
 * @brief get the non-shared thread arguments
 *
 * @param thr a single thread context
 *
 * @return the threads non-shared arguments
 * @see evthr_set_aux()
 */
EVHTP_EXPORT void * evthr_get_aux(evthr_t * thr);


/**
 * @brief starts up the thread + event loop
 *
 * @param evthr
 *
 * @return 0 on success, -1 on error
 * @see evthr_stop()
 */
EVHTP_EXPORT int evthr_start(evthr_t * evthr);


/**
 * @brief stop and shutdown a thread
 *
 * @param evthr
 *
 * @return an evthr_res
 * @see evthr_start()
 */
EVHTP_EXPORT evthr_res evthr_stop(evthr_t * evthr);


/**
 * @brief defer a callback into a thread
 * @note any shared data needs to be reentrant
 *
 * @param evthr the evthread context
 * @param cb callback to execute in the thread
 * @param args arguments to be passed to the callback
 *
 * @return evthr_res
 */
EVHTP_EXPORT evthr_res evthr_defer(evthr_t * evthr, evthr_cb cb, void *);

/**
 * @brief create a new threadpool
 * @note deprecated bty evthr_pool_wexit_new
 *
 * @param nthreads number of threads
 * @param evthr_init_cb callback to execute on a new spawn
 * @param shared args passed to the callback
 *
 * @return a evthr_pool on success, NULL on error
 * @see evthr_pool_free(), evthr_pool_wexit_new()
 */
EVHTP_EXPORT evthr_pool_t * evthr_pool_new(int nthreads, evthr_init_cb, void *) DEPRECATED("will take on the syntax of evthr_pool_wexit_new");


/**
 * @brief works like evthr_pool_new, but holds a thread on-exit callback
 *
 * @param nthreads number of threads
 * @param evthr_init_cb thread on-init callback
 * @param evthr_exit_cb thread on-exit callback
 * @param shared args passed to the callback
 *
 * @return evthr_pool on success, NULL on error
 * @see evthr_pool_free(), evthr_pool_new()
 */
EVHTP_EXPORT evthr_pool_t * evthr_pool_wexit_new(int nthreads, evthr_init_cb, evthr_exit_cb, void *);


/**
 * @brief iterates over each thread and tears it down
 * @note if the on-exit cb is set, it is called for each thread
 *
 * @param pool
 *
 * @return
 * @see evthr_pool_new(), evthr_pool_wexit_new()
 */
EVHTP_EXPORT void evthr_pool_free(evthr_pool_t * pool);


/**
 * @brief spawns all the threads in the pool
 *
 * @param pool
 *
 * @return 0 on success, -1 on error
 * @see evthr_pool_stop()
 */
EVHTP_EXPORT int evthr_pool_start(evthr_pool_t * pool);


/**
 * @brief stops all threads in the pool
 *
 * @param pool
 *
 * @return evthr res
 * @see evthr_pool_start()
 */
EVHTP_EXPORT evthr_res evthr_pool_stop(evthr_pool_t * pool);


/**
 * @brief execute a callback using a thread in the pool
 *
 * @param pool the threadpoool context
 * @param cb callback to execute in a single thread
 * @param arg argument passed to the callback
 *
 * @return res
 * @see evthr_defer()
 */
EVHTP_EXPORT evthr_res evthr_pool_defer(evthr_pool_t * pool, evthr_cb cb, void * arg);


#ifdef __cplusplus
}
#endif

#endif /* __EVTHR_H__ */

