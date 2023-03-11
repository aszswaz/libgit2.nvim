local M = {}

function M.regCmd(parser)
    local cmd = parser:command("status", "获取存储库的文件状态信息")
end

function M.run(args)
    if not args.status then
        return
    end
    M.view(args.repo)
end

-- 查看文件状态
function M.view(repo)
    repo = git.repository.open(repo)
    --[[
    local indexHandle = git.repository.index(repo)
    local treeOid = git.index.write_tree(indexHandle)
    git.index.write(indexHandle)
    local treeHandle = git.tree.lookup(repo, treeOid)
    --]]

    -- 获取文件状态的列表
    local statusList = git.status.list_new(repo, {
        version = git.enum.STATUS_OPTIONS_VERSION,
        show = git.enum.STATUS_SHOW_INDEX_AND_WORKDIR,
    })
    -- 打印所有文件状态
    print("status list size:", #statusList)
    for index = 1, #statusList do
        print(statusList[index])
    end
end

return M
