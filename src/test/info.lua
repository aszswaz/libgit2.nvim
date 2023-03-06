local M = {}

function M.show()
    -- 打印 libgit2 的信息
    local version = git.version()
    print("libgit2 version:", version.major .. "." .. version.minor .. "." .. version.revision)
    print("prerelease:", git.prerelease())
    print("GIT_PUSH_OPTIONS_VERSION:", git.enum.PUSH_OPTIONS_VERSION)
    local features = git.features()
    print("threads:", features.threads)
    print("https:", features.https)
    print("ssh:", features.ssh)
    print("nsec:", features.nsec)
end

return M
