load("//:boost.bzl", "boost")
load("//:chrono.bzl", "copts", "linkopts")

COPTS = copts()
LINKOPTS = linkopts()

cc_library(
    name = "libtime",
    srcs = glob(
        ["src/**/*.cpp"],
    ),
    hdrs = glob(
        ["src/**/*.hpp"],
    ),
    copts = COPTS,
    visibility = ["//visibility:public"],
    deps = [
        "@boost//:algorithm",
        "@boost//:date_time",
    ],
)

cc_binary(
    name = "time",
    srcs = glob([
        "time.cpp",
    ]),
    copts = COPTS,
    deps = [
        "//:libtime",
    ],
)
