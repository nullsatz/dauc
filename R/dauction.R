dauction <- function(weights) {
	ubidders <- sort(unique(weights$bidder))
	nBidders <- length(ubidders)
	
	uitems <- sort(unique(weights$item))
	nItems <- length(uitems)
	
	weights <- weights[order(weights$bidder, weights$item), ]
	
	bidderItems <- .Call('device_auction', nBidders, nItems, weights$weight,
		PACKAGE='dauc')

	bidderItems <- ifelse(bidderItems == -1, NA, bidderItems)
	bidderItems <- bidderItems + 1
	result <- data.frame(bidder=ubidders, item=uitems[bidderItems])
	return(result)
}
