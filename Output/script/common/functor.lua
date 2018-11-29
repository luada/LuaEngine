--[[Writen by Luada

�ṩ�º������ģʽ��֧��

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
	�������������Callback
	@param	fn: Callback����
	@type	fn: function
	@param	arg: �ص������Ĳ�����Ҳ�����ڻص�ʱ���룩
	@type	arg: �����б�
	@param	return��Callback
	@type	return��functor
	--]]
	return	define(
			{ __call = function(self, ...) return self._fn(unpackArgs(self._args, table.pack(...))) end,
			 release = function(self) self._fn = nil self._args = nil end }, 
			{ _fn = fn, _args = table.pack(...)})
end
