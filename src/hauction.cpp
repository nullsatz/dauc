#include<iostream>
#include<stdlib.h>
#include<string.h>
#include"hauction.h"

#define e 1

// each unassigned bidder finds and bids on item j that offers max value
void h_auctionBid(const int nBidders, const int nItems, float * bids,
	float * prices, int * bidderItems, bool & anyUnassigned, float * benefits)
{
	for(int bidder = 0; bidder < nBidders; bidder++) {
		if(bidderItems[bidder] != -1) continue; //unassigned?
		
		//has Unassigned person 
		anyUnassigned = true;
	
		// init the 2nd max object value with very low value
		// for the case when the person is only interested in one object
		int
			maxItem = 0;
		float
			secondMaxItemValue = -1000.0, tempItemValue = 0.0;
	
		// float maxItemValue = a[i * n] - p[0];
		int row = bidder * nItems;
		float maxItemValue = benefits[row] - prices[0];
		for(int j = 1; j < nItems; j++) {
			tempItemValue = benefits[row + j] - prices[j];
			
			//if is higher that the highest
			if(tempItemValue > maxItemValue) {
				secondMaxItemValue = maxItemValue;
	
				maxItem = j;
				maxItemValue = tempItemValue;
			} else if(tempItemValue > secondMaxItemValue) {
				//or if is higher that the second highest
				secondMaxItemValue = tempItemValue;
			}
		}
		// bidding inc from person i for favorite object
		bids[row + maxItem] = maxItemValue - secondMaxItemValue + e;
	}
}

// each thread: an item 
// for that item, look for the highest bid from unassigned bidder
void h_auctionAssign(const int nBidders, const int nItems,
	float * bids, float * prices, int * bidderItems, int * itemBidders)
{
	for(int item = 0; item < nItems; item++) {
		float
			tempBid = -1.0, highestBid = 0.0;
	
		int bidder = -1;
		
		//loop over bidders
		for(int i = 0; i < nBidders; i++) {
			tempBid = bids[i * nItems + item];
			if(tempBid > highestBid) {
					highestBid = tempBid;
					bidder = i;
			}
		}
	
		if(bidder < 0) continue;
	
		// the object j reviews the bid only if
		// bidder != currently assigned bidder
		if(itemBidders[item] == bidder) continue;
	
		//unassign the person that was previously assigned to j:
		if(itemBidders[item] >= 0)
			bidderItems[itemBidders[item]] = -1;
	
		//raise the price to the winning bid
		prices[item] = highestBid;
	
		//assign j to i
		bidderItems[bidder] = item;
		itemBidders[item] = bidder;
	}
}

// benefits[i, j] : desire of bidder i for item j

int * h_auction(int nBidders, int nItems, float * benefits) {
	size_t
		matFSize = nBidders * nItems * sizeof(float),
		itemsFSize = nItems * sizeof(float),
		itemsISize = nItems * sizeof(int),
		biddersISize = nBidders * sizeof(int);

	//bids value
	float * bids = (float *) malloc(matFSize);

	//price[j] : each item j has a price:
	float * prices = (float *) malloc(itemsFSize);

	//each bidder is (bidderItems[x] = item index)
	//	or is not assigned (bidderItems[x] = -1)
	int * bidderItems = (int *) malloc(biddersISize);

	//each object is (itemBidders[x] = bidder index)
	//	or is not assigned (itemBidders[x] = -1)
	int * itemBidders = (int *) malloc(itemsISize);

	// used as a boolean that is TRUE if there is an item not yet assigned
	bool anyUnassigned = true;

	// cleaning/initializing algoritm mem
	memset(prices, 0, itemsFSize);
	memset(bidderItems, -1, biddersISize);
	memset(itemBidders, -1, itemsISize);

	int round = 0;
	while(anyUnassigned) {
		memset(bids, 0, matFSize);
		anyUnassigned = false;

		h_auctionBid(nBidders, nItems, bids, prices, bidderItems, anyUnassigned,
			benefits);
            
		if(anyUnassigned == true) {
			h_auctionAssign(nBidders, nItems, bids, prices, bidderItems,
				itemBidders);
		}
		round++;
	}

	// Release GPU memory
    free(bids);
    free(prices);
	free(itemBidders);

	return bidderItems;
}
