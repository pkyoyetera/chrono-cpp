cc_binary(
    name = "main",
    srcs = [
        "main.cpp",
    ],
    copts = [
        "-std=c++11",
        "-g",
        "-O2",
        #"-I/usr/include/boost",
        #"-L/usr/include/boost",
    ],
    linkopts = [
        "-lboost_date_time",
    ],
    deps = [
        "//src/parsers/en:ENCasualTimeParser",
    ],
)
