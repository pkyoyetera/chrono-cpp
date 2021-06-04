load("//:chrono.bzl", "copts", "linkopts")

COPTS = copts()
LINKOPTS = linkopts()

cc_library(
    name = "test-common",
    srcs = [
        "gtest_main.cc",
    ],
    copts = COPTS,
    deps = [
        "//:libtime",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "resultTest",
    srcs = [
        "result.test/test_result.cpp",
    ],
    deps = [
        "test-common",
    ],
)

cc_test(
    name = "ENCasualTimeTest",
    srcs = [
        "parsers.test/ENCasualTimeParser.test.cpp",
    ],
    deps = [
        "test-common",
    ],
)

cc_test(
    name = "ENCasualDateTest",
    srcs = [
        "parsers.test/ENCasualDateParser.test.cpp",
    ],
    deps = [
        "test-common",
    ],
)

cc_test(
    name = "ENDeadlineFormatTest",
    srcs = [
        "parsers.test/ENDeadlineFormatParser.test.cpp",
    ],
    deps = [
        "test-common",
    ],
)

cc_test(
    name = "overlapRemovalTest",
    srcs = [
        "refiners.test/OverlapRemovalRefiner.test.cpp",
    ],
    deps = [
        "test-common",
    ],
)

cc_test(
    name = "ENDayoftheweek",
    srcs = [
        "parsers.test/ENDayOfTheWeekParser.test.cpp",
    ],
    deps = [
        "test-common",
    ],
)

cc_test(
    name = "ENMonthNameMiddleEndian",
    srcs = [
        "parsers.test/ENMonthNameMiddleEndianParser.test.cpp",
    ],
    deps = [
        "test-common",
    ],
)

cc_test(
    name = "ENMonthName",
    srcs = [
        "parsers.test/ENMonthNameParser.test.cpp",
    ],
    deps = [
        "test-common",
    ],
)

cc_test(
    name = "ENTimeLater",
    srcs = [
        "parsers.test/ENTimeLaterParser.test.cpp",
    ],
    deps = [
        "test-common",
    ],
)

cc_test(
    name = "ENTimeAgo",
    srcs = [
        "parsers.test/ENTimeAgoFormat.test.cpp",
    ],
    deps = [
        "test-common",
    ],
)

cc_test(
    name = "ENISO",
    srcs = [
        "parsers.test/ENISOFormatParser.test.cpp",
    ],
    deps = [
        "test-common",
    ],
)

cc_test(
    name = "ENTimeExpression",
    srcs = [
        "parsers.test/ENTimeExpressionParser.test.cpp",
    ],
    deps = [
        "test-common",
    ],
)

cc_test(
    name = "ExtractTimeZone",
    srcs = [
        "refiners.test/ExtractTimeZoneAbbrev.test.cpp",
    ],
    deps = [
        "test-common",
    ],
)

cc_test(
    name = "US_holidays",
    srcs = [
        "parsers.test/ENUSHolidaysParser.test.cpp",
    ],
    deps = [
        "test-common",
    ],
)

cc_test(
    name = "weekExp",
    srcs = [
        "parsers.test/ENWeekExperessionParser.test.cpp",
    ],
    deps = [
        "test-common",
    ],
)

cc_test(
    name = "relativeFormat",
    srcs = [
        "parsers.test/ENRelativeFormat.test.cpp",
    ],
    deps = [
        "test-common",
    ],
)
