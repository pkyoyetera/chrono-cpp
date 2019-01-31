load("//:boost.bzl", "boost")

cc_binary(
    name = "time",
    srcs = glob(
        [
            "time.cpp",
            "src/**/*.hpp",
            "src/**/*.cpp",
        ],
    ),
    copts = [
        "-std=c++17",
        "-g",
        "-O0",
    ],
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
    copts = [
        "-std=c++17",
        "-g",
        "-O0",
    ],
    #includes = ["src"],
    visibility = ["//visibility:public"],
    deps = [
        "@boost//:date_time",
    ],
)
