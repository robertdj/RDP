
<!-- README.md is generated from README.Rmd. Please edit that file -->

# RDP

<!-- badges: start -->

[![R build
status](https://github.com/robertdj/RDP/workflows/R-CMD-check/badge.svg)](https://github.com/robertdj/RDP/actions)
[![Codecov test
coverage](https://codecov.io/gh/robertdj/RDP/branch/master/graph/badge.svg)](https://codecov.io/gh/robertdj/RDP?branch=main)
<!-- badges: end -->

The {RDP} package contains an implementation of the
[Ramer–Douglas–Peucker
algorithm](https://en.wikipedia.org/wiki/Ramer%E2%80%93Douglas%E2%80%93Peucker_algorithm)
for reducing the number of points in a curve.

There are other R packages that offer implementations of the
Ramer–Douglas–Peucker algorithm, but {RDP} has only a single dependency
and it executes pretty fast.

# Installation

{RDP} is only available on GitHub and can be installed with the [remotes
package](https://remotes.r-lib.org) using this command:

``` r
remotes::install_github("robertdj/RDP")
```

# Example

There is a single function in the {RDP} package. Here is a comparison
(using the [{bench} package](https://bench.r-lib.org)) of the execution
speed and memory usage between the {RDP} implementation and a pure R
implementation in the [kmlShape
package](https://cran.r-project.org/package=kmlShape):

``` r
x <- as.double(0:10^4)
y <- rnorm(length(x))
bench::mark(
    RDP::RamerDouglasPeucker(x, y, epsilon = 0.1),
    kmlShape::DouglasPeuckerEpsilon(x, y, epsilon = 0.1)
) %>% 
    dplyr::select(expression, min, median, mem_alloc)
#> Warning: Some expressions had a GC in every iteration; so filtering is disabled.
#> # A tibble: 2 x 4
#>   expression                                               min  median mem_alloc
#>   <bch:expr>                                           <bch:t> <bch:t> <bch:byt>
#> 1 RDP::RamerDouglasPeucker(x, y, epsilon = 0.1)         3.86ms  4.23ms    3.55MB
#> 2 kmlShape::DouglasPeuckerEpsilon(x, y, epsilon = 0.1)   5.32s   5.32s   37.67MB
```

Here {RDP} is rougly 1000 times faster and using 10 % of the memory. The
outputs are identical (according to `dplyr::all_equal`).

# Acknowledgements

The C++ code is based on [a Gist from
TimSC](https://gist.github.com/TimSC/0813573d77734bcb6f2cd2cf6cc7aa51).
At the time of implementation the code in the Gist is released under
CC0. This makes it [compatible with the
GPL](https://www.gnu.org/licenses/license-list.en.html#GPLCompatibleLicenses)
– as is required for compiled code linking against R.
