load("//:boost.bzl", "boost")
load("//:chrono.bzl", "copts", "linkopts")

COPTS = copts()
LINKOPTS = linkopts()

cc_binary(
    name = "time",
    srcs = glob(
        [
            "time.cpp",
            "src/**/*.hpp",
            "src/**/*.cpp",
        ],
    ),
    copts = COPTS,
    includes = ["src"],
    deps = [
        "@boost//:date_time",
    ],
)

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
        "@boost//:date_time",
    ],
)
