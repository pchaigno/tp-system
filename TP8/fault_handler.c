/*
 * fault_handler.c
 *
 *  Created on: 1 nov. 2008
 *      Author: lescouarnecn
 */
#include "mmu.h"
#include "physical_memory.h"
#include "swap.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

// Page fault handler
//     Free one page if needed
//     Get free page
//     Load
//     Update MMU

enum state_page {SWAPPED, ALLOCATED, NOTMAPPED};

#define PRECLEAN

struct page_table_entry{
	enum state_page status;
	page_phys pp;
};

// Page table
struct page_table_entry* page_table;
int page_table_size;

// List of allocated pages
page_virt* allocated_pages;
int allocated_pages_capacity;
int allocated_pages_size;
int allocated_pages_first;




void init_faultHandler(int nb_pages_virt, int nb_pages_phys) {
	int i;
	page_table = (struct page_table_entry*)malloc(sizeof(struct page_table_entry)*nb_pages_virt);
	page_table_size = nb_pages_virt;
	for(i=0; i<nb_pages_virt; i++) {
		page_table[i].pp = INVALID;
		page_table[i].status = NOTMAPPED;
	}

	allocated_pages = (page_virt*)malloc(sizeof(page_virt)*nb_pages_phys);
	allocated_pages_capacity = nb_pages_phys;
	for(i=0; i<nb_pages_phys; i++) {
		allocated_pages[i]=INVALID;
	}
}

void unloadAPage();
void loadAPage(page_virt pv, page_phys pp);

void page_fault_handler(page_virt pv) {
	// Get a Free block
	if(pm_isFull()){
		printf("PF: Unload a page\n");
		unloadAPage();
	}
	page_phys pp = pm_getFreePage();

	// Mark page as loaded
	printf("PF: Load a page (%d)\n",pv);
	loadAPage(pv, pp);

	// Read from Swap
	if(page_table[pv].status == SWAPPED){
		swap_read(pv,pp);
	}

	// Update page table
	page_table[pv].pp=pp;
	page_table[pv].status=ALLOCATED;

	// Write to MMU
	mmu_addTranslation(pv,pp);

	// That's all
}

#ifdef POLICY_FIFO
void unloadAPage() {
	int i;
	if(mmu_isDirty(allocated_pages[0])) {
		swap_write(allocated_pages[0], page_table[allocated_pages[0]].pp);
		mmu_clearDirtyBit(allocated_pages[pageNum]);
	}
	mmu_clearAccessedBit(allocated_pages[pageNum]);
	mmu_invalidatePage(allocated_pages[pageNum]);
	page_table[allocated_pages[0]].status = SWAPPED;
	pm_freePage(page_table[allocated_pages[0]].pp);

	for(i=0; i<allocated_pages_size-1; i++) {
		allocated_pages[i] = allocated_pages[i+1];
	}
	allocated_pages_size--;
}

void loadAPage(page_virt pv, page_phys pp) {
	if(allocated_pages_size < allocated_pages_capacity){
		allocated_pages[allocated_pages_size] = pv;
		allocated_pages_size++;
	} else {
		printf("Maximum capacity reached (%d).\n", allocated_pages_capacity);
	}
}
#endif

#ifdef POLICY_RANDOM
void unloadAPage() {
	int pageNum = (rand()%allocated_pages_size);
	if(mmu_isDirty(allocated_pages[pageNum])) {
		swap_write(allocated_pages[pageNum], page_table[allocated_pages[pageNum]].pp);
		mmu_clearDirtyBit(allocated_pages[pageNum]);
	}
	mmu_clearAccessedBit(allocated_pages[pageNum]);
	mmu_invalidatePage(allocated_pages[pageNum]);
	page_table[allocated_pages[pageNum]].status = SWAPPED;
	pm_freePage(page_table[allocated_pages[pageNum]].pp);

	allocated_pages[pageNum] = allocated_pages[allocated_pages_size-1];
	allocated_pages_size--;
}

void loadAPage(page_virt pv, page_phys pp) {
	if(allocated_pages_size < allocated_pages_capacity){
		allocated_pages[allocated_pages_size] = pv;
		allocated_pages_size++;
	} else {
		printf("Maximum capacity reached (%d).\n", allocated_pages_capacity);
	}
}
#endif

#ifdef POLICY_LRU
void unloadAPage() {
	int pageNum = -1;
	int i;
	for(i=0; i<allocated_pages_size; i++) {
		if(pageNum == -1) {
			if(mmu_isAccessed(allocated_pages[i])) {
				pageNum = i;
			}
		} else {
			mmu_clearAccessedBit(allocated_pages[i]);
		}
	}

	if(mmu_isDirty(allocated_pages[pageNum])) {
		swap_write(allocated_pages[pageNum], page_table[allocated_pages[pageNum]].pp);
		mmu_clearDirtyBit(allocated_pages[pageNum]);
	}
	mmu_clearAccessedBit(allocated_pages[pageNum]);
	mmu_invalidatePage(allocated_pages[pageNum]);
	page_table[allocated_pages[pageNum]].status = SWAPPED;
	pm_freePage(page_table[allocated_pages[pageNum]].pp);

	allocated_pages[pageNum] = allocated_pages[allocated_pages_size-1];
	allocated_pages_size--;
}

void loadAPage(page_virt pv, page_phys pp) {
	if(allocated_pages_size < allocated_pages_capacity){
		allocated_pages[allocated_pages_size] = pv;
		allocated_pages_size++;
	} else {
		printf("Maximum capacity reached (%d).\n", allocated_pages_capacity);
	}
}
#endif

