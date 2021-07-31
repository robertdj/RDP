#' `RDP`  package
#'
#' Implementation of the [Ramer-Douglas-Peucker algorithm](https://en.wikipedia.org/wiki/Ramer-Douglas-Peucker_algorithm).
#'
#' @references
#' Urs Ramer (1972), "An iterative procedure for the polygonal approximation of plane curves".
#' *Computer Graphics and Image Processing* **1**, 244--256.
#' \doi{10.1016/S0146-664X(72)80017-0}.
#'
#' David H. Douglas and Thomas K. Peucker (1973), "Algorithms for the Reduction of the Number of Points Required to Represent a Digitized Line or its Caricature".
#' *Cartographica* **10**, 112--122.
#' \doi{10.3138/FM57-6770-U75U-7727}.
#'
#' @docType package
#' @name RDP-package
"_PACKAGE"

## usethis namespace: start
#' @useDynLib RDP, .registration = TRUE
## usethis namespace: end
NULL

## usethis namespace: start
#' @importFrom Rcpp sourceCpp
## usethis namespace: end
NULL
