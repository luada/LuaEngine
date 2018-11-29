--[[Writen by Luada

提供仿函数设计模式的支持

Exported API:
	functor(fn, ...)

Example:
	local function callback(id, msg)
		print(id, msg)
	end

	local f = functor(callback, 1)
	f("Hello Functor!") 
--]]

require "base.base"

function functor(fn, ...)
	--[[
	创建任意参数的Callback
	@param	fn: Callback函数
	@type	fn: function
	@param	arg: 回调函数的参数（也可以在回调时传入）
	@type	arg: 参数列表
	@param	return：Callback
	@type	return：functor
	--]]
	return	define(
			{ __call = function(self, ...) return self._fn(unpackArgs(self._args, table.pack(...))) end,
			 release = function(self) self._fn = nil self._args = nil end }, 
			{ _fn = fn, _args = table.pack(...)})
end
