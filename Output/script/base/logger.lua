--[[Writen by Luada

日志输出模块

Exported API:
	DEBUG_MSG(...)
	INFO_MSG(...)
	WARN_MSG(...)
	ERROR_MSG(...)
	FATAL_MSG(...)

	output(str)
	print(...)
--]]

require "base.string"
require "base.debug"

function DEBUG_MSG(...)
	--[[
	调试消息的输出
	--]]
	logger:debug(...)
end

function INFO_MSG(...)
	--[[
	跟踪程序运行进度
	--]]
	logger:info(...)
end

function WARN_MSG(...)
	--[[
	程序运行时发生异常
	--]]
	logger:warn(...)
end

function ERROR_MSG(...)
	--[[
	程序运行时发生可预料的错误,此时通过错误处理,可以让程序恢复正常运行
	--]]
	logger:error(...)
end

function FATAL_MSG(...)
	--[[
	程序运行时发生不可预料的严重错误,一般将终止程序运行
	--]]
	logger:fatal(...)
end

--------------------------------------------------------------------------------
local MAX_STRING_LENGTH = 512
local _print = print

function output(str)
	--[[
	把字符串按最大长度拆分输出
	@param	str: 要输出的字符串
	@type	str：string
	--]]
	if str == nil then
		_print(nil)
	elseif #str > 0 then
		local idx = 1
		local substr = str
		while #substr > 0 do
			local s = string.substr(substr, 1, MAX_STRING_LENGTH)
			_print(s)
			substr = string.substr(substr, #s+1)
		end
	else
		_print()
	end
end

--------------------------------------------------------------------------------
function print(...)
	--[[
	重定义print函数，避免出现参数与输出格式不匹配的情况
	--]]
	local str = string.printf(...)
	output(str)
end
--------------------------------------------------------------------------------

-- 关闭所有消息输出
local OFF = "OFF"

-- 允许所有等级消息输出
local ALL = "ALL"

-- 用于调试消息的输出
local DEBUG = "DEBUG"

-- 用于跟踪程序运行进度
local INFO = "INFO"

-- 程序运行时发生异常
local WARN = "WARN"

-- 程序运行时发生可预料的错误,此时通过错误处理,可以让程序恢复正常运行
local ERROR = "ERROR"

-- 程序运行时发生不可预料的严重错误,一般将终止程序运行
local FATAL = "FATAL"
--------------------------------------------------------------------------------
local LEVEL = {
	[DEBUG] = 1,
	[INFO]  = 2,
	[WARN]  = 3,
	[ERROR] = 4,
	[FATAL] = 5,

	[ALL]   = 0,
	[OFF]   = 100,
}

--------------------------------------------------------------------------------
local function logSetLevel(logger, level)
	--[[
	设置日志输出的等级
	@param	logger：Logger产生的实例
	@type	logger：Object
	@param	level：日志输出等级
	@type	level：int
	--]]
	assert(LEVEL[level], string.format("undefined level `%s'", tostring(level)))
	logger.level = level
end
--------------------------------------------------------------------------------
local function logOutput(logger, level, codeInfo, ...)
	--[[
	日志输出处理函数
	@param	logger：Logger产生的实例
	@type	logger：Object
	@param	level：日志输出等级
	@type	level：string
	@param	codeInfo：调用日志函数（XX_MSG)所在的文件名，函数，行号
	@type	codeInfo：string
	@param	arg：XX_MSG的函数参数
	@type	arg：参数列表
	--]]
	local str = string.printf(os.date(), "   ", level, ":", ...)
	for _, hook in ipairs(logger.hooks) do
		hook(str)
		hook(codeInfo)
		hook("\n")
	end
end
--------------------------------------------------------------------------------
local function logAddHook(logger, hook)
	--[[
	添加日志输出挂接函数
	@param	logger: Logger产生的实例
	@type	logger: Object of Logger
	@param	hook: 输出挂接函数
	@type	hook: functor
	--]]
	table.insert(logger.hooks, hook)
end
--------------------------------------------------------------------------------
local function logRemoveHook(logger, hook)
	--[[
	删除日志输出挂接函数
	@param	logger: Logger产生的实例
	@type	logger: Object of Logger
	@param	hook: 输出挂接函数
	@type	hook: functor
	--]]
	table.rmArrayValue(logger.hooks, hook)
end
--------------------------------------------------------------------------------
local function logMsg(logger, level, ...)
	--[[
	分派日志信息
	@param	logger：Logger产生的实例
	@type	logger：Object
	@param	level：日志输出等级
	@type	level：int
	--]]
	---assert(LEVEL[level], string.format("undefined level `%s'", tostring(level)))
	if LEVEL[level] >= LEVEL[logger.level] then
		return logger:msg(level, getCurrentCodeInf(5), ...)
	end
end
--------------------------------------------------------------------------------
local function logNew(_, append)
	--[[
	Logger的创建函数
	--]]
	local logger = {
		level = DEBUG,
		hooks = { print, },
		
		setLevel = logSetLevel,
		msg = logOutput,
		addHook = logAddHook,
		removeHook = logRemoveHook,
	}

	logger.isDebug = function(logger) return LEVEL[DEBUG] >= LEVEL[logger.level] end
	logger.isInfo = function(logger) return LEVEL[INFO] >= LEVEL[logger.level] end
	logger.isWarn = function(logger) return LEVEL[WARN] >= LEVEL[logger.level] end
	logger.isError = function(logger) return LEVEL[ERROR] >= LEVEL[logger.level] end
	logger.isFatal = function(logger) return LEVEL[FATAL] >= LEVEL[logger.level] end

	logger.debug = function(logger, ...) return logMsg(logger, DEBUG, ...) end
	logger.info = function(logger, ...) return logMsg(logger, INFO, ...) end
	logger.warn = function(logger, ...) return logMsg(logger, WARN, ...) end
	logger.error = function(logger, ...) return logMsg(logger, ERROR, ...) end
	logger.fatal = function(logger, ...) return logMsg(logger, FATAL, ...) end

	return logger
end
--------------------------------------------------------------------------------
local Logger = define({ __call = logNew }, {
	OFF   = OFF,
	ALL   = ALL,
	DEBUG = DEBUG,
	INFO  = INFO,
	WARN  = WARN,
	ERROR = ERROR,
	FATAL = FATAL,
})
--------------------------------------------------------------------------------

logFileHandle = nil

function log2File(...)
	--[[
	输出日志到文件
	--]]
	if logFileHandle then
		local str = string.printf(...)
		logFileHandle:write(str)
		logFileHandle:write("\n")
		logFileHandle:flush()
	end
end

--------------------------------------------------------------------------------
logger = Logger()
logger:addHook(log2File)
