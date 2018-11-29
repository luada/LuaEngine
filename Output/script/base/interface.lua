--[[Writen by Luada

提供接口机制，仅支持单重继承

Exported API:
	interface(super, ...)
	implemented(class, ...)
	isInterface(interface)
	isInterfaceInstance(object)
	superinterface(class)
	subinterfaceof(interface, super)

Example:
	IExecute = interface(nil, "execute")
	IListener = interface(IExecute, "getTarget")

	ListenerImpl = class(IListener)
	
	function ListenerImpl:execute()	
	end
	
	function ListenerImpl:getTarget()	
	end
	
	impl = ListenerImpl()
	IExecute ie = IExecute(impl)
	ie:execute()
	
--]]

require "base.base"
require "base.string"

--------------------------------------------------------------------------------
local interfaceMap = define { __mode = "k" }
local instanceMT = {}
--------------------------------------------------------------------------------
local function dummy()
	--[[
	未被实现接口的默认调用函数
	--]]
	error("Can not execute an interface method.")
end
--------------------------------------------------------------------------------
local function undefined(interface, method)
	--[[
	在接口类interface中解除接口method的定义
	@param	interface:
	@type	interface: interface
	@param	method:
	@type	method: string
	@param	return:
	@type	return: boolean(true or false)
	--]]
	if interface then
		return interface[method] == nil
	end
	return true
end
--------------------------------------------------------------------------------
local function iterator(interface)
	--[[
	返回一个可以访问接口类（interface)的所有接口（包括基类的接口）的迭代子
	@param	interface: 接口类
	@type	interface：Interface
	@param	return：迭代元素由接口索引和接口名组成
	@type	return：iterator(int, string)
	--]]
	local methods = {}
	local index = 0

	while interface do
		for f, v in pairs(interface) do
			methods[#methods + 1] = f
		end
		interface = superinterface(interface)
	end

	function iter()
		local f, v
		if index < #methods then
			index = index + 1
			f, v = index, methods[index]
		end
		return f, v
	end

	return iter
end
--------------------------------------------------------------------------------
local function cast(interface, object)
	--[[
	将object转为接口类interface方式访问的接口类实例
	@param	interface:
	@type	interface: Interface
	@param	object: 继承于interface类产生的实例
	@type	object：object of Class
	@param	return：
	@type	return：object of Interface
	--]]
	if not instanceof(object, interface) then
		return nil
	end

	local class = classof(object)
	local ret, _ = implemented(class, interface)
	if ret then
		local instance = define(instanceMT)
		instance.__interface = interface
		for _, m in iterator(interface) do
			instance[m] = function(self, ...)
				return class[m](object, ...)
			end
		end
		return instance
	end

	return nil
end
--------------------------------------------------------------------------------
local function interfaceToString(interface)
	--[[
	将接口类interface的接口名字符串化
	@param	interface：
	@type	interface：Interface
	@param	return：
	@type	return：string
	--]]
	local members = {}
	local separator = ""
	local ret = StringBuffer("<<interface>> = {")
	while interface do
		for f,_ in pairs(interface) do
			if not members[f] then
				ret = ret .. separator .. string.format("%q", f)
				separator = ", "
			end
			members[f] = true
		end
		interface = superinterface(interface)
	end
	ret = ret .. "}"
	return tostring(ret)
end
--------------------------------------------------------------------------------
function interface(super, ...)
	--[[
	定义接口类；仅支持单重继承
	@param	super: 接口基类；为nil时，表示此接口类没有基类
	@type	super: Interface or nil
	@param	arg: 接口名组成的参数
	@type	arg：string，string，...
	@param	return：继承于super且定义arg接口的接口子类
	@type	return：SubInterface
	--]]
	local methods = {}
	for i = 1, select("#", ...) do
		local method = select(i, ...)
		if type(method) == "string" and undefined(super, method) then
			methods[method] = dummy
		end
	end

	local interface = define({
		__call = cast,
		__index = super,
		__tostring = interfaceToString
	}, methods)

	interfaceMap[interface] = true

	return interface
end
--------------------------------------------------------------------------------
function implemented(class, ...)
	--[[
	检查class继承接口类（arg）的所有接口是否都被实现了
	@param	class:
	@type	class: Class
	@param	arg: 
	@type	arg: interface, interface, ...
	@param	return: 所有接口类的接口都在class实现时返回true；否则为false
	@type	return：boolean（true or false）
	--]]
	class = isclass(class) and class or classof(class)
	if class then
		if class.__interfaces then
			for i = 1, select("#", ...) do
				local interface = select(i, ...)
				if class.__interfaces[interface] ~= true then
					for _, m in iterator(interface) do
						if type(interface[m]) == "function" and not class[m] then
							return false, m
						end
					end
					class.__interfaces[interface] = true
				end
			end
			return true
		end
	end
	return false
end
--------------------------------------------------------------------------------
function isInterface(interface)
	--[[
	测试interface是否为接口类
	@param	interface：
	@type	Interface：any
	@param	return：interface为接口类时返回true；否则返回false
	@type	return：boolean（true or false）
	--]]
	return interfaceMap[interface] == true
end
--------------------------------------------------------------------------------
function isInterfaceInstance(object)
	--[[
	测试object是否为接口类实例
	@param	object：
	@type	object：object of Interface
	@param	return：object为接口类实例时返回true；否则返回false
	@type	return：boolean（true or false）
	--]]
	return getmetatable(object) == instanceMT
end
--------------------------------------------------------------------------------
function superinterface(interface)
	--[[
	获取被接口子类继承的接口基类
	@param	interface:
	@type	interface: Interface
	@param	return:
	@type	return: Interface or nil
	--]]
	local mt = getmetatable(interface)
	return mt and mt.__index or nil
end
--------------------------------------------------------------------------------
function subinterfaceof(interface, super)
	--[[
	测试接口子类interface是否继承于接口基类super
	@param	interface：
	@type	interface：Interface
	@param	super：
	@type	super：Interface
	@param	return：interface继承于super时返回true；否则返回false
	@type	return: boolean(true or false)
	--]]
	while interface do
		if interface == super then return true end
		interface = superinterface(interface)
	end
	return false
end
