--[[Writen by Luada

用于捕获异常,安全运行代码,提供类似try..catch..finally机制.

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
	出错处理辅助函数
	@param	ex：异常实例
	@type	ex：Exception或其子类产生的实例
	--]]
	if tryFlag == 0 then	---没有try时处理的异常
		ex:printStackTrace()
		rawThrow(nil, ex:tostring())
	else
		assertRet = ex
		rawThrow(nil, "")
	end
end

function error(ex)
	--[[
	出错处理函数
	@param	ex：异常信息
	@type	ex：any
	--]]
	if not instanceof(ex, Exception) then
		rawError(Exception(ex))
	end
	rawError(ex)
end

throw = error

function assert(condition, msg)
	--[[
	重定义assert函数，用于抛出AssertException
	@param	condition：assert条件；条件为false时，会抛出AssertException
	@type	condition：boolean
	@param	msg：出现异常时的信息
	@type	msg：string
	--]]
	if not condition then
		ERROR_MSG(string.format("Error Message: %s\n%s", tostring(msg), debug.traceback()))
		throw(AssertException(msg))
	end
end

function try(statement)
	--[[
	捕获异常,安全运行代码,提供类似try..catch..finally
	@param	statement：语法片段，格式见此文件的开始Example处
	@type	statement：smt
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
	安全调用method方法，函数内置try...catch机制
	@param	method：安全调用的方法
	@type	method：function
	@param	arg：method的参数
	@type	arg：参数列表
	@param	return: 运行结果
	@type	return：（status, result）
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
