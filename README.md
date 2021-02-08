# RDP

<!-- badges: start -->
[![R build status](https://github.com/robertdj/RDP/workflows/R-CMD-check/badge.svg)](https://github.com/robertdj/RDP/actions)
[![Codecov test coverage](https://codecov.io/gh/robertdj/RDP/branch/master/graph/badge.svg)](https://codecov.io/gh/robertdj/RDP?branch=main)
<!-- badges: end -->

The {RDP} package contains an implementation of the [Ramer–Douglas–Peucker algorithm
](https://en.wikipedia.org/wiki/Ramer%E2%80%93Douglas%E2%80%93Peucker_algorithm) for reducing the number of points in a curve.

There are already a number of R packages with similar implementations.
However, these packages offer much more functionality and have a large number of dependencies.
My aim is to have minimal dependencies and a single purpose.


## Installation

The package is only available on GitHub and can be installed with the [remotes package](https://remotes.r-lib.org) using this command:

``` r
remotes::install_github("robertdj/RDP")
```


# Acknowledgements

The C++ code is based on [a Gist from TimSC](https://gist.github.com/TimSC/0813573d77734bcb6f2cd2cf6cc7aa51).
At the time of implementation the code in the Gist is released under CC0. 
I believe that this makes it okay to release the {RDP} package under the GPL license -- as is required for compiled code linking against R.
