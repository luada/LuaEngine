--[[Writen by Luada

�ṩ��ļ̳л��ƣ�֧�ֶ��ؼ̳С�

Exported API:
	class(super, ...)
	release(object)
	classof(object)
	className(object)
	islass(class)
	delegate(object, class)
	instanceof(object, class)
	superclass(class)
	subclassof(class, super)
	subclassofInterface(class, interface)
	getInterfaces(class)
	checkImplemented(class)
	isClassInstance(object)

Example:
	local Object = class()

	function Object:__init(id)
		print("object:__init", id)
		self.id = id
	end

	function Object:__release()
		print("object:__release")
	end

	function Object:getId()
		return self.id
	end

	obj = Object(123)
	obj:release(obj)
	obj = nil
--]]

require "base.try"
require "base.base"
require "base.interface"
require "base.singleton"
require "base.serialize"
require "base.property"
require "base.prototype"

--------------------------------------------------------------------------------
local advance = true
local classMap = define { __mode = "k" }
--------------------------------------------------------------------------------
local function rawnew(class, object, ...)
	--[[
	��������class�����Ķ���object�ĳ�ʼ������__init
	@param	class: ��
	@type	class��class
	@param	object����class�����Ķ���
	@type	object��object
	@param	arg����ʼ������__init�Ĳ����б�
	@type	arg�������б�
	@param	return����ʼ�����object
	@type	return��object
	--]]
	
	--[[--------------------------------------------------------------------------
	--Begin����1
		--����˳�򣺻���-������
	---��1�����û���ĳ�ʼ������
	for i = 1, #class.__supers do
		local super = class.__supers[i]
		if isclass(super) then
			rawnew(super, object, ...)
		end
	end
	---(2) ��������ĳ�ʼ������
	if type(class.__init) == "function" then
		class.__init(object, ...)
	end
	return object
	--End����1
	--]]--------------------------------------------------------------------------
	
	---Begin����2
		---�������������°汾�ĳ�ʼ������__init�����û��Լ���������ĳ�ʼ�������ĵ���˳��Ͳ�������
	if type(class.__init) == "function" then
		class.__init(object, ...)
	end
	return object
	---End����2
	
end
--------------------------------------------------------------------------------
local function new(class, ...)
	--[[
	��class�����Ķ���ʱ���õĺ���
	@param	class: ��
	@type	class��class
	@param	arg����ʼ������__init�Ĳ����б�
	@type	arg�������б�
	@param	return����class�������¶���
	@type	return��object
	--]]
	---(0) ���ٵ���ʵ���ļ���
	local inst = rawget(class, "__instance")
	if inst ~= nil then
		return inst	
	end
	
	---��1�����class���еĽӿ��Ƿ񶼱�ʵ����
	if not class.__implemented then
		advance = false
		local ret, method = checkImplemented(class)
		advance = true
		if not ret then
				error(string.format("The %q is not implemented!", method))
			end
		class.__implemented = true
	end
	
	---��2�����class�Ƿ������ڵ���Singleton�ࡣ��������ڴ���ֻ��һ�ݲ����Ķ��󣻷������������¶���
	if inheritfrom(class, Singleton) then
		local object = define(class.__vtbl)
		inst = rawnew(class, object, ...)
		rawset(class, "__instance", inst)
		return inst
	else
		local object = define(class.__vtbl)
		return rawnew(class, object, ...)
	end
end
--------------------------------------------------------------------------------
local function rawrelease(class, object)
	--[[
	��������class�����Ķ���object���ͷź���__release������˳������-������
	@param	class: ��
	@type	class��class
	@param	object����class�����Ķ���
	@type	object��object
	--]]
	---��1������������ͷź���
	if type(class.__release) == "function" then
		class.__release(object)
	end
	---��2�����û�����ͷź���
	for i = 1, #class.__supers do
		local super = class.__supers[i]
		if isclass(super) then
			rawrelease(super, object)
		end
	end
end
--------------------------------------------------------------------------------
function release(object)
	--[[
	�ͷ�object�����ô˺����󣬻��ý���������object�ı�����Ϊnil
	@param	object��
	@type	object��object
	--]]
	if isInterfaceInstance(object) then
		object:release()
		return
	end
	
	local class = classof(object)
	if class then
		rawrelease(class, object)
		if inheritfrom(class, Singleton) then
			rawset(class, "__instance", nil)
		end
	end
end
--------------------------------------------------------------------------------
local function supersToStr(supers, members)
	--[[
	��class�ַ������ĸ�������
	@param	supers���ɻ�����ɵı�
	@type	supers��table
	@param	members����־class��__vtbl���Ƿ񱻷��ʹ�
	@type	members��table
	@param	return: class�ַ�����
	@type	return��string
	--]]
	local ret = StringBuffer("")
	local separator = ""
	for i = 1, #supers do
		local super = supers[i]
		local vtbl = super.__vtbl
		for f,v in pairs(vtbl) do
			if not members[f] and not string.startsWith(f, "_") then
				if type(v) == "string" then
					ret = ret .. separator .. f .. " = " .. string.format("%q", v)
				else
					ret = ret .. separator .. f .. " = " .. tostring(v)
				end
				separator = ", "
			end
			members[f] = true
		end
		local superRet = supersToStr(super.__supers, members)
		ret = ret .. separator
		ret = ret .. superRet
	end
	return ret
end
--------------------------------------------------------------------------------
local classMT = {
	__call = new,
	__index = function(class, field)
		local value = class.__vtbl[field]
		if advance and value and not string.startsWith(field, "_") then
			rawset(class, field, value)
		end
		return value
	end,
	__newindex = function(class, field, value)
		class.__vtbl[field] = value
	end,
	__tostring = function(class)
		local members = {}
		local supers = {class}
		local ret = StringBuffer("<<class>> = {") .. supersToStr(supers, members) .. "}"
		return tostring(ret)
	end
}
--------------------------------------------------------------------------------
local function objectToStr(object)
	--[[
	��object�ַ�����
	@param	object: 
	@type	object: object
	@param	return: object�ַ�����
	@type	return��string
	--]]
	if type(object.tostring) == "function" then
		return object:tostring()
	end

	local separator = ""
	local ret = StringBuffer("<<object>> = {")
	for f,v in pairs(object) do
		if not string.startsWith(f, "_") then
			if type(v) == "string" then
				ret = ret .. separator .. f .. " = " .. string.format("%q", v)
			else
				ret = ret .. separator .. f .. " = " .. tostring(v)
			end
			separator = ", "
		end
	end
	ret = ret .. "}"
	return tostring(ret)
end
--------------------------------------------------------------------------------
local function searchSupersMethod(k, supers)
	--[[
	�ڻ���supers�в��Ҽ�Ϊk��ֵ
	@param	k: ��ֵ
	@type	k: string or number
	@param	supers: �ɻ�����ɵı�
	@type	supers: table
	--]]
	for i=1, #supers do
		local v = supers[i][k]    -- try 'i'-th superclass
		if v then
			return v
		end
	end
end
--------------------------------------------------------------------------------
function delegate(object, class)
	--[[
	��Object����ҽ�һ��Class��ί�У�ʵ�ֵĹ��ܣ�objectû���ҵ�����ʱ��������Class����
	@param	object�����ҽӵ�ʵ��
	@type	object��object of Class
	@param	class����
	@type	class��Class
	--]]
    local mt = getmetatable(object)
    setmetatable(object, {
        __index = function(t, k)
			local v = mt[k]
			return v or class[k]
        end
    })
end
--------------------------------------------------------------------------------
function class(...)
	--[[
	��Ķ��庯�� SubClass = class(Superclass / Interfaces)
	@param	arg: Ҫ���̳еĻ���(�ӿ�)
	@type	arg: ��class,interface��ɵĲ����б�
	@param	return: �̳й�ϵ����������
	@type	return: class
	--]]
	---(1)����superclass��interface
	local interfaces = {}
	local supers = {}
	for i = 1, select("#", ...) do
		local curArg = select(i, ...)
		if isclass(curArg) then
			supers[#supers + 1] = curArg
		elseif isInterface(curArg) then
			interfaces[#interfaces + 1] = curArg
		else
			error(string.format("Wrong type @ param:%s !Param must be an interface or a class!", i))
		end
	end

	local vtbl = {
		__tostring = objectToStr,
		release = release,
	}

	vtbl.__index = vtbl

	local class = define(classMT, {
		__supers = supers,
		__interfaces = interfaces,
		__vtbl = vtbl,
		__init = false,
		__release = false,
	})

	classMap[vtbl] = class

	if #class.__supers > 0 then
		define({
		--[[
		����һ�������л���û����չʵ�ֵĻ���ķ��������ڼ�������
		---	__index = function (t, k) local v = searchSupersMethod(k, class.__supers) t[k] = v return v end
		��������������û�еķ����ڻ����м�ʱ���ң����ڽ�ʡ�ڴ�
		---	__index = function (t, k) local v = searchSupersMethod(k, class.__supers) return v end
		--]]
		--- PVE���õ�һ�ַ���
			__index = function (t, k) local v = searchSupersMethod(k, class.__supers) t[k] = v return v end
		}, vtbl)
	end

	return class
end
--------------------------------------------------------------------------------
function isclass(class)
	--[[
	�ж�class�Ƿ�Ϊ��
	@param	class��
	@type	class: class
	@param	return: �жϽ��
	@type	return��boolean��true or false��
	--]]
	local mt = getmetatable(class)
	if mt == classMT then return true end
	if classMap[mt] then return true end
	return false
end
--------------------------------------------------------------------------------
function classof(object)
	--[[
	��ȡ����object����
	@param	object��
	@type	object��object
	@param	return������object����
	@type	return��class
	--]]
	local mt = getmetatable(object)
	return classMap[mt]
end
--------------------------------------------------------------------------------
function className(object)
	--[[
	��ȫ��������ʵ��object��������
	@param	object��
	@type	object��object
	@param	return����ʵ�ɹ�ʱ�����������֣����򷵻�""
	@type	return��string
	--]]
	return table.getKeyName(_G, object.__interface or classof(object))
end
--------------------------------------------------------------------------------
function isClassInstance(object)
	--[[
	�ж�object�Ƿ�Ϊ�������ʵ��
	@param	return��
	@type	return��boolean��true or false��
	--]]
	return classof(object) ~= nil
end
--------------------------------------------------------------------------------
function getInterfaces(class)
	--[[
	��ȡ��class�̳еĽӿ�
	@param	class:
	@type	class:
	@param	return: ��class�̳еĽӿ�
	@type	return��interface
	--]]
	return rawget(class, "__interfaces")
end
--------------------------------------------------------------------------------
function superclass(class)
	--[[
	��ȡ��class�̳еĻ���
	@param	class:
	@type	class:
	@param	return: ��class�̳еĻ���
	@type	return��class
	--]]
	return rawget(class, "__supers")
end
--------------------------------------------------------------------------------
function subclassof(class, super)
	--[[
	�ж�class�Ƿ�̳�super
	@param	class������
	@type	class��class
	@param	super������
	@type	super��class
	@param	return��class�̳�super���ߺ�superΪͬһ��ʱ������true������Ϊfalse
	@type	return��boolean(true or false)
	--]]
	if class == super then
		return true
	end

	for _, s in pairs(superclass(class)) do
		if subclassof(s, super) then
			return true
		end
	end

	return false
end
--------------------------------------------------------------------------------
function subclassofInterface(class, interface)
	--[[
	���class�Ƿ�̳нӿ�interface
	@param	class������
	@type	class��class
	@param	interface��Ҫ�����ԵĽӿ�
	@type	interface��interface
	@param	return��class�̳�interfacesʱ����true������Ϊfalse
	@type	return: boolean(true or false)
	--]]
	local interfaces = getInterfaces(class)
	for i = 1, #interfaces do
		if subinterfaceof(interfaces[i], interface) then
			return true
		end
	end

	for _, s in pairs(superclass(class)) do
		if subclassofInterface(s, interface) then
			return true
		end
	end

	return false
end
--------------------------------------------------------------------------------
function checkImplemented(class, super)
	--[[
	�����class���еĽӿ��Ƿ񶼱�ʵ����
	@param	class:
	@type	class: class
	@param	super: ��class�̳еĻ��ࣻ�״ε��ã�����super����Ϊnil
	@type	super��class
	@param	return��class���еĽӿڶ���ʵ���ˣ��򷵻أ�true��nil�����򷵻أ�false�� δʵ�ֵĽӿ����֣�
	@type	return����boolean��string or nil��
	--]]
	local ret, method = implemented(class, unpack(getInterfaces(super and super or class)))
	if not ret then
		return ret, method
	end
	
	for _, s in pairs(superclass(super and super or class)) do
		ret, method = checkImplemented(class, s)
		if not ret then
			return ret, method
		end
	end
	
	return true
end
--------------------------------------------------------------------------------
function instanceof(object, class)
	--[[
	�ж϶���object�Ƿ�Ϊclass����������ʵ��
	@param	object�������Զ��󣬻���������
	@type	object��object
	@param	class��
	@type	class��class
	@param	return�����Խ��
	@type	return��boolean��true or false)
	--]]
	if object then
		if isInterfaceInstance(object) then
			return subinterfaceof(object.__interface, class)
		elseif isClassInstance(object) then
			if isclass(class) then
				return subclassof(classof(object), class)
			elseif isInterface(class) then
				return subclassofInterface(classof(object), class)
			end
		end
	end
	return false
end
--------------------------------------------------------------------------------
function inheritfrom(sub, super)
	--[[
	�ж�sub�Ƿ�̳���super
	@param	sub: ����
	@type	sub: class or interface
	@param	super: ����
	@type	super: class or interface
	@param	return:���Խ��
	@type	return:boolean��true or false)
	--]]
	if sub == nil or super == nil then
		return false
	end
	
	if isclass(sub) then
		if isclass(super) then
			return subclassof(sub, super)
		elseif isInterface(super) then
			return subclassofInterface(sub, super)
		end
	elseif isInterface(sub) then
		if isInterface(super) then
			return subinterfaceof(sub, super)
		end
	end
	
	return false
end
--------------------------------------------------------------------------------
local _toString = toString
function toString(value)
	--[[
	��value�ַ�����
	@param	value��
	@type	value��any
	@param	return: value�ַ�����
	@type	return��string
	--]]
	local sb = StringBuffer()
	if isclass(value) or isInterface(value) or classof(value) then
		sb:append(tostring(value))
	else
		sb:append(_toString(value)) ---Proper Tail Call
	end
	return tostring(sb)
end

--------------------------------------------------------------------------------

require "base.exception"
require "base.logger"

--------------------------------------------------------------------------------
