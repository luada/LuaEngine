--[[Writen by Luada

�ṩ�ӿڻ��ƣ���֧�ֵ��ؼ̳�

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
	δ��ʵ�ֽӿڵ�Ĭ�ϵ��ú���
	--]]
	error("Can not execute an interface method.")
end
--------------------------------------------------------------------------------
local function undefined(interface, method)
	--[[
	�ڽӿ���interface�н���ӿ�method�Ķ���
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
	����һ�����Է��ʽӿ��ࣨinterface)�����нӿڣ���������Ľӿڣ��ĵ�����
	@param	interface: �ӿ���
	@type	interface��Interface
	@param	return������Ԫ���ɽӿ������ͽӿ������
	@type	return��iterator(int, string)
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
	��objectתΪ�ӿ���interface��ʽ���ʵĽӿ���ʵ��
	@param	interface:
	@type	interface: Interface
	@param	object: �̳���interface�������ʵ��
	@type	object��object of Class
	@param	return��
	@type	return��object of Interface
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
	���ӿ���interface�Ľӿ����ַ�����
	@param	interface��
	@type	interface��Interface
	@param	return��
	@type	return��string
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
	����ӿ��ࣻ��֧�ֵ��ؼ̳�
	@param	super: �ӿڻ��ࣻΪnilʱ����ʾ�˽ӿ���û�л���
	@type	super: Interface or nil
	@param	arg: �ӿ�����ɵĲ���
	@type	arg��string��string��...
	@param	return���̳���super�Ҷ���arg�ӿڵĽӿ�����
	@type	return��SubInterface
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
	���class�̳нӿ��ࣨarg�������нӿ��Ƿ񶼱�ʵ����
	@param	class:
	@type	class: Class
	@param	arg: 
	@type	arg: interface, interface, ...
	@param	return: ���нӿ���Ľӿڶ���classʵ��ʱ����true������Ϊfalse
	@type	return��boolean��true or false��
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
	����interface�Ƿ�Ϊ�ӿ���
	@param	interface��
	@type	Interface��any
	@param	return��interfaceΪ�ӿ���ʱ����true�����򷵻�false
	@type	return��boolean��true or false��
	--]]
	return interfaceMap[interface] == true
end
--------------------------------------------------------------------------------
function isInterfaceInstance(object)
	--[[
	����object�Ƿ�Ϊ�ӿ���ʵ��
	@param	object��
	@type	object��object of Interface
	@param	return��objectΪ�ӿ���ʵ��ʱ����true�����򷵻�false
	@type	return��boolean��true or false��
	--]]
	return getmetatable(object) == instanceMT
end
--------------------------------------------------------------------------------
function superinterface(interface)
	--[[
	��ȡ���ӿ�����̳еĽӿڻ���
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
	���Խӿ�����interface�Ƿ�̳��ڽӿڻ���super
	@param	interface��
	@type	interface��Interface
	@param	super��
	@type	super��Interface
	@param	return��interface�̳���superʱ����true�����򷵻�false
	@type	return: boolean(true or false)
	--]]
	while interface do
		if interface == super then return true end
		interface = superinterface(interface)
	end
	return false
end
