/*
 * CS3375 Computer Architecture
 * Course Project
 * Cache Simulator Design and Development
 * FALL 2017
 * By Yong Chen
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "cachesim.h"
#include <time.h>    // time()
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        //We need to pass the two arguments, one is output file followed by the tracefile
        printf("Invalid usage of arguments\n");
        printf("Usage: %s <trace file name>\n", argv[0]);
        return 1;
    }

#ifdef DBG
    printf("BLOCK SIZE = %d Bytes\n", BLOCK_SIZE);
    printf("%d-WAY\n", WAY_SIZE);
    printf("CACHE SIZE = %d Bytes\n", CACHE_SIZE);
    printf("NUMBER OF BLOCKS = %d\n", NUM_BLOCKS);
    printf("NUMBER OF SETS = %d\n", NUM_SETS);
    printf("\n");
#endif

    //Declaring the Mapping Technique option as a user input
    int mapping_technique;
    printf("Enter the one of the Mapping Technique from below list\n");
    printf(" 1. Direct Mapping \n 2. Full Way Mapping \n 3. N-Way Mapping\n 4. Two-Level Cache(L1 & L2) \n 5. Exit \n\n");
    scanf("%d", &mapping_technique);
    
    //storing the trace file from the user input argument
    char* trace_file_name = argv[1];

    struct direct_mapped_cache d_cache;
    struct full_way_cache_access f_cache;
    struct n_way_cache_access n_cache;
    struct l1_way_cache_access l1_cache;
    struct l2_way_cache_access l2_cache;

    char mem_request[20];
    uint64_t address;
    FILE *fp;

    /* Opening the memory trace file */
    fp = fopen(trace_file_name, "r");

    //Switch case to switch between the all Mapping Techniques.
    switch(mapping_technique) {
        case 1 :
                    printf("Welcome to the Direct Mapping Technique\n");
                    /* Initialization of all fields in Direct Mapping Technique*/
                    for (int i=0; i<NUM_BLOCKS; i++) {
                        d_cache.valid_field[i] = 0;
                        d_cache.dirty_field[i] = 0;
                        d_cache.tag_field[i] = -1;
                    }
                    d_cache.hits = 0;
                    d_cache.misses = 0;

                    /*While Loop: Copying the hexa decimal address from trace file and converting it to the binary and passing 
                    it to the Direct Papped Cache function*/
                    while (fgets(mem_request, 20, fp)!= NULL) 
                    {
                        address = convert_address(mem_request);
                        direct_mapped_cache_access(&d_cache, address);
                    }
                    /*Print out the results*/
                    printf("\n==================================\n");
                    printf("Cache type:    Direct-Mapped Cache\n");
                    printf("==================================\n");
                    printf("Cache Hits:    %d\n", d_cache.hits);
                    printf("Cache Misses:  %d\n", d_cache.misses);
                    printf("---Total performance---\n");
                    /*Calling the calculating function with hits and misses to calculate the hit and miss ratio*/
                    caluculating_hit_miss_rate(d_cache.hits, d_cache.misses);
                    break;
        case 2 :
                    printf("Welcome to the Full-Way Mapping Technique\n");
                    /* Initialization of all fields in Full Way Mapping Technique*/
                    for (int i = 0; i<NUM_BLOCKS; i++) 
                    {
			            f_cache.valid_field[i] = 0;
			            f_cache.tag_field[i] = -1;
		            }
		            f_cache.hits = 0;
		            f_cache.misses = 0;
                    f_cache.compulsory_misses = 0;
                    f_cache.capacity_misses = 0;

                    /*While Loop: Copying the hexa decimal address from trace file and converting it to the
                    binary and passing it to the Full Way cache function*/
		            while (fgets(mem_request, 20, fp) != NULL) 
                    {
			            address = convert_address(mem_request);
			            full_way_cache_access(&f_cache, address);
		            }
		            /*Print out the results*/
                    printf("BLOCK SIZE = %d Bytes\n", BLOCK_SIZE);
                    printf("CACHE SIZE = %d Bytes\n", CACHE_SIZE);
		            printf("\n==================================\n");
                    printf("Cache type:    Full-Way Cache\n");
                    printf("==================================\n");
                    printf("Cache Hits:                 %d\n", f_cache.hits);
                    printf("Cache Misses:               %d\n", f_cache.misses);
                    printf("Cache Compulsory Misses:    %d\n", f_cache.compulsory_misses);
                    printf("Cache Capacity Misses:      %d\n", f_cache.capacity_misses);
                    printf("\n---Total Full-Way Cache performance---\n");
                    /*Calling the calculating function with hits and misses to calculate the hit and miss ratio*/
                    caluculating_hit_miss_rate(f_cache.hits, f_cache.misses);
                    /*Calling the caluculating_compulsory_capacity_miss_rate function to calculate the 
                    compulsory and capactiy miss ratio*/
                    caluculating_compulsory_capacity_miss_rate(f_cache.hits, f_cache.misses, f_cache.compulsory_misses, f_cache.capacity_misses);
                    printf("\n");
                    break;
        case 3 :
                    printf("Welcome to the N-Way Mapping Technique\n");
                    printf("Please enter the N size in N-Way Mapping\n");
                    int size;   //Value of N in N-way
                    scanf("%d", &size);
                    /* Initialization of all fields in N-Way Mapping Technique*/
                    for (int i = 0; i<NUM_BLOCKS; i++) 
                    {
                        n_cache.valid_field[i] = 0;
                        n_cache.tag_field[i] = -1;
                    }
                    n_cache.hits = 0;
                    n_cache.misses = 0;
                    n_cache.compulsory_misses = 0;
                    n_cache.capacity_misses = 0;
                    
                    /*While Loop: Copying the hexa decimal address from trace file and converting it to 
                    the binary and passing it to the N-Way cache function*/
                    while (fgets(mem_request, 20, fp) != NULL) 
                    {
                        address = convert_address(mem_request);
                        n_way_cache_access(&n_cache, address, size);
                    }
                    /*Printing the results*/
                    printf("BLOCK SIZE = %d Bytes\n", BLOCK_SIZE);
                    printf("%d-WAY\n", size);
                    printf("CACHE SIZE = %d Bytes\n", CACHE_SIZE);
                    printf("NUMBER OF BLOCKS = %d\n", NUM_BLOCKS);
                    printf("\n==================================\n");
                    printf("Cache type:    N-Way Cache\n");
                    printf("==================================\n");
                    printf("Cache Hits:         %d\n", n_cache.hits);
                    printf("Cache Misses:       %d\n", n_cache.misses);
                    printf("Compulsory Misses:  %d\n", n_cache.compulsory_misses);
                    printf("Capacity Misses:    %d\n", n_cache.capacity_misses);
                    printf("\n---Total %d-way Cache Performance---\n", size);
                    /*Calling the calculating function with hits and misses to calculate the hit and miss ratio*/
                    caluculating_hit_miss_rate(n_cache.hits, n_cache.misses);
                    /*Calling the caluculating_compulsory_capacity_miss_rate function to calculate the 
                    compulsory and capactiy miss ratio*/
                    caluculating_compulsory_capacity_miss_rate(n_cache.hits, n_cache.misses, n_cache.compulsory_misses, n_cache.capacity_misses);
                    printf("\n");
                    break;
        case 4 :    
                    printf("Welcome to the L1 & L2 cache levels");
                    /* Initialization of all fields in Two Level Cache*/
                    for (int i = 0; i<L1_NUM_BLOCKS; i++) {
		            	l1_cache.l1_valid_field[i] = 0;
		            	l1_cache.l1_tag_field[i] = -1;
		            }
		            l1_cache.l1_hits = 0;
		            l1_cache.l1_misses = 0;
                    l1_cache.l1_compulsory_misses = 0;
                    l1_cache.l1_capacity_misses = 0;

                    for (int i = 0; i<L2_NUM_BLOCKS; i++) {
		            	l2_cache.l2_valid_field[i] = 0;
		            	l2_cache.l2_tag_field[i] = -1;
		            }

		            l2_cache.l2_hits = 0;
		            l2_cache.l2_misses = 0;
                    l2_cache.l2_compulsory_misses = 0;
                    l2_cache.l2_capacity_misses = 0;

                    /*While loop: Copying the hexa decimal address from trace file and converting it to 
                    the binary and passing it to the Two Level cache function*/
		            while (fgets(mem_request, 20, fp) != NULL) 
                    {
			            address = convert_address(mem_request);
			            two_way_cache_access(&l1_cache, &l2_cache, address);
		            }

                    //Priting the results to the output
		            printf("\n==================================\n");
                    printf("Cache type:    L1-Cache\n");
                    printf("==================================\n");
                    printf("L1 BLOCK SIZE = %d Bytes\n", L1_BLOCK_SIZE);
                    printf("%d-WAY\n", L1_WAY_SIZE);
                    printf("L1 CACHE SIZE = %d Bytes\n", L1_CACHE_SIZE);
                    printf("L1 NUMBER OF BLOCKS = %d\n", L1_NUM_BLOCKS);
                    printf("Cache Hits:               %d\n", l1_cache.l1_hits);
                    printf("Cache Misses:             %d\n", l1_cache.l1_misses);
                    printf("L1 Compulsory Misses:     %d\n", l1_cache.l1_compulsory_misses);
                    printf("L1 Capacity Misses:       %d\n", l1_cache.l1_capacity_misses);
                    printf("\n---L1 Performance---\n");
                    /*Calling the calculating function with hits and misses to calculate the hit and miss ratio*/
                    caluculating_hit_miss_rate(l1_cache.l1_hits, l1_cache.l1_misses);
                    /*Calling the caluculating_compulsory_capacity_miss_rate function to calculate the 
                    compulsory and capactiy miss ratio*/
                    caluculating_compulsory_capacity_miss_rate(l1_cache.l1_hits, l1_cache.l1_misses, l1_cache.l1_compulsory_misses, l1_cache.l1_capacity_misses);

                    printf("\n==================================\n");
                    printf("Cache type:    L2-Cache\n");
                    printf("====================================\n");
                    printf("L2 BLOCK SIZE = %d Bytes\n", L2_BLOCK_SIZE);
                    printf("%d-WAY\n", L2_WAY_SIZE);
                    printf("L2 CACHE SIZE = %d Bytes\n", L2_CACHE_SIZE);
                    printf("L2 NUMBER OF BLOCKS = %d\n", L2_NUM_BLOCKS);
                    printf("Cache Hits:              %d\n", l2_cache.l2_hits);
                    printf("Cache Misses:            %d\n", l2_cache.l2_misses);
                    printf("L2 Compulsory Misses:    %d\n", l2_cache.l2_compulsory_misses);
                    printf("L2 Capacity Misses:      %d\n", l2_cache.l2_capacity_misses);
                    printf("\n---L2 Performance---\n");
                    /*Calling the calculating function with hits and misses to calculate the hit and miss ratio*/
                    caluculating_hit_miss_rate(l2_cache.l2_hits, l2_cache.l2_misses);
                    /*Calling the caluculating_compulsory_capacity_miss_rate function to calculate the 
                    compulsory and capactiy miss ratio*/
                    caluculating_compulsory_capacity_miss_rate(l2_cache.l2_hits, l2_cache.l2_misses, l2_cache.l2_compulsory_misses, l2_cache.l2_capacity_misses);

                    int total_hits_L1_and_L2 = l1_cache.l1_hits + l2_cache.l2_hits; //Calculating the total hits of L1 and L2 cache together.
                    int total_misses_L2 = l2_cache.l2_misses;   //Total misses are counted as L2 Misses
                    printf("---L1 & L2 overal performance---\n");
                    /*Calling the calculating function with L1&L2 hits and L2 misses to calculate the hit and miss ratio*/
                    caluculating_hit_miss_rate(total_hits_L1_and_L2, total_misses_L2);
                    printf("\n");

                    break;

        case 5 :
                    printf("Thank you, exiting the code!!\n"); //To break from the switch loop
                    break;
        default :
                    printf("Invalid Mapping\n" ); //Invalid option
   }

    fclose(fp); //Closing the file which we opened at the top
    return 0;
}

uint64_t convert_address(char memory_addr[])
/* Converts the physical 32-bit address in the trace file to the "binary" \\
 * (a uint64 that can have bitwise operations on it) */
{
    uint64_t binary = 0;
    int i = 0;

    while (memory_addr[i] != '\n') {
        if (memory_addr[i] <= '9' && memory_addr[i] >= '0') {
            binary = (binary*16) + (memory_addr[i] - '0');
        } else {
            if(memory_addr[i] == 'a' || memory_addr[i] == 'A') {
                binary = (binary*16) + 10;
            }
            if(memory_addr[i] == 'b' || memory_addr[i] == 'B') {
                binary = (binary*16) + 11;
            }
            if(memory_addr[i] == 'c' || memory_addr[i] == 'C') {
                binary = (binary*16) + 12;
            }
            if(memory_addr[i] == 'd' || memory_addr[i] == 'D') {
                binary = (binary*16) + 13;
            }
            if(memory_addr[i] == 'e' || memory_addr[i] == 'E') {
                binary = (binary*16) + 14;
            }
            if(memory_addr[i] == 'f' || memory_addr[i] == 'F') {
                binary = (binary*16) + 15;
            }
        }
        i++;
    }

#ifdef DBG
    printf("%s converted to %llu\n", memory_addr, binary);
#endif
    return binary;
}

void direct_mapped_cache_access(struct direct_mapped_cache *cache, uint64_t address)
{
    uint64_t block_addr = address >> (unsigned)log2(BLOCK_SIZE);
    uint64_t index = block_addr % NUM_BLOCKS;
    uint64_t tag = block_addr >> (unsigned)log2(NUM_BLOCKS);

#ifdef DBG
    printf("Memory address: %llu, Block address: %llu, Index: %llu, Tag: %llu ", address, block_addr, index, tag);
#endif

    if (cache->valid_field[index] && cache->tag_field[index] == tag) { /* Cache hit */
        cache->hits += 1;
#ifdef DBG
        printf("Hit!\n");
#endif
    } else {
        /* Cache miss */
        cache->misses += 1;
#ifdef DBG
        printf("Miss!\n");
#endif
        if (cache->valid_field[index] && cache->dirty_field[index]) {
            /* Write the cache block back to memory */
        }
        cache->tag_field[index] = tag;
        cache->valid_field[index] = 1;
        cache->dirty_field[index] = 0;
    }
}


void full_way_cache_access(struct full_way_cache_access *cache, uint64_t address)
{
	uint64_t block_addr = address >> (unsigned)log2(BLOCK_SIZE);
	srand(time(NULL));
	int index;

    //This loop is to verify the current address for an hit
    for ( index = 0; index <= NUM_BLOCKS; index++ )
    {
		// If index reaches the last line means all blocks are full and we didn't get the hit.
		if (index >= NUM_BLOCKS)
		{
			break;
		}

        //If the crrent tag field value is -1, means empty block and we didn't get the hit.
        if ( cache->tag_field[index] == -1 )
        break;

        //Looking for an hit, by comparing the block address and valid field value i.e. 1
        if (cache->valid_field[index] && cache->tag_field[index] == block_addr)
		{
            #ifdef DBG
			    printf("Hit!\n");
            #endif
			// Add Hit
			cache->hits += 1;
			return;
		}
		
    }

	//This block is for compulsory misses
	if (cache->tag_field[index] == -1 && index<NUM_BLOCKS)
	{
        #ifdef DBG
		    printf("Compulsory Miss!\n");
        #endif
		//Add Miss and Compulsory Miss
		cache->misses += 1;
        cache->compulsory_misses += 1;
		//Update Valie bit
		cache->valid_field[index] = 1;
		//Update tag field with block address
		cache->tag_field[index] = block_addr;
	}

	//This block is for capacity misses
	else if (index >= NUM_BLOCKS )
	{
		////Generate a random digit within the NUM_BLOCKS to replace a block
		int random = (rand() % NUM_BLOCKS);
        #ifdef DBG
		    printf("Capacity Miss!\n");
        #endif
		// Add Miss and Capacity Miss
		cache->misses += 1;
        cache->capacity_misses += 1;
		//Replace random line in tag and valied field with block address and 1 respectively.
		cache->tag_field[random] = block_addr;
		cache->valid_field[random] = 1;
	}
}

//Function to calcualte the log value with base 2
unsigned int log_value(unsigned int n)
{
    return (n > 1) ? 1 + log_value(n / 2) : 0;
}

void n_way_cache_access(struct n_way_cache_access *cache, uint64_t address, int n_way)
{
	int x;
	srand(time(NULL));
	//Calculating the number of sets.
	int num_of_sets = NUM_BLOCKS / n_way;

    //Calculating the block address, set_num and tag value with the address.
	uint64_t block_addr = address >> (unsigned)log2(BLOCK_SIZE);
	uint64_t set_num = block_addr % num_of_sets;
	uint64_t tag = block_addr >> log_value(num_of_sets);

    #ifdef DBG
	    printf("Memory address: %llu, Block address: %llu, Set Number: %llu, Tag: %llu ", address, block_addr, set_num, tag);
    #endif

	//This loop is to verify the current address for an hit
	for (x = 0; x < n_way; x++)
	{
        // If index reaches the last line means all blocks are full and we didn't get the hit.
		if (x >= n_way)
		{
			break;
		}

        //If the crrent tag field value is -1, means empty block and we didn't get the hit.
        if ( cache->tag_field[x + (set_num*n_way)] == -1 )
        break;

        //Looking for hit, by comparing the block address and valid field value i.e. 1
        if (cache->valid_field[x + (set_num*n_way)] && cache->tag_field[x + (set_num*n_way)] == tag)
		{
            #ifdef DBG
				printf("Hit!\n");
            #endif
            //Add Hit
			cache->hits += 1;
			return;
		}
	}
	
    //This block is for compulsory misses which means we have free blocks to place and we didn't get an hit
	if (cache->tag_field[x + (set_num*n_way)] == -1 && x < n_way)
	{
        #ifdef DBG
			printf("Compulsory Miss!\n");
        #endif
        //Add Miss and Compulsory Miss
		cache->misses += 1;
        cache->compulsory_misses += 1;
        //Update the valid and tag field with 1 and tag value respectively
		cache->valid_field[x + (set_num*n_way)] = 1;
		cache->tag_field[x + (set_num*n_way)] = tag;
		return;
	}

	//This block is for capacity misses which means all blocks are full and we didn't get an hit.
	else if ( x >= n_way )
	{
        //Generate a random digit within the n_way to replace a block 
		int random = (rand() % n_way);
        #ifdef DBG
		    printf("Capacity Miss!\n");
        #endif
        //Add Miss and Capacity Miss
		cache->misses += 1;
        cache->capacity_misses += 1;
        //Update the Valid Feild and Tag field
		cache->valid_field[random + (set_num*n_way)] = 1;
		cache->tag_field[random + (set_num*n_way)] = tag;
		return;
	}	
}

void two_way_cache_access(struct l1_way_cache_access *cache, struct l2_way_cache_access *l2cache, uint64_t address)
{
	srand(time(NULL));
    int x,y;

    //Calculating the L1 block address, Set Number and tag by using the address.
	uint64_t l1_block_addr = address >> (unsigned)log2(L1_BLOCK_SIZE);
	uint64_t L1_SET_NUM = l1_block_addr % L1_NUM_SETS;
	uint64_t l1_tag = l1_block_addr >> log_value(L1_NUM_SETS);

    #ifdef DBG
	    printf("Level 1: Memory address: %llu, Block address: %llu, Set Number: %llu, Tag: %llu ", address, l1_block_addr, L1_SET_NUM, l1_tag);
    #endif

	//This loop is to verify the current address for an hit
	for (x = 0; x < L1_WAY_SIZE; x++)
	{
        // If index reaches the last line means all blocks are full and we didn't get the hit.
		if (x >= L1_WAY_SIZE)
		{
			break;
		}
        //If the crrent tag field value is -1, means empty block and we didn't get the hit.
        if ( cache->l1_tag_field[ x + (L1_SET_NUM * L1_WAY_SIZE)] == -1 )
        break;
        //Looking for hit, by comparing the block address and valid field value i.e. 1
        if (cache->l1_valid_field[x + (L1_SET_NUM * L1_WAY_SIZE)] && cache->l1_tag_field[x + (L1_SET_NUM * L1_WAY_SIZE)] == l1_tag)
			{
            #ifdef DBG
				printf("Hit!\n");
            #endif
			cache->l1_hits += 1;
			return;
			}
	}
	
    //This block is for compulsory misses which means we have free blocks to place data and we didn't get an hit
	if (cache->l1_tag_field[x + (L1_SET_NUM * L1_WAY_SIZE)] == -1 && x < L1_WAY_SIZE)
	{
        #ifdef DBG
			printf("Miss!\n");
        #endif
        //Add L1 Miss and Compulsory Miss.
		cache->l1_misses += 1;
        cache->l1_compulsory_misses += 1;
        //Update the valis and tag field.
		cache->l1_valid_field[x + (L1_SET_NUM*L1_WAY_SIZE)] = 1;
		cache->l1_tag_field[x + (L1_SET_NUM*L1_WAY_SIZE)] = l1_tag;
        //This is L1 Miss so definitely it'll look from the Level 2, hence calling the level 2 function.
        level_two_calculation(l2cache, address);
		return;
	}

	//This block is for capacity misses which means all the blocks are full and we didn't get an hit in previous blocks.
	else if ( x >= L1_WAY_SIZE )
	{
        //Generate the random number within the L1_WAY_SIZE to replace a block
		int random = (rand() % L1_WAY_SIZE);
        #ifdef DBG
		    printf("Miss!\n");
        #endif
        //Add L1 Miss and Capacity Miss.
		cache->l1_misses += 1;
        cache->l1_capacity_misses += 1;
        //Update the valis and tag field.
		cache->l1_valid_field[random + (L1_SET_NUM*L1_WAY_SIZE)] = 1;
		cache->l1_tag_field[random + (L1_SET_NUM*L1_WAY_SIZE)] = l1_tag;
        //This is L1 Miss so definitely it'll look from the Level 2, hence calling the level 2 cache function. 
        level_two_calculation(l2cache, address);    
		return;
	}
		
}

void level_two_calculation(struct l2_way_cache_access *l2cache, uint64_t address)
{
    srand(time(NULL));
    uint64_t l2_block_addr = address >> (unsigned)log2(L2_BLOCK_SIZE);
	uint64_t L2_SET_NUM = l2_block_addr % L2_NUM_SETS;
	uint64_t l2_tag = l2_block_addr >> log_value(L2_NUM_SETS);
    int y;

    //This loop is to verify the current address for an hit
    for (y = 0; y < L2_WAY_SIZE; y++)
	{
        // If index reaches the last line means all blocks are full and we didn't get the hit.
		if (y >= L2_WAY_SIZE)
		{
		    break;
		}
        //If the crrent tag field value is -1, means empty block and we didn't get the hit.
        if (l2cache->l2_tag_field[ y + (L2_SET_NUM * L2_WAY_SIZE)] == -1 )
        break;
        //Looking for hit, by comparing the block address and valid field value i.e. 1
        if (l2cache->l2_valid_field[y + (L2_SET_NUM * L2_WAY_SIZE)] && l2cache->l2_tag_field[y + (L2_SET_NUM * L2_WAY_SIZE)] == l2_tag)
		{
            #ifdef DBG
		       	printf("Hit!\n");
            #endif
            //Add the L2 hit
			l2cache->l2_hits += 1;
			return;
		}
    }
	
    //This block is for compulsory misses which means we have enough free blocks to replace the data and we didn't get an hit in previous blocks.
	if (l2cache->l2_tag_field[y + (L2_SET_NUM * L2_WAY_SIZE)] == -1 && y < L2_WAY_SIZE)
	{
        #ifdef DBG
		    printf(" Compulsory Miss!\n");
        #endif
        //Add L2 miss and Compulsory Miss.
		l2cache->l2_misses += 1;
        l2cache->l2_compulsory_misses += 1;
        //Update the L2 valid field and Tag field.
		l2cache->l2_valid_field[y + (L2_SET_NUM*L2_WAY_SIZE)] = 1;
		l2cache->l2_tag_field[y + (L2_SET_NUM*L2_WAY_SIZE)] = l2_tag;
		return;
	}

	//This block is for capacity misses
	else if ( y >= L2_WAY_SIZE )
	{
        //Generate a Random number within L2_WAY_SIZE to replace a random block
	    int random = (rand() % L2_WAY_SIZE);
        #ifdef DBG
		    printf("Capacity Miss!\n");
        #endif
        //Add L2 Miss and Capacity Miss
		l2cache->l2_misses += 1;
        l2cache->l2_capacity_misses += 1;
        //Update the L2 valid field and Tag field.
		l2cache->l2_valid_field[random + (L2_SET_NUM*L2_WAY_SIZE)] = 1;
		l2cache->l2_tag_field[random + (L2_SET_NUM*L2_WAY_SIZE)] = l2_tag;
		return;
	}

}

void caluculating_hit_miss_rate(int hits, int misses)
{
    /*Calculating the total accesses, Hit rate, and miss rate*/
    long total_access = hits + misses;
    float hit_rate = (float)hits / total_access * 100;
    float miss_rate = (float)misses / total_access * 100;
    printf("Total Accesses:   %ld\n", total_access);
    printf("Cache Hit rate:   %.4f%\n", hit_rate);
    printf("Cache Miss rate:  %.4f%\n", miss_rate);
    printf("\n");
}
void caluculating_compulsory_capacity_miss_rate(int hits, int misses, int compulsory_misses, int capacity_misses)
{
    /*Calculating the total accesses, capacity_rate, and compulsory_rate w.r.t. to the miss rate*/
    long total_access = hits + misses;
    float miss_rate = (float)misses / total_access * 100;
    float compulsory_miss_rate = (float)compulsory_misses / misses * 100;
    float capacity_miss_rate = (float)capacity_misses / misses * 100;
    float capacity_rate = (miss_rate / 100) * capacity_miss_rate;
    float compulsory_rate = ( miss_rate / 100 ) * compulsory_miss_rate;

    printf("Cache Compulsory Miss rate:  %.4f%\n", compulsory_rate);
    printf("Cache Capacity Miss rate:   %.4f%\n", capacity_rate);
    printf("\n");
}