
<!-- README.md is generated from README.Rmd. Please edit that file -->

# RDP

<!-- badges: start -->

[![CRAN
status](https://www.r-pkg.org/badges/version/RDP)](https://CRAN.R-project.org/package=RDP)
[![R build
status](https://github.com/robertdj/RDP/workflows/R-CMD-check/badge.svg)](https://github.com/robertdj/RDP/actions)
[![Codecov test
coverage](https://codecov.io/gh/robertdj/RDP/branch/main/graph/badge.svg)](https://codecov.io/gh/robertdj/RDP)
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

There is a single function in the {RDP} package. An example from the
Wikipedia page linked to above with a description of the algorithm. The
original line is the black and the approximating line is red.

``` r
x <- seq(from = 0, to = 5, by = 0.01)
y <- exp(-x) * cos(2 * pi * x)
plot(x, y, type = "l")

lines(RDP::RamerDouglasPeucker(x, y, 0.06), col = "red", lty = 2)
```

<img src="man/figures/README-example-1.png" width="100%" />

# Performance

Here is a comparison of the execution speed and memory usage of the
{RDP} implementation and a pure R implementation from the [{kmlShape}
package](https://cran.r-project.org/package=kmlShape):

``` r
x <- as.double(1:10^4)
y <- rnorm(length(x))
bench::mark(
    RDP = RDP::RamerDouglasPeucker(x, y, epsilon = 0.1),
    kmlShape = kmlShape::DouglasPeuckerEpsilon(x, y, epsilon = 0.1)
)
#> Warning: Some expressions had a GC in every iteration; so filtering is disabled.
#> # A tibble: 2 x 6
#>   expression      min   median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr> <bch:tm> <bch:tm>     <dbl> <bch:byt>    <dbl>
#> 1 RDP        822.57µs  938.5µs   945.       5.28MB     4.00
#> 2 kmlShape      5.11s    5.11s     0.196   35.74MB    13.5
```

In this example we see from the [{bench}
package](https://bench.r-lib.org) summary that {RDP} is several 1000
times faster and only use a fraction of the memory.

# Acknowledgements

The C++ code was *originally* based on [a Gist from
TimSC](https://gist.github.com/TimSC/0813573d77734bcb6f2cd2cf6cc7aa51),
but over time it has been almost completely rewritten.
