workspace(name = "time")

# =====================================================================
# Boost
# =====================================================================
load("@time//:boost.bzl", "boost")

boost()

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")

boost_deps()

# =====================================================================
# Googletest
# =====================================================================
new_git_repository(
    name = "googletest",
    build_file = "gmock.BUILD",
    remote = "https://github.com/google/googletest",
    tag = "release-1.8.1",
)
