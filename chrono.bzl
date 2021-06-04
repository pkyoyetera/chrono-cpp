def default_settings():
    native.config_setting(
        name = "C++17",
        values = {
            "define": "C++=17",
        },
        visibility = ["//visibility:public"],
    )

    native.config_setting(
        name = "C++2a",
        values = {
            "define": "C++=2a",
        },
        visibility = ["//visibility:public"],
    )

    native.config_setting(
        name = "dbg_mode",
        values = {
            "compilation_mode": "dbg",
        },
        visibility = ["//visibility:public"],
    )

    native.config_setting(
        name = "opt_mode",
        values = {
            "compilation_mode": "opt",
        },
        visibility = ["//visibility:public"],
    )

    native.config_setting(
        name = "fastbuild_mode",
        values = {
            "compilation_mode": "fastbuild",
        },
        visibility = ["//visibility:public"],
    )

def copts(stdc = "-std=c++17", O = "02"):
    default_settings()
    return select({
        "//:C++17": ["-std=c++17"],
        "//:C++2a": ["-std=c++2a"],
        "//conditions:default": [stdc],
    }) + select({
        "//:dbg_mode": ["-Wall", "-Wextra", "-Wpedantic"],
        "//:fastbuild_mode": ["-g1"],
        "//:opt_mode": ["-DNDEBUG", O],
        "//conditions:default": []
    })

def linkopts():
    return select({
        "//:dbg_mode": ["-g", "-rdynamic"],
        "//:fastbuild_mode": ["-g1", "-rdynamic"],
        "//:opt_mode": ["-rdynamic"],
        "//conditions:default": []
    })
