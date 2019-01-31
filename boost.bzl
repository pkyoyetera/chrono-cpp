def boost():
    if "com_github_nelhage_rules_boost" not in native.existing_rules():
        native.git_repository(
            name = "com_github_nelhage_rules_boost",
            commit = "2bfb03f0fc0de6d39bfe27271d406c0b81d6dcdd",
            remote = "https://github.com/nelhage/rules_boost",
        )
