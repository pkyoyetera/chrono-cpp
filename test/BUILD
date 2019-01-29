cc_test(
    name = "resultTest",
    srcs = [
        "gtest_main.cc",
        "result.test/test_result.cpp",
    ],
    args = [
        "--test_verbose_timeout_warnings",
        "--verbose_failures",
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
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "ENCasualTimeTest",
    srcs = [
        "gtest_main.cc",
        "parsers.test/ENCasualTimeParser.test.cpp",
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
    ],
)

cc_test(
    name = "ENCasualDateTest",
    srcs = [
        "gtest_main.cc",
        "parsers.test/ENCasualDateParser.test.cpp",
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
    ],
)

cc_test(
    name = "ENDeadlineFormatTest",
    srcs = [
        "gtest_main.cc",
        "parsers.test/ENDeadlineFormatParser.test.cpp",
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
        "//src/parsers/en:ENDeadlineFormatParser",
        "//src/utils:utilities",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "overlapRemovalTest",
    srcs = [
        "gtest_main.cc",
        "refiners.test/OverlapRemovalRefiner.test.cpp",
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
        "//src/refiners:overlapRemoval",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "ENDayoftheweek",
    srcs = [
        "gtest_main.cc",
        "parsers.test/ENDayOfTheWeekParser.test.cpp",
    ],
    args = [
        "--verbose_failures",
        "--compilation_mode=dbg -s",
    ],
    linkopts = [
        "-lstdc++",
        "-lboost_date_time",
    ],
    deps = [
        "//src:result",
        "//src/parsers/en:ENDayOfTheWeekParser",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "ENMonthNameMiddleEndian",
    srcs = [
        "gtest_main.cc",
        "parsers.test/ENMonthNameMiddleEndianParser.test.cpp",
    ],
    args = [
        "--verbose_failures",
        "--compilation_mode=dbg -s",
    ],
    copts = [
        "-std=c++14",
        "-O0",
    ],
    linkopts = [
        "-lstdc++",
        "-lboost_date_time",
    ],
    deps = [
        "//src:result",
        "//src/parsers/en:ENMonthNameMiddleEndianParser",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "ENMonthName",
    srcs = [
        "gtest_main.cc",
        "parsers.test/ENMonthNameParser.test.cpp",
    ],
    args = [
        "--verbose_failures",
        "--compilation_mode=dbg -s",
    ],
    copts = [
        "-std=c++14",
        "-O0",
    ],
    linkopts = [
        "-lstdc++",
        "-lboost_date_time",
    ],
    deps = [
        "//src:result",
        "//src/parsers/en:ENMonthNameParser",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "ENTimeLater",
    srcs = [
        "gtest_main.cc",
        "parsers.test/ENTimeLaterParser.test.cpp",
    ],
    args = [
        "--verbose_failures",
        "--compilation_mode=dbg -s",
    ],
    copts = [
        "-std=c++14",
        "-O0",
    ],
    linkopts = [
        "-lstdc++",
        "-lboost_date_time",
    ],
    deps = [
        "//src:result",
        "//src/parsers/en:ENTimeLaterParser",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "ENTimeAgo",
    srcs = [
        "gtest_main.cc",
        "parsers.test/ENTimeAgoFormat.test.cpp",
    ],
    args = [
        "--verbose_failures",
        "--compilation_mode=dbg -s",
    ],
    copts = [
        "-std=c++14",
        "-O0",
    ],
    linkopts = [
        "-lstdc++",
        "-lboost_date_time",
    ],
    deps = [
        "//src:result",
        "//src/parsers/en:ENTimeAgoParser",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "ENISO",
    srcs = [
        "gtest_main.cc",
        "parsers.test/ENISOFormatParser.test.cpp",
    ],
    args = [
        "--verbose_failures",
        "--compilation_mode=dbg -s",
    ],
    copts = [
        "-std=c++14",
        "-O0",
    ],
    linkopts = [
        "-lstdc++",
        "-lboost_date_time",
    ],
    deps = [
        "//src:result",
        "//src/parsers/en:ENISOParser",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "ENTimeExpression",
    srcs = [
        "gtest_main.cc",
        "parsers.test/ENTimeExpressionParser.test.cpp",
    ],
    args = [
        "--verbose_failures",
        "--compilation_mode=dbg -s",
    ],
    copts = [
        "-std=c++14",
        "-O0",
    ],
    linkopts = [
        "-lstdc++",
        "-lboost_date_time",
    ],
    deps = [
        "//src:result",
        "//src/parsers/en:ENTimeExpression",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "ExtractTimeZone",
    srcs = [
        "gtest_main.cc",
        "refiners.test/ExtractTimeZoneAbbrev.test.cpp",
    ],
    args = [
        "--verbose_failures",
        "--compilation_mode=dbg -s",
    ],
    copts = [
        "-std=c++14",
        "-O0",
    ],
    linkopts = [
        "-lstdc++",
        "-lboost_date_time",
    ],
    deps = [
        "//src:result",
        "//src/parsers/en:ENTimeExpression",
        "//src/refiners:extractTimeZoneAbbreviation",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "US_holidays",
    srcs = [
        "gtest_main.cc",
        "parsers.test/ENUSHolidaysParser.test.cpp",
    ],
    args = [
        "--verbose_failures",
        "--compilation_mode=dbg -s",
    ],
    copts = [
        "-std=c++14",
        "-O0",
    ],
    linkopts = [
        "-lstdc++",
        "-lboost_date_time",
    ],
    deps = [
        "//src:result",
        "//src/parsers/en:ENUSHolidays",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "weekExp",
    srcs = [
        "gtest_main.cc",
        "parsers.test/ENWeekExperessionParser.test.cpp",
    ],
    args = [
        "--verbose_failures",
        "--compilation_mode=dbg -s",
    ],
    copts = [
        "-std=c++14",
        "-O0",
    ],
    linkopts = [
        "-lstdc++",
        "-lboost_date_time",
    ],
    deps = [
        "//src:result",
        "//src/parsers/en:ENWeekExpression",
        "@googletest//:gtest_main",
    ],
)
