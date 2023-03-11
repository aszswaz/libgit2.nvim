local M = {}

function M.regCmd(parser)
    local cmd = parser:command("option", "libgit2 的选项")
    cmd:command("show", "打印所有选项")
    cmd:command("set", "设置所有选项，并打印所有选项的值")
end

function M.run(args)
    if not args.option then
        return
    end

    if args.show then
        M.show()
    elseif args.set then
        M.set()
    end
end

function M.show()
    -- 打印 libgit2 的默认全局选项
    print("mwindow_size:", git.get_option "mwindow_size")
    print("mwindow_mapped_limit:", git.get_option "mwindow_mapped_limit")
    print("programdata search_path:", git.get_option("search_path", git.enum.CONFIG_LEVEL_PROGRAMDATA))
    print("system search_path:", git.get_option("search_path", git.enum.CONFIG_LEVEL_SYSTEM))
    print("xdg search_path:", git.get_option("search_path", git.enum.CONFIG_LEVEL_XDG))
    print("global search_path:", git.get_option("search_path", git.enum.CONFIG_LEVEL_GLOBAL))
    local current, allowed = git.get_option "cached_memory"
    print("cached_memory, current:", current, "; allowed:", allowed)
    print("template_path:", git.get_option "template_path")
    print("user_agent:", git.get_option "user_agent")
    print("windows_sharemode:", git.get_option "windows_sharemode")
    print("pack_max_objects:", git.get_option "pack_max_objects")
    print("mwindow_file_limit:", git.get_option "mwindow_file_limit")
    local extensions = git.get_option "extensions"
    for index = 1, #extensions do
        print("extensions:", extensions[index], ";index:", index)
    end
    print("owner_validation:", git.get_option "owner_validation")
end

function M.set()
    git.set_option("mwindow_size", 10)
    git.set_option("mwindow_mapped_limit", 10)
    git.set_option("search_path", git.enum.CONFIG_LEVEL_SYSTEM, "/demo")
    git.set_option("cache_object_limit", git.enum.OBJECT_COMMIT, 1024)
    git.set_option("cache_max_size", 1024)
    git.set_option("template_path", "/demo")
    git.set_option("ssl_cert_locations", "/etc/ssl/certs/ca-bundle.crt", "/etc/ssl/certs")
    git.set_option("user_agent", "libgit2.nvim")
    git.set_option("ssl_ciphers", "demo")
    git.set_option("windows_sharemode", 10)
    git.set_option("pack_max_objects", 1024)
    git.set_option("mwindow_file_limit", 1024)
    git.set_option("odb_packed_priority", 1)
    git.set_option("odb_loose_priority", 1)
    git.set_option("extensions", { "demo", "noop" })
    git.set_option("owner_validation", true)
    git.set_option("caching", true)
    git.set_option("strict_object_creation", true)
    git.set_option("strict_symbolic_ref_creation", true)
    git.set_option("ofs_delta", true)
    git.set_option("fsync_gitdir", true)
    git.set_option("strict_hash_verification", true)
    git.set_option("unsaved_index_safety", true)
    git.set_option("http_expect_continue", true)
    git.set_option("disable_pack_keep_file_checks", true)
    M.show()
end

return M
