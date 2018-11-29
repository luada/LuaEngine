--[[Created by Luada
游戏逻辑调试器
--]]

require "base.debug"
require "base.singleton"
require "common.functor"

--------------------------------------------------------------------------------
Debugger = class(Singleton)

function Debugger:__init()
	self._commands = 
	{
		["/s"] = functor(Debugger._startDbg, self),
		["/e"] = functor(Debugger._stopDbg, self),
		["/h"] = functor(Debugger._help, self)
	}
	self._run = false
	self._dbgIn = functor(Debugger._onSynIn, self)
	self._dbgOut = functor(Debugger._onDbgOut, self)
	self._fetchSrc = functor(Debugger._onFetchSrc, self)
	self._src = {}
end

function Debugger:__release()
	for _, v in pairs(self._commands) do
		v:release()
	end
	self._commands = nil
	self._dbgIn:release()
	self._dbgIn = nil
	self._dbgOut:release()
	self._dbgOut = nil
	self._fetchSrc:release()
	self._fetchSrc = nil
	self._src = nil
end

function Debugger:_startDbg()
	--[[
	启动调试器
	--]]
	if not self._run then
		self._run = true
		startDebug(nil, self._dbgIn, self._dbgOut, self._fetchSrc, self._commands)
		logger:addHook(self._dbgOut)
		DEBUG_MSG("Debugger:_startDbg")
	else
		DEBUG_MSG("Debugger is already running!")
	end
end

function Debugger:_stopDbg()
	--[[
	停止调试器
	--]]
	if self._run then
		DEBUG_MSG("Debugger:_stopDbg")
		self._run = false
		self._src = {}
		stopDebug(nil, self._commands)
		logger:removeHook(self._dbgOut)
	else
		DEBUG_MSG("Debugger is already stopping !")
	end
end

function Debugger:_help()
	--[[
	调试器的帮助说明
	--]]
	DEBUG_MSG("\'/s\' to start Dbg\n\'/e\' to stop Dbg\n\'/h\' for help")
end

function Debugger:_onSynIn()
	--[[
	调试协议获取
	--]]
	return game:ReadDbgInput()
end

function Debugger:_onDbgOut(result)
	--[[
	输出调试结果
	@param	result：调试结果
	@type	result：string
	--]]
	return game:WriteDbgOutput(result)
end

function Debugger:_onFetchSrc(filename, line)
	--[[
	获取源码
	@param	filename：源文件名
	@type	filename：string
	@param	line：行号
	@type	line：int
	@param	return：返回line行附近的代码
	@type	return：string
	--]]
	local src = self._src[filename]
	if src == nil then
		src = loadRes(filename)
		self._src[filename] = src
	end
	
	if src == nil then
		return nil
	end
	
	local sLine = line - 5
	if sLine < 1 then
		sLine = 1
	end
	
	local eLine = line + 5
	local curLine = 1
	local sChar = -1
	while curLine < sLine do
		sChar = string.find(src, "\n", sChar + 1)
		if sChar == nil then
			return nil
		end
		curLine = curLine + 1
	end
	
	local retSrc = ""
	local eChar = 0
	while curLine <= eLine and eChar ~= nil do
		eChar = string.find(src, "\n", sChar + 1)
		local curStr = string.sub(src, sChar + 1, eChar)
		if curLine == line then
			retSrc = retSrc .. "--> " .. curStr
		else
			retSrc = retSrc .. "    " .. curStr
		end
		sChar = eChar
		curLine = curLine + 1
	end
	return retSrc
end

function Debugger:onDebug(dbgInf)
	--[[
	调试协议处理
	@param	dbgInf: 调试协议
	@type	dbgInf: string
	@param	return: 协议处理结果
	@type	return: string
	--]]
	local cmd, ret = doCmd(self._commands, dbgInf)
	return not ret and string.printf("unsupport cmd:", dbgInf) or ""
end
