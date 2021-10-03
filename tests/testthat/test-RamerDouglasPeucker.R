library(zeallot)

test_that("All points are kept when epsilon is zero", {
    n <- 20
    c(x, y) %<-% withr::with_seed(1, list(x = sort(runif(n)), y = rnorm(n)))

    expect_equal(RamerDouglasPeucker(x, y, 0), data.frame(x = x, y = y))
})


test_that("Only endpoints are kept when all points are on a line", {
    n <- 10
    c(x, a, b) %<-% withr::with_seed(1, list(x = sort(runif(n)), a = rnorm(1), b = rnorm(1)))
    y <- a * x + b

    expect_equal(
        RamerDouglasPeucker(x, y, 0.1),
        data.frame(x = c(x[1], x[n]), y = c(y[1], y[n]))
    )
})


test_that("Only endspoints are kept when all points are equal", {
    expect_equal(
        RamerDouglasPeucker(rep(0, 5), rep(0, 5), 1),
        data.frame(x = rep(0, 2), y = rep(0, 2))
    )
})


test_that("Varying epsilon", {
    x <- c(0, 1, 3, 5)
    y <- c(2, 1, 0, 1)

    expect_equal(
        RamerDouglasPeucker(x, y, 0.5),
        data.frame(x = c(0, 3, 5), y = c(2, 0, 1))
    )

    expect_equal(
        RamerDouglasPeucker(x, y, 0.1),
        data.frame(x = c(0, 1, 3, 5), y = c(2, 1, 0, 1))
    )
})


test_that("Vertical segments", {
    x <- c(10, 15, 15, 20)
    y <- c(35, 34, 30, 29)

    expect_equal(
        RamerDouglasPeucker(x, y, 10),
        data.frame(x = c(10, 20), y = c(35, 29))
    )
})


test_that("Horizontal segments", {
    x <- c(10, 15, 16, 21)
    y <- c(35, 35, 30, 30)

    expect_equal(
        RamerDouglasPeucker(x, y, 1),
        data.frame(x = c(10, 15, 16, 21), y = c(35, 35, 30, 30))
    )
})


test_that("Bigger example", {
    x <- c(3.5, 7.3, 23.2, 37.2, 54.6, 62.2, 71.5, 101.3)
    y <- c(21.25, 12.0, 3.1, 12.07, 18.15, 16.45, 9.7, 21.1)

    expect_equal(
        RamerDouglasPeucker(x, y, 5),
        data.frame(x = c(3.5, 23.2, 54.6, 71.5, 101.3), y = c(21.25, 3.1, 18.15, 9.7, 21.1))
    )
})


test_that("Input returned when there are too few points", {
    expect_equal(RamerDouglasPeucker(c(1, 2), c(1, 2), 5), data.frame(x = c(1, 2), y = c(1, 2)))

    expect_equal(RamerDouglasPeucker(1, 1, 5), data.frame(x = 1, y = 1))

    expect_equal(
        RamerDouglasPeucker(numeric(0), numeric(0), 5),
        data.frame(x = numeric(0), y = numeric(0))
    )
})


test_that("Error when x and y have different lengths", {
    expect_error(RamerDouglasPeucker(1:3, 1:4, 5), class = "std::invalid_argument")
})


test_that("Epsilon should be a positive number", {
    expect_error(RamerDouglasPeucker(1:3, 1:3, -1), class = "std::domain_error")
    expect_error(RamerDouglasPeucker(1:3, 1:3, NA_real_), class = "std::domain_error")
})


test_that("Error with NA in coordinates", {
    expect_error(RamerDouglasPeucker(c(1, NA_real_, 3), 1:3, 1), class = "std::invalid_argument")
    expect_error(RamerDouglasPeucker(1:3, c(1, NA_real_, 3), 1), class = "std::invalid_argument")
})
