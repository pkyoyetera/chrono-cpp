load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

def boost():
    if "com_github_nelhage_rules_boost" not in native.existing_rules():
        git_repository(
            name = "com_github_nelhage_rules_boost",
            commit = "2bfb03f0fc0de6d39bfe27271d406c0b81d6dcdd",
            remote = "https://github.com/nelhage/rules_boost",
        )

    # Still needs:
    # load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
    # boost_deps()
