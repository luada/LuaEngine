--[[Writen by Luada

与调试信息相关的模块

Exported API:
	getCurrentCodeInf(level)
	startDebug(thread, in, out)
	stopDebug()
	doCmd(cmds, dbgInf)
	debug.counts()
	debug.showcounts()
	debug.getlocals()
--]]

function getCurrentCodeInf(level)
	--[[
	获取当前运行的代码行信息
	@param	level:堆栈层数
	@type	level: int
	@param	return：源文件 (行号) @ 函数名
	@type	return: string
	--]]
	local info = debug.getinfo(level, "nSl")
	if not info then
		return "???"
	end

	if info.what == "C" then
		return "C function"
	else
		local codeInfo = string.format("%s (%d) @ function %s",
				info.short_src, info.currentline, info.name or "")
		return codeInfo
	end
end

--------------------------------------------------------------------------------
local BreakList = nil 			---断点信息表,格式： { [filename]	= {[line]	= true}, }
local FuncVar = nil 			---局部变量表, 格式：{ [varName]	= {value, index} }
local Step = false 			---标志是否单步调试
local Next = false 		---标志是否跳过函数的单步调试
local OldDepth 	= 0
local PreSrc = ""
local EnableBreak = true

local CallbackIn = nil
local CallbackOut = nil
local CallbackFetchSrc = nil


local function DBG_OUT(...)
	--[[
	调试结果信息输出
	@param	arg:
	@type	arg:
	--]]
	local str = string.printf(...)
	---print(str)
	if CallbackOut ~= nil then
		CallbackOut(str)
	end
end


local function printVar(varName, value)
	--[[
	打印变量
	@param	varName: 变量名
	@type	varName: string
	@param	value: 变量值
	@type	value：any
	--]]
	DBG_OUT(varName .. " = " .. toString(value))
end
---------------------------------------提供给游戏客户端Console的脚本命令---------------------------------
local function getValue(var)
	--[[
	获取变量var的值
	@param	var: 变量
	@type	var：any
	@param	return: 索引为-1时，表示全局变量
	@type	return: (value, index)
	--]]
	
	---（1）找局部变量
	local value = nil
	local index = -1
	if FuncVar ~= nil and FuncVar[var]	~= nil then
		value, index = unpack(FuncVar[var])
		DEBUG_MSG(value, index)
		return value, index
	end
	
	---（2）找全局变量
	if _G[var]	~= nil then
		value =  _G[var]
	end
	DEBUG_MSG(value, index)
	return value, index
end

local function edit(var, newV)
	--[[
	修改变量值
	@param	var：变量名
	@type	var：string
	@param	newV：新值
	@type	newV：any
	--]]
	DBG_OUT("edit:", var, newV)
	
	if type(var) ~= "string" then
		return
	end
	
	local v = var
	local tmp = var
	local count = 1
	local value = nil
	local preValue = nil
	local idx = nil

	while true do
		local index = string.find(tmp,"%.")
		local strLen = string.len(tmp)
		if index then
			if count == 1 then
				v = string.sub(tmp, 1, index - 1)
				value, idx = getValue(v)
			end
			local temp = nil
			tmp = string.sub(tmp, index + 1, strLen)
			local endIndex=string.find(tmp, "%.")
			if endIndex then
				temp = string.sub(tmp, 1, endIndex - 1)
			else
				temp = tmp
			end
			if value and temp then
				preValue = value
				value = value[temp]
			end
		else
			if count == 1 then
				value, idx = getValue(var)
				if value ~= nil then
					if idx == -1 then
						_G[var]	= newV
						DBG_OUT("Set global value (", var , ") from (" ,
								 value, ") to (", newV, ")")
					else
						debug.setlocal(5, idx, newV)
						FuncVar[var] = {newV, idx}
						DBG_OUT("Set local value (", var , ") from (" ,
								 value, ") to (", newV, ")")
					end
				end
			else
				if preValue and tmp then
					preValue[tmp]	= newV
					DBG_OUT("Set ", idx == -1 and "global" or "local", " value (", var , ") from (" ,
							 value, ") to (", newV, ")")
				end
			end
			break
		end
		count = count + 1
	end
end

local function watch(var)
	--[[
	查看变量
	@param	var：变量
	@type	var：string
	--]]
	DBG_OUT("watch:", var)
	
	if type(var) ~= "string" then
		return
	end
	
	local v = var
	local tmp = var
	local count = 1
	local value = nil

	while true do
		local index = string.find(tmp,"%.")
		local strLen = string.len(tmp)
		if index then
			if count == 1 then
				v = string.sub(tmp, 1, index - 1)
				value = getValue(v)
			end
			local temp = nil
			tmp = string.sub(tmp, index + 1, strLen)
			local endIndex=string.find(tmp, "%.")
			if endIndex then
				temp = string.sub(tmp, 1, endIndex - 1)
			else
				temp = tmp
			end
			if value and temp then
				value = value[temp]
			end
		else
			if count == 1 then
				value = getValue(v)
			end
			break
		end
		count = count + 1
	end
	
	printVar(var, value)
end

local function startStep()
	--[[
	单步调试,跳到下一行
	--]]
	DBG_OUT("startStep")
	Step = true
end

local function cancelStep()
	--[[
	取消单步调试
	--]]
	DBG_OUT("cancelStep")
	Step = false
end

local function nextStep()
	--[[
	单步调试,跳到下一行
	--]]
	DBG_OUT("nextStep")
	Next = true
end

local function go()
	--[[
	运行到下一个断点
	--]]
	DBG_OUT("go")
	Step = false
end

local function infoBreak()
	--[[
	打印所有断点的信息
	--]]
	DBG_OUT("infoBreak")
	DBG_OUT("EnableBreak:" .. (EnableBreak and "true" or "false"))
	DBG_OUT("--------------begin infoBreak-----------")	
	if BreakList then
		for filename, breakLines in pairs(BreakList) do
			for line, state in pairs(breakLines) do
				DBG_OUT("file :"..filename..", line :"..line)
			end
		end
	end
	DBG_OUT("--------------end infoBreak-----------")
end

local function toggleBreak()
	--[[
	切换断点状态(打开/关闭)
	--]]
	DBG_OUT("toggleBreak")
	EnableBreak = not EnableBreak
	DBG_OUT("EnableBreak:" .. (EnableBreak and "true" or "false"))
end

local function traceback()
	--[[
	打印当前堆栈
	--]]
	DBG_OUT(debug.traceback("traceback", 5))
end

local function removeBreak(filename, line)
	--[[
	移除断点
	(1) filename和line都为nil时，删除所有断点
	(2) 仅line为nil时，删除filename下的所有断点
	(3) filename和line都不为nil时，删除filename下的第line行的断点
	@param	filename：文件名
	@type	filename：string
	@param	line：行数
	@type	line: int
	--]]
	DBG_OUT("removeBreak", filename, "@", line)
	if not filename then
		if not line then
			BreakList = nil
		end
	else
		if not line then
			if (BreakList) then
				BreakList[filename]	= nil 
			end
		else
			if (BreakList) and BreakList[filename]	then
				BreakList[filename][line]	= nil
			end
		end
	end
end

local function addBreak(filename, line)
	--[[
	增加断点
	@param	filename：文件名
	@type	filename：string
	@param	line：行数
	@type	line: int
	--]]
	if type(filename) ~= "string" or type(line) ~= "number" then
		DBG_OUT("can not addBreak for ", filename, "@", line)
		return
	end
	
	DBG_OUT("addBreak", filename, "@", line)
	
	if not (BreakList) then
		BreakList = {}
	end

	if (BreakList[filename]) then
		BreakList[filename][line]	= true
	else
		BreakList[filename]	= {[line]	= true}
	end
end

local function fillVarsList()
	--[[
	填充当前断点处环境的局部变量
	--]]
	local index = 1
	FuncVar = {}
	while true do
		local name, value = debug.getlocal(4, index)
		if name then
			FuncVar[name]	= {value, index}
			index = index + 1
		else
			break
		end
	end
end

local function doBuff(code)
	--[[
	执行代码
	@param	code：
	@type	code：string
	@param	return：执行状态和结果
	@type	return：(status, result)
	--]]
	DBG_OUT("doBuff")
	
	local strCode = string.format("do local _=%s return _ end", code)
	setLuaDumpFlag(0)
	local status, ret = pcall(loadstring(strCode))
	setLuaDumpFlag(1)
	DBG_OUT(status, ret)
end

local function previewSrc()
	--[[
	预览上一次执行的代码
	--]]
	DBG_OUT("previewSrc")
	DBG_OUT(PreSrc)
end

local function help()
	--[[
	调试指令帮助说明
	--]]
	DBG_OUT("help")
	local str = "[s] start \'step into state\'\n";
	str = str .."[b (filename) (line)] add break point\n"
	str = str .."[r (filename | nil) (line | nil)] remove break point\n"
	str = str .."[i] information for break points\n"
	str = str .."[tb] toggle enable/disable break points\n"
	str = str .."[tr] print the information of calling stacks\n"
	str = str .."[e (var NewValue)] edit a value\n"
	str = str .."[c] cancel \'step into state\'\n"
	str = str .."[n] next step (for step over state)\n"
	str = str .."[g] go (until next break point)\n"
	str = str .."[w (var)] watch a value\n"
	str = str .."[p] preview source\n"
	str = str .."[d code] do buffer\n"
	str = str .."[h] help\n"
	DBG_OUT(str)
end

--------------------------------------------------------------------------------
local command =
{
	["s"]	= startStep,
	["b"]	= addBreak,
	["r"]	= removeBreak,
	["i"]	= infoBreak,
	["tb"]	= toggleBreak,
	["tr"]	= traceback,
	["e"]	= edit,
	["c"]	= cancelStep,
	["n"]	= nextStep,
	["g"]	= go,
	["w"]	= watch,
	["d"]	= doBuff,
	["p"]	= previewSrc,
	["h"]	= help,
}

local fetchCmds = nil

local stopCommand =
{
	["s"]	= true,
	["c"]	= true,
	["g"]	= true,
	["n"]	= true,
}

local NotNeedCompileParam =
{
	["d"]	= true,
}

local function interupt(filename, line)
	--[[
	中断函数
	--]]
	line = line and line or 0
	DBG_OUT(filename.. ":" .. line..": breaking now!")
	
	PreSrc = ""
	if CallbackFetchSrc ~= nil then
		local src = CallbackFetchSrc(filename, line)
		if src ~= nil or src ~= "" then
			PreSrc = src
			DBG_OUT(src)
		end
	end
	
	fillVarsList() ---保存中断处的变量信息
	OldDepth = getTracebackCount() - 2
	
	if Step == true then
		Step = false
	end

	if Next == true then
		Next = false
	end
	
	---中断,直到从控制台获得输入command
	DBG_OUT("debug>")
	while CallbackIn ~= nil do 
		sleep(500)
		local cmdstr = CallbackIn()
		local cmd, ret = doCmd(fetchCmds or command, cmdstr)
		if stopCommand[cmd]	== true then
			break
		end
	end
end

local function onDbgHookTrace(event, line)
	--[[
	对应每行的钩子函数
	@param	event: describe the event that has triggered its call: "call", 
					"return" (or "tail return"), "line", and "count". 
	@type	event: string
	@param	line:
	@type	line: int
	--]]
	local filename = debug.getinfo(2).short_src ---返回的函数所在的文件名字

	filename = string.sub(filename, 10, -3)
	---单步调试
	if Step == true then
		interupt(filename, line)
		return
	end

	---跳过函数的单步调试
	if Next == true then
		local curDepth = getTracebackCount() - 1
		if curDepth <= OldDepth then
			interupt(filename, line)
		end
	end

	---断点调试
	if  EnableBreak and BreakList then
		for name, breakLines in pairs(BreakList) do
			for lineNum, state in pairs(breakLines) do
				if name == filename and line == lineNum then
					interupt(filename, lineNum)
				end
			end
		end
	end
end


function startDebug(thread, callbackIn, callbackOut, callbackFetchSrc, cmds)
	--[[
	进入调试状态
	@param	thread:
	@type	thread: handle to coroutine or nil
	@param	callbackIn: 调试协议输入回调函数
	@type	callbackIn: function
	@param	callbackOut: 调试结果输出回调函数
	@type	callbackOut: function
	@param	callbackFetchSrc: 获取源代码的回调函数
	@type	callbackFetchSrc: function
	@param	cmds: 额外的指令集合
	@type	cmds: table
	--]]
	---"l": The hook is called every time Lua enters a new line of code.
	CallbackIn = callbackIn
	CallbackOut = callbackOut
	CallbackFetchSrc = callbackFetchSrc
	
	DBG_OUT("debug start!")
	
	fetchCmds = cmds
	for k, v in pairs(command) do
		cmds[k]	= v
	end
	
	if thread ~= nil then
		debug.sethook(thread, onDbgHookTrace, "l")
	else
		debug.sethook(onDbgHookTrace, "l")
	end
end

function stopDebug(thread, cmds)
	--[[
	关闭调试状态
	@param	thread:
	@type	thread: handle to coroutine or nil
	@param	cmds: 额外的指令集合
	@type	cmds: table
	--]]
	if thread ~= nil then
		debug.sethook(thread)
	else
		debug.sethook()
	end
	
	fetchCmds = nil
	for k, v in pairs(command) do
		cmds[k]	= nil
	end
	
	DBG_OUT("debug stop!")
	CallbackIn = nil
	CallbackOut = nil
	CallbackFetchSrc = nil
end

function doCmd(cmds, dbgInf)
	--[[
	执行命令
	@param	cmds：命令表
	@type	cmds：table{cmdKey = functor}
	@param	dbgInf: 命令
	@type	dbgInf: string
	@param	return: 执行命令和执行结果
	@type	return：string, boolean
	--]]
	if string.len(dbgInf) == 0 then
		return nil, false
	end
	
	local count = 1
	local cmd = nil
	local paramlist = {}
	for w in string.gmatch(dbgInf, "[%w/.\\-{}]+") do
		if count == 1  then
			cmd = w
		else
			if NotNeedCompileParam[cmd] then
				paramlist[count - 1] = string.sub(dbgInf, string.find(dbgInf, cmd) + string.len(cmd))
				break
			else
				local strCode = string.format("do local _=%s return _ end", w)
				setLuaDumpFlag(0)
				local status, ret = pcall(loadstring(strCode))
				setLuaDumpFlag(1)
				paramlist[count - 1] = status and ret or w
			end
		end
		count = count + 1
	end
	if (cmds[cmd]) then
		cmds[cmd](unpack(paramlist))
		return cmd, true
	else
		DBG_OUT("unsupport cmd:", dbgInf, "\ntype \'h\' for help")
	end
	return cmd, false
end

--------------------------------------------------------------------------------
local Counters = {}
local Names = {}

local function onDbgHookFuncCall()
	--[[
	进入新函数时的回调函数
	--]]
    local f = debug.getinfo(3, "f").func
    if Counters[f]	== nil then --- first time `f' is called?
       Counters[f]	= 1
       Names[f]	= debug.getinfo(3, "Sn")
    else   --- only increment the counter
       Counters[f]	= Counters[f]	+ 1
    end
end

local function getFuncName(func)
	--[[
	获取函数的名字
	@param	func：
	@type	func：
	@param	return：
	@type	return：string
	--]]
    local n = Names[func]
    if n.what == "C" then
       return n.name
    end
    local loc = string.format("[%s]:%s",
           n.short_src, n.linedefined)
    if n.namewhat ~= "" then
       return string.format("%s (%s)", loc, n.name)
    else
       return string.format("%s", loc)
    end
end

function debug.counts()
	--[[
	开始计算整个lua state 每个函数被调用数量
	--]]
	---"c": The hook is called every time Lua calls a function;
	debug.sethook(onDbgHookFuncCall, "c")
end

function debug.showcounts()
	--[[Set
	显示函数名和调用次数，并停止每个函数被调用数量的统计
	--]]
	DBG_OUT("list functoins and stacks:")
	for func, count in pairs(Counters) do
	    DBG_OUT(getFuncName(func), count)
	end
	debug.sethook()
end

function debug.getlocals()
	--[[
	取得自己函数的所有内部变量名和value
	@param	return: 
	@type	return: table{ label = value }
	--]]
	local locals={}

    --- try local variables
    local i = 1
    while true do
       local n, v = debug.getlocal(2, i)
       if not n then break end
       locals[n]=v
       i = i + 1
    end

    --- try upvalues
    local func = debug.getinfo(2).func
    i = 1
    while true do
       local n, v = debug.getupvalue(func, i)
       if not n then break end
       locals[n]=v
       i = i + 1
    end
    return locals
end