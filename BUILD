cc_binary(
    name = "main",
    srcs = [
        "main.cpp",
    ],
    copts = [
        "-std=c++11",
        "-g",
        "-O0",
        "-w",  #no-non-virtual-dto",
    ],
    linkopts = [
        "-lboost_date_time",
    ],
    deps = [
        "//src/parsers/en:ENCasualDateParser",
        "//src/parsers/en:ENCasualTimeParser",
        "//src/parsers/en:ENDayOfTheWeekParser",
        "//src/parsers/en:ENDeadlineFormatParser",
        "//src/parsers/en:ENISOParser",
        "//src/parsers/en:ENMonthNameMiddleEndianParser",
        "//src/parsers/en:ENMonthNameParser",
        "//src/parsers/en:ENTimeAgoParser",
        "//src/parsers/en:ENTimeExpression",
        "//src/parsers/en:ENTimeLaterParser",
        "//src/parsers/en:ENUSHolidays",
        "//src/parsers/en:ENWeekExpression",
        "//src/refiners:extractTimeZoneAbbreviation",
        "//src/refiners:mergeDateAndTime",
        "//src/refiners:mergeDateRange",
        "//src/refiners:overlapRemoval",
    ],
)
