--[[Writen by Luada

���ڲ����쳣,��ȫ���д���,�ṩ����try..catch..finally����.

Exported API:
	throw(exception)
	error(exception)
	try(statement)
	safeCall(method, ...)

Example:
	print("begin")
	try{
		function()
			print("try outer")

			try{
				function()
					print("try inner")
					assert(false, "Haha Error!")
					print("no execute inner")
				end
			,catch = {
				{AssertException,
					function(ex)
						print(ex, ex:getMessage())
						throw(Exception("New Error", ex))
					end
				},
			}
			,finally = function()
				print("finally inner")
			end}

			print("no execute outer")
		end
	,catch = {
		{Exception,
			function(ex)
				print(ex, ex:getMessage())
			end
		},
	}
	,finally = function()
		print("finally outer")
	end}

	print("end")
--]]


local tryFlag = 0
local rawThrow = assert
local assertRet = nil

local function rawError(ex)
	--[[
	������������
	@param	ex���쳣ʵ��
	@type	ex��Exception�������������ʵ��
	--]]
	if tryFlag == 0 then	---û��tryʱ������쳣
		ex:printStackTrace()
		rawThrow(nil, ex:tostring())
	else
		assertRet = ex
		rawThrow(nil, "")
	end
end

function error(ex)
	--[[
	��������
	@param	ex���쳣��Ϣ
	@type	ex��any
	--]]
	if not instanceof(ex, Exception) then
		rawError(Exception(ex))
	end
	rawError(ex)
end

throw = error

function assert(condition, msg)
	--[[
	�ض���assert�����������׳�AssertException
	@param	condition��assert����������Ϊfalseʱ�����׳�AssertException
	@type	condition��boolean
	@param	msg�������쳣ʱ����Ϣ
	@type	msg��string
	--]]
	if not condition then
		ERROR_MSG(string.format("Error Message: %s\n%s", tostring(msg), debug.traceback()))
		throw(AssertException(msg))
	end
end

function try(statement)
	--[[
	�����쳣,��ȫ���д���,�ṩ����try..catch..finally
	@param	statement���﷨Ƭ�Σ���ʽ�����ļ��Ŀ�ʼExample��
	@type	statement��smt
	--]]
	assertRet = nil
	tryFlag = tryFlag + 1
	local status, result = pcall(statement[1])
	tryFlag = tryFlag - 1
	if status then
		return result
	end
	local catched = false
	local ex = assertRet or RuntimeException(result)
	assertRet  = nil
	for i, item in ipairs(statement.catch) do
		if instanceof(ex, item[1]) then
			result = item[2](ex)
			catched = true
			break
		end
	end
	if type(statement.finally) == "function" then
		result = statement.finally() or result
	end
	if not catched then
		error(result)
	end
	return result
end

function safeCall(method, ...)
	--[[
	��ȫ����method��������������try...catch����
	@param	method����ȫ���õķ���
	@type	method��function
	@param	arg��method�Ĳ���
	@type	arg�������б�
	@param	return: ���н��
	@type	return����status, result��
	--]]
	local status, result = false
	local arg = table.pack(...)
	try {
		function()
			if type(method)=="function" then
				result = method(unpack(arg))
				status = true
			else
				ERROR_MSG(getCurrentCodeInf(5), "--->", method, unpack(arg))
			end
		end
	, catch = {
		{Exception,
			function(ex)
				WARN_MSG(getCurrentCodeInf(5), "--->", ex:getMessage())
			end
		}
	}}
	return status, result
end
