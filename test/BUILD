cc_test(
    name = "Result__test",
    srcs = [
        "gtest_main.cc",
        "test_result.cpp",
        # "//src/parsers/en/ENCasualTimeParser.cc",
    ],
    args = [
        "--test_verbose_timeout_warnings",
    ],
    copts = [
        "-std=c++14",
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
        "ENCasualTest.cpp",
        "gtest_main.cc",
    ],
    args = [
        "--test_verbose_timeout_warnings",
    ],
    copts = [
        "-std=c++14",
    ],
    linkopts = [
        "-lstdc++",
    ],
    deps = [
        "//src:result",
        "//src/parsers/en:ENCasualTimeParser",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "entryPointTest",
    srcs = [
        "gtest_main.cc",
        "test_MainEntryPointFile.cpp",
    ],
    args = [
        "--test_verbose_timeout_warnings",
    ],
    copts = [
        "-std=c++14",
    ],
    deps = [
        "//src/parsers/en:ENCasualTimeParser",
        "@googletest//:gtest_main",
    ],
)
