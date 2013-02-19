// system and standard lib includes
#include<vector>
#include<algorithm>
#include<sys/time.h>

// R includes
#include<Rcpp.h>
using namespace Rcpp;

// local includes
#include"auction.h"
#include"hauction.h"

std::vector<float> & convertWeights(NumericVector & weights) {
	std::vector<float> * fWeights = new std::vector<float>(weights.size());
	std::copy(weights.begin(), weights.end(), fWeights->begin());
	return *fWeights;
}

void wrapAssignments(int nBidders, int * assignments,
	IntegerVector& outAssignments)
{
	std::copy(assignments, assignments + nBidders, outAssignments.begin());
}

double timems(timeval start, timeval stop) {
	long long int microseconds = 1000000 * (stop.tv_sec - start.tv_sec)
		+ (stop.tv_usec - start.tv_usec);
	double ms = ((double) (microseconds)) / 1000.0;
	return ms;
}

// weights should be arranged first by bidder then by item
// rows: bidders; cols: items
RcppExport SEXP device_auction(SEXP _nBidders, SEXP _nItems, SEXP _weights) {
	IntegerVector
		vNBidders(_nBidders), vNItems(_nItems);
	int
		nBidders = vNBidders[0], nItems = vNItems[0];
	NumericVector weights(_weights);

	std::vector<float> benefits = convertWeights(weights);

	timeval start;
	gettimeofday(&start, NULL);

	int * assi = d_auction(nBidders, nItems, benefits.data());

	timeval stop;
	gettimeofday(&stop, NULL);

	Rcout << "device auction took " << timems(start, stop) << " ms"
		<< std::endl;

	IntegerVector outAssi(nBidders);
	wrapAssignments(nBidders, assi, outAssi);
	return outAssi;
}

RcppExport SEXP host_auction(SEXP _nBidders, SEXP _nItems, SEXP _weights) {
	IntegerVector
		vNBidders(_nBidders), vNItems(_nItems);
	int
		nBidders = vNBidders[0], nItems = vNItems[0];
	NumericVector weights(_weights);
	std::vector<float> benefits = convertWeights(weights);

	timeval start;
	gettimeofday(&start, NULL);

	int * assi = h_auction(nBidders, nItems, benefits.data());

	timeval stop;
	gettimeofday(&stop, NULL);

	Rcout << "host auction took " << timems(start, stop) << " ms"
		<< std::endl;

	IntegerVector outAssi(nBidders);
	wrapAssignments(nBidders, assi, outAssi);
	return outAssi;
}
