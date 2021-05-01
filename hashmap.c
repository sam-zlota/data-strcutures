
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// TODO: sudo apt install libbsd-dev
// This provides strlcpy
// See "man strlcpy"
#include <bsd/string.h>
#include <string.h>

#include "hashmap.h"


int
hash(char* key)
{
    // TODO: Produce an appropriate hash value.

	int hash = 0;
	int offset = 0;
	
	while(key[offset] != 0) {
	    hash += ( 67 *(offset + 1) * key[offset]);



	    offset++;
	}

    
	return hash;




}

hashmap*
make_hashmap_presize(int nn)
{

	hashmap* hh = calloc(1, sizeof(hashmap));
 
    // TODO: Allocate and initialize a hashmap with capacity 'nn'.
    // Double check "man calloc" to see what that function does.
 
	hh->size = 0;
	hh->capacity = nn;
	hh->data = (hashmap_pair*) calloc(nn, sizeof(hashmap_pair));


	for(int i = 0; i < nn; i++) {
		hashmap_pair* pp = &hh->data[i];
		pp->tomb = false;
		pp->used = false;
	}

	return hh;
}

hashmap*
make_hashmap()
{
	return make_hashmap_presize(4);
}

void
free_hashmap(hashmap* hh)
{
    // TODO: Free all allocated data.
 
	free(hh->data);
	free(hh);  
}


int
hashmap_has(hashmap* hh, char* kk)
{
    return hashmap_get(hh, kk) != -1;
}

int
hashmap_get(hashmap* hh, char* kk)
{
    // TODO: Return the value associated with the
    // key kk.
    // Note: return -1 for key not found.

	int hash_val = hash(kk) % hh->capacity;

	hashmap_pair* pp = &hh->data[hash_val];
	
	int offset = 0;
	while (offset + hash_val < hh->capacity - 1) {

	 	if((!pp[offset].used && !pp[offset].tomb)

			       || (pp[offset].tomb && !pp[offset+1].used)){

			//not used and not tomb
			//tomb and next not used
			return -1;
		}

		if(!pp[offset].tomb && pp[offset].used 
			&& (strcmp(pp[offset].key, kk) == 0)) {
			//not tomb, used, and not collision
			return pp[offset].val;

		}
		

		offset +=1;
	}

	//edge case, last entry in hashmap
	hashmap_pair* last_entry = &hh->data[hh->capacity - 1];
	if((last_entry->used && !last_entry->tomb) &&
		(strcmp(last_entry->key, kk) == 0)) {
			return last_entry->val;
		
	}

	return -1;
	
}


void
hashmap_resize(hashmap* hh) {

	int old_cap = hh->capacity;
	//resize double
	hh->capacity =  old_cap * 2;

	hashmap_pair* data_temp = hh->data;


	hh->data = calloc(hh->capacity, sizeof(hashmap_pair));
	hh->size = 0;

	for(int i = 0; i < old_cap; i ++) {

		hashmap_pair* pp = &data_temp[i];

		if(pp->used) {
			hashmap_put(hh, pp->key, pp->val);
		}	

	}

	free(data_temp);

}



void
hashmap_put(hashmap* hh, char* kk, int vv)
{
    // TODO: Insert the value 'vv' into the hashmap
    // for the key 'kk', replacing any existing value
    // for that key.

   
	int nn = hh->size;

	int cc = hh->capacity;	

	if( (4* nn) / cc > 2) {


		//load factor > 0.5
		hashmap_resize(hh);
	}


	int hash_val = hash(kk) % hh->capacity;
	hashmap_pair* pp = &hh->data[hash_val];
	
	int offset = 0;
	
	while(offset + hash_val < hh-> capacity ) {

		if(!pp[offset].used) {
			//pp[offset].key = *kk;
			strcpy(pp[offset].key, kk);
			pp[offset].val = vv;
			pp[offset].used = true;
			pp[offset].tomb = false;
			hh->size = hh->size + 1;
			break;

		}



		
		if(pp[offset].used && strcmp(kk, pp[offset].key) == 0) {

			pp[offset].val = vv;
			hh->size = hh->size + 1;
			break;
		}

		offset++;
	}

}

void
hashmap_del(hashmap* hh, char* kk)
{
    // TODO: Remove any value associated with
    // this key in the map.
  
	int hash_val = hash(kk) % hh->capacity;

	hashmap_pair* pp = &hh->data[hash_val];

	int offset = 0;

	while(offset + hash_val < hh->capacity ) {
	
		if(strcmp(pp[offset].key, kk) == 0) {	
			pp[offset].tomb = true;
			pp[offset].used = false;
		//	pp[offset].val = 0;
			hh->size = hh->size - 1;
			break;
		}
		offset++;
	}
}


hashmap_pair
hashmap_get_pair(hashmap* hh, int ii)
{
    // TODO: Get the {k,v} pair stored in index 'ii'.
    return hh->data[ii];
}

void
hashmap_dump(hashmap* hh)
{
    printf("== hashmap dump ==\n");
    // TODO: Print out all the keys and values currently
    // in the map, in storage order. Useful for debugging.
    //

    printf("cap: %ld", hh->capacity);
    printf("load_factor: %ld",(10 * hh->size) / hh->capacity);

    for(int ii = 0; ii < hh->capacity; ii++) {
	    hashmap_pair* pp = &hh->data[ii];
	    if(pp->used) {
		    int hash_val = hash(pp->key) % hh->capacity;
		    printf("%ld: %s  => %ld\n", ii, pp->key, pp->val);
		    printf(" \"%s\" hashes to %ld\n", pp->key, hash_val);
		    printf("index: %ld difference of %ld \n",
				    ii, ii - hash_val);
	    }




    }
}
