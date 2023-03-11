local M = {}

function M.regCmd(parser)
    local cmd = parser:command("status", "获取存储库的文件状态信息")
end

function M.run(args)
    if not args.status then
        return
    end
    M.view(args.repo, args)
end

-- 查看文件状态
function M.view(repo, args)
    repo = git.repository.open(repo)

    local opt = {
        version = git.enum.STATUS_OPTIONS_VERSION,
        show = git.enum.STATUS_SHOW_INDEX_AND_WORKDIR,
        flags = {
            include_untracked = true,
            renames_head_to_index = true,
            sort_case_sensitively = true,
        },
        -- pathspec = { "demo04" },
    }

    local enum = git.enum
    -- 获取文件状态的列表
    local statusList = git.status.list_new(repo, opt)
    -- 打印所有文件状态
    print("status list size:", #statusList)
    for index = 1, #statusList do
        local iterm = statusList[index]
        local status = nil

        if iterm.status == enum.STATUS_INDEX_NEW or iterm.status == enum.STATUS_WT_NEW then
            status = "new file"
        elseif iterm.status == enum.STATUS_INDEX_MODIFIED or iterm.status == enum.STATUS_WT_MODIFIED then
            status = "modified"
        elseif iterm.status == enum.STATUS_INDEX_DELETED or iterm.status == enum.STATUS_WT_DELETED then
            status = "deleted"
        elseif iterm.status == enum.STATUS_INDEX_RENAMED or iterm.status == enum.STATUS_WT_RENAMED then
            status = "renamed"
        elseif iterm.status == enum.STATUS_INDEX_TYPECHANGE or iterm.status == enum.STATUS_WT_TYPECHANGE then
            status = "typechange"
        else
            goto continue
        end

        local index = iterm.head_to_index and iterm.head_to_index or iterm.index_to_workdir
        if index.new_file.path ~= index.old_file.path then
           print(status, ": ", index.old_file.path, "->", index.new_file.path)
        else
            print(status, ":", index.new_file.path)
        end
        ::continue::
    end
end

return M
