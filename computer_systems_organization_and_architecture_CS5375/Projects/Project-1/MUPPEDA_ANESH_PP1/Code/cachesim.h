/*
 * CS3375 Computer Architecture
 * Course Project
 * Cache Simulator Design and Development
 * FALL 2017
 * By Yong Chen
 */

#define BLOCK_SIZE  	64       	/* Cache block size (or cache line size) in bytes \\
                                 	(must be power of 2). 4 Bytes = 1 Word */                                                              
#define WAY_SIZE    	1        	/* Associativity; 1-way = direct-mapped */
#define CACHE_SIZE  	65536       /* Cache capacity in bytes (must be power of 2)*/
#define NUM_BLOCKS  	(CACHE_SIZE / BLOCK_SIZE)
#define NUM_SETS    	(NUM_BLOCKS / WAY_SIZE)

/*Here we are defining the L1 Cache variables with the fixed sizes*/
#define L1_BLOCK_SIZE  	64			//L1 block size with 64 bytes.
#define L1_WAY_SIZE    	2			//L1 with 2-Way mapping technique.
#define L1_CACHE_SIZE	65536		//L1 cache size in bytes.
#define L1_NUM_BLOCKS  	(L1_CACHE_SIZE / L1_BLOCK_SIZE)
#define L1_NUM_SETS    	(L1_NUM_BLOCKS / L1_WAY_SIZE)

/*Here we are defining the L2 Cache variables with the fixed sizes*/
#define L2_BLOCK_SIZE  	64			//L2 Block size with 64 bytes.
#define L2_WAY_SIZE    	8 			//L2 cache with 8-Way mapping technique.
#define L2_CACHE_SIZE	1048576		//L2 Cache Size in bytes.
#define L2_NUM_BLOCKS  	(L2_CACHE_SIZE / L2_BLOCK_SIZE)
#define L2_NUM_SETS    	(L2_NUM_BLOCKS / L2_WAY_SIZE)

//#define DBG

/*The data structure of direct-mapped cache*/
struct direct_mapped_cache {
    unsigned valid_field[NUM_BLOCKS];   /* Valid field */
    unsigned dirty_field[NUM_BLOCKS];   /* Dirty field; since we don't distinguish writes and
                                           reads in this project yet, this field doesn't really matter */
    uint64_t tag_field[NUM_BLOCKS];     /* Tag field */
    int hits;                          	/* Hit count */
    int misses;                        	/* Miss count */
};
/*The data structure of N-Way cache*/
struct n_way_cache_access {
	unsigned valid_field[NUM_BLOCKS];   /*Valid field*/
	uint64_t tag_field[NUM_BLOCKS];     /*Tag field*/
	int hits;							/*Hits count*/
    int misses;			                /*Misses Count*/
	int compulsory_misses;				/*Compulsory Misses count*/
	int capacity_misses;				/*Capacity Misses count*/
};
/*The data structure of Full-Way cache*/
struct full_way_cache_access {
	unsigned valid_field[NUM_BLOCKS];   /*Valid field*/
	uint64_t tag_field[NUM_BLOCKS];     /*Tag field*/
	int hits;							/*Hits count*/
    int misses;							/* Misses Count */
	int compulsory_misses;				/*Compulsory Misses count*/
	int capacity_misses;				/*Capacity Misses count*/
};
/*The data structure of Level-1 Cache in two level cache mechanism*/
struct l1_way_cache_access {
	unsigned l1_valid_field[L1_NUM_BLOCKS]; /*Valid field*/
	uint64_t l1_tag_field[L1_NUM_BLOCKS];   /*Tag field*/
	int l1_hits;							/*Hits count*/
    int l1_misses;			                /*Misses Count*/
	int l1_compulsory_misses;				/*Compulsory Misses count*/
	int l1_capacity_misses;					/*Capacity Misses count*/
};
/*The data structure of Level-2 Cache in two level cache mechanism*/
struct l2_way_cache_access {
	unsigned l2_valid_field[L2_NUM_BLOCKS]; /*Valid field*/
	uint64_t l2_tag_field[L2_NUM_BLOCKS];   /*Tag field*/
	int l2_hits;							/*Hits count*/
    int l2_misses;			                /*Misses Count*/
	int l2_compulsory_misses;				/*Compulsory Misses count*/
	int l2_capacity_misses;					/*Capacity Misses count*/
};

/*Read the memory traces and convert it to binary*/
uint64_t convert_address(char memory[]);

/*Simulate the direct-mapped cache*/
void direct_mapped_cache_access(struct direct_mapped_cache *cache, uint64_t address);
/*Simulate the N-Way Cache*/
void n_way_cache_access(struct n_way_cache_access *cache, uint64_t address, int n_sets);
/*Simulate the Full-Way Cache*/
void full_way_cache_access(struct full_way_cache_access *cache, uint64_t address);
/*Simulate the Two Level Cache Mechanism*/
void two_way_cache_access(struct l1_way_cache_access *cache, struct l2_way_cache_access *l2cache, uint64_t address);
/*Simulate the Level Two Cache*/
void level_two_calculation(struct l2_way_cache_access *cache, uint64_t address);
/*Calculating the Hit rate and Miss rate*/
void caluculating_hit_miss_rate(int hits, int misses);
/*Calculating the Compulsory and Capacity Miss rate*/
void caluculating_compulsory_capacity_miss_rate(int hits, int misses, int compulsory_misses, int capacity_misses);