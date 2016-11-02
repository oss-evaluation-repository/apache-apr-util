/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file apr_redis.h
 * @brief Client interface for redis
 * @remark To use this interface you must have a separate redis
 * for more information.
 */

#ifndef APR_REDIS_H
#define APR_REDIS_H

#include "apr.h"
#include "apr_pools.h"
#include "apr_time.h"
#include "apr_strings.h"
#include "apr_network_io.h"
#include "apr_ring.h"
#include "apr_buckets.h"
#include "apr_reslist.h"
#include "apr_hash.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef RC_DEFAULT_SERVER_PORT
#define RC_DEFAULT_SERVER_PORT 6379
#endif

#ifndef RC_DEFAULT_SERVER_MIN
#define RC_DEFAULT_SERVER_MIN 0
#endif

#ifndef RC_DEFAULT_SERVER_SMAX
#define RC_DEFAULT_SERVER_SMAX 1
#endif

#ifndef RC_DEFAULT_SERVER_TTL
#define RC_DEFAULT_SERVER_TTL 600
#endif

/**
 * @defgroup APR_Util_MC Redis Client Routines
 * @ingroup APR_Util
 * @{
 */

/** Specifies the status of a redis server */
typedef enum
{
    APR_RC_SERVER_LIVE, /**< Server is alive and responding to requests */
    APR_RC_SERVER_DEAD  /**< Server is not responding to requests */
} apr_redis_server_status_t;

/** Opaque redis client connection object */
typedef struct apr_redis_conn_t apr_redis_conn_t;

/** Redis Server Info Object */
typedef struct apr_redis_server_t apr_redis_server_t;
struct apr_redis_server_t
{
    const char *host; /**< Hostname of this Server */
    apr_port_t port; /**< Port of this Server */
    apr_redis_server_status_t status; /**< @see apr_redis_server_status_t */
#if APR_HAS_THREADS || defined(DOXYGEN)
    apr_reslist_t *conns; /**< Resource list of actual client connections */
#else
    apr_redis_conn_t *conn;
#endif
    apr_pool_t *p; /** Pool to use for private allocations */
#if APR_HAS_THREADS
    apr_thread_mutex_t *lock;
#endif
    apr_time_t btime;
    apr_uint32_t rwto;
    struct
    {
        int major;
        int minor;
        int patch;
        char *number;
    } version;
};

typedef struct apr_redis_t apr_redis_t;

/* Custom hash callback function prototype, user for server selection.
* @param baton user selected baton
* @param data data to hash
* @param data_len length of data
*/
typedef apr_uint32_t (*apr_redis_hash_func)(void *baton,
                                            const char *data,
                                            const apr_size_t data_len);
/* Custom Server Select callback function prototype.
* @param baton user selected baton
* @param rc redis instance, use rc->live_servers to select a node
* @param hash hash of the selected key.
*/
typedef apr_redis_server_t* (*apr_redis_server_func)(void *baton,
                                                 apr_redis_t *rc,
                                                 const apr_uint32_t hash);

/** Container for a set of redis servers */
struct apr_redis_t
{
    apr_uint32_t flags; /**< Flags, Not currently used */
    apr_uint16_t nalloc; /**< Number of Servers Allocated */
    apr_uint16_t ntotal; /**< Number of Servers Added */
    apr_redis_server_t **live_servers; /**< Array of Servers */
    apr_pool_t *p; /** Pool to use for allocations */
    void *hash_baton;
    apr_redis_hash_func hash_func;
    void *server_baton;
    apr_redis_server_func server_func;
};

/**
 * Creates a crc32 hash used to split keys between servers
 * @param rc The redis client object to use
 * @param data Data to be hashed
 * @param data_len Length of the data to use
 * @return crc32 hash of data
 * @remark The crc32 hash is not compatible with old redisd clients.
 */
APU_DECLARE(apr_uint32_t) apr_redis_hash(apr_redis_t *rc,
                                         const char *data,
                                         const apr_size_t data_len);

/**
 * Pure CRC32 Hash. Used by some clients.
 */
APU_DECLARE(apr_uint32_t) apr_redis_hash_crc32(void *baton,
                                               const char *data,
                                               const apr_size_t data_len);

/**
 * hash compatible with the standard Perl Client.
 */
APU_DECLARE(apr_uint32_t) apr_redis_hash_default(void *baton,
                                                 const char *data,
                                                 const apr_size_t data_len);

/**
 * Picks a server based on a hash
 * @param rc The redis client object to use
 * @param hash Hashed value of a Key
 * @return server that controls specified hash
 * @see apr_redis_hash
 */
APU_DECLARE(apr_redis_server_t *) apr_redis_find_server_hash(apr_redis_t *rc,
                                                             const apr_uint32_t hash);

/**
 * server selection compatible with the standard Perl Client.
 */
APU_DECLARE(apr_redis_server_t *) apr_redis_find_server_hash_default(void *baton,
                                                                      apr_redis_t *rc,
                                                                      const apr_uint32_t hash);

/**
 * Adds a server to a client object
 * @param rc The redis client object to use
 * @param server Server to add
 * @remark Adding servers is not thread safe, and should be done once at startup.
 * @warning Changing servers after startup may cause keys to go to
 * different servers.
 */
APU_DECLARE(apr_status_t) apr_redis_add_server(apr_redis_t *rc,
                                               apr_redis_server_t *server);


/**
 * Finds a Server object based on a hostname/port pair
 * @param rc The redis client object to use
 * @param host Hostname of the server
 * @param port Port of the server
 * @return Server with matching Hostname and Port, or NULL if none was found.
 */
APU_DECLARE(apr_redis_server_t *) apr_redis_find_server(apr_redis_t *rc,
                                                        const char *host,
                                                        apr_port_t port);

/**
 * Enables a Server for use again
 * @param rc The redis client object to use
 * @param rs Server to Activate
 */
APU_DECLARE(apr_status_t) apr_redis_enable_server(apr_redis_t *rc,
                                                  apr_redis_server_t *rs);


/**
 * Disable a Server
 * @param rc The redis client object to use
 * @param rs Server to Disable
 */
APU_DECLARE(apr_status_t) apr_redis_disable_server(apr_redis_t *rc,
                                                   apr_redis_server_t *rs);

/**
 * Creates a new Server Object
 * @param p Pool to use
 * @param host hostname of the server
 * @param port port of the server
 * @param min  minimum number of client sockets to open
 * @param smax soft maximum number of client connections to open
 * @param max  hard maximum number of client connections
 * @param ttl  time to live in microseconds of a client connection
 * @param rwto r/w timeout value in seconds of a client connection
 * @param ns   location of the new server object
 * @see apr_reslist_create
 * @remark min, smax, and max are only used when APR_HAS_THREADS
 */
APU_DECLARE(apr_status_t) apr_redis_server_create(apr_pool_t *p,
                                                  const char *host,
                                                  apr_port_t port,
                                                  apr_uint32_t min,
                                                  apr_uint32_t smax,
                                                  apr_uint32_t max,
                                                  apr_uint32_t ttl,
                                                  apr_uint32_t rwto,
                                                  apr_redis_server_t **ns);
/**
 * Creates a new redisd client object
 * @param p Pool to use
 * @param max_servers maximum number of servers
 * @param flags Not currently used
 * @param rc   location of the new redis client object
 */
APU_DECLARE(apr_status_t) apr_redis_create(apr_pool_t *p,
                                           apr_uint16_t max_servers,
                                           apr_uint32_t flags,
                                           apr_redis_t **rc);

/**
 * Gets a value from the server, allocating the value out of p
 * @param rc client to use
 * @param p Pool to use
 * @param key null terminated string containing the key
 * @param baton location of the allocated value
 * @param len   length of data at baton
 * @param flags any flags set by the client for this key
 * @return 
 */
APU_DECLARE(apr_status_t) apr_redis_getp(apr_redis_t *rc,
                                         apr_pool_t *p,
                                         const char* key,
                                         char **baton,
                                         apr_size_t *len,
                                         apr_uint16_t *flags);

/**
 * Sets a value by key on the server
 * @param rc client to use
 * @param key   null terminated string containing the key
 * @param baton data to store on the server
 * @param data_size   length of data at baton
 * @param timeout time in seconds for the data to live on the server
 * @param flags any flags set by the client for this key
 */
APU_DECLARE(apr_status_t) apr_redis_setex(apr_redis_t *rc,
                                          const char *key,
                                          char *baton,
                                          const apr_size_t data_size,
                                          apr_uint32_t timeout,
                                          apr_uint16_t flags);

/**
 * Deletes a key from a server
 * @param rc client to use
 * @param key   null terminated string containing the key
 * @param timeout time for the delete to stop other clients from adding
 */
APU_DECLARE(apr_status_t) apr_redis_delete(apr_redis_t *rc,
                                           const char *key,
                                           apr_uint32_t timeout);

/**
 * Query a server's version
 * @param rs    server to query
 * @param p     Pool to allocate answer from
 * @param baton location to store server version string
 * @param len   length of the server version string
 */
APU_DECLARE(apr_status_t) apr_redis_version(apr_redis_server_t *rs,
                                            apr_pool_t *p,
                                            char **baton);

typedef struct
{
    /** Version string of this server */
    const char *version;
    /** Process id of this server process */
    apr_uint32_t pid;
    /** Number of seconds this server has been running */
    apr_uint32_t uptime;
    /** current UNIX time according to the server */
    apr_time_t time;
    /** The size of a pointer on the current machine */
    apr_uint32_t pointer_size;
    /** Accumulated user time for this process */
    apr_time_t rusage_user;
    /** Accumulated system time for this process */
    apr_time_t rusage_system;
    /** Current number of items stored by the server */
    apr_uint32_t curr_items;
    /** Total number of items stored by this server */
    apr_uint32_t total_items;
    /** Current number of bytes used by this server to store items */
    apr_uint64_t bytes;
    /** Number of open connections */
    apr_uint32_t curr_connections;
    /** Total number of connections opened since the server started running */
    apr_uint32_t total_connections;
    /** Number of connection structures allocated by the server */
    apr_uint32_t connection_structures;
    /** Cumulative number of retrieval requests */
    apr_uint32_t cmd_get;
    /** Cumulative number of storage requests */
    apr_uint32_t cmd_set;
    /** Number of keys that have been requested and found present */
    apr_uint32_t get_hits;
    /** Number of items that have been requested and not found */
    apr_uint32_t get_misses;
    /** Number of items removed from cache because they passed their
        expiration time */
    apr_uint64_t evictions;
    /** Total number of bytes read by this server */
    apr_uint64_t bytes_read;
    /** Total number of bytes sent by this server */
    apr_uint64_t bytes_written;
    /** Number of bytes this server is allowed to use for storage. */
    apr_uint32_t limit_maxbytes;
    /** Number of threads the server is running (if built with threading) */
    apr_uint32_t threads; 
} apr_redis_stats_t;

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* APR_REDIS_H */
