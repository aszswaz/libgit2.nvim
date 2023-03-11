local M = {}

function M.regCmd(parser)
    parser:command("info", "打印 libgit2 的信息")
end

function M.run(args)
    if not args.info then
        return
    end
    M.show()
end

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
