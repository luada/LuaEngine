--[[Writen by Luada

��־���ģ��

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
	������Ϣ�����
	--]]
	logger:debug(...)
end

function INFO_MSG(...)
	--[[
	���ٳ������н���
	--]]
	logger:info(...)
end

function WARN_MSG(...)
	--[[
	��������ʱ�����쳣
	--]]
	logger:warn(...)
end

function ERROR_MSG(...)
	--[[
	��������ʱ������Ԥ�ϵĴ���,��ʱͨ��������,�����ó���ָ���������
	--]]
	logger:error(...)
end

function FATAL_MSG(...)
	--[[
	��������ʱ��������Ԥ�ϵ����ش���,һ�㽫��ֹ��������
	--]]
	logger:fatal(...)
end

--------------------------------------------------------------------------------
local MAX_STRING_LENGTH = 512
local _print = print

function output(str)
	--[[
	���ַ�������󳤶Ȳ�����
	@param	str: Ҫ������ַ���
	@type	str��string
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
	�ض���print������������ֲ����������ʽ��ƥ������
	--]]
	local str = string.printf(...)
	output(str)
end
--------------------------------------------------------------------------------

-- �ر�������Ϣ���
local OFF = "OFF"

-- �������еȼ���Ϣ���
local ALL = "ALL"

-- ���ڵ�����Ϣ�����
local DEBUG = "DEBUG"

-- ���ڸ��ٳ������н���
local INFO = "INFO"

-- ��������ʱ�����쳣
local WARN = "WARN"

-- ��������ʱ������Ԥ�ϵĴ���,��ʱͨ��������,�����ó���ָ���������
local ERROR = "ERROR"

-- ��������ʱ��������Ԥ�ϵ����ش���,һ�㽫��ֹ��������
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
	������־����ĵȼ�
	@param	logger��Logger������ʵ��
	@type	logger��Object
	@param	level����־����ȼ�
	@type	level��int
	--]]
	assert(LEVEL[level], string.format("undefined level `%s'", tostring(level)))
	logger.level = level
end
--------------------------------------------------------------------------------
local function logOutput(logger, level, codeInfo, ...)
	--[[
	��־���������
	@param	logger��Logger������ʵ��
	@type	logger��Object
	@param	level����־����ȼ�
	@type	level��string
	@param	codeInfo��������־������XX_MSG)���ڵ��ļ������������к�
	@type	codeInfo��string
	@param	arg��XX_MSG�ĺ�������
	@type	arg�������б�
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
	�����־����ҽӺ���
	@param	logger: Logger������ʵ��
	@type	logger: Object of Logger
	@param	hook: ����ҽӺ���
	@type	hook: functor
	--]]
	table.insert(logger.hooks, hook)
end
--------------------------------------------------------------------------------
local function logRemoveHook(logger, hook)
	--[[
	ɾ����־����ҽӺ���
	@param	logger: Logger������ʵ��
	@type	logger: Object of Logger
	@param	hook: ����ҽӺ���
	@type	hook: functor
	--]]
	table.rmArrayValue(logger.hooks, hook)
end
--------------------------------------------------------------------------------
local function logMsg(logger, level, ...)
	--[[
	������־��Ϣ
	@param	logger��Logger������ʵ��
	@type	logger��Object
	@param	level����־����ȼ�
	@type	level��int
	--]]
	---assert(LEVEL[level], string.format("undefined level `%s'", tostring(level)))
	if LEVEL[level] >= LEVEL[logger.level] then
		return logger:msg(level, getCurrentCodeInf(5), ...)
	end
end
--------------------------------------------------------------------------------
local function logNew(_, append)
	--[[
	Logger�Ĵ�������
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
	�����־���ļ�
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
