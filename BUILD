cc_binary(
    name = "main",
    srcs = [
        "main.cpp",
    ],
    copts = [
        "-std=c++11",
        "-g",
        "-O0",
    ],
    linkopts = [
        "-lboost_date_time",
    ],
    deps = [
        "//src/parsers/en:ENCasualDateParser",
        "//src/parsers/en:ENCasualTimeParser",
        "//src/parsers/en:ENDayOfTheWeekParser",
        "//src/parsers/en:ENDeadlineFormatParser",
        "//src/refiners:mergeDateRange",
        "//src/refiners:overlapRemoval",
    ],
)
