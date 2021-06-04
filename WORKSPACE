workspace(name = "chrono")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

# =====================================================================
#    Boost
# =====================================================================
load("@chrono//:boost.bzl", "boost")
boost()

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()

# =====================================================================
#    Googletest
# =====================================================================
git_repository(
    name = "googletest",
    remote = "https://github.com/google/googletest",
    tag = "release-1.8.1",
)

# =====================================================================
#    Google RE2
# =====================================================================
git_repository(
    name = "re2",
    remote = "https://github.com/google/re2",
    tag = "2021-06-01",
)
