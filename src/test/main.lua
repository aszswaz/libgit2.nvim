-- 在 lua 库搜索路径中添加脚本的所在目录
local scriptPath = arg[0]:match ".*/"
package.cpath = package.cpath .. ";" .. scriptPath .. "?.so"
package.path = package.path .. ";" .. scriptPath .. "?.lua"

if not git then
    git = require "libgit2"
end

local mInfo = require "info"
local mOption = require "option"
local mStatus = require "status"
local mRemote = require "remote"
local mRepoistory = require "repoistory"
local mIndex = require "index"
local mCommit = require "commit"

local argparse = require "argparse"
local lfs = require "lfs"

local M = {}

local parser = argparse("libgit2-nvim-test", "libgit2-nvim 的测试脚本")
parser:option("-C --repo", "存储库的路径，默认是工作目录")

parser:command("info", "打印 libgit2 的信息")

local cOption = parser:command("option", "libgit2 的选项")
cOption:command("show", "打印所有选项")
cOption:command("set", "设置所有选项，并打印所有选项的值")

local init = parser:command("init", "初始化存储库")
init:flag("--bare", "创建裸存储库")
local status = parser:command("status", "获取存储库的文件状态信息")
local add = parser:command("add", "把指定的文件添加到存储库")
add:argument("file", "需要添加到存储库的文件")
local commit = parser:command("commit", "记录对存储库的更改")
commit:option("-m --message", "提交信息")

local remote = parser:command("remote", "管理存储库的远程仓库")
local list = remote:command("list", "列出所有远程仓库")
local remoteCreate = remote:command("create", "创建远程仓库")
remoteCreate:argument("name", "远程仓库的名称")
remoteCreate:argument("url", "远程仓库的地址")
list:flag("-v --verbose", "显示远程仓库的地址")

local push = parser:command("push", "推送到远程仓库")

local args = parser:parse()

local repo = args.repo
if not repo then
    repo = lfs.currentdir()
end

git.init()
if args.info then
    mInfo.show()
elseif args.option then
    if args.show then
        mOption.show()
    elseif args.set then
        mOption.set()
    end
elseif args.init then
    mRepoistory.init(repo, args.bare)
elseif args.status then
    mStatus.view(repo)
elseif args.add then
    mIndex.add(repo, args.file)
elseif args.commit then
    mCommit.commit(repo, args.message)
elseif args.remote then
    if args.create then
        mRemote.create(repo, args.name, args.url)
    end
    if args.list then
        mRemote.list(repo, args.verbose)
    end
elseif args.push then
    mRemote.push(repo)
end
git.shutdown()
