cc_library(
    name = "refiner",
    hdrs = [
        "refiner.hpp",
    ],
    visibility = [
        "//visibility:public",
    ],
    deps = [
        "//src/parsers",
        "//src/utils:utilities",
    ],
)

cc_library(
    name = "overlapRemoval",
    hdrs = [
        "OverlapRemovalRefiner.hpp",
    ],
    visibility = [
        "//visibility:public",
    ],
    deps = [
        "//src/refiners:refiner",
    ],
)

cc_library(
    name = "mergeDateRange",
    hdrs = [
        "en/ENMergeDateRangeRefiner.hpp",
    ],
    visibility = [
        "//visibility:public",
    ],
    deps = [
        "//src/refiners:refiner",
    ],
)

cc_library(
    name = "extractTimeZoneAbbreviation",
    hdrs = [
        "ExtractTimeZoneAbbreviation.hpp",
    ],
    visibility = [
        "//visibility:public",
    ],
    deps = [
        "//src/refiners:refiner",
    ],
)

cc_library(
    name = "mergeDateAndTime",
    hdrs = [
        "en/ENMergeDateAndTimeRefiner.hpp",
    ],
    visibility = [
        "//visibility:public",
    ],
    deps = [
        "//src/refiners:refiner",
    ],
)
