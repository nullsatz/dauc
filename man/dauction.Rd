\name{dauction}
\alias{dauction}
\title{Perform an auction algorithm on a GPU}

\description{
	This function performs, with the aid of a GPU, an auction algorithm.
}

\usage{
dauction(weights)
}

\arguments{
	\item{weights}{a data frame with three columns: two character columns for
		storing ids, weights$bidder and weights$item; a numeric column, 
		weights$weight listing the weight for the respective bidder and item.}
}

\value{a data frame with three columns: two character columns for
	storing ids, bidder and item; a boolean column, matched listing whether
	the respective bidder and item have been matched up.
}

\examples{
# make a data frame of random weights with bidder and item ids 1 to 5
bidders <- 1:3
items <- letters[1:5]
weights <- expand.grid(bidder=bidders, item=items)
weights$weight <- runif(length(bidders) * length(items))

# try to make some matches on the gpu
result <- dauction(weights)
print(result)
}
