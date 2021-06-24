
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

{RDP} is [available on CRAN](https://cran.r-project.org/package=RDP) and
can be installed with the command

``` r
install.packages("RDP")
```

{RDP} can also be installed from GitHub with the [{remotes}
package](https://remotes.r-lib.org) using this command:

``` r
remotes::install_github("robertdj/RDP")
```

# Example

There is a single function in the {RDP} package. Here is a comparison of
the execution speed and memory usage of the {RDP} implementation and a
pure R implementation from the [{kmlShape}
package](https://cran.r-project.org/package=kmlShape):

``` r
x <- as.double(0:10^4)
y <- rnorm(length(x))
bench::mark(
    RDP = RDP::RamerDouglasPeucker(x, y, epsilon = 0.1),
    kmlShape = kmlShape::DouglasPeuckerEpsilon(x, y, epsilon = 0.1)
)
#> Warning: Some expressions had a GC in every iteration; so filtering is disabled.
#> # A tibble: 2 x 6
#>   expression      min   median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr> <bch:tm> <bch:tm>     <dbl> <bch:byt>    <dbl>
#> 1 RDP          3.96ms   4.32ms   206.        5.3MB      0  
#> 2 kmlShape      5.44s    5.44s     0.184    35.7MB     12.5
```

In this example we see from the [{bench}
package](https://bench.r-lib.org) summary that {RDP} is more than 1000
times faster and using 10 % of the memory.

# Acknowledgements

The C++ code is based on [a Gist from
TimSC](https://gist.github.com/TimSC/0813573d77734bcb6f2cd2cf6cc7aa51).
At the time of implementation the code in the Gist is released under
CC0. This makes it [compatible with the
GPL](https://www.gnu.org/licenses/license-list.en.html#GPLCompatibleLicenses)
– as is required for compiled code linking against R.
