cc_test(
    name = "Result__test",
    srcs = [
        "gtest_main.cc",
        "test_result.cpp",
        # "//src/parsers/en/ENCasualTimeParser.cc",
    ],
    args = [
        "--test_verbose_timeout_warnings",
        "--verbose_failures",
    ],
    copts = [
        "-std=c++14",
        "-lboost_date_time",
    ],
    deps = [
        "//src:result",
        "//src/parsers/en:ENCasualTimeParser",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "ENCasualTest",
    srcs = [
        "test__ENCasualTimeParser.cpp",
        "gtest_main.cc",
    ],
    args = [
        "--verbose_failures",
        "--test_verbose_timeout_warnings",
    ],
    copts = [
        "-std=c++14",
    ],
    linkopts = [
        "-lstdc++",
        "-lboost_date_time",
    ],
    deps = [
        "//src:result",
        "//src/parsers/en:ENCasualTimeParser",
        "//src/utils:utilities",
        "@googletest//:gtest_main",
        #"@boost//:date_time"
    ],
)

cc_test(
    name = "ENCasualDate__test",
    srcs = [
        "gtest_main.cc",
        "test__ENCasualDateParser.cpp",
    ],
    args = [
        "--verbose_failures",
        "--test_verbose_timeout_warnings",
    ],
    copts = [
        "-std=c++14",
    ],
    linkopts = [
        "-lstdc++",
        "-lboost_date_time",
    ],
    deps = [
        "//src:result",
        "//src/parsers/en:ENCasualDateParser",
        "//src/utils:utilities",
        "@googletest//:gtest_main",
        #"@boost//:date_time"
    ],
)

cc_test(
    name = "entryPointTest",
    srcs = [
        "gtest_main.cc",
        "test_MainEntryPointFile.cpp",
    ],
    args = [
        "--verbose_failures",
        "--test_verbose_timeout_warnings",
    ],
    copts = [
        "-std=c++14",
        "-lboost_date_time",
    ],
    deps = [
        "//src/parsers/en:ENCasualTimeParser",
        "@googletest//:gtest_main",
    ],
)
