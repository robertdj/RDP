test_that("Small RamerDouglasPeucker example", {
    expect_equal(
        RamerDouglasPeucker(1:3, 1:3, 0.5),
        data.frame(x = c(1, 3), y = c(1, 3))
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


test_that("Error when x and y have different lengths", {
    expect_error(RamerDouglasPeucker(1:3, 1:4, 5), class = "std::invalid_argument")
})


test_that("Epsilon should be positive", {
    expect_error(RamerDouglasPeucker(1:3, 1:3, -1), class = "std::domain_error")
    expect_error(RamerDouglasPeucker(1:3, 1:3, NA_real_), class = "std::domain_error")
})


test_that("Error with NA in coordinates", {
    expect_error(RamerDouglasPeucker(c(1, NA_real_, 3), 1:3, 1), class = "std::invalid_argument")
    expect_error(RamerDouglasPeucker(1:3, c(1, NA_real_, 3), 1), class = "std::invalid_argument")
})
